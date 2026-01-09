/**
 * ************************************************************************
 * 
 * @file WirelessParam.h
 * @author quote6 (2293127401@qq.com)
 * @brief 在该文件中提供无线通信所需的参数配置
 * @attention 如果需要修改无线通信对象个数，请在 Wireless.h 文件的
 *            WirelessObj_t 类型中增减枚举元素个数
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2025 quote6 
 * For study and research only, no reprinting
 * ************************************************************************
 */
#ifndef __WIRELESS_PARAM_H
#define __WIRELESS_PARAM_H

#include "Wireless.h"


/* 供外部调用的函数 */
void WirelessObjInfoTableDeInit(void);
void WirelessObjInfoTableConfig(uint8_t slaveNum);
void WirelessTxQueueDeInit(void);
void WirelessRxQueueDeInit(void);


#endif