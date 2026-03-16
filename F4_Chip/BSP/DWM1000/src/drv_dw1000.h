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
/* 是否使能DW1000的注册回调函数功能 */
#define USE_DW1000_REGISTER_CALLBACKS   0U
/* 是否使能DW1000的注册获取计数函数功能 */
#define USE_DW1000_REGISTER_GET_TICK    0U
/* 是否使能DW1000的注册Debug打印功能 */
#define USE_DW1000_REGISTER_DEBUG_PRINT 0U
/* Exported types ------------------------------------------------------------*/
/**
 * @defgroup dw1000_driver dw1000 driver function
 * @brief    dw1000 driver modules
 * @{
 */

/**
 * @addtogroup dw1000_basic_driver
 * @{
 */

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
    DW1000_REG_SYS_STATUS_t SYS_STATUS; // 4字节
    DW1000_REG_LDE_THRESH_t LDE_THRESH; // 2字节
    DW1000_SREG_AON_WCFG_ut AON_WCFG;   // 2字节
    DW1000_SREG_TC_PGDELAY_t TC_PGDELY; // 1字节
    DW1000_REG_RX_TIME_t RX_TIME;       // 18字节
    DW1000_REG_TX_FCTRL_ut TX_FCTRL;    // 5字节
} DW1000_TypeDef;

typedef struct
{
    uint8_t isRxOnAfterTx : 1;                 /**< 在发送完成后是否自动开启接收（通常用于接收应答信息）*/
    uint8_t shouldOtpPartIdRead : 1;           /**< 是否从OTP中加载partID */
    uint8_t shouldOtpLotIdRead : 1;            /**< 是否从OTP中加载LOT ID */
    uint8_t shouldOtpRefVoltRead : 1;          /**< 是否从OTP中加载参考电压值 */
    uint8_t shouldOtpRefTempRead : 1;          /**< 是否从OTP中加载参考温度值 */
    uint8_t shouldOtpUcodeRead : 1;            /**< 是否从OTP中加载微码 */
    uint8_t shouldOtpUcodeReadAfterWakeUp : 1; /**< 是否在唤醒后从OTP中加载微码 */
    uint8_t shouldOtpRevReadAfterWakeUp : 1;   /**< 是否在唤醒后从OTP中加载OTP修订号 */
    uint8_t useSfdNstd : 1;                    /**< 是否使用非标 SFD 以获得更好的性能 */
    uint8_t useDoubleBuff : 1;                 /**< 是否为双缓冲模式 */
    uint8_t useLowPowListen : 1;               /**< 是否使用低功耗监听 */
    uint8_t channel : 3;                       /**< 频道号: DW1000_CHANNEL_x（x取值1-7，不包含6） */
    uint8_t dataRate : 2;                      /**< 数据速率: 110kbps 850kbps 6.8Mbps */
    uint8_t prf : 2;                           /**< 脉冲重复频率: 16MHz 和 64MHz */
    uint8_t phrType : 1;                       /**< PHR模式： StdFrameMode 和 LongFrameMode */
    uint8_t : 5;                               /**< 保留位 */
    uint8_t txPreambleLength;                  /**< 发送前导码长度: 64 128 256 512 1024 2048 4096 */
    uint8_t rxPac;                             /**< 前导码查询块大小（与接收前导码长度相关） */
    uint8_t txPreambleCode;                    /**< 发送前导码 */
    uint8_t rxPreambleCode;                    /**< 接收前导码 */
    uint16_t sfdTimeout;                       /**< SFD超时值（单位 符号）SFD超时 = (前导码长度 + 1 + SFD长度 - PAC大小) */
    uint16_t rxTimeout;                        /**< 接收超时时间（在超时时间内没有接收到帧），设置为0表示禁用超时 */
    uint32_t rxDelayTime;                      /**< 发送帧完后开启接收器的延迟时间（最短6.2us，即使设置为0） */
} DW1000_InitTypeDef;

/**
 * @brief  DW1000 State structure definition
 */
typedef enum {
    DW1000_STATE_ORIGINAL, /**< DW1000原始状态 */
    DW1000_STATE_RESET,    /**< DW1000已复位 */
    DW1000_STATE_READY,    /**< Peripheral Initialized and ready for use           */
    DW1000_STATE_BUSY,     /**< an internal process is ongoing                     */
    DW1000_STATE_BUSY_TX,  /**< Data Transmission process is ongoing               */
    DW1000_STATE_BUSY_RX,  /**< Data Reception process is ongoing                  */
    DW1000_STATE_ERROR,    /**< DW1000 error state                                    */
    DW1000_STATE_ABORT,    /**< DW1000 abort is ongoing                               */
    DW1000_STATE_RESETING  /**< DW1000 复位中 */
} DW1000_StateTypeDef;

/**
 * @brief DW1000 GPIO接口定义
 */
typedef struct {
    // void (*init)(void);             /**< 初始化引脚 */
    // void (*deinit)(void);           /**< 反初始化引脚 */
    uint8_t (*read)(uint8_t* data); /**< 读取引脚高低电平 */
    uint8_t (*write)(uint8_t data); /**< 设置引脚高低电平 */
} DW1000_IF_GPIO_TypeDef;

/**
 * @brief DW1000 SPI接口定义
 */
typedef struct {
    // uint8_t (*init)(void);                                                         /**< SPI接口初始化 */
    // uint8_t (*deinit)(void);                                                       /**< SPI接口反初始化 */
    uint8_t (*highSpeedSet)(void);                                                 /**< 设置SPI接口通信速率高 */
    uint8_t (*lowSpeedSet)(void);                                                  /**< 设置SPI接口通信速率低 */
    uint8_t (*read)(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen);  /**< SPI接口读操作 */
    uint8_t (*write)(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen); /**< SPI接口写操作 */
    DW1000_IF_GPIO_TypeDef nss;                                                    /**< SPI片选引脚 */
} DW1000_IF_SPI_TypeDef;

/**
 * @brief DW1000中断接口（用于出入临界区功能实现）
 */
typedef struct DW1000_IRQ_s {
    void (*enable)(void);    /**< 使能与DW1000的IRQ引脚相连引脚的外部中断 */
    void (*disable)(void);   /**< 禁用能与DW1000的IRQ引脚相连引脚的外部中断 */
    int32_t criticalNesting; /**< 嵌套计数（使用i32类型避免错误调用，易排错） */
} DW1000_IF_IQR_TypeDef;


/**
 * @brief DW1000 handle 结构体定义
 */
typedef struct DW1000_Handle_s {
    // DW1000_LockTypeDef lock;
    DW1000_StateTypeDef state;
    DW1000_TypeDef instance;
    DW1000_InitTypeDef init;
    DW1000_IF_SPI_TypeDef spi;
    DW1000_IF_IQR_TypeDef irq;
    DW1000_IF_GPIO_TypeDef wakeup; /**< wakeup引脚 */
    struct {
        void (*init)(uint8_t mode); /**< 初始化DW1000的RSTn引脚，可以配置为开漏输出（mode = 0）与外部中断（mode = 1） */
        // void (*deinit)(void);           /**< 反初始化与DW1000的RSTn相连的引脚 */
        uint8_t (*read)(uint8_t* data); /**< 读取RSTn引脚的高低电平 */
        uint8_t (*write)(uint8_t data); /**< 设置RSTn引脚的高低电平（只有低电平输出有效） */
        // uint8_t (*itEnable)(void);            /**< 使能与DW1000的RSTn引脚相连引脚的外部中断 */
        // uint8_t (*itDisable)(void);           /**< 禁用与DW1000的RSTn引脚相连引脚的外部中断 */
    } rst;                       /**< rstn引脚 */
    volatile uint32_t errorCode; /**< 错误码，用于在中断函数里记录发生的错误，当退出中断时由外部进行处理 4个字节 */
    DW1000_CB_Data_t cbData;     /**< 回调数据 */
#if (USE_DW1000_REGISTER_DEBUG_PRINT == 1U)
    /* 调试格式化输出函数 */
    void (*debugPrint)(const char* const fmt, ...);
#endif /* USE_DW1000_REGISTER_DEBUG_PRINT */
#if (USE_DW1000_REGISTER_GET_TICK == 1U)
    /*获取Tick值函数 */
    uint32_t (*getTick)(void);
#endif /* USE_DW1000_REGISTER_GET_TICK */
#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
    void (*TxCompleteCallback)(DW1000_Handle_t* handle);
    void (*RxCompleteCallback)(DW1000_Handle_t* handle);
    void (*RxErrorCallback)(DW1000_Handle_t* handle);
    void (*RxTimeoutCallback)(DW1000_Handle_t* handle);
#endif /* USE_DW1000_REGISTER_CALLBACKS */
} DW1000_Handle_t;

#if (USE_DW1000_REGISTER_DEBUG_PRINT == 1U)
typedef void (*DW1000_DebugPrintTypeDef)(const char* const fmt, ...);
#endif /* USE_DW1000_REGISTER_DEBUG_PRINT */

#if (USE_DW1000_REGISTER_GET_TICK == 1U)
typedef uint32_t (*DW1000_GetTickTypeDef)(void);
#endif /* USE_DW1000_REGISTER_GET_TICK */

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
    char driverVersion[32];    /**< 驱动版本 */
    char chipName[32];         /**< 芯片名称 */
    char manufacturerName[32]; /**< 生产商名称 */
    char interface[8];         /**< 接口名称 */
    float supplyVoltage_min_v; /**< 芯片最小供电电压（V） */
    float supplyVoltage_max_v; /**< 芯片最大供电电压（V） */
    float current_max_ma;      /**< 芯片最大电流（mA） */
    float temperature_min;     /**< 芯片最低运行温度（℃） */
    float temperature_max;     /**< 芯片最高运行温度（℃） */
} DW1000_Info_t;

/**
 * @}
 */


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

/**
 * @brief 定义DW1000的接收模式
 */
#define DW1000_RX_MODE_IMMEDIATE                     0x00
#define DW1000_RX_MODE_DELAYED                       0x01
#define DW1000_RX_MODE_IDLE_ON_DELAY_ERR             0x02
#define DW1000_RX_MODE_NO_SYNC_PTRS                  0x04
#define DW1000_RX_MODE_DELAYED_AND_IDLE_ON_DELAY_ERR (DW1000_RX_MODE_DELAYED | DW1000_RX_MODE_IDLE_ON_DELAY_ERR)
#define DW1000_RX_MODE_IMMEDIATE_AND_NO_SYNC_PTRS    (DW1000_RX_MODE_IMMEDIATE |)

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


/* Exported functions --------------------------------------------------------*/

/**
 * @defgroup nrf24l01_link_driver nrf24l01 link driver function
 * @brief    nrf24l01 link driver modules
 * @ingroup  nrf24l01_driver
 * @{
 */

/**
 * @brief 初始化 DW1000_Handle_t 结构体
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] STRUCTURE DW1000_Handle_t 类型
 * @note  无
 */
#define DRIVER_DW1000_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief 初始化SPI init函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI init函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_INIT(HANDLE, FUC)           (HANDLE)->spi.init = FUC

/**
 * @brief 初始化SPI deinit函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI deinit函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_DEINIT(HANDLE, FUC)         (HANDLE)->spi.deinit = FUC

/**
 * @brief 初始化SPI highSpeedSet函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI highSpeedSet函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_HIGH_SPEED_SET(HANDLE, FUC) (HANDLE)->spi.highSpeedSet = FUC

/**
 * @brief 初始化SPI lowSpeedSet函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI lowSpeedSet函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_LOW_SPEED_SET(HANDLE, FUC)  (HANDLE)->spi.lowSpeedSet = FUC

/**
 * @brief 初始化SPI read函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI read函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_READ(HANDLE, FUC)           (HANDLE)->spi.read = FUC

/**
 * @brief 初始化SPI write函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI write函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_WRITE(HANDLE, FUC)          (HANDLE)->spi.write = FUC

/**
 * @brief 初始化SPI NSS的init函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI NSS的init函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_NSS_INIT(HANDLE, FUC)       (HANDLE)->spi.nss.init = FUC

/**
 * @brief 初始化SPI NSS的deinit函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI NSS的deinit函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_NSS_DEINIT(HANDLE, FUC)     (HANDLE)->spi.nss.deinit = FUC

/**
 * @brief 初始化SPI NSS的read函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI NSS的read函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_NSS_READ(HANDLE, FUC)       (HANDLE)->spi.nss.read = FUC

/**
 * @brief 初始化SPI NSS的write函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向SPI NSS的write函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_SPI_NSS_WRITE(HANDLE, FUC)      (HANDLE)->spi.nss.write = FUC

/**
 * @brief 初始化GPIO wakeup的init函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向GPIO wakeup的init函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_GPIO_WAKEUP_INIT(HANDLE, FUC)   (HANDLE)->wakeup.init = FUC

/**
 * @brief 初始化GPIO wakeup的deinit函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向GPIO wakeup的deinit函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_GPIO_WAKEUP_DEINIT(HANDLE, FUC) (HANDLE)->wakeup.deinit = FUC

/**
 * @brief 初始化GPIO wakeup的read函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向GPIO wakeup的read函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_GPIO_WAKEUP_READ(HANDLE, FUC)   (HANDLE)->wakeup.read = FUC

/**
 * @brief 初始化GPIO wakeup的write函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向GPIO wakeup的write函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_GPIO_WAKEUP_WRITE(HANDLE, FUC)  (HANDLE)->wakeup.write = FUC

/**
 * @brief 初始化GPIO RSTn init函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向GPIO RSTn init函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_GPIO_RST_INIT(HANDLE, FUC)      (HANDLE)->rst.init = FUC

/**
 * @brief 初始化GPIO RSTn deinit函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向GPIO RSTn deinit函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_GPIO_RST_DEINIT(HANDLE, FUC)    (HANDLE)->rst.deinit = FUC

/**
 * @brief 初始化GPIO RSTn read函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向GPIO RSTn read函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_GPIO_RST_READ(HANDLE, FUC)      (HANDLE)->rst.read = FUC

/**
 * @brief 初始化GPIO RSTn write函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向GPIO RSTn write函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_GPIO_RST_WRITE(HANDLE, FUC)     (HANDLE)->rst.write = FUC

/**
 * @brief 初始化IRQ enable函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向IRQ enable函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_IRQ_ENABLE(HANDLE, FUC)         (HANDLE)->irq.enable = FUC

/**
 * @brief 初始化IRQ disable函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向IRQ disable函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_IRQ_DISABLE(HANDLE, FUC)        (HANDLE)->irq.disable = FUC

/**
 * @brief 初始化 getTick 函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向getTick函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_GET_TICK(HANDLE, FUC)           (HANDLE)->getTick = FUC

/**
 * @brief 初始化 debugPrint 函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向debugPrint函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_DEBUG_PRINT(HANDLE, FUC)        (HANDLE)->debugPrint = FUC

/**
 * @brief 初始化 TxCompleteCallback 函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向 TxCompleteCallback 函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_TX_CPLT_CALLBACK(HANDLE, FUC)   (HANDLE)->TxCompleteCallback = FUC

/**
 * @brief 初始化 RxCompleteCallback 函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向 RxCompleteCallback 函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_RX_CPLT_CALLBACK(HANDLE, FUC)   (HANDLE)->RxCompleteCallback = FUC

/**
 * @brief 初始化 RxErrorCallback 函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向 RxErrorCallback 函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_RX_ERR_CALLBACK(HANDLE, FUC)    (HANDLE)->RxErrorCallback = FUC

/**
 * @brief 初始化 RxTimeoutCallback 函数
 * @param [in] HANDLE DW1000_Handle_t 指针
 * @param [in] FUC 指向 RxTimeoutCallback 函数的函数指针
 * @note  备注
 */
#define DRIVER_DW1000_LINK_RX_TO_CALLBACK(HANDLE, FUC)     (HANDLE)->RxTimeoutCallback = FUC


/**
 * @}
 */

/**
 * @defgroup dw1000_basic_driver dw1000 basic driver function
 * @brief    dw1000 basic driver modules
 * @ingroup  dw1000_driver
 * @{
 */

#if (USE_Dw1000_REGISTER_DEBUG_PRINT == 1U)
void DW1000_RegisterDebugPrint(DW1000_Handle_t* handle, DW1000_DebugPrintTypeDef debugPrint);
void DW1000_UnregisterDebugPrint(DW1000_Handle_t* handle);
#endif /* USE_Dw1000_REGISTER_DEBUG_PRINT */

#if (USE_DW1000_REGISTER_GET_TICK == 1U)
void DW1000_RegisterGetTick(DW1000_Handle_t* handle, DW1000_GetTickTypeDef getTick);
void DW1000_UnregisterGetTick(DW1000_Handle_t* handle);
#endif /* USE_DW1000_REGISTER_GET_TICK */

#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
uint8_t DW1000_RegisterCallback(DW1000_Handle_t* handle, DW1000_CallbackIdTypeDef callbackId, DW1000_CallbackTypeDef callback);
uint8_t DW1000_UnregisterCallback(DW1000_Handle_t* handle, DW1000_CallbackIdTypeDef callbackId);
#endif /* USE_DW1000_REGISTER_CALLBACKS */

void DW1000_Handle_DeInit(DW1000_Handle_t* handle);
void DW1000_Info_DeInit(DW1000_Info_t* info);
void DW1000_Info(DW1000_Info_t* info);
void DW1000_IRQHandler(DW1000_Handle_t* handle);
uint8_t DW1000_Prepare(DW1000_Handle_t* handle);
uint8_t DW1000_PrepareAfterWakeUp(DW1000_Handle_t* handle);
uint8_t DW1000_Init(DW1000_Handle_t* handle);
uint8_t DW1000_DeInit(DW1000_Handle_t* handle);
uint8_t DW1000_FineGrainTxSeqCmd(DW1000_Handle_t* handle, uint16_t cmd);
uint8_t DW1000_LNA_PA_ModeSet(DW1000_Handle_t* handle, uint32_t mode);
uint8_t DW1000_GPIO_PinModeSelect(DW1000_Handle_t* handle, uint8_t pin, uint8_t mode);
uint8_t DW1000_GPIO_ClockEnable(DW1000_Handle_t* handle);
uint8_t DW1000_GPIO_DirectionSet(DW1000_Handle_t* handle, uint32_t direction);
uint8_t DW1000_GPIO_ValueSet(DW1000_Handle_t* handle, uint32_t out);
uint8_t DW1000_GPIO_ValueGet(DW1000_Handle_t* handle, uint32_t gpio, uint8_t* value);
uint8_t DW1000_IC_RefVoltGet(DW1000_Handle_t* handle);
uint8_t DW1000_IC_RefTempGet(DW1000_Handle_t* handle);
uint32_t DW1000_PartIdGet(DW1000_Handle_t* handle);
uint32_t DW1000_LotIdGet(DW1000_Handle_t* handle);
uint8_t DW1000_DevIdGet(DW1000_Handle_t* handle, uint32_t* id);
uint8_t DW1000_TX_RF_Config(DW1000_Handle_t* handle);
uint8_t DW1000_PreambleLength64Config(DW1000_Handle_t* handle, uint32_t value);
uint8_t DW1000_RX_AntennaDelaySet(DW1000_Handle_t* handle, uint16_t delay);
uint8_t DW1000_TX_AntennaDelaySet(DW1000_Handle_t* handle, uint16_t delay);
uint8_t DW1000_TX_DataWrite(DW1000_Handle_t* handle, uint8_t* data, uint16_t len, uint16_t offset);
uint8_t DW1000_TX_FrameCtrlWrite(DW1000_Handle_t* handle, uint16_t len, uint16_t offset, uint8_t isRanging);
uint8_t DW1000_RX_DataRead(DW1000_Handle_t* handle, uint8_t* data, uint16_t len, uint16_t offset);
uint8_t DW1000_ACC_DataRead(DW1000_Handle_t* handle, uint8_t* data, uint16_t len, uint16_t offset);
uint8_t DW1000_CarrierIntegratorRead(DW1000_Handle_t* handle, int32_t* carrierIntegrator);
uint8_t DW1000_DiagnosticsRead(DW1000_Handle_t* handle);
uint8_t DW1000_TX_TimeStampRead(DW1000_Handle_t* handle, void* timeStamp);
uint8_t DW1000_TX_TimeStamp_H32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_h32);
uint8_t DW1000_TX_TimeStamp_L32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_l32);
uint8_t DW1000_RX_TimeStampRead(DW1000_Handle_t* handle, void* timeStamp);
uint8_t DW1000_RX_TimeStamp_H32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_h32);
uint8_t DW1000_RX_TimeStamp_L32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_l32);
uint8_t DW1000_SystemTimeRead(DW1000_Handle_t* handle, void* sysTime);
uint8_t DW1000_SystemTime_H32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_h32);
uint8_t DW1000_SystemTime_L32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_l32);
uint8_t DW1000_FrameFilterConfig(DW1000_Handle_t* handle, uint8_t config);
uint8_t DW1000_PAN_ID_Set(DW1000_Handle_t* handle, uint16_t PAN_id);
uint8_t DW1000_ShortAddressSet(DW1000_Handle_t* handle, uint16_t shortAddress);
uint8_t DW1000_EuiSet(DW1000_Handle_t* handle, uint64_t eui);
uint8_t DW1000_EuiGet(DW1000_Handle_t* handle, uint64_t* eui);
uint8_t DW1000_OTP_Read(DW1000_Handle_t* handle, uint16_t address, uint32_t* buff, uint16_t len);
uint8_t DW1000_OTP_WriteWord32WithVerify(DW1000_Handle_t* handle, uint32_t data, uint16_t address);
uint8_t DW1000_SleepEnter(DW1000_Handle_t* handle);
uint8_t DW1000_SleepCountConfig(DW1000_Handle_t* handle, uint16_t sleepCount);
uint8_t DW1000_SleepCountCalibrate(DW1000_Handle_t* handle, uint16_t* tick);
uint8_t DW1000_SleepConfig(DW1000_Handle_t* handle, uint16_t mode, uint8_t wake);
uint8_t DW1000_EnterSleepAfterTxSet(DW1000_Handle_t* handle, uint8_t enable);
uint8_t DW1000_WakeUpBySpiRead(DW1000_Handle_t* handle, uint8_t* buff, uint16_t len);
uint8_t DW1000_LoadOperParamSetFromOTP(DW1000_Handle_t* handle, uint16_t param);
uint8_t DW1000_SmartTxPowerSet(DW1000_Handle_t* handle, uint32_t enable);
uint8_t DW1000_AutoAckEnable(DW1000_Handle_t* handle, uint8_t respDelayTime);
uint8_t DW1000_DoubleRxBuffModeSet(DW1000_Handle_t* handle, uint8_t enable);
uint8_t DW1000_RxDoubleBuffEnable(DW1000_Handle_t* handle);
uint8_t DW1000_RxDoubleBuffDisable(DW1000_Handle_t* handle);
uint8_t DW1000_RxAfterTxDelaySet(DW1000_Handle_t* handle, uint32_t rxDelayTime);
uint8_t DW1000_RxBuffPtrsSync(DW1000_Handle_t* handle);
uint8_t DW1000_ForceTrxOff(DW1000_Handle_t* handle);
uint8_t DW1000_TrxDelayTimeH32Set(DW1000_Handle_t* handle, uint32_t delayTime);
uint8_t DW1000_RxReset(DW1000_Handle_t* handle);
uint8_t DW1000_TxStart(DW1000_Handle_t* handle, uint8_t mode);
uint8_t DW1000_IRQ_Check(DW1000_Handle_t* handle, uint8_t* isActive);
uint8_t DW1000_LowPowerListeningSet(DW1000_Handle_t* handle, uint32_t enable);
uint8_t DW1000_LplEnable(DW1000_Handle_t* handle);
uint8_t DW1000_LplDisable(DW1000_Handle_t* handle);
uint8_t DW1000_SnoozeTimeSet(DW1000_Handle_t* handle, uint8_t snoozeTime);
// uint8_t DW1000_LEDsSet(DW1000_Handle_t* handle, uint8_t enable);
uint8_t DW1000_SniffModeSet(DW1000_Handle_t* handle, uint8_t enable, uint8_t timeOn, uint8_t timeOff);
uint8_t DW1000_SniffModeEnable(DW1000_Handle_t* handle, uint8_t timeOn, uint8_t timeOff);
uint8_t DW1000_SniffModeDisable(DW1000_Handle_t* handle);
uint8_t DW1000_RxTimeoutSet(DW1000_Handle_t* handle, uint16_t timeout);
uint8_t DW1000_RxTimeoutEnable(DW1000_Handle_t* handle);
uint8_t DW1000_RxTimeoutDisable(DW1000_Handle_t* handle);
uint8_t DW1000_RxEnable(DW1000_Handle_t* handle, uint8_t mode);
uint8_t DW1000_RxDisable(DW1000_Handle_t* handle);
uint8_t DW1000_PreambleDetectTimeoutSet(DW1000_Handle_t* handle, uint16_t pdTimeout);
uint8_t DW1000_InterruptSet(DW1000_Handle_t* handle, uint32_t event);
uint8_t DW1000_InterruptEnable(DW1000_Handle_t* handle, uint32_t event);
uint8_t DW1000_InterruptDisable(DW1000_Handle_t* handle, uint32_t event);
uint8_t DW1000_EventCountersConfig(DW1000_Handle_t* handle, uint8_t enable);
uint8_t DW1000_EventCountersRead(DW1000_Handle_t* handle);
uint8_t DW1000_HardReset(DW1000_Handle_t* handle);
uint8_t DW1000_SoftReset(DW1000_Handle_t* handle);
uint8_t DW1000_XtalTrimSet(DW1000_Handle_t* handle, uint8_t trimValue);
uint8_t DW1000_XtalTrimGet(DW1000_Handle_t* handle, uint8_t* trimValue);
uint8_t DW1000_CW_ModeConfig(DW1000_Handle_t* handle, uint8_t channel);
uint8_t DW1000_CF_ModeConfig(DW1000_Handle_t* handle, uint32_t repetition);
uint8_t DW1000_RawTempVoltRead(DW1000_Handle_t* handle, uint8_t SPI_isFast, uint8_t* temp, uint8_t* volt);
uint8_t DW1000_RawTempVoltReadInSlowSpi(DW1000_Handle_t* handle, uint8_t* temp, uint8_t* volt);
uint8_t DW1000_RawTempVoltReadInFastSpi(DW1000_Handle_t* handle, uint8_t* temp, uint8_t* volt);
float DW1000_TempRawToRealConvert(DW1000_Handle_t* handle, uint8_t rawTemp);
uint8_t DW1000_TempRealToRawConvert(DW1000_Handle_t* handle, int16_t realTemp_x10);
float DW1000_VoltRawToRealConvert(DW1000_Handle_t* handle, uint8_t rawVolt);
uint8_t DW1000_VoltRealToRawConvert(DW1000_Handle_t* handle, int32_t realVolt_x1000);
uint8_t DW1000_TempReadOnWakeUp(DW1000_Handle_t* handle, uint8_t* temp);
uint8_t DW1000_TempReadOnWakeUp(DW1000_Handle_t* handle, uint8_t* temp);
uint8_t DW1000_VoltReadOnWakeUp(DW1000_Handle_t* handle, uint8_t* volt);
uint8_t DW1000_BW_TempCompCalculate(DW1000_Handle_t* handle, uint16_t target, uint32_t* best);
uint32_t DW1000_TxPowerTempCompCalculate(uint8_t channel, uint32_t txPowerRef, int32_t tempDelta);
uint8_t DW1000_PG_CounterCalculate(DW1000_Handle_t* handle, uint8_t pgdly, uint16_t* delayCount);
uint8_t DW1000_LDE_AlgoDisable(DW1000_Handle_t* handle);
uint8_t DW1000_LDE_AlgoEnable(DW1000_Handle_t* handle);

#if (USE_DW1000_REGISTER_GET_TICK == 1U)
void DW1000_Delay(DW1000_Handle_t* handle, uint32_t delay);
#else  /* USE_DW1000_REGISTER_GET_TICK */
void DW1000_Delay(uint32_t delay);
#endif /* USE_DW1000_REGISTER_GET_TICK */

uint32_t DW1000_GetTick(void);

void DW1000_DebugPrint(const char* const fmt, ...);

void DW1000_TxCompleteCallback(DW1000_Handle_t* handle);
void DW1000_RxCompleteCallback(DW1000_Handle_t* handle);
void DW1000_RxErrorCallback(DW1000_Handle_t* handle);
void DW1000_RxTimeoutCallback(DW1000_Handle_t* handle);

/**
 * @}
 */

/**
 * @defgroup nrf24l01_extern_driver nrf24l01 extern driver function
 * @brief    nrf24l01 extern driver modules
 * @ingroup  nrf24l01_driver
 * @{
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif