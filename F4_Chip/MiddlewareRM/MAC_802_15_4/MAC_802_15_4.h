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
#define NULL ((void*)0)
#endif

/* IEEE 802.15.4 通用MAC帧格式
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
    |                                                  MAC 包头（MHR）                                             | MAC 载荷      | MAC包尾（MFR）|
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
    | 字段   | 帧控制         | 序列号          | 目标PAN ID   | 目标地址  | 源PAN ID   | 源地址    | 辅助安全头      | 帧载荷         | 帧检查序列    |
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
    | Filed  | Frame Control | Sequence Number | Dest PAN ID | Dest Addr | Src PAN ID | Src Addr | Aux Sec Header | Frame Payload | FCS          |
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
    | 字节数 | 2             |   1             | 0/2/8       | 0/2       | 0/2        | 0/2/8    |  0/5/6/10/14    | 不定          | 2            |
    +--------+---------------+-----------------+-------------+-----------+------------+----------+----------------+---------------+--------------+
*/

/* IEEE 802.15.4 MAC帧头的帧控制字段的组成
    +------------+-----------------+---------------+-------------+-----------------+-----------+--------------------+---------------+---------------------+--+
    | Bit [0:2]  | Bit 3           | Bit 4         | Bit 5       | Bit 6           | Bit [7:9] | Bit [10:11]        | Bit [12:13]   | Bit [14:15]         |  |
    +------------+-----------------+---------------+-------------+-----------------+-----------+--------------------+---------------+---------------------+--+
    | Frame Type | Security Enable | Frame Pending | ACK Request | PAN ID Compress | Reserved  | Dest. Address Mode | Frame Version | Source Address Mode |  |
    +------------+-----------------+---------------+-------------+-----------------+-----------+--------------------+---------------+---------------------+--+
    | 帧类型     | 安全使能         | 帧挂起         | 应答请求    | PAN ID 压缩      | 保留      | 目标地址模式        | 帧版本         | 源地址模式           |  |
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
typedef enum FrameType_e{
    FRAME_TYPE_BEACON = 0,
    FRAME_TYPE_DATA,
    FRAME_TYPE_ACK,
    FRAME_TYPE_MAC_CMD,
    FRAME_TYPE_RESERVED,
    FRAME_TYPE_MULTI,
    FRAME_TYPE_FRAGMENT,
    FRAME_TYPE_EXTENDED
}FrameType_t;

/**
 * @brief 安全使能定义
 * @note  指示帧是否包含安全头部
 * @param SECURITY_ENABLE_NO_PROTECTED: 禁用
 * @param SECURITY_ENABLE_PROTECTED: 启用
 */
typedef enum SecurityEnable_e{
    SECURITY_ENABLE_NO_PROTECTED = 0,
    SECURITY_ENABLE_PROTECTED
}SecurityEnable_t;

/**
 * @brief 帧挂起定义
 * @note  仅用于信标帧，指示协调器是否有数据
 * @param FRAME_PENDING_NO_MORE_DATA: 无数据等待
 * @param FRAME_PENDING_MORE_DATA: 协调器有数据等待
 */
typedef enum FramePending_e{
    FRAME_PENDING_NO_MORE_DATA = 0,
    FRAME_PENDING_MORE_DATA
}FramePending_t;

/**
 * @brief 应答请求定义
 * @note  要求接收方返回ACK帧
 * @param ACK_REQUEST_NO_REQUIRED: 不请求应答
 * @param ACK_REQUEST_REQUIRED: 请求应答
 */
typedef enum ACK_Request_e{
    ACK_REQUEST_NO_REQUIRED = 0,
    ACK_REQUEST_REQUIRED
}ACK_Request_t;

/**
 * @brief PAN ID压缩 定义
 * @note  PAN ID压缩位用于指定当源地址和目标地址同时存在时，
 *        MAC帧是否仅包含一个PAN标识符字段。
 * @param PAN_ID_COMPRESS_NO: 不压缩
 * @param PAN_ID_COMPRESS_DEST_EXIST_SOURCE_NOT: 目标地址PAN ID存在，
 *                       源地址省略（默认源地址 PAN ID 与目标地址一致）
 */
typedef enum PAN_ID_Compress_e{
   PAN_ID_COMPRESS_NO = 0,
   PAN_ID_COMPRESS_DEST_EXIST_SOURCE_NOT
}PAN_ID_Compress_t;


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
 * @param DEST_ADDR_MODE_NO_PEND_AND_ADDR: 帧中不存在目标地址或目标PAN ID
 * @param DEST_ADDR_MODE_RESERVED: 保留
 * @param DEST_ADDR_MODE_SHORT_ADDR_16_BITS: 目标地址字段长度16位
 * @param DEST_ADDR_MODE_EXT_ADDR_64_BITS: 目标地址字段长度64位
 */
typedef enum DEST_ADDR_Mode_e{
    DEST_ADDR_MODE_NO_PEND_AND_ADDR = 0,
    DEST_ADDR_MODE_RESERVED,
    DEST_ADDR_MODE_SHORT_ADDR_16_BITS,
    DEST_ADDR_MODE_EXT_ADDR_64_BITS
}DEST_ADDR_Mode_t;

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
typedef enum SRC_ADDR_Mode_e{
    SRC_ADDR_MODE_NO_PEND_AND_ADDR = 0,
    SRC_ADDR_MODE_RESERVED,
    SRC_ADDR_MODE_SHORT_ADDR_16_BITS,
    SRC_ADDR_MODE_EXT_ADDR_64_BITS
}SRC_ADDR_Mode_t;


#pragma pack(1) /* 对于接下来的结构体采用单字节对齐方式 */

/**
 * @brief MAC帧格式中的帧控制字段定义
 * @param frameType: 帧类型 Bit[0:2]
 * @param securityEnable: 安全使能 Bit[3]
 * @param framePending: 帧挂起 Bit[4]
 * @param ACK_Request: 应答请求 Bit[5]
 * @param PAN_ID_Compress: PAN ID压缩 Bit[6]
 * @param reserved: 保留 Bit[7:9]
 * @param destinationAddressMode: 目标地址模式 Bit[10:11]
 * @param frameVersion: 帧版本 Bit[12:13]
 * @param sourceAddressMode: 源地址模式 Bit[14:15]
 */
typedef struct FrameControl_s{
    uint16_t frameType : 3;    
    uint16_t securityEnable : 1;
    uint16_t framePending : 1;
    uint16_t ACK_request : 1;
    uint16_t PAN_ID_compress : 1;
    uint16_t reserved : 3;
    uint16_t destinationAddressMode : 2;
    uint16_t frameVersion : 2;
    uint16_t sourceAddressMode : 2;
}FrameControl_t;

/**
 * @brief 帧控制共用体定义
 * @note  只是为了方便赋值定义的
 */
typedef union FrameControl_u{
    FrameControl_t framecontrol;
    uint16_t framecontroll;
}FrameControl_ut;

#if 0 /* 辅助安全头结构体类型定义（当前为使用所以注释） */
typedef struct MAC_AUX_Security_s
{
    uint8_t security_ctrl;
    uint8_t frame_counter[4];
    uint8_t key_indentifier;
}MAC_AUX_Security_t;
#endif

/**
 * @brief 地址长度定义
 * @note  根据IEEE802.15.4的定义，目标地址长度和源地址长度是在帧控制里控制的，
 *        也就是可变的。但对于目前的项目使用需求，并不需要64位的地址长度，16位
 *        的地址长度足够使用，所以这里固定地址长度为16位，并在下面的结构体定义
 *        中使用
 */
#define ADDRESS_LENGTH 2
#define DST_ADDR_LENGTH ADDRESS_LENGTH
#define SRC_ADDR_LENGTH ADDRESS_LENGTH

/**
 * @brief 目标PAN ID类型定义
 * @note  本项目中固定目标PAN ID类型大小 2字节
 */
typedef struct PAN_ID_DST_s{
    uint8_t id[DST_ADDR_LENGTH];
}PAN_ID_DST_t;

/**
 * @brief 目标地址类型定义
 * @note  本项目中固定目标地址大小 2字节
 */
typedef struct ADDR_DST_s {
    uint8_t addr[DST_ADDR_LENGTH];
}ADDR_DST_t;

#if 0 /* 因为使用了PAN ID压缩，所以这个类型定义不使用 */
/**
 * @brief 源PAN ID类型定义
 * @note  本项目中固定源PAN ID类型大小 2字节
 */
typedef struct PAN_ID_SRC_s{
    uint8_t ID[SRC_ADDR_LENGTH];
}PAN_ID_SRC_t;
#endif

/**
 * @brief 源地址类型定义
 * @note  本项目中固定源地址大小 2字节
 */
typedef struct ADDR_SRC_s {
    uint8_t id[SRC_ADDR_LENGTH];
}ADDR_SRC_t;


/**
 * @brief MAC包头结构体类型定义
 * @note 仅针对当前的项目使用，
 *       设置地址长度为2字节（在帧控制字段设置了对应位）
 *       不使用安全头部（在帧控制字段设置了对应位）
 *       使用PAN ID压缩，帧只含有目标的PAN ID地址（在帧控制字段设置了对应位）
 * @param frameControl: 帧控制 2字节
 * @param sequence_num: 序列号 1字节
 * @param PAN_ID_Destinatnion: 目标PAN ID 2字节
 * @param ADDR_Destination: 目标地址 2字节
 * @param PAN_ID_Source: 源PAN ID 2字节（使用了PAN ID压缩，这个字段无效）
 * @param source_address: 源地址 2字节
 */
typedef struct MHR_802_15_4_s{
    FrameControl_ut frameControl;
    uint8_t sequenceNumber;
    PAN_ID_DST_t PAN_ID_destinatnion;
    ADDR_DST_t ADDR_destination;
    #if 0 /* 使用了PAN ID压缩，所以帧不包含源PAN ID */
    PAN_ID_SRC_t PAN_ID_Source;
    #endif
    ADDR_SRC_t ADDR_source;
    #if 0 /* Aux security字段不使用 */
    MAC_AUX_Security_t aux;
    #endif
}MHR_802_15_4_t;

/* 定义MAC帧头长度 */
#define MAC_HEADER_LENGTH sizeof(MHR_802_15_4_t)

/**
 * @brief MAC帧头共用体类型定义
 * @note  为了方便整体赋值所以定义了该类型
 */
typedef union MHR_802_15_4_u{
    MHR_802_15_4_t mhr;
    uint8_t mhrr[MAC_HEADER_LENGTH];
}MHR_802_15_4_ut;

#pragma pack() /* 恢复默认对齐方式，下面的结构体不需要单字节对齐 */

typedef struct MAC_Frame_802_15_4_s{
    MHR_802_15_4_ut header;
    uint8_t* payload;
    #if 0 /* FCS由硬件自动添加，这里就不添加该字段 */
    uint16_t FCS;
    #endif
}MAC_Frame_802_15_4_t;


/* 定义PAN ID广播地址 */
#define PAN_ID_BROADCAST 0xFFFF

/* 定义广播地址 */
#define ADDR_BROADCAST 0xFFFF

/* 供外部调用的函数 */
#define MAC_FRAME_802_15_4_SEQ_NUM_SET(frame, seq) \
    do {                                           \
        frame->header.mhr.sequenceNumber = seq;    \
    } while (0)

#define MAC_FRAME_802_15_4_PAN_ID_DST_SET(frame, dst) \
    do {                                              \
        frame->header.mhr.PAN_ID_Destinatnion = dst;  \
    } while (0)

#define MAC_FRAME_802_15_4_ADDR_DST_SET(frame, dst) \
    do {                                            \
        frame->header.mhr.ADDR_Destination = dst;   \
    } while (0)

#define MAC_FRAME_802_15_4_ADDR_SRC_SET(frame, dst) \
    do {                                            \
        frame->header.mhr.ADDR_Source = src;        \
    } while (0)

#define MAC_FRAME_CONTROL_AR_ENABLE(frame)                                              \
    do {                                                                                \
        frame->header.mhr.frameControl.framecontrol.ACK_Request = ACK_REQUEST_REQUIRED; \
    } while (0)

#define MAC_FRAME_CONTROL_AR_DISABLE(frame)                                             \
    do {                                                                                \
        frame->header.mhr.frameControl.framecontrol.ACK_Request = ACK_REQUEST_NO_REQUIRED; \
    } while (0)

#define MAC_FRAME_802_15_4_SEQ_NUM_UPDATE(frame) \
    do {                                         \
        frame->header.mhr.sequenceNumber++;      \
    } while (0)

#define MAC_FRAME_802_15_4_SEQ_NUM_GET(frame, seq) \
    do {                                           \
        *seq = frame->header.mhr.sequenceNumber;   \
    } while (0)

#define MAC_FRAME_802_15_4_PAN_ID_DST_GET(frame, dst) \
    do {                                              \
        *dst = frame->header.mhr.PAN_ID_Destinatnion; \
    } while (0)

#define MAC_FRAME_802_15_4_ADDR_DST_GET(frame, dst) \
    do {                                            \
        *dst = frame->header.mhr.ADDR_Destination;  \
    } while (0)

#if 0 /* 因为使用了PAN ID压缩，所以没有源PAN ID，这个宏函数也不会被使用 */
#define MAC_FRAME_802_15_4_PAN_ID_SRC_GET(frame, src) \
    do {                                              \
        *src = frame->header.mhr.PAN_ID_Source;       \
    } while (0)
#endif

#define MAC_FRAME_802_15_4_ADDR_SRC_GET(frame, src) \
    do {                                            \
        *src = frame->header.mhr.ADDR_Source;       \
    } while (0)

uint8_t MAC_Frame_802_15_4_Init(MAC_Frame_802_15_4_t* frame);
uint8_t MAC_Frame_Control_AR_ENABLE(MAC_Frame_802_15_4_t* frame);
uint8_t MAC_Frame_Control_AR_DISABLE(MAC_Frame_802_15_4_t* frame);
uint8_t MAC_Frame_802_15_4_SeqNumSet(MAC_Frame_802_15_4_t* frame, uint8_t seq);
uint8_t MAC_Frame_802_15_4_PAN_ID_DST_Set(MAC_Frame_802_15_4_t* frame, PAN_ID_DST_t dst);
uint8_t MAC_Frame_802_15_4_ADDR_DST_Set(MAC_Frame_802_15_4_t* frame, ADDR_DST_t dst);
// uint8_t MAC_Frame_802_15_4_PAN_ID_SRC_Set(MAC_Frame_802_15_4_t* frame, PAN_ID_SRC_t src);
uint8_t MAC_Frame_802_15_4_ADDR_SRC_Set(MAC_Frame_802_15_4_t* frame, ADDR_SRC_t src);
uint8_t MAC_Frame_802_15_4_SeqNumUpdate(MAC_Frame_802_15_4_t* frame);

uint8_t MAC_Frame_802_15_4_SeqNumGet(MAC_Frame_802_15_4_t* frame, uint8_t* seq);
uint8_t MAC_Frame_802_15_4_PAN_ID_DST_Get(MAC_Frame_802_15_4_t* frame, PAN_ID_DST_t* dst);
uint8_t MAC_Frame_802_15_4_ADDR_DST_Get(MAC_Frame_802_15_4_t* frame, ADDR_DST_t* dst);
// uint8_t MAC_Frame_802_15_4_PAN_ID_SRC_Get(MAC_Frame_802_15_4_t* frame, PAN_ID_SRC_t* src);
uint8_t MAC_Frame_802_15_4_ADDR_SRC_Get(MAC_Frame_802_15_4_t* frame, ADDR_SRC_t* src);


#endif
