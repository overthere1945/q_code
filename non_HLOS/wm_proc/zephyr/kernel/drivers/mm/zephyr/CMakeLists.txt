# ------------------------------------------------------------------------------
#  Copyright (c) Qualcomm Technologies, Inc.
#  All rights reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
# ------------------------------------------------------------------------------

if (CONFIG_QC_MM_EAT)
  zephyr_include_directories(${ZEPHYR_CORE_ZEPHYR_MODULE_DIR}/include)
  zephyr_library()
  zephyr_library_sources(../mm_drv_qcom_eat.c)
  if (CONFIG_DT_HAS_QCOM_RVCP_REMAPPER_ENABLED OR CONFIG_DT_HAS_QCOM_RVCP_REMAPPER_V3_ENABLED)
    zephyr_library_sources(../mm_drv_qcom_rvcp_remapper.c)
  endif()
endif()
