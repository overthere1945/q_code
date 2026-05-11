#pragma once
/*=============================================================================
  @file sns_tppe_request_handler.h

  Contains API required for encoding decoding tppe requests

  Copyright (c) 2022 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/
#include "pb_decode.h"
#include "pb_encode.h"
#include "sns_pb_util.h"
#include "sns_request.h"
#include "sns_std.pb.h"

#include "sns_tppe_messages.h"

bool decode_tppe_config(sns_request const *req, sns_std_request *std_request,
                        sns_all_filters const *const all_filters);

bool decode_tppe_config_sizing(sns_request const *req, sns_triggers_arg_sizing *sizing);
