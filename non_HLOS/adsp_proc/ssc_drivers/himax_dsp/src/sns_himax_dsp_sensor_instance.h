/*
 * 파일명: sns_himax_dsp_sensor_instance.h
 * 목적 및 기능: Himax DSP 하드웨어 인터럽트, SPI 통신, 공유 메모리, Proximity Trigger 이벤트 발행에 필요한 인스턴스 상태를 정의합니다.
 * change(add)-hyungchul-20260429-1620: Proximity 이벤트 publish 대상 SUID(my_suid)를 인스턴스 상태에 추가
 */
#pragma once

#include <stdint.h>
#include "sns_sensor_instance.h"
#include "sns_data_stream.h"
#include "sns_com_port_types.h"
#include "sns_sync_com_port_service.h"
#include "sns_async_com_port.pb.h"
#include "sns_interrupt.pb.h"
#include <qurt.h> // QuRT 메모리 맵핑용 헤더

/* 공유 물리 메모리 주소입니다. Linux UIO 예약 주소와 동일해야 합니다. */
#define SHARED_PHYS_ADDR  0x81EC0000

/* 공유 물리 메모리 크기입니다. 현재 128KB로 사용합니다. */
#define SHARED_SIZE       0x20000

/* 공유 메모리 상태 플래그입니다. 0x00은 ADSP 쓰기 중 또는 빈 상태입니다. */
#define HIMAX_DSP_SHM_FLAG_BUSY   0x00

/* 공유 메모리 상태 플래그입니다. 0xC0은 Android 읽기 가능 상태입니다. */
#define HIMAX_DSP_SHM_FLAG_READY  0xC0

/* SPI 청크 크기입니다. Himax DSP 데이터 동기화를 위해 2KB 단위를 유지합니다. */
#define HIMAX_DSP_SPI_CHUNK_SIZE  2048

/*
 * 구조체명: himax_dsp_com_port_info
 * 목적 및 기능: Himax DSP SPI 통신 포트 설정과 핸들을 보관합니다.
 */
typedef struct himax_dsp_com_port_info
{
  sns_com_port_config        com_config;   /* SPI bus instance, 속도, slave 설정을 보관합니다. */
  sns_sync_com_port_handle  *port_handle;  /* 등록된 동기식 COM 포트 핸들을 보관합니다. */
} himax_dsp_com_port_info;

/*
 * 구조체명: himax_dsp_instance_state
 * 목적 및 기능: Himax DSP 인스턴스가 사용하는 SPI, interrupt stream, publish SUID 상태를 보관합니다.
 */
typedef struct himax_dsp_instance_state
{
  /* COM 포트 정보 및 서비스 */
  himax_dsp_com_port_info        com_port_info;              /* SPI COM 포트 설정과 핸들입니다. */
  sns_sync_com_port_service     *scp_service;                /* 동기식 COM 포트 서비스 포인터입니다. */
  sns_async_com_port_config      ascp_config;                /* 기존 구조 유지용 async COM 설정입니다. */

  /* SUID 및 스트림 관리 */
  sns_sensor_uid                 my_suid;                    /* change(add)-hyungchul-20260429-1620: Proximity 이벤트 publish 대상 Himax 센서 SUID입니다. */
  sns_sensor_uid                 irq_suid;
  sns_data_stream                *interrupt_data_stream;
  sns_data_stream                *async_com_port_data_stream;

  /* 통신 Read Wrapper */
  sns_rc (* com_read)(                                      /* SPI read wrapper 함수 포인터입니다. */
      sns_sync_com_port_service *scp_service,               /* 동기식 COM 포트 서비스입니다. */
      sns_sync_com_port_handle  *port_handle,               /* SPI 포트 핸들입니다. */
      uint32_t                   rega,                      /* SPI read 시작 register/address입니다. */
      uint8_t                   *regv,                      /* SPI read 결과를 저장할 버퍼입니다. */
      uint32_t                   bytes,                     /* 읽을 바이트 수입니다. */
      uint32_t                  *xfer_bytes);               /* 실제 전송 완료 바이트 수입니다. */
} himax_dsp_instance_state;

/* Himax DSP 인스턴스 API 외부 선언입니다. */
extern sns_sensor_instance_api himax_dsp_sensor_instance_api;
