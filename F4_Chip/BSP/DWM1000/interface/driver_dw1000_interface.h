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

uint8_t dw1000_interface_gpio_wakeup_init(void);

uint8_t dw1000_interface_gpio_wakeup_deinit(void);

uint8_t dw1000_interface_gpio_wakeup_read(uint8_t* data);

uint8_t dw1000_interface_gpio_wakeup_write(uint8_t data);

uint8_t dw1000_interface_gpio_reset_init(void);

uint8_t dw1000_interface_gpio_reset_deinit(void);

uint8_t dw1000_interface_gpio_reset_read(uint8_t* data);

uint8_t dw1000_interface_gpio_reset_write(uint8_t data);

void dw1000_interface_irq_enable(void);

void dw1000_interface_irq_disable(void);

void dw1000_initerface_delay_ms(uint32_t ms);

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