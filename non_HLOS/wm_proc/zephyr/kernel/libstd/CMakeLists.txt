#---------------------------------------------------------------------------
# Copyright (c) Qualcomm Technologies, Inc.
# All Rights Reserved.
# Confidential and Proprietary - Qualcomm Technologies, Inc.
#---------------------------------------------------------------------------

zephyr_library()

zephyr_library_sources_ifdef(CONFIG_QC_LIBSTD src/memscpy.c
    src/memsmove.c
    src/strlcpy.c
    src/strlcat.c
    src/secure_memset.c)

zephyr_include_directories_ifdef(CONFIG_QC_LIBSTD inc/stringl inc)