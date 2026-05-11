# ==============================================================================
#                 Copyright (c) 2025 Qualcomm Technologies, Inc.
#                                All Rights Reserved.
#                 Confidential and Proprietary - Qualcomm Technologies, Inc.
# ==============================================================================*/
# $QTI_LICENSE_C$

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

if(CONFIG_QC_PDTSW_UTILITY_GLINK_ENABLE)
  set(PDTSW_UTILITY_GLINK_DEFS inc)
  set(PDTSW_UTILITY_GLINK_API ${CONFIG_SOC}/inc)
  if(CONFIG_QC_PDTSW_PSRAM_ARBITER_ENABLE)
    set(PDTSW_PSRAM_ARBITER_API ${PROD_SW_ROOT}/psram/psram_arbiter/inc)
  endif()
  if(CONFIG_QC_PDTSW_PSRAM_CLIENT_ENABLE)
    set(PDTSW_PSRAM_ARBITER_API ${PROD_SW_ROOT}/psram/psram_client/inc)
  endif()

  set(PDTSW_UTILITY_GLINK_SRC
    ${CONFIG_SOC}/src/pdtsw_utility_glink.c
  )

  zephyr_library_sources(${PDTSW_UTILITY_GLINK_SRC})
  
  zephyr_include_directories(
    ${PDTSW_UTILITY_GLINK_API}
    ${PDTSW_UTILITY_GLINK_DEFS}
    ${PDTSW_PSRAM_ARBITER_API}
  )
else()
  message(STATUS "PDTSW: Utility GLINK is disabled")
endif()