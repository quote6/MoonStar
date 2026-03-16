#ifndef __DRIVER_DW1000_BASIC_H
#define __DRIVER_DWD100_BASIC_H

#include "driver_dw1000_interface.h"

#ifdef __cplusplus
extern "C"{
#endif
/**
 * @defgroup dw1000_example_driver dw1000 example driver function
 * @brief    dw1000 example driver modules
 * @ingroup  dw1000_driver
 * @{
 */

uint8_t dw1000_basic_init(void);

uint8_t dw1000_basic_deinit(void);

uint8_t dw1000_basic_send(uint8_t* buff, uint8_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif