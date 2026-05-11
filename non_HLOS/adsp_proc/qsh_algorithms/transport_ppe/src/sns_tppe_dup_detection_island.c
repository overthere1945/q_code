/*=============================================================================
  @file sns_tppe_dup_detection_island.c

  Contains function definitions required for duplicate detection and filtering

  Copyright (c) 2022-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include "sns_memory_service.h"
#include "sns_time.h"
#include "sns_tppe_crc_cache.h"
#include "sns_tppe_dup_detection.h"
#include "sns_tppe_messages.h"
#include <stdint.h>

#define SNS_TPPE_ONE_SECOND_NS (1000000000ULL)
/*=============================================================================
  Static functions
  ===========================================================================*/
static bool u32_array_contains(uint32_t element, uint32_t *array, size_t len)
{
  int i;
  for(i = 0; i < len && array[i] != element; i++)
    ;

  return i < len;
}

static bool is_system_in_same_period(sns_time cur_time, sns_single_trigger *trigger)
{

  sns_time dup_detect_period_ticks =
      sns_convert_ns_to_ticks(trigger->dup_detect_period * SNS_TPPE_ONE_SECOND_NS);
  bool retval = false;
  if(trigger->dup_detect_period > 0)
  {

    retval = trigger->dup_detect_period_start_ts + dup_detect_period_ticks > cur_time;
  }
  return retval;
}

static void set_start_time_for_trigger(sns_time cur_time, sns_single_trigger *trigger)
{
  sns_time dup_detect_period_ticks =
      sns_convert_ns_to_ticks(trigger->dup_detect_period * SNS_TPPE_ONE_SECOND_NS);
  if(trigger->dup_detect_period > 0)
  {
    while(trigger->dup_detect_period_start_ts + dup_detect_period_ticks < cur_time)
    {
      trigger->dup_detect_period_start_ts =
          trigger->dup_detect_period_start_ts + dup_detect_period_ticks;
    }
  }
}
/*=============================================================================
 Public functions
  ===========================================================================*/

/**
 * loop through all triggers
 * if wakeup triggers and dup triggers contains this trigger
 * check if an event with same crc is already seen
 * if yes, then decrement the effective_trigger_count
 * After passing through all the triggers if effective trigger count > 0
 * the event is a new event and should be sent out
 **/
bool duplicate_filtering_algo(sns_single_trigger *triggers, size_t trigger_count,
                              sns_tppe_crc_cache *crc_cache, sns_tppe_event_trigger *event_triggers)
{
  uint32_t trigger_id = 0;
  uint32_t crc_index = ARRAY_ITEM_INDEX_NOT_FOUND;
  sns_time cur_time = event_triggers->event_ts;
  int32_t effective_trigger_count = event_triggers->trigger_id_count;
  if(event_triggers->crc == 0)
  {
    return false; // cannot perform duplicate detection if crc = 0
  }
  for(int i = 0; i < trigger_count; i++)
  {
    trigger_id = triggers[i].trigger_id;

    if(!is_system_in_same_period(cur_time, &triggers[i]))
    {
      // Dup period for this trigger has expired
      // Clear cache for this trigger
      // Setup start ts to proper time
      sns_tcc_clear_cache_for_trigger(crc_cache, i);
      set_start_time_for_trigger(cur_time, &triggers[i]);
    }
    if(u32_array_contains(trigger_id, event_triggers->trigger_ids,
                          event_triggers->trigger_id_count) &&
       u32_array_contains(trigger_id, event_triggers->dup_trigger_ids,
                          event_triggers->dup_trigger_id_count))
    {
      if(crc_index == ARRAY_ITEM_INDEX_NOT_FOUND)
      {
        // uninitialized
        crc_index = sns_tcc_get_crc_index(crc_cache, event_triggers->crc);
        if(crc_index == ARRAY_ITEM_INDEX_NOT_FOUND)
        {
          // new crc, insert into the table
          crc_index = sns_tcc_insert_crc(crc_cache, event_triggers->crc);
        }
      }
      if(sns_tcc_is_trigger_set(crc_cache, crc_index, i))
      {
        effective_trigger_count--;
      }
      else
      {
        sns_tcc_set_trigger(crc_cache, crc_index, i);
      }
    }
  }
  return effective_trigger_count <= 0;
}

bool is_duplicate_event(sns_sensor_instance *const this)
{
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;

  bool retval = false;
  if(inst_state->dup_detection_required)
  {
    retval = duplicate_filtering_algo(inst_state->all_filters.wakeup_triggers,
                                      inst_state->all_filters.trigger_count, &inst_state->crc_cache,
                                      &inst_state->event_triggers);
  }
  return retval;
}
