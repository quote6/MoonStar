/**
 * ************************************************************************
 *
 * @file MAC_802_15_4.h
 * @author quote6 (2293127401@qq.com)
 * @brief 根据DW1000的功能需要编写的IEEE 802.15.4的帧结构
 *        不完全实现协议的功能，只针对项目所需的功能进行编写
 *
 * ************************************************************************
 * @copyright Copyright (c) 2025 quote6
 * For study and research only, no reprinting
 * ************************************************************************
 */
#ifndef __MAC_802_15_4_H
#define __MAC_802_15_4_H

#include <stdint.h>

#ifndef NULL
#define NULL ((void*) 0)
#endif

#pragma pack(1) /*指定下面的结构体按1字节对齐*/

/* IEEE 802.15.4 通用MAC帧格式
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
    |                                                  MAC 包头（MHR）                                             | MAC 载荷      | MAC包尾（MFR）|
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
    | 字段    | 帧控制         | 序列号          | 目标PAN ID   | 目标地址  | 源PAN ID   | 源地址    | 辅助安全头      | 帧载荷         | 帧检查序列    |
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
    | Filed  | Frame Control | Sequence Number | Dest PAN ID | Dest Addr | Src PAN ID | Src Addr | Aux Sec Header | Frame Payload | FCS          |
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
    | 字节数  | 2             |   1             | 0/2/8       | 0/2       | 0/2        | 0/2/8    |  0/5/6/10/14    | 不定          | 2            |
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
*/

/* IEEE 802.15.4 MAC帧头的帧控制字段的组成
    +------------+-----------------+---------------+-------------+-----------------+-----------+--------------------+---------------+---------------------+--+
    | Bit [0:2]  | Bit 3           | Bit 4         | Bit 5       | Bit 6           | Bit [7:9] | Bit [10:11]        | Bit [12:13]   | Bit [14:15]         |  |
    +------------+-----------------+---------------+-------------+-----------------+-----------+--------------------+---------------+---------------------+--+
    | Frame Type | Security Enable | Frame Pending | ACK Request | PAN ID Compress | Reserved  | Dest. Address Mode | Frame Version | Source Address Mode |  |
    +------------+-----------------+---------------+-------------+-----------------+-----------+--------------------+---------------+---------------------+--+
    | 帧类型      | 安全使能         | 帧挂起         | 应答请求    | PAN ID 压缩      | 保留       | 目标地址模式         | 帧版本         | 源地址模式           |  |
    +------------+-----------------+---------------+-------------+-----------------+-----------+--------------------+---------------+---------------------+--+
*/

/**
 * @brief 帧类型定义
 * @note 定义帧的基本类型
 * @param FRAME_TYPE_BEACON: 信标帧
 * @param FRAME_TYPE_DATA: 数据帧
 * @param FRAME_TYPE_ACK: 应答帧
 * @param FRAME_TYPE_MAC_CMD: MAC命令帧
 */
typedef enum FrameType_e {
    FRAME_TYPE_BEACON = 0,
    FRAME_TYPE_DATA,
    FRAME_TYPE_ACK,
    FRAME_TYPE_MAC_CMD,
    FRAME_TYPE_RESERVED,
    FRAME_TYPE_MULTI,
    FRAME_TYPE_FRAGMENT,
    FRAME_TYPE_EXTENDED
} FrameType_t;

/**
 * @brief 安全使能定义
 * @note  指示帧是否包含安全头部
 * @param SECURITY_ENABLE_NO_PROTECTED: 禁用
 * @param SECURITY_ENABLE_PROTECTED: 启用
 */
typedef enum SecurityEnable_e {
    SECURITY_ENABLE_NO_PROTECTED = 0,
    SECURITY_ENABLE_PROTECTED
} SecurityEnable_t;

/**
 * @brief 帧挂起定义
 * @note  仅用于信标帧，指示协调器是否有数据
 * @param FRAME_PENDING_NO_MORE_DATA: 无数据等待
 * @param FRAME_PENDING_MORE_DATA: 协调器有数据等待
 */
typedef enum FramePending_e {
    FRAME_PENDING_NO_MORE_DATA = 0,
    FRAME_PENDING_MORE_DATA
} FramePending_t;

/**
 * @brief 应答请求定义
 * @note  要求接收方返回ACK帧
 * @param ACK_REQUEST_NO_REQUIRED: 不请求应答
 * @param ACK_REQUEST_REQUIRED: 请求应答
 */
typedef enum AckRequest_e {
    ACK_REQUEST_NO_REQUIRED = 0,
    ACK_REQUEST_REQUIRED
} AckRequest_t;

/**
 * @brief PAN ID压缩 定义
 * @note  PAN ID压缩位用于指定当源地址和目标地址同时存在时，
 *        MAC帧是否仅包含一个PAN标识符字段。
 * @param PAN_ID_COMPRESS_NO: 不压缩
 * @param PAN_ID_COMPRESS_DEST_EXIST_SOURCE_NOT: 目标地址PAN ID存在，
 *                       源地址省略（默认源地址 PAN ID 与目标地址一致）
 */
typedef enum PanIdCompress_e {
    PAN_ID_COMPRESS_NO = 0,
    PAN_ID_COMPRESS_DEST_EXIST_SOURCE_NOT
} PanIdCompress_t;


#if 0 /* 该部分暂时未使用，直接注释 */
typedef enum SEQ_NUM_Suppress_e{
    SEQ_NUM_SUPPRESS_PRESENT = 0,
    SEQ_NUM_SUPPRESS_NOT_PRESENT
}SEQ_NUM_Suppress_t;

typedef enum IE_Present_e{
    IE_PRESENT_NO = 0,
    IE_PRESENT_YES
}IE_Present_t;
#endif

/**
 * @brief 目标地址模式定义
 * @note  用于指定帧是否包含目标地址及地址字段的大小
 * @param DST_ADDR_MODE_NO_PEND_AND_ADDR: 帧中不存在目标地址或目标PAN ID
 * @param DST_ADDR_MODE_RESERVED: 保留
 * @param DST_ADDR_MODE_SHORT_ADDR_16_BITS: 目标地址字段长度16位
 * @param DST_ADDR_MODE_EXT_ADDR_64_BITS: 目标地址字段长度64位
 */
typedef enum DstAddrMode_e {
    DST_ADDR_MODE_NO_PEND_AND_ADDR = 0,
    DST_ADDR_MODE_RESERVED,
    DST_ADDR_MODE_SHORT_ADDR_16_BITS,
    DST_ADDR_MODE_EXT_ADDR_64_BITS
} DstAddrMode_t;

/**
 * @brief 帧版本定义
 * @note  用于指定帧的版本号。其中规定帧版本字段应设置为
          0x00表示兼容IEEE 802.15.4‑2003的帧，
          0x01表示IEEE 802.15.4帧。
          DW1000可以识别这些帧版本字段规则，并将拒绝接收版本不正确的帧。
 */
#define FRAME_VERSION 0x01

/**
 * @brief 源地址模式定义
 * @note  用于指定帧是否包含源地址以及地址字段的大小（若存在）
 * @param SRC_ADDR_MODE_NO_PEND_AND_ADDR: 帧中不存在目标地址或目标PAN ID
 * @param SRC_ADDR_MODE_RESERVED: 保留
 * @param SRC_ADDR_MODE_SHORT_ADDR_16_BITS: 源地址字段长度16位
 * @param SRC_ADDR_MODE_EXT_ADDR_64_BITS: 源地址字段长度64位
 */
typedef enum SrcAddrMode_e {
    SRC_ADDR_MODE_NO_PEND_AND_ADDR = 0,
    SRC_ADDR_MODE_RESERVED,
    SRC_ADDR_MODE_SHORT_ADDR_16_BITS,
    SRC_ADDR_MODE_EXT_ADDR_64_BITS
} SrcAddrMode_t;


#pragma pack(1) /* 对于接下来的结构体采用单字节对齐方式 */

/**
 * @brief MAC帧格式中的帧控制字段定义
 * @param frame_type: 帧类型 Bit[0:2]
 * @param security_enable: 安全使能 Bit[3]
 * @param frame_pending: 帧挂起 Bit[4]
 * @param ack_request: 应答请求 Bit[5]
 * @param pan_id_compress: PAN ID压缩 Bit[6]
 * @param reserved: 保留 Bit[7:9]
 * @param dst_addr_mode: 目标地址模式 Bit[10:11]
 * @param frame_version: 帧版本 Bit[12:13]
 * @param src_addr_mode: 源地址模式 Bit[14:15]
 */
typedef struct FrameControl_s {
    uint16_t frame_type : 3;
    uint16_t security_enable : 1;
    uint16_t frame_pending : 1;
    uint16_t ack_request : 1;
    uint16_t pan_id_compress : 1;
    uint16_t reserved : 3;
    uint16_t dst_adrr_mode : 2;
    uint16_t frame_version : 2;
    uint16_t src_addr_mode : 2;
} FrameControl_t;

/**
 * @brief 帧控制共用体定义
 * @note  只是为了方便赋值定义的
 */
typedef union FrameControl_u {
    FrameControl_t bits;
    uint16_t all;
} FrameControl_ut;

#if 0 /* 辅助安全头结构体类型定义（当前为使用所以注释） */
typedef struct MacAuxSecurity_s
{
    uint8_t security_ctrl;
    uint8_t frame_counter[4];
    uint8_t key_indentifier;
}MacAuxSecurity_t;
#endif

/**
 * @brief 地址长度定义
 * @note  根据IEEE802.15.4的定义，目标地址长度和源地址长度是在帧控制里控制的，
 *        也就是可变的。但对于目前的项目使用需求，并不需要64位的地址长度，16位
 *        的地址长度足够使用，所以这里固定地址长度为16位，并在下面的结构体定义
 *        中使用
 */
#define ADDRESS_LENGTH  2
#define ADDR_DST_LENGTH ADDRESS_LENGTH
#define ADDR_SRC_LENGTH ADDRESS_LENGTH

/**
 * @brief 目标PAN ID类型定义
 * @note  本项目中固定目标PAN ID类型大小 2字节
 */
typedef struct PanIdDst_s {
    uint8_t id[ADDR_DST_LENGTH];
} PanIdDst_t;

/**
 * @brief 目标地址类型定义
 * @note  本项目中固定目标地址大小 2字节
 */
typedef struct AddrDst_s {
    uint8_t addr[ADDR_DST_LENGTH];
} AddrDst_t;

#if 0 /* 因为使用了PAN ID压缩，所以这个类型定义不使用 */
/**
 * @brief 源PAN ID类型定义
 * @note  本项目中固定源PAN ID类型大小 2字节
 */
typedef struct PanIdSrc_s{
    uint8_t id[ADDR_SRC_LENGTH];
}PanIdSrc_t;
#endif

/**
 * @brief 源地址类型定义
 * @note  本项目中固定源地址大小 2字节
 */
typedef struct AddrSrc_s {
    uint8_t id[ADDR_SRC_LENGTH];
} AddrSrc_t;


/**
 * @brief MAC包头结构体类型定义
 * @note 仅针对当前的项目使用，
 *       设置地址长度为2字节（在帧控制字段设置了对应位）
 *       不使用安全头部（在帧控制字段设置了对应位）
 *       使用PAN ID压缩，帧只含有目标的PAN ID地址（在帧控制字段设置了对应位）
 * @param frame_ctrl: 帧控制 2字节
 * @param seq_num: 序列号 1字节
 * @param pan_id_dst: 目标PAN ID 2字节
 * @param addr_dst: 目标地址 2字节
 * @param pan_id_src: 源PAN ID 2字节（使用了PAN ID压缩，这个字段无效）
 * @param addr_src: 源地址 2字节
 */
typedef struct Mhr_802_15_4_s {
    FrameControl_ut frame_ctrl;
    uint8_t seq_num;
    PanIdDst_t pan_id_dst;
    AddrDst_t addr_dst;
#if 0 /* 使用了PAN ID压缩，所以帧不包含源PAN ID */
    PanIdSrc_t pan_id_src;
#endif
    AddrSrc_t addr_src;
#if 0 /* Aux security字段不使用 */
    MacAuxSecurity_t aux;
#endif
} Mhr_802_15_4_t;

/* 定义MAC帧头长度 */
#define MAC_HEADER_LENGTH sizeof(Mhr_802_15_4_t)

/**
 * @brief MAC帧头共用体类型定义
 * @note  为了方便整体赋值所以定义了该类型
 */
typedef union Mhr_802_15_4_u {
    Mhr_802_15_4_t parts;
    uint8_t collection[MAC_HEADER_LENGTH];
} Mhr_802_15_4_ut;

#pragma pack() /* 恢复默认对齐方式，下面的结构体不需要单字节对齐 */

typedef struct MacFrame_802_15_4_s {
    Mhr_802_15_4_ut mhr;
    uint8_t* payload;
#if 0 /* FCS由硬件自动添加，这里就不添加该字段 */
    uint16_t FCS;
#endif
} MacFrame_802_15_4_t;

#pragma pack() /*取消指定对齐，恢复默认对齐*/

/* 定义PAN ID广播地址 */
#define PAN_ID_BROADCAST 0xFFFF

/* 定义广播地址 */
#define ADDR_BROADCAST   0xFFFF

/* 供外部调用的函数 */
#define MAC_FRAME_802_15_4_SEQ_NUM_SET(frame, seq) \
    do {                                           \
        frame->mhr.parts.seq_num = seq;            \
    } while (0)

#define MAC_FRAME_802_15_4_PAN_ID_DST_SET(frame, dst) \
    do {                                              \
        frame->mhr.parts.pan_id_dst = dst;            \
    } while (0)

#define MAC_FRAME_802_15_4_ADDR_DST_SET(frame, dst) \
    do {                                            \
        frame->mhr.parts.addr_dst = dst;            \
    } while (0)

#define MAC_FRAME_802_15_4_ADDR_SRC_SET(frame, dst) \
    do {                                            \
        frame->mhr.parts.addr_src = src;            \
    } while (0)

#define MAC_FRAME_CONTROL_AR_ENABLE(frame)                                   \
    do {                                                                     \
        frame->mhr.parts.frame_ctrl.bits.ack_request = ACK_REQUEST_REQUIRED; \
    } while (0)

#define MAC_FRAME_CONTROL_AR_DISABLE(frame)                                       \
    do {                                                                          \
        frame->mhr.parts.frame_ctrl.bits.ack_request = ACK_REQUEST_NO_REQUIRED; \
    } while (0)

#define MAC_FRAME_802_15_4_SEQ_NUM_UPDATE(frame) \
    do {                                         \
        frame->mhr.parts.seq_num++;              \
    } while (0)

#define MAC_FRAME_802_15_4_SEQ_NUM_GET(frame, seq) \
    do {                                           \
        *seq = frame->mhr.parts.seq_num;           \
    } while (0)

#define MAC_FRAME_802_15_4_PAN_ID_DST_GET(frame, dst) \
    do {                                              \
        *dst = frame->mhr.parts.pan_id_dst;           \
    } while (0)

#define MAC_FRAME_802_15_4_ADDR_DST_GET(frame, dst) \
    do {                                            \
        *dst = frame->mhr.parts.addr_dst;           \
    } while (0)

#if 0 /* 因为使用了PAN ID压缩，所以没有源PAN ID，这个宏函数也不会被使用 */
#define MAC_FRAME_802_15_4_PAN_ID_SRC_GET(frame, src) \
    do {                                              \
        *src = frame->header.mhr.PAN_ID_Source;       \
    } while (0)
#endif

#define MAC_FRAME_802_15_4_ADDR_SRC_GET(frame, src) \
    do {                                            \
        *src = frame->mhr.parts.addr_src;           \
    } while (0)

uint8_t MAC_Frame_802_15_4_Init(MacFrame_802_15_4_t* frame);
uint8_t MAC_Frame_Control_AR_ENABLE(MacFrame_802_15_4_t* frame);
uint8_t MAC_Frame_Control_AR_DISABLE(MacFrame_802_15_4_t* frame);
uint8_t MAC_Frame_802_15_4_SeqNumSet(MacFrame_802_15_4_t* frame, uint8_t seq);
uint8_t MAC_Frame_802_15_4_PAN_ID_DST_Set(MacFrame_802_15_4_t* frame, PanIdDst_t dst);
uint8_t MAC_Frame_802_15_4_ADDR_DST_Set(MacFrame_802_15_4_t* frame, AddrDst_t dst);
// uint8_t MAC_Frame_802_15_4_PAN_ID_SRC_Set(MacFrame_802_15_4_t* frame, PanIdSrc_t src);
uint8_t MAC_Frame_802_15_4_ADDR_SRC_Set(MacFrame_802_15_4_t* frame, AddrSrc_t src);
uint8_t MAC_Frame_802_15_4_SeqNumUpdate(MacFrame_802_15_4_t* frame);

uint8_t MAC_Frame_802_15_4_SeqNumGet(MacFrame_802_15_4_t* frame, uint8_t* seq);
uint8_t MAC_Frame_802_15_4_PAN_ID_DST_Get(MacFrame_802_15_4_t* frame, PanIdDst_t* dst);
uint8_t MAC_Frame_802_15_4_ADDR_DST_Get(MacFrame_802_15_4_t* frame, AddrDst_t* dst);
// uint8_t MAC_Frame_802_15_4_PAN_ID_SRC_Get(MacFrame_802_15_4_t* frame, PanIdSrc_t* src);
uint8_t MAC_Frame_802_15_4_ADDR_SRC_Get(MacFrame_802_15_4_t* frame, AddrSrc_t* src);


#endif
