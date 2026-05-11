/*
 * 파일명: sns_himax_dsp_sensor_instance.c
 * 목적 및 기능: ADSP 환경에서 Himax DSP와 SPI(Instance 5) 통신을 수행하며,
 * 공유 메모리에 이미지를 적재한 후 Android로 Proximity 센서 이벤트를 발생시킵니다.
 * change(add)-hyungchul-20260429-1405: 메모리 복사 완료 후 이벤트 Drop 방지를 위한 토글(Toggle) 로직 및 SPI 삽입부 명시화
 */

/*==============================================================================
  Include Files
  ============================================================================*/
#include <qurt.h>
#include <stdio.h>
#include <string.h>                            /* memcpy를 사용하기 위한 헤더입니다. */
#include "sns_mem_util.h"
#include "sns_sensor_instance.h"
#include "sns_service_manager.h"
#include "sns_stream_service.h"
#include "sns_rc.h"
#include "sns_request.h"
#include "sns_time.h"
#include "sns_sensor_event.h"
#include "sns_types.h"
#include "sns_event_service.h"
#include "sns_memmgr.h"
#include "sns_com_port_priv.h"

#include "sns_himax_dsp_sensor.h"
#include "sns_himax_dsp_sensor_instance.h"

#include "sns_interrupt.pb.h"
#include "sns_async_com_port.pb.h"
#include "sns_proximity.pb.h"                  /* change(add)-hyungchul-20260429-1620: proximity 전용 event payload를 사용합니다. */
#include "sns_std_sensor.pb.h"                 /* sample status enum을 사용하기 위한 헤더입니다. */

#include "pb_encode.h"
#include "pb_decode.h"
#include "sns_pb_util.h"
#include "sns_sync_com_port_service.h"
#include "sns_island.h"

/*==============================================================================
  Local Macros
  ============================================================================*/
/* 두 값 중 작은 값을 선택합니다. */
#define HIMAX_DSP_MIN(a, b) (((a) < (b)) ? (a) : (b))

/*==============================================================================
  Local Global Variables
  ============================================================================*/
/* 공유 메모리 물리 주소에 매핑된 QuRT 가상 주소입니다. */
static unsigned int myddr_base_addr = 0;

/* PSRAM 가상 주소 예비 변수입니다. 현재 로직에서는 사용하지 않습니다. */
static void *psram_virtual_addr = NULL;

/* smem_pool attach 후 저장되는 QuRT 메모리 풀 핸들입니다. */
static qurt_mem_pool_t hwio_pool = 0;

/* 공유 메모리 region 핸들입니다. */
static qurt_mem_region_t shared_mem_region = 0;

/* 공유 메모리 region 속성입니다. */
static qurt_mem_region_attr_t hwio_attr;

/* QuRT 공유 메모리 매핑 상태입니다. 4가 최종 성공입니다. */
static int qurt_init = 0;

/* 초기 interrupt 안정화를 위한 카운터입니다. */
static int int_count = 0;

/*==============================================================================
  Function Definitions
  ============================================================================*/
/*
 * 함수명: himax_com_read_wrapper
 * 목적 및 기능: SCP 서비스를 통해 SPI 버스에서 데이터를 읽어오는 편의 래퍼 함수입니다.
 * 입력 변수: 
 * - scp_service: 동기화 통신 포트 서비스 포인터
 * - port_handle: 통신 포트 핸들
 * - reg_addr: 읽기를 시작할 레지스터 주소 (여기서는 0x00 사용)
 * - bytes: 읽어올 바이트 수
 * 출력 변수: 
 * - buffer: 읽어온 데이터를 저장할 버퍼
 * - xfer_bytes: 실제 전송(읽기) 완료된 바이트 수
 * 리턴 값: sns_rc (통신 성공 여부, 성공 시 SNS_RC_SUCCESS)
 */
static sns_rc himax_com_read_wrapper(
        sns_sync_com_port_service *scp_service,
        sns_sync_com_port_handle  *port_handle,
        uint32_t                   reg_addr,
        uint8_t                   *buffer,
        uint32_t                   bytes,
        uint32_t                  *xfer_bytes)
{
    sns_port_vector port_vec; // 포트 벡터 구조체 선언
    port_vec.buffer = buffer; // 수신 버퍼 연결
    port_vec.bytes = bytes; // 수신할 크기 설정
    port_vec.is_write = false; // 읽기 동작이므로 false 설정
    port_vec.reg_addr = reg_addr; // 레지스터 주소 설정
                                  // SCP API를 호출하여 실제 레지스터(SPI) 읽기 수행
    return scp_service->api->sns_scp_register_rw(port_handle, &port_vec, 1, false, xfer_bytes);
}



/*
 * 목적 및 기능: 공유 메모리 복사 완료 후 Android Proximity 표준 경로로 Himax frame-ready trigger를 전달합니다.
 */
static void himax_dsp_publish_proximity_trigger_event(
        sns_sensor_instance *const instance,
        sns_time timestamp,
        bool is_near)
{
    himax_dsp_instance_state *state = (himax_dsp_instance_state*)instance->state->state;
    sns_proximity_event prox_event = sns_proximity_event_init_default;

    /* change(add)-hyungchul-20260429-1620: proximity proto의 required field를 모두 채웁니다. */
    prox_event.proximity_event_type = is_near ? SNS_PROXIMITY_EVENT_TYPE_NEAR : SNS_PROXIMITY_EVENT_TYPE_FAR;
    prox_event.raw_adc = is_near ? 0 : 5;
    prox_event.status = SNS_STD_SENSOR_SAMPLE_STATUS_ACCURACY_HIGH;

    SNS_INST_PRINTF(HIGH, instance,
                    "Himax DSP: Proximity trigger payload prepared. near=%d raw=%u",
                    is_near ? 1 : 0,
                    prox_event.raw_adc);

    /* change(add)-hyungchul-20260429-1741:
     * proximity payload는 sns_proximity_event이므로 표준 sensor float helper가 아니라
     * generic protobuf event helper(pb_send_event)를 사용합니다.
     */
    pb_send_event(instance,
                  sns_proximity_event_fields,
                  &prox_event,
                  timestamp,
                  SNS_PROXIMITY_MSGID_SNS_PROXIMITY_EVENT,
                  &state->my_suid);

    SNS_INST_PRINTF(HIGH, instance,
                    "Himax DSP: Proximity trigger event sent. near=%d raw=%u",
                    is_near ? 1 : 0,
                    prox_event.raw_adc);
}

/*
 * 함수명: himax_dsp_init_shared_memory
 * 목적 및 기능: 0x81EC0000 물리 공유 메모리를 QuRT 가상 주소로 매핑합니다.
 * 입력 변수:
 * - instance: 로그 출력을 위한 Himax DSP 센서 인스턴스 포인터입니다.
 * 출력 변수: 없음
 * 리턴 값:
 * - true: 공유 메모리 매핑 성공 또는 이미 성공
 * - false: 공유 메모리 매핑 실패 또는 초기 안정화 대기 중
 */
static bool himax_dsp_init_shared_memory(sns_sensor_instance *const instance)
{
    if(int_count < 30)
    {
        int_count = int_count + 1;
    }

    if(qurt_init == 4)
    {
        return true;
    }

    if(int_count <= 20)
    {
        SNS_INST_PRINTF(HIGH, instance, "qurt_init wait: %d, int_count: %d", qurt_init, int_count);
        return false;
    }

    if(qurt_init <= 3)
    {
        if(QURT_EOK == qurt_mem_pool_attach("smem_pool", &hwio_pool))
        {
            qurt_mem_region_attr_init(&hwio_attr);
            qurt_mem_region_attr_set_cache_mode(&hwio_attr, QURT_MEM_CACHE_NONE_SHARED);
            qurt_mem_region_attr_set_mapping(&hwio_attr, QURT_MEM_MAPPING_PHYS_CONTIGUOUS);
            qurt_mem_region_attr_set_physaddr(&hwio_attr, SHARED_PHYS_ADDR);
            qurt_init = 1;

            if(QURT_EOK != qurt_mem_region_create(&shared_mem_region, SHARED_SIZE, hwio_pool, &hwio_attr))
            {
                qurt_init = 2;
            }
            else if(QURT_EOK != qurt_mem_region_attr_get(shared_mem_region, &hwio_attr))
            {
                qurt_init = 3;
            }
            else
            {
                qurt_mem_region_attr_get_virtaddr(&hwio_attr, &myddr_base_addr);
                qurt_init = 4;
                SNS_INST_PRINTF(HIGH, instance, "Himax DSP: Shared memory mapped. virt=0x%X", myddr_base_addr);
            }
        }
    }

    if(qurt_init <= 3)
    {
        SNS_INST_PRINTF(HIGH, instance, "qurt_init: %d, int_count: %d", qurt_init, int_count);
        return false;
    }

    return true;
}

/*
 * 함수명: himax_dsp_handle_interrupt_event
 * 목적 및 기능: GPIO interrupt 발생 시 Himax DSP SPI JPEG 데이터를 읽고 공유 메모리 복사 완료 후 proximity trigger를 발행합니다.
 * 입력 변수:
 * - instance: Himax DSP 센서 인스턴스 포인터입니다.
 * - timestamp: interrupt timestamp입니다.
 * 출력 변수: 없음
 * 리턴 값: 없음
 */
static void himax_dsp_handle_interrupt_event(sns_sensor_instance *const instance, sns_time timestamp)
{
    himax_dsp_instance_state *state = (himax_dsp_instance_state*)instance->state->state;
    uint8_t *buffer = NULL;
    uint32_t xfer_bytes = 0;
    uint32_t total_read_bytes = 0;
    uint32_t header_len = 0;
    uint32_t jpeg_size = 0;
    uint32_t payload_read_bytes = 0;
    uint32_t total_frame_size = 0;
    uint8_t *shared_mem_ptr = NULL;
    bool drop_frame = false;
    bool frame_ready = false;
    bool shared_mem_ready = false;  /* change(add)-hyungchul-20260429-1646: 공유 메모리 미준비 시에도 SPI drain을 계속하기 위한 상태 플래그입니다. */
    static bool prox_toggle = false;

    SNS_INST_PRINTF(LOW, instance, "Himax DSP: GPIO interrupt triggered");

    /* SPI 통신과 DDR 접근이 필요하므로 island mode에서 빠져나옵니다. */
    sns_island_exit_internal();

    /* change(add)-hyungchul-20260429-1646 시작: 공유 메모리 매핑이 아직 준비되지 않아도 SPI는 읽어서 Himax IRQ source를 비웁니다. */
    shared_mem_ready = himax_dsp_init_shared_memory(instance);
    if(shared_mem_ready)
    {
        shared_mem_ptr = (uint8_t *)myddr_base_addr;
        if(NULL == shared_mem_ptr)
        {
            shared_mem_ready = false;
            SNS_INST_PRINTF(ERROR, instance, "Himax DSP: Shared memory virtual address is NULL. SPI drain only.");
        }
    }
    else
    {
        SNS_INST_PRINTF(HIGH, instance, "Himax DSP: Shared memory is not ready. SPI drain only.");
    }

    if(!shared_mem_ready)
    {
        drop_frame = true;
    }
    /* change(add)-hyungchul-20260429-1646 끝 */
    /* 2KB SPI 버퍼를 heap에 할당하여 QuRT stack overflow를 방지합니다. */
    buffer = (uint8_t*)sns_malloc(SNS_HEAP_MAIN, HIMAX_DSP_SPI_CHUNK_SIZE);
    if(NULL == buffer)
    {
        SNS_INST_PRINTF(ERROR, instance, "Himax DSP: SPI buffer allocation failed");
        return;
    }

    /* Android가 이전 frame을 아직 처리 중이면 공유 메모리 write는 skip합니다. */
    if(shared_mem_ready && NULL != shared_mem_ptr && shared_mem_ptr[0] == HIMAX_DSP_SHM_FLAG_READY)
    {
        drop_frame = true;
        SNS_INST_PRINTF(ERROR, instance, "Himax DSP: Android is busy. Drop shared memory write only.");
    }

    /* 첫 번째 2KB chunk를 읽어 header와 일부 JPEG payload를 확보합니다. */
    xfer_bytes = 0;
    if(SNS_RC_SUCCESS != state->com_read(state->scp_service,
                                         state->com_port_info.port_handle,
                                         0x00,
                                         buffer,
                                         HIMAX_DSP_SPI_CHUNK_SIZE,
                                         &xfer_bytes) || xfer_bytes == 0)
    {
        SNS_INST_PRINTF(ERROR, instance, "Himax DSP: First SPI read failed. xfer=%u", xfer_bytes);
        goto cleanup;
    }
    total_read_bytes += xfer_bytes;

    /* Himax frame header sync를 확인합니다. */
    if(!(buffer[0] == 0xC0 && buffer[1] == 0x5A &&
         (buffer[2] == 0x01 || buffer[2] == 0x06 || buffer[2] == 0x16)))
    {
        SNS_INST_PRINTF(ERROR, instance, "Himax DSP: Header sync failed. [%02X %02X %02X]", buffer[0], buffer[1], buffer[2]);
        goto cleanup;
    }

    /* type별 header 길이를 결정합니다. */
    if(buffer[2] == 0x01)
    {
        header_len = 11;
    }
    else
    {
        header_len = 17;
    }

    /* JPEG payload size는 little-endian 4byte로 파싱합니다. */
    jpeg_size = ((uint32_t)buffer[6] << 24) |
                ((uint32_t)buffer[5] << 16) |
                ((uint32_t)buffer[4] << 8)  |
                ((uint32_t)buffer[3]);

    SNS_INST_PRINTF(HIGH, instance,
                    "Himax DSP: Header OK. type=0x%02X header=%u jpeg=%u",
                    buffer[2], header_len, jpeg_size);

    /* header별 메타데이터를 로그로 남깁니다. */
    if(header_len == 11)
    {
        uint8_t result = buffer[7];
        uint8_t similarity = buffer[8];
        uint8_t scene_index = buffer[9];
        uint8_t occl_prob = buffer[10];
        SNS_INST_PRINTF(HIGH, instance,
                        "Himax DSP: 11B result=%u sim=%u scene=%u occ=%u",
                        result, similarity, scene_index, occl_prob);
    }
    else
    {
        uint8_t low_illumination = buffer[7];
        uint8_t similarity = buffer[8];
        uint8_t scene_index = buffer[9];
        uint8_t occl_prob = buffer[10];
        uint8_t blur_value = buffer[11];
        uint8_t expose_0 = buffer[12];
        uint8_t expose_1 = buffer[13];
        uint8_t a_gain = buffer[14];
        uint8_t d_gain_0 = buffer[15];
        uint8_t d_gain_1 = buffer[16];
        SNS_INST_PRINTF(HIGH, instance,
                        "Himax DSP: 17B low=%u sim=%u scene=%u occ=%u blur=%u exp=[%u,%u] gainA=%u gainD=[%u,%u]",
                        low_illumination, similarity, scene_index, occl_prob, blur_value,
                        expose_0, expose_1, a_gain, d_gain_0, d_gain_1);
    }

    /* 공유 메모리 크기를 초과하지 않도록 JPEG size를 제한합니다. */
    if((header_len + jpeg_size) > SHARED_SIZE)
    {
        SNS_INST_PRINTF(ERROR, instance, "Himax DSP: JPEG size exceeds shared memory. clipped.");
        jpeg_size = SHARED_SIZE - header_len;
    }
    total_frame_size = header_len + jpeg_size;

    /* 공유 메모리에 첫 chunk를 복사합니다. 첫 byte는 busy flag로 내려 tearing을 방지합니다. */
    if(!drop_frame && shared_mem_ready && NULL != shared_mem_ptr)
    {
        uint32_t first_copy_len = HIMAX_DSP_MIN(xfer_bytes, total_frame_size);
        buffer[0] = HIMAX_DSP_SHM_FLAG_BUSY;
        memcpy(shared_mem_ptr, buffer, first_copy_len);
    }

    /* 첫 chunk 안에 포함된 payload 양을 계산합니다. */
    if(xfer_bytes > header_len)
    {
        payload_read_bytes = HIMAX_DSP_MIN((xfer_bytes - header_len), jpeg_size);
    }

    /* 남은 payload를 2KB chunk 단위로 읽고 공유 메모리에 이어 붙입니다. */
    while(payload_read_bytes < jpeg_size)
    {
        uint32_t remain = jpeg_size - payload_read_bytes;
        uint32_t chunk = HIMAX_DSP_MIN(remain, HIMAX_DSP_SPI_CHUNK_SIZE);
        uint32_t copy_len = 0;

        xfer_bytes = 0;
        if(SNS_RC_SUCCESS != state->com_read(state->scp_service,
                                             state->com_port_info.port_handle,
                                             0x00,
                                             buffer,
                                             chunk,
                                             &xfer_bytes) || xfer_bytes == 0)
        {
            SNS_INST_PRINTF(ERROR, instance, "Himax DSP: Payload SPI read failed. xfer=%u", xfer_bytes);
            goto cleanup;
        }

        copy_len = HIMAX_DSP_MIN(xfer_bytes, remain);

        if(!drop_frame && shared_mem_ready && NULL != shared_mem_ptr)
        {
            void *current_dest = (void*)(shared_mem_ptr + header_len + payload_read_bytes);
            memcpy(current_dest, buffer, copy_len);
        }

        payload_read_bytes += copy_len;
        total_read_bytes += xfer_bytes;
    }

    /* JPEG payload write가 물리 메모리에 반영되도록 barrier를 수행합니다. */
    qurt_mem_barrier();

    /* 모든 복사가 끝난 경우에만 ready flag를 0xC0으로 올립니다. */
    if(!drop_frame && shared_mem_ready && NULL != shared_mem_ptr)
    {
        shared_mem_ptr[0] = HIMAX_DSP_SHM_FLAG_READY;
        qurt_mem_barrier();
        frame_ready = true;
        SNS_INST_PRINTF(HIGH, instance,
                        "Himax DSP: Shared memory ready. total=%u payload=%u",
                        total_frame_size, jpeg_size);
    }

cleanup:
    /* heap buffer를 해제합니다. */
    if(NULL != buffer)
    {
        sns_free(buffer);
    }

    /* 최종 read byte 수를 로그로 남깁니다. */
    SNS_INST_PRINTF(HIGH, instance, "Himax DSP: Total SPI read bytes=%u", total_read_bytes);

    /* 유효 frame이 공유 메모리에 준비된 경우에만 Android SensorManager 이벤트를 발행합니다. */
    if(frame_ready)
    {
        prox_toggle = !prox_toggle;
        himax_dsp_publish_proximity_trigger_event(instance, timestamp, prox_toggle);
    }

    /* 사용하지 않는 예비 변수를 명시적으로 처리합니다. */
    UNUSED_VAR(psram_virtual_addr);
}

/*
 * 함수명: himax_dsp_inst_init
 * 목적 및 기능: 인스턴스 초기화 시 SPI 통신 버스 파워를 설정하고 하드웨어 인터럽트를 연결합니다.
 * 입력 변수: 
 * - this: 초기화할 센서 인스턴스 포인터
 * - sstate: 센서의 상태 구조체 포인터
 * 출력 변수: 없음
 * 리턴 값: sns_rc (성공 여부, 성공 시 SNS_RC_SUCCESS)
 * change(add)-hyungchul-20260429-1434: 사용자의 원본 하드웨어 설정 로직 복원 적용
 */
sns_rc himax_dsp_inst_init(sns_sensor_instance * const this, sns_sensor_state const *sstate)
{
    himax_dsp_instance_state *state = (himax_dsp_instance_state*) this->state->state;
    himax_dsp_state *sensor_state = (himax_dsp_state*) sstate->state;
    sns_service_manager *service_mgr = this->cb->get_service_manager(this);
    sns_stream_service *stream_mgr = (sns_stream_service*)service_mgr->get_service(service_mgr, SNS_STREAM_SERVICE);

    /* sync COM port service를 획득합니다. */
    state->scp_service = (sns_sync_com_port_service*)service_mgr->get_service(service_mgr, SNS_SYNC_COM_PORT_SERVICE);

    /* SPI read wrapper를 함수 포인터에 연결합니다. */
    state->com_read = himax_com_read_wrapper;

    /* change(add)-hyungchul-20260429-1620: proximity 이벤트 publish를 위해 센서 자신의 SUID를 인스턴스에 복사합니다. */
    sns_memscpy(&state->my_suid, sizeof(state->my_suid), &sensor_state->my_suid, sizeof(sensor_state->my_suid));

    /* interrupt 서비스 SUID를 인스턴스에 복사합니다. */
    sns_memscpy(&state->irq_suid, sizeof(state->irq_suid), &sensor_state->irq_suid, sizeof(sensor_state->irq_suid));

    /* SPI 5번(SSC_QUP_SE4) 설정을 적용합니다. */
    state->com_port_info.com_config.bus_type = SNS_BUS_SPI;
    state->com_port_info.com_config.bus_instance = 5;
    state->com_port_info.com_config.slave_control = 0;
    state->com_port_info.com_config.min_bus_speed_KHz = 15000; // SPI 클럭 최소 15MHz
    state->com_port_info.com_config.max_bus_speed_KHz = 15000; // SPI 클럭 최대 15MHz
    state->com_port_info.com_config.reg_addr_type = 0x0; 

    // SPI 포트 등록 및 오픈
    state->scp_service->api->sns_scp_register_com_port(&state->com_port_info.com_config, &state->com_port_info.port_handle);
    state->scp_service->api->sns_scp_open(state->com_port_info.port_handle);
    state->scp_service->api->sns_scp_update_bus_power(state->com_port_info.port_handle, false); // 슬립 상태로 대기

    /* GPIO 102 interrupt stream을 생성합니다. */
    if(NULL == state->interrupt_data_stream)
    {
        stream_mgr->api->create_sensor_instance_stream(stream_mgr, this, state->irq_suid, &state->interrupt_data_stream);
    }

    /* GPIO 102 rising edge interrupt 요청을 interrupt service에 보냅니다. */
    if(NULL != state->interrupt_data_stream)
    {
        uint8_t buffer[20];
        sns_request irq_req = { .message_id = SNS_INTERRUPT_MSGID_SNS_INTERRUPT_REQ, .request = buffer };
        sns_interrupt_req req_payload = sns_interrupt_req_init_default;

        req_payload.interrupt_trigger_type = SNS_INTERRUPT_TRIGGER_TYPE_RISING; // Rising Edge 트리거
        req_payload.interrupt_num = 102; // 대상 핀: GPIO 102
        req_payload.interrupt_pull_type = SNS_INTERRUPT_PULL_TYPE_PULL_DOWN; // Pull-down 설정
        req_payload.is_chip_pin = true;
        req_payload.interrupt_drive_strength = SNS_INTERRUPT_DRIVE_STRENGTH_2_MILLI_AMP;

        irq_req.request_len = pb_encode_request(buffer, sizeof(buffer), &req_payload, sns_interrupt_req_fields, NULL);
        if (irq_req.request_len > 0)
        {
            state->interrupt_data_stream->api->send_request(state->interrupt_data_stream, &irq_req);
            SNS_INST_PRINTF(HIGH, this, "Himax DSP: Interrupt registered on GPIO %d", req_payload.interrupt_num);
        }
    }

    return SNS_RC_SUCCESS;
}

/*
 * 함수명: himax_dsp_inst_deinit
 * 목적 및 기능: 인스턴스 종료 시 스트림과 통신 포트를 해제하여 시스템 메모리 누수를 방지합니다.
 * 입력 변수: 
 * - this: 종료할 센서 인스턴스 포인터
 * 출력 변수: 없음
 * 리턴 값: sns_rc (성공 시 SNS_RC_SUCCESS)
 * change(add)-hyungchul-20260429-1434: 자원 해제 로직 완벽 복원
 */
sns_rc himax_dsp_inst_deinit(sns_sensor_instance *const this)
{
    himax_dsp_instance_state *state = (himax_dsp_instance_state*) this->state->state;
    sns_service_manager *service_mgr = this->cb->get_service_manager(this);
    sns_stream_service *stream_mgr = (sns_stream_service*) service_mgr->get_service(service_mgr, SNS_STREAM_SERVICE);

    // 인터럽트 스트림 해제
    if(NULL != state->interrupt_data_stream)
    {
        stream_mgr->api->remove_stream(stream_mgr, state->interrupt_data_stream);
        state->interrupt_data_stream = NULL;
    }
    // SPI 포트 연결 종료 및 등록 해제
    if(NULL != state->scp_service && NULL != state->com_port_info.port_handle)
    {
        state->scp_service->api->sns_scp_close(state->com_port_info.port_handle);
        state->scp_service->api->sns_scp_deregister_com_port(&state->com_port_info.port_handle);
    }
    return SNS_RC_SUCCESS;
}

/*
 * 함수명: himax_dsp_inst_notify_event
 * 목적 및 기능: interrupt stream 이벤트를 수신하여 Himax DSP SPI JPEG 처리 함수로 전달합니다.
 * 입력 변수:
 * - this: 센서 인스턴스 포인터입니다.
 * 출력 변수: 없음
 * 리턴 값:
 * - SNS_RC_SUCCESS: 이벤트 처리 완료
 */
sns_rc himax_dsp_inst_notify_event(sns_sensor_instance * const this)
{
    himax_dsp_instance_state *state = (himax_dsp_instance_state*) this->state->state;
    sns_sensor_event *event;

    if(NULL == state->scp_service || NULL == state->com_port_info.port_handle)
    {
        return SNS_RC_SUCCESS;
    }

    state->scp_service->api->sns_scp_update_bus_power(state->com_port_info.port_handle, true);

    if (NULL != state->interrupt_data_stream)
    {
        /*
         * change(add)-hyungchul-20260429-1740:
         * 첫 이벤트는 peek_input()으로 확인해야 합니다.
         * get_next_input()은 현재 이벤트를 pop한 뒤 다음 이벤트를 반환하므로,
         * 루프 시작에서 호출하면 첫 interrupt event를 처리하지 못하고 버릴 수 있습니다.
         */
        event = state->interrupt_data_stream->api->peek_input(state->interrupt_data_stream);
        while (NULL != event)
        {
            if (SNS_INTERRUPT_MSGID_SNS_INTERRUPT_EVENT == event->message_id)
            {
                sns_interrupt_event irq_event = sns_interrupt_event_init_default;
                pb_istream_t stream = pb_istream_from_buffer((pb_byte_t*)event->event, event->event_len);
                // 이벤트 디코딩 성공 시, 실질적인 JPEG 파싱 함수 호출
                if (pb_decode(&stream, sns_interrupt_event_fields, &irq_event))
                {
                    himax_dsp_handle_interrupt_event(this, irq_event.timestamp);
                }
            }
            event = state->interrupt_data_stream->api->get_next_input(state->interrupt_data_stream);
        }
    }

    // 통신 버스 전원 비활성화 (절전 모드 전환)
    state->scp_service->api->sns_scp_update_bus_power(state->com_port_info.port_handle, false);
    return SNS_RC_SUCCESS;
}

/*
 * 함수명: himax_dsp_inst_set_client_config
 * 목적 및 기능: Android SensorManager의 on-change enable request를 수락합니다. 실제 데이터 생성은 GPIO interrupt가 담당합니다.
 * 입력 변수:
 * - this: 센서 인스턴스 포인터입니다.
 * - client_request: client request 포인터입니다.
 * 출력 변수: 없음
 * 리턴 값:
 * - SNS_RC_SUCCESS: 설정 수락
 */
sns_rc himax_dsp_inst_set_client_config(sns_sensor_instance * const this, sns_request const *client_request) 
{ 
    if(NULL != client_request)
    {
        SNS_INST_PRINTF(HIGH, this, "Himax DSP: Client config msg_id=%u", client_request->message_id);
    }
    else
    {
        SNS_INST_PRINTF(HIGH, this, "Himax DSP: Client config is NULL");
    }

    return SNS_RC_SUCCESS;
}

// 인스턴스 API 바인딩 구조체 초기화
sns_sensor_instance_api himax_dsp_sensor_instance_api =
{
    .struct_len = sizeof(sns_sensor_instance_api),
    .init = &himax_dsp_inst_init,
    .deinit = &himax_dsp_inst_deinit,
    .set_client_config = &himax_dsp_inst_set_client_config,
    .notify_event = &himax_dsp_inst_notify_event
};
