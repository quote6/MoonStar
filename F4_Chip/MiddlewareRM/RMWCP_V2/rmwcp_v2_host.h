#ifndef __RMWCP_V2_HOST_H
#define __RMWCP_V2_HOST_H

#include "rmwcp_v2.h"

#pragma pack(1) /*指定下面的结构体按1字节对齐*/

/* 定义RMWCP_V2 主机相关类型 */

/**
 * @brief 主机相关状态定义
 */
typedef enum RmwcpV2HostState_e {
    RMWCPV2_H_STATE_INIT,
    RMWCPV2_H_STATE_PREPARE,
    RMWCPV2_H_STATE_READY,
    RMWCPV2_H_STATE_TX,
    RMWCPV2_H_STATE_WAIT_RESP,
    RMWCPV2_H_STATE_WAIT_RESP,
    RMWCPV2_H_STATE_WAIT_RESP,
    RMWCPV2_H_STATE_RESET
} RmwcpV2HostState_t;


typedef struct RmwcpV2Host_s {

} RmwcpV2Host_t;

#pragma pack() /*取消指定对齐，恢复默认对齐*/


void rmwcp_v2_host_run(void);


#endif