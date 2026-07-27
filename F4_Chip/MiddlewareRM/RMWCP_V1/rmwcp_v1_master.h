#ifndef __RMWCP_V1_MASTER_H
#define __RMWCP_V1_MASTER_H

#include "rmwcp_v1.h"
#include "QueueRM.h"

/* 是否使能RMWCP_V1_MST的注册获取计数函数功能 */
#define USE_RMWCP_V1_MST_REGISTER_GET_TICK 0U
/* 是否使能RMWCP_V1_MST的注册接收函数功能 */
#define USE_RMWCP_V1_MST_REGISTER_RECEIVE  1U
/* 是否使能RMWCP_V1_MST的注册发送函数功能 */
#define USE_RMWCP_V1_MST_REGISTER_TRANSMIT 1U

/* 定义主机支持从机最大数量（不能超过4） */
#define RMWCP_V1_MST_SUPP_SLV_NUM_MAX      4


#pragma pack(1) /*指定下面的结构体按1字节对齐*/

/* 定义RMWCP_V1 主机相关类型 */

/**
 * @brief 主机相关状态定义
 */
typedef enum RmwcpV1MstState_e {
    RMWCP_V1_M_STATE_INIT,      /**< 初始状态 */
    RMWCP_V1_M_STATE_SCAN_PRE,  /**< 扫描准备 */
    RMWCP_V1_M_STATE_SCAN_SEND, /**< 扫描发送 */
    RMWCP_V1_M_STATE_SCAN_WAIT, /**< 扫描等待 */
    RMWCP_V1_M_STATE_SCAN_NEXT, /**< 扫描下一个 */
    RMWCP_V1_M_STATE_READY,     /**< 就绪状态 */
    RMWCP_V1_M_STATE_WORK_PRE,  /**< 发送状态 */
    RMWCP_V1_M_STATE_WORK_SEND, /**< 发送状态 */
    RMWCP_V1_M_STATE_WORK_WAIT, /**< 发送状态 */
    RMWCP_V1_M_STATE_WORK_WAIT, /**< 发送状态 */
    RMWCP_V1_M_STATE_RX,        /**< 接收状态 */
    RMWCP_V1_M_STATE_RX_TO,     /**< 等待超时 */
    RMWCP_V1_M_STATE_HANDLE,    /**< 处理数据包 */
    RMWCP_V1_M_STATE_RESET      /**< 复位状态 */
} RmwcpV1MstState_t;

typedef enum {
    RMWCP_V1_M_EVT_RX_OK,  /**< 接收成功 */
    RMWCP_V1_M_EVT_RX_TO,  /**< 接收超时 */
    RMWCP_V1_M_EVT_EX_ERR, /**< 接收失败 */
} RmwcpV1MstEvt_t;

/**
 * @brief 定义主机记录结构体
 *
 */
typedef struct {
    // uint16_t pack_id;            /**< 数据包ID */
    // uint16_t ack_id;             /**< 应答号 */
    // uint8_t addr_dst;            /**< 目标地址 */
    // uint8_t addr_src;            /**< 源地址 */
    // uint16_t len;                /**< 数据包长度 */
    // RmwcpV1Status_ut stat;       /**< 状态信息 */
    // RmwcpV1Ctrl_ut ctrl;         /**< 控制信息 */
    RmwcpV1Hdr_t hdr;      /**< 帧头信息 */
    uint8_t cont_tout_cnt; /**< 连续超时计数 */
    // uint8_t is_valid;            /**< 该项记录是否有效 */
} RmwcpV1MstRecord_t;

/**
 * @brief 定义主机扫描从机地址范围
 */
typedef struct {
    uint8_t lower;
    uint8_t upper;
} RmwcpV1ScanRange_t;

typedef struct {
    uint8_t addr;                  /**< 主机地址 */
    uint16_t pld_len_max;          /**< 最大载荷（不能超过 RMWCP_V1_PACK_PAYLOAD_LEN_MAX ） */
    uint8_t scan_cnt_max;          /**< 主机最大扫描次数 */
    RmwcpV1ScanRange_t scan_range; /**< 主机扫描从机地址范围 */
    uint8_t tol_cons_to_cnt_max;   /**< 主机容忍最大连续超时次数 */
} RmwcpV1MstCfg_t;

typedef struct {
    RmwcpV1MstState_t state;                                     /**< 主机状态机变量 */
    RmwcpV1MstEvt_t event;                                       /**< 主机事件 */
    uint8_t cur_slv_num;                                         /**< 当前从机个数（不能超过 RMWCP_V1_MST_SUPP_SLV_NUM_MAX ） */
    uint8_t cur_slv_index;                                       /**< 当前从机对象索引号 */
    uint8_t addr_slv[RMWCP_V1_MST_SUPP_SLV_NUM_MAX];             /**< 记录的从机地址 */
    uint32_t acc_tx_len[RMWCP_V1_MST_SUPP_SLV_NUM_MAX];          /**< 累积发送数据包长度（用于调试） */
    uint32_t acc_rx_len[RMWCP_V1_MST_SUPP_SLV_NUM_MAX];          /**< 累积接收数据包长度（用于调试） */
    RmwcpV1MstRecord_t rx_record[RMWCP_V1_MST_SUPP_SLV_NUM_MAX]; /**< 接收从机信息记录 */
    RmwcpV1MstRecord_t tx_record[RMWCP_V1_MST_SUPP_SLV_NUM_MAX]; /**< 发送从机信息记录 */
} RmwcpV1MstCtx_t;

typedef struct {
    RmwcpV1Pack_ut tx_pack;                             /**< 发送数据包 */
    RmwcpV1Pack_ut rx_pack;                             /**< 接收数据包 */
    uint16_t tx_len;                                    /**< 发送数据包长度（单次） */
    uint16_t rx_len;                                    /**< 接收数据包长度（单次） */
    QueueRM_t* rx_queue[RMWCP_V1_MST_SUPP_SLV_NUM_MAX]; /**< 接收队列 */
    QueueRM_t* tx_queue[RMWCP_V1_MST_SUPP_SLV_NUM_MAX]; /**< 发送队列 */
} RmwcpV1MstData_t;

typedef struct {
#if (USE_RMWCP_V1_MST_REGISTER_GET_TICK == 1U)
    /* 获取Tick值函数 */
    uint32_t (*get_tick)(void);
#endif /* USE_RMWCP_V1_MST_REGISTER_GET_TICK */
#if (USE_RMWCP_V1_MST_REGISTER_RECEIVE == 1U)
    /* 获取Tick值函数 */
    uint8_t (*receive)(RmwcpV1Mst_t* const mst, uint32_t timeout);
#endif /* USE_RMWCP_V1_MST_REGISTER_RECEIVE */
#if (USE_RMWCP_V1_MST_REGISTER_TRANSMIT == 1U)
    /* 获取Tick值函数 */
    uint8_t (*transmit)(RmwcpV1Mst_t* const mst, uint32_t timeout);
#endif /* USE_RMWCP_V1_MST_REGISTER_TRANSMIT */
} RmwcpV1MstOps_t;


typedef struct RmwcpV2Mst_s {
    RmwcpV1MstCfg_t cfg;   /* 配置 */
    RmwcpV1MstCtx_t ctx;   /* 上下文 */
    RmwcpV1MstData_t data; /* 数据 */
    RmwcpV1MstOps_t ops;   /* 操作 */
} RmwcpV1Mst_t;

#pragma pack() /*取消指定对齐，恢复默认对齐*/

void rmwcp_v1_master_run(RmwcpV1Mst_t* mst); /* 主机状态机运行函数 */

#endif