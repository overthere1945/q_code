#ifndef __DDIADC_H__
#define __DDIADC_H__

/*============================================================================
  @file DDIAdc.h

  ADC Device Driver Interface header

  Clients may include this header and use these interfaces to read analog
  inputs.

               Copyright (c) Qualcomm Technologies, Inc.
               All Rights Reserved.
               Qualcomm Technologies Proprietary and Confidential.
============================================================================*/
/* $Header: //components/rel/core.qdsp6/8.2.3/api/common/pmic/adc/DDIAdc.h#1 $ */

/*-------------------------------------------------------------------------
 * Include Files
 * ----------------------------------------------------------------------*/
#include "DalDevice.h"
#include "DALSys.h"
#include "DALDeviceId.h"
#include "AdcInputs.h"
#include "PmicAdcTypes.h"

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * ----------------------------------------------------------------------*/
#define DALADC_INTERFACE_VERSION DALINTERFACE_VERSION(4,5)


typedef struct DalAdc DalAdc;
struct DalAdc
{
   struct DalDevice DalDevice;
   DALResult (*RequestConversion)(uint32_t ClientHandle, DalDeviceHandle *_h, const AdcRequestParametersType *pAdcParams, AdcRequestStatusType *pAdcRequestStatus);
   DALResult (*RequestRecalibration)(uint32_t ClientHandle, DalDeviceHandle *_h, const AdcRequestParametersType *pAdcParams);
   DALResult (*GetAdcInputProperties)(uint32_t ClientHandle, DalDeviceHandle *_h, const char *pInputName, uint32_t nInputNameLen, AdcInputPropertiesType *pAdcInputProps, uint32_t uOutLen);
   DALResult (*TMGetInputProperties)(uint32_t ClientHandle, DalDeviceHandle *_h, const char *pInputName, uint32_t nInputNameLen, AdcTMInputPropertiesType *pAdcTMInputProps);
   DALResult (*TMGetRange)(uint32_t ClientHandle, DalDeviceHandle *_h, const AdcTMInputPropertiesType *pAdcTMInputProps, AdcTMRangeType *pAdcTMRange);
   DALResult (*TMSetThreshold)(uint32_t ClientHandle, DalDeviceHandle *_h, const AdcTMRequestParametersType *pAdcTMParams, AdcTMThresholdType eThreshold, const int32_t *pnThresholdDesired, int32_t *pnThresholdSet);
   DALResult (*TMSetEnableThresholds)(uint32_t ClientHandle, DalDeviceHandle *_h, DALBOOL bEnable);
   DALResult (*Read)(uint32_t ClientHandle, DalDeviceHandle *_h, const AdcInputPropertiesType *pAdcInputProps, uint32_t uInputLen, AdcResultType *pAdcResult, uint32_t uOutLen);
   DALResult (*TMSetTolerance)(uint32_t ClientHandle, DalDeviceHandle *_h, const AdcTMRequestParametersType *pAdcTMParams, int32_t nPhysicalCurrent, const int32_t *pnLowerTolerance, const int32_t *pnHigherTolerance);
   DALResult (*ProcessConversionResult)(uint32_t ClientHandle, DalDeviceHandle *_h, uint32_t uAdcCode, const AdcInputPropertiesType *pAdcInputProps, uint32_t uInputLen, AdcResultType *pAdcResult, uint32_t uOutLen, uint32_t* aOutLen);
   DALResult (*ProcessConversionResultInverse)(uint32_t ClientHandle, DalDeviceHandle *_h, int32_t nPhysical, const AdcInputPropertiesType *pAdcInputProps, uint32_t uInputLen, AdcResultType *pAdcResult, uint32_t uOutLen, uint32_t* aOutLen);
};

typedef struct DalAdcHandle DalAdcHandle;
struct DalAdcHandle
{
   uint32_t dwDalHandleId;
   const DalAdc *pVtbl;
   void *pClientCtxt;
   uint32_t dwVtblen;
};

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 * ----------------------------------------------------------------------*/
/**
   @brief Attaches to the ADC driver

   This function attaches the client to the ADC driver. This function is used
   to obtain the device handle which is required to use the ADC driver APIs.

   @param  DevId [in] The device ID to attach to (e.g., DALDEVICEID_ADC)
   @param  hDalDevice [out] Pointer to DAL device handle pointer which will receive
                             a pointer to the ADC device handle

   @return DAL_SUCCESS if the attach was successful. Other return values
           indicate that an error occurred.

*/
#define DAL_AdcDeviceAttach(DevId,hDalDevice)\
        DAL_DeviceAttachEx(NULL,DevId,DALADC_INTERFACE_VERSION,hDalDevice)

/**
   @brief Gets the properties for an analog input from the ADC DAL properties

   This function looks up the properties for an analog input.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  pInputName [in] The name of the analog input to look up
   @param  nInputNameSize [in] The size of the analog input name in bytes
   @param  pAdcInputProps [out] The buffer to write the property values to

   @return DAL_SUCCESS if the input was found. DAL_SUCCESS if the input was
      found in the properties file.

      ADC_ERROR_INVALID_PROPERTY_LENGTH is returned if the
      property was incorrectly defined.

      All other values correspond to DAL error codes, and should
      generally be interpreted to mean that the analog input is
      not present on the target hardware.

*/
static __inline DALResult
DalAdc_GetAdcInputProperties(DalDeviceHandle *_h,  const char *pInputName,  uint32_t nInputNameSize,  AdcInputPropertiesType *pAdcInputProps)
{
   if (DALISREMOTEHANDLE(_h))
   {
      DalRemoteHandle *h_remote = (DalRemoteHandle *)DALPREPREMOTEHANDLE(_h);
      return (h_remote->pVtbl->FCN_8(DALVTBLIDX(((DalAdcHandle*)_h)->pVtbl, GetAdcInputProperties),_h, (void*)pInputName, nInputNameSize, (void*)pAdcInputProps, sizeof(AdcInputPropertiesType)));
   }
   return ((DalAdcHandle *)_h)->pVtbl->GetAdcInputProperties(DALDEVICE_LOCAL_CLIENTID, _h, pInputName, nInputNameSize, pAdcInputProps, sizeof(AdcInputPropertiesType));
}

/**
   @brief Reads an analog input

   This function performs a blocking ADC read for the device and channel specified by the
   client in pAdcInputProps.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  pAdcInputProps [in] Pointer to input properties for the channel
   @param  pAdcResult [out] ADC conversion result

   @see    DAL_AdcDeviceAttach

   @return DAL_SUCCESS if the request was successfully queued.

           Other return values indicate an error and include:
           ADC_ERROR_INVALID_DEVICE_IDX - the client specified an invalid device index
           ADC_ERROR_INVALID_CHANNEL_IDX - the client specified an invalid channel index
           ADC_ERROR_NULL_POINTER - pAdcInputProps or pAdcResult was NULL

*/
static __inline DALResult
DalAdc_Read(DalDeviceHandle *_h,  const AdcInputPropertiesType *pAdcInputProps,  AdcResultType *pAdcResult)
{
   if (DALISREMOTEHANDLE(_h))
   {
      DalRemoteHandle *h_remote = (DalRemoteHandle *)DALPREPREMOTEHANDLE(_h);
      return (h_remote->pVtbl->FCN_8(DALVTBLIDX(((DalAdcHandle*)_h)->pVtbl, Read),_h, (void*)pAdcInputProps, sizeof(AdcInputPropertiesType), (void*)pAdcResult, sizeof(AdcResultType)));
   }
   return ((DalAdcHandle *)_h)->pVtbl->Read(DALDEVICE_LOCAL_CLIENTID, _h, pAdcInputProps, sizeof(AdcInputPropertiesType), pAdcResult, sizeof(AdcResultType));
}

/**
   @brief Reads an analog input

   This function requests the ADC driver to start an ADC conversion on
   the device and channel specified by the client in pAdcParams.

   Conversion requests are processed on a FIFO basis.

   If the hEvent parameter in pAdcParams is a DAL callback event triggered by the
   ADC driver when the conversion is complete. The payload of the callback is a pointer
   to an AdcResultType struct which contains the result of the ADC conversion.

   If hEvent in pAdcParams is NULL, this function will return ADC_ERROR_NULL_POINTER.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  pAdcParams [in] Pointer to client's request parameters
   @param  pAdcRequestStatus [out] Pointer to buffer which will indicate the status of request
                        (Client can pass NULL for pAdcRequestStatus if status is unwanted)

   @see    DAL_AdcDeviceAttach

   @return DAL_SUCCESS if the request was successfully queued.

           Other return values indicate an error and include:
           ADC_ERROR_INVALID_DEVICE_IDX - the client specified an invalid device index
           ADC_ERROR_INVALID_CHANNEL_IDX - the client specified an invalid channel index
           ADC_ERROR_NULL_POINTER - pAdcParams, pAdcParams->hEvent, or pAdcResult was NULL
           ADC_ERROR_DEVICE_QUEUE_FULL - the device request queue was full

*/
static __inline DALResult
DalAdc_RequestConversion(DalDeviceHandle *_h,  const AdcRequestParametersType *pAdcParams,  AdcRequestStatusType *pAdcRequestStatus)
{
   if (DALISREMOTEHANDLE(_h))
   {
      return ADC_ERROR_API_UNSUPPORTED_IN_THIS_CONTEXT;
   }
   return ((DalAdcHandle *)_h)->pVtbl->RequestConversion(DALDEVICE_LOCAL_CLIENTID, _h, pAdcParams, pAdcRequestStatus);
}

/**
   @brief Calibrates an analog input

   This function requests the ADC driver to recalibrate the specified
   channel. If other channels share the physical device configuration
   (as defined in the ADC device BSP), then those channels may also be
   updated by the recalibration.

   The hEvent parameter in pAdcParams is a DAL callback event triggered by the
   ADC driver when the recalibration is complete. The payload of the callback is a pointer
   to an AdcRecalibrationResultType struct which contains the result of the ADC recalibration.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  pAdcParams [in] Pointer to client's request parameters

   @see    DAL_AdcDeviceAttach

   @return DAL_SUCCESS if the request was successfully queued.

           Other return values indicate an error and include:
           ADC_ERROR_INVALID_DEVICE_IDX - the client specified an invalid device index
           ADC_ERROR_INVALID_CHANNEL_IDX - the client specified an invalid channel index
           ADC_ERROR_NULL_POINTER - pAdcParams was NULL
           ADC_ERROR_DEVICE_QUEUE_FULL - the device request queue was full

*/
static __inline DALResult
DalAdc_RequestRecalibration(DalDeviceHandle *_h,  const AdcRequestParametersType *pAdcParams)
{
   if (DALISREMOTEHANDLE(_h))
   {
      return ADC_ERROR_API_UNSUPPORTED_IN_THIS_CONTEXT;
   }
   return ((DalAdcHandle *)_h)->pVtbl->RequestRecalibration(DALDEVICE_LOCAL_CLIENTID, _h, pAdcParams);
}

/**
   @brief Gets the input properties for a given ADC TM channel.

   This function looks up the properties for an analog input.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  pInputName [in] The name of the analog input to look up
   @param  nInputNameLen [in] The size of the analog input name in bytes
   @param  pAdcTMInputProps [out] The buffer to write the property values to

   @return DAL_SUCCESS if the input was found. DAL_SUCCESS if the input was
      found in the properties file.

      ADC_ERROR_INVALID_PROPERTY_LENGTH is returned if the
      property was incorrectly defined.

      All other values correspond to DAL error codes, and should
      generally be interpreted to mean that the analog input is
      not present on the target hardware.

*/
static __inline DALResult
DalAdc_TMGetInputProperties(DalDeviceHandle * _h, const char *pInputName, uint32_t nInputNameLen, AdcTMInputPropertiesType *pAdcTMInputProps)
{
   if (DALISREMOTEHANDLE(_h))
   {
      return ADC_ERROR_API_UNSUPPORTED_IN_THIS_CONTEXT;
   }
   return ((DalAdcHandle *)_h)->pVtbl->TMGetInputProperties(DALDEVICE_LOCAL_CLIENTID, _h, pInputName, nInputNameLen, pAdcTMInputProps);
}

/**
   @brief Gets the range of physical values that can be set for a given TM channel.

   This function gets the minimum and maximum physical value that can be set as a
   threshold for a given VADC TM channel.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  pAdcTMInputProps [in] The VADC TM channel obtained by calling DalAdc_TMGetInputProperties
   @param  pAdcTMRange [out] Structure with minimum and maximum physical values

   @return DAL_SUCCESS if the ranges were successfully determined.

           Other return values indicate an error and include:
           ADC_ERROR_INVALID_DEVICE_IDX - the client specified an invalid device index
           ADC_ERROR_INVALID_CHANNEL_IDX - the client specified an invalid channel index
           ADC_ERROR_NULL_POINTER - pAdcTMInputProps or pAdcTMRange were NULL
*/
static __inline DALResult
DalAdc_TMGetRange(DalDeviceHandle * _h, const AdcTMInputPropertiesType *pAdcTMInputProps, AdcTMRangeType *pAdcTMRange)
{
   if (DALISREMOTEHANDLE(_h))
   {
      return ADC_ERROR_API_UNSUPPORTED_IN_THIS_CONTEXT;
   }
   return ((DalAdcHandle *)_h)->pVtbl->TMGetRange(DALDEVICE_LOCAL_CLIENTID, _h, pAdcTMInputProps, pAdcTMRange);
}

/**
   @brief Sets a threshold with the ADC TM

   The threshold event will be triggered once when the threshold is crossed:
    - ADC_TM_THRESHOLD_LOWER: current reading <= *pnThresholdDesired
    - ADC_TM_THRESHOLD_HIGHER: current reading >= *pnThresholdDesired
   After the event is triggered, the threshold will not trigger the event again
   and will be in a triggered state until the client calls DalAdc_TMSetThreshold
   to set a new threshold.

   The event type can be:
    - DALSYS_EVENT_ATTR_NORMAL: the event will be triggered.
    - DALSYS_EVENT_ATTR_CALLBACK_EVENT: the callback will have a payload pointing
      to a AdcTMCallbackPayloadType struct.

   Note that thresholds can be disabled/re-enabled on a per client basis by calling
   DalAdc_TMSetEnableThresholds. Thresholds are enabled by default, but calling
   DalAdc_TMSetThreshold does not automatically re-enable them if they were previously
   disabled by a call to DalAdc_TMSetEnableThresholds.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  pAdcTMParams [in] Pointer to client's request parameters
           ->adcTMInputProps: [in] input properties from DalAdc_TMGetInputProperties
           ->hEvent: [in] Threshold event that gets set
   @param  eThreshold [in] Type of threshold to monitor
   @param  pnThresholdDesired [in] Threshold value to set in physical units; if NULL the
                                    current threshold is cleared.
   @param  pnThresholdSet [out] The actual threshold value that was set in physical units.

   @see    DAL_AdcDeviceAttach

   @return DAL_SUCCESS if the threshold was successfully set.

           Other return values indicate an error and include:
           ADC_ERROR_OUT_OF_TM_CLIENTS - there are no free TM client handles
           ADC_ERROR_TM_THRESHOLD_OUT_OF_RANGE - the requested threshold is out of range
           ADC_ERROR_TM_BUSY - busy monitoring a tolerance
           ADC_ERROR_INVALID_DEVICE_IDX - the client specified an invalid device index
           ADC_ERROR_INVALID_CHANNEL_IDX - the client specified an invalid channel index
           ADC_ERROR_NULL_POINTER - pAdcParams, pAdcParams->hEvent, or pAdcResult was NULL

*/
static __inline DALResult
DalAdc_TMSetThreshold(DalDeviceHandle *_h, const AdcTMRequestParametersType *pAdcTMParams, AdcTMThresholdType eThreshold, const int32_t *pnThresholdDesired, int32_t *pnThresholdSet)
{
   if (DALISREMOTEHANDLE(_h))
   {
      return ADC_ERROR_API_UNSUPPORTED_IN_THIS_CONTEXT;
   }
   return ((DalAdcHandle *)_h)->pVtbl->TMSetThreshold(DALDEVICE_LOCAL_CLIENTID, _h, pAdcTMParams, eThreshold, pnThresholdDesired, pnThresholdSet);
}

/**
   @brief Enables / disables thresholds for the ADC TM

   By default, thresholds are enabled.

   Thresholds are not monitored while the thresholds are disabled,
   and any threshold crossings which occurred while the thresholds were disabled
   are ignored.

   Threshold values and event handles set by DalAdc_TMSetThreshold
   are still retained while thresholds are disabled.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  bEnable [in] true: enable, false: disable

   @see    DAL_AdcDeviceAttach

   @return DAL_SUCCESS on success.

           Other return values indicate an error and include:
           ADC_ERROR_OUT_OF_TM_CLIENTS - there are no free TM client handles

*/
static __inline DALResult
DalAdc_TMSetEnableThresholds(DalDeviceHandle * _h, DALBOOL bEnable)
{
   if (DALISREMOTEHANDLE(_h))
   {
      return ADC_ERROR_API_UNSUPPORTED_IN_THIS_CONTEXT;
   }
   return ((DalAdcHandle *)_h)->pVtbl->TMSetEnableThresholds(DALDEVICE_LOCAL_CLIENTID, _h, bEnable);
}

/**
   @brief Sets thresholds based on an allowable tolerance / delta.

   This API allows clients to specify a tolerance for how much the measurement can
   change by before being notified, e.g. tell me when XO_THERM changes by 0.02
   degrees C. Thresholds will be set based on the current measurement value +/- the
   allowable delta.

   Once the tolerance has been reached or exceeded, ADC will notify the client and
   automatically set new thresholds for the tolerance. Clients must clear the
   tolerances for ADC to stop monitoring. Tolerances can be cleared by setting
   a NULL value.

   Clients can set or clear either a low tolerance, high tolerance or both during
   the same function call. If the client is already monitoring a tolerance then
   setting a new tolerance will result in an update to the previously set
   tolerance, i.e. the new tolerance will replace the old tolerance.

   A client can set either a threshold or a tolerance on any one measurement but not
   both at the same time. To allow a threshold to be set after registering a
   tolerance, the tolerance should be cleared by passing in NULL parameters for the
   tolerances.

   The client event will be triggered when the tolerance is met or exceeded:
    - Lower: event will trigger when current_value <= original_value - tolerance
    - Upper: event will trigger when current_value >= original_value + tolerance

   @param[in]  _h Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param[in]  pAdcTMParams Pointer to client's request parameters
               ->adcTMInputProps: input properties from DalAdc_TMGetInputProperties
               ->hEvent: threshold event that gets set
   @param[in]  nPhysicalCurrent current physical value of the channel; initial
               thresholds will be (nPhysical - *pnLowerTolerance) and
               (nPhysical + *pnHigherTolerance)
   @param[in]  pnLowerTolerance lower tolerance value; NULL to clear / not set
   @param[in]  pnHigherTolerance higher tolerance value; NULL to clear / not set

   @see    DAL_AdcDeviceAttach

   @return DAL_SUCCESS on success.

           Other return values indicate an error and include:
           ADC_ERROR_OUT_OF_TM_CLIENTS - there are no free TM client handles
           ADC_ERROR_TM_THRESHOLD_OUT_OF_RANGE - the requested threshold is out of range
           ADC_ERROR_TM_BUSY - busy monitoring thresholds that were set
           ADC_ERROR_INVALID_DEVICE_IDX - the client specified an invalid device index
           ADC_ERROR_INVALID_CHANNEL_IDX - the client specified an invalid channel index
           ADC_ERROR_NULL_POINTER - pAdcParams, pAdcParams->hEvent, or pAdcResult was NULL

*/
static __inline DALResult
DalAdc_TMSetTolerance(DalDeviceHandle *_h, const AdcTMRequestParametersType *pAdcTMParams, int32_t nPhysicalCurrent, const int32_t *pnLowerTolerance, const int32_t *pnHigherTolerance)
{
   if (DALISREMOTEHANDLE(_h))
   {
      return ADC_ERROR_API_UNSUPPORTED_IN_THIS_CONTEXT;
   }
   return ((DalAdcHandle *)_h)->pVtbl->TMSetTolerance(DALDEVICE_LOCAL_CLIENTID, _h, pAdcTMParams, nPhysicalCurrent, pnLowerTolerance, pnHigherTolerance);
}

/**
   @brief Scales ADC code to usable values

   This function scales ADC code to usable values, including microvolts and physical values.
   Physical units for the different ADC channels are given in AdcInputs.h.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  pAdcInputProps [in] Pointer to input properties for the channel
   @param  uAdcCode [in] The ADC code
   @param  pAdcResult [out] ADC conversion result

   @see    DAL_AdcDeviceAttach

   @return DAL_SUCCESS if the request was successfully queued.

           Other return values indicate an error and include:
           ADC_ERROR_INVALID_DEVICE_IDX - the client specified an invalid device index
           ADC_ERROR_INVALID_CHANNEL_IDX - the client specified an invalid channel index
           ADC_ERROR_NULL_POINTER - pAdcInputProps or pAdcResult was NULL

*/
static __inline DALResult
DalAdc_ProcessConversionResult(DalDeviceHandle *_h, const AdcInputPropertiesType *pAdcInputProps, uint32_t uAdcCode, AdcResultType *pAdcResult)
{
   if (DALISREMOTEHANDLE(_h))
   {
      DalRemoteHandle *h_remote = (DalRemoteHandle *)DALPREPREMOTEHANDLE(_h);
      return (h_remote->pVtbl->FCN_10((DALVTBLIDX(((DalAdcHandle*)_h)->pVtbl, ProcessConversionResult)), _h, uAdcCode, (void*)pAdcInputProps, sizeof(AdcInputPropertiesType), (void*)pAdcResult, sizeof(AdcResultType), NULL));
   }
   return ((DalAdcHandle *)_h)->pVtbl->ProcessConversionResult(DALDEVICE_LOCAL_CLIENTID, _h, uAdcCode, pAdcInputProps, sizeof(AdcInputPropertiesType), pAdcResult, sizeof(AdcResultType), NULL);
}

/**
   @brief Scales a physical value to ADC code

   This function scales physical values to ADC code and other intermediate values, e.g. microvolts.
   Physical units for the different ADC channels are given in AdcInputs.h.

   @param  _h [in] Handle to the ADC driver instance instantiated by DAL_AdcDeviceAttach
   @param  pAdcInputProps [in] Pointer to input properties for the channel
   @param  nPhysical [in] Physical value
   @param  pAdcResult [out] ADC conversion result

   @see    DAL_AdcDeviceAttach

   @return DAL_SUCCESS if the request was successfully queued.

           Other return values indicate an error and include:
           ADC_ERROR_INVALID_DEVICE_IDX - the client specified an invalid device index
           ADC_ERROR_INVALID_CHANNEL_IDX - the client specified an invalid channel index
           ADC_ERROR_NULL_POINTER - pAdcInputProps or pAdcResult was NULL

*/
static __inline DALResult
DalAdc_ProcessConversionResultInverse(DalDeviceHandle *_h, const AdcInputPropertiesType *pAdcInputProps, int32_t nPhysical, AdcResultType *pAdcResult)
{
   if (DALISREMOTEHANDLE(_h))
   {
      DalRemoteHandle *h_remote = (DalRemoteHandle *)DALPREPREMOTEHANDLE(_h);
      return (h_remote->pVtbl->FCN_10((DALVTBLIDX(((DalAdcHandle*)_h)->pVtbl, ProcessConversionResultInverse)), _h, nPhysical, (void*)pAdcInputProps, sizeof(AdcInputPropertiesType), (void*)pAdcResult, sizeof(AdcResultType), NULL));
   }
   return ((DalAdcHandle *)_h)->pVtbl->ProcessConversionResultInverse(DALDEVICE_LOCAL_CLIENTID, _h, nPhysical, pAdcInputProps, sizeof(AdcInputPropertiesType), pAdcResult, sizeof(AdcResultType), NULL);
}

#endif

