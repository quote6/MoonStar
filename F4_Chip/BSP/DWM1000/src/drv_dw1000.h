/**
 * @file driver_dw1000.h
 * @author quote6 (2293127401@qq.com)
 * @date 2025-11-24
 * @brief
 *
 * @copyright Copyright (c) 2025 quote6
 * For study and research only, no reprinting
 *
 * @par 修改日志
 * <table>
 * <tr><th>Date        <th>Version      <th>Author    <th>Description
 * <tr><td>2025-11-24      <td>{version}   <td>quote6   <td>{Description}
 * </table>
 * ***************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_DW1000_H
#define __DRV_DW1000_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "drv_dw1000_reg_params.h"
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define USE_DW1000_REGISTER_CALLBACKS 0U
/* Exported types ------------------------------------------------------------*/
/** @defgroup DW1000_Exported_Types DW1000 Exported Types
 * @{
 */
/* 定义DW1000寄存器 TC_PGDELAY - 发送校准 脉冲生成器延迟 */
typedef uint8_t DW1000_SREG_TC_PGDELAY_t;

/* 定义DW1000寄存器 TX POWER - 发送功率控制 */
typedef union {
    uint32_t all;
    struct {
        uint8_t BOOSTNorm;
        uint8_t BOOSTP500;
        uint8_t BOOSTP250;
        uint8_t BOOSTP125;
    } bits;
} DW1000_REG_TX_POWER_ut;


/* 定义DW1000寄存器 RX_TIME - 接收时间戳 */
typedef struct {
    uint8_t RX_STAMP[5]; // 接收时间戳
    uint8_t FP_INDEX[2]; // 第一路径索引
    uint8_t FP_AMPL1[2]; // 第一路径振幅点1
    uint8_t RX_RAWST[5]; // 原始接收时间戳
} DW1000_REG_RX_TIME_t;

/* 定义DW1000寄存器 LDE_THRESH - LDE 阈值报告 */
typedef uint16_t DW1000_REG_LDE_THRESH_t;

/* 定义DW1000寄存器 RX_FQUAL - 接收帧质量信息 */
typedef struct {
    uint16_t STD_NOISE; // 噪声标准差
    uint16_t FP_AMPL2;  // 第一路径振幅点2
    uint16_t FP_AMPL3;  // 第一路径振幅点3
    uint16_t CIR_PWR;   // Channel Impulse Response Power - 信道脉冲响应功率
} DW1000_REG_RX_FQUAL_t;

typedef union {
    uint32_t all;
    struct {
        uint32_t RXFLEN : 7;  // 接收帧长度
        uint32_t RXFLE : 3;   // 接收帧长度扩展
        uint32_t : 1;         // 保留
        uint32_t RXNSPL : 2;  // 接收非标前导码长度
        uint32_t RXBR : 2;    // 接收bit速率（数据速率）
        uint32_t RNG : 1;     // 接收器测距
        uint32_t RXPRFR : 2;  // 接收脉冲重复速率
        uint32_t RXPSR : 2;   // 接收前导码长度
        uint32_t RXPACC : 12; // 前导码累积计数
    } bits;
} DW1000_REG_RX_FINFO_ut;


typedef struct {
    // uint32_t EVC_EN : 1;
    // uint32_t EVC_CLR : 1;
    // uint32_t : 30;
    uint16_t EVC_PHE : 12;  // PHY帧头错误事件计数
    uint16_t : 4;           // 保留
    uint16_t EVC_RSE : 12;  // RSD错误事件计数
    uint16_t : 4;           // 保留
    uint16_t EVC_FCG : 12;  // 帧检查序列良好事件计数
    uint16_t : 4;           // 保留
    uint16_t EVC_FCE : 12;  // FCS错误事件计数
    uint16_t : 4;           // 保留
    uint16_t EVC_FFR : 12;  // 帧过滤拒绝计数
    uint16_t : 4;           // 保留
    uint16_t EVC_OVR : 12;  // 接收溢出错误事件计数
    uint16_t : 4;           // 保留
    uint16_t EVC_STO : 12;  // SFD超时错误计数
    uint16_t : 4;           // 保留
    uint16_t EVC_PTO : 12;  // 前导码超时事件计数
    uint16_t : 4;           // 保留
    uint16_t EVC_FWTO : 12; // 接收帧等待超时计数
    uint16_t : 4;           // 保留
    uint16_t EVC_TXFS : 12; // 发送帧事件计数
    uint16_t : 4;           // 保留
    uint16_t EVC_HPW : 12;  // 半周期警告计数
    uint16_t : 4;           // 保留
    uint16_t EVC_TPW : 12;  // 发送器上电警告计数
    uint16_t : 4;           // 保留
} DW1000_REG_DIG_DIAG_t;

typedef union {
    uint16_t all;
    struct {
        uint16_t ONW_RADC : 1;
        uint16_t ONW_RX : 1;
        uint16_t : 1;
        uint16_t ONW_LEUI : 1;
        uint16_t : 2;
        uint16_t ONW_LDC : 1;
        uint16_t ONW_L64P : 1;
        uint16_t PRES_SLEEP : 1;
        uint16_t : 2;
        uint16_t ONW_LLDE : 1;
        uint16_t ONW_LLDO : 1;
        uint16_t : 3
    } bits;
} DW1000_SREG_AON_WCFG_ut;

typedef union {
    uint32_t all;
    struct {
        uint32_t FFEN : 1;
        uint32_t FFBC : 1;
        uint32_t FFAB : 1;
        uint32_t FFAD : 1;
        uint32_t FFAA : 1;
        uint32_t FFAM : 1;
        uint32_t FFAR : 1;
        uint32_t FFA4 : 1;
        uint32_t FFA5 : 1;
        uint32_t HIRQ_POL : 1;
        uint32_t SPI_EDGE : 1;
        uint32_t DIS_FCE : 1;
        uint32_t DIS_SRXB : 1;
        uint32_t DIS_PHE : 1;
        uint32_t DIS_RSED : 1;
        uint32_t FCS_INIT2F : 1;
        uint32_t PHR_MODE : 2;
        uint32_t DIS_STXP : 1;
        uint32_t : 3;
        uint32_t RXM110K : 1;
        uint32_t : 5;
        uint32_t RXWTOE : 1;
        uint32_t RXAUTR : 1;
        uint32_t AUTOACK : 1;
        uint32_t AACKPEND : 1;
    } bits;
} DW1000_REG_SYS_CFG_ut;

/**
 * @brief DW1000 寄存器TX_FCTRL - 发送帧控制定义
 * @note 详细请参考 《DW1000 User Manual V2.18》的
 *        7.2.10 寄存器 0x08 - 发送帧控制
 */
typedef union {
    // uint8_t all[5]; // 【因为有用不到的字段，变成32位】
    uint32_t all;
    struct {
        uint16_t TFLEN : 7;    // 传输帧长度（Transmit Frame Length）
        uint16_t TFLE : 3;     // 传输帧长度扩展（Transmit Frame Length Extension）
        uint16_t : 3;          // 保留
        uint16_t TXBR : 2;     // 发送Bit速率（Transmit Bit Rate）
        uint16_t TR : 1;       // 发送测距使能（Transmit Ranging enable）
        uint16_t TXPRF : 2;    // 发送脉冲重复频率（Transmit Pulse Repetition Frequency）
        uint16_t TXPSR : 2;    // 发送前导码符号重复（Transmit Preamble Symbol Repetition）（这设置了以符号为单位的传输前导序列的长度）
        uint16_t PE : 2;       // 前导码扩展（Preamble Extension）
        uint16_t TXBOFFS : 10; // 发送缓冲区索引偏移（Transmit buffer index offset）
        // uint8_t IFSDELAY;      // 帧间间距（Inter-Frame Spacing）【用不到的字段，暂时注释】
    } bits;
} DW1000_REG_TX_FCTRL_ut;

typedef struct {
    // uint8_t EUID[8]; // 因为未使用，暂时注释
    // uint8_t EUID_Alt[8]; // 因为未使用，暂时注释
    uint32_t partID; // IC Part ID（在初始化时可配置从OTP中读取并保存在此）
    uint32_t lotID;  // IC Lost ID（在初始化时可配置从OTP中读取并保存在此）
    // uint8_t LDOTUNE_CAL[5]; // 因为未使用，暂时注释
    // uint8_t XTAL_Trim; // 因为未使用，暂时注释
    uint8_t refVolt;      // 参考电压（Vmeas @ 3.3V）
    uint8_t refTemp;      // 参考温度（Tmeas @ 23℃）
    uint8_t OTP_Revision; // OTP修订号（在初始化时可配置从OTP中读取并保存在此）
} DW1000_OTP_MEM_t;


typedef struct DW1000_Backup_s {
    uint32_t partID;
    uint32_t lotID;
    uint8_t otpRevision;
    uint8_t refVolt;
    uint8_t refTemp;
    uint32_t txFctrl;
    uint32_t systemConfig;
    uint32_t systemStatus;
    uint16_t sleepMode;
} DW1000_Backup_t;

// typedef struct DW1000_TX_Config_s {
//     uint8_t pulGenDelay; // 脉冲发生器延迟
//     uint32_t txPower;    // 发送功率
// } DW1000_TX_Config_t;

// typedef struct DW1000_RX_DIAG_s {
//     uint16_t maxNoise;
//     uint16_t firstPathAmplitude1;
//     uint16_t standardDeviationNoise;
//     uint16_t firstPathAmplitude2;
//     uint16_t firstPathAmplitude3;
//     uint16_t CIR_maxGrowth; /* Channel Impulse Response max growth */
//     uint16_t RX_preambleCount;
//     uint16_t firstPathIndex;
// } DW1000_RX_DIAG_t;

// typedef struct DW1000_EVT_CNT_s {
//     uint16_t phrError;
//     uint16_t rsdError;
//     uint16_t frameCheckSeqGood;
//     uint16_t frameCheckSeqError;
//     uint16_t frameFilterReject;
//     uint16_t rxOverrunError;
//     uint16_t sfdTimeout;
//     uint16_t preambleTimeout;
//     uint16_t rxFrameWaitTimeout;
//     uint16_t txFrameSent;
//     uint16_t halfPeriodWarning;
//     uint16_t txPowerupWarning;
// } DW1000_EVT_CNT_t;

typedef struct DW1000_CB_Data_t {
    // uint32_t status;
    uint16_t frameLen;
    uint16_t frameCtrl;
    uint8_t isRxRangingFrame : 1; // 是否接收到测距帧
    uint8_t reserced : 7;
} DW1000_CB_Data_t;


typedef enum {
    DW1000_UNLOCKED = 0x00U,
    DW1000_LOCKED = 0x01U
} DW1000_LockTypeDef;


typedef struct {
    DW1000_OTP_MEM_t OTP;               // 11字节
    DW1000_REG_DIG_DIAG_t DIG_DIAG;     // 24字节
    DW1000_REG_RX_FQUAL_t RX_FQUAL;     // 8字节
    DW1000_REG_TX_POWER_ut TX_POWER;    // 4字节
    DW1000_REG_RX_FINFO_ut RX_FINFO;    // 4字节
    DW1000_REG_SYS_CFG_ut SYS_CFG;      // 4字节
    DW1000_REG_LDE_THRESH_t LDE_THRESH; // 2字节
    DW1000_SREG_AON_WCFG_ut AON_WCFG;   // 2字节
    DW1000_SREG_TC_PGDELAY_t TC_PGDELY; // 1字节
    DW1000_REG_RX_TIME_t RX_TIME;       // 18字节
    DW1000_REG_TX_FCTRL_ut TX_FCTRL;    // 5字节
} DW1000_TypeDef;

typedef struct
{
    /* data */
    uint8_t isRxOnAfterTx : 1;                 // 在发送完成后是否自动开启接收（通常用于接收应答信息）
    uint8_t shouldOtpPartIdRead : 1;           // 是否从OTP中加载partID
    uint8_t shouldOtpLotIdRead : 1;            // 是否从OTP中加载LOT ID
    uint8_t shouldOtpRefVoltRead : 1;          // 是否从OTP中加载参考电压值
    uint8_t shouldOtpRefTempRead : 1;          // 是否从OTP中加载参考温度值
    uint8_t shouldOtpUcodeRead : 1;            // 是否从OTP中加载微码
    uint8_t shouldOtpUcodeReadAfterWakeUp : 1; // 是否在唤醒后从OTP中加载微码
    uint8_t shouldOtpRevReadAfterWakeUp : 1;   // 是否在唤醒后从OTP中加载OTP修订号
    uint8_t useSfdNstd : 1;                    // 是否使用非标 SFD 以获得更好的性能
    uint8_t useDoubleBuff : 1;                 // 是否为双缓冲模式
    uint8_t useLowPowListen : 1;               // 是否使用低功耗监听
    uint8_t channel : 3;                       // 频道号: DW1000_CHANNEL_x（x取值1-7，不包含6）
    uint8_t dataRate : 2;                      // 数据速率: 110kbps 850kbps 6.8Mbps
    uint8_t prf : 2;                           // 脉冲重复频率: 16MHz 和 64MHz
    uint8_t phrType : 1;                       // PHR模式： StdFrameMode 和 LongFrameMode
    uint8_t reserved : 5;                      // 保留位
    uint8_t txPreambleLength;                  // 发送前导码长度: 64 128 256 512 1024 2048 4096
    uint8_t rxPac;                             // 前导码查询块大小（与接收前导码长度相关）
    uint8_t sleepMode;                         // 睡眠模式
    uint8_t txPreambleCode;                    // 发送前导码
    uint8_t rxPreambleCode;                    // 接收前导码
    uint16_t sfdTimeout;                       // SFD超时值（单位 符号）
} DW1000_InitTypeDef;

/**
 * @brief  DW1000 State structure definition
 */
typedef enum {
    DW1000_STATE_RESET = 0x00U,   /*!< Peripheral not Initialized                         */
    DW1000_STATE_READY = 0x01U,   /*!< Peripheral Initialized and ready for use           */
    DW1000_STATE_BUSY = 0x02U,    /*!< an internal process is ongoing                     */
    DW1000_STATE_BUSY_TX = 0x03U, /*!< Data Transmission process is ongoing               */
    DW1000_STATE_BUSY_RX = 0x04U, /*!< Data Reception process is ongoing                  */
    DW1000_STATE_ERROR = 0x05U,   /*!< SPI error state                                    */
    DW1000_STATE_ABORT = 0x06U    /*!< SPI abort is ongoing                               */
} DW1000_StateTypeDef;

typedef struct {
    // uint8_t (*init)(void);
    // uint8_t (*deinit)(void);
    uint8_t (*read)(uint8_t data);
    uint8_t (*write)(uint8_t data);
} DW1000_IF_GPIO_TypeDef;

typedef struct {
    // uint8_t (*init)(void);
    // uint8_t (*deinit)(void);
    // uint8_t (*speedSet)(uint8_t speed);
    uint8_t (*highSpeedSet)(void);
    uint8_t (*lowSpeedSet)(void);
    uint8_t (*read)(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen);
    uint8_t (*write)(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen);
} DW1000_IF_SPI_TypeDef;

typedef struct DW1000_IRQ_s {
    uint8_t (*enableStatusGet)(void); // IRQ 使能状态获取函数 返回值0表示未使能，否则为使能
    void (*enable)(void);
    void (*disable)(void);
    uint8_t enableStatus; // 对状态进行暂存，用于退出临界区时实现恢复功能
} DW1000_IF_IQR_TypeDef;


/**
 * @brief DW1000 handle 结构体定义
 */
typedef struct DW1000_Handle_s {
    // DW1000_LockTypeDef lock;
    // DW1000_StateTypeDef state;
    // DW1000_Backup_t backup;
    DW1000_TypeDef instance;        // 83字节
    DW1000_InitTypeDef init;        // 10个字节
    DW1000_IF_GPIO_TypeDef wakeup;  // 8个字节
    DW1000_IF_GPIO_TypeDef reset;   // 8个字节
    DW1000_IF_SPI_TypeDef spi;      // 16个字节
    DW1000_IF_IQR_TypeDef host_irq; // 13个字节
    DW1000_CB_Data_t cbData;        // 5个字节
    volatile uint32_t errorCode;    // 错误码，用于在中断函数里记录发生的错误，当退出中断时由外部进行处理 4个字节
    void (*delay_ms)(uint32_t ms);
    void (*debugPrint)(const char* fmt, ...);
#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
    void (*TxCompleteCallback)(DW1000_Handle_t* handle);
    void (*RxCompleteCallback)(DW1000_Handle_t* handle);
    void (*RxErrorCallback)(DW1000_Handle_t* handle);
    void (*RxTimeoutCallback)(DW1000_Handle_t* handle);
#endif /* USE_DW1000_REGISTER_CALLBACKS */
} DW1000_Handle_t;

#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
typedef enum {
    DW1000_TX_COMPLETE_CB_ID = 0x00U, /*!< DW1000 Tx Completed callback ID         */
    DW1000_RX_COMPLETE_CB_ID = 0x01U, /*!< DW1000 Rx Completed callback ID         */
    DW1000_RX_ERROR_CB_ID = 0x02U,    /*!< DW1000 Rx Error callback ID             */
    DW1000_RX_TIMEOUT_CB_ID = 0x03U   /*!< DW1000 Rx Timeout callback ID           */
} DW1000_CallbackIdTypeDef;

typedef void (*DW1000_CallbackTypeDef)(DW1000_Handle_t* hdw1000);

#endif /* USE_DW1000_REGISTER_CALLBACKS */


/**
 * @brief DW1000 信息 结构体定义
 */
typedef struct DW1000_Info_s {
    char chipName[32];         /**< chip name */
    char manufacturerName[32]; /**< manufacturer name */
    char interface[8];         /**< chip interface name */
    float supplyVoltage_min_v; /**< chip min supply voltage */
    float supplyVoltage_max_v; /**< chip max supply voltage */
    float current_max_ma;      /**< chip max current */
    float temperature_min;     /**< chip min operating temperature */
    float temperature_max;     /**< chip max operating temperature */
    uint32_t driverVersion;    /**< driver version */
} DW1000_Info_t;
/* Exported constants --------------------------------------------------------*/
/** @defgroup DW1000_Error_Code DW1000 Error Code
 * @{
 */
#define DW1000_ERR_CODE_NONE         (0x00000000U) /* 无错误 */
#define DW1000_ERR_CODE_READ         (0x1U << 1)   /* 读取失败（通过SPI读取） */
#define DW1000_ERR_CODE_WRITE        (0x1U << 2)   /* 写入失败（通过SPI写入） */
#define DW1000_ERR_CODE_TRX_OFF      (0x1U << 3)   /* 收发器关闭失败 */
#define DW1000_ERR_CODE_RX_RESET     (0x1U << 4)   /* 接收器复位失败 */
#define DW1000_ERR_CODE_RX_OFF       (0x1U << 5)   /* 接收器关闭失败 */

/**
 * @}
 */

/* DW1000支持的频道数 */
#define DW1000_SUPPORT_CHANNEL_NUM   6
/* DW1000支持的数据速率数量 */
#define DW1000_SUPPORT_DATA_RATE_NUM 3
/* DW1000支持的SFD类型数量 */
#define DW1000_SUPPORT_SFD_TYPE_NUM  2
/* DW1000支持的PRF数量 */
#define DW1000_SUPPORT_PRF_NUM       2
/* DW1000支持的PAC数量 */
#define DW1000_SUPPORT_PAC_NUM       4
/* 支持的前导码数量（包含无前导码情况） */
#define DW1000_SUPPORT_PCODE_NUM     25

/* 索引从0开始是为了方便与频道相关的数组取值 */
#define DW1000_CHANNEL_1             0
#define DW1000_CHANNEL_2             1
#define DW1000_CHANNEL_3             2
#define DW1000_CHANNEL_4             3
#define DW1000_CHANNEL_5             4 /* 频道5 */
#define DW1000_CHANNEL_6             5 /* 频道6不支持 */
#define DW1000_CHANNEL_7             6 /* 频道7 */


#define DW1000_DATA_RATE_110K        0
#define DW1000_DATA_RATE_850K        1
#define DW1000_DATA_RATE_6M8         2

#define DW1000_PAC8                  0
#define DW1000_PAC16                 1
#define DW1000_PAC32                 2
#define DW1000_PAC64                 3

#define DW1000_PRF_4M                0 /* DW1000不支持该频率的PRF */
#define DW1000_PRF_16M               1
#define DW1000_PRF_64M               2

/* 这里的值是初始值，赋值时需要根据字段所在位进行移位 */
#define DW1000_PLEN_4096             0x03U
#define DW1000_PLEN_2048             0x0AU
#define DW1000_PLEN_1536             0x06U
#define DW1000_PLEN_1024             0x02U
#define DW1000_PLEN_512              0x0DU
#define DW1000_PLEN_256              0x09U
#define DW1000_PLEN_128              0x05U
#define DW1000_PLEN_64               0x01U

#define DW1000_PHR_TYPE_STD          0x0U
#define DW1000_PHR_TYPE_EXT          0x1U

/* Exported macro ------------------------------------------------------------*/

#define IS_DW1000_CHANNEL(channel)    \
    ((channel == DW1000_CHANNEL_1) || \
     (channel == DW1000_CHANNEL_2) || \
     (channel == DW1000_CHANNEL_3) || \
     (channel == DW1000_CHANNEL_4) || \
     (channel == DW1000_CHANNEL_5) || \
     (channel == DW1000_CHANNEL_7))

#define IS_DW1000_DATA_RATE(dataRate)       \
    ((dataRate == DW1000_DATA_RATE_110K) || \
     (dataRate == DW1000_DATA_RATE_850K) || \
     (dataRate == DW1000_DATA_RATE_6M8))

#define IS_DW1000_PAC(pac)    \
    ((pac == DW1000_PAC8) ||  \
     (pac == DW1000_PAC16) || \
     (pac == DW1000_PAC32) || \
     (pac == DW1000_PAC64))

#define IS_DW1000_PRF(prf) \
    ((prf == DW1000_PRF_16M) || (prf == DW1000_PRF_64M))


#define IS_DW1000_TX_PLEN(plen)    \
    ((plen == DW1000_PLEN_4096) || \
     (plen == DW1000_PLEN_2048) || \
     (plen == DW1000_PLEN_1536) || \
     (plen == DW1000_PLEN_1024) || \
     (plen == DW1000_PLEN_512) ||  \
     (plen == DW1000_PLEN_256) ||  \
     (plen == DW1000_PLEN_128) ||  \
     (plen == DW1000_PLEN_64))

#define DW1000_CHECK_PRF_PCODE(prf, pcode)                       \
    ((prf == DW1000_PRF_64M) && (pcode >= 9) && (pcode <= 24) || \
     (prf == DW1000_PRF_16M) && (pcode >= 1) && (pcode <= 8))

#define IS_DW1000_PHR_TYPE(phrMode)      \
    ((phrMode == DW1000_PHR_TYPE_STD) || \
     (phrMode == DW1000_PHR_TYPE_EXT))

/**
 * @brief 带错误处理的宏函数
 * @note  主要用途就是减少代码行数，使代码简洁
 *         宏函数内部会判断fun函数返回值是否为0
 *          不为0则执行handle
 * @param[in] fun 要执行的函数，必须有返回值
 *                 返回值0表示正常
 *                 其他值表示有误
 * @param[in] handle 错误处理，返回非0时将执行handle
 */
#define DW1000_EXEC_WITH_ERR_HANDLE(fun, handle) \
    do {                                         \
        if (fun) {                               \
            handle;                              \
        }                                        \
    } while (0)

/**
 * @defgroup dw1000_link_driver dw1000 link driver function
 * @brief    dw1000 link driver modules
 * @ingroup  dw1000_driver
 * @{
 */


/**
 * @brief 初始化 dw1000_handle_t 结构体
 * @param HANDLE 指向 dw1000_handle_t 结构体的指针
 * @param STRUCTURE dw1000_handle_t
 * @note 无
 */
#define DRIVER_DW1000_LINK_INIT(HANDLE, STRUCTURE)       memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief 链接 spi_init 函数
 * @param HANDLE 指向 dw1000_handle_t 结构体的指针
 * @param FUC 指向 spi_init 函数地址
 * @note 无
 */
#define DRIVER_DW1000_LINK_SPI_INIT(HANDLE, FUC)         (HANDLE)->spi_init = FUC


/**
 * @brief 链接 spi_deinit 函数
 * @param HANDLE 指向 dw1000_handle_t 结构体的指针
 * @param FUC 指向 spi_deinit 函数地址
 * @note 无
 */
#define DRIVER_DW1000_LINK_SPI_DEINIT(HANDLE, FUC)       (HANDLE)->spi_deinit = FUC

/**
 * @brief 链接 spi_read 函数
 * @param HANDLE 指向 dw1000_handle_t 结构体的指针
 * @param FUC 指向 spi_read 函数地址
 * @note 无
 */
#define DRIVER_DW1000_LINK_SPI_READ(HANDLE, FUC)         (HANDLE)->spi_read = FUC

/**
 * @brief 链接 spi_write 函数
 * @param HANDLE 指向 dw1000_handle_t 结构体的指针
 * @param FUC 指向 spi_write 函数地址
 * @note 无
 */
#define DRIVER_DW1000_LINK_SPI_WRITE(HANDLE, FUC)        (HANDLE)->spi_write = FUC

/**
 * @brief 链接 delay_ms 函数
 * @param HANDLE 指向 dw1000_handle_t 结构体的指针
 * @param FUC 指向 delay_ms 函数地址
 * @note 无
 */
#define DRIVER_DW1000_LINK_DELAY_MS(HANDLE, FUC)         (HANDLE)->delay_ms = FUC

/**
 * @brief 链接 debug_print 函数
 * @param HANDLE 指向 dw1000_handle_t 结构体的指针
 * @param FUC 指向 debug_print 函数地址
 * @note 无
 */
#define DRIVER_DW1000_LINK_DEBUG_PRINT(HANDLE, FUC)      (HANDLE)->debug_print = FUC

/**
 * @brief 链接 receive_callback 函数
 * @param HANDLE 指向 dw1000_handle_t 结构体的指针
 * @param FUC 指向 receive_callback 函数地址
 * @note  无
 */
#define DRIVER_DW1000_LINK_RECEIVE_CALLBACK(HANDLE, FUC) (HANDLE)->receive_callback = FUC

/**
 * @}
 */


/* Exported functions --------------------------------------------------------*/

#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
uint8_t DW1000_RegisterCallback(DW1000_Handle_t* handle, DW1000_CallbackIdTypeDef callbackId, DW1000_CallbackTypeDef callback);
uint8_t DW1000_UnregisterCallback(DW1000_Handle_t* handle, DW1000_CallbackIdTypeDef callbackId);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
uint8_t DW1000_API_VersionGet(uint32_t* version);
uint8_t DW1000_LocalDataPtrSet(DW1000_Handle_t* handle, uint8_t* ptr);
uint8_t DW1000_IC_RefVoltGet(DW1000_Handle_t* handle);
uint8_t DW1000_IC_RefTempGet(DW1000_Handle_t* handle);
uint32_t DW1000_PartIdGet(DW1000_Handle_t* handle);
uint32_t DW1000_LotIdGet(DW1000_Handle_t* handle);
uint8_t DW1000_DevIdGet(DW1000_Handle_t* handle, uint32_t* dev_id);
uint8_t DW1000_OtpRevisionGet(DW1000_Handle_t* handle, uint8_t* revision);

uint8_t DW1000_FineGrainTxSeqCmd(DW1000_Handle_t* handle, uint16_t cmd);
uint8_t DW1000_LNA_PA_ModeSet(DW1000_Handle_t* handle, uint32_t mode);
uint8_t DW1000_GPIO_ClockEnable(DW1000_Handle_t* handle); // 【后续创建一个函数统一管理这些时钟的开关】
uint8_t DW1000_GPIO_DirectionSet(DW1000_Handle_t* handle, uint32_t direction);
uint8_t DW1000_GPIO_ValueSet(DW1000_Handle_t* handle, uint32_t value);
uint8_t DW1000_GPIO_ValueGet(DW1000_Handle_t* handle, uint32_t gpio, uint8_t* value);


// uint8_t DW1000_Init(DW1000_Handle_t* handle,uint8_t);
/**
 * @defgroup dw1000_basic_driver dw1000 basic driver function
 * @brief    dw1000 basic driver modules
 * @ingroup  dw1000_driver
 * @{
 */


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif