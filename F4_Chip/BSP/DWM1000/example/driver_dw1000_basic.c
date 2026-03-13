#include "driver_dw1000_basic.h"

/* DW1000句柄 */
static DW1000_Handle_t gs_handle;

uint8_t dw1000_basic_init(void) {
    /* 链接函数 */
    DRIVER_DW1000_LINK_INIT(&gs_handle, DW1000_Handle_t);
    // DRIVER_DW1000_LINK_SPI_INIT(&gs_handle, dw1000_interface_spi_init); // 暂未启用该字段
    // DRIVER_DW1000_LINK_SPI_DEINIT(&gs_handle, dw1000_interface_spi_deinit); // 暂未启用该字段
    DRIVER_DW1000_LINK_SPI_READ(&gs_handle, dw1000_interface_spi_read);
    DRIVER_DW1000_LINK_SPI_WRITE(&gs_handle, dw1000_interface_spi_write);
    DRIVER_DW1000_LINK_SPI_HIGH_SPEED_SET(&gs_handle, dw1000_interface_spi_high_speed_set);
    DRIVER_DW1000_LINK_SPI_LOW_SPEED_SET(&gs_handle, dw1000_interface_spi_low_speed_set);
    // DRIVER_DW1000_LINK_GPIO_INIT(&gs_handle, wakeup, dw1000_interface_gpio_wakeup_init);     // 暂未启用该字段
    // DRIVER_DW1000_LINK_GPIO_DEINIT(&gs_handle, wakeup, dw1000_interface_gpio_wakeup_deinit); // 暂未启用该字段
    DRIVER_DW1000_LINK_GPIO_READ(&gs_handle, wakeup, dw1000_interface_gpio_wakeup_read);
    DRIVER_DW1000_LINK_GPIO_WRITE(&gs_handle, wakeup, dw1000_interface_gpio_wakeup_write);
    // DRIVER_DW1000_LINK_GPIO_INIT(&gs_handle, reset, dw1000_interface_gpio_reset_init);     // 暂未启用该字段
    // DRIVER_DW1000_LINK_GPIO_DEINIT(&gs_handle, reset, dw1000_interface_gpio_reset_deinit); // 暂未启用该字段
    DRIVER_DW1000_LINK_GPIO_READ(&gs_handle, reset, dw1000_interface_gpio_reset_read);
    DRIVER_DW1000_LINK_GPIO_WRITE(&gs_handle, reset, dw1000_interface_gpio_reset_write);
    DRIVER_DW1000_LINK_IRQ_ENABLE(&gs_handle,dw1000_interface_irq_enable);
    DRIVER_DW1000_LINK_IRQ_DISABLE(&gs_handle, dw1000_interface_irq_disable);
    DRIVER_DW1000_LINK_DELAY_MS(&gs_handle, dw1000_initerface_delay_ms);
    DRIVER_DW1000_LINK_DEBUG_PRINT(&gs_handle, dw1000_interface_debug_print);

    /* 准备阶段 */
    
}
