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

#ifndef ZEPHYR_INCLUDE_LINKER_QC_H_
#define ZEPHYR_INCLUDE_LINKER_QC_H_

#ifdef CONFIG_QC_GLOBAL_SECTION_ALIGNMENT

#if CONFIG_QC_LMA_BASE > 0 && CONFIG_LLVM_USE_QCLD
#define VMA_LMA_OFFSET (DT_REG_ADDR(DT_CHOSEN(zephyr_sram)) - CONFIG_QC_LMA_BASE)
#define LMA_OPT AT (. - VMA_LMA_OFFSET)
#else
#define LMA_OPT 
#endif /* CONFIG_QC_LMA_BASE > 0 */

#undef SECTION_PROLOGUE
#define SECTION_PROLOGUE(name, options, align) \
    name options : LMA_OPT SUBALIGN(CONFIG_QC_GLOBAL_SECTION_ALIGNMENT)

#undef SECTION_DATA_PROLOGUE
#define SECTION_DATA_PROLOGUE(name, options, align) \
    name options : LMA_OPT SUBALIGN(CONFIG_QC_GLOBAL_SECTION_ALIGNMENT)

#endif /* CONFIG_QC_GLOBAL_SECTION_ALIGNMENT */

#endif /* ZEPHYR_INCLUDE_LINKER_QC_H_ */
