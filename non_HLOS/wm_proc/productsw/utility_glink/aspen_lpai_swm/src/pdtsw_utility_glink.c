/*==============================================================================
                 Copyright (c) 2025 Qualcomm Technologies, Inc.
                                All Rights Reserved.
                 Confidential and Proprietary - Qualcomm Technologies, Inc.
==============================================================================*/
// $QTI_LICENSE_C$

#include <stringl.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "pdtsw_glink.h"
#include "pdtsw_worker_thread.h"
#include "pdtsw_utility_glink.h"
#ifdef CONFIG_QC_PDTSW_PSRAM_CLIENT_ENABLE
#include "pdtsw_psram_client.h"
#endif

LOG_MODULE_REGISTER(pdtsw_util_glink, CONFIG_QC_PDTSW_UTILITY_GLINK_LOG_LEVEL);
#define UTIL_GLINK_ERR(...)  LOG_ERR(__VA_ARGS__)
#define UTIL_GLINK_WRN(...)  LOG_WRN(__VA_ARGS__)
#define UTIL_GLINK_INF(...)  LOG_INF(__VA_ARGS__)
#define UTIL_GLINK_DBG(...)  LOG_DBG(__VA_ARGS__)

#define PDTSW_UTILITY_GLINK_AM_WM_CHANNEL_NAME    "am-wm-utility"

#define PDTSW_UTILITY_GLINK_AM_REMOTE_SS          "awm"

#define PDTSW_UTILITY_GLINK_XPORT_NAME            "SMEM"

/**
 * @brief Event types for GLINK utility operations
 */
typedef enum
{
    PDTSW_UTILITY_GLINK_EVT_LINK_STATE = 0,   /**< Link state change event */
    PDTSW_UTILITY_GLINK_EVT_CHANNEL_STATE,    /**< Channel state change event */
    PDTSW_UTILITY_GLINK_EVT_SIGNAL_CHANGE     /**< Signal change event */
} pdtsw_utility_glink_evt_t;

/**
 * @brief GLINK link identifiers
 */
typedef enum {
    PDTSW_UTILITY_GLINK_AM_WM_LINK = 0,   /**< AM-WM link */
    PDTSW_UTILITY_GLINK_NUM_LINKS         /**< Number of links */
} pdtsw_utility_glink_link_t;

/**
 * @brief Structure to hold GLINK channel information
 */
typedef struct {
    glink_handle_type handle;              /**< GLINK channel handle */
    glink_channel_event_type state;        /**< Channel state */
} pdtsw_utility_glink_channel_info_t;

/**
 * @brief Structure to hold GLINK link information
 */
typedef struct {
    glink_link_handle_type handle;         /**< GLINK link handle */
    glink_link_state_type state;           /**< Link state */
} pdtsw_utility_glink_link_info_t;

/**
 * @brief GLINK utility context structure
 *
 * Holds information about all links and channels
 */
typedef struct {
    pdtsw_utility_glink_link_info_t link_info[PDTSW_UTILITY_GLINK_NUM_LINKS];      /**< Array of link info */
    pdtsw_utility_glink_channel_info_t ch_info[PDTSW_UTILITY_GLINK_NUM_CHANNELS];  /**< Array of channel info */
} pdtsw_utility_glink_ctx_t;

/**
 * @brief Structure for GLINK utility workqueue messages
 */
typedef struct
{
    pdtsw_utility_glink_evt_t evt;             /**< Event type */
    pdtsw_utility_glink_channel_t channel;     /**< Channel identifier */
    size_t size;                              /**< Size of data */
    void* data;                               /**< Pointer to data */
} pdtsw_utility_glink_workq_msg_t;

/**
 * @brief Static context for GLINK utility
 */
static pdtsw_utility_glink_ctx_t pdtsw_utility_glink_ctx = 
{
    .link_info[PDTSW_UTILITY_GLINK_AM_WM_LINK] = {NULL, GLINK_LINK_STATE_DOWN},
    .ch_info[PDTSW_UTILITY_GLINK_AM_WM_CHANNEL] = {NULL, GLINK_LOCAL_DISCONNECTED}
};

/* forward declaration */
static void pdtsw_utility_glink_evt_handler(pdtsw_work_item_t *p_work_item);

/**
 * @brief Send a signal over the specified GLINK channel
 *
 * Sets a signal on the GLINK channel and verifies the local signal state
 *
 * @param[in] channel The GLINK channel to send the signal on
 * @param[in] signal The signal value to be sent
 *
 * @return pdtsw_utility_glink_ret_t Return status of the operation
 */
pdtsw_utility_glink_ret_t pdtsw_utility_glink_send_signal(
    pdtsw_utility_glink_channel_t channel, pdtsw_utility_glink_signal_t signal)
{
    /* Check if the channel is connected before sending the signal */
    if(pdtsw_utility_glink_ctx.ch_info[channel].state != GLINK_CONNECTED)
    {
        UTIL_GLINK_DBG("Chnl not connected. State: %d\n", 
            pdtsw_utility_glink_ctx.ch_info[channel].state);
        return PDTSW_UTILITY_GLINK_INVALID_STATE_ERR;
    }

    /* Set the signal on the GLINK channel */
    uint32_t tx_signal = signal.raw_value;
    glink_err_type glink_err = pdtsw_glink_sigs_set(
        pdtsw_utility_glink_ctx.ch_info[channel].handle, tx_signal);
    if(glink_err != GLINK_STATUS_SUCCESS)
    {
        UTIL_GLINK_DBG("pdtsw_glink_sigs_set() failed: %d\n", glink_err);
        return PDTSW_UTILITY_GLINK_ERROR;
    }

    /* Retrieve the local signal value to verify it matches the sent signal */
    uint32_t local_signal = 0;
    glink_err = pdtsw_glink_sigs_local_get(
        pdtsw_utility_glink_ctx.ch_info[channel].handle, &local_signal);
    if(glink_err != GLINK_STATUS_SUCCESS)
    {
        UTIL_GLINK_DBG("pdtsw_glink_sigs_local_get() failed: %d\n", glink_err);
        return PDTSW_UTILITY_GLINK_ERROR;
    }
    else if(local_signal != tx_signal)
    {
        UTIL_GLINK_DBG("pdtsw_glink_sigs_local_get() returned: %d\n", tx_signal);
        return PDTSW_UTILITY_GLINK_ERROR;
    }

    /* Signal sent and verified successfully */
    return PDTSW_UTILITY_GLINK_SUCCESS;
}

/**
 * @brief Process a remote signal received over GLINK
 *
 * Decodes the signal and invokes appropriate handlers based on signal ID
 *
 * @param[in] channel The glink channel specifying the remote subsystem
 * @param[in] remote_signal The signal value received from the remote side
 * 
 * @return void
 */
static void pdtsw_utility_glink_process_remote_signal(
    pdtsw_utility_glink_channel_t channel, uint32_t remote_signal)
{
    pdtsw_utility_glink_signal_t signal;
    signal.raw_value = remote_signal;

    /*Process based on the type of signal received */
    switch(signal.id)
    {
#ifdef CONFIG_QC_PDTSW_PSRAM_CLIENT_ENABLE
        case PDTSW_UTILITY_GLINK_SIGNAL_ID_PSRAM:
        {
            /* If the signal is for PSRAM, process it using the PSRAM arbiter client */
            pdtsw_psram_client_process_remote_signal(signal);
            break;
        }
#endif
        default:
        {
            /* For any other signal ID, do nothing */
            break;
        }
    }

    return;
}

/**
 * @brief Callback for AM-WM GLINK link state changes
 *
 * Handles link state changes and submits the event to the work queue
 *
 * @param[in] link_info Pointer to the link information structure
 * @param[in] priv Private data passed during registration
 *
 * @return void
 */
static void pdtsw_utility_glink_am_wm_link_state_cb(glink_link_info_type *link_info, void *priv)
{
    /* Check if link_info is NULL */
    if(link_info == NULL)
    {
        UTIL_GLINK_DBG("Arg is NULL\n");
        return;
    }

    /* Allocate memory for the work queue message */
    pdtsw_utility_glink_workq_msg_t *args = (pdtsw_utility_glink_workq_msg_t*)pdtsw_allocate_handler_args(
        sizeof(pdtsw_utility_glink_workq_msg_t) + sizeof(glink_link_state_type), NULL);
    if(NULL == args)
    {
        UTIL_GLINK_DBG("pdtsw_allocate_handler_args() returned NULL\n");
        return;
    }    

    /* Populate the work queue message with event details */
    args->evt = PDTSW_UTILITY_GLINK_EVT_LINK_STATE;
    args->channel = PDTSW_UTILITY_GLINK_AM_WM_CHANNEL;
    args->size = sizeof(glink_link_state_type);
    args->data = (void *)((uint8_t *)args + sizeof(pdtsw_utility_glink_workq_msg_t));
    memscpy(args->data, args->size, (void *)&link_info->link_state, sizeof(glink_link_state_type));
    
    /* Submit the event to the work queue for further processing */
    int ret = pdtsw_submit_to_workq(pdtsw_utility_glink_evt_handler, (void*)args, NULL);
    if(ret)
    {
        UTIL_GLINK_DBG("pdtsw_submit_to_workq() failed: %d\n", ret);
        return;
    }

    return;
}

/**
 * @brief Callback for GLINK channel state changes
 *
 * Handles channel state changes and submits the event to the work queue
 *
 * @param[in] handle GLINK channel handle
 * @param[in] priv Private data passed during channel open
 * @param[in] event Channel event type
 *
 * @return void
 */
static void pdtsw_utility_glink_chnl_state_cb(
    glink_handle_type handle, const void *priv,
    glink_channel_event_type event
)
{
    /* Allocate memory for the work queue message */
    pdtsw_utility_glink_workq_msg_t *args = (pdtsw_utility_glink_workq_msg_t*)pdtsw_allocate_handler_args(
        sizeof(pdtsw_utility_glink_workq_msg_t) + sizeof(glink_channel_event_type), NULL);
    if(NULL == args)
    {
        UTIL_GLINK_DBG("pdtsw_allocate_handler_args() returned NULL\n");
        return;
    }

    /* Populate the work queue message with event details */
    args->evt = PDTSW_UTILITY_GLINK_EVT_CHANNEL_STATE;
    args->channel = (pdtsw_utility_glink_channel_t)priv;
    args->size = sizeof(glink_channel_event_type);
    args->data = (void *)((uint8_t *)args + sizeof(pdtsw_utility_glink_workq_msg_t));
    memscpy(args->data, args->size, (void *)&event, sizeof(glink_channel_event_type));
    
    /* Submit the event to the work queue for further processing */
    int ret = pdtsw_submit_to_workq(pdtsw_utility_glink_evt_handler, (void*)args, NULL);
    if(ret)
    {
        UTIL_GLINK_DBG("pdtsw_submit_to_workq() failed: %d\n", ret);
        return;
    }

    return;
}

/**
 * @brief Callback for GLINK signal change events
 *
 * Handles signal changes and submits the event to the work queue
 *
 * @param[in] handle GLINK channel handle
 * @param[in] priv Private client data passed in glink_open
 * @param[in] prev Previous remote state
 * @param[in] curr Current remote state
 *
 * @return void
 */
static void pdtsw_utility_glink_signal_change_cb(
    glink_handle_type  handle,   /* handle for the glink channel */
    const void         *priv,    /* priv client data passed in glink_open  */
    uint32_t           prev,     /* Previous remote state */
    uint32_t           curr      /* Current remote state */
)
{
    /* Allocate memory for the work queue message */
    pdtsw_utility_glink_workq_msg_t *args = (pdtsw_utility_glink_workq_msg_t*)pdtsw_allocate_handler_args(
        sizeof(pdtsw_utility_glink_workq_msg_t) + sizeof(uint32_t), NULL);
    if(NULL == args)
    {
        UTIL_GLINK_DBG("pdtsw_allocate_handler_args() returned NULL\n");
        return;
    }

    /* Populate the work queue message with event details */
    args->evt = PDTSW_UTILITY_GLINK_EVT_SIGNAL_CHANGE;
    args->channel = (pdtsw_utility_glink_channel_t)priv;
    args->size = sizeof(uint32_t);
    args->data = (void *)((uint8_t *)args + sizeof(pdtsw_utility_glink_workq_msg_t));
    memscpy(args->data, args->size, (void *)&curr, sizeof(uint32_t));
    
    /* Submit the event to the work queue for further processing */
    int ret = pdtsw_submit_to_workq(pdtsw_utility_glink_evt_handler, (void*)args, NULL);
    if(ret)
    {
        UTIL_GLINK_DBG("pdtsw_submit_to_workq() failed: %d\n", ret);
        return;
    }

    return;
}

/* **** Dummy glink callbacks to allow successful glink open **** */
static void pdtsw_utility_glink_rx_cb(
  glink_handle_type handle,     /* handle for the glink channel */
  const void        *priv,      /* priv client data passed in glink_open */
  const void        *pkt_priv,  /* private client data assiciated with the
                                   rx intent that client queued earlier */
  const void        *ptr,       /* pointer to the received buffer */
  size_t            size,       /* size of the packet */
  size_t            intent_used /* size of the intent used for this packet */
)
{
    UTIL_GLINK_DBG("Called\n");
    return;
}

static bool pdtsw_utility_glink_rx_intent_req_cb(
  glink_handle_type         handle,   /* handle for the glink channel */
  const void                *priv,    /* priv client data passed in glink_open */
  size_t                    size      /* Intent size */
)
{
    UTIL_GLINK_DBG("Called\n");
    return false;
}

static void pdtsw_utility_glink_tx_done_cb(
  glink_handle_type handle,    /* handle for the glink channel */
  const void        *priv,     /* priv client data passed in glink_open */
  const void        *pkt_priv, /* private client data assiciated with the
                                  tx pkt that client queued earlier */
  const void        *ptr,      /* pointer to the transmitted buffer */
  size_t            size       /* size of the packet */
)
{
    UTIL_GLINK_DBG("Called\n");
    return;
}
/* **** **************************************************** **** */

/**
 * @brief Open an utility GLINK channel
 *
 * Configures and opens the specified utility GLINK channel
 *
 * @param[in] glink_ch The utility GLINK channel to open
 *
 * @return glink_err_type Status of the operation
 */
static glink_err_type pdtsw_utility_glink_open(pdtsw_utility_glink_channel_t glink_ch)
{
    glink_err_type glink_err = GLINK_STATUS_FAILURE;

    /* Check if the channel argument is valid */
    if(glink_ch >= PDTSW_UTILITY_GLINK_NUM_CHANNELS)
    {
        UTIL_GLINK_DBG("Invalid args\n");
        return glink_err;
    }

    /* Prepare the GLINK open configuration structure */
    glink_open_config_type open_cfg;
    GLINK_OPEN_CONFIG_INIT(open_cfg);

    /* Set channel-specific parameters */
    if(glink_ch == PDTSW_UTILITY_GLINK_AM_WM_CHANNEL)
    {
        open_cfg.name = PDTSW_UTILITY_GLINK_AM_WM_CHANNEL_NAME;
        open_cfg.remote_ss = PDTSW_UTILITY_GLINK_AM_REMOTE_SS;
    }
    else
    {
        return glink_err;
    }

    open_cfg.transport = PDTSW_UTILITY_GLINK_XPORT_NAME;
    /* open_cfg.notify_allocate = pdtsw_utility_allocate_cb; */
    /* open_cfg.notify_deallocate = pdtsw_utility_deallocate_cb; */
    open_cfg.notify_rx = pdtsw_utility_glink_rx_cb;
    open_cfg.notify_rx_intent_req = pdtsw_utility_glink_rx_intent_req_cb;
    open_cfg.notify_state = pdtsw_utility_glink_chnl_state_cb;
    open_cfg.notify_tx_done = pdtsw_utility_glink_tx_done_cb;
    open_cfg.notify_rx_sigs = pdtsw_utility_glink_signal_change_cb;
    /* open_cfg.options = GLINK_OPT_CLIENT_BUFFER_ALLOCATION; */
    open_cfg.priv = (void *)glink_ch;

    /* Attempt to open the GLINK channel */
    glink_err = pdtsw_glink_open(&open_cfg, &pdtsw_utility_glink_ctx.ch_info[glink_ch].handle);
    if(glink_err != GLINK_STATUS_SUCCESS)
    {
        UTIL_GLINK_DBG("Failed for ch:%d, err:%d\n", glink_ch, glink_err);
        return glink_err;
    }

    UTIL_GLINK_DBG("Success for ch:%d\n", glink_ch);
    return glink_err;
}

/**
 * @brief Close an utility GLINK channel
 *
 * Closes the specified utility GLINK channel
 *
 * @param[in] glink_ch The utility GLINK channel to close
 *
 * @return glink_err_type Status of the operation
 */
static glink_err_type pdtsw_utility_glink_close(pdtsw_utility_glink_channel_t glink_ch)
{
    glink_err_type glink_err = GLINK_STATUS_FAILURE;

    /* Check if the channel argument is valid */
    if(glink_ch >= PDTSW_UTILITY_GLINK_NUM_CHANNELS)
    {
        UTIL_GLINK_DBG("Invalid arg\n");
        return glink_err;
    }

    /* Attempt to close the GLINK channel */
    glink_err = pdtsw_glink_close(pdtsw_utility_glink_ctx.ch_info[glink_ch].handle);
    if(glink_err != GLINK_STATUS_SUCCESS)
    {
        UTIL_GLINK_DBG("Failed for ch: %d\n", glink_ch);
        return glink_err;
    }

    /* clear the channel handle */
    pdtsw_utility_glink_ctx.ch_info[glink_ch].handle = NULL;
    
    UTIL_GLINK_DBG("Success for ch: %d\n", glink_ch);
    return glink_err;
}

/**
 * @brief Handle GLINK link state events
 *
 * Processes link state changes and opens or closes channels accordingly
 *
 * @param[in] p_msg Pointer to the work queue message containing link state information
 *
 * @return void
 */
static void pdtsw_utility_glink_link_state_handler(pdtsw_utility_glink_workq_msg_t *p_msg)
{
    /* Check for null arguments */
    if((p_msg == NULL) || (p_msg->data == NULL))
    {
        UTIL_GLINK_DBG("Null args\n");
        return;
    }

    glink_err_type glink_err = GLINK_STATUS_FAILURE;
    /* Extract the link state from the message */
    glink_link_state_type link_state = *((glink_link_state_type *)p_msg->data);

    /* Determine which link is being referenced */
    pdtsw_utility_glink_link_t link;
    if(p_msg->channel == PDTSW_UTILITY_GLINK_AM_WM_CHANNEL)
    {
        link = PDTSW_UTILITY_GLINK_AM_WM_LINK;
    }
    else
    {
        return;
    }

    /* Update the context with the new link state */
    pdtsw_utility_glink_ctx.link_info[link].state = link_state;

    /* Handle link state transitions */
    if (GLINK_LINK_STATE_UP == link_state)
    {
        /* Open the channel if the link is up */
        glink_err = pdtsw_utility_glink_open(p_msg->channel);
        if(glink_err != GLINK_STATUS_SUCCESS)
        {
            UTIL_GLINK_DBG("pdtsw_utility_glink_open() for ch:%d err:%d\n", 
                p_msg->channel, glink_err);
        }
    }
    else if (GLINK_LINK_STATE_DOWN == link_state)
    {
        /* Close the channel if the link is down */
        glink_err = pdtsw_utility_glink_close(p_msg->channel);
        if(glink_err != GLINK_STATUS_SUCCESS)
        {
            UTIL_GLINK_DBG("pdtsw_utility_glink_close() for ch:%d err:%d\n", 
                p_msg->channel, glink_err);
        }
    }
    else
    {
        /* Handle invalid link state */
        UTIL_GLINK_DBG("Invalid Link State: %d\n", link_state);
    }

    return;
}

/**
 * @brief Handle GLINK channel state events
 *
 * Processes channel state changes and updates internal state
 *
 * @param[in] p_msg Pointer to the work queue message containing channel state information
 *
 * @return void
 */
static void pdtsw_utility_glink_chnl_state_handler(pdtsw_utility_glink_workq_msg_t *p_msg)
{
    /* Check for null arguments */
    if((p_msg == NULL) || (p_msg->data == NULL))
    {
        UTIL_GLINK_DBG("Null args\n");
        return;
    }

    /* Extract the channel event from the message */
    glink_channel_event_type ch_evt = *((glink_channel_event_type *)p_msg->data);
    switch (ch_evt)
    {
        case GLINK_CONNECTED:
        {
            /* Channel connected, update state */
            UTIL_GLINK_DBG("Ch:%d Connected\n", p_msg->channel);
            pdtsw_utility_glink_ctx.ch_info[p_msg->channel].state = ch_evt;
            break;
        }
        case GLINK_LOCAL_DISCONNECTED:
        {
            /* Channel locally disconnected */
            UTIL_GLINK_DBG("Ch:%d Local Disconnect\n", p_msg->channel);
            pdtsw_utility_glink_ctx.ch_info[p_msg->channel].state = ch_evt;
            break;
        }
        case GLINK_REMOTE_DISCONNECTED:
        {
            /* Channel remotely disconnected */
            UTIL_GLINK_DBG("Ch:%d Remote Disconnect\n", p_msg->channel);
            if(pdtsw_utility_glink_ctx.ch_info[p_msg->channel].state == GLINK_CONNECTED)
            {
                /* Close the channel if it was previously connected */
                glink_err_type glink_err = pdtsw_utility_glink_close(p_msg->channel);
                if(glink_err != GLINK_STATUS_SUCCESS)
                {
                    UTIL_GLINK_DBG("pdtsw_utility_glink_close() failed for ch:%d, err: %d\n", 
                        p_msg->channel, glink_err);
                    break;
                }
            }

            /* Update the channel state */
            pdtsw_utility_glink_ctx.ch_info[p_msg->channel].state = ch_evt;
            break;
        }
        default:
        {
            /* Handle invalid event */
            UTIL_GLINK_DBG("Invalid evt:%d\n", ch_evt);
            break;
        }
    }

    return;
}

/**
 * @brief Handle GLINK signal change events
 *
 * Retrieves and processes the remote signal state
 *
 * @param[in] p_msg Pointer to the work queue message containing signal change information
 *
 * @return void
 */
static void pdtsw_utility_glink_signal_change_handler(pdtsw_utility_glink_workq_msg_t *p_msg)
{
    /* Check for null arguments */
    if((p_msg == NULL) || (p_msg->data == NULL))
    {
        UTIL_GLINK_DBG("Null args\n");
        return;
    }

    /* Extract the remote signal from the message */
    uint32_t remote_signal = *((uint32_t *)p_msg->data);
    UTIL_GLINK_DBG("Rx Sig: %d\n", remote_signal);

    /* Process the remote signal */
    pdtsw_utility_glink_process_remote_signal(p_msg->channel, remote_signal);

    return;
}

/**
 * @brief Event handler for GLINK utility events.
 *
 * Dispatches events to appropriate handlers based on event type.
 *
 * @param[in] p_work_item Pointer to the work item containing event data
 *
 * @return void
 */
static void pdtsw_utility_glink_evt_handler(pdtsw_work_item_t *p_work_item)
{
    /* Retrieve the message from the work queue item */
    pdtsw_utility_glink_workq_msg_t *msg =
        (pdtsw_utility_glink_workq_msg_t*)pdtsw_get_workq_handler_args(p_work_item);
    if(msg == NULL)
    {
        UTIL_GLINK_DBG("pdtsw_get_workq_handler_args() returned NULL\n");
        return;
    }

    UTIL_GLINK_DBG("Evt: %d\n", msg->evt);

    /* Dispatch the event to the appropriate handler */
    switch (msg->evt)
    {
        case PDTSW_UTILITY_GLINK_EVT_LINK_STATE:
        {
            /* Handle link state event */
            pdtsw_utility_glink_link_state_handler(msg);
            break;
        }
        case PDTSW_UTILITY_GLINK_EVT_CHANNEL_STATE:
        {
            /* Handle channel state event */
            pdtsw_utility_glink_chnl_state_handler(msg);
            break;
        }
        case PDTSW_UTILITY_GLINK_EVT_SIGNAL_CHANGE:
        {
            /* Handle signal change event */
            pdtsw_utility_glink_signal_change_handler(msg);
            break;
        }
        default:
            /* Unknown event type */
            break;
    }
    
    /* Free the handler arguments after processing */
    pdtsw_free_handler_args(p_work_item, NULL);
}

/**
 * @brief Initialize the GLINK utility module
 *
 * Registers link state callbacks and initializes internal context
 *
 * @return int Returns 0 on success, -1 on failure
 */
int pdtsw_utility_glink_init()
{
    glink_err_type glink_err = GLINK_STATUS_FAILURE;

    /* Register for Glink link events. */
    glink_link_id_type link_info;
    GLINK_LINK_ID_STRUCT_INIT(link_info);
    link_info.xport = PDTSW_UTILITY_GLINK_XPORT_NAME;

    /* Register AM-WM link state callback */
    link_info.remote_ss = PDTSW_UTILITY_GLINK_AM_REMOTE_SS;
    link_info.link_notifier = pdtsw_utility_glink_am_wm_link_state_cb;
    glink_err = glink_register_link_state_cb(&link_info, NULL);
    if(GLINK_STATUS_SUCCESS != glink_err)
    {
        UTIL_GLINK_DBG("AM<->WM Link State Cb Reg Failed\n");
        return -1;
    }

    pdtsw_utility_glink_ctx.link_info[PDTSW_UTILITY_GLINK_AM_WM_LINK].handle = link_info.handle;

    UTIL_GLINK_ERR("WM: PDTSW Util Glink Init\n");

    return 0;
}
