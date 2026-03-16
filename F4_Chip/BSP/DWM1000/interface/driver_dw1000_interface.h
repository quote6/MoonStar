#ifndef __DRIVER_DW1000_INTERFACE_H
#define __DRIVER_DW1000_INTERFACE_H

#include "drv_dw1000.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup dw1000_interface_driver dw1000 interface driver function
 * @brief    dw1000 interface driver modules
 * @ingroup  dw1000_driver
 * @{
 */


uint8_t dw1000_interface_spi_init(void);

uint8_t dw1000_interface_spi_deinit(void);

uint8_t dw1000_interface_spi_read(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen);

uint8_t dw1000_interface_spi_write(uint8_t* hdr, uint8_t hdrLen, uint8_t* buf, uint16_t bufLen);

uint8_t dw1000_interface_spi_high_speed_set(void);

uint8_t dw1000_interface_spi_low_speed_set(void);

void dw1000_interface_spi_nss_init(void);

void dw1000_interface_spi_nss_deinit(void);

uint8_t dw1000_interface_spi_nss_read(uint8_t* data);

uint8_t dw1000_interface_spi_nss_write(uint8_t data);

void dw1000_interface_gpio_wakeup_init(void);

void dw1000_interface_gpio_wakeup_deinit(void);

uint8_t dw1000_interface_gpio_wakeup_read(uint8_t* data);

uint8_t dw1000_interface_gpio_wakeup_write(uint8_t data);

void dw1000_interface_gpio_rst_init(uint8_t mode);

void dw1000_interface_gpio_rst_deinit(void);

uint8_t dw1000_interface_gpio_rst_read(uint8_t* data);

uint8_t dw1000_interface_gpio_rst_write(uint8_t data);

void dw1000_interface_irq_enable(void);

void dw1000_interface_irq_disable(void);

uint32_t dw1000_interface_get_tick(void);

void dw1000_interface_debug_print(const char* const fmt, ...);

void dw1000_interface_tx_cplt_callback(DW1000_Handle_t* handle);

void dw1000_interface_rx_cplt_callback(DW1000_Handle_t* handle);

void dw1000_interface_rx_err_callback(DW1000_Handle_t* handle);

void dw1000_interface_rx_timeout_callback(DW1000_Handle_t* handle);





/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif