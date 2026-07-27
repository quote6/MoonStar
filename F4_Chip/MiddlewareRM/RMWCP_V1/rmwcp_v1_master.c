#include "rmwcp_v1_master.h"

#include <string.h>

/* 定义主机无线数据包最大包长默认值 */
#define RMWCP_V1_MST_PLD_LEN_MAX_DEF         1000

/* 定义主机扫描从机地址范围默认值 */
#define RMWCP_V1_MST_SCAN_RANGE_LOWER_DEF    0xF0
#define RMWCP_V1_MST_SCAN_RANGE_UPPER_DEF    0xFF

/* 定义主机扫描从机地址最大次数默认值 */
#define RMWCP_V1_MST_SCAN_CNT_MAX_DEF        3

/* 定义主机容忍最大连续超时次数默认值 */
#define RMWCP_V1_MST_TOL_CONS_TO_CNT_MAX_DEF 3

/* 定义主机无线数据包最大包长（不能超过RMWCP_V2_PACK_LEN_MAX） */


/* 定义主机扫描从机地址范围 */


/* 定义主机扫描从机地址最大次数 */


/* 定义主机允许的最大连续超时次数 */


/* 定义当前从机数 */

/* 定义当前从机索引值 */


/* 定义主机发送数据包 */


/* 定义主机接收数据包 */


/* 定义无线发送长度（单次） */


/* 定义无线接收长度（单次） */


/* 定义无线累积发送长度（用于调试） */

/* 定义无线累积接收长度（调试） */


/**
 * @brief 主机数据包最大载荷值恢复为默认值
 * @param mst 主机指针
 */
void rmwcp_v1_mst_pld_len_max_restore(RmwcpV1Mst_t* const mst) {
    mst->cfg.pld_len_max = RMWCP_V1_MST_PLD_LEN_MAX_DEF;
};

/**
 * @brief 设置主机数据包最大载荷值
 * @param mst 主机指针
 * @param len 设置的长度（不能超过 RMWCP_V1_PACK_PAYLOAD_LEN_MAX 设定的值）
 */
void rmwcp_v1_mst_pld_len_max_set(RmwcpV1Mst_t* const mst, uint16_t len) {
    mst->cfg.pld_len_max = len;
};

/**
 * @brief 主机扫描从机地址范围恢复为默认值
 * @param mst 主机指针
 */
void rmwcp_v1_mst_scan_range_restore(RmwcpV1Mst_t* const mst) {
    mst->cfg.scan_range.lower = RMWCP_V1_MST_SCAN_RANGE_LOWER_DEF;
    mst->cfg.scan_range.upper = RMWCP_V1_MST_SCAN_RANGE_UPPER_DEF;
}

/**
 * @brief 设置主机扫描从机地址范围
 * @param mst 主机指针
 * @param lower 下界
 * @param upper 上界
 */
void rmwcp_V1_mst_scan_range_set(RmwcpV1Mst_t* const mst, uint8_t lower, uint8_t upper) {
    mst->cfg.scan_range.lower = lower;
    mst->cfg.scan_range.upper = upper;
}

/**
 * @brief 主机扫描从机地址最大次数恢复为默认值
 * @param mst 主机指针
 */
void rmwcp_v1_mst_scan_cnt_max_restore(RmwcpV1Mst_t* const mst) {
    mst->cfg.scan_cnt_max = RMWCP_V1_MST_SCAN_CNT_MAX_DEF;
};

/**
 * @brief 主机扫描从机地址最大次数设置
 * @param mst 主机指针
 * @param cnt 最大扫描次数
 */
void rmwcp_v1_mst_scan_cnt_max_set(RmwcpV1Mst_t* const mst, uint8_t cnt) {
    mst->cfg.scan_cnt_max = cnt;
};

/**
 * @brief 主机容忍最大连续超时次数恢复为默认值
 * @param mst 主机指针
 */
void rmwcp_v1_mst_tol_cons_to_cnt_max_restore(RmwcpV1Mst_t* const mst) {
    mst->cfg.tol_cons_to_cnt_max = RMWCP_V1_MST_TOL_CONS_TO_CNT_MAX_DEF;
};

/**
 * @brief 主机容忍最大连续超时次数设置
 * @param mst 主机指针
 * @param cnt 最大连续超时次数
 */
void rmwcp_v1_mst_tol_cons_to_cnt_max_set(RmwcpV1Mst_t* const mst, uint8_t cnt) {
    mst->cfg.tol_cons_to_cnt_max = cnt;
};

uint8_t rmwcp_v1_mst_records_clear(RmwcpV1Mst_t* const mst) {
    memset(mst->ctx.tx_record, 0, sizeof(mst->ctx.tx_record));
    memset(mst->ctx.rx_record, 0, sizeof(mst->ctx.rx_record));
}

uint8_t rmwcp_v1_mst_addr_slv_clear(RmwcpV1Mst_t* const mst) {
    memset(mst->ctx.addr_slv, 0, sizeof(mst->ctx.addr_slv));
}

/**
 * @brief 查询从机地址（在从机地址记录表中查询）
 * @param mst 主机指针
 * @param slv 从机地址（不允许为0）
 * @return uint8_t
 */
int8_t rmwcp_v1_mst_addr_slv_query(RmwcpV1Mst_t* const mst, uint8_t slv) {
    /* 主机指针有误 */
    if (mst == NULL) {
        return -2;
    } else if (slv == 0) {
        return -3;
    }
    /* 当前地址为空 */
    if (mst->ctx.cur_slv_num == 0) {
        return -4;
    }
    for (uint8_t i = 0; i < mst->ctx.cur_slv_num; i++) {
        if (slv == mst->ctx.addr_slv[i]) {
            return i;
        }
    }
    return -1;
}

uint8_t rmwcp_v1_mst_calling_resp_verify(RmwcpV1Mst_t* const mst) {
    /* 如果接收到的数据包的地址对应不上说明接收到的数据有误 */
    if ((mst->data.rx_pack.parts.hdr.addr_src != mst->data.tx_pack.parts.hdr.addr_dst) ||
        (mst->data.rx_pack.parts.hdr.addr_dst != mst->data.tx_pack.parts.hdr.addr_src)) {
        return 1;
    }
    /* 如果数据包类型不是叫号包，则返回错误 */
    else if (!mst->data.rx_pack.parts.hdr.stat.bits.call_resp) {
        return 2;
    }
    /* 如果接收到的 ack_id 不等于发送的 pack_id + len，说明接收到的数据有误 */
    else if (mst->data.rx_pack.parts.hdr.ack_id !=
             mst->data.tx_pack.parts.hdr.pack_id + mst->data.tx_pack.parts.hdr.len) {
        return 3;
    }
    return 0;
}

/**
 * @brief 数据包校验函数
 * @param mst 主机指针
 * @return uint8_t
 */
uint8_t rmwcp_v1_mst_rx_pack_verify(RmwcpV1Mst_t* const mst) {
    /* 如果不是接收到的数据包不是发送给该主机 */
    if (mst->data.rx_pack.parts.hdr.addr_dst != mst->cfg.addr) {
        return 1;
    }
    /* 根据接收到的数据包里的源地址查询从机地址记录表里对应的索引 */
    int8_t index = rmwcp_v1_mst_addr_slv_query(mst, mst->data.rx_pack.parts.hdr.addr_src);
    /* 如果没有从从机地址记录表中查找到目标从机则返回错误 */
    if (index < 0) {
        return 2;
    }
    // /* 如果接收到的数据包的地址对应不上说明接收到的数据有误 */
    // else if ((mst->data.rx_pack.parts.hdr.addr_src != mst->ctx.tx_record[index].addr_dst) ||
    //          (mst->data.rx_pack.parts.hdr.addr_dst != mst->ctx.tx_record[index].addr_src)) {
    //     return 3;
    // }
    /* 更新接收记录表 */
    mst->ctx.rx_record[index].hdr = mst->data.rx_pack.parts.hdr;
    /* 如果接收到的数据包里有叫号命令 */
    if (mst->data.rx_pack.parts.hdr.ctrl.bits.calling) {
        mst->ctx.tx_record[index].hdr.stat.bits.call_err; /* 因为只有主机可以叫号，从机不得发送叫号命令 */
    }
    /* 如果接收到的数据包有复位命令 */
    if (mst->data.rx_pack.parts.hdr.ctrl.bits.rst_cmd) {
        mst->ctx.tx_record[index].hdr.stat.bits.reseted = 1; /* 告知对方已按要求复位 */
    }
    /* 如果接收到的 ack_id 等于发送的 pack_id + len，说明从机已正确接收到的数据 */
    if (mst->data.rx_pack.parts.hdr.ack_id !=
        mst->data.tx_pack.parts.hdr.pack_id + mst->data.tx_pack.parts.hdr.len) {
    }
    return 0;
}

/**
 * @brief 记录从机地址
 * @param mst 主机指针
 * @return uint8_t 已记录的从机数量
 */
uint8_t rmwcp_v1_mst_addr_slv_record(RmwcpV1Mst_t* const mst) {
    if (mst->ctx.cur_slv_num == RMWCP_V1_MST_SUPP_SLV_NUM_MAX) {
        return RMWCP_V1_MST_SUPP_SLV_NUM_MAX; /* 从机地址记录已满，不再记录 */
    }
    /* 遍历从机地址记录表（判断是否记录该从机地址，有则跳过） */
    for (uint8_t i = 0; i < mst->ctx.cur_slv_num; i++) {
        /* 如果该表有空项，直接记录该地址（因为总是从表的开头开始遍历，空项后面一定都是空的） */
        if (mst->ctx.addr_slv[i] == 0x00) {
            mst->ctx.addr_slv[i] = mst->data.rx_pack.parts.hdr.addr_src;
            mst->ctx.cur_slv_num++;
            break;
        }
        /* 如果有非空项，则判断地址是否与其待记录的地址相同 */
        else if (mst->data.rx_pack.parts.hdr.addr_src == mst->ctx.addr_slv[i]) {
            break; /* 从机地址已记录，直接返回 */
        }
        /* 如果不相等就接着遍历 */
    }
    return mst->ctx.cur_slv_num; /* 返回已经记录的从机数量 */
}

/**
 * @brief 从机地址扫描
 * @param mst 主机指针
 * @return uint8_t 扫描到的从机数
 */
uint8_t rmwcp_v1_mst_scan_slv(RmwcpV1Mst_t* const mst) {
    /* 清空从机地址记录表 */
    rmwcp_v1_mst_addr_slv_clear(mst);
    /* 设置发送数据包 */
    mst->data.tx_pack.parts.hdr.pack_id = 0;              /* 包ID固定为0 */
    mst->data.tx_pack.parts.hdr.ack_id = 0;               /* 应答号固定为0 */
    mst->data.tx_pack.parts.hdr.addr_src = mst->cfg.addr; /* 主机地址 */
    mst->data.tx_pack.parts.hdr.ctrl.bits.calling = 1;    /* 叫号包 */
    mst->data.tx_pack.parts.hdr.len = RMWCP_V1_HDR_LEN;   /* 只发送头部长度（即不带有效载荷） */
    mst->ctx.cur_slv_index = 0;                           /* 当前从机索引设置为0 */
    mst->ctx.cur_slv_num = 0;                             /* 从机数量设置为0 */
    /* 开始叫号 */
    for (uint8_t i = 0; i < mst->cfg.scan_cnt_max; i++) {
        for (uint8_t j = mst->cfg.scan_range.lower; j < mst->cfg.scan_range.upper; j++) {
            mst->data.tx_pack.parts.hdr.addr_dst = j;
#if (USE_RMWCP_V1_MST_REGISTER_TRANSMIT == 1U)
            /* 发送数据包 */
            if (mst->ops.transmit(mst, 5)) {
                continue; /* 如果不为0说明发送失败，跳过本次发送，进行下一次发送 */
            }
#else  /* USE_RMWCP_V1_MST_REGISTER_TRANSMIT */
            if (rmwcp_v1_mst_transmit(mst, 5)) {
                continue; /* 如果不为0说明发送失败，跳过本次发送，进行下一次发送 */
            }
#endif /* USE_RMWCP_V1_MST_REGISTER_TRANSMIT */
#if (USE_RMWCP_V1_MST_REGISTER_RECEIVE == 1U)
            /* 接收应答 */
            if (mst->ops.receive(mst, 5)) {
                continue; /* 如果不为0说明发送失败，跳过本次接收，进行下一次发送 */
            }
#else  /* USE_RMWCP_V1_MST_REGISTER_RECEIVE */
            if (rmwcp_v1_mst_receive(mst, 5)) {
                continue;
            }
#endif /* USE_RMWCP_V1_MST_REGISTER_RECEIVE */
            /* 校验叫号应答 */
            if (rmwcp_v1_mst_calling_resp_verify(mst)) {
                continue; /* 如果校验失败，则跳过后续操作，进行下一次发送 */
            }
            /* 解析并记录从机地址数据 */
            if (rmwcp_v1_mst_addr_slv_record(mst) == RMWCP_V1_MST_SUPP_SLV_NUM_MAX) {
                break; /* 如果记录表已满，退出叫号 */
            }
        }
    }
    return mst->ctx.cur_slv_num; /* 返回扫描到的从机个数 */
}


uint8_t rmwcp_v1_mst_tx_pack_load(RmwcpV1Mst_t* const mst) {
    /* 根据发送信息包记录的信息
        主机与对应从机在上一轮的通信过程中
        如果没有接收到重复数据包以及没有无应答发生
        则发送新的数据包
        否则重发上一轮给对应从机的数据包
     */
    if (!mst->ctx.tx_record[mst->ctx.cur_slv_index].stat.bits.no_resp &&
        !mst->ctx.tx_record[mst->ctx.cur_slv_index].stat.bits.repet_pack_id) {
        uint16_t len = QueueRM_GetLength(mst->data.tx_queue[mst->ctx.cur_slv_index]);
        /* 限制包长 */
        if (len > mst->cfg.pld_len_max) {
            len = mst->cfg.pld_len_max;
        }
        /* 设置本次发送无线长度 */
        // mst->data.tx_len = len + RMWCP_V1_HDR_LEN;
        // /* 设置应答号 */
        // mst->data.tx_pack.parts.hdr.ack_id =
        //     mst->ctx.rx_record[mst->ctx.cur_slv_index].pack_id +
        //     mst->ctx.rx_record[mst->ctx.cur_slv_index].len;
        // /* 设置包ID */
        // mst->data.tx_pack.parts.hdr.pack_id = mst->ctx.rx_record[mst->ctx.cur_slv_index].ack_id;

        /* 更新发送记录表 */
        mst->ctx.tx_record[mst->ctx.cur_slv_index].len = len + RMWCP_V1_HDR_LEN;
    }


    QueueRM_Peep(mst->data.tx_queue[mst->ctx.cur_slv_index], mst->data.tx_pack.parts.payload, )
}

inline void rmwcp_v1_master_run(RmwcpV1Mst_t* const mst) {

    if (mst->ctx.state == RMWCP_V1_M_STATE_INIT) {

    }
    /* 准备状态，进行从机扫描 */
    else if (mst->ctx.state == RMWCP_V1_M_STATE_CALL) {
        /* 如果扫描到从机地址 */
        if (rmwcp_v1_mst_scan_slv(mst)) {
            mst->ctx.cur_slv_index = 0; /* 索引设置为0（准备后续与从机依次通信） */
            mst->ctx.state = RMWCP_V1_M_STATE_READY;
        }
        /* 否则保持该状态 */

    }
    switch (mst->ctx.state) {
        case RMWCP_V1_M_STATE_INIT:
            /* code */
            break;

        case RMWCP_V1_M_STATE_SCAN_PRE:
            /* code */
            break;

        case RMWCP_V1_M_STATE_SCAN_SEND:
            /* code */
            break;

        case RMWCP_V1_M_STATE_SCAN_WAIT:
            /* code */
            break;

        case RMWCP_V1_M_STATE_SCAN_NEXT:
            /* code */
            break;

        case RMWCP_V1_M_STATE_WORK_PRE:
            /* code */
            break;
        case RMWCP_V1_M_STATE_WORK_SEND:
            /* code */
            break;
        case RMWCP_V1_M_STATE_WORK_WAIT:
            /* code */
            break;

        default:
            break;
    }
}

/**
 * @brief RMWCP_V1主机获取Tick值函数
 * @return uint32_t 当前tick（默认以1ms为单位）
 * @note  无
 * @attention 用户需要在自己的文件里重新定义该函数以实现正确的功能
 */
__weak uint32_t rmwcp_v1_mst_get_tick(void) {
    return 0;
}

__weak uint8_t rmwcp_v1_mst_receive(RmwcpV1Mst_t* const mst, uint32_t timeout) {
}

__weak uint8_t rmwcp_v1_mst_transmit(RmwcpV1Mst_t* const mst, uint32_t timeout) {
}