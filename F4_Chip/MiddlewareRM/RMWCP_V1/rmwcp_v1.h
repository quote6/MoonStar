#ifndef __RMWCP_V1_H
#define __RMWCP_V1_H

#include <stdint.h>

#pragma pack(1) /*指定下面的结构体按1字节对齐*/

/**
 * @brief RMWCP_V1状态结构体类型定义
 * @param has_rcvd_valid_ack : 接收到有效数据包
 * @param has_rcvd_repet_pack : 接收到重复数据包，这通常是对方没有收到上一次数据包产生的重发情况
 * @param hasRcvdInterferencePack : 接收到错误数据包。该数据包的源地址并不在当前记录的信息表里
 * @param has_lost_data : 数据丢失，对方的应答号与本机进行校验对不上
 */
typedef struct RmwcpV1Status_s {
    uint16_t valid_ack_id : 1;    /**< 接收到有效应答号 */
    uint16_t invalid_ack_id : 1;  /**< 接收到无效应答号 */
    uint16_t valid_pack_id : 1;   /**< 接收到有效数据包ID */
    uint16_t invalid_pack_id : 1; /**< 接收到错误数据包ID */
    uint16_t repet_pack_id : 1;   /**< 接收到重复数据包ID */
    uint16_t error_pack : 1;      /**< 接收到错误数据包 */
    uint16_t no_resp : 1;         /**< 上次发送未收到应答 */
    uint16_t lost_data : 1;       /**< 发生数据丢失 */
    uint16_t reseted : 1;         /**< 已复位 */
    uint16_t call_resp : 1;       /**< 叫号应答 */
    uint16_t call_err : 1;        /**< 叫号错误 */
    uint16_t _ : 6;
} RmwcpV1Status_t;


/* 定义RMWCP_V2状态共用体类型（方便整体赋值） */
typedef union RmwcpV1Status_u {
    RmwcpV1Status_t bits;
    uint16_t all;
} RmwcpV1Status_ut;

typedef struct RmwcpV1Ctrl_s {
    uint16_t rst_cmd : 1; /**< 复位命令 */
    uint16_t calling : 1; /**< 叫号 */
    uint16_t restart : 1; /**< 重新开始 */
    uint16_t _ : 14;
} RmwcpV1Ctrl_t;

typedef union RmwcpV1Ctrl_u {
    RmwcpV1Ctrl_t bits;
    uint16_t all;
} RmwcpV1Ctrl_ut;

// typedef enum {
//     RMWCP_V1_PACK_TYPE_DATA = 0x00,    /**< 数据包 */
//     RMWCP_V1_PACK_TYPE_RESET = 0x01,   /**< 复位命令（只复位协议指针） */
//     RMWCP_V1_PACK_TYPE_CALL = 0x02,    /**< 叫号包 */
//     RMWCP_V1_PACK_TYPE_RESTART = 0x03, /**< 重新开始（退出当前流程重新开始通信）  */
// } RmwcpV1PackType_t;

/**
 * @brief RMWCP_V1的包头类型定义
 * @note
 */
typedef struct RmwcpV1Hdr_s {
    // RmwcpV1PackType_t pack_type; /**< 数据包类型 */
    RmwcpV1Status_ut stat;       /**< 状态信息（反馈给对方） */
    RmwcpV1Ctrl_ut ctrl;         /**< 控制信息（反馈给对方） */
    uint16_t pack_id;            /**< 数据包ID */
    uint16_t ack_id;             /**< 应答号 */
    uint8_t addr_dst;            /**< 目标地址 */
    uint8_t addr_src;            /**< 源地址 */
    uint16_t len;                /**< 数据包总长度（包含有效载荷长度） */
} RmwcpV1Hdr_t;

/* 定义RMWCP_V1数据包头长度 */
#define RMWCP_V1_HDR_LEN              (sizeof(RmwcpV1Hdr_t))

/* 定义RMWCP_V1 数据包有效载荷最大长度 */
#define RMWCP_V1_PACK_PAYLOAD_LEN_MAX 1000

typedef struct RmwcpV1Pack_s {
    RmwcpV1Hdr_t hdr;
    uint8_t payload[RMWCP_V1_PACK_PAYLOAD_LEN_MAX];
} RmwcpV1Pack_t;

/* 定义RMWCP_V1 数据包最大长度 */
#define RMWCP_V1_PACK_LEN_MAX (sizeof(RmwcpV1Pack_t))

typedef union {
    RmwcpV1Pack_t parts;
    uint8_t collection[RMWCP_V1_PACK_LEN_MAX];
} RmwcpV1Pack_ut;


#pragma pack() /*取消指定对齐，恢复默认对齐*/

#endif