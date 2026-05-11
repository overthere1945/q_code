#pragma once
/*=============================================================================
  @file qsh_audio_assert.h

  Abort the program is the expression evaluates to false.

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

#include <stdbool.h>
#include "err.h"


#if defined SSC_TARGET_HEXAGON
#   define QSH_AUDIO_ASSERT(value) if(!(value))\
      ERR_FATAL( #value ,0,0,0)
#elif defined SSC_TARGET_X86
#include <assert.h>
#   define QSH_AUDIO_ASSERT(value) assert(value)
#endif

