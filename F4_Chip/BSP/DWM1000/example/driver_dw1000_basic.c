#include "driver_dw1000_basic.h"

/* DW1000句柄 */
static DW1000_Handle_t gs_handle;

uint8_t dw1000_basic_init(void) {
    /* 链接函数 */
    DRIVER_DW1000_LINK_INIT(&gs_handle, DW1000_Handle_t);
    // DRIVER_DW1000_LINK_SPI_INIT(&gs_handle, dw1000_interface_spi_init);
    // DRIVER_DW1000_LINK_SPI_DEINIT(&gs_handle, dw1000_interface_spi_deinit);
    DRIVER_DW1000_LINK_SPI_HIGH_SPEED_SET(&gs_handle, dw1000_interface_spi_high_speed_set);
    DRIVER_DW1000_LINK_SPI_LOW_SPEED_SET(&gs_handle, dw1000_interface_spi_low_speed_set);
    DRIVER_DW1000_LINK_SPI_READ(&gs_handle, dw1000_interface_spi_read);
    DRIVER_DW1000_LINK_SPI_WRITE(&gs_handle, dw1000_interface_spi_write);
    // DRIVER_DW1000_LINK_SPI_NSS_INIT(&gs_handle, dw1000_interface_spi_nss_init);
    // DRIVER_DW1000_LINK_SPI_NSS_DEINIT(&gs_handle, dw1000_interface_spi_nss_deinit);
    DRIVER_DW1000_LINK_SPI_NSS_READ(&gs_handle, dw1000_interface_spi_nss_read);
    DRIVER_DW1000_LINK_SPI_NSS_WRITE(&gs_handle, dw1000_interface_spi_nss_write);
    // DRIVER_DW1000_LINK_GPIO_WAKEUP_INIT(&gs_handle, dw1000_interface_gpio_wakeup_init);
    // DRIVER_DW1000_LINK_GPIO_WAKEUP_DEINIT(&gs_handle, dw1000_interface_gpio_wakeup_deinit);
    DRIVER_DW1000_LINK_GPIO_WAKEUP_READ(&gs_handle, dw1000_interface_gpio_wakeup_read);
    DRIVER_DW1000_LINK_GPIO_WAKEUP_WRITE(&gs_handle, dw1000_interface_gpio_wakeup_write);
    DRIVER_DW1000_LINK_GPIO_RST_INIT(&gs_handle, dw1000_interface_gpio_rst_init);
    // DRIVER_DW1000_LINK_GPIO_RST_DEINIT(&gs_handle, dw1000_interface_gpio_rst_deinit);
    DRIVER_DW1000_LINK_GPIO_RST_READ(&gs_handle, dw1000_interface_gpio_rst_read);
    DRIVER_DW1000_LINK_GPIO_RST_WRITE(&gs_handle, dw1000_interface_gpio_rst_write);
    DRIVER_DW1000_LINK_IRQ_ENABLE(&gs_handle, dw1000_interface_irq_enable);
    DRIVER_DW1000_LINK_IRQ_DISABLE(&gs_handle, dw1000_interface_irq_disable);
    // DRIVER_DW1000_LINK_GET_TICK(&gs_handle, dw1000_interface_get_tick);
    // DRIVER_DW1000_LINK_DEBUG_PRINT(&gs_handle, dw1000_interface_debug_print);
    // DRIVER_DW1000_LINK_TX_CPLT_CALLBACK(&gs_handle, dw1000_interface_tx_cplt_callback);
    // DRIVER_DW1000_LINK_RX_CPLT_CALLBACK(&gs_handle, dw1000_interface_rx_cplt_callback);
    // DRIVER_DW1000_LINK_RX_ERR_CALLBACK(&gs_handle, dw1000_interface_rx_err_callback);
    // DRIVER_DW1000_LINK_RX_TO_CALLBACK(&gs_handle, dw1000_interface_rx_timeout_callback);
    /* 硬复位DW1000 */
    DW1000_HardReset(&gs_handle);
    /* 准备阶段 */
    DW1000_Prepare(&gs_handle);
    /* 设置相关参数用于初始化 */
    gs_handle.init.channel = DW1000_CHANNEL_2;
    gs_handle.init.prf = DW1000_PRF_64M;
    gs_handle.init.rxPac = DW1000_PAC8;
    gs_handle.init.txPreambleCode = 10;
    gs_handle.init.rxPreambleCode = 10,
    gs_handle.init.useSfdNstd = 1;
    gs_handle.init.dataRate = DW1000_DATA_RATE_6M8;
    gs_handle.init.phrType = DW1000_PHR_TYPE_EXT;
    gs_handle.init.sfdTimeout = 65;
    gs_handle.init.rxDelayTime = 0; // 在发送完成后尽可能快得开启接收
    gs_handle.init.rxTimeout = 0;   // 禁用接收超时
    /* 初始化 */
    DW1000_Init(&gs_handle);
    /* 使能所需中断 */
    DW1000_InterruptEnable(
        &gs_handle,
        // DW1000_INT_EVENT_FRAME_SENT |        // 发送完成事件/中断
        // DW1000_INT_EVENT_FRAME_REJECTED |  // 帧拒绝中断（配置了帧过滤功能时）
        // DW1000_INT_EVENT_RX_TIMEOUT |    // 接收器等待帧超时中断
        DW1000_INT_EVENT_FRAME_RECIVED |        // 接收器接收帧成功中断
            DW1000_INT_EVENT_RX_PHR_ERROR |     // 接收器帧PHY包头错误中断
            DW1000_INT_EVENT_RX_CRC_ERROR |     // 接收器帧错误（CRC校验失败）
            DW1000_INT_EVENT_RX_SYNC_LOST |     // 接收器帧同步丢失中断
            DW1000_INT_EVENT_PREAMBLE_TIMEOUT | // 前导码检测超时
            DW1000_INT_EVENT_SFD_TIMEOUT        // SFD超时中断
    );
    /* 设置接收超时时间 */
    DW1000_RxTimeoutSet(&gs_handle, gs_handle.init.rxTimeout);
    /* 立即开启接收模式 */
    DW1000_RxEnable(&gs_handle, DW1000_RX_MODE_IMMEDIATE);
    /* 使能DW1000对应的中断 */
    gs_handle.irq.enable();
    return 0;
}

inline uint8_t dw1000_basic_deinit(void){
    return DW1000_DeInit(&gs_handle);
}

uint8_t dw1000_basic_send(uint8_t* buff, uint8_t len){
    DW1000_TX_DataWrite(&gs_handle, buff, len, 0);
    DW1000_TX_FrameCtrlWrite(&gs_handle, len + 2, 0, 0); // 实际发送长度还需要加上2个CRC字节
    /* 立即开启发送，并在发送结束后切换为接收状态 */
    DW1000_TxStart(&gs_handle, DW1000_TX_MODE_IMMEDIATE | DW1000_TX_MODE_EXPECT_RESPONSE);
}
