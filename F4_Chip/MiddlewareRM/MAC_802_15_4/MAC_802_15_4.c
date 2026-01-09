#include "MAC_802_15_4.h"

static inline void MyMemoryClear(char* ptr, uint16_t len) {
    while(len--){
        *(ptr++) = 0;
    }
}

static const FrameControl_ut FRAME_CONTROL_TEMPLATE = {
    .framecontrol.frameType = FRAME_TYPE_DATA, /* 默认为数据帧 */
    .framecontrol.securityEnable = SECURITY_ENABLE_NO_PROTECTED, /* 安全使能禁用 */
    .framecontrol.framePending = FRAME_PENDING_NO_MORE_DATA, /* 无数据等待（因为当前帧类型是数据帧） */
    .framecontrol.ACK_Request = ACK_REQUEST_NO_REQUIRED, /* 不请求应答 */
    .framecontrol.PAN_ID_Compress = PAN_ID_COMPRESS_DEST_EXIST_SOURCE_NOT, /* 使用PAN ID压缩 */
    .framecontrol.reserved = 0,/* 保留字段，设置为全0 */
    .framecontrol.destinationAddressMode = DEST_ADDR_MODE_SHORT_ADDR_16_BITS, /* 目标地址长度16位 */
    .framecontrol.frameVersion = FRAME_VERSION, /* 帧版本（当前项目只能设置为0x01） */
    .framecontrol.sourceAddressMode = SRC_ADDR_MODE_SHORT_ADDR_16_BITS /* 源地址长度16位 */
};


/**
 * @brief MAC帧初始化（IEEE802.15.4）
 * @param[in] frame  MAC帧
 * @return uint8_t 函数执行结果
 *  @arg 0: 成功
 *  @arg 其他: 失败
 */
uint8_t MAC_Frame_802_15_4_Init(MAC_Frame_802_15_4_t* frame){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL){
        return 1;
    }
    /* 给结构体参数赋初值 */
    /* 按预先定义好的模板给 帧控制 字段赋值 */
    frame->header.mhr.frameControl = FRAME_CONTROL_TEMPLATE;
    /* 序列号设置为0 */
    frame->header.mhr.sequenceNumber = 0;
    /* 目标PAN ID设置为0，待后续应用进行设置 */
    MyMemoryClear((char*) &frame->header.mhr.PAN_ID_Destinatnion, DST_ADDR_LENGTH);
    /* 目标地址设置为0，待后续应用进行设置 */
    MyMemoryClear((char*) &frame->header.mhr.ADDR_Destination, DST_ADDR_LENGTH);
    /* 源地址设置为0，待后续应用进行设置 */
    MyMemoryClear((char*) &frame->header.mhr.ADDR_Source, SRC_ADDR_LENGTH);
    /* 载荷指针设置为空，待后续应用进行设置 */
    frame->payload = NULL;
    /* 初始化成功返回0 */
    return 0;
}

uint8_t MAC_Frame_Control_AR_ENABLE(MAC_Frame_802_15_4_t* frame){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL){
        return 1;
    }
    MAC_FRAME_CONTROL_AR_ENABLE(frame);
    return 0;
}

uint8_t MAC_Frame_Control_AR_DISABLE(MAC_Frame_802_15_4_t* frame){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL){
        return 1;
    }
    MAC_FRAME_CONTROL_AR_DISABLE(frame);
    return 0;    
}

uint8_t MAC_Frame_802_15_4_SeqNumSet(MAC_Frame_802_15_4_t* frame, uint8_t seq){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL){
        return 1;
    }
    MAC_FRAME_802_15_4_SEQ_NUM_SET(frame, seq);
    return 0;
}

uint8_t MAC_Frame_802_15_4_PAN_ID_DST_Set(MAC_Frame_802_15_4_t* frame, PAN_ID_DST_t dst){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL){
        return 1;
    }
    MAC_FRAME_802_15_4_PAN_ID_DST_SET(frame, dst);
    return 0;
}

uint8_t MAC_Frame_802_15_4_ADDR_DST_Set(MAC_Frame_802_15_4_t* frame, ADDR_DST_t dst){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL){
        return 1;
    }
    MAC_FRAME_802_15_4_ADDR_DST_SET(frame, dst);
    return 0;
}

uint8_t MAC_Frame_802_15_4_ADDR_SRC_Set(MAC_Frame_802_15_4_t* frame, ADDR_SRC_t src){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL){
        return 1;
    }
    MAC_FRAME_802_15_4_ADDR_SRC_SET(frame, src);
    return 0;
}

uint8_t MAC_Frame_802_15_4_SeqNumUpdate(MAC_Frame_802_15_4_t* frame){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL){
        return 1;
    }
    MAC_FRAME_802_15_4_SEQ_NUM_UPDATE(frame);
    return 0;
}

uint8_t MAC_Frame_802_15_4_SeqNumGet(MAC_Frame_802_15_4_t* frame, uint8_t* seq){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL || seq == NULL){
        return 1;
    }
    MAC_FRAME_802_15_4_SEQ_NUM_GET(frame, seq);
    return 0;
}

uint8_t MAC_Frame_802_15_4_PAN_ID_DST_Get(MAC_Frame_802_15_4_t* frame, PAN_ID_DST_t* dst){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL || dst == NULL){
        return 1;
    }
    MAC_FRAME_802_15_4_PAN_ID_DST_GET(frame, dst);
    return 0;
}

uint8_t MAC_Frame_802_15_4_ADDR_DST_Get(MAC_Frame_802_15_4_t* frame, ADDR_DST_t* dst){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL || dst == NULL){
        return 1;
    }
    MAC_FRAME_802_15_4_ADDR_DST_GET(frame, dst);
    return 0;
}

uint8_t MAC_Frame_802_15_4_ADDR_SRC_Get(MAC_Frame_802_15_4_t* frame, ADDR_SRC_t* src){
    /* 对传入参数进行检查，如果为空，直接返回 */
    if(frame == NULL || src == NULL){
        return 1;
    }
    MAC_FRAME_802_15_4_ADDR_SRC_GET(frame, src);
    return 0;
}
