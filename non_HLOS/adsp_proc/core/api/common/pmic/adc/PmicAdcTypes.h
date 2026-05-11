#ifndef __PMICADCTYPES_H__
#define __PMICADCTYPES_H__

/*============================================================================
  @file PmicAdcTypes.h

  ADC Device Driver Interface header

  Clients may include this header and use these interfaces to read analog
  inputs.

  Copyright (c) Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/
/* $Header: //components/rel/core.qdsp6/8.2.3/api/common/pmic/adc/PmicAdcTypes.h#1 $ */

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "AdcInputs.h"
#include "DALSys.h"
#include <stdint.h>
#include <stdbool.h>
#ifdef ARCH_QDSP6
#include "qurt_anysignal.h"
#endif

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/
#define ADC_MAX_INPUT_STRING_LEN 48
#define ADC_CLIENT_NAME_MAX_LEN  24

enum
{
   ADC_SUCCESS = 0,
   ADC_ERROR_INVALID_DEVICE_IDX = 1,
   ADC_ERROR_INVALID_CHANNEL_IDX,
   ADC_ERROR_NULL_POINTER,
   ADC_ERROR_DEVICE_QUEUE_FULL,
   ADC_ERROR_INVALID_PROPERTY_LENGTH,
   ADC_ERROR_OUT_OF_MEMORY,
   ADC_ERROR_API_UNSUPPORTED_IN_THIS_CONTEXT,
   ADC_ERROR_DEVICE_NOT_READY,
   ADC_ERROR_INVALID_PARAMETER,
   ADC_ERROR_OUT_OF_TM_CLIENTS,
   ADC_ERROR_TM_NOT_SUPPORTED,
   ADC_ERROR_TM_THRESHOLD_OUT_OF_RANGE,
   ADC_ERROR_TM_BUSY,
   ADC_ERROR = -1,
   ADC_ERROR_DEVICE_NOT_SUPPORTED = -2,
   ADC_ERROR_INVALID_ADDRESS = -3,
   ADC_ERROR_OUT_CLIENTS_FOR_PD = -4,
};

/*-------------------------------------------------------------------------
 * Type Declarations
 * ----------------------------------------------------------------------*/
typedef uintptr_t AdcHandle; 

typedef uint32_t AdcResult; 

typedef enum
{
   ADC_RESULT_INVALID,
   ADC_RESULT_VALID,
   ADC_RESULT_TIMEOUT,
   ADC_RESULT_FIFO_NOT_EMPTY,
   ADC_RESULT_STALE,
   _ADC_MAX_RESULT_STATUS = 0x7FFFFFFF
} AdcResultStatusType;

typedef enum {
   ADC_EVENT_CALLBACK,
   ADC_EVENT_SIGNAL,
   ADC_EVENT_DALEVENT, /* Keep support for legacy DALSYS events */
   ADC_EVENT_MAX,
} AdcEventTriggerType;

typedef struct
{
   enum
   {
      ADC_REQUEST_STATUS_PENDING,   /* the request is being immediately performed */
      ADC_REQUEST_STATUS_QUEUED,    /* the request was queued */
      ADC_REQUEST_STATUS_ERROR,     /* the request was not started due to an error */
      ADC_REQUEST_STATUS_UNKNOWN,   /* the request status is unknown */
      _ADC_MAX_REQUEST_STATUS = 0x7FFFFFFF
   } eStatus;
} AdcRequestStatusType;

struct AdcCbType {
   void (*cb)(void *, void *, uint32_t);
   void *ctxt;
};

#ifdef ARCH_QDSP6
typedef struct AdcSigEventType {
   qurt_anysignal_t *phEvent;
   uint32_t uSigMask;
} AdcSigEventType;
#else
typedef uintptr_t AdcSigEventType;
#endif

struct AdcEventType {
   void *pEvent;
   AdcEventTriggerType eEvent;
};

typedef struct
{
   uint32_t nDeviceIdx;
   uint32_t nChannelIdx;
} AdcInputPropertiesType;

typedef struct
{
   uint32_t nDeviceIdx;
   uint32_t nChannelIdx;
   union {
      DALSYSEventHandle hEvent;
      struct AdcEventType *pAdcEvent;
   };
} AdcRequestParametersType;

typedef struct
{
   AdcResultStatusType eStatus;  /* status of the conversion */
   uint32_t nDeviceIdx;   /* the device index for this conversion */
   uint32_t nChannelIdx;  /* the channel index for this conversion */
   int32_t nPhysical;     /* result in physical units. Units depend on BSP */
   uint32_t nPercent;     /* result as percentage of reference voltage used
                         * for conversion. 0 = 0%, 65535 = 100% */
   uint32_t nMicrovolts;  /* result in microvolts */
   uint32_t nCode;        /* raw ADC code from hardware */
} AdcResultType;

typedef struct
{
   AdcResultStatusType eStatus;  /* status of the conversion */
   uint32_t uToken;       /* token which identifies this conversion */
   uint32_t uDeviceIdx;   /* the device index for this conversion */
   uint32_t uChannelIdx;  /* the channel index for this conversion */
   int32_t nPhysical1_uV; /* ref 1 in physical units of uV*/
   int32_t nPhysical2_uV; /* ref 2 in physical units of uV */
   uint32_t uCode1;       /* raw ADC code for ref 1 */
   uint32_t uCode2;       /* raw ADC code for ref 2 */
} AdcRecalibrationResultType;

typedef struct
{
   uint32_t uDeviceIdx;
   uint32_t uChannelIdx;
} AdcTMInputPropertiesType;

typedef struct
{
   AdcTMInputPropertiesType adcTMInputProps;
   union {
      DALSYSEventHandle hEvent;
      struct AdcEventType *pAdcEvent;
   };
} AdcTMRequestParametersType;

typedef enum
{
   ADC_TM_THRESHOLD_LOWER,    /* Lower threshold */
   ADC_TM_THRESHOLD_HIGHER,   /* Higher threshold */
   _ADC_TM_NUM_THRESHOLDS,
   _ADC_TM_MAX_THRESHOLD = 0x7FFFFFFF
} AdcTMThresholdType;

typedef struct
{
   int32_t nPhysicalMin;   /* Minimum threshold in physical units */
   int32_t nPhysicalMax;   /* Maximum threshold in physical units */
} AdcTMRangeType;

typedef struct
{
   AdcTMInputPropertiesType adcTMInputProps;   /* TM device and channel indexes */
   AdcTMThresholdType eThresholdTriggered;     /* Type of threshold that triggered */
   int32_t nPhysicalTriggered;                   /* Physical value that triggered */
} AdcTMCallbackPayloadType;

typedef enum
{
   ADC_FG_THRESH_SKIN_HOT,        /* Skin temperature hot threshold */
   ADC_FG_THRESH_SKIN_TOO_HOT,    /* Skin temperature too hot threshold */
   ADC_FG_THRESH_CHARGER_HOT,     /* Charger temperature hot threshold */
   ADC_FG_THRESH_CHARGER_TOO_HOT, /* Charger temperature too hot threshold */
   _ADC_FG_MAX_THRESH = 0x7FFFFFFF
} AdcFGThresholdType;

#endif
/* vim: set ts=3 sw=3 et: */
