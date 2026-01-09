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
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/**
 * @brief chip register definition
 */
#define CHIP_NAME          "Qorvo DW1000" /**< chip name */
#define MANUFACTURER_NAME  "Qorvo"        /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN 2.0f           /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX 3.6f           /**< chip max supply voltage */
#define MAX_CURRENT        0.145f         /**< chip max current */
#define TEMPERATURE_MIN    -40.0f         /**< chip min operating temperature */
#define TEMPERATURE_MAX    85.0f          /**< chip max operating temperature */
#define DRIVER_VERSION     1000

/* 给寄存器地址添加读/写命令 */
// #define DW1000_SPI_CMD_READ(addr)  DW1000_CLEAR_BITS(addr, DW1000_SPI_CMD_MASK)
#define DW1000_SPI_CMD_READ(addr)  (addr & 0x7F)
// #define DW1000_SPI_CMD_WRITE(addr) DW1000_SET_BITS(addr, DW1000_SPI_CMD_MASK)
#define DW1000_SPI_CMD_WRITE(addr) (addr | 0x80)

/* 给寄存器地址添加是否存在子地址标志 */
// #define DW1000_SPI_NOT_SUB_ADDR(addr) DW1000_CLEAR_BITS(addr, DW1000_SPI_SUB_ADDR_MASK)
#define DW1000_SPI_NOT_SUB_ADDR(addr) (addr & 0xBF)
// #define DW1000_SPI_HAS_SUB_ADDR(addr) DW1000_SET_BITS(addr, DW1000_SPI_SUB_ADDR_MASK)
#define DW1000_SPI_HAS_SUB_ADDR(addr) (addr | 0x40)


/* 给子地址添加是否为扩展子地址标志 */
// #define DW1000_SPI_NOT_EXT_SUB_ADDR(sa) DW1000_CLEAR_BITS(sa, DW1000_SPI_EXT_SUB_ADDR_MASK)
#define DW1000_SPI_NOT_EXT_SUB_ADDR(sa) (sa & 0x7F)
// #define DW1000_SPI_HAS_EXT_SUB_ADDR(sa) DW1000_SET_BITS(sa, DW1000_SPI_EXT_SUB_ADDR_MASK)
#define DW1000_SPI_HAS_EXT_SUB_ADDR(sa) (sa | 0x80)

/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
enum DW1000_CHANNEL_e {
    DW1000_CHANNEL_1 = 1,
    DW1000_CHANNEL_2 = 2,
    DW1000_CHANNEL_3 = 3,
    DW1000_CHANNEL_4 = 4,
    DW1000_CHANNEL_5 = 5, /* 通道5 */
    /* 通道6不使用 */
    DW1000_CHANNEL_7 = 7, /* 通道7 */
    DW1000_CHANNEL_NUM
};

#define DW1000_FS_PLL_CFG_CH1 0x09000407UL
#define DW1000_FS_PLL_CFG_CH2 0x08400508UL
#define DW1000_FS_PLL_CFG_CH3 0x08401009UL
#define DW1000_FS_PLL_CFG_CH4 0x08400508UL
#define DW1000_FS_PLL_CFG_CH5 0x0800041DUL

/* 通道6不使用 */

#define DW1000_FS_PLL_CFG_CH7 0x0800041DUL

const static uint32_t DW1000_FS_PLL_CFG[DW1000_CHANNEL_NUM] = {
    DW1000_FS_PLL_CFG_CH1,
    DW1000_FS_PLL_CFG_CH2,
    DW1000_FS_PLL_CFG_CH3,
    DW1000_FS_PLL_CFG_CH4,
    DW1000_FS_PLL_CFG_CH5,
    /* 通道6不使用 */
    DW1000_FS_PLL_CFG_CH7
};
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief spi 接口读取数据
 * @param[in] handle  DW1000 Handle 结构体指针
 * @param[in] reg  寄存器地址
 * @param[in] offset  基于寄存器的子地址偏移（当寄存器长度大于1字节时）
 * @param[in] buf  数据缓存指针
 * @param[in] len  读取数据长度
 * @return uint8_t 函数执行情况
 *  @arg 0: 读取成功
 *  @arg 1: 读取失败
 * @note  因为仅供文件内部调用该函数，所以不对输入参数进行检查（用户不应调用该函数）
 */
static uint8_t _DW1000_SPI_Read(DW1000_Handle_t* handle, uint8_t reg, uint16_t offset, uint8_t* buf, uint16_t len) {
    #ifdef USE_DW1000_FULL_ASSERT
        assert(reg <= 0x3F);
        assert(handle != NULL);
        assert(handle->SPI_read != NULL);
        assert(buf != NULL);
        assert(len != 0);
    #endif /* USE_DW1000_FULL_ASSERT */
    /* 对输入参数进行检查（这里定义为静态不给外界调用，所以默认调用是正确的，也就省略输入参数检查） */
    // if (handle == NULL || handle->SPI_read == NULL || buf == NULL || len == 0) {
    //     return 1;
    // }
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
    } 
    else {
        /* 设置本次spi为读操作且有子地址，且有扩展子地址 */
        header[0] = DW1000_SPI_HAS_SUB_ADDR(reg);
        header[1] = DW1000_SPI_HAS_EXT_SUB_ADDR(offset);
        header[2] = (offset >> 7);
        headerLength = 3;
    }
    return handle->spi.read(header, headerLength, buf, len);
}

static uint8_t _DW1000_SPI_Write(DW1000_Handle_t* handle, uint8_t reg, uint16_t offset, uint8_t* buf, uint16_t len) {
#if USE_DW1000_FULL_ASSERT
    assert(reg <= 0x3F);
    assert(handle != NULL);
    assert(handle->SPI_write != NULL);
    assert(buf != NULL);
    assert(len != 0);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 对输入参数进行检查（这里定义为静态不给外界调用，所以默认调用是正确的，也就省略输入参数检查） */
    // if (handle == NULL || handle->spi_write == NULL || buf == NULL || len == 0) {
    //     return 1;
    // }
    /* 定义头部数组 */
    uint8_t header[3];
    /* 定义头部数组长度 */
    uint8_t headerLength;
    /* 如果偏移为0，即只有寄存器地址，头部字段长度为1字节 */
    if(offset == 0){
        /* 设置本次spi为写操作且无子地址 */
        header[0] = DW1000_SPI_CMD_WRITE(reg);
        headerLength = 1;
    }
    /* 如果偏移不超过127，也就是用7bit可以表示时，说明不需要扩展子地址 */
    else if(offset <= 0x7F){
        /* 设置本次spi为写操作且有子地址，但无扩展子地址 */
        header[0] = DW1000_SPI_HAS_SUB_ADDR(DW1000_SPI_CMD_WRITE(reg));
        header[1] = offset;
        headerLength = 2;
    }
    else{
        /* 设置本次spi为写操作且有子地址，且有扩展子地址 */
        header[0] = DW1000_SPI_HAS_SUB_ADDR(DW1000_SPI_CMD_WRITE(reg));
        header[1] = DW1000_SPI_HAS_EXT_SUB_ADDR(offset);
        header[2] = (offset >> 7);
        headerLength = 3;
    }
    return handle->spi.write(header, headerLength, buf, len);
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
 *  @arg DW1000_CLOCK_CMD_FORCE_LED: 强制LED时钟
 * @return uint8_t 函数执行情况
 *  @arg 0: 配置成功
 *  @arg 其他: 配置失败
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t _DW1000_ClockCmd(DW1000_Handle_t* handle, uint8_t clockCmd) {
#ifdef USE_DW1000_FULL_ASSERT
    assert(clockCmd <= 0x09);
    assert(handle != NULL);
    assert(handle->SPI_write != NULL);
    assert(buf != NULL);
    assert(len != 0);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 对输入参数进行检查（这里定义为静态不给外界调用，所以默认调用是正确的，也就省略输入参数检查） */
    // if (handle == NULL || handle->spi_write == NULL) {
    //     return 1;
    // }
    uint8_t result;
    uint16_t value;
    /* 关于时钟设置只需要低16位数据，这里就读取这个寄存器的2个字节 */
    if (_DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &value, sizeof(value))) {
        return 1;
    }
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
    if (result = _DW1000_SPI_Write(handle,
                          DW1000_REG_PMSC,
                          DW1000_SUB_REG_PMSC_CTRL0_OFFSET,
                          (uint8_t*) &value,
                          1)) {
        return result;
    }
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_PMSC,
                             DW1000_SUB_REG_PMSC_CTRL0_OFFSET + 1,
                             ((uint8_t*) &value) + 1,
                             1);
}

uint8_t _DW1000_OTP_Read(DW1000_Handle_t* handle, uint16_t address, uint32_t* data) {
    // if(handle == NULL || data == NULL){
    //     return 2;
    // }
    // else if(handle->inited != 1){
    //     return 3;
    // }
    uint8_t result;
    /* 写入地址 */
    if ( result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_ADDR_OFFSET, (uint8_t*) &address, sizeof(address))){
        return result;
    }
    uint8_t value;
    /* 设置 OTPREAD 和 OTPRDEN 标志位 */
    DW1000_WRITE_REG(value, DW1000_SUB_REG_OTP_CTRL_BIT_OTPREAD | DW1000_SUB_REG_OTP_CTRL_BIT_OTPRDEN);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &value, sizeof(value))){
        return result;
    }
    /* 清除 OTPREAD 和 OTPRDEN 标志位，OTPREAD实际上会被自动清零，但是OTPRDEN需要手动清零，这里就一块清除 */
    DW1000_WRITE_REG(value, 0x00);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &value, sizeof(value))){
        return result;
    }
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
 * @brief 
 * @param[in] handle  Comment
 * @param[in] mode  Comment
 * @return uint8_t 
 * @note  具体说明可以参考 用户手册 的 6.3.2节
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t _DW1000_OTP_MR_RegSet(DW1000_Handle_t* handle, uint8_t mode){
    // if(handle == NULL || data == NULL){
    //     return 2;
    // }
    // else if(handle->inited != 1){
    //     return 3;
    // }
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
            return 4;
            break;
    }
    /* 编程 MRA, */
    /* MODE_SEL 设置 MRA */
    // OTP_ctrl = 0x0300;
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_MODE_REGISTER_MRA | DW1000_OTP_MODE_SELECT_SET);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1)) {
        return result;
    }
    /* 将mra值写入 */
    // OTP_data = mra;
    DW1000_WRITE_REG(OTP_data, mra);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_WDAT_OFFSET, (uint8_t*) &OTP_data, sizeof(OTP_data))) {
        return result;
    }
    /* 设置 OTPMRWR 位，以将前面的配置写入OTP Mode寄存器中 */
    // OTP_ctrl |= 0x08; //OTP_ctrl = 0x0308
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }
    /* 等待2ms (手册上写的是1ms，但官方的库代码是2ms) */
    handle->delay_ms(2);
    /* 清除 MODE_SEL 位 */
    // OTP_ctrl &= ~0x0100; // OTP_ctrl = 0x0208
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1)) {
        return result;
    }
    /* 设置 AUX Update 位 和 OTPMRWR 位 */
    // OPT_ctrl |= 0x88; // OTP_ctrl = 0x0288;
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK | DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }
    /* 清除 OTPMRWR 位 */
    // OTP_ctrl &= ~0x08; // OTP_ctrl = 0x0280;
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }
    /* 清除 AUX Update 位 */
    // OTP_ctrl &= ~0x80; // OTP_ctrl = 0x0200;
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }

    /* 编程 MRB */
    /* MODE_SEL 设置 SLOW MRB  */
    // OTP_ctrl = 0x0500;
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_MODE_REGISTER_MRB | DW1000_OTP_MODE_SELECT_SET);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1)) {
        return result;
    }
    // OTP_data = mrb;
    DW1000_WRITE_REG(OTP_data, mrb);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_WDAT_OFFSET, (uint8_t*) &OTP_data, sizeof(OTP_data))) {
        return result;
    }
    /* 设置 OTPMRWR 位 */
    // OTP_ctrl |= 0x08; // OTP_ctrl = 0x0508
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }
    /* 等待2ms (手册上写的是1ms，但官方的库代码是2ms) */
    handle->delay_ms(2);
    /* 清除 MODE_SEL 位 */
    // OTP_ctrl &= ~0x0100; // OTP_ctrl = 0x0408
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1)) {
        return result;
    }
    /* 设置AUX Update 位 和 OTPMRWR 位 */
    // OPT_ctrl |= 0x88; // OTP_ctrl = 0x0488;
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK | DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }
    /* 清除 OTPMRWR 位 */
    // OTP_ctrl &= ~0x08; // OTP_ctrl = 0x0480;
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }
    /* 清除 AUX Update 位 */
    // OTP_ctrl &= ~0x80; // OTP_ctrl = 0x0400;
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }

    /* 编程 MR */
    /* MODE_SEL 设置 SLOW MR */
    // OTP_ctrl = 0x0100;
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_MODE_REGISTER_MR | DW1000_OTP_MODE_SELECT_SET);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1)) {
        return result;
    }
    // OTP_data = mr;
    DW1000_WRITE_REG(OTP_data, mr);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_WDAT_OFFSET, (uint8_t*) &OTP_data, sizeof(OTP_data))) {
        return result;
    }
    /* 设置 OTPMRWR 位 */
    // OTP_ctrl |= 0x08; // OTP_ctrl = 0x0108
    DW1000_SET_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }
    /* 等待2ms (手册上写的是1ms，但官方的库代码是2ms) */
    handle->delay_ms(2);
    /* 清除 MODE_SEL 位 */
    // OTP_ctrl &= ~0x0100; // OTP_ctrl = 0x0008
    DW1000_CLEAR_BITS(OTP_ctrl, DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET + 1, ((uint8_t*) &OTP_ctrl) + 1, 1)) {
        return result;
    }
    return 0;
}

uint8_t _DW1000_OTP_Word32Program(DW1000_Handle_t* handle, uint32_t data, uint16_t address) {
    // if(handle == NULL || data == NULL){
    //     return 2;
    // }
    // else if(handle->inited != 1){
    //     return 3;
    // }
    uint8_t result;
    if (result = _DW1000_SPI_Write(handle,DW1000_REG_OTP_IF,DW1000_SUB_REG_OTP_WDAT_OFFSET,(uint8_t*) &data, sizeof(data))) {
        return result;
    }
    if (result = _DW1000_SPI_Write(handle,DW1000_REG_OTP_IF,DW1000_SUB_REG_OTP_CTRL_OFFSET,(uint8_t*) &address, sizeof(address))) {
        return result;
    }
    uint16_t OTP_ctrl;
    /* 设置 OTPPROG 位 */
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_PROGRAM_SET);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }
    /* 清除 OTPPROG 位 */
    DW1000_WRITE_REG(OTP_ctrl, DW1000_OTP_PROGRAM_CLEAR);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &OTP_ctrl, 1)) {
        return result;
    }
    uint8_t OTP_stat;
    do {
        handle->delay_ms(1);
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_STAT_OFFSET, &OTP_stat, sizeof(OTP_stat))) {
            return result;
        }
    } while (DW1000_IS_OTP_PROGRAMMING_DONE(OTP_stat) == 0);
    return 0;
}

uint8_t _DW1000_AON_ConfigurationUpload(DW1000_Handle_t* handle){
    // if(handle == NULL || data == NULL){
    //     return 2;
    // }
    // else if(handle->inited != 1){
    //     return 3;
    // }
    uint8_t result;
    uint8_t data;
    DW1000_WRITE_REG(data, DW1000_AON_CTRL_UPLOAD_CONFIGURATION_SET);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 清空寄存器的值 */
    DW1000_WRITE_REG(data, 0x00);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    return 0;
}

uint8_t _DW1000_AON_ArrayUpload(DW1000_Handle_t* handle){
    // if(handle == NULL || data == NULL){
    //     return 2;
    // }
    // else if(handle->inited != 1){
    //     return 3;
    // }
    uint8_t result;
    uint8_t data;
    /* 清空寄存器的值 */
    DW1000_WRITE_REG(data, 0x00);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    DW1000_WRITE_REG(data, DW1000_AON_CTRL_SAVE_SET);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
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
    // if(handle == NULL || data == NULL){
    //     return 2;
    // }
    // else if(handle->inited != 1){
    //     return 3;
    // }
    uint8_t result;
    uint16_t data = 0x00;
    /* 配置DW1000 LDE配置 */
    DW1000_WRITE_REG(data, DW1000_LDE_PARAM1);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_LDE_CFG1_OFFSET, (uint8_t*) &data, 1)) {
        return result;
    }
    /* 按照config设置LDE调谐配置 */
    DW1000_WRITE_REG(data, config);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_LDE_CFG2_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    return 0;
}

uint8_t _DW1000_LoadUcodeFromROM(DW1000_Handle_t* handle){
    // if(handle == NULL || data == NULL){
    //     return 2;
    // }
    // else if(handle->inited != 1){
    //     return 3;
    // }
    uint8_t result;
    uint16_t data;
    /* 强制使能LDE时钟 */
    if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_LDE)) {
        return result;
    }
    /* 设置LDE微码加载 */
    DW1000_WRITE_REG(data, DW1000_LDE_LOAD_UCODE_ENABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_OTP_CTRL_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 等待微码加载到内存（大概需要120us） */
    handle->delay_ms(1);
    /* 恢复默认时钟（ENABLE_ALL_SEQ） */
    if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ)) {
        return result;
    }
    return 0;
}


uint8_t _DW1000_SequeningDisable(DW1000_Handle_t* handle){
    /* 设置系统时钟为XTI */
    uint8_t result;
    uint16_t data;
    if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI)) {
        return result;
    }
    /* 禁用PMSC控制里的RF 和 RX时钟块 */
    DW1000_WRITE_REG(data, DW1000_PMSC_CTRL_RF_SUB_SYS_DISABLE);
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_PMSC,
                             DW1000_SUB_REG_PMSC_CTRL1_OFFSET,
                             (uint8_t*) &data,
                             sizeof(data));
}

/**
 * @brief DW1000 精细粒度的Tx功率序列控制
 * @param[in] handle  dwt handle 结构体指针
 * @param[in] cmd  配置命令
 *  @arg DW1000_TX_FINE_GRAIN_SEQ_ENABLE: 启用精细粒度的Tx功率序列控制
 *  @arg DW1000_TX_FINE_GRAIN_SEQ_DISABLE: 禁用精细粒度的Tx功率序列控制
 * @return uint8_t 函数执行结果
 *  @arg 0  配置成功
 *  @arg 1  配置失败
 *  @arg 2  handle 为空
 *  @arg 3  handle 未初始化
 * @note  dw1000默认使能精细粒度的Tx序列
 */
uint8_t DW1000_FineGrainTxSeqCmd(DW1000_Handle_t* handle, uint16_t cmd) {
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_PMSC,
                             DW1000_SUB_REG_PMSC_TXFSEQ_OFFSET,
                             &cmd,
                             sizeof(cmd));
}

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
uint8_t DW1000_LNA_PA_ModeSet(DW1000_Handle_t* handle, uint32_t mode){
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    /* 先读取GPIO控制状态寄存器的子寄存器GPIO Mode的内容 */
    uint32_t GPIO_mode;
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_GPIO_CTRL, DW1000_SUB_REG_GPIO_MODE_OFFSET, (uint8_t*) &GPIO_mode, sizeof(GPIO_mode))) {
        return result;
    }
    /* 根据 mode 参数进行配置 */
    DW1000_MODIFY_REG(GPIO_mode, DW1000_LNA_PA_MODE_MASK, mode);
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_GPIO_CTRL,
                             DW1000_SUB_REG_GPIO_MODE_OFFSET,
                             (uint8_t*) &GPIO_mode,
                             sizeof(GPIO_mode));
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
uint8_t DW1000_GPIO_ClockEnable(DW1000_Handle_t* handle){
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    uint32_t PMSC_clockCtrl;
    /* 先读取PMSC寄存器的子寄存器CTRL0的内容 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &PMSC_clockCtrl, sizeof(PMSC_clockCtrl))) {
        return result;
    }
    /* GPRN置位使GPIO端口退出复位状态，然后GPCE置位开启GPIO时钟 */
    DW1000_SET_BITS(PMSC_clockCtrl, DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE | DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN);
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_PMSC,
                             DW1000_SUB_REG_PMSC_CTRL0_OFFSET,
                             (uint8_t*) &PMSC_clockCtrl,
                             sizeof(PMSC_clockCtrl));
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
 * @note  
 */
uint8_t DW1000_GPIO_DirectionSet(DW1000_Handle_t* handle, uint32_t direction) {
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_GPIO_CTRL,
                             DW1000_SUB_REG_GPIO_DIR_OFFSET,
                             (uint8_t*) &direction,
                             sizeof(direction));
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
 * @note  备注
 * @attention  只有对应的GPIO引脚配置为输出时，设置才会生效
 */
uint8_t DW1000_GPIO_ValueSet(DW1000_Handle_t* handle, uint32_t out) {
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_GPIO_CTRL,
                             DW1000_SUB_REG_GPIO_DOUT_OFFSET,
                             (uint8_t*) &out,
                             sizeof(out));
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
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t GPIO_value;
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_GPIO_CTRL, DW1000_SUB_REG_GPIO_RAW_OFFSET, (uint8_t*) &GPIO_value, sizeof(GPIO_value))) {
        return result;
    }
    *value = (GPIO_value & gpio) ? 1 : 0;
    return 0;
}

uint8_t dw1000_get_ic_ref_volt(DW1000_Handle_t* handle, uint8_t* volt) {
    if(handle == NULL || volt == NULL){
        return 2;
    }
    #if (DRIVER_DW1000_PARAM_CHECK == 1)
    assert(pdw1000local->otp_mask & DWT_READ_OTP_BAT);
    #endif
    pdw1000local->vBatP;
    return 0;
}

uint8_t dw1000_get_ic_ref_temp(DW1000_Handle_t* handle, uint8_t* temp){
    if(handle == NULL || temp == NULL){
        return 2;
    }
    #ifdef DWT_API_ERROR_CHECK
    assert(pdw1000local->otp_mask & DWT_READ_OTP_TMP);
    #endif
    pdw1000local->tempP;
    return 0; 
}

uint8_t dw1000_get_part_id(DW1000_Handle_t* handle, uint32_t* id){
    if(handle == NULL || id == NULL){
        return 2;
    }    
    #ifdef DWT_API_ERROR_CHECK
    assert(pdw1000local->otp_mask & DWT_READ_OTP_PID);
    #endif

    pdw1000local->partID;
    return 0;
}

uint8_t dw1000_get_lot_id(DW1000_Handle_t* handle, uint32_t* id){
    if(handle == NULL || id == NULL){
        return 2;
    }
    #ifdef DWT_API_ERROR_CHECK
    assert(pdw1000local->otp_mask & DWT_READ_OTP_LID);
    #endif

    pdw1000local->lotID;
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
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_DEV_ID_Get(DW1000_Handle_t* handle, uint32_t* id){
    if(handle == NULL || id == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    return _DW1000_SPI_Read(handle,
                            DW1000_REG_DEV_ID,
                            0,
                            (uint8_t*) id,
                            sizeof(*id));
}

uint8_t DW1000_TX_RF_Config(DW1000_Handle_t* handle, DW1000_TX_Config_t* config) {
    if(handle == NULL || config == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    if (result = _DW1000_SPI_Write(handle,
                          DW1000_REG_TX_CAL,
                          DW1000_SUB_REG_TC_PGDELAY_OFFSET,
                          (uint8_t*) &config->pulseGeneratorDelay,
                          sizeof(config->pulseGeneratorDelay))) {
        return result;
    }
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_TX_POWER,
                             0,
                             (uint8_t*) &config->TX_power,
                             sizeof(config->TX_power));
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
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint8_t reg_value = DEMOD_GEAR_64L;
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_CRTR, DW1000_SUB_REG_CRTR_GEAR_OFFSET, &reg_value, 1)) {
        return result;
    }
    /** @todo 需要修改*/
    return _DW1000_SPI_Write(handle,
                              DW1000_REG_DRX_CONF,
                              DW1000_SUB_REG_DRX_TUNE2_OFFSET + 2, /* 只修改子寄存器的第3个字节数据 */
                              (uint8_t*) &value,
                              sizeof(value));
}


uint8_t dw1000_configure(DW1000_Handle_t*handle, DW1000_Config_t* config){

    return 0;
}


uint8_t DW1000_RX_AntennaDelaySet(DW1000_Handle_t* handle, uint16_t delay){
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_LDE_IF,
                             DW1000_SUB_REG_LDE_RXANTD_OFFSET,
                             (uint8_t*) &delay,
                             sizeof(delay));
}


uint8_t DW1000_TX_AntennaDelaySet(DW1000_Handle_t* handle, uint16_t delay){
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_TX_ANTD,
                             0,
                             (uint8_t*) &delay,
                             sizeof(delay));
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
uint8_t DW1000_TX_DataWrite(DW1000_Handle_t* handle, uint8_t* data, uint16_t len, uint16_t offset) {
    if(handle == NULL || data == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    else if(len + offset > 1024){
        return 4;
    }
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_TX_BUFFER,
                             offset,
                             data,
                             len - 2); // 减去2字节的CRC
}

uint8_t DW1000_TX_FrameCtrlWrite(DW1000_Handle_t* handle, uint16_t len, uint16_t offset, uint8_t isRanging) {
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    } else if (len + offset > 1024) {
        return 4;
    }
    uint32_t txFCtrl = pdw1000local->txFCTRL;
    /* 设置发送帧控制寄存器的值 */
    /* 设置帧长度 */
    DW1000_MODIFY_REG(txFCtrl, DW1000_REG_TX_FCTRL_BITS_TFLEN_MSK, len);  // 因为len字段在最低位置，所以不需要移位，所以这里直接赋值
    /* 设置发送缓冲区索引偏移 */
    DW1000_MODIFY_REG(txFCtrl, DW1000_REG_TX_FCTRL_BITS_TXBOFFS_MSK, (uint32_t)offset << DW1000_REG_TX_FCTRL_BITS_TXBOFFS_POS);
    /* 设置Ranging位 */
    if (isRanging) {
        DW1000_SET_BITS(txFCtrl, DW1000_REG_TX_FCTRL_BIT_TR_MSK);
    } else {
        DW1000_CLEAR_BITS(txFCtrl, DW1000_REG_TX_FCTRL_BIT_TR_MSK);
    }
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_TX_FCTRL,
                             0,
                             (uint8_t*)&txFCtrl,
                             sizeof(txFCtrl));
}


uint8_t DW1000_RX_DataRead(DW1000_Handle_t* handle, uint8_t* data, uint16_t len, uint16_t offset) {
    if(handle == NULL || data == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    else if(len + offset > 1024){
        return 4;
    }
    return _DW1000_SPI_Read(handle,
                            DW1000_REG_RX_BUFFER,
                            offset,
                            data,
                            len);
}

uint8_t DW1000_ACC_DataRead(DW1000_Handle_t* handle, uint8_t* data, uint16_t len, uint16_t offset) {
    if(handle == NULL || data == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    /* 【这个待修改】 */
    else if(len + offset > 1024){
        return 4;
    }
    uint8_t result;
    /* 强制开启ACC时钟如果我们正在sequenced */
    if(result =_DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_READ_ACC_ON)){
        return result;
    }
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_ACC_MEM, offset, data, len)) {
        return result;
    }
    /* 恢复之前ACC的时钟 */
    return _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_READ_ACC_OFF);
}

#define B20_SIGN_EXTEND_TEST (0x00100000UL)
#define B20_SIGN_EXTEND_MASK (0xFFF00000UL)

uint8_t DW1000_CarrierIntegratorRead(DW1000_Handle_t* handle, int32_t* carrierIntegrator) {
    if(handle == NULL || carrierIntegrator == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    uint8_t buff[4] = {0}; /* 初始化为0，避免干扰后续判断 */
    uint32_t* value = (uint32_t*)buff;
    /* 将 DRX_CAR_INT 寄存器数据读入buff中（共3个字节） */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_CAR_INT_OFFSET, buff, DW1000_SUB_REG_DRX_CAR_INT_LEN)) {
        return result;
    }
    /* 根据第20位（符号位）进行扩展，将21位数据扩展为32位数据 */
    /* 如果是负数，将21位以上的高位都设置为1 */
    if( *value & B20_SIGN_EXTEND_TEST ){
        DW1000_SET_BITS(*value, B20_SIGN_EXTEND_MASK);
    }
    /* 如果是正数，将21位以上的高位都设置为0 */
    else{
        DW1000_CLEAR_BITS(*value, B20_SIGN_EXTEND_MASK);
    }

    /* 将处理后的结果赋值给carrierIntegrator */
    *carrierIntegrator = (int32_t)(*value);
    return 0;
}

/**
 * @brief 
 * @param[in] handle  Comment
 * @param[in] diag  Comment
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 可以直接开一个 DW1000_RX_DIAG_t 就不需要进行指针的转换了
 */
uint8_t DW1000_DiagnosticsRead(DW1000_Handle_t* handle, DW1000_RX_DIAG_t* diag) {
    if (handle == NULL || diag == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint8_t buff[8] = {0};
    void* ptr = buff;
    /* 读取 FP 索引 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_FP_INDEX_OFFSET, buff, DW1000_SUB_REG_FP_INDEX_LEN)) {
        return result;
    }
    diag->firstPathIndex = (uint16_t) (*((uint16_t*) ptr));
    /* LED 诊断信息 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_LDE_IF, DW1000_SUB_REG_LDE_THRESH_OFFSET, buff, DW1000_SUB_REG_LDE_THRESH_LEN)) {
        return result;
    }
    diag->maxNoise = (uint16_t) (*((uint16_t*) ptr));
    /* 一次性读取8个字节数据 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, 0, buff, sizeof(buff))) {
        return result;
    }
    diag->standardDeviationNoise = (uint16_t) (*((uint16_t*) ptr));
    diag->firstPathAmplitude2 = (uint16_t) (*((uint16_t*) ptr + 1));
    diag->firstPathAmplitude3 = (uint16_t) (*((uint16_t*) ptr + 2));
    diag->CIR_maxGrowth = (uint16_t) (*((uint16_t*) ptr + 3));

    if (result = _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_FP_AMPL1_OFFSET, buff, DW1000_SUB_REG_FP_AMPL1_LEN)) {
        return result;
    }
    diag->firstPathAmplitude1 = (uint16_t) (*((uint16_t*) ptr));

    if (result = _DW1000_SPI_Read(handle, DW1000_REG_RX_FINFO, 0, buff, DW1000_REG_RX_FINFO_LEN)) {
        return result;
    }
    diag->RX_preambleCount = ((*((uint32_t*) ptr)) & DW1000_REG_RX_FINFO_BITS_RXPACC_MSK) >> DW1000_REG_RX_FINFO_BITS_RXPACC_POS;
    return 0;
}

uint8_t DW1000_TX_TimeStampRead(DW1000_Handle_t* handle, void* timeStamp){
    if (handle == NULL || timeStamp == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_TX_TIME, DW1000_SUB_REG_TX_STAMP_OFFSET, (uint8_t*)timeStamp, DW1000_SUB_REG_TX_STAMP_LEN);
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
uint8_t DW1000_TX_TimeStamp_H32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_h32){
    if (handle == NULL || timeStamp_h32 == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_TX_TIME, DW1000_SUB_REG_TX_STAMP_OFFSET + 1, (uint8_t*)timeStamp_h32, sizeof(*timeStamp_h32));   
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
uint8_t DW1000_TX_TimeStamp_L32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_l32){
    if (handle == NULL || timeStamp_l32 == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_TX_TIME, DW1000_SUB_REG_TX_STAMP_OFFSET, (uint8_t*)timeStamp_l32, sizeof(*timeStamp_l32));   
}


uint8_t DW1000_RX_TimeStampRead(DW1000_Handle_t* handle, void* timeStamp){
    if (handle == NULL || timeStamp == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_RX_STAMP_OFFSET, (uint8_t*)timeStamp, DW1000_SUB_REG_RX_STAMP_LEN);
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
uint8_t DW1000_RX_TimeStamp_H32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_h32){
    if (handle == NULL || timeStamp_h32 == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_RX_STAMP_OFFSET + 1, (uint8_t*)timeStamp_h32, sizeof(*timeStamp_h32));   
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
uint8_t DW1000_RX_TimeStamp_L32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_l32){
    if (handle == NULL || timeStamp_l32 == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_RX_TIME, DW1000_SUB_REG_RX_STAMP_OFFSET, timeStamp_l32, sizeof(*timeStamp_l32));   
}

uint8_t DW1000_SystemTimeRead(DW1000_Handle_t* handle, void* sysTime){
    if (handle == NULL || sysTime == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_SYS_TIME, 0, (uint8_t*)sysTime, DW1000_REG_SYS_TIME_LEN);
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
uint8_t DW1000_SystemTime_H32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_h32){
    if (handle == NULL || timeStamp_h32 == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_SYS_TIME, 1, (uint8_t*)timeStamp_h32, sizeof(*timeStamp_h32));   
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
uint8_t DW1000_SystemTime_L32_Read(DW1000_Handle_t* handle, uint32_t* timeStamp_l32){
    if (handle == NULL || timeStamp_l32 == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_SYS_TIME, 0, (uint8_t*)timeStamp_l32, sizeof(*timeStamp_l32));
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
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    /* 读取系统配置 */
    uint32_t systemConfig;
    if (result = res_DW1000_SPI_Read(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig))) {
        return result;
    }
    /* 如果没有拒绝所有帧，说明使能了帧过滤 */
    if (config != DW1000_FF_ALL_TYPE_REJECT){
        DW1000_SET_BITS(systemConfig, DW1000_REG_SYS_CFG_BIT_FFEN_MSK);
    }
    else{
        DW1000_CLEAR_BITS(systemConfig, DW1000_REG_SYS_CFG_BIT_FFEN_MSK);

    }
    /* 将帧过滤相关的位域清除 */
    DW1000_CLEAR_BITS(systemConfig, DW1000_FRAME_FILTER_MSK);
    /* 重新设置帧过滤相关的位域 */
    DW1000_SET_BITS(systemConfig, config);
    pdw1000local->sysCFGreg = systemConfig;
    return _DW1000_SPI_Write(handle,
                             DW1000_REG_SYS_CFG,
                             0,
                             &systemConfig,
                             sizeof(systemConfig));
}

uint8_t DW1000_PAN_ID_Set(DW1000_Handle_t* handle, uint16_t PAN_id) {
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Write(handle, DW1000_REG_PANADR, 2, (uint8_t*) &PAN_id, sizeof(PAN_id));
}

uint8_t DW1000_ShortAddressSet(DW1000_Handle_t* handle, uint16_t shortAddress) {
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Write(handle, DW1000_REG_PANADR, 0, (uint8_t*) &shortAddress, sizeof(shortAddress));
}


uint8_t DW1000_EUI_Set(DW1000_Handle_t* handle, uint64_t eui){
// uint8_t DW1000_EUI_Set(DW1000_Handle_t* handle, void* eui){
    // if (handle == NULL || eui == NULL) {
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Write(handle, DW1000_REG_EUI, 0, (uint8_t*) &eui, sizeof(eui));
}

uint8_t DW1000_EUI_Get(DW1000_Handle_t* handle, uint64_t* eui){
    if (handle == NULL || eui == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_EUI, 0, (uint8_t*) eui, sizeof(*eui));
}

uint8_t DW1000_OTP_Read(DW1000_Handle_t* handle, uint16_t address, uint32_t* buff, uint16_t len) {
    if(handle == NULL || buff == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    /* 设置系统时钟为 XTI 以确保OTP读取流程正常 */
    if(result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI)) {
        return result;
    }

    for(uint16_t i = 0; i < len; i++){
        if(_DW1000_OTP_Read(handle, address + i, &buff[i])){
            return 1;
        }
    }
    /* 恢复系统时钟为 PLL */
    // _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_PLL);
    if(result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ)) {
        return result;
    }
    return 0;
}

#define DW1000_OTP_WRITE_RETRY_COUNT_MAX 10

uint8_t DW1000_OTP_WriteWord32WithVerify(DW1000_Handle_t* handle, uint32_t data, uint16_t address) {
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    /* 对编程失败的值重新尝试写入并计入重试次数，超过最大值不再尝试 */
    uint8_t retryCount = 0;
    /* 设置系统时钟为 XTI */
    if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI)) {
        return result;
    }
    /* 注意：电源供电要求为 3.7V ！！！！！ */
    /* 设置为编程模式 */
    if (result = _DW1000_OTP_MR_RegSet(handle, DW1000_OTP_MODE_INIT_PROGRAMMING)) {
        return result;
    }
    uint32_t value;
    while (1) {
        if(result = _DW1000_OTP_Word32Program(handle, data, address)){
            return result;
        }
        if(result = _DW1000_OTP_Read(handle,address,&value)){
            return result;
        }
        if(value == data){
            break;
        }
        else if(retryCount++ >= DW1000_OTP_WRITE_RETRY_COUNT_MAX){
            break;
        }
    }
    /* 在上面的写入过程中，虽然达到最大重试次数才退出，但也有可能写入成功 */
    /* 设置为读取模式 */
    if (result = _DW1000_OTP_MR_RegSet(handle, DW1000_OTP_MODE_LOW_READ_MARGIN)) {
        return result;
    }
    if(result = _DW1000_OTP_Read(handle,address,&value)){
        return result;
    }
    /* 设置OTP模式为复位所有 */
    if (result = _DW1000_OTP_MR_RegSet(handle, DW1000_OTP_MODE_RESET_ALL)) {
        return result;
    }
    if(value != data){
        return 4;
    }
    return 0;
}

uint8_t DW1000_SleepEnter(DW1000_Handle_t* handle){
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    /* 拷贝配置到AON中上传新的配置 */
    return _DW1000_AON_ArrayUpload(handle);
    /* 【总感觉缺少了什么东西，但是官方的库函数只调用了上面一个函数】 */
}

uint8_t DW1000_SleepCountConfig(DW1000_Handle_t* handle, uint16_t sleepCount){
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    uint8_t data;
    /* 强制系统时钟为 XTI */
    if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI)) {
        return result;
    }
    /* 复位睡眠配置确保我们不会意外进入睡眠模式 */
    DW1000_WRITE_REG(data, 0x00);
    /* 往子寄存器 AON_CFG0 的低8位写0x00会改变 默认的LPCLKDIVA值，不过因为通常不会使用到，所以不考虑 */
    if (result =_DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG0_OFFSET, &data, sizeof(data))) {
        return result;
    }
    /* 这里同样将子寄存器 AON_CFG1 的低8位写0x00 */
    if (result =_DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG1_OFFSET, &data, sizeof(data))) {
        return result;
    }
    /* 禁用睡眠计数器 */
    if (result = _DW1000_AON_ConfigurationUpload(handle)) {
        return result;
    }
    /* 设置睡眠计数器 */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON,DW1000_SUB_REG_SLEEP_TIM_OFFSET, (uint8_t*) &sleepCount, sizeof(sleepCount))) {
        return result;
    }
    if (result = _DW1000_AON_ConfigurationUpload(handle)) {
        return result;
    }
    /* 使能睡眠计数器 */
    DW1000_WRITE_REG(data, DW1000_SLEEP_COUNTER_ENABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG1_OFFSET, &data, sizeof(data))) {
        return result;
    }
    if (result = _DW1000_AON_ConfigurationUpload(handle)) {
        return result;
    }

    /* 恢复系统时钟为 PLL */
    if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ)) {
        return result;
    }
    return 0;
}

uint8_t DW1000_SleepCountCalibrate(DW1000_Handle_t* handle, uint16_t* tick) {
    if(handle == NULL || tick == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    uint8_t data;
    /* 使能睡眠计数器校准 */
    DW1000_WRITE_REG(data, DW1000_LOW_POWER_OSC_CALIBRATE_ENABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG1_OFFSET, &data, sizeof(data))) {
        return result;
    }
    if (result = _DW1000_AON_ConfigurationUpload(handle)) {
        return result;
    }

    /* 禁用睡眠计数器校准 */
    DW1000_WRITE_REG(data, DW1000_LOW_POWER_OSC_CALIBRATE_DISABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG1_OFFSET, &data, sizeof(data))) {
        return result;
    }
    if (result = _DW1000_AON_ConfigurationUpload(handle)) {
        return result;
    }
    /* 强制系统时钟为XTI */
    if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI)) {
        return result;
    }
    handle->delay_ms(1);

    /* 读取一个LP振荡器周期所用的XTAL/2周期数 */
    /* 设置地址 先读取高位地址 */
    DW1000_WRITE_REG(data, DW1000_AON_ADDR_LPOSC_CAL_UPPER);
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_ADDR_OFFSET, &data, sizeof(data))) {
        return result;
    }
    /* 使能手动覆盖 */
    DW1000_WRITE_REG(data, DW1000_DIRECT_AON_MEM_ACCESS_ENABLE_SET);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data))) {
        return result;
    }
    /* 读取已写入的确认信息 */
    DW1000_SET_BITS(data, DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data))) {
        return result;
    }
    /* 从AON中读回字节 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_RDAT_OFFSET, (uint8_t*) tick, 1)) {
        return result;
    }
    *tick <<= 8; // 因为读取的是高8位，所以左移8位

    /* 设置地址 读取低位地址 */
    DW1000_WRITE_REG(data, DW1000_AON_ADDR_LPOSC_CAL_LOWER);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_ADDR_OFFSET, &data, sizeof(data))) {
        return result;
    }
    /* 使能手动覆盖 */
    DW1000_WRITE_REG(data, DW1000_DIRECT_AON_MEM_ACCESS_ENABLE_SET);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data))) {
        return result;
    }
    /* 读取已写入的确认信息 */
    DW1000_SET_BITS(data, DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data))) {
        return result;
    }
    /* 从AON中读回字节 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_RDAT_OFFSET, (uint8_t*) tick, 1)) {
        return result;
    }
    /* 禁用手动覆盖 */
    DW1000_CLEAR_BITS(data, 0x00);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CTRL_OFFSET, &data, sizeof(data))) {
        return result;
    }
    /* 设置系统时钟为PLL */
    if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ)) {
        return result;
    }
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
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_SleepConfig(DW1000_Handle_t* handle, uint16_t mode, uint8_t wake) {
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    mode |= pdw1000local->sleep_mode;
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_WCFG_OFFSET, (uint8_t*) &mode, sizeof(mode))) {
        return result;
    }
    return _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_CFG0_OFFSET, &wake, sizeof(wake));
}

/**
 * @brief DW1000 设置发送后进入睡眠模式
 * @param[in] handle  DW1000 Handle 结构体指针
 * @param[in] enable  
 *  @arg DW1000_STATE_AFTER_TX_AUTO_TO_SLEEP: 发送后自动进入睡眠模式
 *  @arg DW1000_STATE_AFTER_TX_NOT_TO_SLEEP: 发送后不自动进入睡眠模式
 * @return uint8_t 函数执行结果
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_EnterSleepAfterTxSet(DW1000_Handle_t* handle, uint8_t enable) {
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    uint32_t PMSC_ctrl1;
    /* 读取寄存器内容 */
    if(result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &PMSC_ctrl1, sizeof(PMSC_ctrl1))) {
        return result;
    }
    /* 设置对应位 */
    DW1000_MODIFY_REG(PMSC_ctrl1, DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP_MSK, enable);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &PMSC_ctrl1, sizeof(PMSC_ctrl1));
}


uint8_t DW1000_WakeUpBySpiRead(DW1000_Handle_t* handle, uint8_t* buff, uint16_t len){
    if(handle == NULL || buff == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    uint32_t devId;
    /* 读取设备ID以确认芯片是否处于深睡眠 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_DEV_ID, 0, (uint8_t*) &devId, sizeof(devId))) {
        return result;
    }
    /* 芯片处于深睡眠，第一次读取失败 */
    if (devId != DW1000_DEV_ID) {
        /* 需要保持芯片片选线拉低至少500us */
        /* 通过进行一次大数据的读取操作实现持续拉低信号 */
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_DEV_ID, 0, buff, len)) {
            return result;
        }
        /* 需要5ms等待外部晶振起振与稳定 */
        /* 注意：除非频率小于3MHz，否则无法轮询状态寄存器 */
        handle->delay_ms(5);
    }
    /* 调试 - 检查芯片是否还处于睡眠模式 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_DEV_ID, 0, (uint8_t*) &devId, sizeof(devId))) {
        return result;
    }
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
uint8_t _DW1000_LoadOperParamSetFromOTP(DW1000_Handle_t* handle, uint16_t param) {
    // if(handle == NULL || data == NULL){
    //     return 2;
    // }
    // else if(handle->inited != 1){
    //     return 3;
    // }
    uint8_t result;
    uint16_t data;
    /* 设置加载操作参数集表 */
    DW1000_WRITE_REG(data, param);
    /* 设置运行参数集加载 */
    DW1000_SET_BITS(data, DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK_MSK);
    /* 强制使能LDE时钟 */
    if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_LDE)) {
        return result;
    }
    /* 写入配置 */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_OTP_IF, DW1000_SUB_REG_OTP_SF_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
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
uint8_t DW1000_SmartTxPowerSet(DW1000_Handle_t* handle, uint32_t enable){
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    uint32_t systemConfig;
    /* 读取 SYS_CFG 寄存器 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig))) {
        return result;
    }
    /* 根据传入参数配置 */
    DW1000_MODIFY_REG(systemConfig, DW1000_REG_SYS_CFG_BIT_DIS_STXP_MSK, enable);
 
    /* 写回 SYS_CFG 寄存器 */
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig));
}

uint8_t DW1000_AutoAckEnable(DW1000_Handle_t* handle, uint8_t responseDelayTime) {
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    uint32_t systemConfig = pdw1000local->sysCFGreg;
    /* 设置自动应答回复延迟 */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_ACK_RESP_T, DW1000_SUB_REG_ACK_TIM_OFFSET, &responseDelayTime, sizeof(responseDelayTime))) {
        return result;
    }
    /* 使能自动应答 */
    DW1000_SET_BITS(systemConfig, DW1000_REG_SYS_CFG_BIT_AUTOACK_MSK);
    pdw1000local->sysCFGreg = systemConfig;
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig));
}

/**
 * @brief DW1000 双缓冲模式配置
 * @param[in] handle  Comment
 * @param[in] enable  Comment
 *  @arg DW1000_DOUBLE_RX_BUFFER_ENABLE: 启用双缓冲模式
 *  @arg DW1000_DOUBLE_RX_BUFFER_DISABLE: 禁用双缓冲模式
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_DoubleRxBuffModeSet(DW1000_Handle_t* handle, uint8_t enable){
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint32_t systemConfig = pdw1000local->sysCFGreg;
    /* 根据传入参数配置双缓冲模式 */
    DW1000_MODIFY_REG(systemConfig, DW1000_REG_SYS_CFG_BIT_DBLBUFF_MSK, enable);
    pdw1000local->sysCFGreg = systemConfig;
    pdw1000local->dblbuffon = enable;
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, 0, (uint8_t*) &systemConfig, sizeof(systemConfig));
}

uint8_t DW1000_RxAfterTxDelaySet(DW1000_Handle_t* handle, uint8_t rxDelayTime) {
    if(handle == NULL){
        return 2;
    }
    else if(handle->inited != 1){
        return 3;
    }
    uint8_t result;
    uint32_t value;
    /* 读取 ACK_RESP_T 寄存器内容 */
    if(result = _DW1000_SPI_Read(handle, DW1000_REG_ACK_RESP_T, 0, (uint8_t*) &value, sizeof(value))) {
        return result;
    }
    /* 设置延迟值 */
    DW1000_MODIFY_REG(value, DW1000_REG_ACK_RESP_T_BITS_W4R_TIM_MSK, rxDelayTime);
    /* 写回 ACK_RESP_T 寄存器 */
    return _DW1000_SPI_Write(handle, DW1000_REG_ACK_RESP_T, 0, &value, sizeof(value));
}

uint8_t DW1000_RxBuffPtrsSync(DW1000_Handle_t* handle) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint8_t data;
    uint8_t buff;
    /* 确保主机的缓冲区指针在开始接收前已对齐 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, DW1000_SUB_REG_SYS_STATUS_3_OFFSET, &buff, sizeof(buff))) {
        return result;
    }
    if (DW1000_READ_BITS(buff, DW1000_SUB_REG_SYS_STATUS_3_BIT_ICRBP_MSK) != DW1000_READ_BITS(buff, DW1000_SUB_REG_SYS_STATUS_3_BIT_HSRBP_MSK)) {
        /* 交换RX Buffer 状态寄存器 */
        DW1000_WRITE_REG(data, 0x01);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL,DW1000_SUB_REG_SYS_CTRL_3_OFFSET,&data, sizeof(data))) {
            return result;
        }
    }
    return 0;
}

uint8_t DW1000_ForceTrxOff(DW1000_Handle_t* handle){
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t sysMask;
    uint32_t data;
    /* 读取 SYS_MASK 寄存器内容（暂存当前中断掩码用于后续恢复） */
    if(result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &sysMask, sizeof(sysMask))) {
        return result;
    }
    /* 关闭中断 */
    stat = decamutexon();

    /* 清除中断掩码（我们不接收不希望的中断） */
    DW1000_SET_BITS(data, 0x00);
    /* 写回 SYS_MASK 寄存器 */
    if(result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &sysMask, sizeof(sysMask))) {
        return result;
    }
    /* 关闭无线收发 */
    DW1000_WRITE_REG(data, DW1000_REG_SYS_CTRL_BIT_TRXOFF);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, (uint8_t*) &data, 1)) {
        return result;
    }
    /* 强制收发机关闭 */
    DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_TX_MSK |
                               DW1000_SYS_STATUS_ALL_RX_ERR_MSK |
                               DW1000_SYS_STATUS_ALL_RX_TO_MSK |
                               DW1000_SYS_STATUS_ALL_RX_GOOD_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, &data, sizeof(data))) {
        return result;
    }

    DW1000_RxBuffPtrsSync(handle);

    /* 恢复中断掩码 */
    if(result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &sysMask, sizeof(sysMask))) {
        return result;
    }

    /* 恢复中断 */
    decamutexoff(stat);
    pdw1000local->wait4resp = 0;
}

uint8_t DW1000_TrxDelayTimeH32Set(DW1000_Handle_t* handle, uint32_t delayTime) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    /* 只写入高32位，低8位忽略 */
    return _DW1000_SPI_Write(handle, DW1000_REG_DX_TIME, 1, (uint8_t*) &delayTime, sizeof(delayTime));
}

uint8_t DW1000_RxReset(DW1000_Handle_t* handle){
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t data;
    /* 设置接收复位 */
    DW1000_WRITE_REG(data, DW1000_SUB_REG_PMSC_CTRL0_3_BITS_SOFTRESET);
    DW1000_CLEAR_BITS(data, DW1000_RX_RESET_ENABLE_8BIT);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC,DW1000_SUB_REG_PMSC_CTRL0_3_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 清除接收复位设置 */
    DW1000_SET_BITS(data, DW1000_RX_RESET_DISABLE_8BIT);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC,DW1000_SUB_REG_PMSC_CTRL0_3_OFFSET, (uint8_t*) &data, sizeof(data));
}


/**
 * @brief 
 * @param[in] handle  Comment
 * @param[in] mode  Comment
 *  @arg DW1000_TX_MODE_IMMEDIATE: 立即发送
 *  @arg DW1000_TX_MODE_DELAYED: 延时发送
 *  @arg DW1000_TX_MODE_EXPECT_RESPONSE: 期望响应
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_TxStart(DW1000_Handle_t* handle, uint8_t mode) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint8_t data = 0x00;
    /* 如果设置了期待响应 */
    if (DW1000_READ_BITS(mode, DW1000_TX_MODE_EXPECT_RESPONSE)) {
        /* 设置等待响应位 */
        DW1000_SET_BITS(data, DW1000_REG_SYS_CTRL_BIT_WAIT4RESP_MSK);
        pdw1000local->wait4resp = 1;
    }
    /* 如果设置了延迟发送 */
    if (DW1000_READ_BITS(mode, DW1000_TX_MODE_DELAYED)) {
        /* 设置延时发送位 并 使能发送 */
        DW1000_SET_BITS(data, DW1000_REG_SYS_CTRL_BIT_TXDLYS_MSK | DW1000_REG_SYS_CTRL_BIT_TXSTRT_MSK);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data))) {
            return result;
        }
        /* 读取系统状态寄存器，只需要偏移地址0x03开始的16位数据 */
        uint16_t systemStatus;
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, DW1000_SUB_REG_SYS_STATUS_3_OFFSET, (uint8_t*) &systemStatus, sizeof(systemStatus))) {
            return result;
        }
        /* 如果出现发送错误（HPDWARN 和 TXPUTE 置位） 
            说明TXDLYS相对于DX_TIME太迟
        */
        if (DW1000_READ_BITS(systemStatus, DW1000_SYS_STATUS_O3_L16_TXERR_MSK)) {
            /* 取消延迟发送，并返回错误 */
            DW1000_WRITE_REG(data, DW1000_REG_SYS_CTRL_BIT_TRXOFF);
            if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data))) {
                return result;
            }
            return 4; // 延时发送未成功启动
        } else {
            return 0; // 延时发送成功启动
        }
    }else {
        /* 立即发送 只需使能发送位 */
        DW1000_SET_BITS(data, DW1000_REG_SYS_CTRL_BIT_TXSTRT_MSK);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data))){
            return result;
        }
    }
    return 0;
}


uint8_t DW1000_CallbacksSet(dwt_cb_t cbTxDone, dwt_cb_t cbRxOk, dwt_cb_t cbRxTo, dwt_cb_t cbRxErr)
{
    pdw1000local->cbTxDone = cbTxDone;
    pdw1000local->cbRxOk = cbRxOk;
    pdw1000local->cbRxTo = cbRxTo;
    pdw1000local->cbRxErr = cbRxErr;
}

uint8_t DW1000_IRQ_Check(DW1000_Handle_t* handle, uint8_t* flag) {
    if(handle == NULL || flag == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint8_t status;
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, &status, sizeof(status))) {
        return result;
    }
    *flag = DW1000_READ_BITS(status, DW1000_REG_SYS_STATUS_BIT_IRQS_MSK);
    return 0;
}

/**
 * @brief 
 * @param[in] handle  Comment
 * @param[in] enable  Comment
 *  @arg DW1000_LOW_POWER_LISTENING_MODE_ENABLE: 启用低功耗监听模式
 *  @arg DW1000_LOW_POWER_LISTENING_MODE_DISABLE: 禁用低功耗监听模式
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_LowPowerListeningSet(DW1000_Handle_t* handle, uint32_t enable) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t pmsc_ctrl1;
    /* 读取 PMSC_CTRL1 寄存器内容 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &pmsc_ctrl1, sizeof(pmsc_ctrl1))) {
        return result;
    }
    /* 根据配置设置相应位 */
    DW1000_MODIFY_REG(pmsc_ctrl1, DW1000_LOW_POWER_LISTENING_MODE_MASK, enable);
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &pmsc_ctrl1, sizeof(pmsc_ctrl1));
}

uint8_t DW1000_SnoozeTimeSet(DW1000_Handle_t* handle, uint8_t snoozeTime) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_SNOZT_OFFSET, (uint8_t*) &snoozeTime, sizeof(snoozeTime));
}

uint8_t DW1000_LEDsSet(DW1000_Handle_t* handle, uint8_t enable) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    ///////////////////////////////////////////////////////
    // return _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_LEDC_OFFSET, (uint8_t*) &config, sizeof(config));
}

/**
 * @brief 
 * @param[in] handle  Comment
 * @param[in] enable  Comment
 *  @arg DW1000_PLL2_SEQ_NORMAL: 正常模式
 *  @arg DW1000_PLL2_SEQ_RX_SNIFF: 监听模式
 * @param[in] timeOn  Comment
 * @param[in] timeOff  Comment
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_SniffModeSet(DW1000_Handle_t* handle, uint8_t enable, uint8_t timeOn, uint8_t timeOff) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t pmsc_ctrl0;
    uint16_t rxSniff;
    /* 读取 PMSC_CTRL0 寄存器内容 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &pmsc_ctrl0, sizeof(pmsc_ctrl0))) {
        return result;
    }
    if (enable != DW1000_PLL2_SEQ_RX_SNIFF) {
        timeOff = 0x00;
        timeOn = 0x00;
    }
    /* 配置 RX_SNIFF 时间 */
    rxSniff = ((uint16_t) timeOff << 8) | timeOn;
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RX_SNIFF, 0, (uint8_t*) &rxSniff, sizeof(rxSniff))) {
        return result;
    }
    /* 根据配置设置相应位 */
    DW1000_MODIFY_REG(pmsc_ctrl0, DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_MSK, enable);
    if (_DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &pmsc_ctrl0, sizeof(pmsc_ctrl0))) {
        return result;
    }
}

/**
 * @brief 
 * @param[in] handle  Comment
 * @param[in] timeout  Comment
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_RxTimeoutSet(DW1000_Handle_t* handle, uint16_t timeout) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint8_t data;
    /* 读取 SYS_CFG 寄存器内容（只需要偏移地址0x03 的 8位数据） */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_CFG, DW1000_SUB_REG_SYS_CFG_3_OFFSET, &data, sizeof(data))) {
        return result;
    }
    if (timeout == 0) {
        /* 禁用接收超时 */
        DW1000_CLEAR_BITS(data, DW1000_SUB_REG_SYS_CFG_3_BIT_RXWTOE_MSK);
        pdw1000local->sysCFGreg &= ~(SYS_CFG_RXWTOE);
    } 
    else {
        /* 启用接收超时 */
        DW1000_SET_BITS(data, DW1000_SUB_REG_SYS_CFG_3_BIT_RXWTOE_MSK);
        pdw1000local->sysCFGreg |= SYS_CFG_RXWTOE;
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_RX_FWTO, 0, (uint8_t*) &timeout, sizeof(timeout))) {
            return result;
        }
    }
    return _DW1000_SPI_Write(handle, DW1000_REG_SYS_CFG, DW1000_SUB_REG_SYS_CFG_3_OFFSET, (uint8_t*) &data, sizeof(data));
}


/**
 * @brief 
 * @param[in] handle  Comment
 * @param[in] mode  Comment
 *  @arg DW1000_RX_MODE_IMMEDIATE: 立即接收
 *  @arg DW1000_RX_MODE_DELAYED: 延时接收
 *  @arg DW1000_RX_MODE_NO_SYNC_PTRS: 不同步接收指针
 *  @arg DW1000_RX_MODE_IDLE_ON_DELAY_ERR: 延时接收错误时进入空闲状态
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_RxEnable(DW1000_Handle_t* handle, uint8_t mode) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint16_t data;
    /* 使能接收 */
    DW1000_WRITE_REG(data, DW1000_REG_SYS_CTRL_BIT_RXENAB);
    /* 如果设置不同步接收指针 */
    if (DW1000_READ_BITS(mode, DW1000_RX_MODE_NO_SYNC_PTRS)) {
        /* 设置不同步接收指针位 */
        if (result = DW1000_RxBuffPtrsSync(handle)) {
            return result;
        }
    }
    /* 如果设置了延时接收 */
    if (DW1000_READ_BITS(mode, DW1000_RX_MODE_DELAYED)) {
        /* 设置延时接收位 */
        DW1000_SET_BITS(data, DW1000_REG_SYS_CTRL_BIT_RXDLYE_MSK);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data))) {
            return result;
        }
        /* 检查错误 */
        uint8_t systemStatus;
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, DW1000_SUB_REG_SYS_STATUS_3_OFFSET, &systemStatus, sizeof(systemStatus))) {
            return result;
        }
        /* 如果延迟以达到则立即开启接收除非使能了 DW1000_RX_MODE_IDLE_ON_DELAY_ERR */
        if (DW1000_READ_BITS(systemStatus, DW1000_SUB_REG_SYS_STATUS_3_BIT_HPDWARN)) {
            /* 强制关闭收发机 */
            if (result = DW1000_ForceTrxOff(handle)) {
                return result;
            }
            /* 如果没有使能延时错误进入空闲模式 */
            if (!DW1000_READ_BITS(mode, DW1000_RX_MODE_IDLE_ON_DELAY_ERR)) {
                /* 重新使能接收 */

                if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data))) {
                    return result;
                }
            }
    } else {
        /* 立即接收 只需使能接收位 */
        DW1000_WRITE_REG(data, DW1000_REG_SYS_CTRL_BIT_RXENAB);
        if (_DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, 0, &data, sizeof(data))) {
            return result;
        }
    }
    return 0;
}


/**
 * @brief 
 * @param[in] handle  Comment
 * @param[in] pdTimeout  Comment
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_PreambleDetectTimeoutSet(DW1000_Handle_t* handle, uint16_t pdTimeout) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Write(handle, DW1000_REG_DRX_CONF, DW1000_SUB_REG_DRX_PRETOC_OFFSET, (uint8_t*) &pdTimeout, sizeof(pdTimeout));
}



/**
 * @brief 
 * @param[in] handle  Comment
 * @param[in] mask  Comment
 *  @arg DW1000_INT_EVENT_FRAME_SENT: 帧发送完成中断
 *  @arg DW1000_INT_EVENT_FRAME_RECIVED: 帧接收完成中断
 *  @arg DW1000_INT_EVENT_RX_PHR_ERROR: PHR错误中断
 *  @arg DW1000_INT_EVENT_RX_CRC_ERROR: CRC错误中断
 *  @arg DW1000_INT_EVENT_RX_SYNC_LOST: 同步丢失中断
 *  @arg DW1000_INT_EVENT_RX_TIMEOUT: 接收超时中断
 *  @arg DW1000_INT_EVENT_SFD_TIMEOUT: SFD超时中断
 *  @arg DW1000_INT_EVENT_FRAME_REJECTED: 帧拒绝中断
 * @param[in] operation  Comment
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_InterruptSet(DW1000_Handle_t * handle, uint32_t mask, uint8_t operation) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t systemMask;
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &systemMask, sizeof(systemMask))) {
        return result;
    }

    // stat = decamutexon();
    
    if (operation == 2){
        DW1000_WRITE_REG(systemMask, mask);
    } else if (operation == 1) {
        DW1000_SET_BITS(systemMask, mask);
    } else{
        DW1000_CLEAR_BITS(systemMask, mask);
    }

    if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_MASK, 0, (uint8_t*) &mask, sizeof(mask))) {
        return result;
    }
    
    // decamutexoff(stat);
    
    return 0;
}

/**
 * @brief 
 * @param[in] handle  Comment
 * @param[in] enable  Comment
 *  @arg DW1000_EVENT_COUNT_ENABLE: 启用事件计数器
 *  @arg DW1000_EVENT_COUNT_DISABLE: 禁用事件计数器
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_EventCountersConfig(DW1000_Handle_t* handle, uint8_t enable) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint8_t eventCounterCtrl;
    /* 需要先清零和使能，不能直接清除 */
    DW1000_WRITE_REG(eventCounterCtrl, DW1000_EVENT_COUNT_CLEAR);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_DIG_DIAG, DW1000_SUB_REG_EVC_CTRL_OFFSET, (uint8_t*) &eventCounterCtrl, sizeof(eventCounterCtrl))) {
        return result;
    }
    /* 根据配置设置相应位 */
    DW1000_MODIFY_REG(eventCounterCtrl, DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN_MSK, enable);
    return _DW1000_SPI_Write(handle, DW1000_REG_DIG_DIAG, DW1000_SUB_REG_EVC_CTRL_OFFSET, (uint8_t*) &eventCounterCtrl, sizeof(eventCounterCtrl));
}

uint8_t DW1000_EventCountersRead(DW1000_Handle_t* handle,DW1000_EVT_CNT_t* counters) {
    if(handle == NULL || counters == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint16_t* ptr;
   /* 因为结构体的成员是按照DW1000芯片寄存器顺序排列的，可以直接给整个结构体赋值 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_DIG_DIAG, DW1000_SUB_REG_EVC_PHE_OFFSET, (uint8_t*) counters, sizeof(DW1000_EVT_CNT_t))) {
        return result;
    }
    /* 有计数器长度为12bit，所以高4位需要清除 */
    for (ptr = (uint16_t*) counters; ptr < (uint16_t*) counters + sizeof(DW1000_EVT_CNT_t) / sizeof(uint16_t); ptr++) {
        *ptr &= 0x0FFF;
    }
    return 0;
}

uint8_t DW1000_Softreset(DW1000_Handle_t* handle) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t data;
    if (result = _DW1000_SequeningDisable(handle)) {
        return result;
    }
    /* 清除 AON 自动下载位（当复位时会触发AON下载） */
    DW1000_WRITE_REG(data, 0x00);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_WCFG_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 清除 wakeup 配置 */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_AON, DW1000_SUB_REG_AON_WCFG_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 上传新的配置 */
    if (result = _DW1000_AON_ArrayUpload(handle)) {
        return result;
    }
    /* 复位 HIF，TX，RX 和 PMSC（复位位置位） */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_3_OFFSET, (uint8_t*) &data, 1)) {
        return result;
    }
    /* 延迟10us等待DW1000 PLL时钟锁定 */
    handle->delay_ms(1);

    /* 清除复位位 */
    DW1000_WRITE_REG(data, 0xF0);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_3_OFFSET, (uint8_t*) &data, 1)) {
        return result;
    }
    pdw1000local->wait4resp = 0;
    return 0;
}

uint8_t DW1000_XtalTrimSet(DW1000_Handle_t* handle, uint8_t trimValue) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    /* 高3位为保留位，必须设置为 011 */
    DW1000_MODIFY_REG(trimValue, 0xE0, 0x60);
    return _DW1000_SPI_Write(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_XTALT_OFFSET, (uint8_t*) &trimValue, sizeof(trimValue));
}

uint8_t DW1000_XtalTrimGet(DW1000_Handle_t* handle, uint8_t* trimValue) {
    if(handle == NULL || trimValue == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result = _DW1000_SPI_Read(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_XTALT_OFFSET, trimValue, sizeof(*trimValue));
    /* 把高三位清零0（因为是保留位） */
    DW1000_CLEAR_BITS(*trimValue, 0xE0);
    return 0;
}

uint8_t DW1000_CW_ModeConfig(DW1000_Handle_t* handle, uint8_t channel) {
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t data;
    /* 禁用 TX/RX RF块sequnecing（CW模式需要） */
    if (result = _DW1000_SequeningDisable(handle)) {
        return result;
    }
    /* 配置RF_PLL */
    DW1000_WRITE_REG(data,DW1000_FS_PLL_CFG[channel]);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_PLLCFG_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 配置 PLL 调谐 */
    DW1000_WRITE_REG(data,DW1000_FS_PLL_TUNE[channel]);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_FS_CTRL, DW1000_SUB_REG_FS_PLLTUNE_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
        return result;
    }
    /* 配置 RF TX 控制寄存器 */
    DW1000_WRITE_REG(data,DW1000_RF_TX_CFG[channel]);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_TXCTRL_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 使能 RF PLL */
    /* 使能 LDO 和 RF PLL 块 */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_ALL_PLL_ENABLE | DW1000_RF_CFG_LDO_FORCE_ENABLE);
    if (result = _DW1000_SPI_Write(handle,DW1000_REG_RF_CONF,DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 使能TX块复位 */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_ALL_PLL_ENABLE | DW1000_RF_CFG_LDO_FORCE_ENABLE | DW1000_RF_CFG_TX_ENABLE | DW1000_RF_CFG_TXB_ENABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF,DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }

    /* 配置TX块 */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL | DW1000_TX_CLOCK_SELECT_FORCE_PLL);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
        return result;
    }
    DW1000_WRITE_REG(data, 0x07); /* 该3位应该也是有关系的，但是手册里只记录了ADCCE位，其余位没有说明 */
    
    /* 禁用 精细颗粒 Tx 时序流程 */
    if (result = DW1000_FineGrainTxSeqCmd(handle, DW1000_TX_FINE_GRAIN_SEQ_DISABLE)) {
        return result;
    }

    /* 配置CW模式 */
    DW1000_WRITE_REG(data, DW1000_TC_PGTEST_MODE_CW);
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PGTEST_OFFSET, (uint8_t*) &data, sizeof(uint8_t));
}

/**
 * @brief
 * @param[in] handle  Comment
 * @param[in] repetition 重复间隔（两次发送的间隔），最小为4，单位大概为 8ns(更精确的值为 512/499.2e6/128 s)
 * @return uint8_t
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_CF_ModeConfig(DW1000_Handle_t* handle, uint32_t repetition){
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t data;
    /* 禁用 TX/RX RF块时序流程（连续帧模式要求） */
    if (result = _DW1000_SequeningDisable(handle)) {
        return result;
    }
    /* 使能 RF PLL 和 TX 块 */
    /* 使能 LDO 和 RF PLL 块 */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_ALL_PLL_ENABLE | DW1000_RF_CFG_LDO_FORCE_ENABLE);
    if (result = _DW1000_SPI_Write(handle,DW1000_REG_RF_CONF,DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 使能TX块复位 */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_ALL_PLL_ENABLE | DW1000_RF_CFG_LDO_FORCE_ENABLE | DW1000_RF_CFG_TX_ENABLE | DW1000_RF_CFG_TXB_ENABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF,DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }

    /* 配置TX块 */
    if (result = _DW1000_ClockCmd(handle, DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL)) {
        return result;
    }
    if (result = _DW1000_TxClockCmd(handle, DW1000_TX_CLOCK_SELECT_FORCE_PLL)) {
        return result;
    }
    
    /* 设置发送重复间隔 */
    if (repetition < 4) {
        repetition = 4;
    }
    if (result = _DW1000_SPI_Write(handle,DW1000_REG_RX_TIME,0,(uint8_t*) &repetition, sizeof(repetition))) {
        return result;
    }
    /* 配置Tx连续帧 */
    DW1000_WRITE_REG(data, DW1000_TEST_MODE_TX_PWR_SPECTRUM_ENABLE);
    return _DW1000_SPI_Write(handle, DW1000_REG_DIG_DIAG, DW1000_SUB_REG_DIAG_TMC_OFFSET, (uint8_t*) &data, sizeof(uint8_t));
}

uint8_t DW1000_TempAndVoltRead(DW1000_Handle_t* handle, uint8_t* temp, uint8_t* volt) {
    if(handle == NULL || temp == NULL || volt == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint16_t data;
    /* 以下操作需要单字节按顺序写入 */
    DW1000_WRITE_REG(data, 0x80); /* 使能 TLD 偏置 */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x11, (uint8_t*) &data, sizeof(uint8_t))) {
        return result;
    }
    DW1000_WRITE_REG(data, 0x0A); /* 使能 TLD 偏置和 ADC 偏置 */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x12, (uint8_t*) &data, sizeof(uint8_t))) {
        return result;
    }
    DW1000_WRITE_REG(data, 0x0F); /* 使能输出（仅当偏置启动并运行后） */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, 0x12, (uint8_t*) &data, sizeof(uint8_t))) {
        return result;
    }
    if(SPI_isFast == 1){
        /* 读取所有SAR输入 */
        DW1000_WRITE_REG(data, 0x00);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
            return result;
        }
        DW1000_WRITE_REG(data, 0x01); /* 设置 SAR 使能 */
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
            return result;
        }
        /* 当时用PLL时钟（以及高速SPI通信），延迟是必须的 */
        handle->delay_ms(1); 
        /* 读取电压和温度 */
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARL_OFFSET, (uint8_t*) &data, sizeof(data))) {
            return result;
        }
    } else { /* 切换为低速时钟 */
        /* 注意：设置系统为外部晶振（必须执行该步骤以确保读取的值是可靠的） */
        if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_FORCE_SYS_XTI)) {
            return result;
        }
        /* 读取所有SAR输入 */
        DW1000_WRITE_REG(data, 0x00);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
            return result;
        }
        DW1000_WRITE_REG(data, 0x01); /* 设置 SAR 使能 */
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
            return result;
        }
        /* 读取电压和温度 */
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARL_OFFSET, (uint8_t*) &data, sizeof(data))) {
            return result;
        }
        /* 恢复系统默认时钟 */
        if (result = _DW1000_ClockCmd(handle, DW1000_CLOCK_CMD_ENABLE_ALL_SEQ)) {
            return result;
        }
    }
    *volt = data; /* 低8位电压值 */
    *temp = data >> 8; /* 高8位温度值 */
    /* 清除 SAR 使能 */
    DW1000_WRITE_REG(data, 0x00);
    return _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_SARC_OFFSET, (uint8_t*) &data, sizeof(uint8_t));
}

#define DW1000_TEMP_RAW_TO_REAL_COEFF 1.14F

float DW1000_TempRawToRealConvert(uint8_t rawTemp) {
    /* 
        根据 DW1000 数据手册 V2.13 中的公式进行转换: 
        Temperature (°C) = ( (SAR_LTEMP – OTP_READ(Vtemp @ 23°C) ) x 1.14) + 23 
    */
    return ((rawTemp - pdw1000local->tempP) * DW1000_TEMP_RAW_TO_REAL_COEFF) + 23 ;
}

// /* 0.1是对原先浮点温度的放大倍数10，这里/10进行还原，乘256,是将数字放大256倍进行转换计算，提高精度 */
// #define DW1000_TEMP_REAL_TO_RAW_CONV  (1 / DW1000_TEMP_RAW_TO_REAL_COEFF)

/**
 * @brief 
 * @param[in] realTemp  Comment
 * @return uint8_t 
 * @note  为了方便计算，原先浮点温度，放大10倍作为整数进行计算
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
uint8_t DW1000_TempRealToRawConvert(int16_t realTemp_x10) {
    /* 
        根据 DW1000 数据手册 V2.13 中的公式进行转换: 
        SAR_LTEMP = ( (Temperature (°C) - 23) / 1.14 ) + OTP_READ(Vtemp @ 23°C)
    */
   /* 放大256倍用于提高精度 */
    int32_t rawTemp = ((realTemp_x10 - 230 + 5) / 10 / DW1000_TEMP_RAW_TO_REAL_COEFF * 256); /* +5是为了四舍五入 */
    /* 在上面的转换中数值被放大了256倍，这里进行还原，同时确保符号位正确 */
    rawTemp = (-rawTemp >> 8);
    rawTemp = -rawTemp;
    return (uint8_t) (rawTemp + pdw1000local->tempP);
}

#define DW1000_VOLT_REAL_TO_RAW_COEFF 137

float DW1000_VoltRawToRealConvert(uint8_t rawVolt) {
    /* 根据 DW1000 数据手册 V2.13 中的公式进行转换: 
        Voltage (V) = ( (SAR_LVBAT – OTP_READ(Vmeas @ 3.3 V) ) / 173 ) + 3.3
    */
   
    return ((float)(rawVolt - pdw1000local->voltP) / DW1000_VOLT_REAL_TO_RAW_COEFF + 3.3F);
}

uint8_t DW1000_VoltRawToRealConvert(int32_t realVolt_x1000) {
    /* 根据 DW1000 数据手册 V2.13 中的公式进行转换: 
        SAR_LVBAT = ( (Voltage (V) - 3.3) * 173 ) + OTP_READ(Vmeas @ 3.3 V)
    */
    int32_t rawVolt = ((realVolt_x1000 - 3300 + 50) / 100 * DW1000_VOLT_REAL_TO_RAW_COEFF); /* +50是为了四舍五入 */
    return (uint8_t) (rawVolt + pdw1000local->voltP);
}

uint8_t DW1000_WakeUpTempRead(DW1000_Handle_t* handle, uint8_t* temp) {
    if(handle == NULL || temp == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    return _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_SAR_LTEMP_OFFSET, temp, sizeof(*temp));
}

uint8_t DW1000_WakeUpVoltRead(DW1000_Handle_t* handle, uint8_t* volt) {
    if(handle == NULL || volt == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
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
uint8_t DW1000_BW_TempCompCalculate(DW1000_Handle_t* handle,uint16_t target,uint32_t* best){
    if(handle == NULL || best == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t data;
    /* 存储相关寄存器变量用于计算完成后复原 */
    uint8_t oldPmscCtrl0;
    uint16_t oldPmscCtrl1;
    uint32_t oldRfConf;
    /* 保存当前寄存器的值（用于后续恢复） */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &oldPmscCtrl0, sizeof(oldPmscCtrl0))) {
        return result;
    }
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &oldPmscCtrl1, sizeof(oldPmscCtrl1))) {
        return result;
    }
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &oldRfConf, sizeof(oldRfConf))) {
        return result;
    }
    /* 设置时钟为XTAL */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI); /* 19MHz */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
        return result;
    }
    /* 禁用时序流程 */
    DW1000_WRITE_REG(data, DW1000_PMSC_ANALOG_RF_CTRL_DISABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) data, sizeof(uint16_t))) {
        return result;
    }
    /* 打开CLK PLL，Mix偏置 和 PG */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_LDO_FORCE_ENABLE | DW1000_RF_CFG_TXB_MIXER_BIAS_ENABLE | DW1000_RF_CFG_PULSE_GEN_ENABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 将系统和Tx时钟设置为PLL */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL | DW1000_TX_CLOCK_SELECT_FORCE_PLL); /* 125MHz */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
        return result;
    }
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
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PGDELAY_OFFSET, (uint8_t*) &BW_current, sizeof(BW_current))) {
            return result;
        }
        /* 设置计算方向和时间 */
        DW1000_WRITE_REG(data, DW1000_PG_CTRL_DIR_CONV_HIGHER | DW1000_PG_TMEAS_MAX);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_CTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
            return result;
        }
        /* 开始计算 */
        DW1000_SET_BITS(data, DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_CTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
            return result;
        }
        /* 等待计算完成 */
        handle->delay_ms(1);
        /* 
            读取计算结果 
            从 TC_PG_STATUS 寄存器读取结果
        */
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_STATUS_OFFSET, (uint8_t*) &countRaw, sizeof(countRaw))) {
            return result;
        }
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
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &oldPmscCtrl0, sizeof(oldPmscCtrl0))) {
        return result;
    }
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &oldPmscCtrl1, sizeof(oldPmscCtrl1))) {
        return result;
    }
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &oldRfConf, sizeof(oldRfConf))) {
        return result;
    }
    return 0;
}

#define DW1000_DA_ATTN_STEP     (2.5)
#define DW1000_MIXER_GAIN_STEP  (0.5)
#define DW1000_MIXER_DA_FRACTOR (DW1000_DA_ATTN_STEP / DW1000_MIXER_GAIN_STEP)

/**
 * @brief 
 * @param[in] txPowerRef  Comment
 * @param[in] powerAdj  Comment
 * @return uint32_t 
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
    for(uint8_t i = 0; i < 4; i++){
        DA_ATTN_change = 0;
        mixerGainChange = powerAdj;
        DA_ATTN_current = ((txPowerRef >> (i * 8)) & 0xE0) >> 5; /* 按字节取高3位 */
        mixerGainCurrent = (txPowerRef >> (i * 8)) & 0x1F; /* 按字节取低5位 */
        /* Mixer Gain 增益值在[4,20]之间表现最好 */
        while ((mixerGainCurrent + mixerGainChange < 4) || (mixerGainCurrent + mixerGainChange > 20)) {
            /* 如果 Mixer 增益值超出范围，调整 DA 衰减进行补偿 */
            if (mixerGainCurrent + mixerGainChange > 20) {
                /* 如果DA衰减达到最大值 */
                if(--DA_ATTN_change == 0){
                    /* 恢复上一次的值并退出循环，DA已达到最大值 */
                    DA_ATTN_change = 1;
                    break;
                }
                mixerGainChange -= (int8_t)DW1000_MIXER_DA_FRACTOR;
            } else if (mixerGainCurrent + mixerGainChange < 4) {
                /* 如果DA衰减达到最小值 */
                if (++DA_ATTN_change == 8) {
                    /* 恢复上一次的值并退出循环，DA已达到最小值 */
                    DA_ATTN_change = 7;
                    break;
                }
                mixerGainChange += (int8_t)DW1000_MIXER_DA_FRACTOR;
            }
        }
        DA_ATTN_new = (DA_ATTN_current + DA_ATTN_change) & 0x07;
        mixerGainNew = (mixerGainCurrent + mixerGainChange) & 0x1F;
        result |= ((uint32_t)(DA_ATTN_new << 5 | mixerGainNew) << (i * 8));
    }
    return result;
}

#define DW1000_TEMP_COMP_FACTOR_CH2 (327) // ((int16_t)(0.0798*4096))
#define DW1000_TEMP_COMP_FACTOR_CH5 (607) // ((int16_t)(0.1482*4096))

uint32_t DW1000_TxPowerTempCompCalculate(uint8_t channel,uint32_t txPowerRef,int8_t tempDelta){
    int8_t powerDelta;
    uint8_t isNegative = 0;
    if (tempDelta < 0) {
        isNegative = 1;
        tempDelta = -tempDelta;
    }
    /* 计算当前温度下的预期功率差 */
    if (channel == 5){
        powerDelta = ((tempDelta * DW1000_TEMP_COMP_FACTOR_CH5) >> 12); /* 除以4096 */
    } else if (channel == 2){
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
#define DW1000_SAMPLE_NUM  10

uint16_t DW1000_PG_CounterCalculate(DW1000_Handle_t* handle,uint8_t pgdly,uint16_t* delayCount){
    if(handle == NULL || delayCount == NULL){
        return 0;
    } else if (handle->inited != 1) {
        return 0;
    }
    uint8_t result;
    uint32_t data;
    /* 暂存相关寄存器的值便于之后复原 */
    uint8_t oldPmscCtrl0;
    uint16_t oldPmscCtrl1;
    uint32_t oldRfConf;
    /* 保存当前寄存器的值（用于后续恢复） */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, &oldPmscCtrl0, sizeof(oldPmscCtrl0))) {
        return result;
    }
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &oldPmscCtrl1, sizeof(oldPmscCtrl1))) {
        return result;
    }
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &oldRfConf, sizeof(oldRfConf))) {
        return result;
    }
    /* 设置时钟为XTAL */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI); /* 19MHz */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
        return result;
    }
    /* 禁用时序流程 */
    DW1000_WRITE_REG(data, DW1000_PMSC_ANALOG_RF_CTRL_DISABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) data, sizeof(uint16_t))) {
        return result;
    }
    /* 打开CLK PLL，Mix偏置 和 PG */
    DW1000_WRITE_REG(data, DW1000_RF_CFG_LDO_FORCE_ENABLE | DW1000_RF_CFG_TXB_MIXER_BIAS_ENABLE | DW1000_RF_CFG_PULSE_GEN_ENABLE);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    /* 将系统和Tx时钟设置为PLL */
    DW1000_WRITE_REG(data, DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL | DW1000_TX_CLOCK_SELECT_FORCE_PLL); /* 125MHz */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
        return result;
    }
    uint32_t sum = 0;
    uint16_t count;

    for (uint8_t i = 0; i < DW1000_SAMPLE_NUM; i++) {
        /* 往PG_DELAY寄存器写入PG延迟值 */
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PGDELAY_OFFSET, (uint8_t*) &pgdly, sizeof(pgdly))) {
            return result;
        }
        /* 设置计算方向和时间 */
        DW1000_WRITE_REG(data, DW1000_PG_CTRL_DIR_CONV_HIGHER | DW1000_PG_TMEAS_MAX);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_CTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
            return result;
        }
        /* 开始计算 */
        DW1000_SET_BITS(data, DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_CTRL_OFFSET, (uint8_t*) &data, sizeof(uint8_t))) {
            return result;
        }
        /* 等待计算完成 */
        handle->delay_ms(1);
        /* 
            读取计算结果 
            从 TC_PG_STATUS 寄存器读取结果
        */
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_TX_CAL, DW1000_SUB_REG_TC_PG_STATUS_OFFSET, (uint8_t*) &count, sizeof(count))) {
            return result;
        }
        sum += count;
    }
    *delayCount = sum / DW1000_SAMPLE_NUM;
    /* 恢复之前保存的寄存器值 */
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL0_OFFSET, (uint8_t*) &oldPmscCtrl0, sizeof(oldPmscCtrl0))) {
        return result;
    }
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_PMSC, DW1000_SUB_REG_PMSC_CTRL1_OFFSET, (uint8_t*) &oldPmscCtrl1, sizeof(oldPmscCtrl1))) {
        return result;
    }
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_RF_CONF, DW1000_SUB_REG_RF_CONF_OFFSET, (uint8_t*) &oldRfConf, sizeof(oldRfConf))) {
        return result;
    }
    return 0;
}

uint8_t DW1000_Init(DW1000_Handle_t* handle){
    if (handle == NULL){
        return 2;
    } else if ()
}


uint8_t DW1000_IRQHandler(DW1000_Handle_t* handle){
    if(handle == NULL){
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;

    return 0;
}

#define DW1000_FCTRL_LEN_MAX 2

/**
 * @brief DW1000 中断服务程序
 * @param[in] handle  Comment
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 可以优化，按照hal库的编写规则
 */
uint8_t DW1000_ISR(DW1000_Handle_t* handle){
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t systemStatus;
    uint32_t data;
    /* 读取SYS_STATUS寄存器低16位 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &systemStatus, sizeof(systemStatus))) {
        return result;
    }
    pdw1000local->cbData.status = systemStatus;

    /* 处理 Rx good frame事件 */
    if (DW1000_READ_BITS(systemStatus, DW1000_REG_SYS_STATUS_BIT_RXFCG_MSK)) {
        uint16_t frameInfo;
        uint16_t frameLen;
        uint16_t frameCtrl;
        /* 清除所有接收标志位（通过往SYS_STATUS对应位写1即可清除） */
        DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_RX_GOOD_MSK);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data))) {
            return result;
        }
        pdw1000local->cbData.rx_flags = 0;
        /* 读取帧信息（只需要最前面的两个字节的数据） */
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_RX_FINFO, 0, (uint8_t*) &frameInfo, sizeof(frameInfo))) {
            return result;
        }
        /* 获取帧长度信息 */
        frameLen = DW1000_READ_BITS(frameInfo, DW1000_RX_FINFO_RXFL_1023_MSK);
        pdw1000local->cbData.datalength = frameLen;
        /* 判断是否为信标帧 */
        if(DW1000_READ_BITS(frameInfo, DW1000_REG_RX_FINFO_BIT_RNG_MSK)){
            pdw1000local->cbData.rx_flags |= DWT_FRAME_TYPE_BEACON;
        }
        /* 读取帧控制信息 */
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_RX_BUFFER, 0, (uint8_t*) &frameCtrl, sizeof(frameCtrl))) {
            return result;
        }
        pdw1000local->cbData.fctrl = frameCtrl;

        if (DW1000_READ_BITS(systemStatus,DW1000_REG_SYS_STATUS_BIT_AAT) && ((pdw1000local->cbData.fctrl[0] & FCTRL_ACK_REQ_MASK) == 0))    {
            /* 清除AAT标志位 */
            DW1000_WRITE_REG(data, DW1000_REG_SYS_STATUS_BIT_AAT);
            if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data))) {
                return result;
            }
            pdw1000local->cbData.status &= ~SYS_STATUS_AAT;
            pdw1000local->wait4resp = 0;
        }
        /* 调用相应的回调函数（如果存在） */
        if (pdw1000local->cbRxOk != NULL) {
            pdw1000local->cbRxOk(&pdw1000local->cbData);
        }

        if (pdw1000local->dblbuffon) {
            /* 切换主机端接收缓冲区指针 */
            DW1000_WRITE_REG(data,DW1000_SUB_REG_SYS_CTRL_3_BIT_HRBPT);
            if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_CTRL, DW1000_SUB_REG_SYS_CTRL_3_OFFSET, (uint8_t*) &data, sizeof(data))) {
                return result;
            }
        }
    }
    /* 处理发送确认事件 */
    if (DW1000_READ_BITS(systemStatus, DW1000_REG_SYS_STATUS_BIT_TXFRS_MSK)) {
        /* 清除发送完成标志位 */
        DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_TX_MSK);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data))) {
            return result;
        }
        /*  */
        if (DW1000_READ_BITS(systemStatus, DW1000_REG_SYS_STATUS_BIT_AAT) && pdw1000local->wait4resp) {
            if (result = DW1000_ForceTrxOff(handle)) {
                return result;
            }
            if (result = DW1000_RxReset(handle)) {
                return result;
            }
        }
        /* 调用相应的回调函数（如果存在） */
        if (pdw1000local->cbTxDone != NULL) {
            pdw1000local->cbTxDone(&pdw1000local->cbData);
        }
    }
    /* 处理帧接收/前导码检测超时事件 */
    if (DW1000_READ_BITS(systemStatus, DW1000_SYS_STATUS_ALL_RX_TO_MSK)) {
        /* 清除接收超时标志位 */
        DW1000_WRITE_REG(data, DW1000_REG_SYS_STATUS_BIT_RXRFTO);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data))) {
            return result;
        }
        pdw1000local->wait4resp = 0;

        if (result = DW1000_ForceTrxOff(handle)) {
                return result;
        }
        if (result = DW1000_RxReset(handle)) {
            return result;
        }

        /* 调用相应的回调函数（如果存在） */
        if (pdw1000local->cbRxTo != NULL) {
            pdw1000local->cbRxTo(&pdw1000local->cbData);
        }
    }
    /* 处理接收错误事件 */
    if (DW1000_READ_BITS(systemStatus, DW1000_SYS_STATUS_ALL_RX_ERR_MSK)) {
        /* 清除接收错误标志位 */
        DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_RX_ERR_MSK);
        if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data))) {
            return result;
        }
        pdw1000local->wait4resp = 0;
        if (result = DW1000_ForceRxOff(handle)) {
            return result;
        }
        if (result = DW1000_RxReset(handle)) {
            return result;
        }
        /* 调用相应的回调函数（如果存在） */
        if (pdw1000local->cbRxErr != NULL) {
            pdw1000local->cbRxErr(&pdw1000local->cbData);
        }
    }
}

/**
 * @brief 
 * @param[in] handle  Comment
 * @return uint8_t 
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 可以优化，按照hal库的编写规则
 */
uint8_t DW1000_ISR_AtLowPowerListening(DW1000_Handle_t* handle){
    if (handle == NULL) {
        return 2;
    } else if (handle->inited != 1) {
        return 3;
    }
    uint8_t result;
    uint32_t data;
    uint32_t systemStatus;
    /* 读取SYS_STATUS寄存器低16位 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &systemStatus, sizeof(systemStatus))) {
        return result;
    }
    pdw1000local->cbData.status = systemStatus;
    /* 处理RX Good Frame事件 */ 
    /* 在低功耗模式中只有 RX Good Frame会触发中断，所以直接处理，不判断事件类型 */
    /* 在清除中断标志前失能低功耗模式避免DW1000在清除中断后立即进入睡眠模式 */
    DW1000_LowPowerListeningSet(handle, DW1000_LOW_POWER_LISTENING_MODE_DISABLE);
    /* 清除所有接收标志位（通过往SYS_STATUS对应位写1即可清除） */
    DW1000_WRITE_REG(data, DW1000_SYS_STATUS_ALL_RX_GOOD_MSK);
    if (result = _DW1000_SPI_Write(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data))) {
        return result;
    }
    pdw1000local->cbData.rx_flags = 0;
    uint16_t frameInfo;
    uint16_t frameLen;
    uint16_t frameCtrl;
    /* 读取帧信息（只需要最前面的两个字节的数据） */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_RX_FINFO, 0, (uint8_t*) &frameInfo, sizeof(frameInfo))) {
        return result;
    }
    /* 获取帧长度信息 */
    frameLen = DW1000_READ_BITS(frameInfo, DW1000_RX_FINFO_RXFL_1023_MSK);
    pdw1000local->cbData.datalength = frameLen;
    /* 判断是否为信标帧 */
    if(DW1000_READ_BITS(frameInfo, DW1000_REG_RX_FINFO_BIT_RNG_MSK)){
        pdw1000local->cbData.rx_flags |= DWT_FRAME_TYPE_BEACON;
    }
    /* 读取帧控制信息 */
    if (result = _DW1000_SPI_Read(handle, DW1000_REG_RX_BUFFER, 0, (uint8_t*) &frameCtrl, sizeof(frameCtrl))) {
        return result;
    }
    pdw1000local->cbData.fctrl = frameCtrl;
    if (DW1000_READ_BITS(systemStatus, DW1000_REG_SYS_STATUS_BIT_AAT) && ((pdw1000local->cbData.fctrl[0] & FCTRL_ACK_REQ_MASK) == 0)) {
        /* 清除AAT标志位 */
        DW1000_WRITE_REG(data, DW1000_REG_SYS_STATUS_BIT_AAT);
        if (result = _DW1000_SPI_Read(handle, DW1000_REG_SYS_STATUS, 0, (uint8_t*) &data, sizeof(data))) {
            return result;
        }
        pdw1000local->cbData.status &= ~SYS_STATUS_AAT;
        pdw1000local->wait4resp = 0;
    }
    /* 调用相应的回调函数（如果存在） */
    if (pdw1000local->cbRxOk != NULL) {
        pdw1000local->cbRxOk(&pdw1000local->cbData);
    }
    return 0;
}

