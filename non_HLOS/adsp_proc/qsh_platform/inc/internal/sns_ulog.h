#pragma once
/** ===========================================================================
 * @file
 *
 * @brief uLog wrapper.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

#ifdef SNS_ULOG_ENABLE

#include "sns_types.h"
#include "micro_ULog.h"
#include "ULogFront.h"

/*=============================================================================
  Macros
  ===========================================================================*/
typedef enum
{
  SNS_ULOG_ID_INIT_SEQ,
  SNS_ULOG_ID_FW_DBG,
  SNS_ULOG_ID_I2C_DBG,
  SNS_ULOG_ID_THREADS,
  // add more IDs as necessary
  SNS_ULOG_ID_MAX
} sns_ulog_id;

/* SNS_ULOG_ADD() adds messages to the uLog file associated with 'ulog_id' */
#define SNS_ULOG_ADD(ulog_id, ...)                                             \
  do                                                                           \
  {                                                                            \
    if(ulog_id < ARR_SIZE(sns_ulogs) && NULL != sns_ulogs[ulog_id].handle)     \
    {                                                                          \
      if(!sns_ulogs[ulog_id].island)                                           \
        ULogFront_RealTimePrintf(sns_ulogs[ulog_id].handle,                    \
                                 NARGS(__VA_ARGS__), __VA_ARGS__);             \
      else                                                                     \
        micro_ULog_RealTimePrintf(sns_ulogs[ulog_id].handle,                   \
                                  NARGS(__VA_ARGS__), __VA_ARGS__);            \
    }                                                                          \
  } while(0)

/* Short cut to add messages to the uLog file associated with
 * SNS_ULOG_ID_INIT_SEQ */
#define SNS_ULOG_INIT_SEQ(...) SNS_ULOG_ADD(SNS_ULOG_ID_INIT_SEQ, __VA_ARGS__)
#define SNS_ULOG_INIT_SEQ_ERR(...)                                             \
  SNS_ULOG_ADD(SNS_ULOG_ID_INIT_SEQ, __VA_ARGS__)

/*=============================================================================
  Structures
  ===========================================================================*/
typedef void *sns_ulog_handle;
typedef int32_t sns_ulog_result;

typedef struct
{
  bool island;    // true to create log in island
  sns_ulog_id id; // one of SNS_ULOG_ID_*
  char *name;     // must be at most 24-char long (including NULL)
  uint32_t size;  // should be power of 2; ULog buffer is provided by Core,
                  // micro Ulog buffer is statically allocated in sns_ulog
  sns_ulog_handle handle; // value returned from ULogFront_RealTimeInit()
  sns_ulog_result result; // result of last call to uLog/micro uLog
} sns_ulog_type;

/*=============================================================================
  Data
  ===========================================================================*/
extern sns_ulog_type sns_ulogs[SNS_ULOG_ID_MAX];

#else // !SNS_ULOG_ENABLE

#include "sns_assert.h"

#define SNS_ULOG_ADD(ulog_id, ...)
#define SNS_ULOG_INIT_SEQ(...)
#define SNS_ULOG_INIT_SEQ_ERR(...) SNS_VERBOSE_ASSERT(false, __VA_ARGS__)

#endif

/*=============================================================================
  Functions
  ===========================================================================*/
void sns_ulog_init(void);
