#include "driver_dw1000_interface.h"

/**
 * @brief 接口 SPI 总线初始化
 * @return uint8_t 执行结果
 *  @arg 0: 成功
 *  @arg 1: 初始化失败
 * @note  备注
 */
uint8_t dw1000_interface_spi_init(void) {
    return 0;
}

/**
 * @brief 接口 SPI 总线反初始化
 * @return uint8_t 执行结果
 *  @arg 0: 成功
 *  @arg 1: 反初始化失败
 * @note  备注
 */
uint8_t dw1000_interface_spi_deinit(void) {
    return 0;
}

/**
 * @brief 接口SPI总线读
 * @param[in] hdr  头部数据指针
 * @param[in] hdrLen  头部数组长度
 * @param[in] buf  有效数据指针
 * @param[in] bufLen  有效数据长度
 * @return uint8_t 执行结果
 *  @arg 0: 成功
 *  @arg 1: 读失败
 * @note  该函数完成两件事，先拉低SPI片选线开始通信，然后将头部数据发送出去，
 *        在此过迟中保持SPI片选线拉低，然后开始SPI读取操作，按参数读取指定
 *        长度的数据
 * @attention 考虑到在SPI操作过程中可能会出现错误，需要进行错误处理，通常的
 *             错误处理为将SPI片选线拉高，然后退出该函数
 */
uint8_t dw1000_interface_spi_read(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen) {
    /* 先拉低SPI片选线 */
    /* 通过SPI发送头部数据 */
    /* 通过SPI读取bufLen个数据并存入buf地址 */
    /* 拉高SPI片选线 */
    return 0;
}

/**
 * @brief 接口SPI总线读
 * @param[in] hdr  头部数据指针
 * @param[in] hdrLen  头部数组长度
 * @param[in] buf  有效数据指针
 * @param[in] bufLen  有效数据长度
 * @return uint8_t 执行结果
 *  @arg 0: 成功
 *  @arg 2: 写失败
 * @note  该函数完成两件事，先拉低SPI片选线开始通信，然后将头部数据发送出去，
 *        在此过迟中保持SPI片选线拉低，然后开始SPI写入操作，按参数读取指定
 *        长度的数据
 * @attention 考虑到在SPI操作过程中可能会出现错误，需要进行错误处理，通常的
 *             错误处理为将SPI片选线拉高，然后退出该函数
 */
uint8_t dw1000_interface_spi_write(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen) {
    /* 先拉低SPI片选线 */
    /* 通过SPI发送头部数据 */
    /* 通过SPI发送从buf地址处开始的bufLen个数据 */
    /* 拉高SPI片选线 */
    return 0;
}

/**
 * @brief 接口SPI总线设置高速
 * @return uint8_t 执行结果
 *  @arg 0: 设置成功
 *  @arg 1: 设置失败
 * @note 最高速率不能超过20MHz
 */
uint8_t dw1000_interface_spi_high_speed_set(void){
    return 0;
}

/**
 * @brief 接口SPI总线设置低速
 * @return uint8_t 执行结果
 *  @arg 0: 设置成功
 *  @arg 1: 设置失败
 * @note  最高速率不能超过3MHz
 */
uint8_t dw1000_interface_spi_low_speed_set(void){
    return 0;
}

/**
 * @brief 接口GPIO wakeup引脚初始化
 * @return uint8_t 执行结果
 *  @arg 0: 成功
 *  @arg 1: 初始化失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_wakeup_init(void) {
    return 0;
}

/**
 * @brief 接口GPIO wakeup引脚反初始化
 * @return uint8_t 执行结果
 *  @arg 0: 成功
 *  @arg 1: 反初始化失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_wakeup_deinit(void) {
    return 0;
}

/**
 * @brief 接口GPIO wakeup引脚读取
 * @return uint8_t 执行结果
 *  @arg 0: 读取成功
 *  @arg 1: 读取失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_wakeup_read(uint8_t* data) {
    return 0;
}

/**
 * @brief 接口GPIO wakeup引脚写入
 * @return uint8_t 执行结果
 *  @arg 0: 读取成功
 *  @arg 1: 写入失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_wakeup_write(uint8_t data) {
    return 0;
}
/**
 * @brief 接口GPIO wakeup引脚初始化
 * @return uint8_t 执行结果
 *  @arg 0: 成功
 *  @arg 1: 初始化失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_reset_init(void) {
    return 0;
}

/**
 * @brief 接口GPIO wakeup引脚反初始化
 * @return uint8_t 执行结果
 *  @arg 0: 成功
 *  @arg 1: 反初始化失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_reset_deinit(void) {
    return 0;
}

/**
 * @brief 接口GPIO wakeup引脚读取
 * @return uint8_t 执行结果
 *  @arg 0: 读取成功
 *  @arg 1: 读取失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_reset_read(uint8_t* data) {
    return 0;
}

/**
 * @brief 接口GPIO wakeup引脚写入
 * @return uint8_t 执行结果
 *  @arg 0: 读取成功
 *  @arg 1: 写入失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_reset_write(uint8_t data) {
    return 0;
}


/**
 * @brief 接口irq 使能中断（DW1000的IRQ引脚对应的主机外部中断）
 * @note  备注
 */
void dw1000_interface_irq_enable(void) {
    
}

/**
 * @brief 接口irq 禁用中断（DW1000的IRQ引脚对应的主机外部中断）
 * @note  备注
 */
void dw1000_interface_irq_disable(void) {
    
}

void dw1000_initerface_delay_ms(uint32_t ms) {
    
}

/**
 * @brief 接口打印格式化数据
 * @param[in] fmt  格式化数据
 * @note  备注
 */
void dw1000_interface_debug_print(const char* const fmt, ...) {
}

/**
 * @brief 接口 发送完成回调函数
 * @param[in] handle  DW1000句柄
 * @note  备注
 */
void dw1000_interface_tx_cplt_callback(DW1000_Handle_t* handle) {
}

/**
 * @brief 接口 接收完成回调
 * @param[in] handle  DW1000句柄
 * @note  备注
 */
void dw1000_interface_rx_cplt_callback(DW1000_Handle_t* handle) {
}

/**
 * @brief 接口 接收错误回调
 * @param[in] handle  DW1000句柄
 * @note  备注
 */
void dw1000_interface_rx_err_callback(DW1000_Handle_t* handle) {
}

/**
 * @brief 接口 接收超时回调
 * @param[in] handle  DW1000句柄
 * @note  备注
 */
void dw1000_interface_rx_timeout_callback(DW1000_Handle_t* handle) {
}