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
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define USE_DW1000_REGISTER_CALLBACKS 0U
#define USE_DW1000_FULL_ASSERT        0U
/* Exported types ------------------------------------------------------------*/
/** @defgroup DW1000_Exported_Types DW1000 Exported Types
 * @{
 */

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

typedef struct DW1000_TX_Config_s {
    uint8_t pulseGeneratorDelay;
    uint32_t TX_power;
} DW1000_TX_Config_t;

typedef struct DW1000_Config_s {
    uint8_t channel;
    uint8_t pulseRepetionFrequency;
    uint8_t TX_preambleLength;
    uint8_t RX_Pac;
    uint8_t TX_preambleCode;
    uint8_t RX_preambleCode;
    uint8_t SFD_isNonStandard;
    uint8_t dataRate;
    uint8_t PHR_mode;
    uint16_t SFD_timeout;
} DW1000_Config_t;

typedef struct DW1000_RX_DIAG_s {
    uint16_t maxNoise;
    uint16_t firstPathAmplitude1;
    uint16_t standardDeviationNoise;
    uint16_t firstPathAmplitude2;
    uint16_t firstPathAmplitude3;
    uint16_t CIR_maxGrowth; /* Channel Impulse Response max growth */
    uint16_t RX_preambleCount;
    uint16_t firstPathIndex;
} DW1000_RX_DIAG_t;

typedef struct DW1000_EVT_CNT_s {
    uint16_t phrError;
    uint16_t rsdError;
    uint16_t frameCheckSeqGood;
    uint16_t frameCheckSeqError;
    uint16_t frameFilterReject;
    uint16_t rxOverrunError;
    uint16_t sfdTimeout;
    uint16_t preambleTimeout;
    uint16_t rxFrameWaitTimeout;
    uint16_t txFrameSent;
    uint16_t halfPeriodWarning;
    uint16_t txPowerupWarning;
} DW1000_EVT_CNT_t;

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


typedef struct
{
    DW1000_RX_DIAG_t rxDiag;
    DW1000_EVT_CNT_t evtCnt;
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
    DW1000_TypeDef* instance;
    // DW1000_StateTypeDef state;
    DW1000_InitTypeDef init;
    DW1000_IF_GPIO_TypeDef wakeup;
    DW1000_IF_GPIO_TypeDef reset;
    DW1000_IF_SPI_TypeDef spi;
    DW1000_IF_IQR_TypeDef host_irq;
    DW1000_Backup_t backup;
    DW1000_CB_Data_t cbData;
    volatile uint32_t errorCode; // 错误码，用于在中断函数里记录发生的错误，当退出中断时由外部进行处理
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

#define DW1000_PLEN_4096             0x0CU
#define DW1000_PLEN_2048             0x28U
#define DW1000_PLEN_1536             0x18U
#define DW1000_PLEN_1024             0x08U
#define DW1000_PLEN_512              0x34U
#define DW1000_PLEN_256              0x24U
#define DW1000_PLEN_128              0x14U
#define DW1000_PLEN_64               0x04U

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
uint8_t DW1000_IC_RefVoltGet(DW1000_Handle_t* handle, uint8_t* volt);
uint8_t DW1000_IC_RefTempGet(DW1000_Handle_t* handle, uint8_t* temp);
uint8_t DW1000_PartIdGet(DW1000_Handle_t* handle, uint32_t* part_id);
uint8_t DW1000_LotIdGet(DW1000_Handle_t* handle, uint16_t* lot_id);
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