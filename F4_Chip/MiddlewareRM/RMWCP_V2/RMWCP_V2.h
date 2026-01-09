/**
 * ************************************************************************
 * 
 * @file RMWCP.h
 * @author quote6 (2293127401@qq.com)
 * @brief RM无线通信协议，版权属于quote6
 *        RMWCP 为 Reimu Wireless Communication Protocol 缩写
 *        基于IEEE 802.15.4 协议在其上实现的协议
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2025 quote6 
 * For study and research only, no reprinting
 * ************************************************************************
 */
#ifndef __RMWCP_V2_H
#define __RMWCP_V2_H

#include <stdint.h>

#pragma pack (1) /*指定下面的结构体按1字节对齐*/

/**
 * @brief RMWCP_V2状态结构体类型定义
 * @param hasRcvdValidPack : 接收到有效数据包
 * @param hasRcvdRepetPack : 接收到重复数据包，这通常是对方没有收到上一次数据包产生的重发情况
 * @param hasRcvdInterferencePack : 接收到错误数据包。该数据包的源地址并不在当前记录的信息表里
 * @param hasLostData : 数据丢失，对方的应答号与本机进行校验对不上
 */
typedef struct RMWCP_V2_Status_s {
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
} RMWCP_V2_Status_t;


/* 定义RMWCP_V2状态共用体类型（方便整体赋值） */
typedef union RMWCP_V2_Status_u {
    RMWCP_V2_Status_t status;
    uint16_t statuss; // 末尾多一个s用于跟上面的变量区分
}RMWCP_V2_Status_ut;

typedef struct RMWCP_V2_Control_s
{
    uint16_t resetCommand : 1; // 复位命令
    uint16_t reserved : 15;    // 保留
}RMWCP_V2_Control_t;

typedef union RMWCP_V2_Control_u
{
    RMWCP_V2_Control_t control;
    uint16_t controll; // 末尾多一个l用于跟上面的变量区分
}RMWCP_V2_Control_ut;

/**
 * @brief RMWCP_V2的包头类型定义
 * @note  三个字段组成 状态、控制和包长
 */
typedef struct RMWCP_V2_HDR_s{
    RMWCP_V2_Status_ut stat;
    RMWCP_V2_Control_ut ctrl;
    uint16_t length;
}RMWCP_V2_HDR_t;


/* 定义数据包有效载荷最大长度 */
#define RMWCP_V2_PACK_PAYLOAD_MAX_LENGTH 1000

typedef struct RMWCP_V2_Pack_s {
    RMWCP_V2_HDR_t hdr;
    uint8_t payload[RMWCP_V2_PACK_PAYLOAD_MAX_LENGTH];
} RMWCP_V2_Pack_t;

/* 定义RMWCP_V2 数据包最大长度 */
#define RMWCP_v2_PACK_MAX_LENGTH (sizeof(RMWCP_V2_Pack_t))

/* 定义RMWCP_V2 数据包公用体类型（方便数据包装包与解包） */
typedef union RMWCP_V2_Pack_u {
    RMWCP_V2_Pack_t pack;
    uint8_t array[RMWCP_v2_PACK_MAX_LENGTH + 4]; // +4 确保公用体中的array能覆盖pack的所有范围
}RMWCP_V2_Pack_ut;


#pragma pack () /*取消指定对齐，恢复默认对齐*/

#endif
