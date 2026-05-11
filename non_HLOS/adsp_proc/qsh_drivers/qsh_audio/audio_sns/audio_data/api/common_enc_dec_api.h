#ifndef COMMON_ENC_DEC_API_H
#define COMMON_ENC_DEC_API_H
/**
 * \file common_enc_dec_api.h
 * \brief
 *    This file contains media format IDs and definitions
 * 
 * \copyright
 *  Copyright (c) 2018-2023 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
// clang-format off
/*
$Header: //components/dev/qsh.drivers/1.0/dekantha.qsh.drivers.1.0.sensorbuild_3_18/qsh_audio/audio_api/common_enc_dec_api.h#1 $
*/
// clang-format on



/** @addtogroup ar_spf_mod_encdec_mods
    These APIs are used by all encoder and decoders, including the PCM Decoder
    and PCM Encoder modules.
*/

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

/*# @h2xml_title1           {Common APIs for Encoders and Decoders}
    @h2xml_title_agile_rev  {Common APIs for Encoders and Decoders}
    @h2xml_title_date       {August 13, 2018} */

/*# @h2xmlx_xmlNumberFormat {int} */

#define PARAM_ID_PCM_OUTPUT_FORMAT_CFG             0x08001008
/*# @h2xmlp_parameter   {"PARAM_ID_PCM_OUTPUT_FORMAT_CFG",
                          PARAM_ID_PCM_OUTPUT_FORMAT_CFG}
    @h2xmlp_description {Identifier for the parameter that configures the PCM
                         output format of the PCM encoder, PCM decoder, or PCM
                         converter. For more details, see the <i>AudioReach
                         Module User Guide</i> (80-VN500-4).}
    @h2xmlp_toolPolicy  {Calibration} */



/** @ingroup ar_spf_mod_encdec_mods
    Payload of the #PARAM_ID_PCM_OUTPUT_FORMAT_CFG parameter.
 */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct param_id_pcm_output_format_cfg_t
{
   uint32_t data_format;
   /**< Format of the PCM output data.

        @valuesbul
        - #INVALID_VALUE (Default)
        - #DATA_FORMAT_FIXED_POINT @tablebulletend */

   /*#< @h2xmle_description {Format of the data}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"INVALID_VALUE"=0,
                             "DATA_FORMAT_FIXED_POINT"=1}
        @h2xmle_policy      {Basic} */

   uint32_t fmt_id;
   /**< Identifier for the data stream format.

        @valuesbul
        - #INVALID_VALUE
        - #MEDIA_FMT_ID_PCM @tablebulletend */

   /*#< @h2xmle_description {ID for the data stream format.}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"INVALID_VALUE"=0,
                             "Media format ID of PCM"=MEDIA_FMT_ID_PCM}
        @h2xmle_policy      {Basic} */

   uint32_t payload_size;
   /**< Size of the payload that immediately follows this structure.

        The payload size does not include bytes added for 32-bit alignment. */

   /*#< @h2xmle_description {Size of the payload that immediately follows this
                             structure. The payload size does not include
                             bytes added for 32-bit alignment.}
        @h2xmle_default     {0}
        @h2xmle_range       {0..0xFFFFFFFF}
        @h2xmle_policy      {Basic} */

#if defined(__H2XML__)
   uint8_t  payload[0];
   /**< Payload for PCM output format configuration of size payload_size.

        The payload structure varies depending on the combination of the
        data_format and fmt_id fields. For example, if data_format =
        #DATA_FORMAT_FIXED_POINT and fmt_id = #MEDIA_FMT_ID_PCM, the payload is
        payload_pcm_output_format_cfg_t.

        The floating-point data format might have a different payload. */

   /*#< @h2xmle_description       {Payload for PCM output format configuration
                                   of size payload_size. \n
                                   The payload structure varies depending on
                                   the combination of the data_format and
                                   fmt_id fields. For example, if data_format
                                   is DATA_FORMAT_FIXED_POINT and fmt_id is
                                   MEDIA_FMT_ID_PCM, the payload is
                                   payload_pcm_output_format_cfg_t. \n
                                   The floating-point data format might have a
                                   different payload.}
       @h2xmle_policy            {Basic}
       @h2xmle_variableArraySize {payload_size} */
#endif
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;
typedef struct param_id_pcm_output_format_cfg_t param_id_pcm_output_format_cfg_t;


/*# @h2xmlp_subStruct
    @h2xmlp_description {Payload for PARAM_ID_PCM_OUTPUT_FORMAT_CFG when fmt_id
                         = MEDIA_FMT_ID_PCM and data_format is fixed point.
                         For more details, see the <i>AudioReach Module User
                         Guide</i> (80-VN500-4).}
    @h2xmlp_toolPolicy  {Calibration} */

/** @ingroup ar_spf_mod_encdec_mods
    Payload of the #PARAM_ID_PCM_OUTPUT_FORMAT_CFG when fmt_id =
    #MEDIA_FMT_ID_PCM and data_format = #DATA_FORMAT_FIXED_POINT.
 */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct payload_pcm_output_format_cfg_t
{
   int16_t bit_width;
   /**< Bit width of each sample.

        @valuesbul
        - #PARAM_VAL_UNSET
        - #PARAM_VAL_NATIVE
        - #PARAM_VAL_INVALID
        - #BIT_WIDTH_16
        - #BIT_WIDTH_24
        - #BIT_WIDTH_32 @tablebulletend */

   /*#< @h2xmle_description {Bit width of each sample (such as 16, 24, or 32
                             bits.}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"PARAM_VAL_UNSET"=-2;
                             "PARAM_VAL_NATIVE"=-1;
                             "PARAM_VAL_INVALID"=0;
                             "BIT_WIDTH_16"=16;
                             "BIT_WIDTH_24"=24;
                             "BIT_WIDTH_32"=32}
        @h2xmle_policy      {Basic} */

   int16_t alignment;
   /**< Indicates the alignment of bits_per_sample in sample_word_size.

        This field is relevant only when bit_width is 24 and bits_per_sample
        is 32.
        - For bit_width 24, bits_per_sample 32, and q_factor 23, alignment
          should be PCM_LSB_ALIGNED.
        - For bit_width 24, bits_per_sample 32, and q_factor 31, alignment
          should be PCM_MSB_ALIGNED. @tablebulletend */

   /*#< @h2xmle_description {Indicates the alignment of bits_per_sample in
                             sample_word_size. This field is relevant only when
                             bit_width is 24 and bits_per_sample is 32.
                             - For bit_width 24, bits_per_sample 32, and
                               q_factor 23, alignment should be
                               PCM_LSB_ALIGNED.
                             - For bit_width 24, bits_per_sample 32, and
                               q_factor 31, alignment should be
                               PCM_MSB_ALIGNED.}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"PARAM_VAL_UNSET"=-2;
                             "PARAM_VAL_NATIVE"=-1;
                             "PARAM_VAL_INVALID"=0;
                             "PCM_LSB_ALIGNED"=1;
                             "PCM_MSB_ALIGNED"=2}
        @h2xmle_policy      {Basic} */

   int16_t bits_per_sample;
   /**< Bits required to store one sample.

        For example:
        - 16-bits per sample always contain 16-bit samples.
        - 24-bits per sample always contain 24-bit samples.
        - 32-bits per sample have the following cases:
           - If bit width = 24 and alignment = PCM_LSB_ALIGNED, 24-bit samples
             are placed in the lower 24 bits of a 32-bit word. The upper bits
             might be sign-extended.
           - If bit width = 24 and alignment = PCM_MSB_ALIGNED, 24-bit samples
             are placed in the upper 24 bits of a 32-bit word. The lower bits
             might not be set to 0.
           - If bit width = 32, 32-bit samples are placed in the 32-bit
             words. @tablebulletend */

   /*#< @h2xmle_description {Bits required to store one sample. For example: \n
                             - 16-bits per sample always contain 16-bit
                               samples.
                             - 24-bits per sample always contain 24-bit
                               samples.
                             - 32-bits per sample have the following cases:
                                - If bit width = 24 and alignment =
                                  PCM_LSB_ALIGNED, 24-bit samples are placed in
                                  the lower 24 bits of a 32-bit word. The upper
                                  bits might be sign-extended.
                                - If bit width = 24 and alignment =
                                  PCM_MSB_ALIGNED, 24-bit samples are placed in
                                  the upper 24 bits of a 32-bit word. The lower
                                  bits might not be set to 0.
                                - If bit width = 32, 32-bit samples are placed
                                  in the 32-bit words.}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"PARAM_VAL_UNSET"=-2;
                             "PARAM_VAL_NATIVE"=-1;
                             "PARAM_VAL_INVALID"=0;
                             "16"=16;
                             "24"=24;
                             "32"=32}
        @h2xmle_policy      {Basic} */

   int16_t q_factor;
   /**< Indicates the Q factor of the PCM data:
        - Q15 for 16-bit_width signed data.
        - Q23 for 24-bit_width signed packed (24-bits_per_sample) data.
        - Q27 for LSB-aligned, 24-bit_width unpacked (32-bits_per_sample)
          signed data used internally to the SPF.
        - Q31 for MSB-aligned, 24-bit_width unpacked (32 bits_per_sample)
          signed data.
        - Q23 for LSB-aligned, 24-bit_width unpacked (32 bits_per_sample)
          signed data.
        - Q31 for 32-bit_width signed data. @tablebulletend */

   /*#< @h2xmle_description {Indicates the Q factor of the PCM data: \n
                             - Q15 for 16-bit_width signed data
                             - Q23 for 24-bit_width signed packed
                               (24-bits_per_sample) data
                             - Q27 for LSB-aligned, 24-bit_width unpacked
                               (32-bits_per_sample) signed data used internally
                               to the SPF
                             - Q31 for MSB-aligned, 24-bit_width unpacked
                               (32 bits_per_sample) signed data
                             - Q23 for LSB-aligned, 24-bit_width unpacked
                               (32 bits_per_sample) signed data
                              -Q31 for 32-bit_width signed data}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"PARAM_VAL_UNSET"=-2;
                             "PARAM_VAL_NATIVE"=-1;
                             "PARAM_VAL_INVALID"=0;
                             "Q15"=15;
                             "Q23"=23;
                             "Q27"=27;
                             "Q31"=31}
        @h2xmle_policy      {Basic} */

   int16_t endianness;
   /**< Indicates whether PCM samples are stored in little endian or big endian
        format.

        @valuesbul
        - #PARAM_VAL_UNSET
        - #PARAM_VAL_NATIVE
        - #PARAM_VAL_INVALID (Default)
        - #PCM_LITTLE_ENDIAN
        - #PCM_BIG_ENDIAN @tablebulletend */

   /*#< @h2xmle_description {Indicates whether PCM samples are stored in little
                             endian or big endian format.}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"PARAM_VAL_UNSET"=-2;
                             "PARAM_VAL_NATIVE"=-1;
                             "PARAM_VAL_INVALID"=0;
                             "PCM_LITTLE_ENDIAN"=1;
                             "PCM_BIG_ENDIAN"=2}
        @h2xmle_policy      {Basic} */

   int16_t interleaved;
   /**< Indicates whether the data is interleaved.

        @valuesbul
        - #PARAM_VAL_UNSET
        - #PARAM_VAL_NATIVE
        - #PARAM_VAL_INVALID (Default)
        - #PCM_INTERLEAVED
        - #PCM_DEINTERLEAVED_PACKED
        - #PCM_DEINTERLEAVED_UNPACKED @tablebulletend */

   /*#< @h2xmle_description {Indicates whether the data is interleaved.}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"PARAM_VAL_UNSET"=-2;
                             "PARAM_VAL_NATIVE"=-1;
                             "PARAM_VAL_INVALID"=0;
                             "PCM_INTERLEAVED"=1;
                             "PCM_DEINTERLEAVED_PACKED"=2;
                             "PCM_DEINTERLEAVED_UNPACKED"=3}
        @h2xmle_policy      {Basic} */

   int16_t reserved;
   /**< Reserved for alignment; must be set to 0. */

   /*#< @h2xmle_description {Reserved for alignment; must be set to 0.}
        @h2xmle_default     {0}
        @h2xmle_policy      {Basic} */

   int16_t num_channels;
   /**< Number of channels in the array.

        @values -2 through 32 (Default = 0) */

   /*#< @h2xmle_description {Number of channels.}
        @h2xmle_default     {0}
        @h2xmle_range       {-2..32}
        @h2xmle_policy      {Basic} */

#if defined(__H2XML__)
   uint8_t channel_mapping[0];
   /**< Array of channel mappings of size num_channels.

        Channel[i] mapping describes channel i. Each element i of the array
        describes channel i inside the buffer where i is less than
        num_channels.

        An unused channel is set to 0. */

   /*#< @h2xmle_description       {Array of channel mappings of size
                                   num_channels. \n
                                   Channel[i] mapping describes channel i. Each
                                   element i of the array describes channel i
                                   inside the buffer where i is less than
                                   num_channels. An unused channel is set to 0.}
        @h2xmle_variableArraySize {num_channels}
		@h2xmle_rangeEnum         {pcm_channel_map}
		@h2xmle_default           {1}
        @h2xmle_policy            {Basic} */
#endif
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;
typedef struct payload_pcm_output_format_cfg_t payload_pcm_output_format_cfg_t;


#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* COMMON_ENC_DEC_API_H */
