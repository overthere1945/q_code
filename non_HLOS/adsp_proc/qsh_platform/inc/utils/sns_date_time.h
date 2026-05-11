#pragma once
/** =============================================================================
 * @file sns_date_time.h
 *
 * @brief
 * SNS date and time implementation
 *
 * @copyright
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *  ===========================================================================
 */

/*=============================================================================
  Include Files
  ===========================================================================*/
#include <stdbool.h>

#include "sns_rc.h"
#include "sns_time.h"

/*=============================================================================
  Macro definitions
  ===========================================================================*/
#define SECONDS_TO_US_MULTIPLIER 1000000ULL
#define SECONDS_TO_NS_MULTIPLIER 1000000000LL

#define SECONDS_PER_MINUTE    60
#define SECONDS_PER_HOUR      3600
#define MINUTES_PER_HOUR      60
#define SECONDS_PER_DAY       86400
#define DAYS_IN_NON_LEAP_YEAR 365
#define DAYS_IN_LEAP_YEAR     366

/*============================================================================
  Structure Definitions and Typedefs
  ===========================================================================*/
/**
 * @brief Days of the week.
 */
typedef enum sns_day_of_week
{
  SNS_MONDAY = 0,
  SNS_TUESDAY,
  SNS_WEDNESDAY,
  SNS_THURSDAY,
  SNS_FRIDAY,
  SNS_SATURDAY,
  SNS_SUNDAY
} sns_day_of_week;

/**
 * @brief Calendar date and time structure.
 */
typedef struct sns_calendar_date_time
{
  uint16_t year;               /**< Year [1970 through 65,535]. */
  int8_t month;                /**< Month of year [1 through 12]. */
  int8_t day;                  /**< Day of month [1 through 31]. */
  int8_t hour;                 /**< Hour of day [0 through 23]. */
  int8_t minute;               /**< Minute of hour [0 through 59]. */
  int8_t second;               /**< Second of minute [0 through 60]. */
  sns_day_of_week day_of_week; /**< Day of the week [0 through 6] or [Monday
                      through Sunday]. */
} sns_calendar_date_time;

/*=============================================================================
  Function Declarations
  ===========================================================================*/
/**
 * @brief Function to convert seconds and time zone offset to the calendar date
 * and time.
 *
 * @param[in]  utc_time_in_seconds UTC time in seconds since 1/1/1970.
 * @param[in]  time_zone_offset Time zone offset minutes.
 * @param[out] calendar_date_time Buffer to store calendar date and time.
 *
 * @return sns_rc SNS_RC_SUCCESS == Success, any other value == Failure
 */
sns_rc sns_date_time_convert_seconds_to_calendar_date_time(
    uint64_t utc_time_in_seconds, int16_t time_zone_offset,
    sns_calendar_date_time *calendar_date_time);
