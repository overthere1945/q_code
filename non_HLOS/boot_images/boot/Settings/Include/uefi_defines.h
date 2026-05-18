#ifndef __UEFI_SW_DEFINES_H__
#define __UEFI_SW_DEFINES_H__

/*===========================================================================

                 Uefi defines Header File

GENERAL DESCRIPTION
  This header file contains declarations and definitions for uefi platform 
  settings.
    
Copyright 2023 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/08/23   rbv     Initial revision.
============================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/

/*===========================================================================
                      EDK2 MACROS for Resource Type
===========================================================================*/
//
// Value of ResourceType in EFI_HOB_RESOURCE_DESCRIPTOR.
//
#define EFI_RESOURCE_SYSTEM_MEMORY          0x00000000
#define EFI_RESOURCE_MEMORY_MAPPED_IO       0x00000001
#define EFI_RESOURCE_IO                     0x00000002
#define EFI_RESOURCE_FIRMWARE_DEVICE        0x00000003
#define EFI_RESOURCE_MEMORY_MAPPED_IO_PORT  0x00000004
#define EFI_RESOURCE_MEMORY_RESERVED        0x00000005
#define EFI_RESOURCE_IO_RESERVED            0x00000006
#define EFI_RESOURCE_MAX_MEMORY_TYPE        0x00000007


/*=========================================================================
                 Enums from Source Code
===========================================================================*/
/*Resource type Enums from MemRegionInfo.h*/
#define AddMem                          0x0 // Add memory as specified
#define AddMemForNonDebugMode           0x1 // Add memory if debug mode is not enabled
#define AddMemForNonDebugOrNonCrashMode 0x2 // Add memory if debug mode is not enabled or
                                            // if debug mode is enabled but crash did not happen
#define ReserveMemForNonCrashMode       0x3 // Reserve memory if crash did not happen
#define NoBuildHob                      0x4 // Do not build HOB for this region
#define AddPeripheral                   0x5 // Hob, Cache Settings
#define HobOnlyNoCacheSetting           0x6 // Hob, No Cache Settings
#define CacheSettingCarveOutOnly        0x7 // No Hob, Cache Setting in DEBUG only
#define AllocOnly                       0x8
#define NoMap                           0x9 // No Hob,skip mapping in mmu and create a hole
#define AddDynamicMem                   0xa // Add dynamic memory that are marked as reserved             
#define ErrorBuildHob                   99  // Return high invalid value on error

/*Memory Type Enums from MdePkg UefiMultiPhase.h*/
#define EfiReservedMemoryType         0x00    
#define EfiLoaderCode                 0x01      
#define EfiLoaderData                 0x02
#define EfiBootServicesCode           0x03 
#define EfiBootServicesData           0x04
#define EfiRuntimeServicesCode        0x05
#define EfiRuntimeServicesData        0x06
#define EfiConventionalMemory         0x07
#define EfiUnusableMemory             0x08
#define EfiACPIReclaimMemory          0x09
#define EfiACPIMemoryNVS              0x0A
#define EfiMemoryMappedIO             0x0B
#define EfiMemoryMappedIOPortSpace    0x0C
#define EfiPalCode                    0x0D
#define EfiPersistentMemory           0x0E
#define EfiMaxMemoryType              0x0F

/*Cache Attribute Enum from ArmLib.h*/
#define ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED               0x00
#define ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_UNCACHED_UNBUFFERED     0x01
#define ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK                        0x02        
#define ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_WRITE_BACK              0x03
#define ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK_NONSHAREABLE           0x04
#define ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_WRITE_BACK_NONSHAREABLE 0x05
#define ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH                     0x06
#define ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_WRITE_THROUGH           0x07
#define ARM_MEMORY_REGION_ATTRIBUTE_DEVICE                            0x08
#define ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_DEVICE                  0x09
#define ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH_XN                  0x20
#define ARM_MEMORY_REGION_ATTRIBUTE_WRITE_THROUGH_XN_RO               0x21
#define ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK_XN                     0x22
#define ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK_XN_RO                  0x23
#define ARM_MEMORY_REGION_ATTRIBUTE_NONSECURE_UNCACHED_UNBUFFERED_XN  0x24
#define ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED_XN            0x25
#define ARM_MEMORY_REGION_ATTRIBUTE_DEVICE_RO                         0x26
#define ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK_RO                     0x27

/*Resrouce Attribute type from PiHob.h*/
#define EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE              0x00000400
#define EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE        0x00000800
#define EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE  0x00001000
#define EFI_RESOURCE_ATTRIBUTE_PRESENT                  0x00000001
#define EFI_RESOURCE_ATTRIBUTE_INITIALIZED              0x00000002
#define EFI_RESOURCE_ATTRIBUTE_TESTED                   0x00000004
#define EFI_RESOURCE_ATTRIBUTE_READ_PROTECTED           0x00000080
#define EFI_RESOURCE_ATTRIBUTE_WRITE_PROTECTED          0x00000100
#define EFI_RESOURCE_ATTRIBUTE_EXECUTION_PROTECTED      0x00000200
#define EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE     0x00002000
#define EFI_RESOURCE_ATTRIBUTE_EXECUTION_PROTECTABLE    0x00400000
#define EFI_RESOURCE_ATTRIBUTE_READ_PROTECTABLE         0x00100000
#define EFI_RESOURCE_ATTRIBUTE_WRITE_PROTECTABLE        0x00200000

#define SYSTEM_MEMORY_RESOURCE_ATTR_SETTINGS1                    \
                EFI_RESOURCE_ATTRIBUTE_PRESENT |                 \
                EFI_RESOURCE_ATTRIBUTE_INITIALIZED |             \
                EFI_RESOURCE_ATTRIBUTE_TESTED

#define SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES1                \
                EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |             \
                EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |       \
                EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE | \
                EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |    \
                EFI_RESOURCE_ATTRIBUTE_EXECUTION_PROTECTABLE |   \
                EFI_RESOURCE_ATTRIBUTE_READ_PROTECTABLE |        \
                EFI_RESOURCE_ATTRIBUTE_WRITE_PROTECTABLE

#define SYSTEM_MEMORY_RESOURCE_ATTR_SETTINGS_CAPABILITIES        \
        (SYSTEM_MEMORY_RESOURCE_ATTR_SETTINGS1 |                  \
        SYSTEM_MEMORY_RESOURCE_ATTR_CAPABILITIES1)

//=========================================================================
//=========================================================================


#endif /*__UEFI_SW_DEFINES_H__*/
