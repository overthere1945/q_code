/*=============================================================================
  @file sns_tppe_so_notes.c

  The file inserts note_type structure needed by island shared object.

  Copyright (c) 2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

#ifdef SNS_DYNLIB_TPPE
/*============================================================================
Include Files
===========================================================================*/
#include "note_type.h"
#include "sns_dl_utils.h"
#include "sns_tppe_ver.h"

/*============================================================================
Constants
===========================================================================*/
#define _STRNG(x) #x
#define STRNG(x)  _STRNG(x)

/*put library and version pair in the new section created using linker script*/
const sns_note_type sns_bimg_lib_ver __attribute__((section(".note.lib.ver")))
__attribute__((visibility("default"))) __attribute__((aligned(0x1000))) = {
    100, 0, 0, "lib.ver.1.0.0.sns_tppe_normal.so:" STRNG(TPPE_VER_MAJOR) "." STRNG(TPPE_VER_MINOR)};
#endif
