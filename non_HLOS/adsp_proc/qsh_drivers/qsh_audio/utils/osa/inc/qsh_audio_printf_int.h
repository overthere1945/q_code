#pragma once
/*=============================================================================
 * @file sns_printf_int.h
 *
 * Internal Printf support.
 *
 * Copyright (c) 2021 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/


#ifdef QSH_AUDIO_ALLOW_SENSOR_FWK_PRINT

/*=============================================================================
  Include Files
  ===========================================================================*/

#include "sns_printf.h"

/*=============================================================================
  Dependencies
  ===========================================================================*/

extern sns_sensor *sns_fw_printf;

#else

#undef FARF_LOW
#define FARF_LOW 1
#undef FARF_MED
#define FARF_MED 1
#undef FARF_HIGH
#define FARF_HIGH 1
#undef FARF_ERROR
#define FARF_ERROR 1
#undef FARF_FATAL
#define FARF_FATAL 1


#include "HAP_farf.h"

#define SNS_PRINTF(prio, sns_fw_printf, fmt, ...) FARF(prio,fmt,##__VA_ARGS__)

#endif // QSH_AUDIO_ALLOW_SENSOR_FWK_PRINT





