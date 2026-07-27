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
 */
uint8_t dw1000_interface_spi_read(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen) {
    /* 通过SPI发送头部数据 */
    /* 通过SPI读取bufLen个数据并存入buf地址 */
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
 */
uint8_t dw1000_interface_spi_write(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen) {
    /* 通过SPI发送头部数据 */
    /* 通过SPI发送从buf地址处开始的bufLen个数据 */
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
 * @brief 接口SPI总线 nss引脚初始化
 * @note  无
 */
void dw1000_interface_spi_nss_init(void){
    
}

/**
 * @brief 接口SPI总线 nss引脚反初始化
 * @note  无
 */
void dw1000_interface_spi_nss_deinit(void){

}

/**
 * @brief 接口SPI总线 nss引脚读取电平
 * @param[in] data  读取到的值存放的位置
 * @return uint8_t 执行结果
 *  @arg 0: 读取成功
 *  @arg 1: 读取失败
 * @note  无
 */
uint8_t dw1000_interface_spi_nss_read(uint8_t* data){
    return 0;
}

/**
 * @brief 接口SPI总线 nss引脚写入电平
 * @param[in] data  待设置的电平值
 * @return uint8_t 执行结果
 *  @arg 0: 设置成功
 *  @arg 1: 设置失败
 * @note  无
 */
uint8_t dw1000_interface_spi_nss_write(uint8_t data){
    return 0;
}

/**
 * @brief 接口GPIO wakeup引脚初始化
 * @note  备注
 */
void dw1000_interface_gpio_wakeup_init(void) {
    
}

/**
 * @brief 接口GPIO wakeup引脚反初始化
 * @note  备注
 */
void dw1000_interface_gpio_wakeup_deinit(void) {
    
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
 * @brief 接口GPIO RSTn引脚初始化
 * @param[in] mode  初始化模式
 *  @arg 0: 开漏模式，禁用对应的外部中断
 *  @arg 1: 配置为外部中断，并使能对应的外部中断
 * @note  备注
 */
void dw1000_interface_gpio_rst_init(uint8_t mode) {
    
}

/**
 * @brief 接口GPIO RSTn引脚反初始化
 * @note  备注
 */
void dw1000_interface_gpio_rst_deinit(void) {
    
}

/**
 * @brief 接口GPIO wakeup引脚读取
 * @return uint8_t 执行结果
 *  @arg 0: 读取成功
 *  @arg 1: 读取失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_rst_read(uint8_t* data) {
    return 0;
}

/**
 * @brief 接口GPIO wakeup引脚写入
 * @return uint8_t 执行结果
 *  @arg 0: 读取成功
 *  @arg 1: 写入失败
 * @note  备注
 */
uint8_t dw1000_interface_gpio_rst_write(uint8_t data) {
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

/**
 * @brief 接口 获取计数值（用于实现延时功能）
 * @return uint32_t 返回的计数值
 * @note  要 USE_DW1000_REGISTER_GET_TICK 宏定义设置为1启用注册获取计数函数功能
 * @attention  需要借助硬件实现一个计数，计数过程不能受代码运行的影响
 */
uint32_t dw1000_interface_get_tick(void){
    return 0;
}

/**
 * @brief 接口打印格式化数据
 * @param[in] fmt  格式化数据
 * @note  要 USE_DW1000_REGISTER_DEBUG_PRINT 宏定义设置为1启用注册Debug打印功能
 */
void dw1000_interface_debug_print(const char* const fmt, ...) {
}

/**
 * @brief 接口 发送完成回调函数
 * @param[in] handle  DW1000句柄
 * @note  要 USE_DW1000_REGISTER_CALLBACKS 宏定义设置为1启用注册回调函数
 */
void dw1000_interface_tx_cplt_callback(DW1000_Handle_t* handle) {
}

/**
 * @brief 接口 接收完成回调
 * @param[in] handle  DW1000句柄
 * @note  要 USE_DW1000_REGISTER_CALLBACKS 宏定义设置为1启用注册回调函数
 */
void dw1000_interface_rx_cplt_callback(DW1000_Handle_t* handle) {
}

/**
 * @brief 接口 接收错误回调
 * @param[in] handle  DW1000句柄
 * @note  要 USE_DW1000_REGISTER_CALLBACKS 宏定义设置为1启用注册回调函数
 */
void dw1000_interface_rx_err_callback(DW1000_Handle_t* handle) {
}

/**
 * @brief 接口 接收超时回调
 * @param[in] handle  DW1000句柄
 * @note  要 USE_DW1000_REGISTER_CALLBACKS 宏定义设置为1启用注册回调函数
 */
void dw1000_interface_rx_timeout_callback(DW1000_Handle_t* handle) {
}