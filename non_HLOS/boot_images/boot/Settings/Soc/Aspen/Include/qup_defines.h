#ifndef __QUP_DEFINES_H__
#define __QUP_DEFINES_H__

/*----------------------------------------------------------------------------
 * BASE: LPASS
 *--------------------------------------------------------------------------*/

#define LPASS_BASE                                                    0x18000000

/*----------------------------------------------------------------------------
 * BASE: I2C_MASTER_0
 *--------------------------------------------------------------------------*/
 
#define I2C_MASTER_0_BASE                                             0x0 

/*----------------------------------------------------------------------------
 * BASE: QUPV3_1_QUPV3_ID_1
 *--------------------------------------------------------------------------*/

#define QUPV3_1_QUPV3_ID_5_BASE                                      0x00A00000

/*----------------------------------------------------------------------------
 * BASE:  QUPV3_2_QUPV3_ID_1
 *--------------------------------------------------------------------------*/

#define  QUPV3_2_QUPV3_ID_1_BASE                                     0x0

/*----------------------------------------------------------------------------
 * QUP Offsets
 *--------------------------------------------------------------------------*/

#define QUPV3_SE_BASE_OFFSET         0x00080000
#define QUPV3_COMMON_BASE_OFFSET     0x000C0000
#define SSC_QUPV3_CORE_BASE_OFFSET   0x0A900000
#define SSC_QUPV3_SE_BASE_OFFSET     0x00080000
#define SSC_QUPV3_COMMON_BASE_OFFSET 0x000C0000

// I2C_MASTER_0 [se0 - se9] [0:9]
#define QUPV3_0_CORE_BASE_ADDRESS        I2C_MASTER_0_BASE
#define QUPV3_0_CORE_COMMON_BASE_ADDRESS I2C_MASTER_0_BASE + QUPV3_SE_BASE_OFFSET
#define QUPV3_0_CORE_SE_BASE_ADDRESS     I2C_MASTER_0_BASE + QUPV3_SE_BASE_OFFSET

// south [se0 - se7] [10:17]
#define QUPV3_1_CORE_BASE_ADDRESS        QUPV3_1_QUPV3_ID_5_BASE
#define QUPV3_1_CORE_COMMON_BASE_ADDRESS QUPV3_1_QUPV3_ID_5_BASE + QUPV3_COMMON_BASE_OFFSET
#define QUPV3_1_CORE_SE_BASE_ADDRESS     QUPV3_1_QUPV3_ID_5_BASE + QUPV3_SE_BASE_OFFSET

// north [se0 - se5] [18:25]
#define QUPV3_2_CORE_BASE_ADDRESS        QUPV3_2_QUPV3_ID_1_BASE
#define QUPV3_2_CORE_COMMON_BASE_ADDRESS QUPV3_2_QUPV3_ID_1_BASE + QUPV3_COMMON_BASE_OFFSET
#define QUPV3_2_CORE_SE_BASE_ADDRESS     QUPV3_2_QUPV3_ID_1_BASE + QUPV3_SE_BASE_OFFSET




//ssc qup [se0 - se8]
#define SSC_QUPV3_CORE_BASE_ADDRESS          (LPASS_BASE + SSC_QUPV3_CORE_BASE_OFFSET)
#define SSC_QUPV3_SE_BASE_ADDRESS            (SSC_QUPV3_CORE_BASE_ADDRESS + SSC_QUPV3_SE_BASE_OFFSET)
#define SSC_QUPV3_COMMON_BASE_ADDRESS        (SSC_QUPV3_CORE_BASE_ADDRESS + SSC_QUPV3_COMMON_BASE_OFFSET)

#define QUP_FLAGS_UNUSED            0
#define VOTE_FOR_CORE2X_CLOCK       (1 << 1)
#define VOTE_FOR_SRC_CLOCK          (1 << 2)
    
    /*Work arounds for the QUP*/
#define IBI_VOTE_FOR_CLOCK          (1 << 3)
#define IBI_AUTO_PROCESS_WA_ENABLE  (1 << 4)
#define IBI_RSC_HSHAKE_ECO_ENABLE   (1 << 5)

#define SE_FLAGS_UNUSED                             0
#define EXCLUSIVE_SE                                (1 << 1)
#define OPTIMISE_TRANSFERS                          (1 << 2)
#define POLLED_MODE                                 (1 << 3)
#define USES_DDR_BUFFER                             (1 << 4)
#define ENABLE_TIMEOUT                              (1 << 5)
#define ENABLE_REGDUMP                              (1 << 6)
#define ENABLE_FATAL                                (1 << 7)
#define ENABLE_FATAL_ON_TIMEOUT                     ENABLE_FATAL | ENABLE_TIMEOUT
#define ENABLE_RECOVER_ON_TIMEOUT                   ENABLE_TIMEOUT
#define ENABLE_FATAL_ON_TIMEOUT_WITH_REGDUMP        ENABLE_FATAL_ON_TIMEOUT   | ENABLE_REGDUMP
#define ENABLE_RECOVER_ON_TIMEOUT_WITH_REGDUMP      ENABLE_RECOVER_ON_TIMEOUT | ENABLE_REGDUMP

    /** Internal used flags **/
#define USES_INTERNAL_DDR_MEM                       (1 << 24)
#define STRETCH_CLK_POST_IBI                        (1 << 25)


#define CORE_IRQ                    (1 << 1)
#define GSI_MODE                    (1 << 2)
#define IBI_PDC_SUPPORT             (1 << 3)
#define UART_RX_PDC_SUPPORT         (1 << 4)

#define SPI_SUPPORTED               (1 << 1)
#define UART_SUPPORTED              (1 << 2)
#define I2C_SUPPORTED               (1 << 3)
#define I3C_SUPPORTED               (1 << 4)
#define I3C_DATA_ONLY_SUPPORTED     (1 << 5)
#define LEGACY_IBI_SUPPORTED        (1 << 6)
#define IBI_CONTROLLER_SUPPORTED    (1 << 7)

#define GPIO_CFG_BUSES(dir, pulltype, drivestrength, strongpull) \
                 (dir | pulltype | drivestrength | strongpull)

#define SET_QUP_TYPE(qup_major,instance_number)      (((qup_major & 0xF) << 4) | (instance_number & 0xF))
#define GET_QUP_MAJOR(qup_type)                      ((qup_type & 0xF0) >> 4)
#define GET_QUP_MINOR(qup_type)                      (qup_type & 0xF)



#define   QUP_0          SET_QUP_TYPE(TOP_QUP,0)
#define   QUP_1          SET_QUP_TYPE(TOP_QUP,1)
#define   QUP_2          SET_QUP_TYPE(TOP_QUP,2)
#define   QUP_N          QUP_2 + 1
#define   TOP_QUP_MAX    GET_QUP_MINOR(QUP_N)
#define   QUP_SSC        SET_QUP_TYPE(SSC_QUP,0)
#define   QUP_SSC_0      QUP_SSC
#define   QUP_SSC_N      QUP_SSC_0 + 1
#define   SSC_QUP_MAX    GET_QUP_MINOR(QUP_SSC_N)
#define   QUP_I2C_HUB    SET_QUP_TYPE(I2C_HUB,0)
#define   QUP_I2C_HUB_0  QUP_I2C_HUB
#define   QUP_I2C_HUB_N  QUP_I2C_HUB_0 + 1
#define   I2C_HUB_MAX    GET_QUP_MINOR(QUP_I2C_HUB_N)
#define   QUP_TYPE_MAX   I2C_HUB_MAX + 1


#define   TOP_QUP       0
#define   SSC_QUP       1
#define   I2C_HUB       2
#define   QUP_MAJOR_MAX I2C_HUB + 1

#endif
