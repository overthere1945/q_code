#pragma once
/** ===========================================================================
 * @file
 *
 * @brief This file contains sensor thread configuration.
 *
 * @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * All rights reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * ===========================================================================*/
/*==============================================================================
  Include Files
  ============================================================================*/

/*=============================================================================
  Macros
  ============================================================================*/

/**
 * @brief  Number of seconds low priority threads can run at a stretch.
 *
 */
#define SNS_THREAD_PRIO_LO_TIMEOUT_SEC 20

/**
 * @brief  Number of seconds high priority threads can run at a stretch.
 *
 */
#define SNS_THREAD_PRIO_HI_TIMEOUT_SEC 5

/**
 * @defgroup SNS_SENSOR_THREAD_NAMES Sensor Thread Names
 * @{
 */
/**
 * @brief Low memory thread: sns_lowmem
 */
#define SNS_LOW_MEM_THREAD "sns_lowmem"

/**
 * @brief ES low memory thread: sns_e_lowmem
 */
#define SNS_ES_LOW_MEM_THREAD "sns_e_lowmem"

/**
 * @brief QMI CM thread: sns_qmi_cm
 */
#define SNS_QMI_CM_THREAD "sns_qmi_cm"

/**
 * @brief GLINK CM thread: sns_cm
 */
#define SNS_GLINK_CM_THREAD "sns_glink_cm"

/**
 * @brief QSocket CM thread I: sns_qcm_I
 */
#define SNS_QSOCKET_CM_THREAD_I "sns_qcm_I"

/**
 * @brief QSocket CM thread N: sns_qcm_N
 */
#define SNS_QSOCKET_CM_THREAD_N "sns_qcm_N"

/**
 * @brief SEE H 0 thread: SNS_SEE_H_0
 */
#define SNS_SEE_H_0_THREAD "SNS_SEE_H_0"

/**
 * @brief SEE H 1 thread: SNS_SEE_H_1
 */
#define SNS_SEE_H_1_THREAD "SNS_SEE_H_1"

/**
 * @brief SIM data thread: sns_sim_data
 */
#define SNS_SIM_DATA_THREAD "sns_sim_data"

/**
 * @brief SEE L 0 thread: SNS_SEE_L_0
 */
#define SNS_SEE_L_0_THREAD "SNS_SEE_L_0"

/**
 * @brief SEE L 1 thread: SNS_SEE_L_1
 */
#define SNS_SEE_L_1_THREAD "SNS_SEE_L_1"

/**
 * @brief SEE L 2 thread: SNS_SEE_L_2
 */
#define SNS_SEE_L_2_THREAD "SNS_SEE_L_2"

/**
 * @brief SEE L 3 thread: SNS_SEE_L_3
 */
#define SNS_SEE_L_3_THREAD "SNS_SEE_L_3"

/**
 * @brief SIM DLF thread: sns_sim_dlf
 */
#define SNS_SIM_DLF_THREAD "sns_sim_dlf"

/**
 * @brief Registry thread: sns_registry
 */
#define SNS_REGISTRY_THREAD "sns_registry"

/**
 * @brief Mqueue server thread: sns_mq_srv
 */
#define SNS_MQ_SRV_THREAD "sns_mq_srv"

/**
 * @brief Mqueue client thread: sns_mq_clt
 */
#define SNS_MQ_CLT_THREAD "sns_mq_clt"

/**
 * @brief Diagnostic thread: sns_diag
 */
#define SNS_DIAG_THREAD "sns_diag"

/**
 * @brief Async UART thread: sns_a_uart
 */
#define SNS_ASYNC_UART_THREAD "sns_a_uart"

/**
 * @brief IRQ thread: sns_irq
 */
#define SNS_IRQ_THREAD "sns_irq"

/**
 * @brief Timer thread: sns_timer
 */
#define SNS_TIMER_THREAD "sns_timer"

/**
 * @brief RPS thread: sns_rps
 */
#define SNS_RPS_THREAD "sns_rps"

/**
 * @brief Signal I thread: sns_sig_i
 */
#define SNS_SIG_I_THREAD "sns_sig_i"

/**
 * @brief Signal NI thread: sns_sig_ni
 */
#define SNS_SIG_NI_THREAD "sns_sig_ni"

/**
 * @brief CCD service thread: sns_ccd_svc
 */
#define SNS_CCD_SERVICE_THREAD "sns_ccd_svc"

/**
 * @brief CCD SWI read thread: sns_ccd_swi
 */
#define SNS_CCD_SWI_READ_THREAD "sns_ccd_swi"

/**
 * @brief QSocket client Island thread: sns_qsoc_c_i
 */
#define SNS_QSOCKET_CLNT_I_THREAD "sns_qsoc_c_i"

/**
 * @brief QSocket client Bigimage thread: sns_qsoc_c_b
 */
#define SNS_QSOCKET_CLNT_B_THREAD "sns_qsoc_c_b"

/**
 * @brief SFE Service thread: sns_sfe_svc
 */
#define SNS_SFE_SERVICE_THREAD "sns_sfe_svc"

/**
 * @brief Init app thread: sns_init_app
 */
#define SNS_INIT_APP_THREAD "sns_init_app"
/** @} */

/*=============================================================================
  Type Definitions
  ============================================================================*/
