/**
 * @file drv_dw1000_reg.h
 * @author quote6 (HeXiHeNian@qq.com)
 * @brief
 * @version 0.1
 * @date 2026-07-28
 *
 * @note 寄存器的长度单位为字节
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

/******************************************************************************/
/**
 * @defgroup DW1000_REG_DEV_ID
 * @brief 寄存器文件 DEV_ID - 0x00 - [只读]
 *
 * 设备标识符
 *
 * @{
 */

#define DW1000_REG_DEV_ID             0x00
#define DW1000_REG_DEV_ID_LEN         (4)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_EUI
 * @brief 寄存器文件 EUI - 0x01 - [读写]
 *
 * 扩展唯一标识符
 *
 * @note 该寄存器可以按字节访问，也就是可以作为8个8字节的子寄存器进行访问
 *
 * @{
 */

#define DW1000_REG_EUI                0x01
#define DW1000_REG_EUI_LEN            (8)

/**
 * @defgroup DW1000_REG_EUI_SR
 * @ingroup DW1000_REG_EUI
 * @{
 */

#define DW1000_SR_EUI_0_OFFSET        0x00 /* 扩展标识符的Bit[7:0] */
#define DW1000_SR_EUI_1_OFFSET        0x01 /* 扩展标识符的Bit[15:8] */
#define DW1000_SR_EUI_2_OFFSET        0x02 /* 扩展标识符的Bit[23:16] */
#define DW1000_SR_EUI_3_OFFSET        0x03 /* 扩展标识符的Bit[31:24] */
#define DW1000_SR_EUI_4_OFFSET        0x04 /* 扩展标识符的Bit[39:32] */
#define DW1000_SR_EUI_5_OFFSET        0x05 /* 制造商ID的Bit[7:0] */
#define DW1000_SR_EUI_6_OFFSET        0x06 /* 制造商ID的Bit[15:8] */
#define DW1000_SR_EUI_7_OFFSET        0x07 /* 制造商ID的Bit[23:16] */

#define DW1000_SR_EUI_X_LEN           (1)

/** @} */
/** @} */
/******************************************************************************/

/* 寄存器文件 保留 - 0x02 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_PANADR
 * @brief 寄存器文件 PANADR - 0x03 - [读写]
 *
 * PAN标识符和短地址
 *
 * @{
 */

#define DW1000_REG_PANADR             0x03
#define DW1000_REG_PANADR_LEN         (4)

/**
 * @defgroup DW1000_REG_PANADR_SR
 * @ingroup DW1000_REG_PANADR
 * @{
 */

#define DW1000_SR_SHORT_ADDR_OFFSET   0x00
#define DW1000_SR_SHORT_ADDR_LEN      (2)

#define DW1000_SR_PAN_ID_OFFSET       0x02
#define DW1000_SR_PAN_ID_LEN          (2)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_SYS_CFG
 * @brief 寄存器文件 SYS_CFG - 0x04 - [读写]
 *
 * 系统配置位图
 *
 * @{
 */

#define DW1000_REG_SYS_CFG            0x04
#define DW1000_REG_SYS_CFG_LEN        (4)

/**
 * @defgroup DW1000_REG_SYS_CFG_SR
 * @ingroup DW1000_REG_SYS_CFG
 *
 * @{
 */

#define DW1000_SR_SYS_CFG_0_OFFSET    0x00
#define DW1000_SR_SYS_CFG_1_OFFSET    0x01
#define DW1000_SR_SYS_CFG_2_OFFSET    0x02
#define DW1000_SR_SYS_CFG_3_OFFSET    0x03

#define DW1000_SR_SYS_CFG_X_LEN       (1)

/** @} */
/** @} */
/******************************************************************************/

/* 寄存器文件 0x05 - 保留 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_SYS_TIME
 * @brief 寄存器文件 SYS_TIME - 0x06 - [只读]
 *
 * 系统时间计数器
 *
 * @{
 */

#define DW1000_REG_SYS_TIME           0x06
#define DW1000_REG_SYS_TIME_LEN       (5)

/** @} */
/******************************************************************************/

/* 寄存器文件 0x07 - 保留 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_TX_FCTRL
 * @brief 寄存器文件 TX_FCTRL - 0x08 - [读写]
 *
 * 发送帧控制
 *
 * @{
 */

#define DW1000_REG_TX_FCTRL           0x08
#define DW1000_REG_TX_FCTRL_LEN       (5)

/**
 * @defgroup DW1000_REG_TX_FCTRL_SR
 * @ingroup DW1000_REG_TX_FCTRL
 *
 * @{
 */

#define DW1000_SR_TX_FCTRL_0_OFFSET   0x00
#define DW1000_SR_TX_FCTRL_0_LEN      (2)

#define DW1000_SR_TX_FCTRL_1_OFFSET   0x02
#define DW1000_SR_TX_FCTRL_1_LEN      (2)

#define DW1000_SR_TX_FCTRL_2_OFFSET   0x04
#define DW1000_SR_TX_FCTRL_2_LEN      (1)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_TX_BUFFER
 * @brief 寄存器文件 TX_BUFFER - 0x09 - [只写]
 *
 * 发送数据缓冲区
 *
 * @{
 */

#define DW1000_REG_TX_BUFFER          0x09
#define DW1000_REG_TX_BUFFER_LEN      (1024)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_DX_TIME
 * @brief 寄存器文件 DX_TIME - 0x0A - [读写]
 *
 * 延迟发送或接收时间
 *
 * @{
 */

#define DW1000_REG_DX_TIME            0x0A
#define DW1000_REG_DX_TIME_LEN        (5)

/** @} */
/******************************************************************************/

/* 寄存器文件 0x0B - 保留 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_RX_FWTO
 * @brief 寄存器文件 RX_FWTO - 0x0C - [读写]
 *
 * 接收帧等待超时时间
 *
 * @{
 */

#define DW1000_REG_RX_FWTO            0x0C
#define DW1000_REG_RX_FWTO_LEN        (2)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_SYS_CTRL
 * @brief 寄存器文件 SYS_CTRL - 0x0D - [特殊读写]
 *
 * 系统控制寄存器
 *
 * @{
 */

#define DW1000_REG_SYS_CTRL           0x0D
#define DW1000_REG_SYS_CTRL_LEN       (4)

/**
 * @defgroup DW1000_REG_SYS_CTRL_SR
 * @ingroup DW1000_REG_SYS_CTRL
 *
 * @{
 */

#define DW1000_SR_SYS_CTRL_0_OFFSET   0x00
#define DW1000_SR_SYS_CTRL_1_OFFSET   0x01
#define DW1000_SR_SYS_CTRL_2_OFFSET   0x02
#define DW1000_SR_SYS_CTRL_3_OFFSET   0x03

#define DW1000_SR_SYS_CTRL_X_LEN      (1)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_SYS_MASK
 * @brief 寄存器文件 SYS_MASK - 0x0E - [读写]
 *
 * 系统事件掩码寄存器
 *
 * @{
 */

#define DW1000_REG_SYS_MASK           0x0E
#define DW1000_REG_SYS_MASK_LEN       (4)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_SYS_STATUS
 * @brief 寄存器文件 SYS_STATUS -0x0F - [特殊读写]
 *
 * 系统事件状态寄存器
 *
 * @{
 */

#define DW1000_REG_SYS_STATUS         0x0F
#define DW1000_REG_SYS_STATUS_LEN     (5)

/**
 * @defgroup DW1000_REG_SYS_STATUS_SR
 * @ingroup DW1000_REG_SYS_STATUS
 */

#define DW1000_SR_SYS_STATUS_0_OFFSET 0x00
#define DW1000_SR_SYS_STATUS_1_OFFSET 0x01
#define DW1000_SR_SYS_STATUS_2_OFFSET 0x02
#define DW1000_SR_SYS_STATUS_3_OFFSET 0x03
#define DW1000_SR_SYS_STATUS_4_OFFSET 0x04

#define DW1000_SR_SYS_STATUS_X_LEN    (1)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_RX_FINFO
 * @brief 寄存器文件 RX_FINFO - 0x10 - [只读双缓冲]
 *
 * 接收帧信息寄存器
 *
 * @{
 */

#define DW1000_REG_RX_FINFO           0x10
#define DW1000_REG_RX_FINFO_LEN       (4)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_RX_BUFFER
 * @brief 寄存器文件 RX_BUFFER - 0x11 - [只读双缓冲]
 *
 * 接收帧数据
 *
 * @{
 */

#define DW1000_REG_RX_BUFFER          0x11
#define DW1000_REG_RX_BUFFER_LEN      (1024)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_RX_FQUAL
 * @brief 寄存器文件 RX_FQUAL - 0x12 - [只读双缓冲]
 *
 * 接收帧质量信息
 *
 * @note  用户手册中并未有4个子寄存器，但是为了方便访问，这里划分为4个子寄存器
 *
 * @{
 */

#define DW1000_REG_RX_FQUAL           0x12
#define DW1000_REG_RX_FQUAL_LEN       (8)

/**
 * @defgroup DW1000_REG_RX_FQUAL_SR
 * @ingroup DW1000_REG_RX_FQUAL
 *
 * @{
 */

#define DW1000_SR_STD_NOISE_OFFSET    0x00
#define DW1000_SR_STD_NOISE_LEN       (2)

#define DW1000_SR_FP_AMPL2_OFFSET     0x02
#define DW1000_SR_FP_AMPL2_LEN        (2)

#define DW1000_SR_PP_AMPL3_OFFSET     0x04
#define DW1000_SR_PP_AMPL3_LEN        (2)

#define DW1000_SR_CIR_PWR_OFFSET      0x06
#define DW1000_SR_CIR_PWR_LEN         (2)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_RX_TTCKI
 * @brief 寄存器文件 RX_TTCKI - 0x13 - [只读双缓冲]
 *
 * 接收器时间跟踪间隔
 *
 * @{
 */

#define DW1000_REG_RX_TTCKI           0x13
#define DW1000_REG_RX_TTCKI_LEN       (4) /* 字节 */

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_RX_TTCKO
 * @brief 寄存器文件 RX_TTCKO - 0x14 - [只读双缓冲]
 *
 * 接收器时间跟踪偏移量
 *
 * @note  用户手册中并未有4个子寄存器，但是为了方便访问，这里划分为4个子寄存器
 *
 * @{
 */

#define DW1000_REG_RX_TTCKO           0x14
#define DW1000_REG_RX_TTCKO_LEN       (5)

/**
 * @defgroup DW1000_REG_RX_TTCKO_SR
 * @ingroup DW1000_REG_RX_TTCKO
 *
 * @{
 */

#define DW1000_SR_RX_TTCKO_0_OFFSET   0x00
#define DW1000_SR_RX_TTCKO_0_LEN      (3)

#define DW1000_SR_RX_TTCKO_1_OFFSET   0x03
#define DW1000_SR_RX_TTCKO_1_LEN      (1)

#define DW1000_SR_RX_TTCKO_2_OFFSET   0x04
#define DW1000_SR_RX_TTCKO_2_LEN      (1)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_RX_TIME
 * @brief 寄存器文件 RX_TIME - 0x15 - [只读双缓冲]
 *
 * 接收时间戳
 *
 * @{
 */

#define DW1000_REG_RX_TIME            0x15
#define DW1000_REG_RX_TIME_LEN        (14)

/**
 * @defgroup DW1000_REG_RX_TIME_SR
 * @ingroup DW1000_REG_RX_TIME
 *
 * @{
 */

#define DW1000_SR_RX_STAMP_OFFSET     0x00
#define DW1000_SR_RX_STAMP_LEN        (5)

#define DW1000_SR_FP_INDEX_OFFSET     0x05
#define DW1000_SR_FP_INDEX_LEN        (2)

#define DW1000_SR_FP_AMPL1_OFFSET     0x07
#define DW1000_SR_FP_AMPL1_LEN        (2)

#define DW1000_SR_RX_RAWST_OFFSET     0x09
#define DW1000_SR_RX_RAWST_LEN        (5)

/** @} */
/** @} */
/******************************************************************************/

/* 寄存器文件 保留 - 0x16 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_TX_TIME
 * @brief 寄存器文件 TX_TIME - 0x17 - [只读]
 *
 * 消息发送时间戳
 *
 * @{
 */

#define DW1000_REG_TX_TIME            0x17
#define DW1000_REG_TX_TIME_LEN        (10)

/**
 * @defgroup DW1000_REG_TX_TIME_SR
 * @ingroup DW1000_REG_TX_TIME
 *
 * @{
 */

#define DW1000_SR_TX_STAMP_OFFSET     0x00
#define DW1000_SR_TX_STAMP_LEN        (5)

#define DW1000_SR_TX_RAWST_OFFSET     0x05
#define DW1000_SR_TX_RAWST_LEN        (5)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_TX_ANTD
 * @brief 寄存器文件 TX_ANTD - 0x18 - [读写]
 *
 * 发送至天线时延
 *
 * @{
 */

#define DW1000_REG_TX_ANTD            0x18
#define DW1000_REG_TX_ANTD_LEN        (4)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_SYS_STATE
 * @brief 寄存器文件 SYS_STATE - 0x19 - [只读]
 *
 * DW1000系统状态信息
 *
 * @{
 */

#define DW1000_REG_SYS_STATE          0x19
#define DW1000_REG_SYS_STATE_LEN      (4)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_ACK_RESP_T
 * @brief 寄存器文件 ACK_RESP_T - 0x1A - [读写]
 *
 * 应答时间和响应时间
 *
 * @{
 */

#define DW1000_REG_ACK_RESP_T         0x1A
#define DW1000_REG_ACK_RESP_T_LEN     (4)

/**
 * @defgroup DW1000_REG_ACK_RESP_T_SR
 * @ingroup DW1000_REG_ACK_RESP_T
 *
 * @{
 */

#define DW1000_SR_W4R_TIM_OFFSET      0x00
#define DW1000_SR_W4R_TIM_LEN         (3)

#define DW1000_SR_ACK_TIM_OFFSET      0x03
#define DW1000_SR_ACK_TIM_LEN         (1)

/** @} */
/** @} */
/******************************************************************************/

/* 寄存器文件 保留 - 0x1B */

/******************************************************************************/

/* 寄存器文件 保留 - 0x1C */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_RX_SNIFF
 * @brief 寄存器文件 RX_SNIFF - 0x1D - [读写]
 *
 * 嗅探模式配置
 *
 * @{
 */

#define DW1000_REG_RX_SNIFF           0x1D
#define DW1000_REG_RX_SNIFF_LEN       (4)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_TX_POWER
 * @brief 寄存器文件 TX_POWER - 0x1E - [读写]
 *
 * TX功率控制
 *
 * @{
 */

#define DW1000_REG_TX_POWER           0x1E
#define DW1000_REG_TX_POWER_LEN       (4)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_CHAN_CTRL
 * @brief 寄存器文件 CHAN_CTRL - 0x1F - [读写]
 *
 * 通道控制
 *
 * @{
 */

#define DW1000_REG_CHAN_CTRL          0x1F
#define DW1000_REG_CHAN_CTRL_LEN      (4)

/** @} */
/******************************************************************************/

/* 寄存器文件 保留 - 0x20 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_USR_SFD
 * @brief 寄存器文件 USR_SFD - 0x21 - [读写]
 *
 * 用户定义SFD序列
 *
 * @{
 */

#define DW1000_REG_USR_SFD            0x21
#define DW1000_REG_USR_SFD_LEN        (41)

/**
 * @defgroup DW1000_REG_USR_SFD_SR
 * @ingroup DW1000_REG_USR_SFD
 *
 * @{
 */

#define DW1000_SR_SFD_LENGTH_OFFSET   0x00
#define DW1000_SR_SFD_LENGTH_LEN      (1)

#define DW1000_SR_TX_SSFD_MAGL_OFFSET 0x01
#define DW1000_SR_TX_SSFD_MAGL_LEN    (1)

#define DW1000_SR_TX_SSFD_MAGH_OFFSET 0x02
#define DW1000_SR_TX_SSFD_MAGH_LEN    (1)

#define DW1000_SR_TX_SSFD_SGNL_OFFSET 0x03
#define DW1000_SR_TX_SSFD_SGNL_LEN    (1)

#define DW1000_SR_TX_SSFD_SGNH_OFFSET 0x04
#define DW1000_SR_TX_SSFD_SGNH_LEN    (1)

#define DW1000_SR_RX_SSFD_MAGL_OFFSET 0x05
#define DW1000_SR_RX_SSFD_MAGL_LEN    (1)

#define DW1000_SR_RX_SSFD_MAGH_OFFSET 0x06
#define DW1000_SR_RX_SSFD_MAGH_LEN    (1)

#define DW1000_SR_RX_SSFD_SGNL_OFFSET 0x07
#define DW1000_SR_RX_SSFD_SGNL_LEN    (1)

#define DW1000_SR_RX_SSFD_SGNH_OFFSET 0x08
#define DW1000_SR_RX_SSFD_SGNH_LEN    (1)

#define DW1000_SR_TX_LSFD_MAG_OFFSET  0x09
#define DW1000_SR_TX_LSFD_MAG_LEN     (8)

#define DW1000_SR_TX_LSFD_SGN_OFFSET  0x11
#define DW1000_SR_TX_LSFD_SGN_LEN     (8)

#define DW1000_SR_RX_LSFD_MAG_OFFSET  0x19
#define DW1000_SR_RX_LSFD_MAG_LEN     (8)

#define DW1000_SR_RX_LSFD_SGN_OFFSET  0x21
#define DW1000_SR_RX_LSFD_SGN_LEN     (8)

/** @} */
/** @} */
/******************************************************************************/

/* 寄存器文件 保留 - 0x22 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_AGC_CTRL
 * @brief 寄存器文件 AGC_CTRL - 0x23 - [读写]
 *
 * AGC配置与控制
 *
 * @{
 */

#define DW1000_REG_AGC_CTRL           0x23
#define DW1000_REG_AGC_CTRL_LEN       (33)

/**
 * @defgroup DW1000_REG_AGC_CTRL_SR
 * @ingroup DW1000_REG_AGC_CTRL
 *
 * @{
 */

/* 偏移地址 0x00 保留 */

#define DW1000_SR_AGC_CTRL1_OFFSET    0x02
#define DW1000_SR_AGC_CTRL1_LEN       (2)

#define DW1000_SR_AGC_TUNE1_OFFSET    0x04
#define DW1000_SR_AGC_TUNE1_LEN       (2)

/* 偏移地址 0x06 保留 */

#define DW1000_SR_AGC_TUNE2_OFFSET    0x0C
#define DW1000_SR_AGC_TUNE2_LEN       (4)

/* 偏移地址 0x10 保留 */

#define DW1000_SR_AGC_TUNE3_OFFSET    0x10
#define DW1000_SR_AGC_TUNE3_LEN       (2)

/* 偏移地址 0x14 保留 */

#define DW1000_SR_AGC_STAT1_OFFSET    0x1E
#define DW1000_SR_AGC_STAT1_LEN       (3)

/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_EXT_SYNC
 * @brief 寄存器文件 EXT_SYNC - 0x24 - [读写]
 *
 * 外部同步控制
 *
 * @{
 */

#define DW1000_REG_EXT_SYNC           0x24
#define DW1000_REG_EXT_SYNC_LEN       (12)

/**
 * @defgroup DW1000_REG_EXT_SYNC_SR
 * @ingroup DW1000_REG_EXT_SYNC
 *
 * @{
 */

#define DW1000_SR_EC_CTRL_OFFSET      0x00
#define DW1000_SR_EC_CTRL_LEN         (4)

#define DW1000_SR_EC_RXTC_OFFSET      0x04
#define DW1000_SR_EC_RXTC_LEN         (4)

#define DW1000_SR_EC_GOLP_OFFSET      0x08
#define DW1000_SR_EC_GOLP_LEN         (4)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_ACC_MEM
 * @brief 寄存器文件 ACC_MEM - 0x25 - [只读]
 *
 * CIR累加器内存
 *
 * @{
 */

#define DW1000_REG_ACC_MEM            0x25
#define DW1000_REG_ACC_MEM_LEN        (4064)

/**
 * @defgroup DW1000_REG_ACC_MEM_SR
 * @ingroup DW1000_REG_ACC_MEM
 *
 * @{
 */

#define DW1000_SR_CIRx_REAL_OFFSET(x) (0x00 + ((x) * 4)) /* x 取值 0-1015 */
#define DW1000_SR_CIRx_IMAG_OFFSET(x) (0x02 + ((x) * 4)) /* x 取值 0-1015 */
#define DW1000_SR_CIRx_REAL_LEN       (2)
#define DW1000_SR_CIRx_IMAG_LEN       (2)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_GPIO_CTRL
 * @brief 寄存器文件 GPIO_CTRL - 0x26 - [读写]
 *
 * GPIO控制和状态
 *
 * @{
 */

#define DW1000_REG_GPIO_CTRL          0x26
#define DW1000_REG_GPIO_CTRL_LEN      (44)

/**
 * @defgroup DW1000_REG_GPIO_CTRL_SR
 * @ingroup DW1000_REG_GPIO_CTRL
 *
 * @{
 */

#define DW1000_SR_GPIO_MODE_OFFSET    0x00
#define DW1000_SR_GPIO_MODE_LEN       (4)

/* 偏移地址 0x04 保留 */

#define DW1000_SR_GPIO_DIR_OFFSET     0x08
#define DW1000_SR_GPIO_DIR_LEN        (4)

#define DW1000_SR_GPIO_DOUT_OFFSET    0x0C
#define DW1000_SR_GPIO_DOUT_LEN       (4)

#define DW1000_SR_GPIO_IRQE_OFFSET    0x10
#define DW1000_SR_GPIO_IRQE_LEN       (4)

#define DW1000_SR_GPIO_ISEN_OFFSET    0x14
#define DW1000_SR_GPIO_ISEN_LEN       (4)

#define DW1000_SR_GPIO_IMODE_OFFSET   0x18
#define DW1000_SR_GPIO_IMODE_LEN      (4)

#define DW1000_SR_GPIO_IBES_OFFSET    0x1C
#define DW1000_SR_GPIO_IBES_LEN       (4)

#define DW1000_SR_GPIO_ICLR_OFFSET    0x20
#define DW1000_SR_GPIO_ICLR_LEN       (4)

#define DW1000_SR_GPIO_IDBE_OFFSET    0x24
#define DW1000_SR_GPIO_IDBE_LEN       (4)

#define DW1000_SR_GPIO_RAW_OFFSET     0x28
#define DW1000_SR_GPIO_RAW_LEN        (4)


/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_DRX_CONF
 * @brief 寄存器文件 DRX_CONF - 0x27 - [不定]
 *
 * 数字接收器配置
 *
 * @{
 */

#define DW1000_REG_DRX_CONF           0x27
#define DW1000_REG_DRX_CONF_LEN       (46)

/**
 * @defgroup DW1000_REG_DRX_CONF_SR
 * @ingroup DW1000_REG_DRX_CONF
 *
 * @{
 */

/* 偏移地址 0x00 保留 */

#define DW1000_SR_DRX_TUNE0B_OFFSET   0x02
#define DW1000_SR_DRX_TUNE0B_LEN      (2)

#define DW1000_SR_DRX_TUNE1A_OFFSET   0x04
#define DW1000_SR_DRX_TUNE1A_LEN      (2)

#define DW1000_SR_DRX_TUNE1B_OFFSET   0x06
#define DW1000_SR_DRX_TUNE1B_LEN      (2)

#define DW1000_SR_DRX_TUNE2_OFFSET    0x08
#define DW1000_SR_DRX_TUNE2_LEN       (4)

/* 偏移地址 0x0C 保留 */

#define DW1000_SR_DRX_SFDTOC_OFFSET   0x20
#define DW1000_SR_DRX_SFDTOC_LEN      (2)

#define DW1000_SR_DRX_PRETOC_OFFSET   0x24
#define DW1000_SR_DRX_PRETOC_LEN      (2)

#define DW1000_SR_DRX_TUNE4H_OFFSET   0x26
#define DW1000_SR_DRX_TUNE4H_LEN      (2)

#define DW1000_SR_DRX_CAR_INT_OFFSET  0x28
#define DW1000_SR_DRX_CAR_INT_LEN     (3)

#define DW1000_SR_RXPACC_NOSAT_OFFSET 0x2C
#define DW1000_SR_RXPACC_NOSAT_LEN    (2)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_RF_CONF
 * @brief 寄存器文件 RF_CONF - 0x28 - [不定]
 *
 * 模拟射频配置块
 *
 * @{
 */

#define DW1000_REG_RF_CONF            0x28
#define DW1000_REG_RF_CONF_LEN        (58)

/**
 * @defgroup DW1000_REG_RF_CONF_SR
 * @ingroup DW1000_REG_RF_CONF
 *
 * @{
 */

#define DW1000_SR_RF_CONF_OFFSET      0x00
#define DW1000_SR_RF_CONF_LEN         (4)

/* 偏移地址 0x04 保留 */

#define DW1000_SR_RF_RXCTRLH_OFFSET   0x0B
#define DW1000_SR_RF_RXCTRLH_LEN      (1)

#define DW1000_SR_RF_TXCTRL_OFFSET    0x0C
#define DW1000_SR_RF_TXCTRL_LEN       (3)

/* 偏移地址 0x10 保留 */

#define DW1000_SR_RF_STATUS_OFFSET    0x2C
#define DW1000_SR_RF_STATUS_LEN       (4)

#define DW1000_SR_LDOTUNE_OFFSET      0x30
#define DW1000_SR_LDOTUNE_LEN         (5)

/** @} */
/** @} */
/******************************************************************************/

/* 寄存器文件 保留 - 0x29 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_TX_CAL
 * @brief 寄存器文件 TX_CAL - 0x2A - [读写]
 *
 * 发送器校准模块
 *
 * @{
 */

#define DW1000_REG_TX_CAL             0x2A
#define DW1000_REG_TX_CAL_LEN         (1)

/**
 * @defgroup DW1000_REG_TX_CAL_SR
 * @ingroup DW1000_REG_TX_CAL
 *
 * @{
 */

#define DW1000_SR_TC_SARC_OFFSET      0x00
#define DW1000_SR_TC_SARC_LEN         (2)

/* 偏移地址 0x02 保留 */

#define DW1000_SR_TC_SARL_OFFSET      0x03
#define DW1000_SR_TC_SARL_LEN         (3)

#define DW1000_SR_SAR_LVBAT_OFFSET    0x03
#define DW1000_SR_SAR_LVBAT_LEN       (1)

#define DW1000_SR_SAR_LTEMP_OFFSET    0x04
#define DW1000_SR_SAR_LTEMP_LEN       (1)

#define DW1000_SR_TC_SARW_OFFSET      0x06
#define DW1000_SR_TC_SARW_LEN         (2)

#define DW1000_SR_TC_PG_CTRL_OFFSET   0x08
#define DW1000_SR_TC_PG_CTRL_LEN      (1)

#define DW1000_SR_TC_PG_STATUS_OFFSET 0x09
#define DW1000_SR_TC_PG_STATUS_LEN    (2)

#define DW1000_SR_TC_PGDELAY_OFFSET   0x0B
#define DW1000_SR_TC_PGDELAY_LEN      (1)

#define DW1000_SR_TC_PGTEST_OFFSET    0x0C
#define DW1000_SR_TC_PGTEST_LEN       (1)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_FS_CTRL
 * @brief 寄存器文件 FS_CTRL - 0x2B - [不定]
 *
 * 频率合成器控制模块
 *
 * @{
 */

#define DW1000_REG_FS_CTRL            0x2B
#define DW1000_REG_FS_CTRL_LEN        (21)

/**
 * @defgroup DW1000_REG_FS_CTRL_SR
 * @ingroup DW1000_REG_FS_CTRL
 *
 * @{
 */

/* 偏移地址 0x00 保留 */

#define DW1000_SR_FS_PLLCFG_OFFSET    0x07
#define DW1000_SR_FS_PLLCFG_LEN       (4)

#define DW1000_SR_FS_PLLTUNE_OFFSET   0x0B
#define DW1000_SR_FS_PLLTUNE_LEN      (1)

/* 偏移地址 0x0C 保留 */

#define DW1000_SR_FS_XTALT_OFFSET     0x0E
#define DW1000_SR_FS_XTALT_LEN        (1)

/* 偏移地址 0x0F 保留 */

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_AON
 * @brief 寄存器文件 AON - 0x2C - [不定]
 *
 * 常开系统控制接口
 *
 * @{
 */

#define DW1000_REG_AON                0x2C
#define DW1000_REG_AON_LEN            (12)

/**
 * @defgroup DW1000_REG_AON_SR
 * @ingroup DW1000_REG_AON
 *
 * @{
 */

#define DW1000_SR_AON_WCFG_OFFSET     0x00
#define DW1000_SR_AON_WCFG_LEN        (2)

#define DW1000_SR_AON_CTRL_OFFSET     0x02
#define DW1000_SR_AON_CTRL_LEN        (1)

#define DW1000_SR_AON_RDAT_OFFSET     0x03
#define DW1000_SR_AON_RDAT_LEN        (1)

#define DW1000_SR_AON_ADDR_OFFSET     0x04
#define DW1000_SR_AON_ADDR_LEN        (1)

/* 偏移地址 0x05 保留 */

#define DW1000_SR_AON_CFG0_OFFSET     0x06
#define DW1000_SR_AON_CFG0_LEN        (4)

/* 这里为了方便，将4字节的子寄存器拆出一个2字节的子寄存器用于单独设置 */
#define DW1000_SR_SLEEP_TIM_OFFSET    0x08
#define DW1000_SR_SLEEP_TIM_LEN       (2)

#define DW1000_SR_AON_CFG1_OFFSET     0x0A
#define DW1000_SR_AON_CFG1_LEN        (2)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_OTP_IF
 * @brief 寄存器文件 OTP_IF - 0x2D - [不定]
 *
 * OTP内存接口
 *
 * @{
 */

#define DW1000_REG_OTP_IF             0x2D
#define DW1000_REG_OTP_IF_LEN         (18)

/**
 * @defgroup DW1000_REG_OTP_IF_SR
 * @ingroup DW1000_REG_OTP_IF
 *
 * @{
 */

#define DW1000_SR_OTP_WDAT_OFFSET     0x00
#define DW1000_SR_OTP_WDAT_LEN        (4)

#define DW1000_SR_OTP_ADDR_OFFSET     0x04
#define DW1000_SR_OTP_ADDR_LEN        (2)

#define DW1000_SR_OTP_CTRL_OFFSET     0x06
#define DW1000_SR_OTP_CTRL_LEN        (2)

#define DW1000_SR_OTP_STAT_OFFSET     0x08
#define DW1000_SR_OTP_STAT_LEN        (2)

#define DW1000_SR_OTP_RDAT_OFFSET     0x0A
#define DW1000_SR_OTP_RDAT_LEN        (4)

#define DW1000_SR_OTP_SRDAT_OFFSET    0x0E
#define DW1000_SR_OTP_SRDAT_LEN       (4)

#define DW1000_SR_OTP_SF_OFFSET       0x12
#define DW1000_SR_OTP_SF_LEN          (1)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_LDE_IF
 * @brief 寄存器文件 LDE_IF - 0x2E - [不定]
 *
 * 上升沿检测接口
 *
 * @{
 */

#define DW1000_REG_LDE_IF             0x2E
#define DW1000_REG_LDE_IF_LEN         (10246)

/**
 * @defgroup
 * @ingroup
 *
 * @{
 */

#define DW1000_SR_LDE_THRESH_OFFSET   0x0000
#define DW1000_SR_LDE_THRESH_LEN      (2)

#define DW1000_SR_LDE_CFG1_OFFSET     0x0806
#define DW1000_SR_LDE_CFG1_LEN        (1)

#define DW1000_SR_LDE_PPINDX_OFFSET   0x1000
#define DW1000_SR_LDE_PPINDX_LEN      (2)

#define DW1000_SR_LDE_PPAMPL_OFFSET   0x1002
#define DW1000_SR_LDE_PPAMPL_LEN      (2)

#define DW1000_SR_LDE_RXANTD_OFFSET   0x1804
#define DW1000_SR_LDE_RXANTD_LEN      (2)

#define DW1000_SR_LDE_CFG2_OFFSET     0x1806
#define DW1000_SR_LDE_CFG2_LEN        (2)

#define DW1000_SR_LDE_REPC_OFFSET     0x2804
#define DW1000_SR_LDE_REPC_LEN        (2)

/** @} */
/** @} */
/******************************************************************************/
/**
 * @defgroup DW1000_REG_DIG_DIAG
 * @brief 寄存器文件 DIG_DIAG - 0x2F - [不定]
 *
 * 数字诊断接口
 *
 * @{
 */

#define DW1000_REG_DIG_DIAG           0x2F
#define DW1000_REG_DIG_DIAG_LEN       (41)

/**
 * @defgroup DW1000_REG_DIG_DIAG_SR
 * @ingroup DW1000_REG_DIG_DIAG
 *
 * @{
 */

#define DW1000_SR_EVC_CTRL_OFFSET     0x00
#define DW1000_SR_EVC_CTRL_LEN        (4)

#define DW1000_SR_EVC_PHE_OFFSET      0x04
#define DW1000_SR_EVC_PHE_LEN         (2)

#define DW1000_SR_EVC_RSE_OFFSET      0x06
#define DW1000_SR_EVC_RSE_LEN         (2)

#define DW1000_SR_EVC_FCG_OFFSET      0x08
#define DW1000_SR_EVC_FCG_LEN         (2)

#define DW1000_SR_EVC_FCE_OFFSET      0x0A
#define DW1000_SR_EVC_FCE_LEN         (2)

#define DW1000_SR_EVC_FFR_OFFSET      0x0C
#define DW1000_SR_EVC_FFR_LEN         (2)

#define DW1000_SR_EVC_OVR_OFFSET      0x0E
#define DW1000_SR_EVC_OVR_LEN         (2)

#define DW1000_SR_EVC_STO_OFFSET      0x10
#define DW1000_SR_EVC_STO_LEN         (2)

#define DW1000_SR_EVC_PTO_OFFSET      0x12
#define DW1000_SR_EVC_PTO_LEN         (2)

#define DW1000_SR_EVC_FWTO_OFFSET     0x14
#define DW1000_SR_EVC_FWTO_LEN        (2)

#define DW1000_SR_EVC_TXFS_OFFSET     0x16
#define DW1000_SR_EVC_TXFS_LEN        (2)

#define DW1000_SR_EVC_HPW_OFFSET      0x18
#define DW1000_SR_EVC_HPW_LEN         (2)

#define DW1000_SR_EVC_TPW_OFFSET      0x1A
#define DW1000_SR_EVC_TPW_LEN         (2)

/* 偏移地址 0x1C 保留 */

#define DW1000_SR_DIAG_TMC_OFFSET     0x24
#define DW1000_SR_DIAG_TMC_LEN        (2)

/** @} */
/** @} */
/******************************************************************************/

/* 寄存器文件 保留 - 0x30 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_CRTR
 * @brief 寄存器文件 CRTR - 0x31 - 【名字未知，用户手册未更新】
 *
 * @{
 */

#define DW1000_REG_CRTR               0x31

/**
 * @defgroup
 * @ingroup
 *
 * @{
 */

#define DW1000_SR_CRTR_GEAR_OFFSET    0x28

/** @} */
/** @} */
/******************************************************************************/

/* 寄存器文件 保留 - 0x32 到 0x35 */

/******************************************************************************/
/**
 * @defgroup DW1000_REG_PMSC
 * @brief 寄存器文件 PMSC - 0x36 - [不定]
 *
 * 电源管理系统控制块
 *
 * @{
 */

#define DW1000_REG_PMSC               0x36
#define DW1000_REG_PMSC_LEN           (44)

/**
 * @defgroup
 * @ingroup
 *
 * @{
 */

#define DW1000_SR_PMSC_CTRL0_OFFSET   0x00
#define DW1000_SR_PMSC_CTRL0_LEN      (4)

#define DW1000_SR_PMSC_CTRL0_3_OFFSET 0x03

#define DW1000_SR_PMSC_CTRL1_OFFSET   0x04
#define DW1000_SR_PMSC_CTRL1_LEN      (4)

#define DW1000_SR_PMSC_CTRL1_2_OFFSET 0x02

/* 偏移地址 0x08 保留 */

#define DW1000_SR_PMSC_SNOZT_OFFSET   0x0C
#define DW1000_SR_PMSC_SNOZT_LEN      (1)

/* 偏移地址 0x10 保留 */

#define DW1000_SR_PMSC_TXFSEQ_OFFSET  0x26
#define DW1000_SR_PMSC_TXFSEQ_LEN     (2)

#define DW1000_SR_PMSC_LEDC_OFFSET    0x28
#define DW1000_SR_PMSC_LEDC_LEN       (4)

/** @} */
/** @} */
/******************************************************************************/

/* 寄存器文件 保留 - 0x37到0x3F */

/******************************************************************************/
