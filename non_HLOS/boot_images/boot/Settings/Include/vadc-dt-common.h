#ifndef __VADC_DT_COMMON_H__
#define __VADC_DT_COMMON_H__

/*============================================================================
  FILE:         vadc-dt-common.h

  OVERVIEW:     Prototypes for VADC DT common bindings

  DEPENDENCIES: None

  Copyright (c) Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/
#define   VADC_DECIMATION_RATIO_256  0
#define   VADC_DECIMATION_RATIO_512  1
#define   VADC_DECIMATION_RATIO_1024 2

#define   VADC_AVERAGE_1_SAMPLE      0
#define   VADC_AVERAGE_2_SAMPLES     1
#define   VADC_AVERAGE_4_SAMPLES     2
#define   VADC_AVERAGE_8_SAMPLES     3
#define   VADC_AVERAGE_16_SAMPLES    4

#define   VADC_SETTLING_DELAY_0_US    0
#define   VADC_SETTLING_DELAY_100_US  1
#define   VADC_SETTLING_DELAY_200_US  2
#define   VADC_SETTLING_DELAY_300_US  3
#define   VADC_SETTLING_DELAY_400_US  4
#define   VADC_SETTLING_DELAY_500_US  5
#define   VADC_SETTLING_DELAY_600_US  6
#define   VADC_SETTLING_DELAY_700_US  7
#define   VADC_SETTLING_DELAY_800_US  8
#define   VADC_SETTLING_DELAY_900_US  9
#define   VADC_SETTLING_DELAY_1_MS    10
#define   VADC_SETTLING_DELAY_2_MS    11
#define   VADC_SETTLING_DELAY_4_MS    12
#define   VADC_SETTLING_DELAY_6_MS    13
#define   VADC_SETTLING_DELAY_8_MS    14
#define   VADC_SETTLING_DELAY_10_MS   15
#define   VADC_SETTLING_DELAY_16_MS   16
#define   VADC_SETTLING_DELAY_32_MS   17
#define   VADC_SETTLING_DELAY_64_MS   18
#define   VADC_SETTLING_DELAY_128_MS  19

#define   VADC_CAL_METHOD_NO_CAL             0
#define   VADC_CAL_METHOD_RATIOMETRIC        1
#define   VADC_CAL_METHOD_ABSOLUTE           2
#define   VADC_CAL_METHOD_RATIOMETRIC_GEN4   3
#define   VADC_CAL_METHOD_ABS_CURRENT_SIGNED VADC_CAL_METHOD_ABSOLUTE
#define   VADC_CAL_METHOD_ABS_CURRENT        0x0102
#define   VADC_CAL_METHOD_ABS_TEMPERATURE    0x0202

#define   VADC_SCALE_TO_MILLIVOLTS                  0
#define   VADC_SCALE_DIE_TEMP_TO_MILLIDEGREES       1
#define   VADC_SCALE_CHG_TEMP_TO_MILLIDEGREES       2
#define   VADC_SCALE_CHG_TEMP_TO_MILLIDEGREES_V2    3
#define   VADC_SCALE_INTERPOLATE_FROM_MILLIVOLTS    4
#define   VADC_SCALE_THERMISTOR                     5
#define   VADC_SCALE_RESISTOR_DIVIDER               6
#define   VADC_SCALE_INTERPOLATE_FROM_MICROVOLTS    7
#define   VADC_SCALE_CHG_TEMP_TO_MILLIDEGREES_V3    8

#define   VADC_SDAM_ASID(bus_id, sid)   ((bus_id << 5) | sid)

#endif
