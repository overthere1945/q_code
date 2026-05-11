/*
* 파일명: sns_himax_dsp_sensor.h
* 목적 및 기능: Himax DSP 센서 관리를 위한 SUID 정의, 공통 매크로, 그리고 센서 상태(State) 구조체를 정의합니다.
* change(add)-hyungchul-20260429-1346: Android HAL 필터링 우회를 위해 센서 타입을 표준 근접(Proximity) 센서로 위장
*/
#pragma once

#include "sns_data_stream.h"
#include "sns_diag_service.h"
#include "sns_printf.h"
#include "sns_sensor.h"
#include "sns_suid_util.h"
#include "sns_sync_com_port_service.h"
#include "sns_himax_dsp_sensor_instance.h"

// Himax DSP 전용 고유 SUID 및 속성 정의
#define HIMAX_DSP_SUID_0 \
{  \
    .sensor_uid =  \
    {  \
        0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22, 0x33, 0x44, \
        0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0xAA, 0xBB  \
    }  \
}

// change(add)-hyungchul-20260429-1346: "image_sensor"에서 "proximity"로 변경하여 표준 센서로 위장
#define HIMAX_DSP_TYPE "proximity"
#define HIMAX_DSP_PROTO "sns_proximity.proto"
#define SUID_NUM 3

/* Forward Declaration of Himax Sensor API */
extern sns_sensor_api himax_dsp_sensor_api;

/*
* 구조체명: himax_dsp_state
* 목적 및 기능: 센서 레벨에서의 스트림, SUID 리스트, COM 포트 정보 등을 보관하는 상태 구조체
*/
typedef struct himax_dsp_state
{
  /* 데이터 스트림 관리 */
  sns_data_stream              *fw_stream;   // 프레임워크 이벤트 수신용
  
  /* 의존하는 센서(서비스)들의 SUID */
  sns_sensor_uid               irq_suid;     // 인터럽트 처리 서비스 SUID
  sns_sensor_uid               acp_suid;     // 비동기 COM 포트 서비스 SUID
  sns_sensor_uid               my_suid;      // Himax DSP 본연의 SUID
  SNS_SUID_LOOKUP_DATA(SUID_NUM) suid_lookup_data;/* 기존 구조 호환용 SUID lookup 데이터입니다. */

  /* COM 포트(SPI) 설정 정보 */
  himax_dsp_com_port_info      com_port_info;

  sns_diag_service             *diag_service;     /* diag 서비스 포인터입니다. */
  sns_sync_com_port_service    *scp_service;      /* sync COM 포트 서비스 포인터입니다. */
  
  /* 전력 및 인터럽트 설정 */
  uint32_t                      irq_pin;          /* interrupt GPIO 번호 저장용 필드입니다. */
  bool                          is_island_mode;   /* island mode 상태 저장용 필드입니다. */
  bool                          hw_is_present;    /* HW 정상 인식 여부입니다. */
} himax_dsp_state;

/* SUID 이벤트 처리 함수 선언입니다. */
void himax_dsp_sensor_process_suid_events(sns_sensor * const this);

/* Android/HLOS client request 처리 함수 선언입니다. */
sns_sensor_instance* himax_dsp_sensor_set_client_request(sns_sensor * const this,
        struct sns_request const *exist_request,
        struct sns_request const *new_request,
        bool remove);

/* 센서 notify_event 콜백 선언입니다. */
sns_rc himax_dsp_sensor_notify_event(sns_sensor * const this);

/* 센서 init 콜백 선언입니다. */
sns_rc himax_dsp_init(sns_sensor * const this);

/* 센서 deinit 콜백 선언입니다. */
sns_rc himax_dsp_deinit(sns_sensor * const this);
