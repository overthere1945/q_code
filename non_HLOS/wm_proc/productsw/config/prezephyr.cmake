# ------------------------------------------------------------------------------
#  Copyright (c) Qualcomm Technologies, Inc.
#  All rights reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
# ------------------------------------------------------------------------------

set(PROD_SW_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(PDTSW_COMMON_API ${PROD_SW_ROOT}/common)

list(APPEND ZEPHYR_EXTRA_MODULES ${PROD_SW_ROOT})

if("${CMAKE_BINARY_DIR}" MATCHES "lpai_awm_img")
    qc_get_config_files(${CMAKE_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/config.yml PDTSW_CFGS)

    if(NOT "${PDTSW_CFGS}" STREQUAL "")
        list(LENGTH PDTSW_CFGS _cfg_count)
        if(NOT _cfg_count EQUAL 2)
            message(FATAL_ERROR "PDTSW_CFGS: Expected exactly 2 files, got ${_cfg_count}")
        endif()

        list(GET PDTSW_CFGS 0 _pdtsw_conf)
        list(GET PDTSW_CFGS 1 _pdtsw_cmake)

        # Validate file extensions
        if(NOT _pdtsw_conf MATCHES "\\.conf$")
            message(FATAL_ERROR "PDTSW_CFGS: 1st file must be a .conf, got: ${_pdtsw_conf}")
        endif()
        if(NOT _pdtsw_cmake MATCHES "\\.cmake$")
            message(FATAL_ERROR "PDTSW_CFGS: 2nd file must be a .cmake, got: ${_pdtsw_cmake}")
        endif()

        message(STATUS "PDTSW CONF: ${_pdtsw_conf}")
        message(STATUS "PDTSW CMAKE: ${_pdtsw_cmake}")

        list(APPEND OVERLAY_CONFIG "${_pdtsw_conf}")
        include("${_pdtsw_cmake}")
    else()
        message(FATAL_ERROR "PDTSW_CFGS is empty")
    endif()
endif()

# Adds wm_proc/app/test component to the build
set(LPAI_APP_TEST ${CMAKE_CURRENT_LIST_DIR}/../../app/test)
list(APPEND ZEPHYR_EXTRA_MODULES ${LPAI_APP_TEST})
