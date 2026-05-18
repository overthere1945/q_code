/**
 * @file
 * @brief Qualcomm Memory Management Driver RVCP Remapper Driver
 *
 * This file provides Qualcomm support for external address translations
 * using the RVCP hardware remapper. It reads the remapper at boot and
 * determines the external address translations in use.
 * 
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>

#include <drivers/mm/system_mm_qcom.h>


/*
 * This structure captures the remapper configuration from DT.
 * addr: The address of the first remapper register.
 * reg_count: The number of remapper registers.
 * reg_step: The step size in bytes between remapper registers.
 * mem_addr: The base address of the local region that this remapper remaps.
 * mem_size: The size in bytes of the remapped region.
 */
struct rvcp_remapper_config {
    uintptr_t addr;
    uint32_t  reg_count;
    size_t    reg_step;
    uintptr_t mem_addr;
    size_t    mem_size;
};

#define RVCP_REMAPPER_ENABLE_BMSK      0x00000001

/*
 * Initialize an rvcp remapper device.
 */
int rvcp_remapper_init(const struct device *dev)
{
    const struct rvcp_remapper_config *cfg = dev->config;
    uintptr_t reg_addr = cfg->addr, ext_addr, local_addr;
    uint32_t reg_idx, reg_value, page_size, page_select_bmsk;
    
    // The page size for each register is the total memory in the remapper
    // split between the number of registers.
    page_size = cfg->mem_size / cfg->reg_count;

    // Determine the page select bit mask, which is the upper bits of the
    // address beyond the page size.
    // E.g, page size of 128 MB = 0x800_0000 has a page select bit mask of
    // 0xF800_0000.
    page_select_bmsk = ~(page_size - 1);

    // Iterate over each register in the remapper, check if it is enabled,
    // and if it is then add the remapping to the EAT table.
    for (reg_idx = 0; reg_idx < cfg->reg_count; reg_idx++)
    {
        reg_addr = cfg->addr + (reg_idx * cfg->reg_step);
        reg_value = sys_read32(reg_addr);

        if (reg_value & RVCP_REMAPPER_ENABLE_BMSK)
        {
            ext_addr = reg_value & page_select_bmsk;
            local_addr = cfg->mem_addr + (reg_idx * page_size);
            sys_mm_drv_add_eat_region(local_addr, ext_addr, page_size);
        }
    }

    return 0;
}

/* 
 * Initialize any RCVPv1/v2 remapper devices.
 */

#define DT_DRV_COMPAT qcom_rvcp_remapper

#define RVCP_REMAPPER_CONFIG(node_id) {                     \
    .addr      = DT_REG_ADDR(node_id),                      \
    .reg_count = DT_REG_SIZE(node_id) / 4,                  \
    .reg_step  = 4,                                         \
    .mem_addr  = DT_PROP_BY_IDX(node_id, memory_region, 0), \
    .mem_size  = DT_PROP_BY_IDX(node_id, memory_region, 1)  \
}

#define RVCP_REMAPPER_DEFINE(inst)                                    \
    static struct rvcp_remapper_config rvcp_remapper_config_##inst =  \
        RVCP_REMAPPER_CONFIG(DT_DRV_INST(inst));                      \
                                                                      \
    DEVICE_DT_INST_DEFINE(inst, rvcp_remapper_init, NULL, NULL,       \
                          &rvcp_remapper_config_##inst, POST_KERNEL,  \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, NULL);

DT_INST_FOREACH_STATUS_OKAY(RVCP_REMAPPER_DEFINE);

/* 
 * Initialize any RCVPv3 remapper devices.
 */

#undef DT_DRV_COMPAT
#define DT_DRV_COMPAT qcom_rvcp_remapper_v3

#define RVCP_REMAPPER_V3_CONFIG(node_id) {                  \
    .addr      = DT_REG_ADDR(node_id),                      \
    .reg_count = DT_REG_SIZE(node_id) / 8,                  \
    .reg_step  = 8,                                         \
    .mem_addr  = DT_PROP_BY_IDX(node_id, memory_region, 0), \
    .mem_size  = DT_PROP_BY_IDX(node_id, memory_region, 1)  \
}

#define RVCP_REMAPPER_V3_DEFINE(inst)                                   \
    static struct rvcp_remapper_config rvcp_remapper_v3_config_##inst = \
        RVCP_REMAPPER_V3_CONFIG(DT_DRV_INST(inst));                     \
                                                                        \
    DEVICE_DT_INST_DEFINE(inst, rvcp_remapper_init, NULL, NULL,         \
                          &rvcp_remapper_v3_config_##inst, POST_KERNEL, \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, NULL);

DT_INST_FOREACH_STATUS_OKAY(RVCP_REMAPPER_V3_DEFINE);
