#pragma once
/** ============================================================================
 * @file
 *
 * @brief Stationary position indicator header file.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc.and/or its subsidiaries.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 ===========================================================================*/

#include <stdint.h>
#include "sns_circular_buffer.h"
#include "sns_fx_iir.h"

#define SPI_ACCEL_COLS (3)

/**
 * @brief Stationary position indicator states.
 *
 */
enum SPI_STATES
{
  SPI_UNKNOWN = 0,
  SPI_REST = 1,
  SPI_MOVING = 2
};

/**
 * @brief Stationary position indicator configuration.
 *
 */
typedef struct
{
  int32_t sample_rate;       /* !< Hz      (Q16) */
  int32_t detect_threshold;  /* !< accel, m/s/s, (Q16) */
  int32_t accel_window_time; /* !< seconds (Q16) */
  int32_t op_interval_time;  /* !< seconds (Q16) */
  int32_t filter_enabled;    /* !< boolean */
  int32_t fc;                /* !< filter cutoff frequency in Hz,
                              *    if enabled (Q16)
                              */
} spi_config_s;

/**
 * @brief Stationary position indicator input.
 *
 */
typedef struct
{
  int32_t a[SPI_ACCEL_COLS]; /*!< accel, m/s/s, (Q16) */
} spi_input_s;

/**
 * @brief Stationary position indicator output.
 *
 */
typedef struct
{
  int32_t spi_state;
  int32_t event;
} spi_output_s;

/**
 * @brief Stationary position indicator state.
 *
 */
typedef struct
{
  spi_config_s config;
  buffer_type buf;
  int32_t buf_sum[SPI_ACCEL_COLS];
  int32_t buf_sum_sq[SPI_ACCEL_COLS];
  int32_t spi_state;
  fx_iir_state_s *fx_iir_lpf;
  int32_t motion_timer;
  int32_t op_interval_samples;
  int32_t detect_threshold;
} spi_state_s;

/**
 * @brief Calculates size of memory.
 *
 * @param[in] config_data  Pointer to the algorithm's configuration data.
 *
 * @return 
 *  -int32_t: Size (in bytes) for stationary position indicator.
 *
 */
int32_t spi_mem_req(spi_config_s *config_data);

/**
 * @brief Initializes the stationary position indicator.
 *
 * @param[in] mem           Pointer to the algorithm's memory.
 * @param[in] config_data   Pointer to the algorithm's configuration data.
 *
 * @return
 *  - spi_state_s:          Pointer to the initialized algorithm's internal
 *                          state structure.
 *
 */
spi_state_s *spi_state_reset(spi_config_s *config_data, void *mem);

/**
 * @brief Updates the stationary position indicator with new inputs.
 *
 * @param[in] spi_algo    Pointer to the algorithm's internal state structure.
 * @param[in] input       Pointer to the algorithm's input structure.
 * @param[out] output     Pointer to the algorithm's output structure.
 *
 * @return
 *   - None.
 */
void spi_update(spi_state_s *spi_algo, spi_input_s *input,
                spi_output_s *output);
