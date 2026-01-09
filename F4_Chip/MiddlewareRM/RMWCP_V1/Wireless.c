#include "Wireless.h"


/* 定义无线通信对象信息表指针（方便动态使用） */
WirelessObjInfo_t* wirelessObjInfoTablePtr[WIRELESS_OBJ_NUM];

/* 定义无线发送队列指针（方便动态使用） */
QueueRM_t* wirelessTxQueuePtr[WIRELESS_OBJ_NUM];

/* 定义无线接收队列指针（方便动态使用） */
QueueRM_t* wirelessRxQueuePtr[WIRELESS_OBJ_NUM];

/* 定义无线数据包最大包长(不含包头，只包含数据有效载荷长度) */
uint16_t wirelessPackMaxSize = 800;

/* 定义发送数据包 */
WirelessPack WirelessTxPack;

/* 定义接收数据包 */
WirelessPack WirelessRxPack;

/* 定义无线接收状态 */
WirelessStatus_ut wirelessStatus;
WirelessControl_ut wirelessControl;

/* 定义无线发送长度（不是总长度，该变量用于某次传输，传输完就会清除） */
uint32_t wirelessTxLength;
/* 定义无线接收长度（不是总长度，该变量用于某次传输，传输完就会清除）*/
uint32_t wirelessRxLength;

/* 无线发送缓冲区总共发送长度（一般用于测试） */
uint32_t wirelessTxTotalLength[WIRELESS_OBJ_NUM];
/* 无线接收缓冲区总共接收长度（一般用于测试） */
uint32_t wirelessRxTotalLength[WIRELESS_OBJ_NUM];

/**
 * @brief 无线发送信息表指针
 * @note  创建这个表的作用是，首先当在完成接收数据包的校验后，会根据数据表里的length值，将发送缓冲区里对应长度的空间释放。
 *        然后计算发送缓冲区里的剩余空间长度，在设置的最大有效载荷长度范围内，填写新的length值，表示本次发送的数据包长度。
 *        packID在处理接收数据包的时候已经更新，不在发送函数里进行设置。ackID在发送函数中进行设置。
 *        当收到对方的复位控制要求后，会在接收校验函数中复位ackID字段。
 *        当在接收校验函数中生成复位控制要求时，会复位packID。
 * @param sourceAddress 只在WirelessInfoTableInit()函数中赋值，赋值后不再更改其值
 * @param targetAddress 只在WirelessInfoTableInit()函数中赋值，赋值后不再更改其值
 * @param status 在WirelessRxPackVerify()中接收到有效数据包后根据情况设置相应标志以及在WirelessTimeoutRecord()设置相关状态
 * @param control 只在WirelessRxPackVerify()中接收到有效数据包后根据情况设置相应标志
 * @param packID 只在WirelessRxPackVerify()中接收到有效数据包后根据情况设置
 * @param ackID 只在WirelessTxPackFill()里如果是不是重发数据包就会被赋值
 * @param length 在WirelessTxPackFill()中如果是发送新数据包就会根据发送缓冲区队列长度进行赋值
 * @attention 在使用无线信息表时确保两个信息表的目的地址和源地址是对应的，否则调用相关函数时会出现错误！！！
 */
static WirelessInfo_t wirelessTxInfoTable[WIRELESS_OBJ_NUM];


/**
 * @brief 无线接收信息表指针
 * @note  创建这个信息表的作用是完成无线接收数据包的有效载荷搬运到接收缓冲区工作。完成接收数据包的校验后，对应的接收缓冲区接收
 *        函数就会根据更新完的无线发送信息表的status字段的值，以及无线接收信息表里的length字段，执行相应的接收工作。
 *        只要是发送给本机的数据包，就会将数据包里的数据搬运到接收缓冲区里。然后更新发送信息表里的ackID字段。
 * @attention 在使用无线信息表时确保两个信息表的目的地址和源地址是对应的，否则调用相关函数时会出现错误！！！
 */
static WirelessInfo_t wirelessRxInfoTable[WIRELESS_OBJ_NUM];

void WirelessPackMaxSizeDeInit(void){
    wirelessPackMaxSize = 800;
}

/**
 * @brief 无线数据包最大包长设置
 * @param[in] size  最大包长（单位字节）
 * @note  取值范围在64到1000之间
 */
void WirelessPackMaxSizeSet(uint16_t size){
    wirelessPackMaxSize = size;
}


/**
 * @brief 无线发送接收信息表项配置函数
 * @param[in] obj  信息表元素对象编号
 * @param[in] objInfo 无线通信对象信息（包含源地址和目标地址）
 * @attention 这个函数只能在无线通信开始前进行调用，如果进行无线通信过程中，最好不要修改
 *            否则可能会发生未知错误
 * @attention 请务必确保两个信息表里的源地址和目标地址是对应的，否则会导致后续接收函数执行错误！！！
 */
void WirelessInfoTableItemConfig(WirelessObj_t obj, const WirelessObjInfo_t* const objInfo) {
    /* 对无线发送信息表相关项目进行配置 */
    wirelessTxInfoTable[obj].objInfo = *objInfo;
    // wirelessTxInfoTablePtr[obj]->objInfo.sourceAddress = objInfo->sourceAddress;
    // wirelessTxInfoTablePtr[obj]->objInfo.targetAddress = objInfo->targetAddress;
    /* 因为配置了信息表，其余字段设置为默认值（这里由调用保证只在初始化调用一次） */
    // wirelessTxInfoTable[obj].status.statuss = 0;
    // wirelessTxInfoTable[obj].control.controll = 0;
    // wirelessTxInfoTable[obj].packID = 0;
    // wirelessTxInfoTable[obj].ackID = 0;
    // wirelessTxInfoTable[obj].length = 0;
    /* 对无线接收信息表相关项目进行配置（这里由调用保证只在初始化调用一次） */
    /* 对于接收信息表来说，源地址和目标的含义与函数参数中的源地址和目标地址是相反的，所以交叉赋值 */
    wirelessRxInfoTable[obj].objInfo.sourceAddress = objInfo->targetAddress;
    wirelessRxInfoTable[obj].objInfo.targetAddress = objInfo->sourceAddress;
    /* 因为配置了信息表，其余字段设置为默认值（这里由调用保证只在初始化调用一次） */
    // wirelessRxInfoTable[obj].status.statuss = 0;
    // wirelessRxInfoTable[obj].control.controll = 0;
    // wirelessRxInfoTable[obj].packID = 0;
    // wirelessRxInfoTable[obj].ackID = 0;
    // wirelessRxInfoTable[obj].length = 0;
}


/**
 * @brief 无线对象信息表指针配置函数
 * @param[in] obj  无线对象编号
 * @param[in] objInfo  无线对象信息指针
 * @return uint8_t 函数执行结果
 *         @arg 0: 初始话化成功
 *         @arg 1: 初始化失败，输入参数有误
 * @note  在无线信息表项配置里会对无线发送和接收信息表一同配置
 * @attention 这个函数只能在无线通信开始前进行调用，如果进行无线通信过程中，最好不要修改
 *            否则可能会发生未知错误
 */
uint8_t WirelessObjInfoTablePtrConfig(WirelessObj_t obj, WirelessObjInfo_t* const objInfo) {
    /* 如果传入的参数有误直接返回错误 */
    if (obj >= WIRELESS_OBJ_NUM || objInfo == NULL) {
        return 1;
    }
    wirelessObjInfoTablePtr[obj] = objInfo;
    /* 无线信息表一同进行更新 */
    WirelessInfoTableItemConfig(obj, objInfo);
    return 0;
}

uint8_t WirelessTxQueueInit(WirelessObj_t obj, QueueRM_t* const queue){
    /* 如果传入的参数有误直接返回错误 */
    if(obj >= WIRELESS_OBJ_NUM || queue == NULL){
        return 1;
    }
    wirelessTxQueuePtr[obj] = queue;
    return 0;
}
uint8_t WirelessRxQueueInit(WirelessObj_t obj, QueueRM_t* const queue){
    /* 如果传入的参数有误直接返回错误 */
    if(obj >= WIRELESS_OBJ_NUM || queue == NULL){
        return 1;
    }
    wirelessRxQueuePtr[obj] = queue;
    return 0;
}

/**
 * @brief 无线信息表查找函数
 * @return int8_t 查找到的索引号，如果返回 -1 说明查找失败
 * @note  同时对接收信息表和发送信息表进行查找，只有都匹配上才算查找成功
 * @attention 在使用该函数进行查找之前，务必保证信息表内容已正确初始化
 */
int8_t WirelessInfoTableQuery(void){
    for(uint8_t i = 0; i < WIRELESS_OBJ_NUM; i++){
        /* 对接收信息表和发送信息表同时进行搜索 */
        if(
            WirelessRxPack.pack.info.objInfo.targetAddress == wirelessRxInfoTable[i].objInfo.targetAddress &&
            WirelessRxPack.pack.info.objInfo.sourceAddress == wirelessRxInfoTable[i].objInfo.sourceAddress &&
            /* 接收信息表的源地址和目标地址是相反的 */
            WirelessRxPack.pack.info.objInfo.targetAddress == wirelessTxInfoTable[i].objInfo.sourceAddress &&
            WirelessRxPack.pack.info.objInfo.sourceAddress == wirelessTxInfoTable[i].objInfo.targetAddress 
        ){
        /* 返回当前索引 */
        return i;
        }
    }
    /* 接收到的数据包地址不在信息表中，说明接收到的数据包有误 */
    return -1;
}

/**
 * @brief 无线数据包检验函数
 * @param[in] WirelessObj_t  无线对象编号
 * @return uint8_t 函数执行结果
 *         @arg 0 : 检验通过       @arg 1 : 本次接收数据包存在问题
 * @note  函数内部首先在无线接收信息表中查询与当前接收到的数据包匹配的目标，
 *        寻找到匹配目标后，通过packID字段判断是否是重发数据包以及对方是否确认上一次
 *        本次回复的应答号，根据结果设置相应的状态位。只要在信息表里配上对象，不管状态如何，
 *        最终都会将接收数据包的packID更新到本地的无线接收信息表对应项目中。
 *        查找完无线接收信息表并且匹配到对象后，就可以查找无线发送信息里的匹配对象。
 *        寻找到匹配对象后，就开始通过ackID判断对方是否接收了本机上次发送的数据。
 *        如果应答号通过校验，则标志收到有效应答号，此时外部可以根据发送信息表的status字段
 *        释放对应的缓冲区空间。但即使没有通过校验，如判断出丢包情况，只是设置本机的packID从0开始，
 *        以及设置control里的复位操作，在外部仍进行释放缓冲区空间操作，不会进行重发数据包。
 *       （经过考虑认为这种情况不应该重发数据包，丢了的包就让它丢了）
 *        关于control字段的响应，当收到对方的数据包里control字段有复位命令，那么就会复位发送信息表里的
 *        ackID。
 * @attention 在调用该函数前要确保无线接收发送信息表的元素信息是符合要求的，否则该函数会导致该函数执行错误！！！
 */
uint8_t WirelessRxPackVerify(WirelessObj_t obj) {
    /* 复位无线状态和无线控制变量用于后续赋值 */
    wirelessStatus.statuss = 0x0000;
    wirelessControl.controll = 0x0000;
    /* ================================对无线接收信息表进行查询，根据对应的packID判断是否是重发数据包================================ */
    /* 如果接收信息表里的obj信息与接收到的无线数据包地址不一致，这说明接收的数据包有问题 */
    if(
        WirelessRxPack.pack.info.objInfo.targetAddress != wirelessRxInfoTable[obj].objInfo.targetAddress ||
        WirelessRxPack.pack.info.objInfo.sourceAddress != wirelessRxInfoTable[obj].objInfo.sourceAddress
    ){
        /* 接收到的数据包地址不在信息表中，说明接收到的数据包有误 */
        return 1;
    }
    /* 判断对方是否发送复位命令，如果发送了复位命令，就不进行下面的packID判断 */
    if (WirelessRxPack.pack.info.control.control.resetCommand) {
        /* 更新状态，用于下次发送时告知对方已按复位控制要求完成复位 */
        wirelessStatus.status.isReseted = 1;
        /* 重置应答号（实际上不用更新，因为在发送数据包填充函数里直接将无线接收数据包里的packID与length相加赋值给ackID） */
        // wirelessTxInfoTable[obj].ackID = 0;
    }
    /* 当对方发送的包ID是上次接收到的数据包的pack+length，说明对方已经确认了上次接收的数据，这次接收到新的数据包 */
    else if(WirelessRxPack.pack.info.packID == (wirelessRxInfoTable[obj].packID + wirelessRxInfoTable[obj].length)){
        /* 设置接收到新数据包标记 */
        wirelessStatus.status.hasRcvdValidPack = 1;
    }
    /* 本次接收到的packID与无线接收信息表（即存储的上一次接收到的数据包）里的packID一样，说明这是重发数据包 */
    else if(WirelessRxPack.pack.info.packID == wirelessRxInfoTable[obj].packID){
        /* 设置接收到重发数据包标记 */
        wirelessStatus.status.hasRcvdRepetPack = 1;
    }
    /* 如果packID不对也不是重发包，这说明本机或（和）对方通信中发送问题，
        一般来说就是需要复位了。不过在发送数据包填充函数中，默认是将对方的packID更新到本地即可，
        就下次发送数据包的ackID更新为本次接收数据包的PackID + length
        我们这里只设置标志位指示这种情况 */
    else{
        /* 按接收数据包更新应答号 */
        // wirelessTxInfoTable[obj].ackID = WirelessRxPack.pack.info.packID;
        wirelessStatus.status.hasRcvdWrongPack = 1;
    }
    /* 更新无线接收信息表 */
    wirelessRxInfoTable[obj] = WirelessRxPack.pack.info;

    /* ================================对无线发送信息表进行查询，根据ackID判断通信================================ */
    /* 如果发送信息表里的obj信息与接收到的无线数据包地址不一致，这说明接收的数据包有问题 */
    if(// 注意发送和接收的源与目标是相反的，交叉比较
        WirelessRxPack.pack.info.objInfo.targetAddress != wirelessTxInfoTable[obj].objInfo.sourceAddress ||
        WirelessRxPack.pack.info.objInfo.sourceAddress != wirelessTxInfoTable[obj].objInfo.targetAddress
    ){
        /* 接收到的数据包地址不在信息表中，说明接收到的数据包有误 */
        return 1;
    }
    /* 复位发送信息表的状态和控制状态，用于后续的赋值 */
    // wirelessTxInfoTable[obj].status.statuss = 0x0000;
    // wirelessTxInfoTable[obj].control.controll = 0x0000;
    /* 检验应答号（正确的应答号为 接收到的ackID == 上次发送的 packID + length） */
    if(WirelessRxPack.pack.info.ackID == (wirelessTxInfoTable[obj].packID + wirelessTxInfoTable[obj].length)){
        /* 更新接收无线发送数据表 */
        /* 更新发送包ID，为下一次发送做准备 */
        wirelessTxInfoTable[obj].packID = WirelessRxPack.pack.info.ackID;
        /* 通过校验，设置接收到有效确认标志 */
        wirelessStatus.status.hasRcvdValidAck = 1;
        /* 相应缓冲区进行更新，对对方确认接收到的数据可以释放其在缓冲区内的空间（这部分在函数外部执行） */
    }
    /* 否则校验失败认为丢数 */
    else{
        /* 设置复位命令，控制对方进行复位操作，让对方复位应答号 */
        // wirelessTxInfoTable[obj].control |= WIRELESS_CONTROL_RESET_COMMAND;
        wirelessControl.control.resetCommand = 1;
        /* 将本机的发送包ID重置为 0  */
        wirelessTxInfoTable[obj].packID = 0;
        /* 设置发送包状态为错误的应答号，可能存在丢包情况，用于告知对方 */
        wirelessStatus.status.hasRcvdWrongAck = 1;
    }
    /* 更新发送信息表的status字段 */
    wirelessTxInfoTable[obj].status = wirelessStatus;
    /* 更新发送信息表的control字段 */
    wirelessTxInfoTable[obj].control = wirelessControl;
    /* 校验通过，正常返回 */
    return 0;
}

// /**
//  * @brief 无线接收缓冲区刷新函数
//  * @param[in] obj  无线对象编号
//  * @return uint8_t 函数执行情况
//  * @note  根据wirelessRxInfo存储的信息，将无线接收数据包里的数据存入无线接收队列的缓冲区里
//  * @attention  该函数必须在WirelessRxPackVerify调用后才能调用，
//  *             接收一次数据包才允许调用一次，不能重复调用
//  */
// uint8_t WirelessRxBuffRefresh(WirelessObj_t obj){
//     /* 从接收信息表里提取接收到的数据包length字段*/
//     uint16_t length = wirelessRxInfoTable[obj].length;
//     /* length字段包含wirelessInfo_t字段的长度，有效载荷长度需要减去这个长度 */
//     length -= sizeof(WirelessInfo_t);
//     /* 写入接收缓冲区里对应长度的数据 */
//     return QueueRM_PutArrayIn(&wirelessRxQueue[obj],WirelessRxPack.pack.payload,length,NULL);
// }

// /**
//  * @brief 无线发送缓冲区刷新函数
//  * @param[in] obj  无线对象编号
//  * @return uint8_t 函数执行情况
//  * @note  根据wirelessTxInfo存储的信息，将无线发送队列的缓冲区里相应长度数据释放
//  * @attention  该函数必须在WirelessRxPackVerify调用后才能调用，
//  *             接收一次数据包才允许调用一次，不能重复调用
//  */
// uint8_t WirelessTxBuffRefresh(WirelessObj_t obj){
//     /* 获取上一次发送长度（在完成接收数据包校验后，校验成功的话，这一个数据包长度已确认对方接收到了） */
//     uint16_t length = wirelessTxInfoTable[obj].length;
//     /* length字段包含wirelessInfo_t字段的长度，释放缓冲区时需要减去这个长度 */
//     length -= sizeof(WirelessInfo_t);
//     /* 释放发送缓冲区里对应长度的数据 */
//     return QueueRM_Discard(&wirelessTxQueue[obj], length, NULL);
// }

// /**
//  * @brief 无线发送信息表更新函数
//  * @param[in] obj  无线对象编号
//  * @note  根据发送缓冲区剩余数据长度（在有效载荷长度范围内），填写无线发送信息的length字段
//  * @attention  填充发送数据包前调用，packID字段在接收数据包验证函数这里赋值，因此不在此赋值
//  */
// void WirelessTxInfoTableUpdate(WirelessObj_t obj){
//     /* 获取发送缓冲区里剩余数据长度 */
//     uint16_t length = QueueRM_GetLength(&wirelessTxQueue[obj]);
    
//     /* 限制长度，保证长度在数据有效载荷最大长度范围内 */
//     if(length > WIRELESS_PACK_PAYLOAD_MAX_LENGTH){
//         length = WIRELESS_PACK_PAYLOAD_MAX_LENGTH;
//     }
//     /* 长度加上无线数据包头部字段长度 */
//     length += sizeof(WirelessInfo_t);
//     /* 更新length字段 */
//     wirelessTxInfoTable[obj].length = length;
//     /* 更新ackID */
//     wirelessTxInfoTable[obj].ackID += wirelessRxInfoTable[obj].length;
// }


void WirelessTimeoutRecord(WirelessObj_t obj){
    /* 在发送信息表里的status字段记录超时情况 */
    /* 清除有效接收标志 */
    wirelessTxInfoTable[obj].status.status.hasRcvdValidPack = 0;
    /* 设置未接收到应答标志 */
    wirelessTxInfoTable[obj].status.status.hasNoRcvdResponse = 1;
}


/**
 * @brief 无线发送数据包填充函数
 * @param[in] obj  无线对象编号
 * @note  根据无线发送信息表里的信息填充无线发送数据包
 * @attention  调用前要确保无线发送信息表里的信息是正确的
 */
void WirelessTxPackFill(WirelessObj_t obj){
    /* 
        根据发送信息表status字段里的重复包答标志确定本次发送是否需要重发包，
        重复包标志不为1，就发送新的数据包
        重发数据包有两种情况：上次发送时没有收到应答；发现对方发送的是重复数据包
    */
    if(
        !wirelessTxInfoTable[obj].status.status.hasRcvdRepetPack &&
        !wirelessTxInfoTable[obj].status.status.hasNoRcvdResponse
    ){
        /* 发送新数据包前需要更新发送信息表 */
        /* 获取发送缓冲区里剩余数据长度 */
        // uint16_t length = QueueRM_GetLength(&wirelessTxQueue[obj]);
        uint16_t length = QueueRM_GetLength(wirelessTxQueuePtr[obj]);
        
        /* 限制长度，保证长度在数据有效载荷最大长度范围内 */
        if(length > WIRELESS_PACK_PAYLOAD_MAX_LENGTH){
            length = WIRELESS_PACK_PAYLOAD_MAX_LENGTH;
        }
        /* 限制无线数据包包长 */
        if(length > wirelessPackMaxSize){
            length = wirelessPackMaxSize;
        }
        /* 累加无线发送长度（用于测试） */
        wirelessTxTotalLength[obj] += length;
        /* 长度加上无线数据包头部字段长度 */
        length += sizeof(WirelessInfo_t);
        /* 更新length字段 */
        wirelessTxInfoTable[obj].length = length;
        /* 更新ackID */
        // wirelessTxInfoTable[obj].ackID += wirelessRxInfoTable[obj].length;
        wirelessTxInfoTable[obj].ackID = wirelessRxInfoTable[obj].packID + wirelessRxInfoTable[obj].length;
    }
    /* 将发送信息表的数据填充到发送数据包 */
    WirelessTxPack.pack.info = wirelessTxInfoTable[obj];
    /* 将发送缓冲区的数据搬运到发送数据包的有效载荷里 */
    QueueRM_Peep(
        // &wirelessTxQueue[obj], 
        wirelessTxQueuePtr[obj], 
        WirelessTxPack.pack.payload, 
        wirelessTxInfoTable[obj].length - sizeof(WirelessInfo_t), 
        NULL
    );
}

void WirelessRxPackHandle(WirelessObj_t obj){
    /* 根据发送信息表的状态判断收到的数据包是否为重复数据包 */
    if(!wirelessTxInfoTable[obj].status.status.hasRcvdRepetPack){
        /* 接收到新数据包，刷新接收缓冲区（将接收到的有效数据放入缓冲区） */
        /* 从接收信息表里提取接收到的数据包length字段*/
        uint16_t length = wirelessRxInfoTable[obj].length;
        /* length字段包含wirelessInfo_t字段的长度，有效载荷长度需要减去这个长度 */
        length -= sizeof(WirelessInfo_t);
        /* 写入接收缓冲区里对应长度的数据 */
        QueueRM_PutArrayIn(
            // &wirelessRxQueue[obj],
            wirelessRxQueuePtr[obj],
            WirelessRxPack.pack.payload,
            length,
            NULL
        );
        /* 累加无线接收长度（用于测试） */
        wirelessRxTotalLength[obj] += length;
        /* 更新发送缓冲区，释放对方已确认接收长度的数据 */
        /* 获取上一次发送长度（在完成接收数据包校验后，校验成功的话，这一个数据包长度已确认对方接收到了） */
        length = wirelessTxInfoTable[obj].length;
        /* length字段包含wirelessInfo_t字段的长度，释放缓冲区时需要减去这个长度 */
        length -= sizeof(WirelessInfo_t);
        /* 释放发送缓冲区里对应长度的数据 */
        // QueueRM_Discard(&wirelessTxQueue[obj], length, NULL);
        QueueRM_Discard(wirelessTxQueuePtr[obj], length, NULL);
    }
    /* 重复数据包不需要做接收相关操作 */
}