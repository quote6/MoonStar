#include "rmwcp_v2_host.h"
#include "mac_802_15_4.h"

typedef struct {
    Mhr_802_15_4_t mhr;
    RmwcpV2Hdr_t rhr;
}RmwcpV2HostRecord_t;

/* 定义主机无线数据包最大包长（不能超过RMWCP_V2_PACK_LEN_MAX） */
static uint16_t RMWCP_V2_HOST_PACK_LEN_MAX = 1000;
