/**
 * ************************************************************************
 *
 * @file driver_dw1000.c
 * @author quote6 (2293127401@qq.com)
 * @brief
 * @warning 这里的函数都不是线程安全的，多线程情况下需要进行额外的临界保护
 * ************************************************************************
 * @copyright Copyright (c) 2025 quote6
 * For study and research only, no reprinting
 * ************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "drv_dw1000.h"
#include "drv_dw1000_assert.h"
#include "drv_dw1000_reg_params.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct {
    uint32_t lo32;
    uint16_t target[DW1000_SUPPORT_PRF_NUM];
} DW1000_AGC_CFG_t;
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* 定义相关位操作宏函数用于寄存器操作 */
#define DW1000_SET_BITS(reg, bits)                 ((reg) |= (bits))
#define DW1000_CLEAR_BITS(reg, bits)               ((reg) &= ~(bits))
#define DW1000_READ_BITS(reg, bits)                ((reg) & (bits))
#define DW1000_CLEAR_REG(reg)                      ((reg) = 0)
#define DW1000_WRITE_REG(reg, value)               ((reg) = (value))
#define DW1000_READ_REG(reg)                       ((reg))
#define DW1000_MODIFY_REG(reg, clearMask, setMask) ((reg) = (((reg) & (~(clearMask))) | (setMask)))

/**
 * @brief chip register definition
 */
#define CHIP_NAME                                  "Qorvo DW1000" /**< chip name */
#define MANUFACTURER_NAME                          "Qorvo"        /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN                         2.0f           /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX                         3.6f           /**< chip max supply voltage */
#define MAX_CURRENT                                0.145f         /**< chip max current */
#define TEMPERATURE_MIN                            -40.0f         /**< chip min operating temperature */
#define TEMPERATURE_MAX                            85.0f          /**< chip max operating temperature */
#define DRIVER_VERSION                             1000

/* 给寄存器地址添加读/写命令 */
// #define DW1000_SPI_CMD_READ(addr)  DW1000_CLEAR_BITS(addr, DW1000_SPI_CMD_MASK)
#define DW1000_SPI_CMD_READ(addr)                  (addr & 0x7F)
// #define DW1000_SPI_CMD_WRITE(addr) DW1000_SET_BITS(addr, DW1000_SPI_CMD_MASK)
#define DW1000_SPI_CMD_WRITE(addr)                 (addr | 0x80)

/* 给寄存器地址添加是否存在子地址标志 */
// #define DW1000_SPI_NOT_SUB_ADDR(addr) DW1000_CLEAR_BITS(addr, DW1000_SPI_SUB_ADDR_MASK)
#define DW1000_SPI_NOT_SUB_ADDR(addr)              (addr & 0xBF)
// #define DW1000_SPI_HAS_SUB_ADDR(addr) DW1000_SET_BITS(addr, DW1000_SPI_SUB_ADDR_MASK)
#define DW1000_SPI_HAS_SUB_ADDR(addr)              (addr | 0x40)


/* 给子地址添加是否为扩展子地址标志 */
// #define DW1000_SPI_NOT_EXT_SUB_ADDR(sa) DW1000_CLEAR_BITS(sa, DW1000_SPI_EXT_SUB_ADDR_MASK)
#define DW1000_SPI_NOT_EXT_SUB_ADDR(sa)            (sa & 0x7F)
// #define DW1000_SPI_HAS_EXT_SUB_ADDR(sa) DW1000_SET_BITS(sa, DW1000_SPI_EXT_SUB_ADDR_MASK)
#define DW1000_SPI_HAS_EXT_SUB_ADDR(sa)            (sa | 0x80)

/* 使用该宏避免 gcc/g++ 编译警告 */
#define DW1000_UNUSED(X)                           (void) X

/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/

/**
 * @brief 频率合成器 PLL配置（根据频道不同设置不同参数）
 */
const static uint32_t DW1000_FS_PLL_CFG[DW1000_SUPPORT_CHANNEL_NUM + 1] = {
    DW1000_FS_PLL_CFG_CH1,
    DW1000_FS_PLL_CFG_CH2,
    DW1000_FS_PLL_CFG_CH3,
    DW1000_FS_PLL_CFG_CH4,
    DW1000_FS_PLL_CFG_CH5,
    0, /* 频道6不支持 */
    DW1000_FS_PLL_CFG_CH7
};

/**
 * @brief 频率合成器 PLL调谐（根据频道不同设置不同参数）
 */
const static uint8_t DW1000_FS_PLL_TUNE[DW1000_SUPPORT_CHANNEL_NUM + 1] = {
    DW1000_FS_PLL_TUNE_CH1,
    DW1000_FS_PLL_TUNE_CH2,
    DW1000_FS_PLL_TUNE_CH3,
    DW1000_FS_PLL_TUNE_CH4,
    DW1000_FS_PLL_TUNE_CH5,
    0, /* 频道6不支持 */
    DW1000_FS_PLL_TUNE_CH7
};

/**
 * @brief 接收器配置（根据频道不同设置不同参数）
 */
const static uint8_t DW1000_RX_CONFIG[DW1000_SUPPORT_CHANNEL_NUM + 1] = {
    DW1000_RF_RX_CTRLH_CH1,
    DW1000_RF_RX_CTRLH_CH2,
    DW1000_RF_RX_CTRLH_CH3,
    DW1000_RF_RX_CTRLH_CH4,
    DW1000_RF_RX_CTRLH_CH5,
    0, /* 频道6不支持 */
    DW1000_RF_RX_CTRLH_CH7
};

/**
 * @brief 发送器配置（根据频道不同设置不同参数）
 */
const static uint32_t DW1000_TX_CONFIG[DW1000_SUPPORT_CHANNEL_NUM + 1] = {
    DW1000_RF_TXCTRL_CH1,
    DW1000_RF_TXCTRL_CH2,
    DW1000_RF_TXCTRL_CH3,
    DW1000_RF_TXCTRL_CH4,
    DW1000_RF_TXCTRL_CH5,
    0, /* 频道6不支持 */
    DW1000_RF_TXCTRL_CH7,
};

/**
 * @brief 调谐寄存器0b配置（根据数据速率和SFD类型设置不同参数）
 */
const static uint16_t DW1000_SFD_THRESHOLD[DW1000_SUPPORT_DATA_RATE_NUM][DW1000_SUPPORT_SFD_TYPE_NUM] = {
    { DW1000_DRX_TUNE0B_110K_STD,
      DW1000_DRX_TUNE0B_110K_NSTD },
    { DW1000_DRX_TUNE0B_850K_STD,
      DW1000_DRX_TUNE0B_850K_NSTD },
    { DW1000_DRX_TUNE0B_6M8_STD,
      DW1000_DRX_TUNE0B_6M8_NSTD }
};

/**
 * @brief 调谐寄存器1a配置（根据PRF频率设置不同参数）
 */
const static uint16_t DW1000_DRX_TUNE1A[DW1000_SUPPORT_PRF_NUM] = {
    DW1000_DRX_TUNE1A_PRF16M,
    DW1000_DRX_TUNE1A_PRF64M
};

/**
 * @brief 调谐寄存器2配置（根据PAC尺寸和PRF频率设置不同参数）
 */
const static uint32_t DW1000_DRX_TUNE2[DW1000_SUPPORT_PRF_NUM][DW1000_SUPPORT_PAC_NUM] = {
    { DW1000_DRX_TUNE2_PAC8_PRF16M,
      DW1000_DRX_TUNE2_PAC16_PRF16M,
      DW1000_DRX_TUNE2_PAC32_PRF16M,
      DW1000_DRX_TUNE2_PAC64_PRF16M },
    { DW1000_DRX_TUNE2_PAC8_PRF64M,
      DW1000_DRX_TUNE2_PAC16_PRF64M,
      DW1000_DRX_TUNE2_PAC32_PRF64M,
      DW1000_DRX_TUNE2_PAC64_PRF64M },
};

/**
 * @brief LDE副本系数配置（根据接收前导码的不同设置不同参数）
 */
const static uint16_t DW1000_LDE_REPLICA_COEFF[DW1000_SUPPORT_PCODE_NUM] = {
    0, // 无前导码
    DW1000_LDE_REPC_PCODE_1,
    DW1000_LDE_REPC_PCODE_2,
    DW1000_LDE_REPC_PCODE_3,
    DW1000_LDE_REPC_PCODE_4,
    DW1000_LDE_REPC_PCODE_5,
    DW1000_LDE_REPC_PCODE_6,
    DW1000_LDE_REPC_PCODE_7,
    DW1000_LDE_REPC_PCODE_8,
    DW1000_LDE_REPC_PCODE_9,
    DW1000_LDE_REPC_PCODE_10,
    DW1000_LDE_REPC_PCODE_11,
    DW1000_LDE_REPC_PCODE_12,
    DW1000_LDE_REPC_PCODE_13,
    DW1000_LDE_REPC_PCODE_14,
    DW1000_LDE_REPC_PCODE_15,
    DW1000_LDE_REPC_PCODE_16,
    DW1000_LDE_REPC_PCODE_17,
    DW1000_LDE_REPC_PCODE_18,
    DW1000_LDE_REPC_PCODE_19,
    DW1000_LDE_REPC_PCODE_20,
    DW1000_LDE_REPC_PCODE_21,
    DW1000_LDE_REPC_PCODE_22,
    DW1000_LDE_REPC_PCODE_23,
    DW1000_LDE_REPC_PCODE_24
};

/**
 * @brief AGC调谐寄存器配置
 * @note 对于AGC_TUNE1寄存器的值根据PRF频率不同设置不同的值
 *       对于AGC_TUNE2寄存器的值为固定值
 */
const static DW1000_AGC_CFG_t DW1000_AGC_CFG = {
    DW1000_AGC_TUNE2_DEFAULT,
    { DW1000_AGC_TUNE1_PRF16M, DW1000_AGC_TUNE1_PRF64M }
};

/**
 * @brief DW1000非标SFD长度配置（根据数据速率设置不同值）
 */
const static uint8_t DW1000_NSTD_SFD_LEN[DW1000_SUPPORT_DATA_RATE_NUM] = {
    DW1000_USR_SFD_NSTD_110K_LEN,
    DW1000_USR_SFD_NSTD_850K_LEN,
    DW1000_USR_SFD_NSTD_6M8_LEN
};

/* 补丁代码，官方工程自带的，暂时不清楚用途 */
#define DW1000_FCTRL_ACK_REQ_MSK 0x20
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief DW1000 SPI 接口读取数据
 * @param[in] handle  DW1000句柄
 * @param[in] reg  寄存器地址
 * @param[in] offset  基于寄存器的子地址偏移（当寄存器长度大于1字节时）
 * @param[in] buf  数据缓存指针
 * @param[in] len  读取数据长度
 * @return uint8_t 函数执行情况
 *  @arg 0: SPI接口读操作成功
 *  @arg 1: SPI接口读操作失败
 * @note  因为仅供文件内部调用该函数，所以不对输入参数进行检查（用户不应调用该函数）
 */
static uint8_t _DW1000_SPI_Read(DW1000_Handle_t* handle, uint8_t reg, uint16_t offset, uint8_t* buf, uint16_t len) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(reg <= 0x3F);
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(handle->SPI_read != NULL);
    DW1000_ASSERT_PARAM(buf != NULL);
    DW1000_ASSERT_PARAM(len != 0);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 定义头部数组 */
    uint8_t header[3];
    /* 定义头部数组长度 */
    uint8_t headerLength;
    /* 如果偏移为0，即只有寄存器地址，头部字段长度为1字节 */
    if (offset == 0) {
        /* 设置本次spi为读操作且无子地址 */
        header[0] = reg;
        headerLength = 1;
    }
    /* 如果偏移不超过127，也就是用7bit可以表示时，说明不需要扩展子地址 */
    else if (offset <= 0x7F) {
        /* 设置本次spi为读操作且有子地址，但无扩展子地址 */
        header[0] = DW1000_SPI_HAS_SUB_ADDR(reg);
        header[1] = offset;
        headerLength = 2;
    } else {
        /* 设置本次spi为读操作且有子地址，且有扩展子地址 */
        header[0] = DW1000_SPI_HAS_SUB_ADDR(reg);
        header[1] = DW1000_SPI_HAS_EXT_SUB_ADDR(offset);
        header[2] = (offset >> 7);
        headerLength = 3;
    }
    return handle->spi.read(header, headerLength, buf, len);
}

/**
 * @brief DW1000 SPI 接口写入数据
 * @param[in] handle  DW1000句柄
 * @param[in] reg  寄存器地址
 * @param[in] offset  基于寄存器的子地址偏移（当寄存器长度大于1字节时）
 * @param[in] buf  数据缓存指针
 * @param[in] len  写入数据长度
 * @return uint8_t 函数执行情况
 *  @arg 0: SPI接口写操作成功
 *  @arg 2: SPI接口写操作失败
 * @note  因为仅供文件内部调用该函数，所以不对输入参数进行检查（用户不应调用该函数）
 */
static uint8_t _DW1000_SPI_Write(DW1000_Handle_t* handle, uint8_t reg, uint16_t offset, uint8_t* buf, uint16_t len) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(reg <= 0x3F);
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(handle->SPI_write != NULL);
    DW1000_ASSERT_PARAM(buf != NULL);
    DW1000_ASSERT_PARAM(len != 0);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 定义头部数组 */
    uint8_t header[3];
    /* 定义头部数组长度 */
    uint8_t headerLength;
    /* 如果偏移为0，即只有寄存器地址，头部字段长度为1字节 */
    if (offset == 0) {
        /* 设置本次spi为写操作且无子地址 */
        header[0] = DW1000_SPI_CMD_WRITE(reg);
        headerLength = 1;
    }
    /* 如果偏移不超过127，也就是用7bit可以表示时，说明不需要扩展子地址 */
    else if (offset <= 0x7F) {
        /* 设置本次spi为写操作且有子地址，但无扩展子地址 */
        header[0] = DW1000_SPI_HAS_SUB_ADDR(DW1000_SPI_CMD_WRITE(reg));
        header[1] = offset;
        headerLength = 2;
    } else {
        /* 设置本次spi为写操作且有子地址，且有扩展子地址 */
        header[0] = DW1000_SPI_HAS_SUB_ADDR(DW1000_SPI_CMD_WRITE(reg));
        header[1] = DW1000_SPI_HAS_EXT_SUB_ADDR(offset);
        header[2] = (offset >> 7);
        headerLength = 3;
    }
    return handle->spi.write(header, headerLength, buf, len);
}

/**
 * @brief 设置主机spi接口为低速率
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果 0: 设置成功 2: 设置失败
 * @note  对于DW1000某些设置需要SPI速率小于等于3MHz
 */
static inline uint8_t _DW1000_SPI_LowSpeedSet(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle);
    DW1000_ASSERT_PARAM(handle->spi.lowSpeedSet != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return handle->spi.lowSpeedSet();
}

/**
 * @brief 设置主机spi接口为高速率
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果 0: 设置成功 2: 设置失败
 * @note  在通常情况下，SPI速率越高，通信效率也越好
 * @attention SPI通信最高速率不能超过20MHz
 */
static inline uint8_t _DW1000_SPI_HighSpeedSet(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle);
    DW1000_ASSERT_PARAM(handle->spi.highSpeedSet != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return handle->spi.highSpeedSet();
}


/**
 * @brief DW1000进入临界区
 * @param[in] handle  DW1000句柄
 * @note  _DW1000_CriticalEnter 和 _DW1000_CriticalExit
 *        必须成对使用，且顺序不可改变
 *        使用环境为单线程以及启用DW1000外部中断的情况下
 *        对于多线程（RTOS）环境下，需要自行对DW1000进行临界保护
 *        本对函数的作用进行是保证在执行临界区内的代码时的原子操作
 *        如在临界区内读取DW1000的寄存器并修改后写回，这一个过程要
 *        保证完整性，不能没完成又去读取或写入DW1000的其他寄存器
 */
static void _DW1000_CriticalEnter(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(handle->host_irq.enableStatusGet != NULL);
    DW1000_ASSERT_PARAM(handle->host_irq.disable != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 进入临界区前，暂存使能状态便于退出时进行恢复 */
    if (handle->host_irq.enableStatus = handle->host_irq.enableStatusGet()) {
        handle->host_irq.disable();
    }
}

/**
 * @brief DW1000进入临界区
 * @param[in] handle  句柄
 * @note  _DW1000_CriticalEnter 和 _DW1000_CriticalExit
 *        必须成对使用，且顺序不可改变
 *        使用环境为单线程以及启用DW1000外部中断的情况下
 *        对于多线程（RTOS）环境下，需要自行对DW1000进行临界保护
 *        本对函数的作用进行是保证在执行临界区内的代码时的原子操作
 *        如在临界区内读取DW1000的寄存器并修改后写回，这一个过程要
 *        保证完整性，不能没完成又去读取或写入DW1000的其他寄存器
 */
static void _DW1000_CriticalExit(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(handle->host_irq.enable != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 根据进入临界区时暂存的值，恢复IRQ的使能状态 */
    if (handle->host_irq.enableStatus) {
        handle->host_irq.enable();
    }
}

/**
 * @brief 定义DW1000 时钟控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_CLOCK_CMD_ENABLE_ALL_SEQ 0x01
#define DW1000_CLOCK_CMD_FORCE_SYS_XTI  0x02
#define DW1000_CLOCK_CMD_FORCE_SYS_PLL  0x03
#define DW1000_CLOCK_CMD_READ_ACC_ON    0x04
#define DW1000_CLOCK_CMD_READ_ACC_OFF   0x05
#define DW1000_CLOCK_CMD_FORCE_OTP_ON   0x06
#define DW1000_CLOCK_CMD_FORCE_OTP_OFF  0x07
#define DW1000_CLOCK_CMD_FORCE_TX_PLL   0x08
#define DW1000_CLOCK_CMD_FORCE_LDE      0x09

/**
 * @brief DW1000 时钟配置
 * @param[in] handle  DW1000 Handle 结构体指针
 * @param[in] clockCmd  时钟配置命令
 *  @arg DW1000_CLOCK_CMD_ENABLE_ALL_SEQ: 使能所有时钟序列
 *  @arg DW1000_CLOCK_CMD_FORCE_SYS_XTI: 强制系统时钟为XTI
 *  @arg DW1000_CLOCK_CMD_FORCE_SYS_PLL: 强制系统时钟为PLL
 *  @arg DW1000_CLOCK_CMD_READ_ACC_ON: 使能读取ACC时钟
 *  @arg DW1000_CLOCK_CMD_READ_ACC_OFF: 禁用读取ACC时钟
 *  @arg DW1000_CLOCK_CMD_FORCE_OTP_ON: 强制使能OTP时钟
 *  @arg DW1000_CLOCK_CMD_FORCE_OTP_OFF: 强制禁用OTP时钟
 *  @arg DW1000_CLOCK_CMD_FORCE_TX_PLL: 强制发送时钟为PLL
 *  @arg DW1000_CLOCK_CMD_FORCE_LDE: 强制LED时钟
 * @return uint8_t 函数执行情况
 *  @arg 0: 配置成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 * @todo  【原本打算将该函数替换掉，分成几个设置对应时钟的函数，目前仍保留该函数】
 */
uint8_t _DW1000_ClockCmd(DW1000_Handle_t* handle, uint8_t clockCmd) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(clockCmd == DW1000_CLOCK_CMD_ENABLE_ALL_SEQ ||
                        clockCmd == DW1000_CLOCK_CMD_FORCE_SYS_XTI ||
                        clockCmd == DW1000_CLOCK_CMD_FORCE_SYS_PLL ||
                        clockCmd == DW1000_CLOCK_CMD_READ_ACC_ON ||
                        clockCmd == DW1000_CLOCK_CMD_READ_ACC_OFF ||
                        clockCmd == DW1000_CLOCK_CMD_FORCE_OTP_ON ||
                        clockCmd == DW1000_CLOCK_CMD_FORCE_OTP_OFF ||
                        clockCmd == DW1000_CLOCK_CMD_FORCE_TX_PLL ||
                        clockCmd == DW1000_CLOCK_CMD_FORCE_LDE);
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t value;
    /* 关于时钟设置只需要低16位数据，这里就读取这个寄存器的2个字节 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value)),
        return 1);
    switch (clockCmd) {
        case DW1000_CLOCK_CMD_ENABLE_ALL_SEQ:
            DW1000_CLEAR_BITS(value,
                              0xFFUL |
                                  // DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_MSK |
                                  // DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_MSK |
                                  // DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_MSK |
                                  // DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_MSK |
                                  // DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE_MSK |
                                  DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE_MSK);
            break;
        case DW1000_CLOCK_CMD_FORCE_SYS_XTI:
            DW1000_MODIFY_REG(value,
                              DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_MSK,
                              DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI);
            break;
        case DW1000_CLOCK_CMD_FORCE_SYS_PLL:
            DW1000_MODIFY_REG(value,
                              DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_MSK,
                              DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL);
            break;
        case DW1000_CLOCK_CMD_READ_ACC_ON:
            DW1000_MODIFY_REG(value,
                              DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_MSK | DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_MSK,
                              DW1000_ACC_MEM_CLOCK_ENABLE | DW1000_ACC_CLOCK_FORCE_ENABLE | DW1000_RX_CLOCK_SELECT_FORCE_PLL);
            break;
        case DW1000_CLOCK_CMD_READ_ACC_OFF:
            DW1000_CLEAR_BITS(value,
                              DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_MSK | DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_MSK |
                                  DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE_MSK);
            break;
        case DW1000_CLOCK_CMD_FORCE_OTP_ON:
            DW1000_SET_BITS(value, DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE_MSK);
            break;
        case DW1000_CLOCK_CMD_FORCE_OTP_OFF:
            DW1000_CLEAR_BITS(value, DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE_MSK);
            break;
        case DW1000_CLOCK_CMD_FORCE_TX_PLL:
            DW1000_MODIFY_REG(value,
                              DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_MSK,
                              DW1000_TX_CLOCK_SELECT_FORCE_PLL);
            break;
        case DW1000_CLOCK_CMD_FORCE_LDE:
            /* 关于这个0x0301具体含义暂不清楚 */
            DW1000_WRITE_REG(value, 0x0301);
            break;

        default:
            break;
    }
    /* 16位数据需要分两次写入，先写入低字节，再写入高字节 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &value, sizeof(uint8_t)),
        return 2);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1, ((uint8_t*) &value) + 1, sizeof(uint8_t));
}

/**
 * @brief DW1000读取OTP内存
 * @param[in] handle  DW1000句柄
 * @param[in] address  OTP中的数据地址
 * @param[in] data  待读出数据存放的指针
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 * @note  确保 MR,MRa,MRb 被复位为0
 */
uint8_t _DW1000_OTP_Read(DW1000_Handle_t* handle, uint16_t address, uint32_t* data) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(data != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 写入地址 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_ADDR_OFFSET, (uint8_t*) &address, sizeof(address)),
        return 2);
    uint8_t value;
    /* 设置 OTPREAD 和 OTPRDEN 标志位 */
    DW1000_WRITE_REG(value, DW1000_SUB_REG_OTP_CTRL_BIT_OTPREAD | DW1000_SUB_REG_OTP_CTRL_BIT_OTPRDEN);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &value, sizeof(value)),
        return 2);
    /* 清除 OTPREAD 和 OTPRDEN 标志位，OTPREAD实际上会被自动清零，但是OTPRDEN需要手动清零，这里就一块清除 */
    DW1000_WRITE_REG(value, 0x00);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &value, sizeof(value)),
        return 2);
    /* 从OTP_RDAT子寄存器中读取前面设定OTP对应地址里的数据 */
    return _DW1000_SPI_Read(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_RDAT_OFFSET, (uint8_t*) data, sizeof(*data));
}

#define DW1000_OTP_MODE_RESET_ALL            0x00
#define DW1000_OTP_MODE_INIT_PROGRAMMING     0x01
#define DW1000_OTP_MODE_SOAK_PROGRAMMING     0x02
#define DW1000_OTP_MODE_HIGH_VPP             0x03
#define DW1000_OTP_MODE_LOW_READ_MARGIN      0x04
#define DW1000_OTP_MODE_ARRAY_CLEAN          0x05
#define DW1000_OTP_MODE_VERY_LOW_READ_MARGIN 0x06


/**
 * @brief DW1000配置 MR寄存器以进行
 * @param[in] handle  Comment
 * @param[in] mode  模式选择，取值如下
 *              DW1000_OTP_MODE_RESET_ALL
 *              DW1000_OTP_MODE_INIT_PROGRAMMING
 *              DW1000_OTP_MODE_SOAK_PROGRAMMING
 *              DW1000_OTP_MODE_HIGH_VPP
 *              DW1000_OTP_MODE_LOW_READ_MARGIN
 *              DW1000_OTP_MODE_ARRAY_CLEAN
 *              DW1000_OTP_MODE_VERY_LOW_READ_MARGIN
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 *  @arg 3: 输入参数 mode 有误
 * @note  配置MR寄存器以进行初始编程（启用电荷泵）。
 *        读取裕量用于强调从已编程位回读的读取操作。
 *        在正常操作中，这一要求会放宽。
 *        具体说明可以参考 用户手册 的 6.3.2节
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t _DW1000_OTP_MR_RegSet(DW1000_Handle_t* handle, uint8_t mode) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(mode == DW1000_OTP_MODE_RESET_ALL ||
                        mode == DW1000_OTP_MODE_INIT_PROGRAMMING ||
                        mode == DW1000_OTP_MODE_SOAK_PROGRAMMING ||
                        mode == DW1000_OTP_MODE_HIGH_VPP ||
                        mode == DW1000_OTP_MODE_LOW_READ_MARGIN ||
                        mode == DW1000_OTP_MODE_ARRAY_CLEAN ||
                        mode == DW1000_OTP_MODE_VERY_LOW_READ_MARGIN);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t result;
    uint16_t OTP_ctrl;
    uint16_t OTP_data;
    uint16_t mr = 0;
    uint16_t mra = 0;
    uint16_t mrb = 0;

    switch (mode) {
        case DW1000_OTP_MODE_RESET_ALL:
            mr = 0x0000;
            mra = 0x0000;
            mrb = 0x0000;
            break;
        case DW1000_OTP_MODE_INIT_PROGRAMMING:
            mr = 0x1024;
            mra = 0x9220;
            mrb = 0x000E;
            break;
        case DW1000_OTP_MODE_SOAK_PROGRAMMING:
            mr = 0x1824;
            mra = 0x9220;
            mrb = 0x0003;
            break;
        case DW1000_OTP_MODE_HIGH_VPP:
            mr = 0x1824;
            mra = 0x9220;
            mrb = 0x004E;
            break;
        case DW1000_OTP_MODE_LOW_READ_MARGIN:
            mr = 0x0000;
            mra = 0x0000;
            mrb = 0x0003;
            break;
        case DW1000_OTP_MODE_ARRAY_CLEAN:
            mr = 0x0024;
            mra = 0x0000;
            mrb = 0x0003;
            break;
        case DW1000_OTP_MODE_VERY_LOW_READ_MARGIN:
            mr = 0x0000;
            mra = 0x0000;
            mrb = 0x0003;
            break;
        default:
            return 3;
            break;
    }
    /* 编程 MRA, */
    /* MODE_SEL 设置 MRA */
    // OTP_ctrl = 0x0300;
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_MODE_REGISTER_MRA | DW1000_OTP_MODE_SELECT_SET);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1),
        return 2);
    /* 将mra值写入 */
    // OTP_data = mra;
    DW1000_WRITE_REG(OTP_data, mra);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_WDAT_OFFSET, (uint8_t*) &OTP_data, sizeof(OTP_data)),
        return 2);
    /* 设置 OTPMRWR 位，以将前面的配置写入OTP Mode寄存器中 */
    // OTP_ctrl |= 0x08; //OTP_ctrl = 0x0308
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 等待2ms (手册上写的是1ms，但官方的库代码是2ms) */
    handle->delay_ms(2);
    /* 清除 MODE_SEL 位 */
    // OTP_ctrl &= ~0x0100; // OTP_ctrl = 0x0208
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1),
        return 2);
    /* 设置 AUX Update 位 和 OTPMRWR 位 */
    // OPT_ctrl |= 0x88; // OTP_ctrl = 0x0288;
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK | DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 清除 OTPMRWR 位 */
    // OTP_ctrl &= ~0x08; // OTP_ctrl = 0x0280;
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 清除 AUX Update 位 */
    // OTP_ctrl &= ~0x80; // OTP_ctrl = 0x0200;
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 编程 MRB */
    /* MODE_SEL 设置 SLOW MRB  */
    // OTP_ctrl = 0x0500;
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_MODE_REGISTER_MRB | DW1000_OTP_MODE_SELECT_SET);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1),
        return 2);
    // OTP_data = mrb;
    DW1000_WRITE_REG(OTP_data, mrb);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_WDAT_OFFSET, (uint8_t*) &OTP_data, sizeof(OTP_data)),
        return 2);
    /* 设置 OTPMRWR 位 */
    // OTP_ctrl |= 0x08; // OTP_ctrl = 0x0508
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 等待2ms (手册上写的是1ms，但官方的库代码是2ms) */
    handle->delay_ms(2);
    /* 清除 MODE_SEL 位 */
    // OTP_ctrl &= ~0x0100; // OTP_ctrl = 0x0408
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1),
        return 2);
    /* 设置AUX Update 位 和 OTPMRWR 位 */
    // OPT_ctrl |= 0x88; // OTP_ctrl = 0x0488;
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK | DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 清除 OTPMRWR 位 */
    // OTP_ctrl &= ~0x08; // OTP_ctrl = 0x0480;
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 清除 AUX Update 位 */
    // OTP_ctrl &= ~0x80; // OTP_ctrl = 0x0400;
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 编程 MR */
    /* MODE_SEL 设置 SLOW MR */
    // OTP_ctrl = 0x0100;
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_MODE_REGISTER_MR | DW1000_OTP_MODE_SELECT_SET);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1),
        return 2);
    // OTP_data = mr;
    DW1000_WRITE_REG(OTP_data, mr);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_WDAT_OFFSET, (uint8_t*) &OTP_data, sizeof(OTP_data)),
        return 2);
    /* 设置 OTPMRWR 位 */
    // OTP_ctrl |= 0x08; // OTP_ctrl = 0x0108
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 等待2ms (手册上写的是1ms，但官方的库代码是2ms) */
    handle->delay_ms(2);
    /* 清除 MODE_SEL 位 */
    // OTP_ctrl &= ~0x0100; // OTP_ctrl = 0x0008
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1),
        return 2);
    return 0;
}

uint8_t _DW1000_OTP_Word32Program(DW1000_Handle_t* handle, uint32_t data, uint16_t address) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_WDAT_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &address, sizeof(address)),
        return 2);
    uint16_t OTP_ctrl;
    /* 设置 OTPPROG 位 */
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_PROGRAM_SET);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    /* 清除 OTPPROG 位 */
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_PROGRAM_CLEAR);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1),
        return 2);
    uint8_t OTP_stat;
    do {
        handle->delay_ms(1);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_STAT_OFFSET, &OTP_stat, sizeof(OTP_stat)),
            return 1);
    } while (DW1000_IS_OTP_PROGRAMMING_DONE(OTP_stat) == 0);
    return 0;
}

uint8_t _DW1000_AON_ConfigurationUpload(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t data;
    DW1000_WRITE_REG(data, DW1000_AON_CTRL_UPLOAD_CONFIGURATION_SET);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 清空寄存器的值 */
    DW1000_WRITE_REG(data, 0x00);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    return 0;
}

uint8_t _DW1000_AON_ArrayUpload(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t data;
    /* 清空寄存器的值 */
    DW1000_WRITE_REG(data, 0x00);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    DW1000_WRITE_REG(data, DW1000_AON_CTRL_SAVE_SET);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    return 0;
}

/**
 * @brief DW1000 LDE配置
 * @param[in] handle  Comment
 * @param[in] config  LDE调谐配置参数
 *  @arg DW1000_LDE_TUNE_PARAM_IN_PRF16MHz: 16MHz PRF下的LDE调谐配置参数
 *  @arg DW1000_LDE_TUNE_PARAM_IN_PRF64MHz: 64MHz PRF下的LDE调谐配置参数
 * @return uint8_t 函数执行情况
 *  @arg 0: 配置成功
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t _DW1000_LDE_Config(DW1000_Handle_t* handle, uint16_t config) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(config == DW1000_LDE_TUNE_PARAM_IN_PRF16MHz ||
                        config == DW1000_LDE_TUNE_PARAM_IN_PRF64MHz);
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t data = 0x00;
    /* 配置DW1000 LDE配置 */
    DW1000_WRITE_REG(data, DW1000_LDE_PARAM1);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_LDE_CFG1_OFFSET, (uint8_t*) &data, 1),
        return 2);
    /* 按照config设置LDE调谐配置 */
    DW1000_WRITE_REG(data, config);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_LDE_CFG2_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    return 0;
}

uint8_t _DW1000_LoadUcodeFromROM(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t data;
    /* 强制使能LDE时钟 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_LDE),
        return 3);
    /* 设置LDE微码加载 */
    DW1000_WRITE_REG(data, DW1000_LDE_LOAD_UCODE_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 等待微码加载到内存（大概需要120us） */
    handle->delay_ms(1);
    /* 恢复默认时钟（ENABLE_ALL_SEQ） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ),
        return 3);
    return 0;
}


uint8_t _DW1000_SequeningDisable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
/* 设置系统时钟为XTI */
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t data;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI),
        return 3);
    /* 禁用PMSC控制里的RF 和 RX时钟块 */
    DW1000_WRITE_REG(data, DW1000_PMSC_CTRL_RF_SUB_SYS_DISABLE);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &data, sizeof(data));
}

#define DW1000_SYSTEM_CLOCK_SELECTION_AUTO 0x00
#define DW1000_SYSTEM_CLOCK_SELECTION_XTI  0x01
#define DW1000_SYSTEM_CLOCK_SELECTION_PLL  0x02

/**
 * @brief DW1000 系统时钟选择
 * @param[in] handle  DW1000 Handle 结构体指针
 * @param[in] clockSelect  系统时钟选择
 *  @arg DW1000_SYSTEM_CLOCK_SELECT_AUTO: 自动选择系统时钟
 *  @arg DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI: 强制选择XTI作为系统时钟
 *  @arg DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL: 强制选择PLL作为系统时钟
 * @return uint8_t 函数执行情况
 *  @arg 0: 配置成功
 *  @arg 其他: 配置失败
 * @note  备注
 */
uint8_t _DW1000_SystemClockSelect(DW1000_Handle_t* handle, uint8_t clock) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(clock == DW1000_SYSTEM_CLOCK_SELECT_AUTO ||
                        clock == DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI ||
                        clock == DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t value;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value)),
        return 1);
    DW1000_MODIFY_REG(value, DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_MSK, clock);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value));
}

#define DW1000_RX_CLOCK_SELECTION_AUTO 0x00
#define DW1000_RX_CLOCK_SELECTION_XTI  0x01
#define DW1000_RX_CLOCK_SELECTION_PLL  0x02
#define DW1000_RX_CLOCK_SELECTION_OFF  0x03

/**
 * @brief
 * @param[in] handle  Comment
 * @param[in] clock  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t _DW1000_RxClockSelect(DW1000_Handle_t* handle, uint8_t clock) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(clock == DW1000_RX_CLOCK_SELECTION_AUTO ||
                        clock == DW1000_RX_CLOCK_SELECTION_XTI ||
                        clock == DW1000_RX_CLOCK_SELECTION_PLL ||
                        clock == DW1000_RX_CLOCK_SELECTION_OFF);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t value;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value)),
        return 1);
    DW1000_MODIFY_REG(value, DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_MSK, clock);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value));
}

#define DW1000_TX_CLOCK_SELECTION_AUTO 0x00
#define DW1000_TX_CLOCK_SELECTION_XTI  0x01
#define DW1000_TX_CLOCK_SELECTION_PLL  0x02
#define DW1000_TX_CLOCK_SELECTION_OFF  0x03

/**
 * @brief
 * @param[in] handle  Comment
 * @param[in] clock  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t _DW1000_TxClockSelect(DW1000_Handle_t* handle, uint8_t clock) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(clock == DW1000_TX_CLOCK_SELECTION_AUTO ||
                        clock == DW1000_TX_CLOCK_SELECTION_XTI ||
                        clock == DW1000_TX_CLOCK_SELECTION_PLL ||
                        clock == DW1000_TX_CLOCK_SELECTION_OFF);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t value;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value)),
        return 1);
    DW1000_MODIFY_REG(value, DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_MSK, clock);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value));
}

#define DW1000_ADC_CLOCK_ENABLE  0x01
#define DW1000_ADC_CLOCK_DISABLE 0x00

uint8_t _DW1000_ADC_ClockCmd(DW1000_Handle_t* handle, uint8_t cmd) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(cmd == DW1000_ADC_CLOCK_ENABLE || cmd == DW1000_ADC_CLOCK_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t value;
    /* 这里为了方便只读取8位数据，并只对ADC时钟使能位进行操作 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1, &value, sizeof(value)),
        return 1);
    DW1000_MODIFY_REG(value, (0x1 << (DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE_POS - 8)), cmd);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1, &value, sizeof(value));
}

#define DW1000_ACC_CLOCK_ENABLE  0x01
#define DW1000_ACC_CLOCK_DISABLE 0x00

uint8_t _DW1000_ACC_ClockCmd(DW1000_Handle_t* handle, uint8_t cmd) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(cmd == DW1000_ACC_CLOCK_ENABLE || cmd == DW1000_ACC_CLOCK_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t value;
    /* 这里为了方便只读取8位数据，并只对ACC时钟使能位进行操作 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value)),
        return 1);
    DW1000_MODIFY_REG(value, DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_MSK, cmd);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value));
}

#define DW1000_ACC_MEM_CLOCK_ENABLE  0x01
#define DW1000_ACC_MEM_CLOCK_DISABLE 0x00

uint8_t _DW1000_ACC_MEM_ClockCmd(DW1000_Handle_t* handle, uint8_t cmd) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(cmd == DW1000_ACC_MEM_CLOCK_ENABLE || cmd == DW1000_ACC_MEM_CLOCK_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t value;
    /* 这里为了方便只读取8位数据，并只对ACC MEM时钟使能位进行操作 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1, &value, sizeof(value)),
        return 1);
    DW1000_MODIFY_REG(value, (0x1 << (DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE_POS - 8)), cmd);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1, &value, sizeof(value));
}


#define DW1000_OTP_CLOCK_ENABLE  0x01
#define DW1000_OTP_CLOCK_DISABLE 0x00

uint8_t _DW1000_OTP_ClockCmd(DW1000_Handle_t* handle, uint8_t cmd) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(cmd == DW1000_OTP_CLOCK_ENABLE || cmd == DW1000_OTP_CLOCK_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t value;
    /* 这里为了方便只读取8位数据，并只对ACC MEM时钟使能位进行操作 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1, &value, sizeof(value)),
        return 1);
    DW1000_MODIFY_REG(value, (0x1 << (DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE_POS - 8)), cmd);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1, &value, sizeof(value));
}

/* 该函数由于《用户手册》没有详细说明该位，暂时留下该函数占位 */
uint8_t _DW1000_LDE_ClockCmd(DW1000_Handle_t* handle, uint8_t cmd) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(cmd == 0); // 【待完善】
#endif                             /* USE_DW1000_FULL_ASSERT */
    // uint8_t value;
    // /* 这里为了方便只读取8位数据，并只对LDE时钟使能位进行操作 */
    // if (_DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1, &value, sizeof(value))) {
    //     return 1;
    // }
    // DW1000_MODIFY_REG(value, (0x1 << (DW1000_SUB_REG_PMSC_CTRL0_BIT_LDECE_POS - 8)), cmd);
    // return _DW1000_SPI_Write(handle,
    //                          DW1000_REG_PMSC,
    //                          DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1,
    //                          &value,
    //                          sizeof(value));
}


/**
 * @brief 定义DW1000 精细粒度的Tx功率序列控制值
 * @note 默认（复位后）使用 DW1000_TX_FINE_GRAIN_SEQ_ENABLE
 * @note 该参数位于寄存器 PMSC - 0x36
 *       子寄存器 PMSC_TXFSEQ - 0x26
 * @param DW1000_TX_FINE_GRAIN_SEQ_ENABLE: 使能精细粒度的Tx功率序列控制
 * @param DW1000_TX_FINE_GRAIN_SEQ_DISABLE: 禁用精细粒度的Tx功率序列控制
 */
#define DW1000_TX_FINE_GRAIN_SEQ_ENABLE  0x0B74
#define DW1000_TX_FINE_GRAIN_SEQ_DISABLE 0x0000


/**
 * @brief DW1000 精细粒度的Tx功率序列控制
 * @param[in] handle  dwt handle 结构体指针
 * @param[in] cmd  配置命令
 *  @arg DW1000_TX_FINE_GRAIN_SEQ_ENABLE: 启用精细粒度的Tx功率序列控制
 *  @arg DW1000_TX_FINE_GRAIN_SEQ_DISABLE: 禁用精细粒度的Tx功率序列控制
 * @return uint8_t 函数执行结果
 *  @arg 0  配置成功
 *  @arg 1  配置失败
 * @note  dw1000默认使能精细粒度的Tx序列
 */
inline uint8_t DW1000_FineGrainTxSeqCmd(DW1000_Handle_t* handle, uint16_t cmd) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(cmd == DW1000_TX_FINE_GRAIN_SEQ_ENABLE || cmd == DW1000_TX_FINE_GRAIN_SEQ_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_TXFSEQ_OFFSET, &cmd, sizeof(cmd));
}


#define DW1000_LNA_PA_MODE_NONE    0x00000000
#define DW1000_LNA_PA_MODE_LNA     DW1000_GPIO_MODE_P6_EXTRXE
#define DW1000_LNA_PA_MODE_PA      (DW1000_GPIO_MODE_P4_EXTPA | DW1000_GPIO_MODE_P5_EXTTXE)
#define DW1000_LNA_PA_MODE_ALL     (DW1000_LNA_PA_MODE_LNA | DW1000_LNA_PA_MODE_PA)
#define DW1000_LNA_PA_MODE_DEFAULT DW1000_LNA_PA_MODE_NONE

/**
 * @brief DW1000 设置 LNA 和 PA 模式
 * @param[in] handle  dw1000 handle 结构体指针
 * @param[in] mode  模式
 *  @arg DW1000_LNA_PA_MODE_NONE    LNA和PA均禁用（默认模式）
 *  @arg DW1000_LNA_PA_MODE_LNA     启用LNA
 *  @arg DW1000_LNA_PA_MODE_PA      启用PA
 *  @arg DW1000_LNA_PA_MODE_ALL     启用LNA和PA
 * @return uint8_t 函数执行结果
 *  @arg 0  设置成功
 *  @arg 1  设置失败
 * @attention  使能PA功能需要失能精细粒度的Tx序列控制功能
 *             需先调用 dw1000_fine_grain_tx_seq_config 进行配置
 */
uint8_t DW1000_LNA_PA_ModeSet(DW1000_Handle_t* handle, uint32_t mode) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(
        mode == DW1000_LNA_PA_MODE_NONE ||
        mode == DW1000_LNA_PA_MODE_LNA ||
        mode == DW1000_LNA_PA_MODE_PA ||
        mode == DW1000_LNA_PA_MODE_ALL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 先读取GPIO控制状态寄存器的子寄存器GPIO Mode的内容 */
    uint32_t GPIO_mode;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_GPIO_CTRL, DW1000_SUB_REG_GPIO_MODE_OFFSET, (uint8_t*) &GPIO_mode, sizeof(GPIO_mode)),
        return 1);
    /* 根据 mode 参数进行配置 */
    DW1000_MODIFY_REG(GPIO_mode, DW1000_LNA_PA_MODE_MASK, mode);
    return _DW1000_SPI_Write(handle, DW1000_REG_GPIO_CTRL, DW1000_SUB_REG_GPIO_MODE_OFFSET, (uint8_t*) &GPIO_mode, sizeof(GPIO_mode));
}


/**
 * @brief
 * @param[in] handle  Comment
 * @param[in] pin  取值0-7，表示GPIO引脚编号
 * @param[in] mode  取值0-3，表示GPIO引脚模式
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_GPIO_PinModeSelect(DW1000_Handle_t* handle, uint8_t pin, uint8_t mode) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(pin >= 0 && pin <= 7);
    DW1000_ASSERT_PARAM(mode >= 0 && mode <= 3);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 先读取GPIO控制状态寄存器的子寄存器GPIO Mode的内容 */
    uint32_t GPIO_mode;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_GPIO_CTRL, DW1000_SUB_REG_GPIO_MODE_OFFSET, (uint8_t*) &GPIO_mode, sizeof(GPIO_mode)),
        return 1);
    /* 根据 mode 参数进行配置 */
    DW1000_MODIFY_REG(GPIO_mode, DW1000_GPIO_MODE_PIN_MASK(pin), DW1000_GPIO_MODE_SELECT(pin, mode));
    return _DW1000_SPI_Write(handle, DW1000_REG_GPIO_CTRL, DW1000_SUB_REG_GPIO_MODE_OFFSET, (uint8_t*) &GPIO_mode, sizeof(GPIO_mode));
}

/**
 * @brief DW1000 使能GPIO时钟
 * @param[in] handle  dw1000 handle 结构体指针
 * @return uint8_t 函数执行结果
 *  @arg 0  设置成功
 *  @arg 1  设置失败
 *  @arg 2  handle 为空
 *  @arg 3  handle 未初始化
 * @note  备注
 */
uint8_t DW1000_GPIO_ClockEnable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t PMSC_clockCtrl;
    /* 先读取PMSC寄存器的子寄存器CTRL0的内容 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &PMSC_clockCtrl, sizeof(PMSC_clockCtrl)),
        return 1);
    /* GPRN置位使GPIO端口退出复位状态，然后GPCE置位开启GPIO时钟 */
    DW1000_SET_BITS(PMSC_clockCtrl, DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE | DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &PMSC_clockCtrl, sizeof(PMSC_clockCtrl));
}

/**
 * @brief DW1000设置GPIO的方向
 * @param[in] handle  dw1000 handle 结构体指针
 * @param[in] gpio gpio配置
 *  可以使用 | 运算组合使用以下参数（参数可在 driver_dw1000.h 文件里查询）：
 *  @arg DW1000_GPIO_DIRECTION_OUTPUT_Px: 编号x可以取0-8，表示该编号的引脚为输出
 *  @arg DW1000_GPIO_DIRECTION_INPUT_Px: 编号x可以取0-8，表示该编号的引脚为输入
 * @return uint8_t 函数执行结果
 *  @arg 0  设置成功
 *  @arg 1  设置失败
 *  @arg 2  handle 为空
 *  @arg 3  handle 未初始化
 * @note  因为函数体内部只是调用其他函数，使用inline关键字建议编译器对其进行优化
 */
inline uint8_t DW1000_GPIO_DirectionSet(DW1000_Handle_t* handle, uint32_t direction) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_GPIO_CTRL, DW1000_SUB_REG_GPIO_DIR_OFFSET, (uint8_t*) &direction, sizeof(direction));
}

/**
 * @brief DW1000设置GPIO的输出电平
 * @param[in] handle  dw1000 handle 结构体指针
 * @param[in] out  输出电平
 * 可以使用 | 运算组合使用以下参数（参数可在 driver_dw1000.h 文件里查询）：
 *  @arg DW1000_GPIO_OUTPUT_STATE_LOW_Px: 编号x引脚输出低电平，x取值0-8
 *  @arg DW1000_GPIO_OUTPUT_STATE_HIGH_Px: 编号x引脚输出高电平，x取值0-8
 * @return uint8_t 函数执行结果
 *  @arg 0  设置成功
 *  @arg 1  设置失败
 *  @arg 2  handle 为空
 *  @arg 3  handle 未初始化
 * @note  因为函数体内部只是调用其他函数，使用inline关键字建议编译器对其进行优化
 * @attention  只有对应的GPIO引脚配置为输出时，设置才会生效
 */
inline uint8_t DW1000_GPIO_ValueSet(DW1000_Handle_t* handle, uint32_t out) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_GPIO_CTRL, DW1000_SUB_REG_GPIO_DOUT_OFFSET, (uint8_t*) &out, sizeof(out));
}

/**
 * @brief 获取GPIO引脚的当前电平状态
 * @param[in] handle  dw1000 handle 结构体指针
 * @param[in] gpio  GPIO引脚掩码
 *  @arg DW1000_GPIO_RAW_STATE_Px: x取值0-8
 * @param[out] value  GPIO引脚的电平状态，1表示高电平，0表示低电平
 * @return uint8_t 函数执行结果
 *  @arg 0  设置成功
 *  @arg 1  设置失败
 *  @arg 2  handle 为空
 *  @arg 3  handle 未初始化
 * @note  备注
 */
uint8_t DW1000_GPIO_ValueGet(DW1000_Handle_t* handle, uint32_t gpio, uint8_t* value) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t GPIO_value;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_GPIO_CTRL, DW1000_SUB_REG_GPIO_RAW_OFFSET, (uint8_t*) &GPIO_value, sizeof(GPIO_value)),
        return 1);
    *value = (GPIO_value & gpio) ? 1 : 0;
    return 0;
}

/**
 * @brief
 * @param[in] handle  Comment
 * @param[in] volt  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 需要增加一个DW1000_IC_RefVoltCopyGet()函数，用于区分读取的是备份的值还是实际寄存器中的值
 */
inline uint8_t DW1000_IC_RefVoltGet(DW1000_Handle_t* handle, uint8_t* volt) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(volt != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    *volt = handle->backup.refVolt;
    return 0;
}

inline uint8_t DW1000_IC_RefTempGet(DW1000_Handle_t* handle, uint8_t* temp) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(temp != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    *temp = handle->backup.refTemp;
    return 0;
}

inline uint8_t DW1000_partIdGet(DW1000_Handle_t* handle, uint32_t* id) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(id != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    *id = handle->backup.partID;
    return 0;
}

inline uint8_t DW1000_lotIdGet(DW1000_Handle_t* handle, uint32_t* id) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(id != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    *id = handle->backup.lotID;
    return 0;
}

/**
 * @brief DW1000获取设备ID
 * @param[in] handle  DW1000 handle 结构体指针
 * @param[in] id  设备ID，读取正确的话，结果应为 0xDECA0130
 * @return uint8_t 函数执行结果
 *  @arg 0  设置成功
 *  @arg 1  设置失败
 *  @arg 2  handle 为空
 *  @arg 3  handle 未初始化
 * @note  因为函数体内部只是调用其他函数，使用inline关键字建议编译器对其进行优化
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
inline uint8_t DW1000_DEV_ID_Get(DW1000_Handle_t* handle, uint32_t* id) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(id != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_DEV_ID, 0, (uint8_t*) id, sizeof(*id));
}

uint8_t DW1000_TX_RF_Config(DW1000_Handle_t* handle, DW1000_TX_Config_t* config) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(config != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /** @todo 将结构体的变量名缩短 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PGDELAY_OFFSET, (uint8_t*) &config->pulseGeneratorDelay, sizeof(config->pulseGeneratorDelay)),
        return 2);
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_POWER, 0, (uint8_t*) &config->TX_power, sizeof(config->TX_power));
}


#define DEMOD_GEAR_64L 0x47
/**
 * @brief DW1000 配置64长度前导码时的优化参数
 * @param[in] handle  DW1000 handle 结构体指针
 * @param[in] value  优化参数
 *  @arg DW1000_DRX_TUN2_UNCONF_SFD_TH_PRF16: 当PRF设置为16MHz时使用该参数
 *  @arg DW1000_DRX_TUN2_UNCONF_SFD_TH_PRF64: 当PRF设置为64MHz时使用该参数
 * @return uint8_t 函数执行结果
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_PreambleLength64Config(DW1000_Handle_t* handle, uint32_t value) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t reg_value = DEMOD_GEAR_64L;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_CRTR, DW1000_SUB_REG_CRTR_GEAR_OFFSET, &reg_value, 1),
        return 2);
    /** @todo 需要修改*/
    return _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF,
                             DW1000_SUB_REG_DRX_TUNE2_OFFSET + 2, /* 只修改子寄存器的第3个字节数据 */
                             (uint8_t*) &value, sizeof(value));
}

inline uint8_t DW1000_RX_AntennaDelaySet(DW1000_Handle_t* handle, uint16_t delay) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_LDE_RXANTD_OFFSET, (uint8_t*) &delay, sizeof(delay));
}


inline uint8_t DW1000_TX_AntennaDelaySet(DW1000_Handle_t* handle, uint16_t delay) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_ANTD, 0, (uint8_t*) &delay, sizeof(delay));
}

/**
 * @brief DW1000 写入待发送数据到TX缓冲区
 * @param[in] handle  DW1000 handle 结构体指针
 * @param[in] data  待写入的数据指针
 * @param[in] len  数据长度（默认包含2字节的CRC，因此传入的len值应大于等于2）
 * @param[in] offset  写入偏移量
 * @return uint8_t  函数执行结果
 * @arg 0  写入成功
 * @arg 1  写入失败
 * @arg 2  handle 或 data 为空
 * @arg 3  handle 未初始化
 * @arg 4  写入数据超出缓冲区大小
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
inline uint8_t DW1000_TX_DataWrite(DW1000_Handle_t* handle, uint8_t* data, uint16_t len, uint16_t offset) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(data != NULL);
    DW1000_ASSERT_PARAM(len + offset <= 1024);
#endif                                                                             /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_BUFFER, offset, data, len - 2); // 减去2字节的CRC
}

/**
 * @brief
 * @param[in] handle  Comment
 * @param[in] len  Comment
 * @param[in] offset  Comment
 * @param[in] isRanging  1: 测距帧 0: 非测距帧
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_TX_FrameCtrlWrite(DW1000_Handle_t* handle, uint16_t len, uint16_t offset, uint8_t isRanging) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(len + offset <= 1024);
    DW1000_ASSERT_PARAM(isRanging == 0 || isRanging == 1);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t txFctrl = handle->backup.txFctrl;
    /* 设置发送帧控制寄存器的值 */
    /* 设置帧长度 */
    DW1000_MODIFY_REG(txFctrl, DW1000_REG_TX_FCTRL_BITS_TFLEN_MSK, len); // 因为len字段在最低位置，所以不需要移位，所以这里直接赋值
    /* 设置发送缓冲区索引偏移 */
    DW1000_MODIFY_REG(txFctrl, DW1000_REG_TX_FCTRL_BITS_TXBOFFS_MSK, (uint32_t) offset << DW1000_REG_TX_FCTRL_BITS_TXBOFFS_POS);
    /* 设置Ranging位 */
    DW1000_MODIFY_REG(txFctrl, DW1000_REG_TX_FCTRL_BIT_TR_MSK, (uint32_t) isRanging << DW1000_REG_TX_FCTRL_BIT_TR_POS);
    // if (isRanging) {
    //     DW1000_SET_BITS(txFctrl, DW1000_REG_TX_FCTRL_BIT_TR_MSK);
    // } else {
    //     DW1000_CLEAR_BITS(txFctrl, DW1000_REG_TX_FCTRL_BIT_TR_MSK);
    // }
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_FCTRL, 0, (uint8_t*) &txFctrl, sizeof(txFctrl));
}


inline uint8_t DW1000_RX_DataRead(DW1000_Handle_t* handle, uint8_t* data, uint16_t len, uint16_t offset) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(data != NULL);
    DW1000_ASSERT_PARAM(len + offset <= 1024);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_RX_BUFFER, offset, data, len);
}

uint8_t DW1000_ACC_DataRead(DW1000_Handle_t* handle, uint8_t* data, uint16_t len, uint16_t offset) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(data != NULL);
    DW1000_ASSERT_PARAM(len + offset <= 1024));
#endif /* USE_DW1000_FULL_ASSERT */
    /* 强制开启ACC时钟如果我们正在sequenced */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_READ_ACC_ON),
        return 3);
    /** @todo 以后打算替换成这些代码，前提需要验证，可能有损坏芯片的风险【谨慎】 */
    // if (result = _DW1000_ACC_ClockCmd(handle, DW1000_ACC_CLOCK_ENABLE)){
    //     return result;
    // }
    // if (result = _DW1000_ACC_MEM_ClockCmd(handle,DW1000_ACC_MEM_CLOCK_ENABLE)){
    //     return result;
    // }
    // if (result = _DW1000_RxClockSelect(handle, DW1000_RX_CLOCK_SELECTION_PLL)){
    //     return result;
    // }
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_ACC_MEM, offset, data, len),
        return 1);
    /* 恢复之前ACC的时钟 */
    return _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_READ_ACC_OFF);
    /** @todo 以后打算替换成这些代码，前提需要验证，可能有损坏芯片的风险【谨慎】 */
    // if (result = _DW1000_RxClockSelect(handle, DW1000_RX_CLOCK_SELECTION_AUTO)){
    //     return result;
    // }
    // if (result = _DW1000_ACC_MEM_ClockCmd(handle, DW1000_ACC_MEM_CLOCK_DISABLE)) {
    //     return result;
    // }
    // if (result = _DW1000_ACC_ClockCmd(handle, DW1000_ACC_CLOCK_DISABLE)) {
    //     return result;
    // }
}

#define B20_SIGN_EXTEND_TEST (0x00100000UL)
#define B20_SIGN_EXTEND_MASK (0xFFF00000UL)

/**
 * @brief DW1000读取载波恢复积分值
 * @param[in] handle  Comment
 * @param[in] carrierIntegrator  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_CarrierIntegratorRead(DW1000_Handle_t* handle, int32_t* carrierIntegrator) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle !-NULL);
    DW1000_ASSERT_PARAM(carrierIntegrator != NULL);
#endif                       /* USE_DW1000_FULL_ASSERT */
    uint8_t buff[4] = { 0 }; /* 初始化为0，避免干扰后续判断 */
    uint32_t* value = (uint32_t*) buff;
    /* 将 DRX_CAR_INT 寄存器数据读入buff中（共3个字节） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_CAR_INT_OFFSET, buff, DW1000_SUB_REG_DRX_CAR_INT_LEN),
        return 1);
    /* 根据第20位（符号位）进行扩展，将21位数据扩展为32位数据 */
    /* 如果是负数，将21位以上的高位都设置为1 */
    if (*value & B20_SIGN_EXTEND_TEST) {
        DW1000_SET_BITS(*value, B20_SIGN_EXTEND_MASK);
    }
    /* 如果是正数，将21位以上的高位都设置为0 */
    else {
        DW1000_CLEAR_BITS(*value, B20_SIGN_EXTEND_MASK);
    }
    /* 将处理后的结果赋值给carrierIntegrator */
    *carrierIntegrator = (int32_t) (*value);
    return 0;
}

/**
 * @brief
 * @param[in] handle  DW1000句柄
 * @param[in] diag  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 可以直接开一个 DW1000_RX_DIAG_t 就不需要进行指针的转换了
 *        把这个结构体集中到DW1000句柄里
 */
uint8_t DW1000_DiagnosticsRead(DW1000_Handle_t* handle, DW1000_RX_DIAG_t* diag) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(diag != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t buff[8] = { 0 };
    void* ptr = buff;
    /* 读取 FP 索引 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_FP_INDEX_OFFSET, buff, DW1000_SUB_REG_FP_INDEX_LEN),
        return 1);
    diag->firstPathIndex = (uint16_t) (*((uint16_t*) ptr));
    /* LED 诊断信息 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_LDE_THRESH_OFFSET, buff, DW1000_SUB_REG_LDE_THRESH_LEN),
        return 1);
    diag->maxNoise = (uint16_t) (*((uint16_t*) ptr));
    /* 一次性读取8个字节数据 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, 0, buff, sizeof(buff)),
        return 1);
    diag->standardDeviationNoise = (uint16_t) (*((uint16_t*) ptr));
    diag->firstPathAmplitude2 = (uint16_t) (*((uint16_t*) ptr + 1));
    diag->firstPathAmplitude3 = (uint16_t) (*((uint16_t*) ptr + 2));
    diag->CIR_maxGrowth = (uint16_t) (*((uint16_t*) ptr + 3));
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_FP_AMPL1_OFFSET, buff, DW1000_SUB_REG_FP_AMPL1_LEN),
        return 1);
    diag->firstPathAmplitude1 = (uint16_t) (*((uint16_t*) ptr));
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_RX_FINFO, 0, buff, DW1000_REG_RX_FINFO_LEN),
        return 1);
    diag->RX_preambleCount = ((*((uint32_t*) ptr)) & DW1000_REG_RX_FINFO_BITS_RXPACC_MSK) >> DW1000_REG_RX_FINFO_BITS_RXPACC_POS;
    return 0;
}

inline uint8_t DW1000_TX_TimeStampRead(DW1000_Handle_t* handle, void* timeStamp) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(timeStamp != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_TX_TIME, DW1000_SUB_REG_TX_STAMP_OFFSET, (uint8_t*) timeStamp, DW1000_SUB_REG_TX_STAMP_LEN);
}

/**
 * @brief DW1000 读取 40位发送时间戳的高32位
 * @param[in] handle  Comment
 * @param[in] timeStamp_h32  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
inline uint8_t DW1000_TX_TimeStamp_H32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_h32) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(timeStamp_h32 != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_TX_TIME, DW1000_SUB_REG_TX_STAMP_OFFSET + 1, (uint8_t*) timeStamp_h32, sizeof(*timeStamp_h32));
}

/**
 * @brief DW1000 读取 40位发送时间戳的低32位
 * @param[in] handle  Comment
 * @param[in] timeStamp_l32  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
inline uint8_t DW1000_TX_TimeStamp_L32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_l32) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(timeStamp_l32 != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_TX_TIME, DW1000_SUB_REG_TX_STAMP_OFFSET, (uint8_t*) timeStamp_l32, sizeof(*timeStamp_l32));
}


inline uint8_t DW1000_RX_TimeStampRead(DW1000_Handle_t* handle, void* timeStamp) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(timeStamp != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_RX_STAMP_OFFSET, (uint8_t*) timeStamp, DW1000_SUB_REG_RX_STAMP_LEN);
}


/**
 * @brief DW1000 读取 40位接收时间戳的高32位
 * @param[in] handle  Comment
 * @param[in] timeStamp_h32  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
inline uint8_t DW1000_RX_TimeStamp_H32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_h32) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(timeStamp_h32 != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_RX_STAMP_OFFSET + 1, (uint8_t*) timeStamp_h32, sizeof(*timeStamp_h32));
}

/**
 * @brief DW1000 读取 40位接收时间戳的低32位
 * @param[in] handle  Comment
 * @param[in] timeStamp_l32  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
inline uint8_t DW1000_RX_TimeStamp_L32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_l32) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(timeStamp_l32 != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_RX_STAMP_OFFSET, timeStamp_l32, sizeof(*timeStamp_l32));
}

inline uint8_t DW1000_SystemTimeRead(DW1000_Handle_t* handle, void* sysTime) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(sysTime != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_SYS_TIME, 0, (uint8_t*) sysTime, DW1000_REG_SYS_TIME_LEN);
}

/**
 * @brief DW1000 读取 40位接收时间戳的高32位
 * @param[in] handle  Comment
 * @param[in] timeStamp_h32  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
inline uint8_t DW1000_SystemTime_H32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_h32) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(timeStamp_h32 != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_SYS_TIME, 1, (uint8_t*) timeStamp_h32, sizeof(*timeStamp_h32));
}

/**
 * @brief DW1000 读取 40位接收时间戳的低32位
 * @param[in] handle  Comment
 * @param[in] timeStamp_l32  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
inline uint8_t DW1000_SystemTime_L32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_l32) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(timeStamp_l32 != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_SYS_TIME, 0, (uint8_t*) timeStamp_l32, sizeof(*timeStamp_l32));
}

/**
 * @brief DW1000 帧过滤配置
 * @param[in] handle  DW1000 Handle 结构体指针
 * @param[in] enable  Comment
 *  可以使用 | 运算组合使用以下参数（参数可在 driver_dw1000.h 文件里查询）：
 *  @arg DW1000_FF_ALL_TYPE_REJECT: 拒绝所有帧
 *  @arg DW1000_FF_ALL_TYPE_ALLOW: 接收所有帧
 *  @arg DW1000_FF_BEHAVE_COORDINATOR: 作为协调器接收帧
 *  @arg DW1000_FF_BEHAVE_DEVICE: 作为终端设备接收帧
 *  @arg DW1000_FF_BEACON_FRAME_ALLOW: 接收信标帧
 *  @arg DW1000_FF_BEACON_FRAME_REJECT: 拒绝信标帧
 *  @arg DW1000_FF_DATA_FRAME_ALLOW: 接收数据帧
 *  @arg DW1000_FF_DATA_FRAME_REJECT: 拒绝数据帧
 *  @arg DW1000_FF_ACK_FRAME_ALLOW: 接收应答帧
 *  @arg DW1000_FF_ACK_FRAME_REJECT: 拒绝应答帧
 *  @arg DW1000_FF_MAC_CMD_FRAME_ALLOW: 接收MAC命令帧
 *  @arg DW1000_FF_MAC_CMD_FRAME_REJECT: 拒绝MAC命令帧
 *  @arg DW1000_FF_RESERVED_FRAME_ALLOW: 接收保留帧
 *  @arg DW1000_FF_RESERVED_FRAME_REJECT: 拒绝保留帧
 *  @arg DW1000_FF_FRAME_TYPE_FIELD4_ALLOW: 接收类型字段为4的帧
 *  @arg DW1000_FF_FRAME_TYPE_FIELD4_REJECT: 拒绝类型字段为4的帧
 *  @arg DW1000_FF_FRAME_TYPE_FIELD5_ALLOW: 接收类型字段为5的帧
 *  @arg DW1000_FF_FRAME_TYPE_FIELD5_REJECT: 拒绝类型字段为5的帧
 * @return uint8_t 函数执行结果
 *  @arg 0  配置成功
 *  @arg 1  配置失败
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_FrameFilterConfig(DW1000_Handle_t* handle, uint8_t config) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 读取系统配置 */
    uint32_t systemConfig;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig)),
        return 1);
    /* 如果没有拒绝所有帧，说明使能了帧过滤 */
    if (config != DW1000_FF_ALL_TYPE_REJECT) {
        DW1000_SET_BITS(systemConfig, DW1000_REG_SYS_CFG_BIT_FFEN_MSK);
    } else {
        DW1000_CLEAR_BITS(systemConfig, DW1000_REG_SYS_CFG_BIT_FFEN_MSK);
    }
    /* 将帧过滤相关的位域清除 */
    DW1000_CLEAR_BITS(systemConfig, DW1000_FRAME_FILTER_MSK);
    /* 重新设置帧过滤相关的位域 */
    DW1000_SET_BITS(systemConfig, config);
    /* 备份systemConfig寄存器值【其实应当写入后再读取以进行准确备份】 */
    handle->backup.systemConfig = systemConfig;
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, &systemConfig, sizeof(systemConfig));
}

inline uint8_t DW1000_PAN_ID_Set(DW1000_Handle_t* handle, uint16_t PAN_id) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_PANADR, 2, (uint8_t*) &PAN_id, sizeof(PAN_id));
}

inline uint8_t DW1000_ShortAddressSet(DW1000_Handle_t* handle, uint16_t shortAddress) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_PANADR, 0, (uint8_t*) &shortAddress, sizeof(shortAddress));
}


inline uint8_t DW1000_EUI_Set(DW1000_Handle_t* handle, uint64_t eui) {
    // uint8_t DW1000_EUI_Set(DW1000_Handle_t* handle, void* eui){
    // if (handle == NULL || eui == NULL) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_EUI, 0, (uint8_t*) &eui, sizeof(eui));
}

inline uint8_t DW1000_EUI_Get(DW1000_Handle_t* handle, uint64_t* eui) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(eui != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_EUI, 0, (uint8_t*) eui, sizeof(*eui));
}

uint8_t DW1000_OTP_Read(DW1000_Handle_t* handle, uint16_t address, uint32_t* buff, uint16_t len) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(buff !-NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 设置系统时钟为 XTI 以确保OTP读取流程正常 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI),
        return 3);
    /** @todo 打算将上面的函数更换下方的函数，需要进行测试【谨慎】 */
    // if (result = _DW1000_SystemClockSelect(handle, DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI)) {
    //     return result;
    // }

    for (uint16_t i = 0; i < len; i++) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, address + i, &buff[i]),
            return 4);
    }
    /* 恢复系统时钟为 PLL */
    // _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_PLL);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ),
        return 3);
    /** @todo 打算将上面的函数替换掉，但是原来的函数功能并不了解，暂时搁置 */
    return 0;
}

#define DW1000_OTP_WRITE_RETRY_COUNT_MAX 10

uint8_t DW1000_OTP_WriteWord32WithVerify(DW1000_Handle_t* handle, uint32_t data, uint16_t address) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 对编程失败的值重新尝试写入并计入重试次数，超过最大值不再尝试 */
    uint8_t retryCount = 0;
    /* 设置系统时钟为 XTI */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI),
        return 3);
    /* 注意：电源供电要求为 3.7V ！！！！！ */
    /* 设置为编程模式 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_OTP_MR_RegSet(handle, DW1000_OTP_MODE_INIT_PROGRAMMING),
        return 4);
    uint32_t value;
    while (1) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Word32Program(handle, data, address),
            return 5);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, address, &value),
            return 6);
        if (value == data) {
            break;
        } else if (retryCount++ >= DW1000_OTP_WRITE_RETRY_COUNT_MAX) {
            break;
        }
    }
    /* 在上面的写入过程中，虽然达到最大重试次数才退出，但也有可能写入成功 */
    /* 设置为读取模式 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_OTP_MR_RegSet(handle, DW1000_OTP_MODE_LOW_READ_MARGIN),
        return 4);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_OTP_Read(handle, address, &value),
        return 6);
    /* 设置OTP模式为复位所有 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_OTP_MR_RegSet(handle, DW1000_OTP_MODE_RESET_ALL),
        return 4);
    if (value != data) {
        return 7;
    }
    return 0;
}

/**
 * @brief DW1000进入睡眠
 * @param[in] handle  DW1000句柄
 * @return uint8_t
 * @note 该函数使DW1000进入睡眠或深睡眠模式，
 *       在调用该函数前需先调用 DW1000_SleepConfig 函数
 *       以设置睡眠和唤醒的相关参数
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
inline uint8_t DW1000_SleepEnter(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 拷贝配置到AON中上传新的配置 */
    /* 【总感觉缺少了什么东西，但是官方的库函数只调用了这一个函数】 */
    return _DW1000_AON_ArrayUpload(handle);
}

/**
 * @brief 设置睡眠计数器的值，该函数指设置只设置该28为计数器的高16位
 * @param[in] handle  DW1000句柄
 * @param[in] sleepCount  设定的计数值
 * @return uint8_t
 * @note  备注
 * @attention  该函数需在 DW1000_SleepConfig 函数调用前运行，同时SPI速率需小于3MHz
 */
uint8_t DW1000_SleepCountConfig(DW1000_Handle_t* handle, uint16_t sleepCount) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t data;
    /* 强制系统时钟为 XTI */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI),
        return 3);
    /* 复位睡眠配置确保我们不会意外进入睡眠模式 */
    DW1000_WRITE_REG(data, 0x00);
    /* 往子寄存器 AON_CFG0 的低8位写0x00会改变 默认的LPCLKDIVA值，不过因为通常不会使用到，所以不考虑 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG0_OFFSET, &data, sizeof(data)),
        return 2);
    /* 这里同样将子寄存器 AON_CFG1 的低8位写0x00 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG1_OFFSET, &data, sizeof(data)),
        return 2);
    /* 禁用睡眠计数器 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_AON_ConfigurationUpload(handle),
        return 4);
    /* 设置睡眠计数器 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_SLEEP_TIM_OFFSET, (uint8_t*) &sleepCount, sizeof(sleepCount)),
        return 2);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_AON_ConfigurationUpload(handle),
        return 4);
    /* 使能睡眠计数器 */
    DW1000_WRITE_REG(data, DW1000_SLEEP_COUNTER_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG1_OFFSET, &data, sizeof(data)),
        return 2);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_AON_ConfigurationUpload(handle),
        return 4);
    /* 恢复系统时钟为 PLL */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ),
        return 3);
    return 0;
}

/**
 * @brief 校准本地振荡器，因为其频率会根据温度和电压在7到13kHz之间变化
 * @param[in] handle  Comment
 * @param[in] tick  Comment
 * @return uint8_t
 * @note  返回每个低功耗振荡器周期内XTAL/2的周期数。低功耗振荡器（LP OSC）频率 = 19.2 MHz/返回值
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_SleepCountCalibrate(DW1000_Handle_t* handle, uint16_t* tick) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(tick != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t data;
    /* 使能睡眠计数器校准 */
    DW1000_WRITE_REG(data, DW1000_LOW_POWER_OSC_CALIBRATE_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG1_OFFSET, &data, sizeof(data)),
        return 2);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_AON_ConfigurationUpload(handle),
        return 3);
    /* 禁用睡眠计数器校准 */
    DW1000_WRITE_REG(data, DW1000_LOW_POWER_OSC_CALIBRATE_DISABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG1_OFFSET, &data, sizeof(data)),
        return 2);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_AON_ConfigurationUpload(handle),
        return 3);
    /* 强制系统时钟为XTI */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI),
        return 4);
    handle->delay_ms(1);
    /* 读取一个LP振荡器周期所用的XTAL/2周期数 */
    /* 设置地址 先读取高位地址 */
    DW1000_WRITE_REG(data, DW1000_AON_ADDR_LPOSC_CAL_UPPER);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_ADDR_OFFSET, &data, sizeof(data)),
        return 1);
    /* 使能手动覆盖 */
    DW1000_WRITE_REG(data, DW1000_DIRECT_AON_MEM_ACCESS_ENABLE_SET);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data)),
        return 2);
    /* 读取已写入的确认信息 */
    DW1000_SET_BITS(data, DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data)),
        return 2);
    /* 从AON中读回字节 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_RDAT_OFFSET, (uint8_t*) tick, 1),
        return 1);
    *tick <<= 8; // 因为读取的是高8位，所以左移8位
    /* 设置地址 读取低位地址 */
    DW1000_WRITE_REG(data, DW1000_AON_ADDR_LPOSC_CAL_LOWER);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_ADDR_OFFSET, &data, sizeof(data)),
        return 2);
    /* 使能手动覆盖 */
    DW1000_WRITE_REG(data, DW1000_DIRECT_AON_MEM_ACCESS_ENABLE_SET);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data)),
        return 2);
    /* 读取已写入的确认信息 */
    DW1000_SET_BITS(data, DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data)),
        return 2);
    /* 从AON中读回字节 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_RDAT_OFFSET, (uint8_t*) tick, 1),
        return 1);
    /* 禁用手动覆盖 */
    DW1000_CLEAR_BITS(data, 0x00);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data)),
        return 2);
    /* 设置系统时钟为PLL */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ),
        return 4);
    return 0;
}


/**
 * @brief DW1000 睡眠配置
 * @param[in] handle  Comment
 * @param[in] mode  睡眠模式相关配置
 * 可以使用 | 运算组合使用以下参数（参数可在 driver_dw1000.h 文件里查询）：
 *  @arg DW1000_PRESERVE_SLEEP: 保持睡眠模式
 *  @arg DW1000_PRESERVE_SLEEP_NOT: 不保持睡眠模式
 *  @arg DW1000_ONW_RX_OPER_PARAM_SET_LOAD_L64: 唤醒后RX运行参数集加载Length64参数集
 *  @arg DW1000_ONW_RX_OPER_PARAM_SET_RESET: 唤醒后RX运行参数集重置为默认值
 *  @arg DW1000_ONW_HIF_REG_SET_CONFIG_RESTORE: 唤醒后HIF寄存器配置恢复
 *  @arg DW1000_ONW_HIF_REG_SET_CONFIG_RESET: 唤醒后HIF寄存器配置重置
 *  @arg DW1000_ONW_EUI_LOAD_FROM_OTP: 唤醒后从OTP加载EUI
 *  @arg DW1000_ONW_EUI_LOAD_RESET_VAL: 唤醒后EUI重置为默认值
 *  @arg DW1000_ONW_RX_TURN_ON: 唤醒后开启RX
 *  @arg DW1000_ONW_RX_TURN_NOP: 唤醒后不操作RX
 *  @arg DW1000_ONW_ADC_RUN: 唤醒后开启ADC
 *  @arg DW1000_ONW_ADC_NOP: 唤醒后不操作ADC
 * @param[in] wake  唤醒模式相关配置
 *  @arg DW1000_SLEEP_CONFIG_ENABLE: 启用睡眠配置
 *  @arg DW1000_SLEEP_CONFIG_DISABLE: 禁用睡眠配置
 *  @arg DW1000_WAKEUP_BY_PIN_ENABLE: 启用外部唤醒引脚唤醒
 *  @arg DW1000_WAKEUP_BY_PIN_DISABLE: 禁用外部唤醒引脚唤醒
 *  @arg DW1000_WAKEUP_BY_SPI_ACCESS_ENABLE: 启用SPI访问唤醒
 *  @arg DW1000_WAKEUP_BY_SPI_ACCESS_DISABLE: 禁用SPI访问唤醒
 *  @arg DW1000_WAKEUP_BY_SLEEP_COUTNTER_ENABLE: 启用睡眠计数器唤醒
 *  @arg DW1000_WAKEUP_BY_SLEEP_COUTNTER_DISABLE: 禁用睡眠计数器唤醒
 *  @arg DW1000_LOW_POWER_DIVIDER_ENABLE: 启用低功耗时钟分频器
 *  @arg DW1000_LOW_POWER_DIVIDER_DISABLE: 禁用低功耗时钟分频器
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 */
uint8_t DW1000_SleepConfig(DW1000_Handle_t* handle, uint16_t mode, uint8_t wake) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(mode == DW1000_PRESERVE_SLEEP ||
                        mode == DW1000_PRESERVE_SLEEP_NOT ||
                        mode == DW1000_ONW_RX_OPER_PARAM_SET_LOAD_L64 ||
                        mode == DW1000_ONW_RX_OPER_PARAM_SET_RESET ||
                        mode == DW1000_ONW_HIF_REG_SET_CONFIG_RESTORE ||
                        mode == DW1000_ONW_HIF_REG_SET_CONFIG_RESET ||
                        mode == DW1000_ONW_EUI_LOAD_FROM_OTP ||
                        mode == DW1000_ONW_EUI_LOAD_RESET_VAL ||
                        mode == DW1000_ONW_RX_TURN_ON ||
                        mode == DW1000_ONW_RX_TURN_NOP ||
                        mode == DW1000_ONW_ADC_RUN ||
                        mode == DW1000_ONW_ADC_NOP);
    DW1000_ASSERT_PARAM(wake == DW1000_SLEEP_CONFIG_ENABLE ||
                        wake == DW1000_SLEEP_CONFIG_DISABLE ||
                        wake == DW1000_WAKEUP_BY_PIN_ENABLE ||
                        wake == DW1000_WAKEUP_BY_PIN_DISABLE ||
                        wake == DW1000_WAKEUP_BY_SPI_ACCESS_ENABLE ||
                        wake == DW1000_WAKEUP_BY_SPI_ACCESS_DISABLE ||
                        wake == DW1000_WAKEUP_BY_SLEEP_COUTNTER_ENABLE ||
                        wake == DW1000_WAKEUP_BY_SLEEP_COUTNTER_DISABLE ||
                        wake == DW1000_LOW_POWER_DIVIDER_ENABLE ||
                        wake == DW1000_LOW_POWER_DIVIDER_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    mode != handle->backup.sleepMode;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_WCFG_OFFSET, (uint8_t*) &mode, sizeof(mode)),
        return 2);
    return _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG0_OFFSET, &wake, sizeof(wake));
}

/**
 * @brief DW1000 设置发送后进入睡眠模式
 * @param[in] handle  DW1000 Handle 结构体指针
 * @param[in] enable  1：启用 0：禁用
 *  @arg DW1000_STATE_AFTER_TX_AUTO_TO_SLEEP: 发送后自动进入睡眠模式
 *  @arg DW1000_STATE_AFTER_TX_NOT_TO_SLEEP: 发送后不自动进入睡眠模式
 * @return uint8_t 函数执行结果
 * @note  设置自动发送后进入睡眠模式位。当帧发送完成后设备将自动进入深睡眠模式
 * @attention  在调用该函数前必须先调用 DW1000_SleepConfig 去设置唤醒参数
 * IRQ必须为0（无效的，如：没有挂起事件）
 */
uint8_t DW1000_EnterSleepAfterTxSet(DW1000_Handle_t* handle, uint8_t enable) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(enable == DW1000_STATE_AFTER_TX_AUTO_TO_SLEEP ||
                        enable == DW1000_STATE_AFTER_TX_NOT_TO_SLEEP);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t PMSC_ctrl1;
    /* 读取寄存器内容 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &PMSC_ctrl1, sizeof(PMSC_ctrl1)),
        return 1);
    /* 设置对应位 */
    DW1000_MODIFY_REG(PMSC_ctrl1, DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP_MSK, (uint32_t) enable);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &PMSC_ctrl1, sizeof(PMSC_ctrl1));
}

/**
 * @brief 通过SPI读取将DW1000从深睡眠中唤醒
 * @param[in] handle  DW1000句柄
 * @param[in] buff  虚拟缓冲区指针（缓冲区里的并不是为了通信，是通过读取操作来唤醒DW1000）
 * @param[in] len  读取长度（读取长度与片选信号拉低时间相关）
 * @return uint8_t
 * @note  当SPI的片选线持续拉低至少500us后，处于深睡眠的DW1000，将会被唤醒
 *        读取的字节数与持续拉低的片选线之间的有如下的公式
 *        length (bytes) = time (s) * byte_rate (Hz)
 *        byte_rate = spi_rate(Hz) / 8 (SPI连续发送字节时)
 *        为了节约事件和功耗，系统设计者需要精确确定byte_rate值
 *        如果配置wakeup引脚也可以使用该引脚唤醒DW1000
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_WakeUpBySpiRead(DW1000_Handle_t* handle, uint8_t* buff, uint16_t len) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(buff != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t devId;
    /* 读取设备ID以确认芯片是否处于深睡眠 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_DEV_ID, 0, (uint8_t*) &devId, sizeof(devId)),
        return 1);
    /* 芯片处于深睡眠，第一次读取失败 */
    if (devId != DW1000_DEV_ID) {
        /* 需要保持芯片片选线拉低至少500us */
        /* 通过进行一次大数据的读取操作实现持续拉低信号 */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_DEV_ID, 0, buff, len),
            return 1);
        /* 需要5ms等待外部晶振起振与稳定 */
        /* 注意：除非频率小于3MHz，否则无法轮询状态寄存器 */
        handle->delay_ms(5);
    }
    /* 调试 - 检查芯片是否还处于睡眠模式 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_DEV_ID, 0, (uint8_t*) &devId, sizeof(devId)),
        return 1);
    if (devId != DW1000_DEV_ID) {
        return 4;
    }
    return 0;
}

/**
 * @brief DW1000 从OTP加载操作参数集表
 * @param[in] handle  DW1000 Handle 结构体指针
 * @param[in] param  Comment
 *  @arg DW1000_OPER_PARAM_SET_SELECT_L64: 使用Length64运行参数集
 *  @arg DW1000_OPER_PARAM_SET_SELECT_TIGHT: 使用Tight运行参数集
 *  @arg DW1000_OPER_PARAM_SET_SELECT_DEFAULT: 使用默认运行参数集表
 * @return uint8_t 函数执行结果
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_LoadOperParamSetFromOTP(DW1000_Handle_t* handle, uint16_t param) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(param == DW1000_OPER_PARAM_SET_SELECT_L64 ||
                        param == DW1000_OPER_PARAM_SET_SELECT_TIGHT ||
                        param == DW1000_OPER_PARAM_SET_SELECT_DEFAULT);
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t data;
    /* 设置加载操作参数集表 */
    DW1000_WRITE_REG(data, param);
    /* 设置运行参数集加载 */
    DW1000_SET_BITS(data, DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK_MSK);
    /* 强制使能LDE时钟 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_LDE),
        return 3);
    /* 写入配置 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_SF_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 恢复默认时钟（ENABLE_ALL_SEQ） */
    return _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ);
}

/**
 * @brief DW1000 智能发送功率控制配置
 * @param[in] handle  DW1000 Handle 结构体指针
 * @param[in] enable  使能配置
 *  @arg DW1000_SMART_TX_POWER_CONTROL_ENABLE: 启用智能发送功率控制
 *  @arg DW1000_SMART_TX_POWER_CONTROL_DISABLE: 禁用智能发送功率控制
 * @return uint8_t 函数执行结果
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_SmartTxPowerSet(DW1000_Handle_t* handle, uint32_t enable) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(enable == DW1000_SMART_TX_POWER_CONTROL_ENABLE || enable == DW1000_SMART_TX_POWER_CONTROL_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t systemConfig;
    /* 读取 SYS_CFG 寄存器 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig)),
        return 1);
    /* 根据传入参数配置 */
    DW1000_MODIFY_REG(systemConfig, DW1000_REG_SYS_CFG_BIT_DIS_STXP_MSK, enable);
    /* 写回 SYS_CFG 寄存器 */
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig));
}

/**
 * @brief DW1000 自动应答配置
 * @param[in] handle  DW1000句柄
 * @param[in] respDelayTime  值非零时为发送后的延迟，最大值255（单位：符号）
 *                               值为0时表示尽可能快的发送
 * @return uint8_t
 * @note  备注
 * @attention  需要同时使能帧过滤
 */
uint8_t DW1000_AutoAckEnable(DW1000_Handle_t* handle, uint8_t respDelayTime) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(respDelayTime <= 0xFF);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t systemConfig = handle->backup.systemConfig;
    /* 设置自动应答回复延迟 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_ACK_RESP_T, DW1000_SUB_REG_ACK_TIM_OFFSET, &respDelayTime, sizeof(respDelayTime)),
        return 2);
    /* 使能自动应答 */
    DW1000_SET_BITS(systemConfig, DW1000_REG_SYS_CFG_BIT_AUTOACK_MSK);
    /* 更新系统配置备份值【实际上准确方式为先将值写入后再读取出来进行保存】 */
    handle->backup.systemConfig = systemConfig;
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig));
}

/**
 * @brief DW1000 双缓冲模式配置
 * @param[in] handle  DW1000句柄
 * @param[in] enable  Comment
 *  @arg DW1000_DOUBLE_RX_BUFFER_ENABLE: 启用双缓冲模式
 *  @arg DW1000_DOUBLE_RX_BUFFER_DISABLE: 禁用双缓冲模式
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_DoubleRxBuffModeSet(DW1000_Handle_t* handle, uint8_t enable) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(enable == DW1000_DOUBLE_RX_BUFFER_ENABLE ||
                        enable == DW1000_DOUBLE_RX_BUFFER_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t systemConfig = handle->backup.systemConfig;
    /* 根据传入参数配置双缓冲模式 */
    DW1000_MODIFY_REG(systemConfig, DW1000_REG_SYS_CFG_BIT_DIS_DRXB_MSK, (uint32_t) enable);
    handle->backup.systemConfig = systemConfig;
    handle->init.useDoubleBuff = (enable == DW1000_DOUBLE_RX_BUFFER_ENABLE) ? 1 : 0;
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig));
}

/**
 * @brief DW1000使能接收双缓冲
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 */
uint8_t DW1000_RxDoubleBuffEnable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t systemConfig = handle->backup.systemConfig;
    /* 根据传入参数配置双缓冲模式 */
    DW1000_MODIFY_REG(systemConfig, DW1000_REG_SYS_CFG_BIT_DIS_DRXB_MSK, DW1000_DOUBLE_RX_BUFFER_ENABLE);
    handle->backup.systemConfig = systemConfig;
    handle->init.useDoubleBuff = 1;
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig));
}

/**
 * @brief DW1000警用接收双缓冲
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 */
uint8_t DW1000_RxDoubleBuffDisable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t systemConfig = handle->backup.systemConfig;
    /* 根据传入参数配置双缓冲模式 */
    DW1000_MODIFY_REG(systemConfig, DW1000_REG_SYS_CFG_BIT_DIS_DRXB_MSK, DW1000_DOUBLE_RX_BUFFER_DISABLE);
    handle->backup.systemConfig = systemConfig;
    handle->init.useDoubleBuff = 0;
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig));
}

/**
 * @brief 设置发送后接收的延迟时间
 * @param[in] handle  Comment
 * @param[in] rxDelayTime  (有效值20位) 延迟单位为 UWB 毫秒
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_RxAfterTxDelaySet(DW1000_Handle_t* handle, uint32_t rxDelayTime) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(rxDelayTime < (0x1 << 20));
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t value;
    /* 读取 ACK_RESP_T 寄存器内容 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_ACK_RESP_T, 0, (uint8_t*) &value, sizeof(value)),
        return 1);
    /* 设置延迟值 */
    DW1000_MODIFY_REG(value, DW1000_REG_ACK_RESP_T_BITS_W4R_TIM_MSK, rxDelayTime);
    /* 写回 ACK_RESP_T 寄存器 */
    return _DW1000_SPI_Write(handle, DW1000_REG_ACK_RESP_T, 0, &value, sizeof(value));
}

/**
 * @brief 同步接收缓冲区指针
 * @param[in] handle  Comment
 * @return uint8_t
 * @note  需要确保主机（或IC）缓冲区指针在开始接收前对齐
 */
uint8_t DW1000_RxBuffPtrsSync(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t data;
    uint8_t buff;
    /* 确保主机的缓冲区指针在开始接收前已对齐 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, DW1000_SUB_REG_SYS_STATUS_3_OFFSET, &buff, sizeof(buff)),
        return 1);
    if ((!!DW1000_READ_BITS(buff, DW1000_SUB_REG_SYS_STATUS_3_BIT_ICRBP_MSK)) !=
        (!!DW1000_READ_BITS(buff, DW1000_SUB_REG_SYS_STATUS_3_BIT_HSRBP_MSK))) {
        /* 交换RX Buffer 状态寄存器 */
        DW1000_WRITE_REG(data, 0x01);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, DW1000_SUB_REG_SYS_CTRL_3_OFFSET, &data, sizeof(data)),
            return 2);
    }
    return 0;
}

/**
 * @brief 关闭收发器
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果
 * @note  备注
 */
uint8_t DW1000_ForceTrxOff(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t systemEventMask;
    uint32_t data;
    /* 读取 SYS_MASK 寄存器内容（暂存当前中断掩码用于后续恢复） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &systemEventMask, sizeof(systemEventMask)),
        return 1);
    /* 关闭中断 */
    _DW1000_CriticalEnter(handle);

    /* 清除中断掩码（我们不接收不希望的中断） */
    DW1000_SET_BITS(data, 0x00);
    /* 写回 SYS_MASK 寄存器 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &systemEventMask, sizeof(systemEventMask)),
        return 2);
    /* 关闭无线收发 */
    DW1000_WRITE_REG(data, DW1000_REG_SYS_CTRL_BIT_TRXOFF);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, (uint8_t*) &data, 1),
        return 2);
    /* 强制收发机关闭 */
    DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_TX_MSK |
                               DW1000_SYS_STATUS_ALL_RX_ERR_MSK |
                               DW1000_SYS_STATUS_ALL_RX_TO_MSK |
                               DW1000_SYS_STATUS_ALL_RX_GOOD_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, &data, sizeof(data)),
        return 2);
    /* */
    DW1000_EXEC_WITH_ERR_HANDLE(
        DW1000_RxBuffPtrsSync(handle),
        return 3);
    /* 恢复中断掩码 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &systemEventMask, sizeof(systemEventMask)),
        return 2);
    /* 恢复中断 */
    _DW1000_CriticalExit(handle);
    handle->init.isRxOnAfterTx = 0;
    return 0;
}

/**
 * @brief 此API函数用于配置延迟发送时间或延迟接收开始时间
 * @param[in] handle  Comment
 * @param[in] delayTime  发送/接收开始时间（这32位应该是发送消息或开启接收器时系统时间的高32位）
 * @return uint8_t
 * @note  备注
 */
inline uint8_t DW1000_TrxDelayTimeH32Set(DW1000_Handle_t* handle, uint32_t delayTime) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 只写入高32位，低8位忽略 */
    return _DW1000_SPI_Write(handle, DW1000_REG_DX_TIME, 1, (uint8_t*) &delayTime, sizeof(delayTime));
}

/**
 * @brief 复位DW1000的接收器
 * @param[in] handle  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_RxReset(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t data;
    /* 设置接收复位 */
    DW1000_WRITE_REG(data, DW1000_SUB_REG_PMSC_CTRL0_3_BITS_SOFTRESET);
    DW1000_CLEAR_BITS(data, DW1000_RX_RESET_ENABLE_8BIT);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_3_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 清除接收复位设置 */
    DW1000_SET_BITS(data, DW1000_RX_RESET_DISABLE_8BIT);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_3_OFFSET, (uint8_t*) &data, sizeof(data));
}


/**
 * @brief DW1000开始发送
 * @param[in] handle  DW1000句柄
 * @param[in] mode  发送模式，取值如下
 *  @arg DW1000_TX_MODE_IMMEDIATE: 立即发送
 *  @arg DW1000_TX_MODE_DELAYED: 延时发送
 *  @arg DW1000_TX_MODE_EXPECT_RESPONSE: 期望响应（当发送完成后自动打开接收器）
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note 返回值具体内容请自行查看函数体内部了解
 * @attention  如果延迟发送的时间已经错过，则延迟发送将会被取消
 */
uint8_t DW1000_TxStart(DW1000_Handle_t* handle, uint8_t mode) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM((mode == DW1000_TX_MODE_IMMEDIATE) ||
                        (mode == DW1000_TX_MODE_DELAYED) ||
                        (mode == DW1000_TX_MODE_IMMEDIATE | DW1000_TX_MODE_EXPECT_RESPONSE) ||
                        (mode == DW1000_TX_MODE_DELAYED | DW1000_TX_MODE_EXPECT_RESPONSE));
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t data = 0x00;
    /* 如果设置了期待响应 */
    if (DW1000_READ_BITS(mode, DW1000_TX_MODE_EXPECT_RESPONSE)) {
        /* 设置等待响应位 */
        DW1000_SET_BITS(data, DW1000_REG_SYS_CTRL_BIT_WAIT4RESP_MSK);
        handle->init.isRxOnAfterTx = 1;
    }
    /* 如果设置了延迟发送 */
    if (DW1000_READ_BITS(mode, DW1000_TX_MODE_DELAYED)) {
        /* 设置延时发送位 并 使能发送 */
        DW1000_SET_BITS(data, DW1000_REG_SYS_CTRL_BIT_TXDLYS_MSK | DW1000_REG_SYS_CTRL_BIT_TXSTRT_MSK);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data)),
            return 1);
        /* 读取系统状态寄存器，只需要偏移地址0x03开始的16位数据 */
        uint16_t systemStatus;
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, DW1000_SUB_REG_SYS_STATUS_3_OFFSET, (uint8_t*) &systemStatus, sizeof(systemStatus)),
            return 2);
        /* 如果出现发送错误（HPDWARN 和 TXPUTE 置位）
            说明TXDLYS相对于DX_TIME太迟
        */
        if (DW1000_READ_BITS(systemStatus, DW1000_SYS_STATUS_O3_L16_TXERR_MSK)) {
            /* 取消延迟发送，并返回错误 */
            DW1000_WRITE_REG(data, DW1000_REG_SYS_CTRL_BIT_TRXOFF);
            DW1000_EXEC_WITH_ERR_HANDLE(
                _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data)),
                return 3);
            return 4; // 延时发送未成功启动
        } else {
            return 0; // 延时发送成功启动
        }
    } else {
        /* 立即发送 只需使能发送位 */
        DW1000_SET_BITS(data, DW1000_REG_SYS_CTRL_BIT_TXSTRT_MSK);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data)),
            return 5);
    }
    return 0;
}

#if (USE_DW1000_REGISTER_CALLBACKS == 1U)

/**
 * @brief DW1000注册回调函数
 * @param[in] handle  DW1000句柄
 * @param[in] callbackId  回调函数ID，用于指示哪种回调函数
 *  @arg DW1000_TX_COMPLETE_CB_ID: 发送完成回调函数ID
 *  @arg DW1000_RX_COMPLETE_CB_ID: 接收完成回到函数ID
 *  @arg DW1000_RX_ERROR_CB_ID: 接收错误回调函数ID
 *  @arg DW1000_RX_TIMEOUT_CB_ID: 接收超时回调函数ID
 * @param[in] callback  回调函数
 * @return uint8_t 执行结果 0: 注销成功 1: 输入参数错误
 */
uint8_t DW1000_RegisterCallback(DW1000_Handle_t* handle, DW1000_CallbackIdTypeDef callbackId, DW1000_CallbackTypeDef callback) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(callback != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t result = 0;
    /* 进入临界区关闭中断，避免在设置中断回调时被中断 */
    _DW1000_CriticalEnter(handle);

    switch (callbackId) {
        case DW1000_TX_COMPLETE_CB_ID:
            handle->TxCompleteCallback = callback;
            break;
        case DW1000_RX_COMPLETE_CB_ID:
            handle->RxCompleteCallback = callback;
            break;
        case DW1000_RX_ERROR_CB_ID:
            handle->RxErrorCallback = callback;
            break;
        case DW1000_RX_TIMEOUT_CB_ID:
            handle->RxTimeoutCallback = callback;
            break;
        default:
            result = 1;
    }
    /* 退出临界区 */
    _DW1000_CriticalExit(handle);
    return result;
}

/**
 * @brief DW1000注销回调函数
 * @param[in] handle  DW1000句柄
 * @param[in] callbackId  回调函数ID，用于指示哪种回调函数
 *  @arg DW1000_TX_COMPLETE_CB_ID: 发送完成回调函数ID
 *  @arg DW1000_RX_COMPLETE_CB_ID: 接收完成回到函数ID
 *  @arg DW1000_RX_ERROR_CB_ID: 接收错误回调函数ID
 *  @arg DW1000_RX_TIMEOUT_CB_ID: 接收超时回调函数ID
 * @return uint8_t 执行结果 0: 注销成功 1: 输入参数错误
 * @note  注销回调函数后，对应的回调函数会使用默认的DW1000回调函数
 */
uint8_t DW1000_UnregisterCallback(DW1000_Handle_t* handle, DW1000_CallbackIdTypeDef callbackId) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(callback != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t result = 0;
    /* 进入临界区关闭中断，避免在设置中断回调时被中断 */
    _DW1000_CriticalEnter(handle);

    switch (callbackId) {
        case DW1000_TX_COMPLETE_CB_ID:
            handle->TxCompleteCallback = DW1000_TxCompleteCallback;
            break;
        case DW1000_RX_COMPLETE_CB_ID:
            handle->RxCompleteCallback = DW1000_RxCompleteCallback;
            break;
        case DW1000_RX_ERROR_CB_ID:
            handle->RxErrorCallback = DW1000_RxErrorCallback;
            break;
        case DW1000_RX_TIMEOUT_CB_ID:
            handle->RxTimeoutCallback = DW1000_RxTimeoutCallback;
            break;
        default:
            result = 1;
    }
    /* 退出临界区 */
    _DW1000_CriticalExit(handle);
    return result;
}

#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

/**
 * @brief 检查DW1000的中断请求是否有效（替代使用中断函数）
 * @param[in] handle  DW1000句柄
 * @param[in] isActive  0: 无有效中断 1: 中断有效
 * @return uint8_t 函数执行结果
 *  @arg 0: 执行成功
 *  @arg 1: 读取SYS_STATUS失败
 */
uint8_t DW1000_IRQ_Check(DW1000_Handle_t* handle, uint8_t* isActive) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(flag != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t status;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, &status, sizeof(status)),
        return 1);
    /* 对于本次操作只需要低8位字节 */
    *isActive = !!DW1000_READ_BITS(status, DW1000_REG_SYS_STATUS_BIT_IRQS_MSK);
    return 0;
}

/**
 * @brief DW1000 低功耗监听设置
 * @param[in] handle  DW1000句柄
 * @param[in] enable  使能参数，取值如下
 *  @arg DW1000_LOW_POWER_LISTENING_MODE_ENABLE: 启用低功耗监听模式
 *  @arg DW1000_LOW_POWER_LISTENING_MODE_DISABLE: 禁用低功耗监听模式
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  低功耗监听是DW1000的一项功能，该功能使设备主要处于休眠状态，但会定期唤醒（在“长睡眠”状态之后），
          在极短的时间内对空气进行采样，以获取前导序列。这种前导序列的采样“监听”。
          该阶段实际上是由“短休眠”时间分隔的两个接收阶段。
          更多详细信息，请参阅DW1000用户手册中的“低功耗监听”部分。
 * @attention 在使能低功耗监听前必须调用以下函数对其进行完全配置：
        使用 DW1000_SleepConfig 配置长睡眠阶段。输入参数 mode 至少包含
              DW1000_PRESERVE_SLEEP，DW1000_ONW_HIF_REG_SET_CONFIG_RESTORE，DW1000_ONW_RX_TURN_ON
            且输入参数 wake 同时至少包含
              DW1000_WAKEUP_BY_SLEEP_COUTNTER_ENABLE，DW1000_SLEEP_CONFIG_ENABLE
        使用 DW1000_SleepCountCalibrate 配置 长睡眠 阶段持续时间
        使用 DW1000_SnoozeTimeSet 配置 snooze 阶段持续时间
        使用 DW1000_PreambleDetectTimeoutSet 设置接收阶段持续时间
        使用 DW1000_InterruptSet 只使能RX Good帧中断
        通过调用 DW1000_SleepEnter 进入睡眠 或 调用 DW1000_RxEnable 激活接收功能
        来触发 低功耗监听模式
        请参阅低功耗监听示例（Decawave 网站上随API分发的示例8a/8b）
        这些示例构成了一个工作示例代码，展示如何正确使用低功耗监听
 */
uint8_t DW1000_LowPowerListeningSet(DW1000_Handle_t* handle, uint32_t enable) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(enable == DW1000_LOW_POWER_LISTENING_MODE_ENABLE ||
                        enable == DW1000_LOW_POWER_LISTENING_MODE_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t result;
    uint32_t pmsc_ctrl1;
    /* 读取 PMSC_CTRL1 寄存器内容 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &pmsc_ctrl1, sizeof(pmsc_ctrl1))) {
        return result;
    }
    /* 根据配置设置相应位 */
    DW1000_MODIFY_REG(pmsc_ctrl1, DW1000_LOW_POWER_LISTENING_MODE_MASK, enable);
    handle->init.useLowPowListen = (enable == DW1000_LOW_POWER_LISTENING_MODE_ENABLE) ? 1 : 0;
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &pmsc_ctrl1, sizeof(pmsc_ctrl1));
}

/**
 * @brief DW1000使能低功耗监听
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI 接口读操作失败
 *  @arg 2: SPI 接口写操作失败
 * @note  低功耗监听是DW1000的一项功能，该功能使设备主要处于休眠状态，但会定期唤醒（在“长睡眠”状态之后），
          在极短的时间内对空气进行采样，以获取前导序列。这种前导序列的采样“监听”。
          该阶段实际上是由“短休眠”时间分隔的两个接收阶段。
          更多详细信息，请参阅DW1000用户手册中的“低功耗监听”部分。
 * @attention 在使能低功耗监听前必须调用以下函数对其进行完全配置：
        使用 DW1000_SleepConfig 配置长睡眠阶段。输入参数 mode 至少包含
              DW1000_PRESERVE_SLEEP，DW1000_ONW_HIF_REG_SET_CONFIG_RESTORE，DW1000_ONW_RX_TURN_ON
            且输入参数 wake 同时至少包含
              DW1000_WAKEUP_BY_SLEEP_COUTNTER_ENABLE，DW1000_SLEEP_CONFIG_ENABLE
        使用 DW1000_SleepCountCalibrate 配置 长睡眠 阶段持续时间
        使用 DW1000_SnoozeTimeSet 配置 snooze 阶段持续时间
        使用 DW1000_PreambleDetectTimeoutSet 设置接收阶段持续时间
        使用 DW1000_InterruptSet 只使能RX Good帧中断
        通过调用 DW1000_SleepEnter 进入睡眠 或 调用 DW1000_RxEnable 激活接收功能
        来触发 低功耗监听模式
        请参阅低功耗监听示例（Decawave 网站上随API分发的示例8a/8b）
        这些示例构成了一个工作示例代码，展示如何正确使用低功耗监听
 */
uint8_t DW1000_LplEnable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t pmsc_ctrl1;
    /* 读取 PMSC_CTRL1 寄存器内容 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC,
                         DW1000_SUB_REG_PMSC_CTRL1_OFFSET,
                         (uint8_t*) &pmsc_ctrl1, sizeof(pmsc_ctrl1)),
        return 1);
    /* 使能低功耗监听 */
    DW1000_SET_BITS(pmsc_ctrl1, DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_MSK | DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_MSK);
    handle->init.useLowPowListen = 1;
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &pmsc_ctrl1, sizeof(pmsc_ctrl1));
}

/**
 * @brief DW1000禁用低功耗监听
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 * @note  低功耗监听是DW1000的一项功能，该功能使设备主要处于休眠状态，但会定期唤醒（在“长睡眠”状态之后），
          在极短的时间内对空气进行采样，以获取前导序列。这种前导序列的采样“监听”。
          该阶段实际上是由“短休眠”时间分隔的两个接收阶段。
          更多详细信息，请参阅DW1000用户手册中的“低功耗监听”部分。
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_LplDisable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t pmsc_ctrl1;
    /* 读取 PMSC_CTRL1 寄存器内容 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC,
                         DW1000_SUB_REG_PMSC_CTRL1_OFFSET,
                         (uint8_t*) &pmsc_ctrl1, sizeof(pmsc_ctrl1)),
        return 1);
    /* 使能低功耗监听 */
    DW1000_CLEAR_BITS(pmsc_ctrl1, DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_MSK | DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_MSK);
    handle->init.useLowPowListen = 0;
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &pmsc_ctrl1, sizeof(pmsc_ctrl1));
}


inline uint8_t DW1000_SnoozeTimeSet(DW1000_Handle_t* handle, uint8_t snoozeTime) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_SNOZT_OFFSET, (uint8_t*) &snoozeTime, sizeof(snoozeTime));
}

/**
 * @brief DW1000设置LED
 * @param[in] handle  DW1000句柄
 * @param[in] enable  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 具体内容待未来补充
 */
uint8_t DW1000_LEDsSet(DW1000_Handle_t* handle, uint8_t enable) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(enable); // 【留待后续修改】
#endif                           /* USE_DW1000_FULL_ASSERT */
    ///////////////////////////////////////////////////////
    // return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_LEDC_OFFSET, (uint8_t*) &config, sizeof(config));
}

/**
 * @brief 配置Sniff模式（使能或失能）
 * @param[in] handle  DW1000句柄
 * @param[in] enable  是否使能Sniff模式
 *  @arg DW1000_PLL2_SEQ_NORMAL: 正常模式（不使能Sniff模式）
 *  @arg DW1000_PLL2_SEQ_RX_SNIFF: 监听模式（使能Sniff模式）
 * @param[in] timeOn  接收器开启阶段的持续时间，以PAC大小的倍数表示。
 *                    计数器会自动将1个PAC大小的值加到设定的数值上。
 *                    可设置的最小值为1（即开启时间为2个PAC大小），最大值为15。
 * @param[in] timeOff  接收器关闭的持续时间，单位为128/125 μs（约1μs），最大值255
 * @return uint8_t 执行结果 0: 执行成功 其他: 运行错误
 * @note  SNIFF模式是一种低功耗接收模式，在这种模式下，接收器会按顺序开启和关闭，而不是一直处于开启状态。
 *        开关状态消耗的时间由 timeOn 和 timeOff 两个参数设置
 * @warning  未来会弃用该函数，推荐使用 DW1000_SniffModeEnable 和 DW1000_SniffModeDisable 替代
 */
uint8_t DW1000_SniffModeSet(DW1000_Handle_t* handle, uint8_t enable, uint8_t timeOn, uint8_t timeOff) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM((enable == DW1000_PLL2_SEQ_NORMAL) ||
           (enable == DW1000_PLL2_SEQ_RX_SNIFF);
    DW1000_ASSERT_PARAM((timeOn > 0) && (timeOn <= 15));
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t pmsc_ctrl0;
    uint16_t rxSniff;
    /* 读取 PMSC_CTRL0 寄存器内容 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &pmsc_ctrl0, sizeof(pmsc_ctrl0)),
        return 1);
    /* 当禁用Sniff模式时，自动将传入开关时间参数清零 */
    if (enable != DW1000_PLL2_SEQ_RX_SNIFF) {
        timeOff = 0x00;
        timeOn = 0x00;
    }
    /* 配置 RX_SNIFF 时间 */
    rxSniff = ((uint16_t) timeOff << 8) | timeOn;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RX_SNIFF, 0, (uint8_t*) &rxSniff, sizeof(rxSniff)),
        return 2);
    /* 根据配置设置相应位 */
    DW1000_MODIFY_REG(pmsc_ctrl0, DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_MSK, enable);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &pmsc_ctrl0, sizeof(pmsc_ctrl0));
}

/**
 * @brief DW1000使能Sniff模式
 * @param[in] handle  DW1000句柄
 * @param[in] timeOn  接收器开启阶段的持续时间，以PAC大小的倍数表示。
 *                    计数器会自动将1个PAC大小的值加到设定的数值上。
 *                    可设置的最小值为1（即开启时间为2个PAC大小），最大值为15。
 * @param[in] timeOff  接收器关闭的持续时间，单位为128/125 μs（约1μs），最大值255
 * @return uint8_t 执行结果 0: 执行成功 其他: 运行错误
 * @note  SNIFF模式是一种低功耗接收模式，在这种模式下，接收器会按顺序开启和关闭，而不是一直处于开启状态。
 *        开关状态消耗的时间由 timeOn 和 timeOff 两个参数设置
 */
uint8_t DW1000_SniffModeEnable(DW1000_Handle_t* handle, uint8_t timeOn, uint8_t timeOff) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM((timeOn > 0) && (timeOn <= 15));
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t pmsc_ctrl0;
    uint16_t rxSniff;
    /* 读取 PMSC_CTRL0 寄存器内容 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &pmsc_ctrl0, sizeof(pmsc_ctrl0)),
        return 1);
    /* 配置 RX_SNIFF 时间 */
    rxSniff = ((uint16_t) timeOff << 8) | timeOn;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RX_SNIFF, 0, (uint8_t*) &rxSniff, sizeof(rxSniff)),
        return 2);
    /* 根据配置设置相应位 */
    DW1000_SET_BITS(pmsc_ctrl0, DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_MSK);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &pmsc_ctrl0, sizeof(pmsc_ctrl0));
}

/**
 * @brief DW1000禁用Sniff模式
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果 0: 执行成功 其他: 运行错误
 * @note  备注
 */
uint8_t DW1000_SniffModeDisable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t pmsc_ctrl0;
    uint16_t rxSniff = 0x0000;
    /* 读取 PMSC_CTRL0 寄存器内容 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &pmsc_ctrl0, sizeof(pmsc_ctrl0)),
        return 1);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RX_SNIFF, 0, (uint8_t*) &rxSniff, sizeof(rxSniff)),
        return 2);
    /* 根据配置设置相应位 */
    DW1000_SET_BITS(pmsc_ctrl0, DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_MSK);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &pmsc_ctrl0, sizeof(pmsc_ctrl0));
}


/**
 * @brief 设置接收超时
 * @param[in] handle  DW1000句柄
 * @param[in] timeout  0：禁用接收超时
 *                   非0：启用接收超时，超时时间从接收使能开始计算
 *                   时间单位为1.0256us（512/499.2MHz）
 * @return uint8_t
 * @note  这里原版的官方库函数不同，这里只会设置接收超时时间
 *        并不会使能或禁用接收超时功能，因此在在设置接收超时前
 *        需要调用 DW1000_RxTimeoutEnable 来开启接收超时功能
 *        或调用 DW1000_RxTimeoutDisable 关闭接收超时功能
 */
inline uint8_t DW1000_RxTimeoutSet(DW1000_Handle_t* handle, uint16_t timeout) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_RX_FWTO, 0, (uint8_t*) &timeout, sizeof(timeout));
}

/**
 * @brief DW1000接收超时功能使能
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 */
uint8_t DW1000_RxTimeoutEnable(DW1000_Handle_t* handle) {
    uint8_t data;
    /* 读取 SYS_CFG 寄存器内容（只需要偏移地址0x03 的 8位数据） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_CFG, DW1000_SUB_REG_SYS_CFG_3_OFFSET, &data, sizeof(data)),
        return 1);
    /* 启用接收超时 */
    DW1000_SET_BITS(data, DW1000_SUB_REG_SYS_CFG_3_BIT_RXWTOE_MSK);
    /* 更新备份的系统配置 */
    DW1000_SET_BITS(handle->backup.systemConfig, DW1000_REG_SYS_CFG_BIT_RXWTOE_MSK);
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, DW1000_SUB_REG_SYS_CFG_3_OFFSET, (uint8_t*) &data, sizeof(data));
}

/**
 * @brief DW1000接收超时功能禁用
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 */
uint8_t DW1000_RxTimeoutDisable(DW1000_Handle_t* handle) {
    uint8_t data;
    /* 读取 SYS_CFG 寄存器内容（只需要偏移地址0x03 的 8位数据） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_CFG, DW1000_SUB_REG_SYS_CFG_3_OFFSET, &data, sizeof(data)),
        return 1);
    /* 启用接收超时 */
    DW1000_CLEAR_BITS(data, DW1000_SUB_REG_SYS_CFG_3_BIT_RXWTOE_MSK);
    /* 更新备份的系统配置 */
    DW1000_CLEAR_BITS(handle->backup.systemConfig, DW1000_REG_SYS_CFG_BIT_RXWTOE_MSK);
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, DW1000_SUB_REG_SYS_CFG_3_OFFSET, (uint8_t*) &data, sizeof(data));
}


#define DW1000_RX_MODE_IMMEDIATE                     0x00
#define DW1000_RX_MODE_DELAYED                       0x01
#define DW1000_RX_MODE_IDLE_ON_DELAY_ERR             0x02
#define DW1000_RX_MODE_NO_SYNC_PTRS                  0x04
#define DW1000_RX_MODE_DELAYED_AND_IDLE_ON_DELAY_ERR (DW1000_RX_MODE_DELAYED | DW1000_RX_MODE_IDLE_ON_DELAY_ERR)
#define DW1000_RX_MODE_IMMEDIATE_AND_NO_SYNC_PTRS    (DW1000_RX_MODE_IMMEDIATE |)


/**
 * @brief 此调用会开启接收器，可以是立即开启或延迟开启（取决于模式参数）。
 *        若发生“延迟”错误，则仅当未设置DWT_IDLE_ON_DLY_ERR时，接收器才会开启。
 *        接收器将保持开启状态，监听任何消息，
 *        直到接收到一个正确的帧、一个错误（CRC、PHY报头、Reed Solomon错误）
 *        或超时（SFD、前导码或帧）。
 * @param[in] handle  Comment
 * @param[in] mode  Comment
 *  @arg DW1000_RX_MODE_IMMEDIATE: 立即接收
 *  @arg DW1000_RX_MODE_DELAYED: 延时接收
 *  @arg DW1000_RX_MODE_NO_SYNC_PTRS: 不同步接收指针
 *  @arg DW1000_RX_MODE_IDLE_ON_DELAY_ERR: 延时接收错误时进入空闲状态
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 *  @arg 3: 接收缓冲区指针同步失败
 *  @arg 4: 强制关闭收发器失败
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 后面打算替换掉该函数，该函数功能职责不单一，配置与使能应当分离
 */
uint8_t DW1000_RxEnable(DW1000_Handle_t* handle, uint8_t mode) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(mode == DW1000_RX_MODE_IMMEDIATE ||
                        mode == DW1000_RX_MODE_DELAYED ||
                        mode == DW1000_RX_MODE_NO_SYNCDW1000_RX_MODE_DELAYED_AND_IDLE_ON_DELAY_ERR_PTRS ||
                        mode == DW1000_RX_MODE_IMMEDIATE_AND_NO_SYNC_PTRS);
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t data;
    /* 使能接收 */
    DW1000_WRITE_REG(data, DW1000_REG_SYS_CTRL_BIT_RXENAB);
    /* 如果设置不同步接收指针 */
    if (DW1000_READ_BITS(mode, DW1000_RX_MODE_NO_SYNC_PTRS)) {
        /* 设置不同步接收指针位 */
        DW1000_EXEC_WITH_ERR_HANDLE(
            DW1000_RxBuffPtrsSync(handle),
            return 3);
    }
    /* 如果设置了延时接收 */
    if (DW1000_READ_BITS(mode, DW1000_RX_MODE_DELAYED)) {
        /* 设置延时接收位 */
        DW1000_SET_BITS(data, DW1000_REG_SYS_CTRL_BIT_RXDLYE_MSK);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data)),
            return 2);
        /* 检查错误 */
        uint8_t systemStatus;
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, DW1000_SUB_REG_SYS_STATUS_3_OFFSET, &systemStatus, sizeof(systemStatus)),
            return 2);
        /* 如果延迟以达到则立即开启接收除非使能了 DW1000_RX_MODE_IDLE_ON_DELAY_ERR */
        if (DW1000_READ_BITS(systemStatus, DW1000_SUB_REG_SYS_STATUS_3_BIT_HPDWARN)) {
            /* 强制关闭收发机 */
            DW1000_EXEC_WITH_ERR_HANDLE(
                DW1000_ForceTrxOff(handle),
                return 4);
            /* 如果没有使能延时错误进入空闲模式 */
            if (!DW1000_READ_BITS(mode, DW1000_RX_MODE_IDLE_ON_DELAY_ERR)) {
                /* 重新使能接收 */
                DW1000_EXEC_WITH_ERR_HANDLE(
                    _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data)),
                    return 2);
            }
        }
    } else {
        /* 立即接收 只需使能接收位 */
        DW1000_WRITE_REG(data, DW1000_REG_SYS_CTRL_BIT_RXENAB);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data)),
            return 2);
    }
    return 0;
}

/**
 * @brief DW1000接收禁用
 * @param[in] handle  DW1000句柄
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 有使能函数就应该由禁用函数，但是官方库并未编写
 *       这里预留，以后编写
 */
uint8_t DW1000_RxDisable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
}


/**
 * @brief 设置前导码检测超时时间
 * @param[in] handle  DW1000句柄
 * @param[in] pdTimeout  0：禁用前导码检测超时
 *                     非0：设置前导码检测超时时间
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  前导码检测超时，以PAC大小的倍数表示。
 *        计数器会自动将值设置为PAC大小的倍数加1。
 *        可设置的最小值为1（即超时时间为2个PAC大小）。
 */
inline uint8_t DW1000_PreambleDetectTimeoutSet(DW1000_Handle_t* handle, uint16_t pdTimeout) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_PRETOC_OFFSET, (uint8_t*) &pdTimeout, sizeof(pdTimeout));
}


/**
 * @brief DW1000设置中断
 * @param[in] handle  DW1000句柄
 * @param[in] event  中断事件，可以是以下的 | 组合
 *  @arg DW1000_INT_EVENT_FRAME_SENT: 帧发送完成中断
 *  @arg DW1000_INT_EVENT_FRAME_RECIVED: 帧接收完成中断
 *  @arg DW1000_INT_EVENT_RX_PHR_ERROR: PHR错误中断
 *  @arg DW1000_INT_EVENT_RX_CRC_ERROR: CRC错误中断
 *  @arg DW1000_INT_EVENT_RX_SYNC_LOST: 同步丢失中断
 *  @arg DW1000_INT_EVENT_RX_TIMEOUT: 接收超时中断
 *  @arg DW1000_INT_EVENT_SFD_TIMEOUT: SFD超时中断
 *  @arg DW1000_INT_EVENT_FRAME_REJECTED: 帧拒绝中断
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 2: SPI接口写操作失败
 * @note  该函数会清除原来的中断配置更新为写入的新中断配置
 */
uint8_t DW1000_InterruptSet(DW1000_Handle_t* handle, uint32_t event) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM((event & DW1000_INT_EVENT_FRAME_SENT) ||
                        (event & DW1000_INT_EVENT_FRAME_RECIVED) ||
                        (event & DW1000_INT_EVENT_RX_PHR_ERROR) ||
                        (event & DW1000_INT_EVENT_RX_CRC_ERROR) ||
                        (event & DW1000_INT_EVENT_RX_SYNC_LOST) ||
                        (event & DW1000_INT_EVENT_RX_TIMEOUT) ||
                        (event & DW1000_INT_EVENT_SFD_TIMEOUT) ||
                        (event & DW1000_INT_EVENT_FRAME_REJECTED));
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t systemEventMask;
    /* DW1000进入临界区，避免在设置中断时被产生的中断打断 */
    _DW1000_CriticalEnter(handle);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &event, sizeof(event)),
        return 2);
    /* DW1000退出临界区 */
    _DW1000_CriticalExit(handle);
    return 0;
}

/**
 * @brief DW1000中断使能函数
 * @param[in] handle  DW1000句柄
 * @param[in] event  中断事件，可以是以下的 | 组合
 *  @arg DW1000_INT_EVENT_FRAME_SENT: 帧发送完成中断
 *  @arg DW1000_INT_EVENT_FRAME_RECIVED: 帧接收完成中断
 *  @arg DW1000_INT_EVENT_RX_PHR_ERROR: PHR错误中断
 *  @arg DW1000_INT_EVENT_RX_CRC_ERROR: CRC错误中断
 *  @arg DW1000_INT_EVENT_RX_SYNC_LOST: 同步丢失中断
 *  @arg DW1000_INT_EVENT_RX_TIMEOUT: 接收超时中断
 *  @arg DW1000_INT_EVENT_SFD_TIMEOUT: SFD超时中断
 *  @arg DW1000_INT_EVENT_FRAME_REJECTED: 帧拒绝中断
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 * @note  该函数只会使能指定的中断事件，其他中断事件配置不变
 */
uint8_t DW1000_InterruptEnable(DW1000_Handle_t* handle, uint32_t event) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM((event & DW1000_INT_EVENT_FRAME_SENT) ||
                        (event & DW1000_INT_EVENT_FRAME_RECIVED) ||
                        (event & DW1000_INT_EVENT_RX_PHR_ERROR) ||
                        (event & DW1000_INT_EVENT_RX_CRC_ERROR) ||
                        (event & DW1000_INT_EVENT_RX_SYNC_LOST) ||
                        (event & DW1000_INT_EVENT_RX_TIMEOUT) ||
                        (event & DW1000_INT_EVENT_SFD_TIMEOUT) ||
                        (event & DW1000_INT_EVENT_FRAME_REJECTED));
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t systemEventMask;
    _DW1000_CriticalEnter(handle);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &systemEventMask, sizeof(systemEventMask)),
        return 1);
    DW1000_SET_BITS(systemEventMask, event);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &systemEventMask, sizeof(systemEventMask)),
        return 2);
    _DW1000_CriticalExit(handle);
    return 0;
}

/**
 * @brief DW1000中断禁用函数
 * @param[in] handle  DW1000句柄
 * @param[in] event  中断事件，可以是以下的 | 组合
 *  @arg DW1000_INT_EVENT_FRAME_SENT: 帧发送完成中断
 *  @arg DW1000_INT_EVENT_FRAME_RECIVED: 帧接收完成中断
 *  @arg DW1000_INT_EVENT_RX_PHR_ERROR: PHR错误中断
 *  @arg DW1000_INT_EVENT_RX_CRC_ERROR: CRC错误中断
 *  @arg DW1000_INT_EVENT_RX_SYNC_LOST: 同步丢失中断
 *  @arg DW1000_INT_EVENT_RX_TIMEOUT: 接收超时中断
 *  @arg DW1000_INT_EVENT_SFD_TIMEOUT: SFD超时中断
 *  @arg DW1000_INT_EVENT_FRAME_REJECTED: 帧拒绝中断
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 * @note  该函数只会使能指定的中断事件，其他中断事件配置不变
 */
uint8_t DW1000_InterruptDisable(DW1000_Handle_t* handle, uint32_t event) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM((event & DW1000_INT_EVENT_FRAME_SENT) ||
                        (event & DW1000_INT_EVENT_FRAME_RECIVED) ||
                        (event & DW1000_INT_EVENT_RX_PHR_ERROR) ||
                        (event & DW1000_INT_EVENT_RX_CRC_ERROR) ||
                        (event & DW1000_INT_EVENT_RX_SYNC_LOST) ||
                        (event & DW1000_INT_EVENT_RX_TIMEOUT) ||
                        (event & DW1000_INT_EVENT_SFD_TIMEOUT) ||
                        (event & DW1000_INT_EVENT_FRAME_REJECTED));
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t systemEventMask;
    _DW1000_CriticalEnter(handle);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &systemEventMask, sizeof(systemEventMask)),
        return 1);
    DW1000_CLEAR_BITS(systemEventMask, event);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &systemEventMask, sizeof(systemEventMask)),
        return 2);
    _DW1000_CriticalExit(handle);
    return 0;
}


/**
 * @brief
 * @param[in] handle  Comment
 * @param[in] enable  Comment
 *  @arg DW1000_EVENT_COUNT_ENABLE: 启用事件计数器
 *  @arg DW1000_EVENT_COUNT_DISABLE: 禁用事件计数器
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: SPI接口读操作失败
 *  @arg 2: SPI接口写操作失败
 * @note  备注
 */
uint8_t DW1000_EventCountersConfig(DW1000_Handle_t* handle, uint8_t enable) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(enable == DW1000_EVENT_COUNT_ENABLE ||
                        enable == DW1000_EVENT_COUNT_DISABLE);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t eventCounterCtrl;
    /* 需要先清零和失能，不能直接清除 */
    DW1000_WRITE_REG(eventCounterCtrl, DW1000_EVENT_COUNT_CLEAR);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_DIG_DIAG, DW1000_SUB_REG_EVC_CTRL_OFFSET,
                          (uint8_t*) &eventCounterCtrl, sizeof(eventCounterCtrl)),
        return 2);
    /* 根据配置设置相应位 */
    DW1000_MODIFY_REG(eventCounterCtrl, DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN_MSK, enable);
    return _DW1000_SPI_Write(handle, DW1000_REG_DIG_DIAG, DW1000_SUB_REG_EVC_CTRL_OFFSET, (uint8_t*) &eventCounterCtrl, sizeof(eventCounterCtrl));
}

/**
 * @brief
 * @param[in] handle  DW1000句柄
 * @param[in] counters  Comment
 * @return uint8_t 执行结果
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 将函数修改为读取数据到句柄内部的字段里
 */
uint8_t DW1000_EventCountersRead(DW1000_Handle_t* handle, DW1000_EVT_CNT_t* counters) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(counters != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t* ptr;
    /* 因为结构体的成员是按照DW1000芯片寄存器顺序排列的，可以直接给整个结构体赋值 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_DIG_DIAG, DW1000_SUB_REG_EVC_PHE_OFFSET, (uint8_t*) counters, sizeof(DW1000_EVT_CNT_t)),
        return 1);
    /* 有计数器长度为12bit，所以高4位需要清除 */
    for (ptr = (uint16_t*) counters; ptr < (uint16_t*) counters + sizeof(DW1000_EVT_CNT_t) / sizeof(uint16_t); ptr++) {
        *ptr &= 0x0FFF;
    }
    return 0;
}

/**
 * @brief 软件复位DW1000
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_Softreset(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t data;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SequeningDisable(handle),
        return 3);
    /* 清除 AON 自动下载位（当复位时会触发AON下载） */
    DW1000_WRITE_REG(data, 0x00);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_WCFG_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 清除 wakeup 配置 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_WCFG_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 上传新的配置 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_AON_ArrayUpload(handle),
        return 4);
    /* 复位 HIF，TX，RX 和 PMSC（复位位置位） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_3_OFFSET, (uint8_t*) &data, 1),
        return 1);
    /* 延迟10us等待DW1000 PLL时钟锁定 */
    handle->delay_ms(1);
    /* 清除复位位 */
    DW1000_WRITE_REG(data, 0xF0);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_3_OFFSET, (uint8_t*) &data, 1),
        return 2);
    handle->init.isRxOnAfterTx = 0;
    return 0;
}

/**
 * @brief 调整晶振频率
 * @param[in] handle  Comment
 * @param[in] trimValue  晶振修正值（取值0x00-0x1F）31级（每级约1.5ppm）
 * @return uint8_t uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_XtalTrimSet(DW1000_Handle_t* handle, uint8_t trimValue) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(trimValue <= 0x1F);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 高3位为保留位，必须设置为 011 */
    DW1000_MODIFY_REG(trimValue, 0xE0, 0x60);
    return _DW1000_SPI_Write(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_XTALT_OFFSET, (uint8_t*) &trimValue, sizeof(trimValue));
}

/**
 * @brief DW1000获取晶振修正值
 * @param[in] handle  DW1000句柄
 * @param[in] trimValue  获取到的晶振修正值
 * @return uint8_t 执行结果
 * @note  备注
 */
uint8_t DW1000_XtalTrimGet(DW1000_Handle_t* handle, uint8_t* trimValue) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(trimValue != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_XTALT_OFFSET, trimValue, sizeof(*trimValue)),
        return 1);
    /* 把高三位清零0（因为是保留位） */
    DW1000_CLEAR_BITS(*trimValue, 0xE0);
    return 0;
}

/**
 * @brief 设置DW1000在指定频道上发送连续波信号
 * @param[in] handle  DW1000句柄
 * @param[in] channel  频道号 取值1-7，不包含6
 *            @arg DW1000_CHANNEL_1: 频道1
 *            @arg DW1000_CHANNEL_2: 频道2
 *            @arg DW1000_CHANNEL_3: 频道3
 *            @arg DW1000_CHANNEL_4: 频道4
 *            @arg DW1000_CHANNEL_5: 频道5
 *            @arg DW1000_CHANNEL_7: 频道7
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 */
uint8_t DW1000_CW_ModeConfig(DW1000_Handle_t* handle, uint8_t channel) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(IS_DW1000_CHANNEL(channel));
    DW1000_ASSERT_PARAM(channel <= 7 && channel > 0 && channel != 6);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t data;
    /* 禁用 TX/RX RF块sequnecing（CW模式需要） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SequeningDisable(handle),
        return 3);
    /* 配置RF_PLL（对于给定的频道） */
    DW1000_WRITE_REG(data, DW1000_FS_PLL_CFG[channel]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_PLLCFG_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 配置 PLL 调谐 */
    DW1000_WRITE_REG(data, DW1000_FS_PLL_TUNE[channel]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_PLLTUNE_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 配置 RF TX 控制寄存器 */
    DW1000_WRITE_REG(data, DW1000_TX_CONFIG[channel]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_TXCTRL_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 使能 RF PLL */
    /* 使能 LDO 和 RF PLL 块 */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_ALL_PLL_ENABLE | DW1000_RF_CFG_ALL_LDO_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 使能TX块复位 */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_SWITCH_TX | DW1000_RF_CFG_ALL_LDO_ENABLE | DW1000_RF_CFG_ALL_PLL_ENABLE | DW1000_RF_CFG_TXB_ALL_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 配置TX块 */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL | DW1000_TX_CLOCK_SELECT_FORCE_PLL);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    DW1000_WRITE_REG(data, 0x07); /* 该3位应该也是有关系的，但是手册里只记录了ADCCE位，其余位没有说明 */
    /* 禁用 精细颗粒 Tx 时序流程 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        DW1000_FineGrainTxSeqCmd(handle, DW1000_TX_FINE_GRAIN_SEQ_DISABLE),
        return 4);
    /* 配置CW模式 */
    DW1000_WRITE_REG(data, DW1000_TC_PGTEST_MODE_CW);
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PGTEST_OFFSET, (uint8_t*) &data, sizeof(uint8_t));
}

/**
 * @brief 此功能将DW1000设置为连续发送帧模式，以便进行监管审批测试
 * @param[in] handle  DW1000句柄
 * @param[in] repetition 重复间隔（两次发送的间隔），最小为4，单位大概为 8ns(更精确的值为 512/499.2e6/128 s)
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 */
uint8_t DW1000_CF_ModeConfig(DW1000_Handle_t* handle, uint32_t repetition) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(repetition >= 4);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t data;
    /* 禁用 TX/RX RF块时序流程（连续帧模式要求） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SequeningDisable(handle),
        return 3);
    /* 使能 RF PLL 和 TX 块 */
    /* 使能 LDO 和 RF PLL 块 */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_ALL_PLL_ENABLE | DW1000_RF_CFG_ALL_LDO_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 使能TX块复位 */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_SWITCH_TX | DW1000_RF_CFG_ALL_LDO_ENABLE | DW1000_RF_CFG_ALL_PLL_ENABLE | DW1000_RF_CFG_TXB_ALL_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 配置TX块 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL),
        return 4);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_TxClockCmd(handle, DW1000_TX_CLOCK_SELECT_FORCE_PLL),
        return 5);
    /* 设置发送重复间隔 */
    if (repetition < 4) {
        repetition = 4;
    }
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RX_TIME, 0, (uint8_t*) &repetition, sizeof(repetition)),
        return 2);
    /* 配置Tx连续帧 */
    DW1000_WRITE_REG(data, DW1000_TEST_MODE_TX_PWR_SPECTRUM_ENABLE);
    return _DW1000_SPI_Write(handle, DW1000_REG_DIG_DIAG, DW1000_SUB_REG_DIAG_TMC_OFFSET, (uint8_t*) &data, sizeof(uint8_t));
}

/**
 * @brief 读取温度与电压原始值
 * @param[in] handle  DW1000句柄
 * @param[in] SPI_isFast: 当前SPI是否处于高速率模式（以3MHz为分界线）
 *                        0: 非高速  其他: 高速
 * @param[out] temp  温度原始值
 * @param[out] volt  电压原始值
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @attention 为了正确读取温度，应使用晶振时钟进行读取
 *            然而这意味着接收器将被关闭，如果需要接收器
 *            处于开启装填状态，那么将使用定时器来确保读
 *            取之前数值已稳定
 * @note  备注
 * @todo 未来打算弃用该函数，使用 DW1000_RawTempVoltReadInSlowSpi 和 DW1000_RawTempVoltReadInFastSpi进行替代
 */
uint8_t DW1000_RawTempVoltRead(DW1000_Handle_t* handle, uint8_t SPI_isFast, uint8_t* temp, uint8_t* volt) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(temp != NULL);
    DW1000_ASSERT_PARAM(volt != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t data;
    /* 以下操作需要单字节按顺序写入 */
    DW1000_WRITE_REG(data, 0x80); /* 使能 TLD 偏置 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x11, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    DW1000_WRITE_REG(data, 0x0A); /* 使能 TLD 偏置和 ADC 偏置 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x12, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    DW1000_WRITE_REG(data, 0x0F); /* 使能输出（仅当偏置启动并运行后） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x12, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    if (SPI_isFast) {
        /* 读取所有SAR输入 */
        DW1000_WRITE_REG(data, 0x00);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
            return 2);
        DW1000_WRITE_REG(data, 0x01); /* 设置 SAR 使能 */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
            return 2);
        /* 当时用PLL时钟（以及高速SPI通信），延迟是必须的 */
        handle->delay_ms(1);
        /* 读取电压和温度 */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARL_OFFSET, (uint8_t*) &data, sizeof(data)),
            return 2);
    } else { /* 切换为低速时钟 */
        /* 注意：设置系统为外部晶振（必须执行该步骤以确保读取的值是可靠的） */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI),
            return 3);
        /* 读取所有SAR输入 */
        DW1000_WRITE_REG(data, 0x00);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
            return 2);
        DW1000_WRITE_REG(data, 0x01); /* 设置 SAR 使能 */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
            return 2);
        /* 读取电压和温度 */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARL_OFFSET, (uint8_t*) &data, sizeof(data)),
            return 1);
        /* 恢复系统默认时钟 */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ),
            return 3);
    }
    *volt = data;      /* 低8位电压值 */
    *temp = data >> 8; /* 高8位温度值 */
    /* 清除 SAR 使能 */
    DW1000_WRITE_REG(data, 0x00);
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t));
}

/**
 * @brief DW1000在低速SPI下读取原始温度与电压数据
 * @param[in] handle  DW1000句柄
 * @param[in] temp  用于存储原始温度值
 * @param[in] volt  用于存储原始电压值
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_RawTempVoltReadInSlowSpi(DW1000_Handle_t* handle, uint8_t* temp, uint8_t* volt) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(temp != NULL);
    DW1000_ASSERT_PARAM(volt != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t data;
    /* 以下操作需要单字节按顺序写入 */
    DW1000_WRITE_REG(data, 0x80); /* 使能 TLD 偏置 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x11, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    DW1000_WRITE_REG(data, 0x0A); /* 使能 TLD 偏置和 ADC 偏置 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x12, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    DW1000_WRITE_REG(data, 0x0F); /* 使能输出（仅当偏置启动并运行后） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x12, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 切换为低速时钟 */
    /* 注意：设置系统为外部晶振（必须执行该步骤以确保读取的值是可靠的） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI),
        return 3);
    /* 读取所有SAR输入 */
    DW1000_WRITE_REG(data, 0x00);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    DW1000_WRITE_REG(data, 0x01); /* 设置 SAR 使能 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 读取电压和温度 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARL_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 1);
    /* 恢复系统默认时钟 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ),
        return 3);
    *volt = data;      /* 低8位电压值 */
    *temp = data >> 8; /* 高8位温度值 */
    /* 清除 SAR 使能 */
    DW1000_WRITE_REG(data, 0x00);
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t));
}

/**
 * @brief DW1000在高速SPI下读取原始温度与电压数据
 * @param[in] handle  Comment
 * @param[in] temp  Comment
 * @param[in] volt  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_RawTempVoltReadInFastSpi(DW1000_Handle_t* handle, uint8_t* temp, uint8_t* volt) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(temp != NULL);
    DW1000_ASSERT_PARAM(volt != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint16_t data;
    /* 以下操作需要单字节按顺序写入 */
    DW1000_WRITE_REG(data, 0x80); /* 使能 TLD 偏置 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x11, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    DW1000_WRITE_REG(data, 0x0A); /* 使能 TLD 偏置和 ADC 偏置 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x12, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    DW1000_WRITE_REG(data, 0x0F); /* 使能输出（仅当偏置启动并运行后） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x12, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 读取所有SAR输入 */
    DW1000_WRITE_REG(data, 0x00);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    DW1000_WRITE_REG(data, 0x01); /* 设置 SAR 使能 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 当时用PLL时钟（以及高速SPI通信），延迟是必须的 */
    handle->delay_ms(1);
    /* 读取电压和温度 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARL_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 1);
    *volt = data;      /* 低8位电压值 */
    *temp = data >> 8; /* 高8位温度值 */
    /* 清除 SAR 使能 */
    DW1000_WRITE_REG(data, 0x00);
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t));
}


#define DW1000_TEMP_RAW_TO_REAL_COEFF 1.14F

inline float DW1000_TempRawToRealConvert(DW1000_Handle_t* handle, uint8_t rawTemp) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /*
        根据 DW1000 数据手册 V2.13 中的公式进行转换:
        Temperature (°C) = ( (SAR_LTEMP – OTP_READ(Vtemp @ 23°C) ) x 1.14) + 23
    */
    return ((rawTemp - handle->backup.refTemp) * DW1000_TEMP_RAW_TO_REAL_COEFF) + 23;
}

// /* 0.1是对原先浮点温度的放大倍数10，这里/10进行还原，乘256,是将数字放大256倍进行转换计算，提高精度 */
// #define DW1000_TEMP_REAL_TO_RAW_CONV  (1 / DW1000_TEMP_RAW_TO_REAL_COEFF)

/**
 * @brief
 * @param[in] handle  Comment
 * @param[in] realTemp_x10  Comment
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_TempRealToRawConvert(DW1000_Handle_t* handle, int16_t realTemp_x10) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /*
        根据 DW1000 数据手册 V2.13 中的公式进行转换:
        SAR_LTEMP = ( (Temperature (°C) - 23) / 1.14 ) + OTP_READ(Vtemp @ 23°C)
    */
    /* 放大256倍用于提高精度 */
    int32_t rawTemp = ((realTemp_x10 - 230 + 5) / 10 / DW1000_TEMP_RAW_TO_REAL_COEFF * 256); /* +5是为了四舍五入 */
    /* 在上面的转换中数值被放大了256倍，这里进行还原，同时确保符号位正确 */
    rawTemp = (-rawTemp >> 8);
    rawTemp = -rawTemp;
    return (uint8_t) (rawTemp + handle->backup.refTemp);
}

#define DW1000_VOLT_REAL_TO_RAW_COEFF 137

inline float DW1000_VoltRawToRealConvert(DW1000_Handle_t* handle, uint8_t rawVolt) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 根据 DW1000 数据手册 V2.13 中的公式进行转换:
        Voltage (V) = ( (SAR_LVBAT – OTP_READ(Vmeas @ 3.3 V) ) / 173 ) + 3.3
    */
    return ((float) (rawVolt - handle->backup.refVolt) / DW1000_VOLT_REAL_TO_RAW_COEFF + 3.3F);
}

uint8_t DW1000_VoltRawToRealConvert(DW1000_Handle_t* handle, int32_t realVolt_x1000) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 根据 DW1000 数据手册 V2.13 中的公式进行转换:
        SAR_LVBAT = ( (Voltage (V) - 3.3) * 173 ) + OTP_READ(Vmeas @ 3.3 V)
    */
    int32_t rawVolt = ((realVolt_x1000 - 3300 + 50) / 100 * DW1000_VOLT_REAL_TO_RAW_COEFF); /* +50是为了四舍五入 */
    return (uint8_t) (rawVolt + handle->backup.refVolt);
}

inline uint8_t DW1000_TempReadOnWakeUp(DW1000_Handle_t* handle, uint8_t* temp) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(temp != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_SAR_LTEMP_OFFSET, temp, sizeof(*temp));
}

inline uint8_t DW1000_VoltReadOnWakeUp(DW1000_Handle_t* handle, uint8_t* volt) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(volt != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    return _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_SAR_LVBAT_OFFSET, volt, sizeof(*volt));
}

/**
 * @brief DW1000 带宽温度补偿计算
 * @param[in] handle  Comment
 * @param[in] target  Comment
 * @param[in] best  Comment
 * @return uint8_t
 * @note  备注
 * @attention  SPI通信速率必须低于3MHz
 * The sleep to allow the calibration to complete is set to 1ms here, but can be as low as 10us.
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_BW_TempCompCalculate(DW1000_Handle_t* handle, uint16_t target, uint32_t* best) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(best != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t data;
    /* 存储相关寄存器变量用于计算完成后复原 */
    uint8_t oldPmscCtrl0;
    uint16_t oldPmscCtrl1;
    uint32_t oldRfConf;
    /* 保存当前寄存器的值（用于后续恢复） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &oldPmscCtrl0, sizeof(oldPmscCtrl0)),
        return 1);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &oldPmscCtrl1, sizeof(oldPmscCtrl1)),
        return 1);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &oldRfConf, sizeof(oldRfConf)),
        return 1);
    /* 设置时钟为XTAL */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI); /* 19MHz */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 禁用时序流程 */
    DW1000_WRITE_REG(data, DW1000_PMSC_ANALOG_RF_CTRL_DISABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) data, sizeof(uint16_t)),
        return 2);
    /* 打开CLK PLL，Mix偏置 和 PG */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_ALL_LDO_ENABLE | DW1000_RF_CFG_PULSE_GEN_ENABLE | DW1000_RF_CFG_TXB_MIXER_BIAS_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 将系统和Tx时钟设置为PLL */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL | DW1000_TX_CLOCK_SELECT_FORCE_PLL); /* 125MHz */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 从最高位（bit8）开始猜每一位的0和1（本质上就是二分法搜索） */
    uint8_t BW_current;
    uint8_t BW_best;
    /* 初始最低增量是我们允许计数值与目标值之间的最大差值。如果算法成功，当计数值更接近目标时，它将被一个较小的值覆盖 */
    int32_t countDeltaMin = 300;
    uint16_t countRaw;
    uint16_t countDelta;
    /* 从 bit7 开始测试（根据之前测试，最高位（bit8）肯定为1，所以直接开始猜测下一位） */
    for (uint8_t i = 0x40; i != 0; i >>= 1) {
        BW_current |= i;
        /* 往PG_DELAY寄存器写入猜测的BW值 */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PGDELAY_OFFSET, (uint8_t*) &BW_current, sizeof(BW_current)),
            return 2);
        /* 设置计算方向和时间 */
        DW1000_WRITE_REG(data, DW1000_PG_CTRL_DIR_CONV_HIGHER | DW1000_PG_TMEAS_MAX);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_CTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
            return 2);
        /* 开始计算 */
        DW1000_SET_BITS(data, DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_CTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
            return 2);
        /* 等待计算完成 */
        handle->delay_ms(1);
        /*
            读取计算结果
            从 TC_PG_STATUS 寄存器读取结果
        */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_STATUS_OFFSET, (uint8_t*) &countRaw, sizeof(countRaw)),
            return 1);
        /* 跟踪最接近目标的值，避免超调 */
        if (countRaw > target) {
            countDelta = countRaw - target;
        } else {
            countDelta = target - countRaw;
        }
        if (countDelta < countDeltaMin) {
            countDeltaMin = countDelta;
            BW_best = BW_current;
        }
        /* BW值太大，清除当前位 */
        if (countRaw < target) {
            DW1000_CLEAR_BITS(BW_current, i);
        }
    }
    /* 将最佳值返回 */
    *best = BW_best;
    /* 恢复之前保存的寄存器值 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &oldPmscCtrl0, sizeof(oldPmscCtrl0)),
        return 2);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &oldPmscCtrl1, sizeof(oldPmscCtrl1)),
        return 2);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &oldRfConf, sizeof(oldRfConf)),
        return 2);
    return 0;
}

#define DW1000_DA_ATTN_STEP     (2.5)
#define DW1000_MIXER_GAIN_STEP  (0.5)
#define DW1000_MIXER_DA_FRACTOR (DW1000_DA_ATTN_STEP / DW1000_MIXER_GAIN_STEP)

/**
 * @brief 该函数计算对TX_POWER寄存器的适当更改，以补偿不同温度下的TX功率输出。
 * @param[in] txPowerRef  在进行参考测量时记录的TX_POWER寄存器值
 * @param[in] powerAdj  功率电平调整，每级调整0.5dB
 * @return uint32_t 需编程到TX_POWER寄存器中的温度补偿值
 * @note  ATTN = attenuation 衰减
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint32_t _DW1000_TxPowerTempCompCompute(uint32_t txPowerRef, int32_t powerAdj) {
    int8_t DA_ATTN_change;
    int8_t mixerGainChange;
    uint8_t DA_ATTN_current;
    uint8_t mixerGainCurrent;
    uint8_t DA_ATTN_new;
    uint8_t mixerGainNew;
    uint32_t result;
    for (uint8_t i = 0; i < 4; i++) {
        DA_ATTN_change = 0;
        mixerGainChange = powerAdj;
        DA_ATTN_current = ((txPowerRef >> (i * 8)) & 0xE0) >> 5; /* 按字节取高3位 */
        mixerGainCurrent = (txPowerRef >> (i * 8)) & 0x1F;       /* 按字节取低5位 */
        /* Mixer Gain 增益值在[4,20]之间表现最好 */
        while ((mixerGainCurrent + mixerGainChange < 4) || (mixerGainCurrent + mixerGainChange > 20)) {
            /* 如果 Mixer 增益值超出范围，调整 DA 衰减进行补偿 */
            if (mixerGainCurrent + mixerGainChange > 20) {
                /* 如果DA衰减达到最大值 */
                if (--DA_ATTN_change == 0) {
                    /* 恢复上一次的值并退出循环，DA已达到最大值 */
                    DA_ATTN_change = 1;
                    break;
                }
                mixerGainChange -= (int8_t) DW1000_MIXER_DA_FRACTOR;
            } else if (mixerGainCurrent + mixerGainChange < 4) {
                /* 如果DA衰减达到最小值 */
                if (++DA_ATTN_change == 8) {
                    /* 恢复上一次的值并退出循环，DA已达到最小值 */
                    DA_ATTN_change = 7;
                    break;
                }
                mixerGainChange += (int8_t) DW1000_MIXER_DA_FRACTOR;
            }
        }
        DA_ATTN_new = (DA_ATTN_current + DA_ATTN_change) & 0x07;
        mixerGainNew = (mixerGainCurrent + mixerGainChange) & 0x1F;
        result |= ((uint32_t) (DA_ATTN_new << 5 | mixerGainNew) << (i * 8));
    }
    return result;
}

#define DW1000_TEMP_COMP_FACTOR_CH2 (327) // ((int16_t)(0.0798*4096))
#define DW1000_TEMP_COMP_FACTOR_CH5 (607) // ((int16_t)(0.1482*4096))

/**
 * @brief 此函数用于确定随温度变化的DW1000的校正功率设置（TX_POWER设置）
 * @param[in] channel  功率电平补偿的通道：2 或 5
 * @param[in] txPowerRef 在进行参考测量时记录的TX_POWER寄存器值
 * @param[in] tempDelta  当前环境温度（原始值单位）与参考测量时温度（原始值单位）之间的差值
 * @return uint32_t 已校正的TX_POWER寄存器值
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint32_t DW1000_TxPowerTempCompCalculate(uint8_t channel, uint32_t txPowerRef, int32_t tempDelta) {
    int8_t powerDelta;
    uint8_t isNegative = 0;
    if (tempDelta < 0) {
        isNegative = 1;
        tempDelta = -tempDelta;
    }
    /* 计算当前温度下的预期功率差 */
    if (channel == 5) {
        powerDelta = ((tempDelta * DW1000_TEMP_COMP_FACTOR_CH5) >> 12); /* 除以4096 */
    } else if (channel == 2) {
        powerDelta = ((tempDelta * DW1000_TEMP_COMP_FACTOR_CH2) >> 12); /* 除以4096 */
    } else {
        /* 其他信道不进行温度补偿 */
        powerDelta = 0;
    }
    if (isNegative) {
        powerDelta = -powerDelta;
    }
    if (powerDelta == 0) {
        return txPowerRef;
    }
    return _DW1000_TxPowerTempCompCompute(txPowerRef, powerDelta);
}
/* 定义采样次数，通过求平均进行平滑以消除噪声干扰 */
#define DW1000_SAMPLE_NUM 10

/**
 * @brief 此函数用于计算给定PG_DELAY时脉冲发生器计数器寄存器（PGC_STATUS）中的值
          这是用来进行参考测量的，记录下的参考值用于在温度变化时调整设备的带宽。
 * @param[in] handle  DW1000句柄
 * @param[in] pgdly  设置PG_DELAY（以控制带宽），并找到相应的计数值
 * @param[in] delayCount  根据提供的PG_DELAY值计算出的PGC_STATUS计数值——用作后续带宽调整的参考
 * @return uint8_t 执行结果 0: 执行成功 其他: 执行错误
 * @note  备注
 * @attention  SPI通信频率必须低于3MHz
 */
uint8_t DW1000_PG_CounterCalculate(DW1000_Handle_t* handle, uint8_t pgdly, uint16_t* delayCount) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(delayCount != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t data;
    /* 暂存相关寄存器的值便于之后复原 */
    uint8_t oldPmscCtrl0;
    uint16_t oldPmscCtrl1;
    uint32_t oldRfConf;
    /* 保存当前寄存器的值（用于后续恢复） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &oldPmscCtrl0, sizeof(oldPmscCtrl0)),
        return 1);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &oldPmscCtrl1, sizeof(oldPmscCtrl1)),
        return 1);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &oldRfConf, sizeof(oldRfConf)),
        return 1);
    /* 设置时钟为XTAL */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI); /* 19MHz */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 禁用时序流程 */
    DW1000_WRITE_REG(data, DW1000_PMSC_ANALOG_RF_CTRL_DISABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) data, sizeof(uint16_t)),
        return 2);
    /* 打开CLK PLL，Mix偏置 和 PG */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_ALL_LDO_ENABLE | DW1000_RF_CFG_PULSE_GEN_ENABLE | DW1000_RF_CFG_TXB_MIXER_BIAS_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 将系统和Tx时钟设置为PLL */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL | DW1000_TX_CLOCK_SELECT_FORCE_PLL); /* 125MHz */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    uint32_t sum = 0;
    uint16_t count;
    for (uint8_t i = 0; i < DW1000_SAMPLE_NUM; i++) {
        /* 往PG_DELAY寄存器写入PG延迟值 */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PGDELAY_OFFSET, (uint8_t*) &pgdly, sizeof(pgdly)),
            return 2);
        /* 设置计算方向和时间 */
        DW1000_WRITE_REG(data, DW1000_PG_CTRL_DIR_CONV_HIGHER | DW1000_PG_TMEAS_MAX);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_CTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
            return 2);
        /* 开始计算 */
        DW1000_SET_BITS(data, DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_CTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
            return 2);
        /* 等待计算完成 */
        handle->delay_ms(1);
        /*
            读取计算结果
            从 TC_PG_STATUS 寄存器读取结果
        */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_STATUS_OFFSET, (uint8_t*) &count, sizeof(count)),
            return 1);
        sum += count;
    }
    *delayCount = sum / DW1000_SAMPLE_NUM;
    /* 恢复之前保存的寄存器值 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &oldPmscCtrl0, sizeof(oldPmscCtrl0)),
        return 2);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &oldPmscCtrl1, sizeof(oldPmscCtrl1)),
        return 2);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &oldRfConf, sizeof(oldRfConf)),
        return 2);
    return 0;
}

/**
 * @brief 禁用LDE算法
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果 0; 执行成功 其他: 执行错误
 * @note  备注
 */
uint8_t DW1000_LDE_AlgoDisable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t data;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC,
                         DW1000_SUB_REG_PMSC_CTRL1_2_OFFSET,
                         &data, sizeof(data)),
        return 1);
    DW1000_CLEAR_BITS(data, DW1000_SUB_REG_PMSC_CTRL1_2_BIT_LDERUNE_MSK);
    return _DW1000_SPI_Read(handle, DW1000_REG_PMSC,
                            DW1000_SUB_REG_PMSC_CTRL1_2_OFFSET,
                            &data, sizeof(data));
}

/**
 * @brief 使能LDE算法
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果 0; 执行成功 其他: 执行错误
 * @note  备注
 */
uint8_t DW1000_LDE_AlgoEnable(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    uint8_t data;
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_PMSC,
                         DW1000_SUB_REG_PMSC_CTRL1_2_OFFSET,
                         &data, sizeof(data)),
        return 1);
    DW1000_SET_BITS(data, DW1000_SUB_REG_PMSC_CTRL1_2_BIT_LDERUNE_MSK);
    return _DW1000_SPI_Read(handle, DW1000_REG_PMSC,
                            DW1000_SUB_REG_PMSC_CTRL1_2_OFFSET,
                            &data, sizeof(data));
}

#define DW1000_XTAL_TRIM_MIDRANG 0x10

// uint8_t DW1000_DeInit(DW1000_Handle_t* handle){
//     #if (USE_DW1000_FULL_ASSERT == 1U)
//     DW1000_ASSERT_PARAM(handle != NULL);
//     #endif /* USE_DW1000_FULL_ASSERT */
// }


/**
 * @brief DW1000准备函数
 * @param[in] handle  Comment
 * @return uint8_t 执行结果
 *  @arg 0: 执行成功
 *  @arg 1: 执行失败
 *  @arg 11: 设备ID校验失败
 * @note  在进行进行具体初始化前的准备工作
 * @attention SPI通信速率必须小于3MHz
 */
uint8_t DW1000_Prepare(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t data;
    /* 在执行下面流程前需要确保DW1000的SPI通信速率小于3MHz */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_LowSpeedSet(handle),
        return 3);
    /* 读取设备ID */
    DW1000_EXEC_WITH_ERR_HANDLE(
        DW1000_DEV_ID_Get(handle, &data),
        return 4);
    /* 如果读取到的设备ID有误则直接退出 */
    if (data != DW1000_DEV_ID) {
        return 5;
    }
    /* 软复位DW1000（确保在执行准备工作流程前DW1000已完全复位） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        DW1000_Softreset(handle),
        return 6);
    /* 将时钟切换为XTI以确保后面读取OTP内的值可靠 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI),
        return 7);

    /* 配置 PLL 时钟锁定检测调谐 */
    DW1000_WRITE_REG(data, DW1000_PLL_LOCK_DETECT_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_EXT_SYNC,
                          DW1000_SUB_REG_EC_CTRL_OFFSET,
                          (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* DW1000在上电初始化时应从OTP中读取LDO值 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_LDO_TUNE, &data),
        return 8);
    /* 如果最低8位有值 */
    if (DW1000_READ_BITS(data, 0xFF)) {
        DW1000_WRITE_REG(data, DW1000_SUB_REG_OTP_SF_BIT_LDO_KICK_MSK);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF,
                              DW1000_SUB_REG_OTP_SF_OFFSET,
                              (uint8_t*) data, sizeof(uint8_t)),
            return 2);
        /* 记录设置，便于之后配置睡眠模式时使用 */
        DW1000_SET_BITS(handle->backup.sleepMode, DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDO_MSK);
    }

    /* 读取OTP修订号和晶振微调值 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_OTP_REV_XTAL_TRIM, &data),
        return 8);
    uint8_t xtalTrim = data & 0x1F; // data的低8位是XTAL_Trim值（实际上只有低5位有效）
    /* 备份OTP修订号 */
    handle->backup.otpRevision = data >> 8; // data的[15:0]是OTP Revision值
    /* 如果该值为0说明没有微调值 */
    if (!xtalTrim) {
        /* 如果没有微调值就是设置为中值 */
        xtalTrim = DW1000_XTAL_TRIM_MIDRANG;
    }
    /* 设置XTAL 微调值 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        DW1000_XtalTrimSet(handle, xtalTrim),
        return 9);

    /* 根据配置选择是否从OTP中读取partID */
    if (handle->init.shouldOtpPartIdRead) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_PART_ID, &data),
            return 8);
        handle->backup.partID = data;
    }
    /* 根据配置选择是否从OTP中读取LotID */
    if (handle->init.shouldOtpLotIdRead) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_LOT_ID, &data),
            return 8);
        handle->backup.lotID = data;
    }
    /* 根据配置选择是否从OTP中读取refVolt */
    if (handle->init.shouldOtpRefVoltRead) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_REF_VOLT, &data),
            return 8);
        handle->backup.refVolt = data;
    }
    /* 根据配置选择是否从OTP中读取refTemp */
    if (handle->init.shouldOtpRefTempRead) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_REF_TEMP, &data),
            return 8);
        handle->backup.refTemp = data;
    }
    /* 根据配置选择是否从OTP中读取微码 */
    if (handle->init.shouldOtpUcodeRead) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_LoadUcodeFromROM(handle),
            return 10);
        /* 如果在初始化时加载了微码在唤醒阶段也需要加载微码 */
        DW1000_SET_BITS(handle->backup.sleepMode, DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDE_MSK);
    } else { // 如果不加载微码就需要禁用LDE
        DW1000_EXEC_WITH_ERR_HANDLE(
            DW1000_LDE_AlgoDisable(handle),
            return 11);
    }
    /* 系统时钟切换为sequencing */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ),
        return 7);
    /* 位于AON_CFG1寄存器的这3个位必须被清零以确保DW1000在深睡眠模式下正确运行 */
    DW1000_WRITE_REG(data, 0x00);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON,
                          DW1000_SUB_REG_AON_CFG1_OFFSET,
                          (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 读取系统配置寄存器并将值进行备份 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &data, sizeof(data)),
        return 1);
    handle->backup.systemConfig = data;

    // 【此处官方还有保存了一个关于长帧的标志位，个人认为用不上，所以省略】

    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_TX_FCTRL, 0, (uint8_t*) &data, sizeof(data)),
        return 1);
    handle->backup.txFctrl = data;

    /* 准备工作完成，可以将SPI速率恢复到高速率 */
    return _DW1000_SPI_HighSpeedSet(handle);
}

uint8_t DW1000_PrepareAfterWakeUp(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle);
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t data;
    /* 在执行下面流程前需要确保DW1000的SPI通信速率小于3MHz */
    DW1000_EXEC_WITH_ERR_HANDLE(_DW1000_SPI_LowSpeedSet(handle), return 3);
    /* 读取设备ID */
    DW1000_EXEC_WITH_ERR_HANDLE(DW1000_DEV_ID_Get(handle, &data), return 4);
    /* 如果读取到的设备ID有误则直接退出 */
    if (data != DW1000_DEV_ID) {
        return 11;
    }
    /* 当DW1000从睡眠中唤醒时不要复位DW1000 */

    if (
        !handle->init.shouldOtpRefTempRead &&
        !handle->init.shouldOtpRefVoltRead &&
        !handle->init.shouldOtpLotIdRead &&
        !handle->init.shouldOtpPartIdRead &&
        !handle->init.shouldOtpRevReadAfterWakeUp) {
        /* 设置系统时钟为XTI */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SystemClockSelect(handle, DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI),
            return 5);
    }
    /* 配置 PLL 时钟锁定检测调谐 */
    DW1000_WRITE_REG(data, DW1000_PLL_LOCK_DETECT_ENABLE);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_EXT_SYNC, DW1000_SUB_REG_EC_CTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 在DW1000唤醒后调用该函数时，不用读取再从OTP中读取LDO，因为已在上电初始化时完成了 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_LDOTUNE_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 1);
    /* 如果LDO_TUNE寄存器内已有非默认值，说明已经从OTP中读取过LDO值了，只需记录设置 */
    if (data != DW1000_LDO_TUNE_DEFAULT) {
        DW1000_SET_BITS(handle->backup.sleepMode, DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDO_POS);
    }
    /* 根据配置选择是否读取OTP修订号 */
    if (handle->init.shouldOtpRevReadAfterWakeUp) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_OTP_REV, &data),
            return 6);
        handle->backup.otpRevision = (data >> 8);
    } else {
        /* 如果不需要OTP修订好，在唤醒后调用该函数将会把该字段清零 */
        handle->backup.otpRevision = 0;
    }
    /* 根据配置选择是否从OTP中读取partID */
    if (handle->init.shouldOtpPartIdRead) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_PART_ID, &data),
            return 6);
        handle->backup.partID = data;
    }
    /* 根据配置选择是否从OTP中读取LotID */
    if (handle->init.shouldOtpLotIdRead) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_LOT_ID, &data),
            return 6);
        handle->backup.lotID = data;
    }
    /* 根据配置选择是否从OTP中读取refVolt */
    if (handle->init.shouldOtpRefVoltRead) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_REF_VOLT, &data),
            return 6);
        handle->backup.refVolt = data;
    }
    /* 根据配置选择是否从OTP中读取refTemp */
    if (handle->init.shouldOtpRefTempRead) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_OTP_Read(handle, DW1000_OTP_ADDRESS_REF_TEMP, &data),
            return 6);
        handle->backup.refTemp = data;
    }
    /* 这里假设微码在上电准备过程中已经从ROM中被加载，所以更新睡眠设置 */
    if (handle->init.shouldOtpUcodeReadAfterWakeUp) {
        DW1000_SET_BITS(handle->backup.sleepMode, DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDE_MSK);
    }
    /* 设置系统时钟为sequencing */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ),
        return 7);
    /* 位于AON_CFG1寄存器的这3个位必须被清零以确保DW1000在深睡眠模式下正确运行 */
    DW1000_WRITE_REG(data, 0x00);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AON,
                          DW1000_SUB_REG_AON_CFG1_OFFSET,
                          (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 读取系统配置寄存器并将值进行备份 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &data, sizeof(data)),
        return 1);
    handle->backup.systemConfig = data;

    // 【此处官方还有保存了一个关于长帧的标志位，个人认为用不上，所以省略】

    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_TX_FCTRL, 0, (uint8_t*) &data, sizeof(data)),
        return 1);
    handle->backup.txFctrl = data;

    /* 准备工作完成，可以将SPI速率恢复到高速率 */
    return _DW1000_SPI_HighSpeedSet(handle);
}

uint8_t DW1000_Init(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
    DW1000_ASSERT_PARAM(IS_DW1000_DATA_RATE(handle->init.dataRate));
    DW1000_ASSERT_PARAM(IS_DW1000_PAC(handle->init.rxPac));
    DW1000_ASSERT_PARAM(IS_DW1000_CHANNEL(handle->init.channel));
    DW1000_ASSERT_PARAM(IS_DW1000_TX_PLEN(handle->init.txPreambleLength));
    DW1000_ASSERT_PARAM(DW1000_CHECK_PRF_PCODE(
        handle->init.prf, handle->init.txPreambleCode));
    DW1000_ASSERT_PARAM(DW1000_CHECK_PRF_PCODE(
        handle->init.prf, handle->init.rxPreambleCode));
    DW1000_ASSERT_PARAM(IS_DW1000_PHR_TYPE(handle->init.phrType));
#endif /* USE_DW1000_FULL_ASSERT */
    uint32_t data;

    /* 数据速率为110kpbps时需要特殊的配置 */
    if (handle->init.dataRate == DW1000_DATA_RATE_110K) {
        DW1000_SET_BITS(handle->backup.systemConfig, DW1000_REG_SYS_CFG_BIT_RXM110K_MSK);
    } else {
        DW1000_CLEAR_BITS(handle->backup.systemConfig, DW1000_REG_SYS_CFG_BIT_RXM110K_MSK);
    }

    /* 根据设置配置PHR模式（标准帧与扩展帧） */
    if (handle->init.phrType == DW1000_PHR_TYPE_STD) {
        DW1000_MODIFY_REG(handle->backup.systemConfig,
                          DW1000_REG_SYS_CFG_BIT_RXM110K_MSK,
                          DW1000_PHR_MODE_STANDARD);
    } else {
        DW1000_MODIFY_REG(handle->backup.systemConfig,
                          DW1000_REG_SYS_CFG_BIT_RXM110K_MSK,
                          DW1000_PHR_MODE_EXTENDED);
    }
    DW1000_WRITE_REG(data, handle->backup.systemConfig);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, &data, sizeof(data)),
        return 2);
    /* 设置LDE副本系数 */
    data = DW1000_LDE_REPLICA_COEFF[handle->init.rxPreambleCode];
    if (handle->init.dataRate == DW1000_DATA_RATE_110K) {
        /* 当数据速率为110kbps时该值必须除以8 */
        data >>= 3;
    }
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_LDE_REPC_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
        return 2);
    /* 配置LDE */
    if (handle->init.prf == DW1000_PRF_16M) {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_LDE_Config(handle, DW1000_LDE_TUNE_PARAM_IN_PRF16MHz),
            return 3);
    } else {
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_LDE_Config(handle, DW1000_LDE_TUNE_PARAM_IN_PRF64MHz),
            return 3);
    }
    /* 设置 PLL2 和 RF PLL 块的配置（对于给定的频道）*/
    DW1000_WRITE_REG(data, DW1000_FS_PLL_CFG[handle->init.channel]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_PLLCFG_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 设置 PLL2 和 RF PLL 块的调谐（对于给定的频道） */
    DW1000_WRITE_REG(data, DW1000_FS_PLL_TUNE[handle->init.channel]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_PLLTUNE_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 配置 RF RX块（对于给定的频道） */
    DW1000_WRITE_REG(data, DW1000_RX_CONFIG[handle->init.channel]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_RXCTRLH_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 配置RF TX块（对于给定的频道和PRF） */
    /* 配置RF TX控制 */
    DW1000_WRITE_REG(data, DW1000_TX_CONFIG[handle->init.channel]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_TXCTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
        return 2);
    /* 配置基带参数（对于给定的PRF,数据速率，PAC和SFD设置） */
    /* DTUNE0b */
    DW1000_WRITE_REG(data, DW1000_SFD_THRESHOLD[handle->init.dataRate][handle->init.useSfdNstd]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_TUNE0b_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
        return 2);
    /* DTUNE1a */
    DW1000_WRITE_REG(data, DW1000_DRX_TUNE1A[handle->init.prf]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_RX_CONFIG, DW1000_SUB_REG_DRX_TUNE1a_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
        return 2);
    if (handle->init.dataRate == DW1000_DATA_RATE_110K) {
        DW1000_WRITE_REG(data, DW1000_DRX_TUNE1B_110K_PL);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_TUNE1b_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
            return 2);
    } else if (handle->init.txPreambleLength == DW1000_PLEN_64) {
        DW1000_WRITE_REG(data, DW1000_DRX_TUNE1B_6M8_PS);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_TUNE1b_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
            return 2);
        DW1000_WRITE_REG(data, DW1000_DRX_TUNE4H_PLEN64);
        /* DTUNE4H */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_TUNE4H_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
            return 2);
    } else {
        DW1000_WRITE_REG(data, DW1000_DRX_TUNE1B_850K_6M8_PM);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_TUNE1b_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
            return 2);
        /* DTUNE4H */
        DW1000_WRITE_REG(data, DW1000_DRX_TUNE4H_PLEN128_PLUS);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_TUNE4H_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
            return 2);
    }
    /* DTUNE2 */
    DW1000_WRITE_REG(data, DW1000_DRX_TUNE2[handle->init.prf][handle->init.rxPac]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_TUNE2_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 设置SFD检测超时时间 */
    /* 这里默认禁止关闭SFD检测，以防止在低功耗应用中产生较大的功耗
       因为关闭SFD超时检测，会导致DW1000在接收到错误的SFD时仍保持
       接收状态，除非控制器给DW1000发送其他命令才会解除
    */
    if (handle->init.sfdTimeout == 0) {
        handle->init.sfdTimeout = DW1000_SFD_TIMEOUT_DEFAULT;
    }
    DW1000_WRITE_REG(data, handle->init.sfdTimeout);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_SFDTOC_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
        return 2);
    /* 配置AGC参数 */
    DW1000_WRITE_REG(data, DW1000_AGC_CFG.lo32);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AGC_CTRL, DW1000_SUB_REG_AGC_TUNE2_OFFSET, (uint8_t*) &data, sizeof(data)),
        return 2);
    DW1000_WRITE_REG(data, DW1000_AGC_CFG.target[handle->init.prf]);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_AGC_CTRL, DW1000_SUB_REG_AGC_TUNE1_OFFSET, (uint8_t*) &data, sizeof(uint16_t)),
        return 2);
    /* 如果设置了非标SFD（用于提高性能） */
    if (handle->init.useSfdNstd) {
        DW1000_WRITE_REG(data, DW1000_NSTD_SFD_LEN[handle->init.dataRate]);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_USR_SFD, 0x00, (uint8_t*) &data, sizeof(uint8_t)),
            return 2);
    }
    /* 频道控制配置 */
    DW1000_WRITE_REG(data, 0);
    /* 发送频道选择（频道号 + 1是因为定义的时候频道1的值从0开始） */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_CHAN_CTRL_BITS_TX_CHAN_MSK,
        (handle->init.channel + 1) << DW1000_REG_CHAN_CTRL_BITS_TX_CHAN_POS);
    /* 接收频道选择（频道号 + 1是因为定义的时候频道1的值从0开始） */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_MSK,
        (handle->init.channel + 1) << DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_POS);
    /* 接收使用的PRF（这里-1 也是因为定义的时候使用的 1和 2，映射为具体值1 和 2） */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_CHAN_CTRL_BITS_RXPRF_MSK,
        (handle->init.prf) << DW1000_REG_CHAN_CTRL_BITS_RXPRF_POS);
    /* 使能用户指定发送器上的SFD类型 */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_CHAN_CTRL_BIT_TNSSFD_MSK,
        (handle->init.useSfdNstd) << DW1000_REG_CHAN_CTRL_BIT_TNSSFD_POS);
    /* 使能用户指定接收上的SFD类型 */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_CHAN_CTRL_BIT_RNSSFD_MSK,
        (handle->init.useSfdNstd) << DW1000_REG_CHAN_CTRL_BIT_RNSSFD_POS);
    /* 使能非标Deca公司专有的SFD序列 */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_CHAN_CTRL_BIT_DWSFD_MSK,
        (handle->init.useSfdNstd) << DW1000_REG_CHAN_CTRL_BIT_DWSFD_POS);
    /* 设置发送前导码 */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_CHAN_CTRL_BITS_TX_PCODE_MSK,
        (handle->init.txPreambleCode) << DW1000_REG_CHAN_CTRL_BITS_TX_PCODE_POS);
    /* 设置发送前导码 */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_CHAN_CTRL_BITS_RX_PCODE_MSK,
        (handle->init.rxPreambleCode) << DW1000_REG_CHAN_CTRL_BITS_RX_PCODE_POS);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_CHAN_CTRL, 0, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 设置发送前导码大小，PRF和数据速率 */
    DW1000_WRITE_REG(data, 0);
    /* 设置发送前导码长度 */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_TX_FCTRL_BITS_TXPSR_MSK,
        (handle->init.txPreambleLength) << DW1000_REG_TX_FCTRL_BITS_TXPSR_POS);
    /* 设置发送PRF */
    DW1000_MODIFY_REG(
        data,
        DW1000_REG_TX_FCTRL_BITS_TXPRF_MSK,
        (handle->init.prf) << DW1000_REG_TX_FCTRL_BITS_TXPRF_POS);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_TX_FCTRL, 0, (uint8_t*) &data, sizeof(data)),
        return 2);
    /* 备份txFctrl写入值 */
    handle->backup.txFctrl = data;
    /* SFD传输模式由DW1000根据用户TX请求进行初始化，
      但对于自动确认（auto-ACK）传输，则不会进行初始化（由于IC问题）。
      下面的SYS_CTRL写入操作解决了这个问题，它通过同时启动和终止一次传输，
      在SFD配置或重新配置后正确初始化SFD。在编写这段代码时，该问题尚未被记录在文档中。
      它应被纳入下一版DW1000用户手册（2016年7月发布的v2.09版本）中。
      */
    DW1000_WRITE_REG(data, DW1000_SYS_CTRL_TX_START | DW1000_SYS_CTRL_TRX_OFF);
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, (uint8_t*) &data, sizeof(uint8_t));
}

/* 本来是打算给中断函数设置返回值的，但因为是在中断里执行，
   遇到错误也应该保证中断里的其他分支也能完成，不能提前返回，
   所以就设置为无返回值，在回调里遇到错误需要用户自行设置
   错误码
 */
/**
 * @brief DW1000发送完成回调函数
 * @param[in] handle  DW1000句柄
 * @note  这里的回调函数不应修改，当需要使用时
 *        请在用户自己的文件里重新定义一个同名函数
 */
__weak void DW1000_TxCompleteCallback(DW1000_Handle_t* handle) {
    DW1000_UNUSED(handle);
}

/**
 * @brief DW1000接收完成回调函数
 * @param[in] handle  DW1000句柄
 * @note  这里的回调函数不应修改，当需要使用时
 *        请在用户自己的文件里重新定义一个同名函数
 */
__weak void DW1000_RxCompleteCallback(DW1000_Handle_t* handle) {
    DW1000_UNUSED(handle);
}

/**
 * @brief DW1000接收错误回调函数
 * @param[in] handle  DW1000句柄
 * @note  这里的回调函数不应修改，当需要使用时
 *        请在用户自己的文件里重新定义一个同名函数
 */
__weak void DW1000_RxErrorCallback(DW1000_Handle_t* handle) {
    DW1000_UNUSED(handle);
}

/**
 * @brief DW1000接收超时回调函数
 * @param[in] handle  DW1000句柄
 * @note  这里的回调函数不应修改，当需要使用时
 *        请在用户自己的文件里重新定义一个同名函数
 */
__weak void DW1000_RxTimeoutCallback(DW1000_Handle_t* handle) {
    DW1000_UNUSED(handle);
}

void DW1000_IRQHandler(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 如果启用了低功耗监听就进入低功耗监听的ISR */
    if (handle->init.useLowPowListen) {
        DW1000_ISR_LPL(handle);
    } else { // 否则进入通用ISR
        DW1000_ISR(handle);
    }
}

#define DW1000_FCTRL_LEN_MAX 2

/**
 * @brief DW1000 中断服务程序
 * @param[in] handle  DW1000句柄
 * @return uint8_t 执行结果 0:执行成功 其他: 执行错误
 * @note  备注
 * @attention
 */
void DW1000_ISR(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif                                                     /* USE_DW1000_FULL_ASSERT */
    uint32_t systemStatus = DW1000_SYS_STATUS_VAL_DEFAULT; // 为了避免意外赋值，这里赋值为0
    uint32_t data = 0;                                     // 为了避免意外赋值，这里赋值为0
    /* 清空错误码 */
    DW1000_WRITE_REG(handle->errorCode, DW1000_ERR_CODE_NONE);
    /* 读取SYS_STATUS寄存器低16位 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &systemStatus, sizeof(systemStatus)),
        DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_READ));
    handle->backup.systemStatus = systemStatus;

    /* 处理 Rx good frame事件 */
    if (DW1000_READ_BITS(systemStatus, DW1000_REG_SYS_STATUS_BIT_RXFCG_MSK)) {
        uint16_t frameInfo;
        uint16_t frameLen;
        uint16_t frameCtrl;
        /* 清除所有接收标志位（通过往SYS_STATUS对应位写1即可清除） */
        DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_RX_GOOD_MSK);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data)),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_WRITE));
        /* 读取帧信息（只需要最前面的两个字节的数据） */
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_RX_FINFO, 0, (uint8_t*) &frameInfo, sizeof(frameInfo)),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_READ));
        /* 获取帧长度信息 */
        frameLen = DW1000_READ_BITS(frameInfo, DW1000_RX_FINFO_RXFL_1023_MSK);
        if (handle->init.phrType == DW1000_PHR_TYPE_STD) {
            frameLen = DW1000_READ_BITS(frameLen, DW1000_RX_FINFO_RXFL_127_MSK);
        }
        handle->cbData.frameLen = frameLen;
        /* 判断是否为测距帧 */
        if (DW1000_READ_BITS(frameInfo, DW1000_REG_RX_FINFO_BIT_RNG_MSK)) {
            handle->cbData.isRxRangingFrame = 1;
        } else {
            handle->cbData.isRxRangingFrame = 0;
        }
        /* 读取帧控制信息 */
        DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_RX_GOOD_MSK);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_RX_BUFFER, 0, (uint8_t*) &frameCtrl, sizeof(frameCtrl)),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_READ));
        handle->cbData.frameCtrl = frameCtrl;
        /* 由于前一帧未被正确接收，当正确接收到一个不请求确认的帧（实际上并未发送ACK帧）时，
           可以设置AAT位。如果AAT位已设置，请检查帧控制中的ACK请求位以进行确认
           （此实现仅适用于符合IEEE802.15.4-2011标准的帧）。
           在编写这段代码时，该问题尚未被记录在文档中。
           它应包含在下一版DW1000用户手册（2016年7月发布的v2.09版本）中。
         */
        if (DW1000_READ_BITS(systemStatus, DW1000_REG_SYS_STATUS_BIT_AAT) &&
            !DW1000_READ_BITS(frameCtrl, DW1000_FCTRL_ACK_REQ_MSK)) {
            /* 清除AAT标志位 */
            DW1000_WRITE_REG(data, DW1000_REG_SYS_STATUS_BIT_AAT);
            DW1000_EXEC_WITH_ERR_HANDLE(
                _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data)),
                DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_READ));
            DW1000_CLEAR_BITS(handle->backup.systemStatus, DW1000_REG_SYS_STATUS_BIT_AAT_MSK);
            handle->init.isRxOnAfterTx = 0;
        }
/* 调用相应的回调函数 */
#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
        handle->RxCompleteCallback(handle);
#else
        DW1000_RxCompleteCallback(handle);
#endif /* USE_DW1000_REGISTER_CALLBACKS */

        if (handle->init.useDoubleBuff) {
            /* 切换主机端接收缓冲区指针 */
            DW1000_WRITE_REG(data, DW1000_SUB_REG_SYS_CTRL_3_BIT_HRBPT);
            DW1000_EXEC_WITH_ERR_HANDLE(
                _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, DW1000_SUB_REG_SYS_CTRL_3_OFFSET, (uint8_t*) &data, sizeof(uint8_t)),
                DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_WRITE));
        }
    }
    /* 处理发送确认事件 */
    if (DW1000_READ_BITS(systemStatus, DW1000_REG_SYS_STATUS_BIT_TXFRS_MSK)) {
        /* 清除发送完成标志位 */
        DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_TX_MSK);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data)),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_WRITE));
        /* 如果此TXFRS中断是由于我们通过使用wait4resp对上一个TX（发送）接收到的响应（设置ACK请求位）
           所请求的ACK自动传输而产生的（并假设该TX的IRQ处理已处理完毕），
           那么我们需要处理IC问题，在这种情况下，IC会再次开启RX（即因为在ACK TX之后错误地应用了wait4resp）。
           参见《DW1000用户手册》中的“发送并自动等待响应”部分
        */
        if (DW1000_READ_BITS(systemStatus, DW1000_REG_SYS_STATUS_BIT_AAT) && handle->init.isRxOnAfterTx) {
            /* 关闭接收 */
            DW1000_EXEC_WITH_ERR_HANDLE(
                DW1000_ForceTrxOff(handle),
                DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_TRX_OFF));
            /* 以防我们执行过迟，已经收到一帧数据，先进行复位 */
            DW1000_EXEC_WITH_ERR_HANDLE(
                DW1000_RxReset(handle),
                DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_RX_RESET));
        }
/* 调用相应的回调函数 */
#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
        handle->TxCompleteCallback(handle);
#else
        DW1000_TxCompleteCallback(handle);
#endif /* USE_DW1000_REGISTER_CALLBACKS */
    }
    /* 处理帧接收/前导码检测超时事件 */
    if (DW1000_READ_BITS(systemStatus, DW1000_SYS_STATUS_ALL_RX_TO_MSK)) {
        /* 清除接收超时标志位 */
        DW1000_WRITE_REG(data, DW1000_REG_SYS_STATUS_BIT_RXRFTO);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data)),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_WRITE));
        handle->init.isRxOnAfterTx = 0;
        /* 由于在错误情况后接收器重启存在问题，因此在发生任何错误或超时事件后，
           必须执行RX重置，以确保正确计算下一个良好帧的时间戳。
           请参阅《DW1000用户手册》中的“RX消息时间戳”部分。
        */
        DW1000_EXEC_WITH_ERR_HANDLE(
            DW1000_ForceTrxOff(handle),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_TRX_OFF));
        DW1000_EXEC_WITH_ERR_HANDLE(
            DW1000_RxReset(handle),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_RX_RESET));
/* 调用相应的回调函数 */
#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
        handle->RxTimeoutCallback(handle);
#else
        DW1000_RxTimeoutCallback(handle);
#endif /* USE_DW1000_REGISTER_CALLBACKS */
    }
    /* 处理接收错误事件 */
    if (DW1000_READ_BITS(systemStatus, DW1000_SYS_STATUS_ALL_RX_ERR_MSK)) {
        /* 清除接收错误标志位 */
        DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_RX_ERR_MSK);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data)),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_WRITE));
        handle->init.isRxOnAfterTx = 0;
        /* 由于在错误情况后接收器重启存在问题，因此在发生任何错误或超时事件后，
           必须执行RX重置，以确保正确计算下一个良好帧的时间戳。
           请参阅《DW1000用户手册》中的“RX消息时间戳”部分。
        */
        DW1000_EXEC_WITH_ERR_HANDLE(
            DW1000_ForceRxOff(handle),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_TRX_OFF));
        DW1000_EXEC_WITH_ERR_HANDLE(
            DW1000_RxReset(handle),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_RX_RESET));
/* 调用相应的回调函数 */
#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
        handle->RxErrorCallback(handle);
#else
        DW1000_RxErrorCallback(handle);
#endif /* USE_DW1000_REGISTER_CALLBACKS */
    }
}

/**
 * @brief 低功耗监听下的DW1000 ISR
 * @param[in] handle  DW100句柄
 * @return uint8_t 执行结果 0:执行成功 其他: 执行错误
 * @note  当启用低功耗功能时，执行该中断服务函数
 * @attention 这个版本的中断服务函数是针对单缓冲区接收设计的
 *            只会处理接收完成的中断（事件），其他中断情况不处理，只清除标志位
 */
void DW1000_ISR_LPL(DW1000_Handle_t* handle) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(handle != NULL);
#endif /* USE_DW1000_FULL_ASSERT */
    // uint8_t result;
    uint32_t data;
    uint32_t systemStatus;
    /* 清空错误码 */
    DW1000_WRITE_REG(handle->errorCode, DW1000_ERR_CODE_NONE);
    /* 读取SYS_STATUS寄存器低16位 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &systemStatus, sizeof(systemStatus)),
        DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_READ));
    handle->backup.systemStatus = systemStatus;
    /* 处理RX Good Frame事件 */
    /* 在低功耗模式中只有 RX Good Frame会触发中断，所以直接处理，不判断事件类型 */
    /* 在清除中断标志前失能低功耗模式避免DW1000在清除中断后立即进入睡眠模式 */
    DW1000_LowPowerListeningSet(handle, DW1000_LOW_POWER_LISTENING_MODE_DISABLE);
    /* 清除所有接收标志位（通过往SYS_STATUS对应位写1即可清除） */
    DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_RX_GOOD_MSK);
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data)),
        DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_WRITE));
    uint16_t frameInfo;
    uint16_t frameLen;
    uint16_t frameCtrl;
    /* 读取帧信息（只需要最前面的两个字节的数据） */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_RX_FINFO, 0, (uint8_t*) &frameInfo, sizeof(frameInfo)),
        DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_READ));
    /* 获取帧长度信息 */
    frameLen = DW1000_READ_BITS(frameInfo, DW1000_RX_FINFO_RXFL_1023_MSK);
    if (handle->init.phrType == DW1000_PHR_TYPE_STD) { // 如果是标准帧，则长度在0-127
        frameLen = DW1000_READ_BITS(frameLen, DW1000_RX_FINFO_RXFL_127_MSK);
    }
    handle->cbData.frameLen = frameLen;
    /* 判断是否为测距帧 */
    if (DW1000_READ_BITS(frameInfo, DW1000_REG_RX_FINFO_BIT_RNG_MSK)) {
        handle->cbData.isRxRangingFrame = 1;
    } else {
        handle->cbData.isRxRangingFrame = 0;
    }
    /* 读取帧控制信息 */
    DW1000_EXEC_WITH_ERR_HANDLE(
        _DW1000_SPI_Read(handle, DW1000_REG_RX_BUFFER, 0, (uint8_t*) &frameCtrl, sizeof(frameCtrl)),
        DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_READ));
    handle->cbData.frameCtrl = frameCtrl;
    /* 由于前一帧未被正确接收，当正确接收到一个不请求确认的帧（实际上并未发送ACK帧）时，
       可以设置AAT位。如果AAT位已设置，请检查帧控制中的ACK请求位以进行确认
       （此实现仅适用于符合IEEE802.15.4-2011标准的帧）。
       在编写这段代码时，该问题尚未被记录在文档中。
       它应包含在下一版DW1000用户手册（2016年7月发布的v2.09版本）中。
    */
    if (DW1000_READ_BITS(systemStatus, DW1000_REG_SYS_STATUS_BIT_AAT) &&
        !DW1000_READ_BITS(frameCtrl, DW1000_FCTRL_ACK_REQ_MSK)) {
        /* 清除AAT标志位 */
        DW1000_WRITE_REG(data, DW1000_REG_SYS_STATUS_BIT_AAT);
        DW1000_EXEC_WITH_ERR_HANDLE(
            _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data)),
            DW1000_SET_BITS(handle->errorCode, DW1000_ERR_CODE_READ));
        DW1000_CLEAR_BITS(handle->backup.systemStatus, DW1000_REG_SYS_STATUS_BIT_AAT_POS);
        handle->init.isRxOnAfterTx = 0;
    }
/* 调用相应的回调函数 */
#if (USE_DW1000_REGISTER_CALLBACKS == 1U)
    handle->RxCompleteCallback(handle);
#else
    DW1000_RxCompleteCallback(handle);
#endif /* USE_DW1000_REGISTER_CALLBACKS */
    return 0;
}
