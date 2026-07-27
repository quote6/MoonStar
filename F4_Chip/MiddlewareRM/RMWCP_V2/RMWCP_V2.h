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

#pragma pack(1) /*指定下面的结构体按1字节对齐*/

/**
 * @brief RMWCP_V2状态结构体类型定义
 * @param has_rcvd_valid_ack : 接收到有效数据包
 * @param has_rcvd_repet_pack : 接收到重复数据包，这通常是对方没有收到上一次数据包产生的重发情况
 * @param hasRcvdInterferencePack : 接收到错误数据包。该数据包的源地址并不在当前记录的信息表里
 * @param has_lost_data : 数据丢失，对方的应答号与本机进行校验对不上
 */
typedef struct {
    uint16_t has_rcvd_valid_ack : 1;  /**< 接收到有效确认信息 */
    uint16_t has_rcvd_wrong_ack : 1;  /**< 接收到错误确认信息 */
    uint16_t has_rcvd_valid_pack : 1; /**< 接收到有效数据包ID */
    uint16_t has_rcvd_wrong_pack : 1; /**< 接收到错误数据包ID */
    uint16_t has_rcvd_repet_pack : 1; /**< 接收到重复数据包ID */
    uint16_t has_rcvd_error_pack : 1; /**< 接收到错误数据包 */
    uint16_t has_no_rcvd_resp : 1;    /**< 上次发送未收到应答 */
    uint16_t has_lost_data : 1;       /**< 发生数据丢失 */
    uint16_t is_reseted : 1;          /**< 已复位 */
    uint16_t reserved : 7;            /**< 保留 */
} RmwcpV2Status_t;


/* 定义RMWCP_V2状态共用体类型（方便整体赋值） */
typedef union RmwcpV2Status_u {
    RmwcpV2Status_t bits;
    uint16_t all;
} RmwcpV2Status_ut;

typedef struct RmwcpV2Ctrl_s {
    uint16_t rst_cmd : 1;   // 复位命令
    uint16_t reserved : 15; // 保留
} RmwcpV2Ctrl_t;

typedef union RmwcpV2Ctrl_u {
    RmwcpV2Ctrl_t bits;
    uint16_t all;
} RmwcpV2Ctrl_ut;

/**
 * @brief RMWCP_V2的包头类型定义
 * @note  三个字段组成 状态、控制和包长
 */
typedef struct RmwcpV2Hdr_s {
    RmwcpV2Status_ut stat;
    RmwcpV2Ctrl_ut ctrl;
    uint16_t length;
} RmwcpV2Hdr_t;


/* 定义数据包有效载荷最大长度 */
#define RMWCP_V2_PACK_PAYLOAD_MAX_LENGTH 1000

typedef struct RmwcpV2Pack_s {
    RmwcpV2Hdr_t hdr;
    uint8_t payload[RMWCP_V2_PACK_PAYLOAD_MAX_LENGTH];
} RmwcpV2Pack_t;

/* 定义RMWCP_V2 数据包最大长度 */
#define RMWCP_V2_PACK_MAX_LENGTH (sizeof(RmwcpV2Pack_t))

/* 定义RMWCP_V2 数据包公用体类型（方便数据包装包与解包） */
typedef union RmwcpV2Pack_u {
    RmwcpV2Pack_t parts;
    uint8_t collection[RMWCP_V2_PACK_MAX_LENGTH];
} RmwcpV2Pack_ut;


#pragma pack() /*取消指定对齐，恢复默认对齐*/

#endif
