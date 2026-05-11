/** ============================================================================
 * @file
 *
 * @brief Contains function definitions required for duplicate detection 
 * and filtering.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 * ===========================================================================*/

#include "sns_diag_service.h"
#include "sns_math_ops.h"
#include "sns_memory_service.h"
#include "sns_pb_util.h"
#include "sns_std.pb.h"
#include "sns_tppe_debug_dump_util.h"
#include <stdarg.h>
#include <stdio.h>
#include "sns_service_manager.h"

#define DDU_ALREADY_INITIALIZED(thiz, inst) ((thiz->instance == inst) && (thiz->buffer))

/*============================================================================
  static functions
===========================================================================*/
/**
1. encode request
2. send event
**/
sns_rc sns_ddu_publish_resp(sns_debug_dump_util *thiz, bool complete)
{
  bool rc = false;

  sns_std_debug_resp resp_event = sns_std_debug_resp_init_default;
  resp_event.has_partial_resp = true;
  resp_event.partial_resp = !complete;
  pb_buffer_arg payload_arg = {.buf = thiz->buffer, .buf_len = thiz->cur_index};

  resp_event.debug_string.funcs.encode = &pb_encode_string_cb;
  resp_event.debug_string.arg = &payload_arg;

  rc = pb_send_event((sns_sensor_instance *)thiz->instance, sns_std_debug_resp_fields, &resp_event,
                     sns_get_system_time(), SNS_STD_MSGID_SNS_STD_DEBUG_RESP, NULL);

  return rc ? SNS_RC_SUCCESS : SNS_RC_FAILED;
}

/*============================================================================
  public functions
===========================================================================*/
/**
1. update structure members
2. get max supported size by the system
3. allocate buffer for this size on heap
**/
sns_rc sns_ddu_init(sns_debug_dump_util *thiz, sns_sensor_instance *const instance)
{

  if(!thiz || !instance || DDU_ALREADY_INITIALIZED(thiz, instance))
  {
    return SNS_RC_FAILED;
  }

  thiz->instance = instance;
  thiz->cur_index = 0;
  thiz->max_buffer_len = SNS_DDU_MAX_DUMP_STR_LEN;
  sns_service_manager *manager = instance->cb->get_service_manager(instance);
  sns_memory_service *memory_service =
      (sns_memory_service *)manager->get_service(manager, SNS_MEMORY_SERVICE);
  sns_diag_service *diag_service =
      (sns_diag_service *)manager->get_service(manager, SNS_DIAG_SERVICE);
  size_t max_packet_size = diag_service->api->get_max_log_size(diag_service);
  if(max_packet_size)
  {
    thiz->max_buffer_len = SNS_MIN(max_packet_size, SNS_DDU_MAX_DUMP_STR_LEN);
  }
  thiz->buffer = memory_service->api->malloc(instance, thiz->max_buffer_len);

  return (thiz->buffer) ? SNS_RC_SUCCESS : SNS_RC_FAILED;
}

/**
1. Try to insert the string into buffer
2. If it is overflowing the buffer, send the existing buffer to client as a
   partial event.
3. Copy the string to the buffer.
4. If string is bigger than supported packet size to begin with, return not
supported. This is a rare case scenerio, maybe implement a function in future to
break into smaller packets
**/
sns_rc sns_ddu_dump_str(sns_debug_dump_util *thiz, const char *const format, ...)
{
  size_t length = 0;
  sns_rc rc = SNS_RC_SUCCESS;
  va_list args;

  if(!thiz || !thiz->buffer)
  {
    rc = SNS_RC_FAILED;
  }
  else
  {
    va_start(args, format);
    length = vsnprintf(&thiz->buffer[thiz->cur_index], thiz->available_size, format, args);
    va_end(args);
    if(length <= thiz->max_buffer_len)
    {
      if(length + thiz->cur_index >= thiz->max_buffer_len)
      {
        sns_ddu_publish_resp(thiz, false);
        thiz->cur_index = 0;
        thiz->available_size = thiz->max_buffer_len;
        va_start(args, format);
        length = vsnprintf(&thiz->buffer[thiz->cur_index], thiz->available_size, format, args);
        va_end(args);
      }
      thiz->cur_index += length;
      thiz->available_size = thiz->max_buffer_len - thiz->cur_index;
    }
  }
  return rc;
}
/**
1. If all data in the buffer is not sent already, send it
2. Free the buffer
3. Deinit is the only function that sends a complete event. This forces
   the user to call deinit.
**/
void sns_ddu_deinit(sns_debug_dump_util *thiz)
{
  if(thiz->cur_index || (!thiz->cur_index && thiz->buffer))
  {
    sns_ddu_publish_resp(thiz, true);
    thiz->cur_index = 0;
  }
  if(thiz->buffer)
  {
    sns_service_manager *manager =
        thiz->instance->cb->get_service_manager((sns_sensor_instance *)thiz->instance);
    sns_memory_service *memory_service =
        (sns_memory_service *)manager->get_service(manager, SNS_MEMORY_SERVICE);
    memory_service->api->free((sns_sensor_instance *)thiz->instance, thiz->buffer);
    thiz->buffer = NULL;
  }
}
