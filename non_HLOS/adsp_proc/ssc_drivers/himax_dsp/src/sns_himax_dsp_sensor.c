/*
* 파일명: sns_himax_dsp_sensor.c
* 목적 및 기능: Himax DSP 센서의 라이프사이클(초기화/해제)을 관리하고, SUID 획득 및 클라이언트 요청을 인스턴스로 라우팅합니다.
* change(add)-hyungchul-20260429-1346: 센서 이름 및 벤더 정보를 Proximity Trigger 목적으로 변경
*/

/*==============================================================================
  Include Files
  ============================================================================*/
#include <string.h>
#include "sns_mem_util.h"
#include "sns_service_manager.h"
#include "sns_stream_service.h"
#include "sns_sensor_util.h"
#include "sns_attribute_util.h"           /* sns_publish_attribute를 사용하기 위한 헤더입니다. */
#include "sns_himax_dsp_sensor.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "sns_suid.pb.h"
#include "sns_pb_util.h"
#include "sns_suid_util.h"
#include "sns_gpio_service.h"
#include "sns_std.pb.h"                   /* sns_std_attr_value_data 타입을 사용하기 위한 헤더입니다. */
#include "sns_std_sensor.pb.h"

/*==============================================================================
  Function Definitions
  ============================================================================*/
/*
 * 함수명: himax_dsp_publish_available
 * 목적 및 기능: Himax DSP Proximity Trigger 센서의 AVAILABLE attribute를 publish합니다.
 * 입력 변수:
 * - this: 센서 객체 포인터입니다.
 * - available: true이면 Android/HLOS client가 사용할 수 있음을 의미합니다.
 * - completed: true이면 attribute publish 완료 상태로 표시합니다.
 * 출력 변수: 없음
 * 리턴 값: 없음
 */
static void himax_dsp_publish_available(sns_sensor *const this, bool available, bool completed)
{
  sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
  value.has_boolean = true;
  value.boolean = available;
  sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_AVAILABLE, &value, 1, completed);
}

/*
 * 함수명: himax_dsp_publish_attributes
 * 목적 및 기능: Android Sensor HAL이 Himax DSP를 표준 proximity 센서로 인식하도록 필수 속성을 publish합니다.
 * 입력 변수:
 * - this: 센서 객체 포인터입니다.
 * 출력 변수: 없음
 * 리턴 값: 없음
 */
static void himax_dsp_publish_attributes(sns_sensor *const this)
{
  // change(add)-hyungchul-20260429-1346: 안드로이드 프레임워크에 등록될 위장 센서 정보 설정
  char const name[] = "scarlett_himax_Trigger";
  char const vendor[] = "LGE";
  char const type[] = "proximity";
  
  /* change(add)-hyungchul-20260429-1620: proximity 전용 event payload proto를 명시합니다. */
  char const proto[] = HIMAX_DSP_PROTO;

  /* 센서 이름 attribute를 publish합니다. */
  {
  sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
  
  // 센서 이름 등록
  value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((struct pb_buffer_arg){ .buf = name, .buf_len = sizeof(name) });
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_NAME, &value, 1, false);
  }
  
  // 센서 벤더 등록
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((struct pb_buffer_arg){ .buf = vendor, .buf_len = sizeof(vendor) });
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VENDOR, &value, 1, false);
  }
  
  // 센서 타입 등록
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((struct pb_buffer_arg){ .buf = type, .buf_len = sizeof(type) });
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_TYPE, &value, 1, false);
  }

  /* 센서 API proto attribute를 sns_proximity.proto로 publish합니다. */
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.str.funcs.encode = pb_encode_string_cb;
    value.str.arg = &((struct pb_buffer_arg){ .buf = proto, .buf_len = sizeof(proto) });
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_API, &value, 1, false);
  }

  /* 센서 version attribute를 publish합니다. */
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = 1;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_VERSION, &value, 1, false);
  }

  /* 센서 stream type을 on-change로 publish합니다. Proximity trigger는 NEAR/FAR 변화 이벤트만 보냅니다. */
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_sint = true;
    value.sint = SNS_STD_SENSOR_STREAM_TYPE_ON_CHANGE;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_STREAM_TYPE, &value, 1, false);
  }

  /* Proximity distance range를 cm 단위로 publish합니다. 0cm=NEAR, 5cm=FAR trigger로 사용합니다. */
  {
    sns_std_attr_value_data values[1] = { sns_std_attr_value_data_init_default };
    sns_std_attr_value_data range[2] = { sns_std_attr_value_data_init_default, sns_std_attr_value_data_init_default };

    range[0].has_flt = true;
    range[0].flt = 0.0f;
    range[1].has_flt = true;
    range[1].flt = 5.0f;

    values[0].has_subtype = true;
    values[0].subtype.values.funcs.encode = sns_pb_encode_attr_cb;
    values[0].subtype.values.arg = &((struct pb_buffer_arg){ .buf = range, .buf_len = 2 });

    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_RANGES, values, 1, false);
  }

  /* 실제 물리 센서처럼 HAL에 노출되도록 physical_sensor attribute를 publish합니다. */
  {
    sns_std_attr_value_data value = sns_std_attr_value_data_init_default;
    value.has_boolean = true;
    value.boolean = true;
    sns_publish_attribute(this, SNS_STD_SENSOR_ATTRID_PHYSICAL_SENSOR, &value, 1, false);
  }

  /* change(add)-hyungchul-20260429-1620: SUID와 interrupt stream 준비 전에는 available=false로 두어 조기 activation race를 막습니다. */
  himax_dsp_publish_available(this, false, false);
}

/*==============================================================================
  Public Function Definitions
  ============================================================================*/
/*
* 함수명: himax_dsp_send_suid_req
* 목적 및 기능: 프레임워크(SEE)에 필요한 하위 서비스(인터럽트, 통신 등)의 SUID를 요청합니다.
* 입력 변수: this(센서 포인터), data_type(요청할 서비스명), data_type_len(길이)
* 출력 변수: 없음
* 리턴 값: 없음
*/
void himax_dsp_send_suid_req(sns_sensor *this, char const *data_type, uint32_t data_type_len)
{
  uint8_t buffer[50];
  sns_memset(buffer, 0, sizeof(buffer));

  himax_dsp_state *state = (himax_dsp_state*)this->state->state;
  sns_service_manager *manager = this->cb->get_service_manager(this);
  sns_stream_service *stream_service = (sns_stream_service*)manager->get_service(manager, SNS_STREAM_SERVICE);

  struct pb_buffer_arg data = (struct pb_buffer_arg){ .buf = data_type, .buf_len = data_type_len };
  sns_suid_req suid_req = sns_suid_req_init_default;

    suid_req.has_register_updates = true;
    suid_req.register_updates = true;
    suid_req.data_type.funcs.encode = &pb_encode_string_cb;
    suid_req.data_type.arg = &data;

  if(NULL == state->fw_stream)
  {
    stream_service->api->create_sensor_stream(stream_service, this, sns_get_suid_lookup(), &state->fw_stream);
  }

  if(NULL != state->fw_stream)
  {
    size_t encoded_len = pb_encode_request(buffer, sizeof(buffer), &suid_req, sns_suid_req_fields, NULL);
    if(encoded_len > 0)
    {
      sns_request request = {
        .message_id = SNS_SUID_MSGID_SNS_SUID_REQ,
        .request_len = encoded_len,
        .request = buffer
      };
      state->fw_stream->api->send_request(state->fw_stream, &request);
    }
  }
}

/*
 * 함수명: himax_dsp_sensor_process_suid_events
 * 목적 및 기능: SUID lookup 응답을 처리하여 interrupt SUID와 async_com_port SUID를 저장합니다.
 * 입력 변수:
 * - this: 센서 객체 포인터입니다.
 * 출력 변수: 없음
 * 리턴 값: 없음
 */
void himax_dsp_sensor_process_suid_events(sns_sensor *const this)
{
  himax_dsp_state *state = (himax_dsp_state*)this->state->state;

  if(NULL == state->fw_stream)
  {
    return;
  }

  for(; 0 != state->fw_stream->api->get_input_cnt(state->fw_stream);
      state->fw_stream->api->get_next_input(state->fw_stream))
  {
    sns_sensor_event *event = state->fw_stream->api->peek_input(state->fw_stream);

    if(NULL == event)
    {
      break;
    }

    if(SNS_SUID_MSGID_SNS_SUID_EVENT == event->message_id)
    {
      pb_istream_t stream = pb_istream_from_buffer((void*)event->event, event->event_len);
      sns_suid_event suid_event = sns_suid_event_init_default;

      struct pb_buffer_arg data_type_arg = { .buf = NULL, .buf_len = 0 };
      sns_sensor_uid uid_list;
      sns_suid_search suid_search;

      suid_search.suid = &uid_list;
      suid_search.num_of_suids = 0;

      suid_event.data_type.funcs.decode = &pb_decode_string_cb;
      suid_event.data_type.arg = &data_type_arg;
      suid_event.suid.funcs.decode = &pb_decode_suid_event;
      suid_event.suid.arg = &suid_search;

      if(pb_decode(&stream, sns_suid_event_fields, &suid_event) && suid_search.num_of_suids > 0)
      {
        if(0 == strncmp((char*)data_type_arg.buf, "interrupt", data_type_arg.buf_len))
        {
          state->irq_suid = uid_list;
        }
        else if(0 == strncmp((char*)data_type_arg.buf, "async_com_port", data_type_arg.buf_len))
        {
          state->acp_suid = uid_list;
        }
      }
    }
  }
}


/*
* 함수명: himax_dsp_sensor_notify_event
* 목적 및 기능: SEE 프레임워크로부터 SUID 검색 결과 등의 이벤트를 수신받아 상태를 업데이트합니다.
 * 입력 변수:
 * - this: 센서 객체 포인터입니다.
 * 출력 변수: 없음
 * 리턴 값:
 * - SNS_RC_SUCCESS: 이벤트 처리 완료
 */
sns_rc himax_dsp_sensor_notify_event(sns_sensor *const this)
{
  himax_dsp_state *state = (himax_dsp_state*)this->state->state;

  if(NULL != state->fw_stream)
  {
    himax_dsp_sensor_process_suid_events(this);

    if((0 != sns_memcmp(&state->acp_suid, &((sns_sensor_uid){{0}}), sizeof(state->acp_suid))) &&
       (0 != sns_memcmp(&state->irq_suid, &((sns_sensor_uid){{0}}), sizeof(state->irq_suid))))
    {
      state->hw_is_present = true;

      if(NULL == sns_sensor_util_get_shared_instance(this))
      {
        sns_sensor_instance *instance = this->cb->create_instance(this, sizeof(himax_dsp_instance_state));
        if(NULL != instance)
        {
          SNS_PRINTF(HIGH, this, "Himax DSP: Instance Auto-created to wait for GPIO interrupt");
        }
      }

      /* change(add)-hyungchul-20260429-1620: interrupt/acp SUID 준비가 끝난 뒤 Android SensorManager에 available=true를 알립니다. */
      himax_dsp_publish_available(this, true, true);

      sns_sensor_util_remove_sensor_stream(this, &state->fw_stream);
    }
  }
  return SNS_RC_SUCCESS;
}

/*
 * 함수명: himax_dsp_get_sensor_uid
 * 목적 및 기능: Himax DSP Proximity Trigger 센서의 고유 SUID를 반환합니다.
 * 입력 변수:
 * - this: 센서 객체 포인터입니다.
 * 출력 변수: 없음
 * 리턴 값:
 * - Himax DSP 센서 SUID 포인터
 */
static sns_sensor_uid const* himax_dsp_get_sensor_uid(sns_sensor const * const this)
{
  himax_dsp_state *state = (himax_dsp_state*)this->state->state;
  return &state->my_suid;
}

/*
 * 함수명: himax_dsp_sensor_set_client_request
 * 목적 및 기능: Android SensorManager의 proximity enable/remove 요청을 Himax shared instance에 연결합니다.
 * 입력 변수:
 * - this: 센서 객체 포인터입니다.
 * - exist_request: 기존 client request 포인터입니다.
 * - new_request: 신규 client request 포인터입니다.
 * - remove: request 제거 여부입니다.
 * 출력 변수: 없음
 * 리턴 값:
 * - 사용 중인 Himax shared instance 또는 NULL
 */
sns_sensor_instance* himax_dsp_sensor_set_client_request(sns_sensor * const this,
        struct sns_request const *exist_request,
        struct sns_request const *new_request,
        bool remove)
{
  sns_sensor_instance *instance = sns_sensor_util_get_shared_instance(this);

  if(remove)
  {
    if(NULL != instance && NULL != exist_request)
    {
      instance->cb->remove_client_request(instance, exist_request);
    }

    return NULL;
  }

  if(NULL == instance)
  {
    instance = this->cb->create_instance(this, sizeof(himax_dsp_instance_state));
  }

  if(NULL != instance)
  {
    if(NULL != exist_request)
    {
      instance->cb->remove_client_request(instance, exist_request);
    }

    if(NULL != new_request)
    {
      instance->cb->add_client_request(instance, new_request);
      this->instance_api->set_client_config(instance, new_request);
    }
  }

  return instance;
}

/*
 * 함수명: himax_dsp_init
 * 목적 및 기능: 센서 객체 생성 시 기본 SUID, GPIO 전원, SUID lookup, proximity 속성을 초기화합니다.
 * 입력 변수:
 * - this: 센서 객체 포인터입니다.
 * 출력 변수: 없음
 * 리턴 값:
 * - SNS_RC_SUCCESS: 초기화 완료
 */
sns_rc himax_dsp_init(sns_sensor * const this)
{
  himax_dsp_state *state = (himax_dsp_state*) this->state->state;
  struct sns_service_manager *smgr = this->cb->get_service_manager(this);
  sns_gpio_service *gpio_service;

  state->diag_service = (sns_diag_service *) smgr->get_service(smgr, SNS_DIAG_SERVICE);
  state->scp_service = (sns_sync_com_port_service *) smgr->get_service(smgr, SNS_SYNC_COM_PORT_SERVICE);
  state->my_suid = (sns_sensor_uid)HIMAX_DSP_SUID_0;
  state->irq_pin = 102;
  state->is_island_mode = false;
  state->hw_is_present = false;

  gpio_service = (sns_gpio_service *)smgr->get_service(smgr, SNS_GPIO_SERVICE);
  if(NULL != gpio_service && NULL != gpio_service->api)
  {
    gpio_service->api->write_gpio(106, true, SNS_GPIO_DRIVE_STRENGTH_2_MILLI_AMP,
                                  SNS_GPIO_PULL_TYPE_NO_PULL, SNS_GPIO_STATE_HIGH);
    SNS_PRINTF(HIGH, this, "Set GPIO 106 to HIGH");

    gpio_service->api->write_gpio(14, true, SNS_GPIO_DRIVE_STRENGTH_2_MILLI_AMP,
                                  SNS_GPIO_PULL_TYPE_NO_PULL, SNS_GPIO_STATE_HIGH);
    SNS_PRINTF(HIGH, this, "Set GPIO 14 to HIGH");

    gpio_service->api->write_gpio(15, true, SNS_GPIO_DRIVE_STRENGTH_2_MILLI_AMP,
                                  SNS_GPIO_PULL_TYPE_NO_PULL, SNS_GPIO_STATE_HIGH);
    SNS_PRINTF(HIGH, this, "Set GPIO 15 to HIGH");
  }

  himax_dsp_publish_attributes(this);

  himax_dsp_send_suid_req(this, "interrupt", 9);
  himax_dsp_send_suid_req(this, "async_com_port", 15);

  return SNS_RC_SUCCESS;
}

/*
 * 함수명: himax_dsp_deinit
 * 목적 및 기능: 센서 객체 해제 시 별도 리소스 정리가 필요 없음을 명시합니다.
 * 입력 변수:
 * - this: 센서 객체 포인터입니다.
 * 출력 변수: 없음
 * 리턴 값:
 * - SNS_RC_SUCCESS: 해제 완료
 */
sns_rc himax_dsp_deinit(sns_sensor * const this)
{
  UNUSED_VAR(this);
  return SNS_RC_SUCCESS;
}

/* 센서 API 바인딩 구조체입니다. */
sns_sensor_api himax_dsp_sensor_api =
{
  .struct_len = sizeof(sns_sensor_api),
  .init = &himax_dsp_init,
  .deinit = &himax_dsp_deinit,
  .get_sensor_uid = &himax_dsp_get_sensor_uid,
  .set_client_request = &himax_dsp_sensor_set_client_request,
  .notify_event = &himax_dsp_sensor_notify_event,
};
