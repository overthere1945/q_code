/**
 * \file spf_begin_pragma.h
 * \brief 
 *  	 This file defines pragma to ignore zero length array warnings for different compilers
 * 
 * \copyright
 *    Copyright (c) 2021 Qualcomm Technologies, Inc.
 *    All Rights Reserved.
 *    Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
// clang-format off
/*
$Header: //components/rel/qsh.drivers/1.0.2/qsh_audio/audio_api/spf_begin_pragma.h#1 $
*/
// clang-format on
 

#if defined( __qdsp6__ )
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wzero-length-array"
#elif defined( __GNUC__ )
#elif defined( __arm__ )
#elif defined( _MSC_VER )
  #pragma warning( push )
  #pragma warning( disable:4200 )
#elif defined( __H2XML__)
#else
  #error "Unsupported compiler."
#endif
