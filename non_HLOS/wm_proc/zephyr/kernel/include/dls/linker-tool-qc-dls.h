/*
 * Copyright (c) Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.

 * @file
 * @brief QC linker defs for DLS memory
 *
 * This header file undefines and redefines necessary macros used by 
 * the linker scripts, allowing use of existing Zephyr linker scripts, with QC
 * specific image requirements
 */

#if defined(CONFIG_QC_DLS)
#undef GROUP_LINK_IN
#define GROUP_LINK_IN(where) > where AT > ROMABLE_REGION

#undef GROUP_DATA_LINK_IN
#define GROUP_DATA_LINK_IN(vregion, lregion) > vregion AT > ROMABLE_REGION

#undef SECTION_PROLOGUE
#define SECTION_PROLOGUE(name, options, align) \
    name options : ALIGN(CONFIG_QC_DLS_SECTION_ALIGNMENT)

#undef SECTION_DATA_PROLOGUE
#define SECTION_DATA_PROLOGUE(name, options, align) \
    name options : ALIGN(CONFIG_QC_DLS_SECTION_ALIGNMENT)

#endif /* CONFIG_QC_DLS */

