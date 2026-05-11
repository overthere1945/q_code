#pragma once
/** ============================================================================
 *  @file
 *
 *  @brief Definitions for OSA attributes.
 *
 *  @copyright Copyright (c) Qualcomm Technologies, Inc. and / or its
 *  subsidiaries. All Rights Reserved. Confidential and Proprietary - Qualcomm
 *  Technologies, Inc.
 *  ===========================================================================
 */

/*
*******************************************************************************
                               Macros
*******************************************************************************
*/
#if defined(SSC_TARGET_HEXAGON)
#define __SIZEOF_ATTR_THREAD 64
#define __SIZEOF_ATTR_LOCK   64
#define __SIZEOF_ATTR_TIMER  64
#define __SIZEOF_ATTR_SEM    64
#define __SIZEOF_LOCK        32
#define __SIZEOF_SIGNAL      16
#elif defined(SDC)
#define __SIZEOF_ATTR_THREAD 64
#define __SIZEOF_ATTR_LOCK   64
#define __SIZEOF_ATTR_TIMER  64
#define __SIZEOF_ATTR_SEM    64
#define __SIZEOF_LOCK        64
#define __SIZEOF_SIGNAL      0
#elif defined(SSC_TARGET_X86)
#define __SIZEOF_ATTR_THREAD 88
#define __SIZEOF_ATTR_LOCK   64
#define __SIZEOF_ATTR_TIMER  64
#define __SIZEOF_ATTR_SEM    64
#define __SIZEOF_LOCK        40
#define __SIZEOF_SIGNAL      96
#elif defined(SSC_TARGET_ASPEN_SWM)
#define __SIZEOF_ATTR_THREAD    64
#define __SIZEOF_ATTR_LOCK      16
#define __SIZEOF_ATTR_TIMER     16
#define __SIZEOF_ATTR_SEM       16
#define __SIZEOF_LOCK           32
#define __SIZEOF_SIGNAL         24
#endif

/*
*******************************************************************************
                               Typedefs
*******************************************************************************
*/
/**
 * @brief Memory type.
 *
 */
typedef enum
{
  SNS_OSA_MEM_TYPE_NORMAL,     /*!< Non island memory. */
  SNS_OSA_MEM_TYPE_ISLAND,     /*!< Island memory. */
  SNS_OSA_MEM_TYPE_ISLAND_SSC, /*!< Island SSC memory. */
  SNS_OSA_MEM_TYPE_MAX
} sns_osa_mem_type;

/**
 * @brief Use normal memory when QSH is not operating in island mode.
 */
#ifdef SNS_DISABLE_ISLAND
#define SNS_OSA_MEM_TYPE_ISLAND     SNS_OSA_MEM_TYPE_NORMAL
#define SNS_OSA_MEM_TYPE_ISLAND_SSC SNS_OSA_MEM_TYPE_NORMAL
#endif
