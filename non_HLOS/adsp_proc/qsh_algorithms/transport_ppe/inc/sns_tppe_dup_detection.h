#pragma once
/*=============================================================================
  @file sns_tppe_dup_detection.h

  Contains function declarations required for duplicate detection and filtering

  Copyright (c) 2022-23 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/
#include "sns_tppe_sensor_instance.h"

#define TPPE_DUP_DETECT_CRC_SIZE sizeof(uint32_t)

/**
 * @name duplicate_filtering_algo
 * @param [i] triggers : Array of all the triggers set by user
 * @param [i] trigger_count : Number of elements in triggers array
 * @param [i] crc_cache : cache of all CRCs sentout from this instance
 * @param [i] event_triggers: Filter results & metadata for this event
 * @return bool : True if event is classified as duplicate
 * @brief
 * Loop through all triggers
 * If trigger Id is present in wakeup trigger,duplicate triggers and its
 * crc cache has the event crc. It is classified as duplicate for this trigger.
 * decrement effective trigger count for this trigger
 * Once all triggers are done, if event has 0, effective trigger count,
 * it will be dropped
 *
 **/

bool duplicate_filtering_algo(sns_single_trigger *triggers, size_t trigger_count,
                              sns_tppe_crc_cache *crc_cache,
                              sns_tppe_event_trigger *event_triggers);
/**
 * @name is_duplicate_event
 * @param [i] this : sensor instance pointer
 * @return bool : True if event is classified duplicate
 * @brief
 **/
bool is_duplicate_event(sns_sensor_instance *const this);