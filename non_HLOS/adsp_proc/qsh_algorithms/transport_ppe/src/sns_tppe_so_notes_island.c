/*=============================================================================
  @file sns_tppe_so_notes_island.c

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

#define _STRNG(x) #x
#define STRNG(x)  _STRNG(x)

/*
 * the variable suggests DL loaded to load library in uimage memory.
 */
const note_type sns_uimg_dl_ver __attribute__((section(".note.qti.uimg.dl.ver")))
__attribute__((visibility("default"))) = {24,
                                          16,
                                          0,
                                          "uimg.dl.ver." STRNG(UIMG_DL_VER_MAJOR) "." STRNG(
                                              UIMG_DL_VER_MINOR) "." STRNG(UIMG_DL_VER_MAINT),
                                          {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

/*put library and version pair in the new section created using linker script*/
const sns_note_type sns_uimg_lib_ver __attribute__((section(".note.lib.ver")))
__attribute__((visibility("default"))) __attribute__((aligned(0x1000))) = {
    100, 0, 0, "lib.ver.1.0.0.sns_tppe_island.so:" STRNG(TPPE_VER_MAJOR) "." STRNG(TPPE_VER_MINOR)};
#endif
