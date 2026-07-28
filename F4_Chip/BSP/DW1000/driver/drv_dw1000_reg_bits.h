/**
 * @file drv_dw1000_reg.h
 * @author quote6 (HeXiHeNian@qq.com)
 * @brief
 * @version 0.1
 * @date 2026-07-23
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#include <stdint.h>

#pragma pack(push, 1)

/* Exported constants --------------------------------------------------------*/

/******************************************************************************/
/**
 * @brief 寄存器 DEV_ID - 0x00 - RO
 *
 * 设备标识符
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t  REV : 4; /**< Revision */
        uint8_t  VER : 4; /**< Version */
        uint8_t  MODEL;   /**< 表示设备类型，dw1000为0x01 */
        uint16_t RIDTAG;  /**< 寄存器标识标签,固定为 0xDECA */
    };
} dw1000_reg_dev_id_t;

/*--------------------------------------- DEV_ID 位定义 -----------------------*/
/**
 * @brief Bit[31:16]: RIDTAG - Register Identification Tag 寄存器标识标签
 * @note  计划对对所有的Decadewave器件使用相同的值。该值为0xDECA。
 */
#define DW1000_REG_DEV_ID_BITS_RIDTAG_POS (16U)
#define DW1000_REG_DEV_ID_BITS_RIDTAG_LEN (16U) /* 位 */
#define DW1000_REG_DEV_ID_BITS_RIDTAG_MSK (0xFFFFUL << DW1000_REG_DEV_ID_BITS_RIDTAG_POS)
#define DW1000_REG_DEV_ID_BITS_RIDTAG     DW1000_REG_DEV_ID_BITS_RIDTAG_MSK

/**
 * @brief Bit[15:8]: MODEL - 型号标识设备
 * @note  DW1000的设备类型为0x01。
 */
#define DW1000_REG_DEV_ID_BITS_MODEL_POS  (8U)
#define DW1000_REG_DEV_ID_BITS_MODEL_LEN  (8U) /* 位 */
#define DW1000_REG_DEV_ID_BITS_MODEL_MSK  (0xFFUL << DW1000_REG_DEV_ID_BITS_MODEL_POS)
#define DW1000_REG_DEV_ID_BITS_MODEL      DW1000_REG_DEV_ID_BITS_MODEL_MSK

/**
 * @brief Bit[7:4]: VER - Verison 版本
 * @note  如果生产的新版本与之前的版本有重大差异，则此数字将被更新。
 */
#define DW1000_REG_DEV_ID_BITS_VER_POS    (4U)
#define DW1000_REG_DEV_ID_BITS_VER_LEN    (4U) /* 位 */
#define DW1000_REG_DEV_ID_BITS_VER_MSK    (0xFUL << DW1000_REG_DEV_ID_BITS_VER_POS)
#define DW1000_REG_DEV_ID_BITS_VER        DW1000_REG_DEV_ID_BITS_VER_MSK

/**
 * @brief Bit[3:0]: REV - Revision 修订
 * @note  当进行小幅更正和操作更改时会更新此编号
 */
#define DW1000_REG_DEV_ID_BITS_REV_POS    (0U)
#define DW1000_REG_DEV_ID_BITS_REV_LEN    (4U) /* 位 */
#define DW1000_REG_DEV_ID_BITS_REV_MSK    (0xFUL << DW1000_REG_DEV_ID_BITS_REV_POS)
#define DW1000_REG_DEV_ID_BITS_REV        DW1000_REG_DEV_ID_BITS_REV_MSK
/******************************************************************************/

/**
 * @brief 寄存器 EUI - 0x01 - RW
 *
 * 扩展唯一标识符
 */
typedef struct {
    uint8_t DID[5]; /**< 设备扩展ID */
    uint8_t OUI[3]; /**< 组织唯一标识符 */
} dw1000_reg_eui_t;

/* 保留 - 0x02 */

/**
 * @brief 寄存器 PANADR - 0x03 - RW
 *
 * PAN标识符和短地址
 */
typedef union {
    uint8_t value[4];
    struct {
        uint16_t SHORT_ADDR;
        uint16_t PAN_ID;
    };
} dw1000_reg_panadr_t;

/**
 * @brief 寄存器 SYS_CFG - 0x04 - RW
 *
 * 系统配置
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t FFEN : 1;
        uint8_t FFBC : 1;
        uint8_t FFAB : 1;
        uint8_t FFAD : 1;
        uint8_t FFAA : 1;
        uint8_t FFAM : 1;
        uint8_t FFAR : 1;
        uint8_t FFA4 : 1;
        uint8_t FFA5 : 1;
        uint8_t HIRQ_POL : 1;
        uint8_t SPI_EDGE : 1;
        uint8_t DIS_FCE : 1;
        uint8_t DIS_DRXB : 1;
        uint8_t DIS_PHE : 1;
        uint8_t DIS_RSED : 1;
        uint8_t FCS_INIT2F : 1;
        uint8_t PHR_MODE : 2;
        uint8_t DIS_STXP : 1;
        uint8_t : 3;
        uint8_t RXM110K : 1;
        uint8_t : 5;
        uint8_t RXWTOE : 1;
        uint8_t RXAUTR : 1;
        uint8_t AUTOACK : 1;
        uint8_t AACKPEND : 1;
    };
} dw1000_reg_sys_cfg_t;


/* 寄存器 保留 - 0x05 */


/**
 * @brief 寄存器 SYS_TIME - 0x06 - RO
 *
 * 系统时间计数器
 */
typedef uint8_t dw1000_reg_sys_time_t[5];


/* 寄存器 保留 - 0x07 */


/**
 * @brief 寄存器 TX_FCTRL - 0x08 - RW
 *
 * 发送帧控制
 */
typedef union {
    uint8_t value[5];
    struct {
        uint16_t TFLEN : 7;
        uint16_t TFLE : 3;
        uint16_t : 3;
        uint16_t TXBR : 2;
        uint16_t TR : 1;
        uint16_t TXPRF : 2;
        uint16_t TXPSR : 2;
        uint16_t PE : 2;
        uint16_t TXBOFFS : 10;
        uint8_t  IFSDELAY;
    };
} dw1000_reg_tx_fctrl_t;

/**
 * @brief 寄存器 TX_BUFFER - 0x09 - WO
 *
 * 发送数据缓冲区
 */
typedef uint8_t dw1000_reg_tx_buffer_t[1024];

/**
 * @brief 寄存器 DX_TIME - 0x0A - RW
 *
 * 延迟发送或延迟接收时间
 */
typedef uint8_t dw1000_reg_dx_time_t[5];


/* 寄存器 保留 - 0x0B */


/**
 * @brief 寄存器 RX_FWTO - 0x0C - RW
 *
 * 接收帧等待超时时间
 */
typedef uint16_t dw1000_reg_rx_fwto_t;


/**
 * @brief 寄存器 SYS_CTRL - 0x0D - SRW
 *
 * 系统控制寄存器
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t SFCST : 1;
        uint8_t TXSTRT : 1;
        uint8_t TXDLYS : 1;
        uint8_t CANSFCS : 1;
        uint8_t : 2;
        uint8_t TRXOFF : 1;
        uint8_t WAIT4RESP : 1;
        uint8_t RXENAB : 1;
        uint8_t RXDLYE : 1;
        uint8_t : 6;
        uint8_t : 8;
        uint8_t HRBPT : 1;
        uint8_t : 7;
    };
} dw1000_reg_sys_ctrl_t;


/**
 * @brief 寄存器 SYS_MASK - 0x0E - RW
 *
 * 系统事件掩码寄存器
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t : 1;
        uint8_t MCPLOCK : 1;
        uint8_t MESYNCR : 1;
        uint8_t MAAT : 1;
        uint8_t MTXFRB : 1;
        uint8_t MTXPRS : 1;
        uint8_t MTXPHS : 1;
        uint8_t MTXFRS : 1;
        uint8_t MRXPRD : 1;
        uint8_t MRXSFDD : 1;
        uint8_t MLDEDONE : 1;
        uint8_t MRXPHD : 1;
        uint8_t MRXPHE : 1;
        uint8_t MRXDFR : 1;
        uint8_t MRXFCG : 1;
        uint8_t MRXFCE : 1;
        uint8_t MRXRFSL : 1;
        uint8_t MRXRFTO : 1;
        uint8_t MLDEERR : 1;
        uint8_t : 1;
        uint8_t MRXOVRR : 1;
        uint8_t MRXPTO : 1;
        uint8_t MGPIOIRQ : 1;
        uint8_t MSLP2INIT : 1;
        uint8_t MRFPLLLL : 1;
        uint8_t MCPLLLL : 1;
        uint8_t MRXSFDTO : 1;
        uint8_t MHPDWARN : 1;
        uint8_t MTXBERR : 1;
        uint8_t MAFFREJ : 1;
        uint8_t : 2;
    };
} dw1000_reg_sys_mask_t;


/**
 * @brief 寄存器 SYS_STATUS - 0x0F - SRW
 *
 * 系统事件状态寄存器
 */
typedef union {
    uint8_t value[5];
    struct {
        uint8_t IRQS : 1;
        uint8_t CPLOCK : 1;
        uint8_t ESYNCR : 1;
        uint8_t AAT : 1;
        uint8_t TXFRB : 1;
        uint8_t TXPRS : 1;
        uint8_t TXPHS : 1;
        uint8_t TXFRS : 1;
        uint8_t RXPRD : 1;
        uint8_t RXSFDD : 1;
        uint8_t LDEDONE : 1;
        uint8_t RXPHD : 1;
        uint8_t RXPHE : 1;
        uint8_t RXDFR : 1;
        uint8_t RXFCG : 1;
        uint8_t RXFCE : 1;
        uint8_t RXRFSL : 1;
        uint8_t RXRFTO : 1;
        uint8_t LDEERR : 1;
        uint8_t : 1;
        uint8_t RXOVRR : 1;
        uint8_t RXPTO : 1;
        uint8_t GPIOIRQ : 1;
        uint8_t SLEP2INIT : 1;
        uint8_t RFPLL_LL : 1;
        uint8_t CLKPLL_LL : 1;
        uint8_t RXSFDTO : 1;
        uint8_t HPDWARN : 1;
        uint8_t TXBERR : 1;
        uint8_t AFFREJ : 1;
        uint8_t HSRBP : 1;
        uint8_t ICRBP : 1;
        uint8_t RXRSCS : 1;
        uint8_t RXPREJ : 1;
        uint8_t TXPUTE : 1;
        uint8_t : 5;
    };
} dw1000_reg_sys_staus_t;


/**
 * @brief 寄存器 RX_FINFO - 0x10 - ROD
 *
 * 接收帧信息寄存器
 */
typedef union {
    uint8_t value[4];
    struct {
        uint16_t RXFLEN : 7;
        uint16_t RXFLE : 3;
        uint16_t : 1;
        uint16_t RXNSPL : 2;
        uint16_t RXBR : 2;
        uint16_t RNG : 1;
        uint16_t RXPRFR : 2;
        uint16_t RXPSR : 2;
        uint16_t RXPACC : 12;
    };
} dw1000_reg_rx_finfo_t;


/**
 * @brief 寄存器 RX_BUFFER - 0x11 - ROD
 *
 * 接收帧缓冲区
 */
typedef uint8_t dw1000_reg_rx_buffer_t[1024];


/**
 * @brief 寄存器 RX_FQUAL - 0x12 - ROD
 *
 * 接收帧质量信息
 */
typedef union {
    uint8_t value[8];
    struct {
        uint16_t STD_NOISE;
        uint16_t FP_AMPL2;
        uint16_t FP_AMPL3;
        uint16_t CIR_PWR;
    };
} dw1000_reg_rx_fqual_t;


/**
 * @brief 寄存器 RX_TTCKI - 0x13 - ROD
 *
 * 接收器时间跟踪间隔
 */
typedef uint32_t dw1000_reg_rx_ttcki_t;


/**
 * @brief 寄存器 RX_TTCKO - 0x14 - ROD
 *
 * 接收机时间跟踪偏移
 */
typedef union {
    uint8_t value[5];
    struct {
        uint32_t RXTOFS : 19;
        uint32_t : 5;
        uint32_t RSMPDEL : 8;
        uint8_t  RCPHASE : 7;
        uint8_t : 1;
    };
} dw1000_reg_rx_ttcko_t;


/**
 * @brief 寄存器 RX+TIME - 0x15 - ROD
 *
 * 接收时间戳
 */
typedef union {
    uint8_t value[14];
    struct {
        uint8_t  RX_STAMP[5];
        uint16_t FP_INDEX;
        uint16_t FP_AMPL1;
        uint8_t  RX_RAWST[5];
    };
} dw1000_reg_rx_time_t;


/* 寄存器 保留 - 0x16 */


/**
 * @brief 寄存器 TX_TIME - 0x17 - RO
 *
 * 发送时间戳
 */
typedef union {
    uint8_t value[10];
    struct {
        uint8_t TX_STAMP[5];
        uint8_t TX_RAWST[5];
    };
} dw1000_reg_tx_time_t;

/**
 * @brief 寄存器 TX_ANTD - 0x18 - RW
 *
 * 发送器天线延迟
 */
typedef uint16_t dw1000_reg_tx_antd_t;

/**
 * @brief 寄存器 SYS_STATE - 0x19 - RO
 *
 * DW1000状态信息
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t TX_STATE : 4;
        uint8_t : 4;
        uint8_t RX_STATE : 5;
        uint8_t : 3;
        uint8_t PMSC_STATE : 4;
        uint8_t : 4;
        uint8_t : 8;
    };
} dw1000_reg_sys_state_t;


/**
 * @brief 寄存器 ACK_RESP_T - 0x1A - RW
 *
 * 应答时间和响应时间
 */
typedef union {
    uint8_t value[4];
    struct {
        uint32_t W4R_TIM : 20;
        uint32_t : 4;
        uint32_t ACK_TIM : 8;
    };
} dw1000_reg_ack_resp_time_t;


/* 寄存器 保留 - 0x1B */


/* 寄存器 保留 - 0x1C */


/**
 * @brief 寄存器 RX_SNIFF - 0x1D - RW
 *
 * SNIFF模式
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t SNIFF_ONT : 4;
        uint8_t : 4;
        uint8_t SNIFF_OFFT;
        uint16_t : 16;
    };
} dw1000_reg_rx_sniff_t;


/**
 * @brief 寄存器 TX_POWER - 0x1E - RW
 *
 * 发送功率控制
 * @todo 需要好好研读这块的用户手册内容
 */
typedef union {
    uint8_t value[4];
    struct {
        struct {
            uint8_t FINE_GAIN : 5;
            uint8_t CRS_GAIN : 3;
        } BOOSTNORM;
        struct {
            uint8_t FINE_GAIN : 5;
            uint8_t CRS_GAIN : 3;
        } BOOSTP500;
        struct {
            uint8_t FINE_GAIN : 5;
            uint8_t CRS_GAIN : 3;
        } BOOSTP250;
        struct {
            uint8_t FINE_GAIN : 5;
            uint8_t CRS_GAIN : 3;
        } BOOSTP125;
    };
    struct {
        uint8_t : 8;
        struct {
            uint8_t FINE_GAIN : 5;
            uint8_t CRS_GAIN : 3;
        } TXPOWPHR;
        struct {
            uint8_t FINE_GAIN : 5;
            uint8_t CRS_GAIN : 3;
        } TXPOWSD;
        uint8_t : 8;
    };
} dw1000_reg_tx_power_t;


/**
 * @brief 寄存器 CHAN_CTRL - 0x1F - RW
 *
 * 频道控制
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t TX_CHAN : 4;
        uint8_t RX_CHAN : 4;
        uint8_t : 8;
        uint16_t : 1;
        uint16_t DWSFD : 1;
        uint16_t RXPRF : 2;
        uint16_t TNSSFD : 1;
        uint16_t RNSSFD : 1;
        uint16_t TX_PCODE : 5;
        uint16_t RX_PCODE : 5;
    };
} dw1000_reg_chan_ctrl_t;


/* 寄存器 保留 - 0x20 */


/**
 * @brief 寄存器 USR_SFD - 0x21 - RW
 *
 * 用户定义的SFD序列
 */
typedef union {
    uint8_t value[41];
    struct {
        uint8_t SFD_LENGTH;
        uint8_t TX_SSFD_MAG[2];
        uint8_t TX_SSFD_SGN[2];
        uint8_t RX_SSFD_MAG[2];
        uint8_t RX_SSFD_SGN[2];
        uint8_t TX_LSFD_MAG[8];
        uint8_t TX_LSFD_SGN[8];
        uint8_t RX_LSFD_MAG[8];
        uint8_t RX_LSFD_SGN[8];
    };
} dw1000_reg_usr_sfd_t;


/* 寄存器 保留 - 0x22 */


/**
 * @brief 子寄存器 AGC_CTRL1 - 0x23:02 - RW
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t DIS_AM : 1;
        uint8_t : 7;
    };
} dw1000_sr_agc_ctrl1_t;

/**
 * @brief 子寄存器 AGC_TUNE1 - 0x23:04 - RW
 */
typedef uint16_t dw1000_sr_agc_tune1_t;

/* 子寄存器 保留 - 0x23:06 */

/**
 * @brief 子寄存器 AGC_TUNE2 - 0x23:0C - RW
 */
typedef uint32_t dw1000_sr_agc_tune2_t;

/* 子寄存器 保留 - 0x23:10 */

/**
 * @brief 子寄存器 AGC_TUNE3 - 0x23:12 - RW
 */
typedef uint16_t dw1000_sr_agc_tune3_t;

/* 子寄存器 保留 - 0x23:14 */

/**
 * @brief 子寄存器 AGC_STAT1 - 0x23:1E - RW
 */
typedef union {
    uint8_t value[3];
    struct {
        uint32_t : 6;
        uint32_t EDG1 : 5;
        uint32_t EDV2 : 9;
        uint32_t : 4;
    };
} dw1000_sr_agc_stat1_t;

/**
 * @brief 寄存器 AGC_CTRL - 0x23 -RW
 *
 * AGC配置和控制
 */
typedef union {
    uint8_t value[33];
    struct {
        uint8_t               RES1[2];
        dw1000_sr_agc_ctrl1_t AGC_CTRL1;
        dw1000_sr_agc_tune1_t AGC_TUNE1;
        uint8_t               RES2[6];
        dw1000_sr_agc_tune2_t AGC_TUNE2;
        uint8_t               RES3[2];
        dw1000_sr_agc_tune3_t AGC_TUNE3;
        uint8_t               RES4[10];
        dw1000_sr_agc_stat1_t AGC_STAT1;
    };
} dw1000_reg_agc_ctrl_t;


/**
 * @brief 子寄存器 EC_CTRL - 0x24:00 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint16_t OSTSM : 1;
        uint16_t OSRSM : 1;
        uint16_t PLLLDT : 1;
        uint16_t WAIT : 8;
        uint16_t OSTRM : 1;
        uint16_t : 4;
        uint16_t : 16;
    };
} dw1000_sr_ec_ctrl_t;

/**
 * @brief 子寄存器 EC_RXTC - 0x24:04 - RO
 */
typedef struct {
    uint32_t RX_TS_EST;
} dw1000_sr_ec_rxtc_t;

/**
 * @brief 子寄存器 EC_GOLP - 0x24:08 - RO
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t OFFSET_EXT : 6;
        uint8_t : 2;
        uint8_t : 8;
        uint8_t : 8;
        uint8_t : 8;
    };
} dw1000_sr_ec_golp_t;

/**
 * @brief 寄存器 EXT_SYNC - 0x24 - RW
 *
 * 外部同步控制
 */
typedef union {
    uint8_t value[12];
    struct {
        dw1000_sr_ec_ctrl_t EC_CTRL;
        dw1000_sr_ec_rxtc_t EC_RXTC;
        dw1000_sr_ec_golp_t EC_GOLP;
    };
} dw1000_reg_ext_sync_t;


/**
 * @brief 子寄存器 CIR - 0x25:000到0x25:FDF -RO
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t real_low;
        uint8_t real_high;
        uint8_t imag_low;
        uint8_t imag_high;
    };
} dw1000_sr_cir_t;

/**
 * @brief 寄存器 ACC_MEM - 0x25 - RO
 *
 * 累加器信道脉冲响应存储器
 */
typedef union {
    uint8_t         value[4064];
    dw1000_sr_cir_t CIR[1016];
} dw1000_reg_acc_mem_t;


/**
 * @brief 子寄存器 GPIO_MODE - 0x26:00 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t : 6;
        uint8_t MSGP0 : 2;
        uint8_t MSGP1 : 2;
        uint8_t MSGP2 : 2;
        uint8_t MSGP3 : 2;
        uint8_t MSGP4 : 2;
        uint8_t MSGP5 : 2;
        uint8_t MSGP6 : 2;
        uint8_t MSGP7 : 2;
        uint8_t MSGP8 : 2;
        uint8_t : 8;
    };
} dw1000_sr_gpio_mode_t;

/* 子寄存器 保留 - 0x26:04 */

/**
 * @brief 子寄存器 GPIO_DIR - 0x26:08 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t GDP0 : 1;
        uint8_t GDP1 : 1;
        uint8_t GDP2 : 1;
        uint8_t GDP3 : 1;
        uint8_t GDM0 : 1;
        uint8_t GDM1 : 1;
        uint8_t GDM2 : 1;
        uint8_t GDM3 : 1;
        uint8_t GDP4 : 1;
        uint8_t GDP5 : 1;
        uint8_t GDP6 : 1;
        uint8_t GDP7 : 1;
        uint8_t GDM4 : 1;
        uint8_t GDM5 : 1;
        uint8_t GDM6 : 1;
        uint8_t GDM7 : 1;
        uint8_t GDP8 : 1;
        uint8_t : 3;
        uint8_t GDM8 : 1;
        uint8_t : 3;
        uint8_t : 8;
    };
} dw1000_sr_gpio_dir_t;

/**
 * @brief 子寄存器 GPIO_DOUT - 0x26:0C - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t GOP0 : 1;
        uint8_t GOP1 : 1;
        uint8_t GOP2 : 1;
        uint8_t GOP3 : 1;
        uint8_t GOM0 : 1;
        uint8_t GOM1 : 1;
        uint8_t GOM2 : 1;
        uint8_t GOM3 : 1;
        uint8_t GOP4 : 1;
        uint8_t GOP5 : 1;
        uint8_t GOP6 : 1;
        uint8_t GOP7 : 1;
        uint8_t GOM4 : 1;
        uint8_t GOM5 : 1;
        uint8_t GOM6 : 1;
        uint8_t GOM7 : 1;
        uint8_t GOP8 : 1;
        uint8_t : 3;
        uint8_t GOM8 : 1;
        uint8_t : 3;
        uint8_t : 8;
    };
} dw1000_sr_gpio_dout_t;

/**
 * @brief 子寄存器 GPIO_IRQE - 0x26:10 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t GIRQE0 : 1;
        uint8_t GIRQE1 : 1;
        uint8_t GIRQE2 : 1;
        uint8_t GIRQE3 : 1;
        uint8_t GIRQE4 : 1;
        uint8_t GIRQE5 : 1;
        uint8_t GIRQE6 : 1;
        uint8_t GIRQE7 : 1;
        uint8_t GIRQE8 : 1;
        uint8_t : 7;
        uint16_t : 16;
    };
} dw1000_sr_gpio_irqe_t;

/**
 * @brief 子寄存器 GPIO_ISEN - 0x26:14 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t GISEN0 : 1;
        uint8_t GISEN1 : 1;
        uint8_t GISEN2 : 1;
        uint8_t GISEN4 : 1;
        uint8_t GISEN5 : 1;
        uint8_t GISEN6 : 1;
        uint8_t GISEN7 : 1;
        uint8_t GISEN8 : 1;
        uint8_t : 7;
        uint16_t : 16;
    };
} dw1000_sr_gpio_isen_t;

/**
 * @brief 子寄存器 GPIO_IMODE - 0x26:18 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t GIMOD0 : 1;
        uint8_t GIMOD1 : 1;
        uint8_t GIMOD2 : 1;
        uint8_t GIMOD4 : 1;
        uint8_t GIMOD5 : 1;
        uint8_t GIMOD6 : 1;
        uint8_t GIMOD7 : 1;
        uint8_t GIMOD8 : 1;
        uint8_t : 7;
        uint16_t : 16;
    };
} dw1000_sr_gpio_imode_t;

/**
 * @brief 子寄存器 GPIO_IBES - 0x26:1C - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t GIBES0 : 1;
        uint8_t GIBES1 : 1;
        uint8_t GIBES2 : 1;
        uint8_t GIBES4 : 1;
        uint8_t GIBES5 : 1;
        uint8_t GIBES6 : 1;
        uint8_t GIBES7 : 1;
        uint8_t GIBES8 : 1;
        uint8_t : 7;
        uint16_t : 16;
    };
} dw1000_sr_gpio_ibes_t;

/**
 * @brief 子寄存器 GPIO_ICLR - 0x26:20 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t GICLR0 : 1;
        uint8_t GICLR1 : 1;
        uint8_t GICLR2 : 1;
        uint8_t GICLR4 : 1;
        uint8_t GICLR5 : 1;
        uint8_t GICLR6 : 1;
        uint8_t GICLR7 : 1;
        uint8_t GICLR8 : 1;
        uint8_t : 7;
        uint16_t : 16;
    };
} dw1000_sr_gpio_iclr_t;

/**
 * @brief 子寄存器 GPIO_IDBE - 0x26:24 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t GIDBE0 : 1;
        uint8_t GIDBE1 : 1;
        uint8_t GIDBE2 : 1;
        uint8_t GIDBE4 : 1;
        uint8_t GIDBE5 : 1;
        uint8_t GIDBE6 : 1;
        uint8_t GIDBE7 : 1;
        uint8_t GIDBE8 : 1;
        uint8_t : 7;
        uint16_t : 16;
    };
} dw1000_sr_gpio_idbe_t;

/**
 * @brief 子寄存器 GPIO_RAW - 0x26:28 - RO
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t GRAWP0 : 1;
        uint8_t GRAWP1 : 1;
        uint8_t GRAWP2 : 1;
        uint8_t GRAWP4 : 1;
        uint8_t GRAWP5 : 1;
        uint8_t GRAWP6 : 1;
        uint8_t GRAWP7 : 1;
        uint8_t GRAWP8 : 1;
        uint8_t : 7;
        uint16_t : 16;
    };
} dw1000_sr_gpio_raw_t;

/**
 * @brief 寄存器 GPIO_CTRL - 0x26 - RW
 *
 * GPIO控制和状态
 */
typedef union {
    uint8_t value[44];
    struct {
        dw1000_sr_gpio_mode_t  GPIO_MODE;
        uint8_t                RES1[4];
        dw1000_sr_gpio_dir_t   GPIO_DIR;
        dw1000_sr_gpio_dout_t  GPIO_DOUT;
        dw1000_sr_gpio_irqe_t  GPIO_IRQE;
        dw1000_sr_gpio_isen_t  GPIO_ISEN;
        dw1000_sr_gpio_imode_t GPIO_IMODE;
        dw1000_sr_gpio_ibes_t  GPIO_IBES;
        dw1000_sr_gpio_iclr_t  GPIO_ICLR;
        dw1000_sr_gpio_idbe_t  GPIO_IDBE;
        dw1000_sr_gpio_raw_t   GPIO_RAW;
    };
} dw1000_reg_gpio_ctrl_t;


/**
 * @brief 子寄存器 DRX_TUNE0B - 0x27:02 - RW
 */
typedef uint16_t dw1000_sr_drx_tune0b_t;

/**
 * @brief 子寄存器 DRX_TUNE1A - 0x27:04 - RW
 */
typedef uint16_t dw1000_sr_drx_tune1a_t;

/**
 * @brief 子寄存器 DRX_TUNE1B - 0x27:06 - RW
 */
typedef uint16_t dw1000_sr_drx_tune1b_t;

/**
 * @brief 子寄存器 DRX_TUNE2 - 0x27:08 - RW
 */
typedef uint16_t dw1000_sr_drx_tune2_t;

/**
 * @brief 子寄存器 DRX_SFDTOC - 0x27:20 - RW
 */
typedef uint16_t dwd1000_sr_drx_sfdtoc_t;

/**
 * @brief 子寄存器 DRX_PRETOC - 0x27:24 - RW
 */
typedef uint16_t dw1000_sr_drx_pretoc_t;

/**
 * @brief 子寄存器 DRX_TUNE4H - 0x27:26 - RW
 */
typedef uint16_t dw1000_sr_drx_tune4h_t;

/**
 * @brief 子寄存器 DRX_CAR_INT - 0x27:28 - RW
 */
typedef uint16_t dw1000_sr_drx_car_int_t;

/**
 * @brief 子寄存器 RXPACC_NOSAT - 0x27:2C - RW
 */
typedef uint16_t dw1000_sr_rxpacc_nosat_t;

/**
 * @brief 寄存器 DRX_CONF - 0x27 - RW
 *
 * 数字接收器配置
 */
typedef union {
    uint8_t value[46];
    struct {
        uint8_t                  RES1[2];
        dw1000_sr_drx_tune0b_t   DRX_TUNE0B;
        dw1000_sr_drx_tune1a_t   DRX_TUNE1A;
        dw1000_sr_drx_tune1b_t   DRX_TUNE1B;
        dw1000_sr_drx_tune2_t    DRX_TUNE2;
        uint8_t                  RES2[20];
        dwd1000_sr_drx_sfdtoc_t  DRX_SFDTOC;
        uint8_t                  RES[2];
        dw1000_sr_drx_pretoc_t   DRX_PRETOC;
        dw1000_sr_drx_tune4h_t   DRX_TUNE4H;
        dw1000_sr_drx_car_int_t  DRX_CAR_INT;
        dw1000_sr_rxpacc_nosat_t RXPACC_NOSAT;
    };
} dw1000_reg_drx_conf_t;


/**
 * @brief 子寄存器 RF_CONF - 0x28:00 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t : 8;
        uint8_t TXFEN : 5;
        uint8_t PLLFEN : 3;
        uint8_t LDOFEN : 5;
        uint8_t TXRXSW : 2;
        uint8_t : 1;
        uint8_t : 8;
    };
} dw1000_sr_rf_conf_t;

/* 子寄存器 保留 - 0x28:04 */

/**
 * @brief 子寄存器 RF_RXCTRLH - 0x28:0B - RW
 */
typedef uint8_t dw1000_sr_rf_rxctrlh_t;

/**
 * @brief 子寄存器 RF_TXCTRL - 0x28:0C - RW
 */
typedef union {
    uint8_t value[3];
    struct {
        uint16_t : 5;
        uint16_t TXMTUNE : 4;
        uint16_t TXMQ : 3;
        uint16_t : 4;
        uint8_t : 8;
    };
} dw1000_sr_rf_txctrl_t;

/* 子寄存器 保留 - 0x28:10 */

/**
 * @brief 子寄存器 RF_STATUS - 0x28:2C - RO
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t CPLLLOCK : 1;
        uint8_t CPLLLOW : 1;
        uint8_t CPLLHIGH : 1;
        uint8_t RFPLLLOCK : 1;
        uint8_t : 4;
        uint8_t : 8;
        uint16_t : 16;
    };
} dw1000_sr_rf_status_t;

/**
 * @brief 子寄存器 LDOTUNE - 0x28:30 - RW
 */
typedef uint8_t dw1000_sr_ldotune_t[5];

/**
 * @brief 寄存器 RF_CONF - 0x28 -
 *
 * 模拟RF配置块
 */
typedef union {
    uint8_t value[53];
    struct {
        dw1000_sr_rf_conf_t    RF_CONF;
        uint8_t                RES1[7];
        dw1000_sr_rf_rxctrlh_t RF_RXCTRLH;
        dw1000_sr_rf_txctrl_t  RF_TXCTRL;
        uint8_t                RES2[16];
        dw1000_sr_rf_status_t  RF_STATUS;
        dw1000_sr_ldotune_t    LDOTUNE;
    };
} dw1000_reg_rf_conf_t;


/* 寄存器 保留 - 0x29 */


/**
 * @brief 子寄存器 TX_SARC - 0x2A:00 - RW
 */
typedef union {
    uint8_t value[2];
    struct {
        uint8_t SAR_CTRL : 1;
        uint8_t : 7;
        uint8_t : 8;
    };
} dw1000_sr_tc_sarc_t;

/**
 * @brief 子寄存器 TC_SARL - 0x2A:03 - RW
 */
typedef union {
    uint8_t value[3];
    struct {
        uint8_t SAR_LVBAT;
        uint8_t SAR_LTEMP;
        uint8_t : 8;
    };
} dw1000_sr_tc_sarl_t;

/**
 * @brief 子寄存器 TC_SARW - 0x2A:06 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint8_t SAR_WVBAT;
        uint8_t SAR_WTEMP;
    };
} dw1000_sr_tc_sarw_t;

/**
 * @brief 子寄存器 TC_PG_CTRL - 0x2A:08 - RW
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t PG_START : 1;
        uint8_t : 1;
        uint8_t PG_TMEAS : 4;
        uint8_t : 2;
    };
} dw1000_sr_tc_pg_ctrl_t;

/**
 * @brief 子寄存器 TC_PG_STATUS - 0x2A:09 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t DELAY_CNT : 12;
        uint16_t : 4;
    };
} dw1000_sr_tc_pg_status_t;

/**
 * @brief 子寄存器 TC_PGDELAY - 0x2A:0B - RW
 */
typedef uint8_t dw1000_sr_tc_pgdelay_t;

/**
 * @brief 子寄存器 TC_PGTEST- 0x2A:0C - RW
 */
typedef uint8_t dw1000_sr_tc_pgtest_t;

/**
 * @brief 寄存器 TX_CAL - 0x2A -
 *
 * 发送校准块
 */
typedef union {
    uint8_t value[13];
    struct {
        dw1000_sr_tc_sarc_t      TC_SARC;
        uint8_t                  RES;
        dw1000_sr_tc_sarl_t      TC_SARL;
        dw1000_sr_tc_sarw_t      TC_SARW;
        dw1000_sr_tc_pg_ctrl_t   TC_PG_CTRL;
        dw1000_sr_tc_pg_status_t TC_PG_STATUS;
        dw1000_sr_tc_pgdelay_t   TC_PGDELAY;
        dw1000_sr_tc_pgtest_t    TC_PGTEST;
    };
} dw1000_reg_tx_cal_t;


/**
 * @brief 子寄存器 FS_PLLCFG - 0x2B:07 - RW
 */
typedef uint32_t dw1000_sr_fs_pllcfg_t;

/**
 * @brief 子寄存器 FS_PLLTUNE - 0x2B:0B - RW
 */
typedef uint8_t dw1000_sr_fs_plltune_t;

/**
 * @brief 子寄存器 FS_XTALT - 0x2B:0E - RW
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t XTALT : 5;
        uint8_t : 3;
    };
} dw1000_sr_fs_xtalt_t;

/**
 * @brief 寄存器 FS_CTRL - 0x2B -
 *
 * 频率合成控制块
 */
typedef union {
    uint8_t value[21];
    struct {
        uint8_t                RES1[7];
        dw1000_sr_fs_pllcfg_t  FS_PLLCFG;
        dw1000_sr_fs_plltune_t FS_PLLTUNE;
        uint8_t                RES2[2];
        dw1000_sr_fs_xtalt_t   FS_XTALT;
        uint8_t                RES3[6];
    };
} dw1000_reg_fs_ctrl_t;


/**
 * @brief 子寄存器 AON_WCFG - 0x2C:00 - RW
 */
typedef union {
    uint8_t value[2];
    struct {
        uint8_t ONW_RADC : 1;
        uint8_t ONW_RX : 1;
        uint8_t : 1;
        uint8_t ONW_LEUI : 1;
        uint8_t : 2;
        uint8_t ONW_LDC : 1;
        uint8_t ONW_L64P : 1;
        uint8_t PRES_SLEEP : 1;
        uint8_t : 2;
        uint8_t ONW_LLDE : 1;
        uint8_t ONW_LLDO : 1;
        uint8_t : 3;
    };
} dw1000_sr_aon_wcfg_t;

/**
 * @brief 子寄存器 AON_CTRL - 0x2C:02 - RW
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t RESTORE : 1;
        uint8_t SAVE : 1;
        uint8_t UPL_CFG : 1;
        uint8_t DCA_READ : 1;
        uint8_t : 3;
        uint8_t DCA_ENAB : 1;
    };
} dw1000_sr_aon_ctrl_t;

/**
 * @brief 子寄存器 AON_RDAT - 0x2C:03 - RW
 */
typedef uint8_t dw1000_sr_aon_rdat_t;

/**
 * @brief 子寄存器 AON_ADDR - 0x2C:04 - RW
 */
typedef uint8_t dw1000_sr_aon_addr_t;

/* 子寄存器 保留 - 0x2C:05 - RW */

/**
 * @brief 子寄存器 AON_CFG0 - 0x2C:06 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint16_t SLEEP_EN : 1;
        uint16_t WAKE_PIN : 1;
        uint16_t WAKE_SPI : 1;
        uint16_t WAKE_CNT : 1;
        uint16_t LPDIV_EN : 1;
        uint16_t LPCLKDIVA : 11;
        uint16_t SLEEP_TIM;
    };
} dw1000_sr_aon_cfg0_t;

/**
 * @brief 子寄存器 AON_CFG1 - 0x2C:0A - RW
 */
typedef union {
    uint8_t value[2];
    struct {
        uint8_t SLEEP_CEN : 1;
        uint8_t SMXX : 1;
        uint8_t LPOSC_CAL : 1;
        uint8_t : 5;
        uint8_t : 8;
    };
} dw1000_sr_aon_cfg1_t;


/**
 * @brief 寄存器 AON - 0x2C - RW
 *
 * AON系统控制接口
 */
typedef union {
    uint8_t value[12];
    struct {
        dw1000_sr_aon_wcfg_t AON_WCFG;
        dw1000_sr_aon_ctrl_t AON_CTRL;
        dw1000_sr_aon_rdat_t AON_RDAT;
        dw1000_sr_aon_addr_t AON_ADDR;
        uint8_t              RES;
        dw1000_sr_aon_cfg0_t AON_CFG0;
        dw1000_sr_aon_cfg1_t AON_CFG1;
    };
} dw1000_reg_aon_t;


/**
 * @brief 子寄存器 OTP_WDAT - 0x2D:00 - RW
 */
typedef uint32_t dw1000_sr_otp_wdat_t;

/**
 * @brief 子寄存器 OTP_ADDR - 0x2D:04 - RW
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t OTPADDR : 11;
        uint16_t : 5;
    };
} dw1000_sr_otp_addr_t;

/**
 * @brief 子寄存器 OTP_CTRL - 0x2D:06 - RW
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t OTPRDEN : 1;
        uint16_t OTPREAD : 1;
        uint16_t : 1;
        uint16_t OTPMRWR : 1;
        uint16_t : 2;
        uint16_t OTPPROG : 1;
        uint16_t OTPMR : 4;
        uint16_t : 4;
        uint16_t LDELOAD : 1;
    };
} dw1000_sr_otp_ctrl_t;

/**
 * @brief 子寄存器 OTP_STAT - 0x2D:08 - RW
 */
typedef union {
    uint8_t value[2];
    struct {
        uint8_t OTPPRGD : 1;
        uint8_t OTPVPOK : 1;
        uint8_t : 6;
        uint8_t : 8;
    };
} dw1000_sr_otp_stat_t;

/**
 * @brief 子寄存器 OTP_RDAT - 0x2D:0A - R
 */
typedef uint32_t dw1000_sr_otp_rdat_t;

/**
 * @brief 子寄存器 OTP_SRDAT- 0x2D:0E - RW
 */
typedef uint32_t dw1000_sr_otp_srdat_t;

/**
 * @brief 子寄存器 OTP_SF - 0x2D:12 - RW
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t OPS_KICK : 1;
        uint8_t LDO_KICK : 1;
        uint8_t : 3;
        uint8_t OPS_SEL : 2;
        uint8_t : 1;
    };
} dw1000_sr_otp_sf_t;

/**
 * @brief 寄存器 OTP_IF - 0x2D -
 *
 * OTP存储接口
 */
typedef union {
    uint8_t value[19];
    struct {
        dw1000_sr_otp_wdat_t  OTP_WDAT;
        dw1000_sr_otp_addr_t  OTP_ADDR;
        dw1000_sr_otp_ctrl_t  OTP_CTRL;
        dw1000_sr_otp_rdat_t  OTP_RDAT;
        dw1000_sr_otp_srdat_t OTP_SRDAT;
        dw1000_sr_otp_sf_t    OTP_SF;
    };
} dw1000_reg_otp_if_t;


/**
 * @brief 子寄存器 LDE_THRESH - 0x2E:0000 - RO
 */
typedef uint16_t dw1000_sr_lde_thresh_t;

/* 子寄存器 保留 - 0x2E:0002 */

/**
 * @brief 子寄存器 LDE_CFG1 - 0x2E:0806 - RW
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t NTM : 5;
        uint8_t PMULT : 3;
    };
} dw1000_sr_lde_cfg1_t;

/* 子寄存器 保留 - 0x2E:0807 */

/**
 * @brief 子寄存器 LDE_PPINDX - 0x2E:1000 - RO
 */
typedef uint16_t dw1000_sr_lde_ppindx_t;

/**
 * @brief 子寄存器 LDE_PPAMPL - 0x2E:1002 - RO
 */
typedef uint16_t dw1000_sr_lde_ppampl_t;

/* 子寄存器 保留 - 0x2E:1004 */

/**
 * @brief 子寄存器 LDE_RXANTD - 0x2E:1804 - RW
 */
typedef uint16_t dw1000_sr_lde_rxantd_t;

/**
 * @brief 子寄存器 LDE_CFG2 - 0x2E:1806 - RW
 */
typedef uint16_t dw1000_sr_lde_cfg2_t;

/* 子寄存器 保留 - 0x2E:1808 */

/**
 * @brief 子寄存器 LDE_REPC - 0x2E:2804 - RW
 */
typedef uint16_t dw1000_sr_lde_repc_t;

/**
 * @brief 寄存器 LDE_IF - 0x2E -
 *
 * 前边沿检测接口
 */
typedef union {
    uint8_t value[10246];
    struct {
        dw1000_sr_lde_thresh_t LDE_THRESH;
        uint8_t                RES1[2052];
        dw1000_sr_lde_cfg1_t   LDE_CFG1;
        uint8_t                RES2[2041];
        dw1000_sr_lde_ppindx_t LDE_PPINDX;
        dw1000_sr_lde_ppampl_t LDE_PPAMPL;
        uint8_t                RES3[2048];
        dw1000_sr_lde_rxantd_t LDE_RXANTD;
        dw1000_sr_lde_cfg2_t   LDE_CFG2;
        uint8_t                RES4[4092];
        dw1000_sr_lde_repc_t   LDE_REPC;
    };
} dw1000_reg_lde_if_t;


/**
 * @brief 子寄存器 EVC_CTRL - 0x2F:00 - SRW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t EVC_EN : 1;
        uint8_t EVC_CLR : 1;
        uint8_t : 6;
        uint8_t : 8;
        uint16_t : 8;
    };
} dw1000_sr_evc_ctrl_t;

/**
 * @brief 子寄存器 EVC_PHE - 0x2F:04 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_PHE : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_phe_t;

/**
 * @brief 子寄存器 EVC_RSE - 0x2F:06 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_RSE : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_rse_t;

/**
 * @brief 子寄存器 EVC_FCG - 0x2F:08 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_FCG : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_fcg_t;

/**
 * @brief 子寄存器 EVC_FCE - 0x2F:0A - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_FCE : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_fce_t;

/**
 * @brief 子寄存器 EVC_FFR - 0x2F:0C - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_FFR : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_ffr_t;

/**
 * @brief 子寄存器 EVC_OVR - 0x2F:0E - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_OVR : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_ovr_t;

/**
 * @brief 子寄存器 EVC_STO - 0x2F:10 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_STO : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_sto_t;

/**
 * @brief 子寄存器 EVC_PTO - 0x2F:12 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_PTO : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_pto_t;

/**
 * @brief 子寄存器 EVC_FWTO - 0x2F:14 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_FWTO : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_fwto_t;

/**
 * @brief 子寄存器 EVC_TXFS - 0x2F:16 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_TXFS : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_txfs_t;

/**
 * @brief 子寄存器 EVC_HPW - 0x2F:18 - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_HPW : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_hpw_t;

/**
 * @brief 子寄存器 EVC_TPW - 0x2F:1A - RO
 */
typedef union {
    uint8_t value[2];
    struct {
        uint16_t EVC_TPW : 12;
        uint16_t : 4;
    };
} dw1000_sr_evc_tpw_t;

/* 子寄存器 保留 - 0x2F:1C */

/**
 * @brief 子寄存器 DIAG_TMC - 0x2F:24 - RW
 */
typedef union {
    uint8_t value[2];
    struct {
        uint8_t : 4;
        uint8_t TX_PSTM : 1;
        uint8_t : 3;
        uint8_t : 8;
    };
} dw1000_sr_diag_tmc_t;

/**
 * @brief 寄存器 DIG_DIAG - 0x2F -
 *
 * 数字诊断接口
 */
typedef union {
    uint8_t value[38];
    struct {
        dw1000_sr_evc_ctrl_t EVC_CTRL;
        dw1000_sr_evc_phe_t  EVC_PHE;
        dw1000_sr_evc_rse_t  EVC_RSE;
        dw1000_sr_evc_fcg_t  EVC_FCG;
        dw1000_sr_evc_fce_t  EVC_FCE;
        dw1000_sr_evc_ffr_t  EVC_FFR;
        dw1000_sr_evc_ovr_t  EVC_OVR;
        dw1000_sr_evc_sto_t  EVC_STO;
        dw1000_sr_evc_pto_t  EVC_PTO;
        dw1000_sr_evc_fwto_t EVC_FWTO;
        dw1000_sr_evc_txfs_t EVC_TXFS;
        dw1000_sr_evc_hpw_t  EVC_HPW;
        dw1000_sr_evc_tpw_t  EVC_TPW;
        uint8_t              RES[8];
        dw1000_sr_diag_tmc_t DIAG_TMC;
    };
} dw1000_reg_dig_diag_t;


/* 寄存器 保留 - 0x30到0x35 */


/**
 * @brief 子寄存器 PMSC_CTRL0 - 0x36:00 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t SYSCLKS : 2;
        uint8_t RXCLKS : 2;
        uint8_t TXCLKS : 2;
        uint8_t FACE : 1;
        uint8_t : 1;
        uint8_t : 2;
        uint8_t ADCCE : 1;
        uint8_t : 4;
        uint8_t AMCE : 1;
        uint8_t GPCE : 1;
        uint8_t GPRN : 1;
        uint8_t GPDCE : 1;
        uint8_t GPDRN : 1;
        uint8_t : 3;
        uint8_t KHZCLKEN : 1;
        uint8_t PLL2_SEQ_EN : 1;
        uint8_t : 3;
        uint8_t SOFTRESET : 4;
    };
} dw1000_sr_pmsc_ctrl0_t;

/**
 * @brief 子寄存器 PMSC_CTRL1 - 0x36:04 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint16_t : 1;
        uint16_t ARX2INIT : 1;
        uint16_t : 1;
        uint16_t PKTSEQ : 8;
        uint16_t ATXSLP : 1;
        uint16_t ARXSLP : 1;
        uint16_t SNOZE : 1;
        uint16_t SNOZR : 1;
        uint16_t PLLSYN : 1;
        uint16_t : 1;
        uint16_t LDERUNE : 1;
        uint16_t : 8;
        uint16_t KHZCLKDIV : 6;
    };
} dw1000_sr_pmsc_ctrl1_t;

/* 子寄存器 保留 - 0x36:08 */

/**
 * @brief 子寄存器 PMSC_SNOZT - 0x36:0C - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t SNOZ_TIM;
        uint8_t : 8;
        uint16_t : 16;
    };
} dw1000_sr_pmsc_snozt_t;

/* 子寄存器 保留 - 0x36:10 */

/**
 * @brief 子寄存器 PMSC_TXFSEQ - 0x36:26 - RW
 */
typedef struct {
    uint16_t TXFINESEQ;
} dw1000_sr_pmsc_txfseq_t;

/**
 * @brief 子寄存器 PMSC_LEDC - 0x36:28 - RW
 */
typedef union {
    uint8_t value[4];
    struct {
        uint8_t BLINK_TIM;
        uint8_t BLNKEN : 1;
        uint8_t : 7;
        uint8_t BLNKNOW : 4;
        uint8_t : 4;
        uint8_t : 8;
    };
} dw1000_sr_pmsc_ledc_t;


/**
 * @brief 寄存器 PMSC - 0x36 -
 *
 * 电源管理和系统控制
 */
typedef union {
    uint8_t value[44];
    struct {
        dw1000_sr_pmsc_ctrl0_t  PMSC_CTRL0;
        dw1000_sr_pmsc_ctrl1_t  PMSC_CTRL1;
        uint8_t                 RES1[4];
        dw1000_sr_pmsc_snozt_t  PMSC_SNOZT;
        uint8_t                 RES2[22];
        dw1000_sr_pmsc_txfseq_t PMSC_TXFSEQ;
        dw1000_sr_pmsc_ledc_t   PMSC_LEDC;
    };
} dw1000_reg_pmsc_t;


/* 寄存器 保留 - 0x37到0x3F */


#pragma pack(pop)
