# ------------------------------------------------------------------------------
#  Copyright (c) Qualcomm Technologies, Inc.
#  All rights reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
# ------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/prezephyr.cmake)

list(APPEND DTS_ROOT ${PROD_SW_ROOT}/debug/dt_settings)
list(APPEND DTC_OVERLAY_FILE ${PROD_SW_ROOT}/debug/dt_settings/soc/aspen_lpai_swm/sw_diag.dtsi)

list(APPEND DTS_ROOT ${PROD_SW_ROOT}/psram/config)
list(APPEND DTC_OVERLAY_FILE ${PROD_SW_ROOT}/psram/config/dts/soc/aspen_lpai_swm/psram_partition.overlay)
list(APPEND DTC_OVERLAY_FILE ${PROD_SW_ROOT}/debug/dt_settings/soc/aspen_lpai_swm/qsr_partition.overlay)
list(APPEND DTC_OVERLAY_FILE ${PROD_SW_ROOT}/debug/dt_settings/soc/aspen_lpai_swm/diag_buffer_mode_config.overlay)

set(PSRAM_PARTITION_LD_FILE ${PROD_SW_ROOT}/psram/psram_partition_swm.ld)


set(QSR_PARTITION_LD_FILE ${PROD_SW_ROOT}/debug/dt_settings/qsr_partition_swm.ld)

