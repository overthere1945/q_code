/**
    @file  fw_config.c
    @brief fw loading implementation
 */
/*
===============================================================================

                               Edit History

$Header:

when       who     what, where, why
--------   ---     ------------------------------------------------------------
11/07/25   GKR     Vote for SSC GDSC during FW loading
11/03/25   SS      Fix to configure REG80 Image register for 3W SPI.
10/15/25   GKR     Added changes to support RUMI
05/22/24   GKR     Changes to collapse IBI CD1 and IBI CD2 in case of non IBI usecases
05/13/25   RK      Added SWA for CESTA bug on aspen
10/01/24   KSN     Updated changes for SPI_3W support
09/09/24   GKR     Added RESET Frequency with 0 & FORCE DOMAIN ENABLE Flag for Common clocks & SE Clocks
08/19/24   GKR     Removed WA to Keep Uclock vote as we migrated to new clock APIs
06/26/24   GKR     FW Loading Driver update to support multiple SSC QUPs
*/
/*=============================================================================
            Copyright (c) Qualcomm Technologies, Incorporated.
                              All rights reserved.
              Qualcomm Technologies, Confidential and Proprietary.
=============================================================================*/

#include "qup_common.h"
#include "fw_config.h"
#include "fw_devcfg.h"
#include "qup_devcfg.h"
#include "qup_alloc.h"
#include "qup_plat.h"
#include "qup_log.h"
#include "qup_hal.h"
#include "gpi.h"
#include "DALSys.h"
#include "DDIHWIO.h"
#include "Clock.h"
#include "ClockDefs.h"
#include "ClockImage.h"
#include "qup_hwio.h"
#include "qup_ibi_hwio.h"
#include "HALhwio.h"
#include "DALDeviceId.h"
#include "DDIPlatformInfo.h"
#include <stringl/stringl.h>

#include "DTBExtnLib.h"
#include "qurt.h"

#ifdef QUP_KPI_PROFILING
#include "CoreTime.h"
#endif

#define FW_LOAD_FREQUENCY_HZ 19200000
#define IBI_FREQ_KHZ      38400

#define IS_LEGACY(x)          (x != 0x104)

#define MAX_IBI_CD_DOMAINS 3

#ifndef UNUSED

#define UNUSED(x) (void)x

#endif

#ifdef QUP_KPI_PROFILING
qup_fw_load_kpi qup_fw_loading_kpi[NUM_SSC_QUP][MAX_SE_PER_SSC_QUP];
qup_fw_load_kpi gsi_fw_loading_kpi[NUM_SSC_QUP];
#endif

#define SSC_FW_DT_NAME "/soc/ssc_qup_fw_cfg"

extern   ClockHandle qup_clock_handle;
static   uint32      qupv3_hw_ver                                  = 0;
static   boolean     is_platform_rumi                              = FALSE;
volatile boolean     se_fw_loaded[NUM_SSC_QUP][MAX_SE_PER_SSC_QUP] = { FALSE };
volatile boolean     qup_fw_loaded                                 = FALSE;
static   uint32      ibi_cd_domain_enable_bmsk = 0;
// t32 control
volatile boolean disable_fw_loading = FALSE;

void qup_fw_init (void);

boolean fw_disable_se_clock (uint32 ssc_qup_id, uint32 se_index)
{
    ClockResult res;
    ClockIdType se_clk_id;
    char se_name_str[50];
    int ret_value = 0;

    fdt_node_handle node;

    if (qup_clock_handle == NULL) { return FALSE; }

    ret_value = snprintf(se_name_str, sizeof(se_name_str), "/soc/SSC_QUP_%lu/SSC_QUP_%lu_SE_%lu",ssc_qup_id, ssc_qup_id, se_index);
    if (ret_value < 0 || ret_value >= sizeof(se_name_str)) {
        QUP_LOG(LEVEL_ERROR, "fw_disable_se_clock: snprintf failed or buffer too small\n");
        return FALSE;
    }

    ret_value = fdt_get_node_handle(&node, NULL, se_name_str);
    if (ret_value) return FALSE;

    ret_value = Clock_GetIdDT(qup_clock_handle, &node, "se-clk", &se_clk_id);
    if (ret_value) { return FALSE; }


    res = Clock_Disable(qup_clock_handle, se_clk_id);
    if (CLOCK_SUCCESS != res) { return FALSE; }

    res = Clock_SetFrequencyEx(qup_clock_handle, se_clk_id, 0, CLOCK_FREQUENCY_HZ_EXACT, NULL, CLOCK_FLAG_FORCE_DOMAIN, 0);
    if (CLOCK_SUCCESS != res) { return FALSE; }

    return TRUE;
}

boolean fw_enable_se_clock (uint32 ssc_qup_id, uint32 se_index, boolean enable_dfs)
{
    ClockResult res = CLOCK_SUCCESS;
    ClockIdType se_clk_id = 0;
    char se_name_str[50];
    int ret_value = 0;

    fdt_node_handle node;

    if (qup_clock_handle == NULL) { return FALSE; }

    ret_value = snprintf(se_name_str, sizeof(se_name_str), "/soc/SSC_QUP_%lu/SSC_QUP_%lu_SE_%lu",ssc_qup_id, ssc_qup_id, se_index);
    if (ret_value < 0 || ret_value >= sizeof(se_name_str)) {
        QUP_LOG(LEVEL_ERROR, "fw_enable_se_clock: snprintf failed or buffer too small\n");
        return FALSE;
    }

    ret_value = fdt_get_node_handle(&node, NULL, se_name_str);
    if (ret_value) return FALSE;

    ret_value = Clock_GetIdDT(qup_clock_handle, &node, "se-clk", &se_clk_id);
    if (ret_value) { return FALSE; }

    if (enable_dfs)
    {
        res = Clock_Configure(qup_clock_handle, se_clk_id, CLOCK_CONFIG_DFS_ENABLE);
        if (CLOCK_SUCCESS != res) { return FALSE; }
    }

    res = Clock_Configure(qup_clock_handle, se_clk_id, CLOCK_CONFIG_MEM_CORE_FORCE_ON);
    if (CLOCK_SUCCESS != res) { return FALSE; }

    res = Clock_SetFrequencyEx(qup_clock_handle, se_clk_id, FW_LOAD_FREQUENCY_HZ, CLOCK_FREQUENCY_HZ_EXACT, NULL, CLOCK_FLAG_FORCE_DOMAIN, 0);
    if (CLOCK_SUCCESS != res) { return FALSE; }

    res = Clock_Enable(qup_clock_handle, se_clk_id);
    if (CLOCK_SUCCESS != res) { return FALSE; }

    return TRUE;
}

/**
 * @brief Enable or disable all IBI CD (Clock Domain) clocks.
 *
 * This function iterates through all IBI CD domains defined under the device tree node
 * "/soc/ssc_ibi_cd_domains", retrieves their clock IDs, and enables or disables them
 * based on the input flag.
 *
 * @param enable Boolean flag:
 *               - TRUE  -> Enable all IBI CD clocks
 *               - FALSE -> Disable all IBI CD clocks
 *
 * @return TRUE if all operations succeed or if no more domains are found.
 *         FALSE if any error occurs during node retrieval, clock ID lookup,
 *         or clock enable/disable operations.
 */
static boolean fw_set_ibi_cd_domains(boolean enable)
{
    fdt_node_handle node;
    int ret_value = 0;
    ClockIdType ibi_cd_id = 0;
    ClockResult res = CLOCK_SUCCESS;
    int i = 0;
    char ibi_cd_name[16];  // Increased size to safely hold names like "ibi_cd10"

    /*-----------------------------------------------------------------------*/
    /* Retrieve the node handle for the IBI CD domains from the device tree */
    /*-----------------------------------------------------------------------*/
    ret_value = fdt_get_node_handle(&node, NULL, "/soc/ssc_ibi_cd_domains");
    if (ret_value) {
        QUP_LOG(LEVEL_ERROR, "Failed to get node handle for /soc/ssc_ibi_cd_domains\n");
        return FALSE;
    }

    /*-----------------------------------------------------------------------*/
    /* Iterate through all possible IBI CD domain indices                    */
    /*-----------------------------------------------------------------------*/
    for (i = 0; i < MAX_IBI_CD_DOMAINS; i++)
    {
        /* if domain not enabled skip disable request to that domain */
        if (!enable && !(ibi_cd_domain_enable_bmsk & (1 << i))) continue;

        /*-----------------------------------------------------------------------*/
        /* Construct the clock domain name string (e.g., "ibi_cd0", "ibi_cd1")   */
        /*-----------------------------------------------------------------------*/
        ret_value = snprintf(ibi_cd_name, sizeof(ibi_cd_name), "ibi_cd%d", i);
        if (ret_value < 0 || ret_value >= sizeof(ibi_cd_name)) {
            QUP_LOG(LEVEL_ERROR, "fw_set_ibi_cd_domains: snprintf failed for index %d\n", i);
            return FALSE;
        }

        /*-----------------------------------------------------------------------*/
        /* Retrieve the clock ID for the current domain                         */
        /*-----------------------------------------------------------------------*/
        res = Clock_GetIdDT(qup_clock_handle, &node, ibi_cd_name, &ibi_cd_id);

        if (CLOCK_SUCCESS != res)
        {
            /*-----------------------------------------------------------------------*/
            /* If the clock is not found, assume no more domains and exit loop      */
            /*-----------------------------------------------------------------------*/
            if (res == CLOCK_ERROR_NOT_FOUND) {
                break;
            } else {
                QUP_LOG(LEVEL_ERROR, "Failed to get clock ID for %s (index %d)\n", ibi_cd_name, i);
                return FALSE;
            }
        }

        /*-----------------------------------------------------------------------*/
        /* Enable or disable the clock based on the input flag                   */
        /*-----------------------------------------------------------------------*/
        res = enable ? Clock_Enable(qup_clock_handle, ibi_cd_id)
                     : Clock_Disable(qup_clock_handle, ibi_cd_id);

        /*-----------------------------------------------------------------------*/
        /* Return FALSE if the clock operation fails                            */
        /*-----------------------------------------------------------------------*/
        if (CLOCK_SUCCESS != res) {
            QUP_LOG(LEVEL_ERROR, "Failed to %s clock for %s (id: %d)\n",
                    enable ? "enable" : "disable", ibi_cd_name, ibi_cd_id);
            return FALSE;
        }

        if (enable) ibi_cd_domain_enable_bmsk |= 1 << i;
        else ibi_cd_domain_enable_bmsk &= ~(1 << i);

    }

    /*-----------------------------------------------------------------------*/
    /* All clock operations completed successfully                           */
    /*-----------------------------------------------------------------------*/
    return TRUE;
}

/*boolean fw_disable_ibi_ahb_clock(void)
{
    ClockResult res;
    ClockIdType clk_id;
    uint8 ssc_ibi_clock[24] = "scc_ibi_ahb2ahb_s_clk";

    if (qup_clock_handle == NULL) { return FALSE; }

    res = Clock_GetId(qup_clock_handle, (const char *) ssc_ibi_clock, &clk_id);
    if (CLOCK_SUCCESS != res) { return FALSE; }

    res = Clock_Disable(qup_clock_handle, clk_id);
    if (CLOCK_SUCCESS != res) { return FALSE; }

    return TRUE;
}
boolean fw_enable_ibi_ahb_clock(void)
{
    ClockResult res;
    ClockIdType clk_id;
    uint8 ssc_ibi_clock[24] = "scc_ibi_ahb2ahb_s_clk";

    if (qup_clock_handle == NULL) { return FALSE; }

    res = Clock_GetId(qup_clock_handle, (const char *) ssc_ibi_clock, &clk_id);
    if (CLOCK_SUCCESS != res) { return FALSE; }

    res = Clock_Enable(qup_clock_handle, clk_id);
    if (CLOCK_SUCCESS != res) { return FALSE; }

    return TRUE;
}
*/
boolean fw_disable_common_clocks (qup_dev_cfg *qcfg)
{
    ClockResult res;
    qup_common_clock_idx i = 0;

    if (qup_clock_handle == NULL) { return FALSE; }

    for (i = QUP_COMMON_CLOCKS_MAX; i > 0 ; i--)
    {
        res = Clock_Disable(qup_clock_handle, qcfg->common_clk_id[i-1]);  // disable common clocks in reverse order of enabling
        if (CLOCK_SUCCESS != res)
        {
            QUP_LOG(LEVEL_ERROR, "Failed to disable common clock id : %d\n", i - 1 );
            return FALSE;
        }
    }

    if(qup_get_chip_family() != CHIPINFO_FAMILY_ASPEN)
    {
        res = Clock_SetFrequencyEx(qup_clock_handle, qcfg->common_clk_id[QUP_CORE_2X_CLOCK_IDX], 0,
                                   CLOCK_FREQUENCY_HZ_AT_LEAST, NULL, CLOCK_FLAG_FORCE_DOMAIN_ENABLE, 0);
        if (CLOCK_SUCCESS != res) { return FALSE; }
    }

    return TRUE;

}

boolean fw_enable_common_clocks (qup_dev_cfg *qcfg)
{
    ClockResult res = CLOCK_SUCCESS;
    qup_common_clock_idx i = 0;

    if (qup_clock_handle == NULL) { return FALSE; }


    res = Clock_Configure(qup_clock_handle, qcfg->common_clk_id[QUP_CORE_2X_CLOCK_IDX], CLOCK_CONFIG_MEM_CORE_FORCE_ON);
    if (CLOCK_SUCCESS != res)
    {
        QUP_LOG(LEVEL_ERROR, "Failed to configure scc_qupv3_2xcore_clk\n");
        return FALSE;
    }

    res = Clock_SetFrequencyEx(qup_clock_handle, qcfg->common_clk_id[QUP_CORE_2X_CLOCK_IDX], qcfg->core_freq_hz ,
                               CLOCK_FREQUENCY_HZ_AT_LEAST, NULL, CLOCK_FLAG_FORCE_DOMAIN_ENABLE, 0);
    if (CLOCK_SUCCESS != res)
    {
        QUP_LOG(LEVEL_ERROR, "Failed to set clock frequency Clock_SetFrequencyEx\n");
        return FALSE;
    }

    res = Clock_Configure(qup_clock_handle, qcfg->common_clk_id[QUP_CORE_CLOCK_IDX], CLOCK_CONFIG_MEM_CORE_FORCE_ON);
    if (CLOCK_SUCCESS != res)
    {
        QUP_LOG(LEVEL_ERROR, "Failed to configure scc_qupv3_core_clk\n");
        return FALSE;
    }

    for ( i = 0; i < QUP_COMMON_CLOCKS_MAX; i++)
    {
        res = Clock_Enable(qup_clock_handle, qcfg->common_clk_id[i]);
        if (CLOCK_SUCCESS != res)
        {
            QUP_LOG(LEVEL_ERROR, "Failed to enable qup common clk idx: %d", i);
            return FALSE;
        }
    }

    return TRUE;
}

// from tzbsp_copy_init_se_firmware
void init_and_load_se_firmware (uint8 *se_base, elf_se_hdr *hdr, se_cfg *scfg)
{
    const uint32 *fw_val_arr  = (const uint32 *)((uint8*) hdr + hdr->fw_offset);
    const uint8  *cfg_idx_arr = (const uint8  *)          hdr + hdr->cfg_idx_offset;
    const uint32 *cfg_val_arr = (const uint32 *)((uint8*) hdr + hdr->cfg_val_offset);

    uint32 mask;
    uint32 i;
    uint16 fw_revision = 0x0;
    fw_revision = ((hdr->fw_version)>>8);

    if (scfg->dfs_mode)
    {
    HWIO_OUTXF(GENI_CFG_BASE(se_base), GENI_DFS_IF_CFG, DFS_IF_EN, 0x1);
    }
    HWIO_OUTXF(GENI_CFG_BASE(se_base), GENI_OUTPUT_CTRL, IO_OUTPUT_CTRL, 0x0);

    mask = (HWIO_FMSK(GENI_CGC_CTRL, PROG_RAM_SCLK_OFF) |
            HWIO_FMSK(GENI_CGC_CTRL, PROG_RAM_HCLK_OFF));
    HWIO_OUTXM(GENI_CFG_BASE(se_base), GENI_CGC_CTRL, mask, mask);
    HWIO_OUTXF(GENI_SE_SEC_BASE(se_base), GENI_CLK_CTRL, SER_CLK_SEL, 0x0);
    HWIO_OUTXM(GENI_CFG_BASE(se_base), GENI_CGC_CTRL, mask, 0x0);


    HWIO_OUTX(GENI_IMAGE_BASE(se_base), GENI_FW_REVISION,
            (HWIO_FVAL(GENI_FW_REVISION, PROTOCOL, hdr->serial_protocol) |
            HWIO_FVAL(GENI_FW_REVISION, VERSION,  fw_revision)));

    HWIO_OUTX(GENI_IMAGE_BASE(se_base), GENI_S_FW_REVISION,
            (HWIO_FVAL(GENI_S_FW_REVISION, PROTOCOL, hdr->serial_protocol) |
            HWIO_FVAL(GENI_S_FW_REVISION, VERSION,  fw_revision)));

    memscpy(HWIO_GENI_CFG_RAMn_ADDR(GENI_IMAGE_BASE(se_base), 0),
           hdr->fw_size_in_items * sizeof(uint32),      //this is static, doesn't change from target to target
        fw_val_arr,
        hdr->fw_size_in_items * sizeof(uint32));


    HWIO_OUTX(GENI_CFG_BASE(se_base), GENI_INIT_CFG_REVISION,
            HWIO_FVAL(GENI_INIT_CFG_REVISION, VERSION, hdr->cfg_version));

    HWIO_OUTX(GENI_CFG_BASE(se_base), GENI_S_INIT_CFG_REVISION,
            HWIO_FVAL(GENI_S_INIT_CFG_REVISION, VERSION, hdr->cfg_version));

    for (i = 0; i < hdr->cfg_size_in_items; i++)
    {
        HWIO_OUTX(GENI_IMAGE_REGS_BASE(se_base) + (cfg_idx_arr[i] * sizeof(uint32)), GENI_CFG_REG0, cfg_val_arr[i]);
    }

    HWIO_OUTX(GENI_DATA_BASE(se_base), GENI_RX_RFR_WATERMARK_REG,
            HWIO_FVAL(GENI_RX_RFR_WATERMARK_REG, RX_RFR_WATERMARK,
                HWIO_INXF(GENI_SE_DMA_BASE(se_base), SE_HW_PARAM_1, RX_FIFO_DEPTH) - 2));

    HWIO_OUTX(GENI_CFG_BASE(se_base), GENI_FORCE_DEFAULT_REG, 0x1);
    HWIO_OUTXF(GENI_CFG_BASE(se_base), GENI_OUTPUT_CTRL, IO_OUTPUT_CTRL, 0x7f);     //IOS 7

    HWIO_OUTXF(GENI_DATA_BASE(se_base),GENI_HW_IRQ_EN,M_IBI_IRQ_EN,0);
    HWIO_OUTXF(GENI_DATA_BASE(se_base),GENI_HW_IRQ_IGNORE_ON_ACTIVE,M_IBI_IRQ_IGNORE, 0);
    HWIO_OUTXF(GENI_DATA_BASE(se_base),GENI_HW_IRQ_CMD_PARAM_0,M_IBI_IRQ_PARAM_STOP_STALL, 0);
    HWIO_OUTXF(GENI_DATA_BASE(se_base),GENI_HW_IRQ_CMD_PARAM_0,M_IBI_IRQ_PARAM_7E, 0);
    HWIO_OUTXF(GENI_DATA_BASE(se_base),GENI_I3C_IBI_LEGACY, I3C_IBI_SSC_IBI_SEL, 1);

    if (qupv3_hw_ver >= 0x10010000)
    {
        HWIO_OUTX(GENI_SE_DMA_BASE(se_base), DMA_GENERAL_CFG,
                (HWIO_FMSK(DMA_GENERAL_CFG, AHB_SEC_SLV_CLK_CGC_ON) |
                HWIO_FMSK(DMA_GENERAL_CFG, DMA_AHB_SLV_CLK_CGC_ON) |
                HWIO_FMSK(DMA_GENERAL_CFG, DMA_TX_CLK_CGC_ON)      |
                HWIO_FMSK(DMA_GENERAL_CFG, DMA_RX_CLK_CGC_ON)));

        HWIO_OUTX(GENI_CFG_BASE(se_base), GENI_CGC_CTRL,
                (HWIO_FMSK(GENI_CGC_CTRL, EXT_CLK_CGC_ON)         |
                HWIO_FMSK(GENI_CGC_CTRL, RX_CLK_CGC_ON)          |
                HWIO_FMSK(GENI_CGC_CTRL, TX_CLK_CGC_ON)          |
                HWIO_FMSK(GENI_CGC_CTRL, SCLK_CGC_ON)            |
                HWIO_FMSK(GENI_CGC_CTRL, DATA_AHB_CLK_CGC_ON)    |
                HWIO_FMSK(GENI_CGC_CTRL, CFG_AHB_WR_CLK_CGC_ON)  |
                HWIO_FMSK(GENI_CGC_CTRL, CFG_AHB_CLK_CGC_ON)));
    }


    /* HPG section 3.1.7.6 */
    if (scfg->mode == GSI)
    {
        HWIO_OUTXF(GENI_IMAGE_REGS_BASE(se_base), GENI_DMA_MODE_EN, GENI_DMA_MODE_EN, 0x1);
        HWIO_OUTX (GENI_SE_DMA_BASE(se_base), SE_IRQ_EN, 0x0);
        HWIO_OUTX (GENI_SE_DMA_BASE(se_base), SE_GSI_EVENT_EN, HWIO_SE_GSI_EVENT_EN_RMSK);
    }
    else if (scfg->mode == FIFO)
    {
        HWIO_OUTXF(GENI_IMAGE_REGS_BASE(se_base), GENI_DMA_MODE_EN, GENI_DMA_MODE_EN, 0x0);
        HWIO_OUTX (GENI_SE_DMA_BASE(se_base), SE_IRQ_EN, HWIO_SE_IRQ_EN_RMSK);
        HWIO_OUTX (GENI_SE_DMA_BASE(se_base), SE_GSI_EVENT_EN, 0x0);
    }
    else if (scfg->mode == CPU_DMA)
    {
        HWIO_OUTXF(GENI_IMAGE_REGS_BASE(se_base), GENI_DMA_MODE_EN, GENI_DMA_MODE_EN, 0x1);
        HWIO_OUTX (GENI_SE_DMA_BASE(se_base), SE_IRQ_EN, HWIO_SE_IRQ_EN_RMSK);
        HWIO_OUTX (GENI_SE_DMA_BASE(se_base), SE_GSI_EVENT_EN, 0x0);
    }

    /* HPG section 3.1.7.7 */
    /* GPI mode */
    mask = (HWIO_FMSK(GENI_M_IRQ_ENABLE, M_CMD_OVERRUN_EN)    |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, M_ILLEGAL_CMD_EN)    |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, M_CMD_FAILURE_EN)    |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, M_CMD_CANCEL_EN)     |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, M_CMD_ABORT_EN)      |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, M_TIMESTAMP_EN)      |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, IO_DATA_ASSERT_EN)   |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, IO_DATA_DEASSERT_EN) |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, RX_FIFO_WR_ERR_EN)   |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, RX_FIFO_RD_ERR_EN)   |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, TX_FIFO_WR_ERR_EN)   |
            HWIO_FMSK(GENI_M_IRQ_ENABLE, TX_FIFO_RD_ERR_EN));
    if (scfg->protocol == SE_PROTOCOL_I3C || scfg->protocol == SE_PROTOCOL_I3C_IBI)
    {
        mask |= HWIO_FMSK(GENI_M_IRQ_ENABLE, M_GP_SYNC_IRQ_0_EN);
    }

    //HPG 3.2.2.12 SPI 3-4 wire Specific Configuration
    if(scfg->protocol == SE_PROTOCOL_SPI_3W)
    {
        /* Configure GENI_CFG_REG80. IO_MACRO_RX_DATA_IN_SEL (To select PAD1 as rx_data_in) to 1 in 3 wire mode.
           (Default selection is 0 for 4 wire mode)  */
        HWIO_OUTXF(GENI_IMAGE_REGS_BASE(se_base),GENI_CFG_REG80,IO_MACRO_RX_DATA_IN_SEL,1);
    }

    HWIO_OUTXM(GENI_DATA_BASE(se_base), GENI_M_IRQ_ENABLE, mask, mask);

    mask = (HWIO_FMSK(GENI_S_IRQ_ENABLE, S_CMD_OVERRUN_EN)  |
            HWIO_FMSK(GENI_S_IRQ_ENABLE, S_ILLEGAL_CMD_EN)  |
            HWIO_FMSK(GENI_S_IRQ_ENABLE, S_CMD_CANCEL_EN)   |
            HWIO_FMSK(GENI_S_IRQ_ENABLE, S_CMD_ABORT_EN)    |
            HWIO_FMSK(GENI_S_IRQ_ENABLE, S_GP_IRQ_0_EN)     |
            HWIO_FMSK(GENI_S_IRQ_ENABLE, S_GP_IRQ_1_EN)     |
            HWIO_FMSK(GENI_S_IRQ_ENABLE, S_GP_IRQ_2_EN)     |
            HWIO_FMSK(GENI_S_IRQ_ENABLE, S_GP_IRQ_3_EN)     |
            HWIO_FMSK(GENI_S_IRQ_ENABLE, RX_FIFO_WR_ERR_EN) |
            HWIO_FMSK(GENI_S_IRQ_ENABLE, RX_FIFO_RD_ERR_EN));
    HWIO_OUTXM(GENI_DATA_BASE(se_base), GENI_S_IRQ_ENABLE, mask, mask);

    /* HPG section 3.1.7.8 */
    /* GPI/DMA mode */
    mask = (HWIO_FMSK(DMA_TX_IRQ_EN_SET, RESET_DONE_EN_SET) |
            HWIO_FMSK(DMA_TX_IRQ_EN_SET, SBE_EN_SET)        |
            HWIO_FMSK(DMA_TX_IRQ_EN_SET, DMA_DONE_EN_SET));
    HWIO_OUTX(GENI_SE_DMA_BASE(se_base), DMA_TX_IRQ_EN_SET, mask);

    mask = (HWIO_FMSK(DMA_RX_IRQ_EN_SET, FLUSH_DONE_EN_SET) |
            HWIO_FMSK(DMA_RX_IRQ_EN_SET, RESET_DONE_EN_SET) |
            HWIO_FMSK(DMA_RX_IRQ_EN_SET, SBE_EN_SET)        |
            HWIO_FMSK(DMA_RX_IRQ_EN_SET, DMA_DONE_EN_SET));
    HWIO_OUTX(GENI_SE_DMA_BASE(se_base), DMA_RX_IRQ_EN_SET, mask);


    /* HPG section 3.1.7.12 */
    /* TBD try this here*/
    HWIO_OUTX(GENI_CFG_BASE(se_base), GENI_FORCE_DEFAULT_REG, 0x1);

    mask = (HWIO_FMSK(GENI_CGC_CTRL, PROG_RAM_SCLK_OFF) |
            HWIO_FMSK(GENI_CGC_CTRL, PROG_RAM_HCLK_OFF));
    HWIO_OUTXM(GENI_CFG_BASE(se_base), GENI_CGC_CTRL, mask, mask);
    HWIO_OUTXF(GENI_SE_SEC_BASE(se_base), GENI_CLK_CTRL, SER_CLK_SEL, 0x1);
    HWIO_OUTXM(GENI_CFG_BASE(se_base), GENI_CGC_CTRL, mask, 0x0);

    /* HPG section 3.1.7.13 */
    /* GSI/DMA mode */
    HWIO_OUTXF(GENI_SE_SEC_BASE(se_base), DMA_IF_EN, DMA_IF_EN, 0x1);

    /* HPG section 3.1.7.14 */
    if ((scfg->mode == MIXED) || (scfg->mode == FIFO))
    {
        HWIO_OUTXF(GENI_SE_SEC_BASE(se_base), FIFO_IF_DISABLE, FIFO_IF_DISABLE, 0x0);
    }
    else
    {
        HWIO_OUTXF(GENI_SE_SEC_BASE(se_base), FIFO_IF_DISABLE, FIFO_IF_DISABLE, 0x1);
    }
}

void qup_fw_detect_rumi_platform(void)
{
    DALResult                       result = DAL_SUCCESS;
    DalDeviceHandle                *pDevice =  NULL;

    result = DAL_DeviceAttach(DALDEVICEID_PLATFORMINFO, &pDevice);
    if(result != DAL_SUCCESS)
    {
        QUP_LOG(LEVEL_ERROR, "get_rumi_platform: DAL_PlatformInfo Attach Failed :0x%x",result);
        return;
    }

    //GET PLATFORM
    DalPlatformInfoPlatformInfoType plat = { 0 };
    DalPlatformInfo_GetPlatformInfo(pDevice,&plat);
    if(plat.platform ==  DALPLATFORMINFO_TYPE_RUMI )
    {
        is_platform_rumi = TRUE;
    }
}

boolean se_fw_load(uint8 *se_base, fw_type *fw_list, se_cfg *scfg, uint32 ssc_qup_id, uint32 se_index)
{
    elf_se_hdr *hdr = NULL;
    uint32 index = 0;
    UNUSED(index);

    while (fw_list->start != NULL)
    {
        hdr = (elf_se_hdr *) (fw_list->start);
        if ((hdr->magic             == 0x57464553)      &&
            (hdr->serial_protocol   == scfg->protocol)  &&
            (hdr->core_version      <= qupv3_hw_ver))
        {
            QUP_LOG(LEVEL_INFO, "[QUP_FW] fw_list se  = %d\n", index);
            break;
        }
        hdr = NULL;
        fw_list++;
        index++;
    }

    if (hdr == NULL) { return FALSE; }

    if (fw_enable_se_clock (ssc_qup_id,se_index, scfg->dfs_mode) == FALSE)  { return FALSE; }
    init_and_load_se_firmware (se_base, hdr, scfg);
    if(!is_platform_rumi)
    {
        if (fw_disable_se_clock (ssc_qup_id,se_index) == FALSE) { return FALSE; }
    }

    return TRUE;
}

boolean gpi_fw_load(QUP_TYPE qup_id, fw_type *fw_list)
{
    elf_qsi_hdr *hdr = NULL;
    uint32 index = 0;
    UNUSED(index);

    while (fw_list->start != NULL)
    {
        hdr = (elf_qsi_hdr *) (fw_list->start);
        if ((hdr->magic         == 0x20495351)  &&
            (hdr->core_version  <= qupv3_hw_ver))
        {
            QUP_LOG(LEVEL_INFO, "[QUP_FW] fw_list gsi = %d\n", index);
            break;
        }
        hdr = NULL;
        fw_list++;
        index++;
    }

    if (hdr == NULL) { return FALSE; }

    gpi_firmware_load(qup_id, fw_list->start, fw_list->size);

    return TRUE;
}

// reference - tz
void fw_init_qup_common(uint8 *qup_common_base)
{
    qupv3_hw_ver = HWIO_INX(qup_common_base, QUPV3_HW_VERSION);
    QUP_LOG(LEVEL_INFO, "[QUP_FW] qupv3_hw_ver   = 0x%08x\n", qupv3_hw_ver);

    if(!is_platform_rumi)
    {
    /* HPG section 3.1.2 */
    HWIO_OUTXF(qup_common_base, QUPV3_COMMON_CFG,
                                FAST_SWITCH_TO_HIGH_DISABLE,
                                0x01);
    /* HPG section 3.1.7.3 */
    HWIO_OUTXF(qup_common_base, QUPV3_SE_AHB_M_CFG,
                                AHB_M_CLK_CGC_ON,
                                0x01);

    HWIO_OUTXF(qup_common_base, QUPV3_COMMON_CGC_CTRL,
                                COMMON_CSR_SLV_CLK_CGC_ON,
                                0x01);
    }
}

boolean fw_enable_legacy_ibi(uint8 *se_base, uint8 *ibi_base)
{

    HWIO_OUTX  (GENI_DATA_BASE(se_base), GENI_I3C_IBI_CTRL_EN, 0);
    HWIO_OUTXF (GENI_DATA_BASE(se_base), GENI_I3C_IBI_LEGACY, I3C_IBI_LEGACY_EN, 1);
    HWIO_OUTXF (GENI_DATA_BASE(se_base), GENI_I3C_IBI_LEGACY, I3C_IBI_LEGACY_PORTS_EN, 1);
    HWIO_OUTXF (GENI_DATA_BASE(se_base), GENI_I3C_IBI_LEGACY, I3C_IBI_SSC_IBI_SEL, 1);
    HWIO_OUTX  (GENI_CFG_BASE(se_base), GENI_CFG_SEQ_START, 1);
    HWIO_OUTXF (ibi_base,I3C_IBI_LEGACY_MODE, IBI_LEGACY_MODE_EN, 0x1);

    return TRUE;
}

static int qup_fw_update_se_cfg (uint8 ssc_qup_id, uint8 se_id, se_cfg* scfg)
{
    int ret_value = -1;
    char dt_handle_name[50];
    char *platform_cfg = "cfg";
    void *blob = NULL;
    fdt_node_handle node;
    uint32 size = 0;

    if (is_platform_rumi)
    {
        platform_cfg = "cfg_rumi";
    }

    ret_value = snprintf(dt_handle_name, sizeof(dt_handle_name), "%s/ssc_qup_%d/se%d_%s",SSC_FW_DT_NAME, ssc_qup_id, se_id, platform_cfg);
    if (ret_value < 0 || ret_value >= sizeof(dt_handle_name)) {
        QUP_LOG(LEVEL_ERROR, "qup_fw_init: snprintf failed for ssc_qup_%d/se%d\n", ssc_qup_id, se_id);
        goto ON_EXIT;
    }

    ret_value = fdt_get_node_handle(&node, blob, dt_handle_name);
    if (ret_value)
    {
        /* Try default dt node open if it is rumi */
        if (is_platform_rumi)
        {
            ret_value = snprintf(dt_handle_name, sizeof(dt_handle_name), "%s/ssc_qup_%d/se%d_%s",SSC_FW_DT_NAME, ssc_qup_id, se_id, "cfg");
            if (ret_value < 0 || ret_value >= sizeof(dt_handle_name))
            {
                 QUP_LOG(LEVEL_ERROR, "qup_fw_init: snprintf failed for ssc_qup_%d/se%d (rumi fallback)\n", ssc_qup_id, se_id);
                 goto ON_EXIT;
            }
            ret_value = fdt_get_node_handle(&node, blob, dt_handle_name);
        }
        if (ret_value) goto ON_EXIT;
    }

    ret_value = fdt_get_prop_values_size_of_node(&node, &size);
    if (ret_value) goto ON_EXIT;

    ret_value = fdt_get_prop_values_of_node(&node, "wwwhbbbbb", (void *)scfg, size);
    if (ret_value) goto ON_EXIT;


    QUP_LOG(LEVEL_INFO, "[QUP_FW] offset      = 0x%08x\n", scfg->offset);
    QUP_LOG(LEVEL_INFO, "[QUP_FW] protocol    = %d\n",     scfg->protocol);
    QUP_LOG(LEVEL_INFO, "[QUP_FW] mode        = %d\n",     scfg->mode);
    QUP_LOG(LEVEL_INFO, "[QUP_FW] load_fw     = %d\n",     scfg->load_fw);
    QUP_LOG(LEVEL_INFO, "[QUP_FW] dfs_mode    = %d\n",     scfg->dfs_mode);

    if (scfg->ibi_base)
    {
        scfg->ibi_base = (uint32)qup_convert_hw_phy_to_vir_addr((void*)scfg->ibi_base);
    }

ON_EXIT:
    if (ret_value)
    {
        QUP_LOG(LEVEL_ERROR,"qup_fw_init - ERR %d- failed to read DTB!\n",ret_value);
    }
    return ret_value;
}

void qup_fw_init (void)
{

    ClockResult eResult = CLOCK_SUCCESS;
    fw_type *fw_list    = NULL;
    se_cfg se_fw_config= {0};
    uint8 num_ssc_qup = 0, i = 0, j = 0;
    se_cfg  *scfg = NULL;
    qup_dev_cfg *qcfg = NULL;

    void *blob = NULL;
    fdt_node_handle node;
    char dt_handle_name[50];
    int ret_value = -1;
    ClockIdType gdsc_pwr_domain_id = 0;
    boolean ssc_gdsc_enabled = FALSE;

    qup_fw_detect_rumi_platform();

    if (disable_fw_loading) { return; }

    //LOAD CONFIG
    eResult = Clock_Attach(&qup_clock_handle, "fw_config");
    if (CLOCK_SUCCESS != eResult) { return; }

    ret_value = fdt_get_node_handle(&node, NULL, "/soc/ssc_pwr_domains");
    if (ret_value < 0)
    {
        QUP_LOG(LEVEL_ERROR, "qup_fw_init: get node failed for ssc_pwr_domains :0x%x",ret_value);
        ret_value = -1;
        goto ON_EXIT;
    }

    eResult = Clock_GetIdDT(qup_clock_handle, &node, "ssc_gdsc", &gdsc_pwr_domain_id);
    if (CLOCK_SUCCESS != eResult)
    {
        QUP_LOG(LEVEL_ERROR, "qup_fw_init: Clock_GetIdDT failed for gdsc_pwr_domain_id : %d status :0x%x",gdsc_pwr_domain_id, eResult);
        ret_value = -1;
        goto ON_EXIT;
    }

    eResult= Clock_Enable(qup_clock_handle, gdsc_pwr_domain_id );
    if (CLOCK_SUCCESS != eResult)
    {
        QUP_LOG(LEVEL_ERROR, "qup_fw_init: Clock_Enable failed for gdsc_pwr_domain_id : %d status :0x%x",gdsc_pwr_domain_id, eResult);
        ret_value = -1;
        goto ON_EXIT;
    }
    ssc_gdsc_enabled = TRUE;

    qurt_mem_barrier();

    fw_set_ibi_cd_domains(TRUE);

    qurt_mem_barrier();

    fw_list = fw_get_fw_list();

    ret_value = snprintf(dt_handle_name, sizeof(dt_handle_name), SSC_FW_DT_NAME);
    if (ret_value < 0 || ret_value >= sizeof(dt_handle_name)) {
        QUP_LOG(LEVEL_ERROR, "qup_fw_init: snprintf failed for SSC_FW_DT_NAME\n ret_value : %d", ret_value);
        goto ON_EXIT;
    }

    ret_value = fdt_get_node_handle(&node, blob, dt_handle_name);
    if (ret_value) goto ON_EXIT;

    ret_value = fdt_get_uint8_prop(&node, "num_ssc_qup", (uint8 *)&num_ssc_qup);
    if (ret_value) goto ON_EXIT;


    /* Iterate for each SSC QUP */
    for ( i = 0; i < num_ssc_qup; i++)
    {
        qcfg = (qup_dev_cfg*)qup_get_common_dev_cfg(SET_QUP_TYPE(SSC_QUP,i));
        if (qcfg == NULL) goto ON_EXIT;

        if (fw_enable_common_clocks (qcfg) == FALSE) { return; }

        fw_init_qup_common(qcfg->common_base_addr);

        /* Iterate for Each SE in SSC QUP */
        for (j = 0; j < qcfg->num_se; j++)
        {
            uint8 *se_base = NULL;

            scfg = (se_cfg *)&se_fw_config;

            ret_value = qup_fw_update_se_cfg(i, j, scfg);

            if(ret_value)
            {
                continue;
            }
            if (scfg->load_fw == FALSE) { continue; }

#ifdef QUP_KPI_PROFILING
            qup_fw_loading_kpi[i][j].protocol = scfg->protocol;
            qup_fw_loading_kpi[i][j].start_ticks = CoreTimetick_Get64();
#endif
            se_base = (uint8 *) (qcfg->core_base_addr + scfg->offset);
            se_fw_loaded[i][j] = se_fw_load(se_base, fw_list, scfg, i, j);

            if((scfg->ibi_base)  && IS_LEGACY(scfg->protocol)) //non IBI protocol on IBI Supported SE
            {
                if(!fw_enable_legacy_ibi(se_base,(uint8*)scfg->ibi_base))
                {
                    //FATAL
                }
            }

#ifdef QUP_KPI_PROFILING
            qup_fw_loading_kpi[i][j].total_ticks = CoreTimetick_Get64() - qup_fw_loading_kpi[i].start_ticks;
            QUP_LOG(LEVEL_PERF, "SSC_QUP_SE%d loaded with protocol %d in  %dusec", i, scfg->protocol,
                            (qup_fw_loading_kpi[i].total_ticks*10)/192);
#endif

            QUP_LOG(LEVEL_INFO, "[QUP_FW] se loaded   = %d\n", se_fw_loaded[i]);
        }
#ifdef QUP_KPI_PROFILING
        gsi_fw_loading_kpi[i].start_ticks = CoreTimetick_Get64();
#endif

        qup_fw_loaded = gpi_fw_load(qcfg->qup_id, fw_list);
#ifdef QUP_KPI_PROFILING

        gsi_fw_loading_kpi[i].total_ticks = CoreTimetick_Get64() - gsi_fw_loading_kpi[i].start_ticks;
        QUP_LOG(LEVEL_PERF, "SSC_QUP_GSI FW Loaded in  %dusec", (gsi_fw_loading_kpi[i].total_ticks*10)/192);
#endif

        QUP_LOG(LEVEL_INFO, "[QUP_FW] GSI loaded = %d\n", qup_fw_loaded);

        if(!is_platform_rumi)
        {
            if (fw_disable_common_clocks (qcfg) == FALSE) { return; }
        }
    }



ON_EXIT:

    fw_set_ibi_cd_domains(FALSE);

    if (ssc_gdsc_enabled)
    {
        eResult = Clock_Disable(qup_clock_handle, gdsc_pwr_domain_id );
        if (CLOCK_SUCCESS != eResult)
        {
            QUP_LOG(LEVEL_ERROR, "qup_fw_init: Clock_Disable failed for gdsc_pwr_domain_id : %d status :0x%x",gdsc_pwr_domain_id, eResult);
            return;
        }
    }
    if (qup_clock_handle != NULL)
    {
        Clock_Detach(qup_clock_handle);
        qup_clock_handle = NULL;
    }
    if (ret_value)
    {
        QUP_LOG(LEVEL_ERROR,"qup_fw_init - ERR %d- failed to read DTB!\n",ret_value);
        return;
    }

     return;

}

boolean se_is_fw_loaded (uint32 ssc_qup_index, uint32 se_index)
{
    return se_fw_loaded[ssc_qup_index][se_index];
}

boolean qup_is_fw_loaded (void)
{
    return qup_fw_loaded;
}

uint32 qup_hw_version (void)
{
    return qupv3_hw_ver;
}
