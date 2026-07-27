#include "DWM1000Param.h"
#include "deca_device_api.h"

#define DWT_CONFIG_TEMPLATE_NUM 8

/* dwt外设模板（简化配置，提供预先一套配置好的模板供选择） */
/**
 * @brief DW1000配置模板
 * @note  提供预先一套配置好的模板供选择，简化配置
 * @attention 在默认初始化中，默认选择 编号3（编号从0开始）的模板配置DW1000
 */
static dwt_config_t dwtConfigTemplate[DWT_CONFIG_TEMPLATE_NUM] = {
    { .chan = 1,                      /* Channel number. */
      .prf = DWT_PRF_16M,             /* Pulse repetition frequency. */
      .txPreambLength = DWT_PLEN_256, /* Preamble length. Used in TX only. */
      .rxPAC = DWT_PAC16,             /* Preamble acquisition chunk size. Used in RX only. */
      .txCode = 1,                    /* TX preamble code. Used in TX only. */
      .rxCode = 1,                    /* RX preamble code. Used in RX only. */
      .nsSFD = 1,                     /* 0 to use standard SFD, 1 to use non-standard SFD. */
      .dataRate = DWT_BR_6M8,         /* Data rate. */
      .phrMode = DWT_PHRMODE_EXT,     /* PHY header mode. */
      .sfdTO = (257 + 64 - 16) },     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */

    { .chan = 1,                      /* Channel number. */
      .prf = DWT_PRF_64M,             /* Pulse repetition frequency. */
      .txPreambLength = DWT_PLEN_256, /* Preamble length. Used in TX only. */
      .rxPAC = DWT_PAC16,             /* Preamble acquisition chunk size. Used in RX only. */
      .txCode = 9,                    /* TX preamble code. Used in TX only. */
      .rxCode = 9,                    /* RX preamble code. Used in RX only. */
      .nsSFD = 1,                     /* 0 to use standard SFD, 1 to use non-standard SFD. */
      .dataRate = DWT_BR_6M8,         /* Data rate. */
      .phrMode = DWT_PHRMODE_EXT,     /* PHY header mode. */
      .sfdTO = (257 + 64 - 16) },     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */

    { .chan = 2,                      /* Channel number. */
      .prf = DWT_PRF_16M,             /* Pulse repetition frequency. */
      .txPreambLength = DWT_PLEN_256, /* Preamble length. Used in TX only. */
      .rxPAC = DWT_PAC16,             /* Preamble acquisition chunk size. Used in RX only. */
      .txCode = 3,                    /* TX preamble code. Used in TX only. */
      .rxCode = 3,                    /* RX preamble code. Used in RX only. */
      .nsSFD = 1,                     /* 0 to use standard SFD, 1 to use non-standard SFD. */
      .dataRate = DWT_BR_6M8,         /* Data rate. */
      .phrMode = DWT_PHRMODE_EXT,     /* PHY header mode. */
      .sfdTO = (257 + 64 - 16) },     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */

    { .chan = 2,                     /* Channel number. */
      .prf = DWT_PRF_64M,            /* Pulse repetition frequency. */
      .txPreambLength = DWT_PLEN_64, /* Preamble length. Used in TX only. */
      .rxPAC = DWT_PAC8,             /* Preamble acquisition chunk size. Used in RX only. */
      .txCode = 10,                  /* TX preamble code. Used in TX only. */
      .rxCode = 10,                  /* RX preamble code. Used in RX only. */
      .nsSFD = 1,                    /* 0 to use standard SFD, 1 to use non-standard SFD. */
      .dataRate = DWT_BR_6M8,        /* Data rate. */
      .phrMode = DWT_PHRMODE_EXT,    /* PHY header mode. */
      .sfdTO = (65) },               /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */

    { .chan = 3,                      /* Channel number. */
      .prf = DWT_PRF_16M,             /* Pulse repetition frequency. */
      .txPreambLength = DWT_PLEN_256, /* Preamble length. Used in TX only. */
      .rxPAC = DWT_PAC16,             /* Preamble acquisition chunk size. Used in RX only. */
      .txCode = 5,                    /* TX preamble code. Used in TX only. */
      .rxCode = 5,                    /* RX preamble code. Used in RX only. */
      .nsSFD = 1,                     /* 0 to use standard SFD, 1 to use non-standard SFD. */
      .dataRate = DWT_BR_6M8,         /* Data rate. */
      .phrMode = DWT_PHRMODE_EXT,     /* PHY header mode. */
      .sfdTO = (257 + 64 - 16) },     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */

    { .chan = 3,                      /* Channel number. */
      .prf = DWT_PRF_64M,             /* Pulse repetition frequency. */
      .txPreambLength = DWT_PLEN_256, /* Preamble length. Used in TX only. */
      .rxPAC = DWT_PAC16,             /* Preamble acquisition chunk size. Used in RX only. */
      .txCode = 11,                   /* TX preamble code. Used in TX only. */
      .rxCode = 11,                   /* RX preamble code. Used in RX only. */
      .nsSFD = 1,                     /* 0 to use standard SFD, 1 to use non-standard SFD. */
      .dataRate = DWT_BR_6M8,         /* Data rate. */
      .phrMode = DWT_PHRMODE_EXT,     /* PHY header mode. */
      .sfdTO = (257 + 64 - 16) },     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */

    { .chan = 5,                      /* Channel number. */
      .prf = DWT_PRF_16M,             /* Pulse repetition frequency. */
      .txPreambLength = DWT_PLEN_256, /* Preamble length. Used in TX only. */
      .rxPAC = DWT_PAC16,             /* Preamble acquisition chunk size. Used in RX only. */
      .txCode = 4,                    /* TX preamble code. Used in TX only. */
      .rxCode = 4,                    /* RX preamble code. Used in RX only. */
      .nsSFD = 1,                     /* 0 to use standard SFD, 1 to use non-standard SFD. */
      .dataRate = DWT_BR_6M8,         /* Data rate. */
      .phrMode = DWT_PHRMODE_EXT,     /* PHY header mode. */
      .sfdTO = (257 + 64 - 16) },     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */

    { .chan = 5,                      /* Channel number. */
      .prf = DWT_PRF_64M,             /* Pulse repetition frequency. */
      .txPreambLength = DWT_PLEN_256, /* Preamble length. Used in TX only. */
      .rxPAC = DWT_PAC16,             /* Preamble acquisition chunk size. Used in RX only. */
      .txCode = 12,                   /* TX preamble code. Used in TX only. */
      .rxCode = 12,                   /* RX preamble code. Used in RX only. */
      .nsSFD = 1,                     /* 0 to use standard SFD, 1 to use non-standard SFD. */
      .dataRate = DWT_BR_6M8,         /* Data rate. */
      .phrMode = DWT_PHRMODE_EXT,     /* PHY header mode. */
      .sfdTO = (257 + 64 - 16) },     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};


DWM1000_Config_t DWM1000_Configuration = {
    .dwtConfiguration = &dwtConfigTemplate[3],
    /* 发送帧完成后打开接收器的延迟时间（最短6.2us） */
    .rxDelayTime = 0,
    /* 接收超时时间（在超时时间内没有接收到帧） */
    .rxRespTimeout = 3000 //2600【经过测试，这个值需要谨慎设置，目前它是引起通信故障的主要原因】
};



void DWM1000_ParamDeInit(void){
	/* 使用默认模板号3初始化DW1000 */
	DWM1000_Configuration.dwtConfiguration = &dwtConfigTemplate[3],
    /* 发送帧完成后打开接收器的延迟时间（最短6.2us） */
    DWM1000_Configuration.rxDelayTime = 0;
    /* 接收超时时间（在超时时间内没有接收到帧） */
    DWM1000_Configuration.rxRespTimeout = 3000; //2600【经过测试，这个值需要谨慎设置，目前它是引起通信故障的主要原因】
	DWM1000_ConfigurationPtrInit(&DWM1000_Configuration);
}

uint8_t DWM1000_ParamInit(uint8_t templateNum, uint32_t delayTime, uint16_t timeout) {
    uint8_t result = 0;
    /* 如果传入的模板号超过范围，则提示错误，并按默认模板号进行配置 */
    if (templateNum >= DWT_CONFIG_TEMPLATE_NUM) {
        result = 1;
		templateNum = 3;
    }
	DWM1000_Configuration.dwtConfiguration = &dwtConfigTemplate[templateNum];
	DWM1000_Configuration.rxDelayTime = delayTime;
	DWM1000_Configuration.rxRespTimeout = timeout;
	/* 按照上面配置初始化DWM1000 */
	DWM1000_ConfigurationPtrInit(&DWM1000_Configuration);
	return result;
}