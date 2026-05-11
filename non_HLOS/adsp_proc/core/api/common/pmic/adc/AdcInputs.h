#ifndef __ADC_INPUTS_H__
#define __ADC_INPUTS_H__
/*============================================================================
  @file AdcInputs.h

  Analog-to-Digital Converter driver standard ADC input names.

  This header provides macros to provide common names to access ADC input
  properties.

               Copyright (c) 2008-2020 Qualcomm Technologies, Inc.
               All Rights Reserved.
               Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.qdsp6/8.2.3/api/common/pmic/adc/AdcInputs.h#1 $ */

/** BATT_ID_OHMS - nPhysical units are in ohms */
#define ADC_INPUT_BATT_ID_OHMS            "BATT_ID_OHMS"

/** BATT_ID_OHMS_PU_30K - nPhysical units are in ohms */
#define ADC_INPUT_BATT_ID_OHMS_PU_30K     "BATT_ID_OHMS_PU_30K"

/** BATT_ID_OHMS_PU_400K - nPhysical units are in ohms */
#define ADC_INPUT_BATT_ID_OHMS_PU_400K    "BATT_ID_OHMS_PU_400K"

/** BATT_THERM - nPhysical units are in degrees C */
#define ADC_INPUT_BATT_THERM              "BATT_THERM"

/** BATT_THERM_PU_30K - nPhysical units are in degrees C */
#define ADC_INPUT_BATT_THERM_PU_30K       "BATT_THERM_PU_30K"

/** BATT_THERM_PU_400K - nPhysical units are in degrees C */
#define ADC_INPUT_BATT_THERM_PU_400K      "BATT_THERM_PU_400K"

/** CHG_TEMP - nPhysical units are in 0.001 degrees C  */
#define ADC_INPUT_CHG_TEMP                "CHG_TEMP"

/** HARDWARE_ID - nPhysical units are in millivolts */
#define ADC_INPUT_PMIC_HARDWARE_ID        "PMIC_HARDWARE_ID"

/** MSM_THERM - nPhysical units are in degrees C */
#define ADC_INPUT_MSM_THERM               "MSM_THERM"

/** PA_THERM - nPhysical units are in degrees C */
#define ADC_INPUT_PA_THERM                "PA_THERM"

/** PA_THERM1 - nPhysical units are in degrees C */
#define ADC_INPUT_PA_THERM1               "PA_THERM1"

/** PLATFORM_ID - nPhysical units are in millivolts */
#define ADC_INPUT_PLATFORM_ID             "PLATFORM_ID"

/** BOARD_ID - nPhysical units are in millivolts */
#define ADC_INPUT_BOARD_ID                "BOARD_ID"

/** SKU_ID - nPhysical units are in millivolts */
#define ADC_INPUT_SKU_ID                  "SKU_ID"

/** HW_REV_ID - nPhysical units are in millivolts */
#define ADC_INPUT_HW_REV_ID               "HW_REV_ID"

/** PMIC_THERM - nPhysical units are in 0.001 degrees C*/
#define ADC_INPUT_PMIC_THERM              "PMIC_THERM"

/** PMIC_TEMP1 (First PMIC's temp) - nPhysical units are in 0.001 degrees C */
#define ADC_INPUT_PMIC_TEMP1              ADC_INPUT_PMIC_THERM

/** PMIC_TEMP2 (Second PMIC's temp) - nPhysical units are in 0.001 degrees C */
#define ADC_INPUT_PMIC_TEMP2              "PMIC_TEMP2"

/** PMIC_TEMP3 (Third PMIC's temp) - nPhysical units are in 0.001 degrees C */
#define ADC_INPUT_PMIC_TEMP3              "PMIC_TEMP3"

/** USB_IN - nPhysical units are in millivolts */
#define ADC_INPUT_USB_IN                  "USB_IN"

/** VBATT - nPhysical units are in millivolts */
#define ADC_INPUT_VBATT                   "VBATT"

/** VBATT - nPhysical units are in millivolts */
#define ADC_INPUT_VBATT_2S                "VBATT_2S"

/** VBATT_2S_MID - nPhysical units are in millivolts */
#define ADC_INPUT_VBATT_2S_MID            "VBATT_2S_MID"

/** VBATT when GSM PA ON - nPhysical units are in millivolts */
#define ADC_INPUT_VBATT_GSM               "VBATT_GSM"

/** VBATT when flash ramp up is done - nPhysical units are in millivolts */
#define ADC_INPUT_VBATT_FLASH             "VBATT_FLASH"

/** IBAT */
#define ADC_INPUT_IBATT_MA                "IBATT_MA"

/** VCHG - nPhysical units are in millivolts */
#define ADC_INPUT_VCHG                    "VCHG"

/** VCOIN - nPhysical units are in millivolts */
#define ADC_INPUT_VCOIN                   "VCOIN"

/** VPH_PWR - nPhysical units are in millivolts */
#define ADC_INPUT_VPH_PWR                 "VPH_PWR"

/** XO_THERM - nPhysical units are in 2^-10 degrees C */
#define ADC_INPUT_XO_THERM                "XO_THERM"

/** XO_THERM_GPS_LOW - nPhysical units are in 2^-10 degrees C */
#define ADC_INPUT_XO_THERM_GPS_LOW        "XO_THERM_GPS_LOW"

/** XO_THERM_GPS_MED - nPhysical units are in 2^-10 degrees C */
#define ADC_INPUT_XO_THERM_GPS_MED        "XO_THERM_GPS_MED"

/** XO_THERM_GPS_HIGH - nPhysical units are in 2^-10 degrees C */
#define ADC_INPUT_XO_THERM_GPS_HIGH       "XO_THERM_GPS_HIGH"

/** XO_THERM_GPS - nPhysical units are in 2^-10 degrees C */
#define ADC_INPUT_XO_THERM_GPS            "XO_THERM_GPS"

/** XO_THERM_MV - nPhysical units are in millivolts */
#define ADC_INPUT_XO_THERM_MV             "XO_THERM_MV"

/** DC_IN - nPhysical units are in millivolts */
#define ADC_INPUT_DC_IN                   "DC_IN"

/** SYS_THERM1 - nPhysical units are in degrees C */
#define ADC_INPUT_SYS_THERM1              "SYS_THERM1"

/** SYS_THERM2 - nPhysical units are in degrees C */
#define ADC_INPUT_SYS_THERM2              "SYS_THERM2"

/** SYS_THERM3 - nPhysical units are in degrees C */
#define ADC_INPUT_SYS_THERM3              "SYS_THERM3"

/** SYS_THERM4 - nPhysical units are in degrees C */
#define ADC_INPUT_SYS_THERM4              "SYS_THERM4"

/** SYS_THERM5 - nPhysical units are in degrees C */
#define ADC_INPUT_SYS_THERM5              "SYS_THERM5"

/** USB_DATA - nPhysical units are in millivolts */
#define ADC_INPUT_USB_DATA                "USB_DATA"

/** USB_ID - nPhysical units are in ohms */
#define ADC_INPUT_USB_ID                  "USB_ID"

/** USB_DP - nPhysical units are in millivolts */
#define ADC_INPUT_USB_DP                  "USB_DP"

/** USB_DN - nPhysical units are in millivolts */
#define ADC_INPUT_USB_DN                  "USB_DN"

/** VSYS - nPhysical units are in millivolts */
#define ADC_INPUT_VSYS                    "VSYS"

/** SL_ID - nPhysical units are in millivolts */
#define ADC_INPUT_SL_ID                   "SL_ID"

/** V3P15V - nPhysical units are in millivolts */
#define ADC_INPUT_V3P15V                  "V3P15V"

/** CHG_TEMP - nPhysical units are in degree C */
#define ADC_INPUT_CHG_TEMP                "CHG_TEMP"

/** VWLS - nPhysical units are in millivolts  */
#define ADC_INPUT_VWLS                    "VWLS"

/** VWLS - nPhysical units are in millivolts  */
#define ADC_INPUT_WLS_THERM               "WLS_THERM"

/** CC1_ID - nPhysical units are in ohms  */
#define ADC_INPUT_CC1_ID_OHMS             "CC1_ID_OHMS"

/** SBU - nPhysical units are in millivolts  */
#define ADC_INPUT_SBU                     "SBU"

/*
 * The following channels are available on different PMICs,
 * and the macros have a parameter to allow the caller to
 * specify the PMIC. The "pmic" parameter has the form
 * PM_<suffix>, e.g. PM_A, PM_B, PM_C, etc.
 *
 * Note: keep in mind that these macros are expanded at build
 * time, so the "pmic" parameter cannot be a variable.
 *
 * Example: ADC_INPUT_DIE_TEMP(PM_A)
 */

/** DIE_TEMP(pmic) - nPhysical units are in milli degrees C */
#define ADC_INPUT_DIE_TEMP(pmic)          "DIE_TEMP_" #pmic

/** CONN_THERM(pmic) - nPhysical units are in degrees C */
#define ADC_INPUT_CONN_THERM(pmic)        "CONN_THERM_" #pmic

/** USB_IN_V_MV(pmic) - nPhysical units are in millivolts */
#define ADC_INPUT_USB_IN_V_MV(pmic)       "USB_IN_V_MV_" #pmic

/** USB_IN_I_MA(pmic) - nPhysical units are in milliamps */
#define ADC_INPUT_USB_IN_I_MA(pmic)       "USB_IN_I_MA_" #pmic

#endif

