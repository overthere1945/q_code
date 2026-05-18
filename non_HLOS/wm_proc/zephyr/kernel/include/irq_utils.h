/*
 * Copyright (c) Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

 #ifndef IRQ_UTILS_H
 #define IRQ_UTILS_H
 
 #include <zephyr/irq.h>
 #ifdef CONFIG_MULTI_LEVEL_INTERRUPTS
 #include <zephyr/irq_multilevel.h>
 #endif
 
 
 /**
  * Convert an interrupt vector to the Zephyr IRQ number.
  *
  * Use this for external interrupts that are second-level (i.e. RISC-V PLIC) when
  * CONFIG_MULTI_LEVEL_INTERRUPTS is enabled but otherwise first level.
  *
  * @param irq IRQ line number
  *
  * @return The Zephyr irq vector used to connect the interrupt.
  */
 static inline unsigned int irq_to_zephyr(unsigned int irq)
 {
 #ifdef CONFIG_MULTI_LEVEL_INTERRUPTS
   return IRQ_TO_L2(irq) | CONFIG_2ND_LVL_INTR_00_OFFSET;
 #else
   return irq;
 #endif
 }
 
 
 #endif /* IRQ_UTILS_H */
 