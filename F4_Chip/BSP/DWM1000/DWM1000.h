#ifndef __DWM1000_H
#define __DWM1000_H

#include <stdint.h>
#include "deca_device_api.h"
#include "deca_regs.h"

/* 定义 DWM1000 发送缓冲区长度 */
#define DWM1000_TX_BUFF_LENGTH 1024
/* 定义 DWM1000 接收缓冲区长度 */
#define DWM1000_RX_BUFF_LENGTH 1024

/* 定义DWM1000等待接收数据包计数最大值（如果达到最大值通常说明DWM1000工作状态有问题） */
#define DWM1000_WAIT_RX_COUNT_MAX      500000


/* DWM1000配置信息结构体类型定义 */
typedef struct DWM1000_Config_s
{
    dwt_config_t* dwtConfiguration;
    uint32_t rxDelayTime;
    uint16_t rxRespTimeout;
}DWM1000_Config_t;

typedef enum DWM1000WorkState_e {
    DWM1000_WORK_STATE_WAIT_PACK = 0x00, // 等待接收数据包状态
    DWM1000_WORK_STATE_SEND_PACK,        // 发送数据包状态
    DWM1000_WORK_STATE_HANDLE_PACK,      // 处理接收到的数据包状态
    DWM1000_WORK_STATE_RESEND_PACK,      // 重发上次的数据包状态
    DWM1000_WORK_STATE_NUM               // 状态个数
} DWM1000WorkState_t;

typedef enum DWM1000_GPIO_e
{
    DWM1000_GPIO_0 = (0x01 << 0),
    DWM1000_GPIO_1 = (0x01 << 1),
    DWM1000_GPIO_2 = (0x01 << 2),
    DWM1000_GPIO_3 = (0x01 << 3),
    DWM1000_GPIO_4 = (0x01 << 4),
    DWM1000_GPIO_5 = (0x01 << 5),
    DWM1000_GPIO_6 = (0x01 << 6),
    DWM1000_GPIO_7 = (0x01 << 7),
}DWM1000_GPIO_t;

typedef enum DWM1000_GPIO_Mode_e{
    DWM1000_GPIO_OUTPUT = 0x00,
    DWM1000_GPIO_INPUT = 0x01,
}DWM1000_GPIO_Mode_t;

typedef enum DWM1000_GPIO_Level_e
{
    DWM1000_GPIO_LEVEL_RESET = 0x00,
    DWM1000_GPIO_LEVEL_SET = 0x01,
}DWM1000_GPIO_Level_t;


/* 供外部使用的变量 */
// extern DWM1000_Config_t* DWM1000_ConfigurationPtr; // DWM1000配置指针
extern DWM1000WorkState_t DWM1000_WorkState;       // DWM1000 工作状态
extern uint32_t DWM1000_WaitRxCount;               // DWM1000等待接收计数
extern uint8_t DWM1000_TxBuffer[];                 // DWM1000 发送缓冲区
extern uint8_t DWM1000_RxBuffer[];                 // DWM1000 接收缓冲区

/* 供外部调用的函数 */
uint8_t DWM1000_ConfigurationPtrInit(DWM1000_Config_t* config);
uint8_t DWM1000_Init(void);
uint8_t DWM1000_SendFrame(const uint8_t* const frame, const uint16_t length);
#define DWM1000_RxEnable()                       dwt_rxenable(DWT_START_RX_IMMEDIATE)
#define DWM1000_ReadRxBuff(buff, length, offset) dwt_readrxdata(buff, length, offset)
/* DWM1000 GPIO 相关操作函数 */
void DWM1000_GPIO_Init(DWM1000_GPIO_t GPIOx, DWM1000_GPIO_Mode_t mode);
void DWM1000_GPIO_SetBits(DWM1000_GPIO_t GPIOx);
void DWM1000_GPIO_ResetBits(DWM1000_GPIO_t GPIOx);
void DWM1000_GPIO_ToggleBit(DWM1000_GPIO_t GPIOx);
void DWM1000_GPIO_Write(DWM1000_GPIO_t GPIOx, DWM1000_GPIO_Level_t value);
DWM1000_GPIO_Level_t DWM1000_GPIO_ReadBit(DWM1000_GPIO_t GPIOx);


/* DWM1000获取状态函数 */
#define DWM1000_GetSelfState()  dwt_read32bitreg(SYS_STATE_ID)
/* 判断DWM1000是否处于空闲状态，0x00010000来源于用户手册里的SYS_STATE寄存器，具体含义自行查询 */
#define IS_DWM1000_IN_IDLE      (DWM1000_GetSelfState() == 0x00010000)

/* 需由外部实现的回调函数 */
void DWM1000_Callback_TxConf(const dwt_cb_data_t* cb_data);    // DWM1000发送完成回调函数
void DWM1000_Callback_RxOK(const dwt_cb_data_t* cb_data);      // DWM1000接收完成回调函数
void DWM1000_Callback_RxTimeout(const dwt_cb_data_t* cb_data); // DWM1000接收超时回调函数
void DWM1000_Callback_RxError(const dwt_cb_data_t* cb_data);   // DWM1000接收错误回调函数



#endif
