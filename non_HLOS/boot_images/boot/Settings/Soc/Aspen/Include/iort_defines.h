//   Copyright (c)  Qualcomm Technologies, Inc. All rights reserved.
//   Confidential and Proprietary - Qualcomm Technologies, Inc

#define IORT_WORLD_ID_NON_SECURE 0x0
#define IORT_WORLD_ID_SECURE     0x1
#define IORT_WORLD_ID_SHIFT      31

#define IORT_DYNAMIC_MAPPING_NO          0x0
#define IORT_DYNAMIC_MAPPING_YES_DEFAULT 0x0
#define IORT_DYNAMIC_MAPPING_YES         0x1
#define IORT_DYNAMIC_MAPPING_SHIFT       30

#define IORT_TRANSLATION_TYPE_S2_ONLY      0x0
#define IORT_TRANSLATION_TYPE_S2CR_BYPASS  0x1
#define IORT_TRANSLATION_TYPE_SINGLE_STAGE 0x1
#define IORT_TRANSLATION_TYPE_NESTED       0x3
#define IORT_TRANSLATION_TYPE_SHIFT        24

#define IORT_VMID_SHIFT 16

#define ACPI_OEM_ID "QCOM"
#define ACPI_OEM_TABLE_ID 0x51434F4D 0x45444B32
#define ACPI_OEM_REVISION 0x00008998
#define ACPI_CREATOR_ID 0x51434F4D
#define ACPI_CREATOR_REVISION 0x00000001


#define AC_VM_TZ                 1
#define AC_VM_HLOS               3
#define AC_VM_ADSP_Q6_ELF        6   /* Single */
#define AC_VM_CP_BITSTREAM       9
#define AC_VM_CP_PIXEL           10
#define AC_VM_CP_NON_PIXEL       11
#define AC_VM_VIDEO_FW           12   /* IPA not equal PA */
#define AC_VM_CP_CAMERA          13
#define AC_VM_LPASS              22   /* Single */  /* IPA = PA */
#define AC_VM_CP_CAMERA_PREVIEW  29
#define AC_VM_CDSP_Q6_ELF        30   /* Single stage */
#define AC_VM_SHARED_GPU_PIL     38   /* GPU PIL uses this shared VM */
#define AC_VM_CP_CDSP            42   /* CP VM for CDSP block */
#define AC_VM_TRUSTED_UI         45   /* Tools need value. Trusted VM managed by Hypervisor. Few places Guest OS VM is used for trusted UI */
#define AC_VM_OEM                49   /* Guest OS VM for OEM */
#define AC_VM_DCP_ELF            64
#define AC_VM_DISPLAY            65
#define AC_VM_AM_ELF             90
#define AC_VM_WM_ELF             91