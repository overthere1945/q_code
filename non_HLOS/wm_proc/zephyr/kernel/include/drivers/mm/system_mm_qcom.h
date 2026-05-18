/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

/**
 * @file
 * @brief Qualcomm Memory Management Driver Extension APIs
 *
 * This contains Qualcomm-specific API extensions for
 * the system-wide memory management driver. The primary purpose is
 * to provide support for External Address Translations (EAT) where
 * the physical address used by the local processor is translated
 * by hardware when it exits the subsystem onto the main system bus.
 * 
 * Two APIs are provided to map between the local and external physical
 * addresses:
 * 
 * Local -> External:
 *   sys_mm_drv_ext_phys_get(void *phys, uintptr_t *ext_phys);
 *   
 * External -> Local:
 *   sys_mm_drv_map_phys_get(void *ext_phys, uintptr_t *phys);
 * 
 *   or equivalently (and without need for this header):
 * 
 *   #include <zephyr/sys/device_mmio.h> 
 *   device_map(mm_reg_t *phys, uintptr_t ext_phys, 0, 0);
 * 
 * This functionality is modelled on the RAT (region address translation)
 * supported by some TI devices and depends on CONFIG_EXTERNAL_ADDRESS_TRANSLATION.
 * 
 */

#ifndef ZEPHYR_INCLUDE_DRIVERS_SYSTEM_MM_QCOM_H_
#define ZEPHYR_INCLUDE_DRIVERS_SYSTEM_MM_QCOM_H_

#include <zephyr/types.h>
#include <zephyr/drivers/mm/system_mm.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Get the external physical memory address from a local one.
 *
 * The function finds the external physical address for the given local 
 * address when external address translation is used.
 * The "external" physical address is what the SOC outside of the current
 * processor subsystem sees. The "local" physical address is what the current
 * processor uses. Some devices contains a top-level memory remapper
 * that modifies the local physical address to a different one (for
 * example a RISC-V processor inside an ARM-based SOC). When providing a
 * physical address to a DMA engine outside the subsystem the external physical 
 * address will need to be used.
 * 
 * For converting in the other direction use either sys_mm_drv_page_phys_get
 * or device_map.
 *
 * @param      phys     Local (to this processor) physical address.
 * @param[out] ext_phys The external physical address the outside
 *                      environment sees as corresponding to the
 *                      given local physical address.
 *
 * @retval 0 if ext_phys was populated.
 * @retval -EINVAL if invalid arguments are provided
 */
int sys_mm_drv_ext_phys_get(void *phys, uintptr_t *ext_phys);


/**
 * @brief Add a new external address translation region.
 *
 * The function adds a new external address translation region to the EAT
 * driver table.
 *
 * @param      local_addr   Local (to this processor) physical address.
 * @param      ext_addr     External physical address.
 * @param      size         Size in bytes of the region.
 *
 * @retval 0 on success, asserts if insufficient space.
 */
int sys_mm_drv_add_eat_region (uintptr_t local_addr, uintptr_t ext_addr, size_t size);


/**
 * @brief Print defined external address translations.
 *
 * The function prints all current external address translations to the
 * kernel log (printk).
 */
void sys_mm_drv_print_eat_regions (void);


#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_DRIVERS_SYSTEM_MM_QCOM_H_ */
