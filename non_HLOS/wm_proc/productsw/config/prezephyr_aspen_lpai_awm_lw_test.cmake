# ------------------------------------------------------------------------------
#  Copyright (c) Qualcomm Technologies, Inc.
#  All rights reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
# ------------------------------------------------------------------------------

list(APPEND DTS_ROOT ${PROD_SW_ROOT}/rsb_service/config)
list(APPEND DTC_OVERLAY_FILE ${PROD_SW_ROOT}/rsb_service/config/dts/pat9401e1.overlay)

list(APPEND DTS_ROOT ${PROD_SW_ROOT}/debug/dt_settings)
list(APPEND DTC_OVERLAY_FILE ${PROD_SW_ROOT}/debug/dt_settings/soc/aspen_lpai_awm/sw_diag.dtsi)

list(APPEND DTS_ROOT ${PROD_SW_ROOT}/psram/config)
list(APPEND DTC_OVERLAY_FILE ${PROD_SW_ROOT}/psram/config/dts/soc/aspen_lpai_awm/psram_partition.overlay)
list(APPEND DTC_OVERLAY_FILE ${PROD_SW_ROOT}/debug/dt_settings/soc/aspen_lpai_awm/qsr_partition.overlay)

set(PSRAM_PARTITION_LD_FILE ${PROD_SW_ROOT}/psram/psram_partition_awm.ld)

set(QSR_PARTITION_LD_FILE ${PROD_SW_ROOT}/debug/dt_settings/qsr_partition_awm.ld)
