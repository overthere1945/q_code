/**
 * @file
 * @brief Qualcomm Memory Management Driver External Address Translation Code
 *
 * This file provides Qualcomm support for external address translations.
 * 
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/__assert.h>

#include <drivers/mm/system_mm_qcom.h>


#define DT_DRV_COMPAT qcom_ext_addr_trans


typedef struct {
    uintptr_t local_addr;
    uintptr_t external_addr;
    size_t    size;
} translated_phys_memory_t;

#define EXT_TRANS_MEMORY(node_id) { \
    .local_addr    = DT_REG_ADDR(node_id), \
    .external_addr = DT_PROP(node_id, external_address), \
    .size          = DT_REG_SIZE(node_id) \
},

#ifdef CONFIG_DT_HAS_QCOM_EXT_ADDR_TRANS_ENABLED
    #define MAX_TRANSLATED_REGIONS DT_INST_CHILD_NUM(0) + CONFIG_QC_MM_EAT_MAX_REGIONS
#else
    #define MAX_TRANSLATED_REGIONS CONFIG_QC_MM_EAT_MAX_REGIONS
#endif

typedef struct {
    uint32_t                 num_regions;
    translated_phys_memory_t regions[MAX_TRANSLATED_REGIONS];
} translated_memory_config_t;

static translated_memory_config_t translated_memory_config = {
    #ifdef CONFIG_DT_HAS_QCOM_EXT_ADDR_TRANS_ENABLED
    .num_regions = DT_INST_CHILD_NUM(0),
    .regions = {
        DT_INST_FOREACH_CHILD(0, EXT_TRANS_MEMORY)
    }
    #endif
};

K_MUTEX_DEFINE(qcom_eat_mutex);


/*
 * This implementation is converting from an external physical address
 * ("virt") to the local one ("phys") that the processor should use.
 * 
 * This is invoked either directly or via the device_map function (when
 * CONFIG_EXTERNAL_ADDRESS_TRANSLATION is defined).
 * 
 * See system_mm.h
 */
int sys_mm_drv_page_phys_get(void *virt, uintptr_t *phys)
{
    uintptr_t ext_phys = (uintptr_t)virt;
    uint32_t i;

    if (phys == NULL)
    {
        return -EINVAL;
    }

    for (i = 0; i < translated_memory_config.num_regions; i++)
    {
        if (ext_phys >= translated_memory_config.regions[i].external_addr &&
            ext_phys < translated_memory_config.regions[i].external_addr + translated_memory_config.regions[i].size)
        {
            *phys = translated_memory_config.regions[i].local_addr + 
               (ext_phys - translated_memory_config.regions[i].external_addr);
            return 0;
        }
    }

    /* No translation. */
    *phys = ext_phys;
    return 0;
}

/*
 * This implementation is converting from a local physical address
 * to an external one.
 * 
 * See system_mm_qcom.h
 */
int sys_mm_drv_ext_phys_get(void *phys, uintptr_t *ext_phys)
{
    uint32_t i;

    if (ext_phys == NULL)
    {
        return -EINVAL;
    }

    for (i = 0; i < translated_memory_config.num_regions; i++)
    {
        if ((uintptr_t)phys >= translated_memory_config.regions[i].local_addr &&
            (uintptr_t)phys < translated_memory_config.regions[i].local_addr + translated_memory_config.regions[i].size)
        {
            *ext_phys = translated_memory_config.regions[i].external_addr + 
               ((uintptr_t)phys - translated_memory_config.regions[i].local_addr);
            return 0;
        }
    }

    /* No translation. */
    *ext_phys = (uintptr_t)phys;
    return 0;
}

/*
 * See system_mm_qcom.h
 */
int sys_mm_drv_add_eat_region (uintptr_t local_addr, uintptr_t ext_addr, size_t size)
{
    translated_phys_memory_t *region;

    k_mutex_lock(&qcom_eat_mutex, K_FOREVER);

    __ASSERT(translated_memory_config.num_regions < MAX_TRANSLATED_REGIONS,
        "Exceeding maximum number of translated regions");

    region = &translated_memory_config.regions[translated_memory_config.num_regions];
    region->local_addr = local_addr;
    region->external_addr = ext_addr;
    region->size = size;
    translated_memory_config.num_regions++;

    k_mutex_unlock(&qcom_eat_mutex);

    return 0;
}

/*
 * See system_mm_qcom.h
 */
void sys_mm_drv_print_eat_regions (void)
{
    uint32_t i;
    const translated_phys_memory_t *region;

    printk("-------- External Address Translations --------\n");
    printk("Local Address Range      External Address Range\n");

    for (i = 0; i < translated_memory_config.num_regions; i++)
    {
        region = &translated_memory_config.regions[i];
        printk("0x%08lx--0x%08lx   0x%08lx--0x%08lx\n", 
            region->local_addr, region->local_addr + region->size - 1,
            region->external_addr, region->external_addr + region->size - 1);
    }
}
