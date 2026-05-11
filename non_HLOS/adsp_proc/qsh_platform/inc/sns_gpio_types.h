#pragma once
/** ============================================================================
 * @file
 *
 * @brief GPIO types
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/

/*=============================================================================
  Includes
  ===========================================================================*/

/*=============================================================================
  Type Definitions
  ===========================================================================*/

/**
 * @brief Types of GPIO pin drive strength.
 *
 */
typedef enum
{
  SNS_GPIO_DRIVE_STRENGTH_2_MILLI_AMP = 0,  /*!< Specify a 2 mA drive.  */
  SNS_GPIO_DRIVE_STRENGTH_4_MILLI_AMP = 1,  /*!< Specify a 4 mA drive.  */
  SNS_GPIO_DRIVE_STRENGTH_6_MILLI_AMP = 2,  /*!< Specify a 6 mA drive.  */
  SNS_GPIO_DRIVE_STRENGTH_8_MILLI_AMP = 3,  /*!< Specify an 8 mA drive. */
  SNS_GPIO_DRIVE_STRENGTH_10_MILLI_AMP = 4, /*!< Specify a 10 mA drive. */
  SNS_GPIO_DRIVE_STRENGTH_12_MILLI_AMP = 5, /*!< Specify a 12 mA drive. */
  SNS_GPIO_DRIVE_STRENGTH_14_MILLI_AMP = 6, /*!< Specify a 14 mA drive. */
  SNS_GPIO_DRIVE_STRENGTH_16_MILLI_AMP = 7  /*!< Specify a 16 mA drive. */
} sns_gpio_drive_strength;

/**
 * @brief Types of GPIO pin pull.
 *
 */
typedef enum
{
  SNS_GPIO_PULL_TYPE_NO_PULL = 0,   /*!< Do not specify a pull. */
  SNS_GPIO_PULL_TYPE_PULL_DOWN = 1, /*!< Pull the GPIO down.    */
  SNS_GPIO_PULL_TYPE_KEEPER = 2,    /*!< Designate as a Keeper. */
  SNS_GPIO_PULL_TYPE_PULL_UP = 3    /*!< Pull the pin up.       */
} sns_gpio_pull_type;

/**
 * @brief GPIO state enum.
 *
 */
typedef enum
{
  SNS_GPIO_STATE_LOW, /*!< GPIO state low. */
  SNS_GPIO_STATE_HIGH /*!< GPIO state high. */
} sns_gpio_state;
