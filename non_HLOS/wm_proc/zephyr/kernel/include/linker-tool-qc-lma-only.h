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

#ifndef ZEPHYR_INCLUDE_LINKER_QC_LMA_ONLY_H_
#define ZEPHYR_INCLUDE_LINKER_QC_LMA_ONLY_H_

#ifdef VMA_LMA_OFFSET
#undef VMA_LMA_OFFSET
#endif
#define VMA_LMA_OFFSET (DT_REG_ADDR(DT_CHOSEN(zephyr_sram)) - CONFIG_QC_LMA_BASE)

#if CONFIG_QC_LMA_BASE > 0
#undef SECTION_PROLOGUE
#define SECTION_PROLOGUE(name, options, align) \
    name options : AT (. - VMA_LMA_OFFSET) align

#undef SECTION_DATA_PROLOGUE
#define SECTION_DATA_PROLOGUE(name, options, align) \
    name options : AT (. - VMA_LMA_OFFSET) align
#endif /* CONFIG_QC_LMA_BASE > 0 */

#endif /* ZEPHYR_INCLUDE_LINKER_QC_LMA_ONLY_H_ */
