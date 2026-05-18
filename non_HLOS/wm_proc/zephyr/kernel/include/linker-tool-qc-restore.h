/*
 * Copyright (c) Qualcomm Technologies, Inc.
 */

/**
 * @file
 * @brief QC linker defs
 *
 * This header file undefines and redefines necessary macros used by 
 * the linker scripts, allowing use of existing Zephyr linker scripts, with QC
 * specific image requirements
 */

#ifndef ZEPHYR_INCLUDE_LINKER_QC_RESTORE_H_
#define ZEPHYR_INCLUDE_LINKER_QC_RESTORE_H_

#undef SECTION_PROLOGUE
#define SECTION_PROLOGUE(name, options, align) \
    name options : align

#undef SECTION_DATA_PROLOGUE
#define SECTION_DATA_PROLOGUE(name, options, align) \
    name options : align

#endif /* ZEPHYR_INCLUDE_LINKER_QC_RESTORE_H_ */
