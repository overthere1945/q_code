#pragma once

/*======================================================================================
  @file qsh_audio_osa_attr.h

  @brief
  Definitions for OSA attributes

  Copyright (c) 2021 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  ======================================================================================*/

/*
*****************************************************************************************
                               Includes
*****************************************************************************************
*/
#if defined(SSC_TARGET_HEXAGON)
#define __SIZEOF_ATTR_THREAD    64
#define __SIZEOF_ATTR_LOCK      64
#define __SIZEOF_ATTR_TIMER     64
#define __SIZEOF_ATTR_SEM       64
#define __SIZEOF_LOCK           32
#elif defined(SDC)
#define __SIZEOF_ATTR_THREAD    64
#define __SIZEOF_ATTR_LOCK      64
#define __SIZEOF_ATTR_TIMER     64
#define __SIZEOF_ATTR_SEM       64
#define __SIZEOF_LOCK           64
#elif defined(SSC_TARGET_X86)
#define __SIZEOF_ATTR_THREAD    88
#define __SIZEOF_ATTR_LOCK      64
#define __SIZEOF_ATTR_TIMER     64
#define __SIZEOF_ATTR_SEM       64
#define __SIZEOF_LOCK           40
#endif

/*
*****************************************************************************************
                               Typedefs
*****************************************************************************************
*/
typedef enum
{
  QSH_AUDIO_OSA_MEM_TYPE_NORMAL,
  QSH_AUDIO_OSA_MEM_TYPE_ISLAND,
  QSH_AUDIO_OSA_MEM_TYPE_MAX
} qsh_audio_osa_mem_type;


