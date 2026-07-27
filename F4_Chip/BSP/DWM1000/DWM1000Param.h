#ifndef __DWM1000_PARAM_H
#define __DWM1000_PARAM_H

#include "DWM1000.h"

void DWM1000_ParamDeInit(void);
uint8_t DWM1000_ParamInit(uint8_t templateNum, uint32_t delayTime, uint16_t timeout);

#endif