/**
 * ************************************************************************
 * 
 * @file Wireless.h
 * @author quote6 (2293127401@qq.com)
 * @brief 无线通信协议相关定义都在此处 
 * @todo 以后有机会的话，将这些静态变量修改为动态内存分配
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2025 quote6 
 * For study and research only, no reprinting
 * ************************************************************************
 */
#ifndef __WIRELESS_H
#define __WIRELESS_H

#include <stdint.h>
#include "QueueRM.h"

/**
 * @brief 无线通信状态结构体类型
 * @param hasRcvdValidPack : 接收到有效数据包
 * @param hasRcvdRepetPack : 接收到重复数据包，这通常是对方没有收到上一次数据包产生的重发情况
 * @param hasRcvdInterferencePack : 接收到错误数据包。该数据包的源地址并不在当前记录的信息表里
 * @param hasLostData : 数据丢失，对方的应答号与本机进行校验对不上
 */
typedef struct WirelessStatus_s
{
    uint16_t hasRcvdValidAck : 1;   // 接收到有效确认信息
    uint16_t hasRcvdWrongAck : 1;   // 接收到错误确认信息
    uint16_t hasRcvdValidPack : 1;  // 接收到有效数据包ID
    uint16_t hasRcvdWrongPack : 1;  // 接收到错误数据包ID
    uint16_t hasRcvdRepetPack : 1;  // 接收到重复数据包ID
    uint16_t hasRcvdErrorPack : 1;  // 接收到错误数据包
    uint16_t hasNoRcvdResponse : 1; // 上次发送未收到应答
    uint16_t hasLostData : 1;       // 发生数据丢失
    uint16_t isReseted : 1;         // 已复位
    uint16_t reserved : 7;          // 保留
} WirelessStatus_t;

/* 定义无线状态共用体类型（方便整体赋值） */
typedef union WirelessStatus_u
{
    WirelessStatus_t status;
    uint16_t statuss; // 末尾多一个s用于跟上面的变量区分
}WirelessStatus_ut;

// /* 定义无线状态枚举类型 */
// typedef enum WirelessStatus_e{
//     WIRELESS_STATUS_NORMAL = 0x0000,
//     WIRELESS_STATUS_HAS_RCVD_VAILD_ACK  =   (0x0001 << 0),
//     WIRELESS_STATUS_HAS_RCVD_WRONG_ACK  =   (0x0001 << 1),
//     WIRELESS_STATUS_HAS_RCVD_VAILD_PACK =   (0x0001 << 2),
//     WIRELESS_STATUS_HAS_RCVD_WRONG_PACK =   (0x0001 << 3),
//     WIRELESS_STATUS_HAS_RCVD_REPET_PACK =   (0x0001 << 4),
//     WIRELESS_STATUS_HAS_RCVD_ERROR_PACK =   (0x0001 << 5),
//     WIRELESS_STATUS_HAS_LOST_PACK =         (0x0001 << 6),
//     WIRELESS_STATUS_IS_RESETED =            (0x0001 << 7),
// }WirelessStatus_et;

typedef struct WirelessControl_s
{
    uint16_t resetCommand : 1; // 复位命令
    uint16_t reserved : 15;    // 保留
}WirelessControl_t;

typedef union WirelessControl_u
{
    WirelessControl_t control;
    uint16_t controll; // 末尾多一个l用于跟上面的变量区分
}WirelessControl_ut;


// /* 定义无线控制枚举类型 */
// typedef enum WirelessControl_e{
//     WIRELESS_CONTROL_NONE = 0x0000,
//     WIRELESS_CONTROL_RESET_COMMAND = (0x0001 << 0),
// }WirelessControl_et;


/**
 * @brief 无线对象枚举编号
 * @note  无线对象的含义就类似寄快递
 *        有几个人需要寄快递就有几个对象
 */
typedef enum WirelessObj_e{
    WIRELESS_OBJ_0 = 0x00,
    // WIRELESS_OBJ_1 = 0x01,
    // WIRELESS_OBJ_2 = 0x02,
    // WIRELESS_OBJ_3 = 0x03,
    WIRELESS_OBJ_NUM,// 无线对象个数
}WirelessObj_t;


#pragma pack (1) /*指定下面的结构体按1字节对齐*/
/**
 * @brief 无线对象信息结构体类型
 * @note  无线对象信息还是以寄快递为例
 *        源地址就是寄件人的地址，目的地址就是收件人的地址。
 *        因为是无线的底层是广播通信，所以对于无线来说是没有
 *        必要给无线通信底层定义地址，我们只对数据包定义地址
 *        对方看到是期望的包裹就收取，并返回给对方一个包裹
 *        否则就不接收该包裹
 */
typedef struct WirelessObjInfo_s
{
    uint8_t sourceAddress; // 无线通信的源地址
    uint8_t targetAddress; // 无线通信的目标地址
}WirelessObjInfo_t;
#pragma pack () /*取消指定对齐，恢复缺省对齐*/

/**
 * @brief 无线通信信息结构体类型
 * @param sourceAddress : 无线通信数据的源地址；每一个无线数据包来源都分配一个固定的地址（用于确定通信数据来源）
 * @param targetAddress : 无线通信数据的目标地址；每一个无线数据包来源都分配一个固定的地址（用于确定通信数据目标）
 * @param status ： 当前本机无线通信状态；用于告知对方当前本机状态，方便对方根据状态采取不同的控制指示
 * @param control ： 对通信对象的控制指示；一般是根据接收的通信对象状态采取相应的控制指示，如复位信息表
 * @param packID : 无线通信数据包ID；用于判断是否为重发数据包，ID具有唯一性。ID类似tcp里的ID，也就是指针，指示当前本机给目标对象发送的累积数据长度
 * @param ackID : 无线通信数据包应答号；供对方判断是否需要重发数据包；也类似tcp里的应答号，指示当前本机从目标对象处接收的累计数据长度
 * @param legnth : 无线通信数据包长度字段；有效载荷长度 + 信息字段长度（WIRELESS_PACK_INFO_LEGNTH）
 */
typedef struct WirelessInfo_s
{
    WirelessObjInfo_t objInfo;  // 无线通信对象信息（包含源地址和目标地址）
    WirelessStatus_ut status;   // 向对方反馈的通信状态信息
    WirelessControl_ut control; // 向对方提供控制字段（根据接收的状态字段来设置对应的控制）
    uint16_t packID;            // 无线通信的ID
    uint16_t ackID;             // 应答号
    uint16_t length;            // 有效载荷长度 + 信息字段长度（WIRELESS_PACK_INFO_LEGNTH）
}WirelessInfo_t;

/* 定义无线数据包信息字段长度 */
#define WIRELESS_PACK_INFO_LEGNTH (sizeof(WirelessInfo_t))
/* 定义无线数据包有效载荷最大长度 */
#define WIRELESS_PACK_PAYLOAD_MAX_LENGTH 1000

/* 定义无线数据包结构体类型 */
typedef struct WirelessPack_s
{
    WirelessInfo_t info;                               // 无线数据包的相关信息
    uint8_t payload[WIRELESS_PACK_PAYLOAD_MAX_LENGTH]; // 载荷数据
}WirelessPack_t;

/* 定义无线数据包的最大长度 */
#define WIRELESS_PACK_MAX_LENGTH (sizeof(WirelessPack_t))
// #define WIRELESS_PACK_MAX_LENGTH (WIRELESS_PACK_INFO_LEGNTH + WIRELESS_PACK_PAYLOAD_MAX_LENGTH)

/* 定义无线数据包公用体类型（方便数据包装包与解包） */
typedef union WirelessPack_u
{
    WirelessPack_t pack;
    uint8_t array[WIRELESS_PACK_MAX_LENGTH + 4];// +4 确保公用体中的array能覆盖pack的所有范围
}WirelessPack;



/* 供外部使用的变量 */
// extern QueueRM_t wirelessTxQueue[WIRELESS_OBJ_NUM];         // 无线发送队列
// extern QueueRM_t wirelessRxQueue[WIRELESS_OBJ_NUM];         // 无线接收队列
extern WirelessObjInfo_t* wirelessObjInfoTablePtr[WIRELESS_OBJ_NUM]; // 无线通信对象信息表指针
extern QueueRM_t* wirelessTxQueuePtr[WIRELESS_OBJ_NUM];              // 无线发送队列指针
extern QueueRM_t* wirelessRxQueuePtr[WIRELESS_OBJ_NUM];              // 无线接收队列指针
extern WirelessPack WirelessTxPack;                                  // 无线发送数据包
extern WirelessPack WirelessRxPack;                                  // 无线接收数据包
extern uint32_t wirelessTxLength;                                    // 无线发送长度
extern uint32_t wirelessRxLength;                                    // 无线接收长度
// extern WirelessStatus_ut wirelessStatus;                // 无线接收状态

/* 供外部调用的函数 */
void WirelessPackMaxSizeDeInit(void);
void WirelessPackMaxSizeSet(uint16_t size);
uint8_t WirelessObjInfoTablePtrConfig(WirelessObj_t obj, WirelessObjInfo_t* const objInfo);
uint8_t WirelessTxQueueInit(WirelessObj_t obj, QueueRM_t* const queue);
uint8_t WirelessRxQueueInit(WirelessObj_t obj, QueueRM_t* const queue);
// void WirelessInfoTableItemConfig(WirelessObj_t obj, const WirelessObjInfo_t* const objInfo);
void WirelessTxPackFill(WirelessObj_t obj); // 无线发送数据包填充函数
// void WirelessResendPack();
void WirelessRxPackHandle(WirelessObj_t obj);    // 无线接收数据包处理函数
void WirelessTimeoutRecord(WirelessObj_t obj);   // 无线超时记录
int8_t WirelessInfoTableQuery(void);             // 无线信息表查询
uint8_t WirelessRxPackVerify(WirelessObj_t obj); // 无线接收数据包校验

// uint8_t WirelessRxBuffRefresh(WirelessObj_t obj);
// uint8_t WirelessTxBuffRefresh(WirelessObj_t obj);
// void WirelessTxInfoTableUpdate(WirelessObj_t obj);


/* 需要由外部实现的函数 */
void WirelessInput(WirelessObj_t obj);
void WirelessOuput(WirelessObj_t obj);



#endif
