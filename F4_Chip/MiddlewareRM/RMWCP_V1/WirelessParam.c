#include "WirelessParam.h"
#include "QueueRM.h"

// /* 定义无线通信对象信息表 */
// WirelessObjInfo_t wirelessObjInfoTable[WIRELESS_OBJ_NUM] = {
//     /* 根据不同对象地址实现主机与多个从机通信 */
//     #ifdef WIRELESS_SLAVE_0
//     {
//         .sourceAddress = 0x02,
//         .targetAddress = 0x01
//     },
//     #elif defined WIRELESS_SLAVE_1
//     {
//         .sourceAddress = 0x03,
//         .targetAddress = 0x01
//     },
//     #elif defined WIRELESS_SLAVE_2
//     {
//         .sourceAddress = 0x04,
//         .targetAddress = 0x01
//     },
//     #elif defined WIRELESS_SLAVE_3
//     {
//         .sourceAddress = 0x05,
//         .targetAddress = 0x01
//     },
//     #else
//         #error "Please define the slave number!!!"
//     #endif
// };

/* 定义无线通信对象信息表 */
WirelessObjInfo_t wirelessObjInfoTable[4] = {
    /* 根据不同对象地址实现主机与多个从机通信 */
    {
        .sourceAddress = 0x02,
        .targetAddress = 0x01
    },
    {
        .sourceAddress = 0x03,
        .targetAddress = 0x01
    },
    {
        .sourceAddress = 0x04,
        .targetAddress = 0x01
    },
    {
        .sourceAddress = 0x05,
        .targetAddress = 0x01
    }
};

/* 定义无线发送队列 */
QUEUE_RM_CREATE_STATIC_QUEUE(wirelessTx_Obj_0, 13, 1)
// QUEUE_RM_CREATE_STATIC_QUEUE(wirelessTx_Obj_1, 13, 1)
// QUEUE_RM_CREATE_STATIC_QUEUE(wirelessTx_Obj_2, 13, 1)
// QUEUE_RM_CREATE_STATIC_QUEUE(wirelessTx_Obj_3, 13, 1)

/* 定义无线接收队列 */
QUEUE_RM_CREATE_STATIC_QUEUE(wirelessRx_Obj_0, 13, 1)
// QUEUE_RM_CREATE_STATIC_QUEUE(wirelessRx_Obj_1, 13, 1)
// QUEUE_RM_CREATE_STATIC_QUEUE(wirelessRx_Obj_2, 13, 1)
// QUEUE_RM_CREATE_STATIC_QUEUE(wirelessRx_Obj_3, 13, 1)


void WirelessObjInfoTableDeInit(void){
    for(uint8_t i = 0; i < WIRELESS_OBJ_NUM; i++){
        WirelessObjInfoTablePtrConfig((WirelessObj_t) i, &wirelessObjInfoTable[i]);
    }
}

void WirelessObjInfoTableConfig(uint8_t slaveNum) {
    WirelessObjInfoTablePtrConfig(WIRELESS_OBJ_0, &wirelessObjInfoTable[slaveNum]);
}

void WirelessTxQueueDeInit(void) {
    wirelessTxQueuePtr[WIRELESS_OBJ_0] = &QUEUE_RM_GET_STATIC_QUEUE_NAME(wirelessTx_Obj_0);
}

void WirelessRxQueueDeInit(void) {
    wirelessRxQueuePtr[WIRELESS_OBJ_0] = &QUEUE_RM_GET_STATIC_QUEUE_NAME(wirelessRx_Obj_0);
}