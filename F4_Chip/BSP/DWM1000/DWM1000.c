#include "DWM1000.h"

#include "deca_spi.h"
#include "port.h"

/* 定义DWM1000配置指针（方便动态配置） */
DWM1000_Config_t* DWM1000_ConfigurationPtr;

/* DWM1000 状态机变量定义 */
DWM1000WorkState_t DWM1000_WorkState = DWM1000_WORK_STATE_SEND_PACK; // 上电默认处于发送状态
/* DWM1000等待接收计数，用于检查DWM1000是否一直处于等待接收数据包状态，如果是，就会采取相关措施处理 */
uint32_t DWM1000_WaitRxCount;

/* DWM1000 发送缓冲区定义 */
uint8_t DWM1000_TxBuffer[DWM1000_TX_BUFF_LENGTH];
/* DWM1000 接收缓冲区定义 */
uint8_t DWM1000_RxBuffer[DWM1000_RX_BUFF_LENGTH];


/* DWM1000 相关中断回调函数 */
// static void rx_ok_cb(const dwt_cb_data_t* cb_data);
// static void rx_to_cb(const dwt_cb_data_t* cb_data);
// static void rx_err_cb(const dwt_cb_data_t* cb_data);
// static void tx_conf_cb(const dwt_cb_data_t* cb_data);

/**
 * @brief DWM1000配置指针初始化
 * @param[in] config  配置指针
 * @return uint8_t 函数执行情况
 * @note  必须初始化DWM1000配置指针后才可以进行后续的DWM1000初始化操作
 */
uint8_t DWM1000_ConfigurationPtrInit(DWM1000_Config_t* config) {
    if(config == NULL){
        return 1;
    }
    DWM1000_ConfigurationPtr = config;
    return 0;
}

/**
 * @brief DWM1000初始化函数
 * @return uint8_t 初始化情况
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  必须对先初始化DWM1000的配置指针后才能调用该函数进行初始化！！！
 */
uint8_t DWM1000_Init(void) {

    /* Install DW1000 IRQ handler. */
    port_set_deca_isr(dwt_isr);

    reset_DW1000();
    port_set_dw1000_slowrate();
    if (dwt_initialise(DWT_LOADNONE) == DWT_ERROR) {
        while (1)
            ;
    }
    port_set_dw1000_fastrate();

    /* 配置 DW1000 */
    // dwt_configure(&DWM1000_Configuration.dwtConfiguration);
    dwt_configure(DWM1000_ConfigurationPtr->dwtConfiguration);

    /* 注册 RX 回调 */
    dwt_setcallbacks(
        DWM1000_Callback_TxConf,    // 发送完成回调函数
        DWM1000_Callback_RxOK,      // 接收完成回调函数
        DWM1000_Callback_RxTimeout, // 接收超时回调函数
        DWM1000_Callback_RxError    // 接收错误回调函数
    );

    /* 使能所需中断 */
    dwt_setinterrupt(
        DWT_INT_TFRS |  // 发送完成事件
        DWT_INT_RFCG |  // 接收器FCS良好事件（CRC校验通过）
        DWT_INT_RFTO |  // 接收器等待帧超时事件
        DWT_INT_RXPTO | // 前导码检测超时事件
        DWT_INT_RPHE |  // 接收器PHY报头错误
        DWT_INT_RFCE |  // 接收器FCS错误
        DWT_INT_RFSL |  // 接收器帧同步丢失
        DWT_INT_SFDT,   // SFD超时
        1
    );

    /* 设置发送帧完成后打开接收器的延迟时间（最短6.2us） */
    dwt_setrxaftertxdelay(DWM1000_ConfigurationPtr->rxDelayTime);

    /* 设置接收超时时间（在超时时间内没有接收到帧） */
    dwt_setrxtimeout(DWM1000_ConfigurationPtr->rxRespTimeout);
 
//    while (1) {
//        /* 
//        当 DWM1000 处于 IDLE 状态时，发送与接收都没有处于工作状态，为了工作需要，我们需要将其配置为接收模式
//        */
//        // if(DWM1000_GetState() == DWM1000_STATE_IDLE){
//        //     dwt_forcetrxoff();
//        //     dwt_rxreset();
//        //     dwt_rxenable(DWT_START_RX_IMMEDIATE);//立即进入接收状态
//        // }

//        /* 处于接收状态 */
//        if(DWM1000_WorkState == 0){

//        }
//        /* 处于发送状态 */
//        else if(DWM1000_WorkState == 1){
//            DWM1000_WorkState = 0;// 切换状态为接收
//            /* Increment the blink frame sequence number (modulo 256). */
//            tx_msg[BLINK_FRAME_SN_IDX]++;
//            DWM1000_SendFrame(tx_msg,sizeof(tx_msg));
//        } 
//        /* 处于超时状态 */
//        else if(DWM1000_WorkState == 3){
//            DWM1000_WorkState = 0; // 切换状态为接收
//            /* 重发数据包 */
//            DWM1000_SendFrame(tx_msg,sizeof(tx_msg));
//        }
//    }
    /*
        前面都配置完后，使能DWM1000对应的外部中断服务函数
    */
    port_EnableEXT_IRQ();
    return 0;
}

uint8_t DWM1000_SendFrame(const uint8_t* const frame, const uint16_t length) {
    /* 下面两行代码是否有必要有待检查 */
    // dwt_forcetrxoff();
    // dwt_rxreset();

    dwt_writetxdata(length + 2, (uint8*) frame, 0); // length + 2 的原因是发送的数据还会携带两个字节的CRC
    dwt_writetxfctrl(length + 2, 0, 0);             // length + 2 的原因是发送的数据还会携带两个字节的CRC
    /* 立即开启发送器，并在发送结束后切换为接收状态 */
    dwt_starttx(DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED);
    return 0;
}


/**
 * @brief DWM1000 GPIO映射表
 * @note  根据GPIOx(x = 0 ... 7)映射相应的寄存器位
 */
const static uint32_t DWM1000_GPIO_Map[8][2] = {
    { GxM0, GxP0 },
    { GxM1, GxP1 },
    { GxM2, GxP2 },
    { GxM3, GxP3 },
    { GxM4, GxP4 },
    { GxM5, GxP5 },
    { GxM6, GxP6 },
    { GxM7, GxP7 },
};

/**
 * @brief DWM1000 GPIO初始化函数
 * @param[in] GPIOx  DWM1000的GPIO编号
 *          @arg DWM1000_GPIO_x: x取0到7
 * @param[in] mode DWM1000的引脚模式
 *          @arg DWM1000_GPIO_OUTPUT: 引脚设置为输出模式
 *          @arg DWM1000_GPIO_INPUT: 引脚设置为输入模式
 * @note 传入的参数可以是引脚编号的 |
 * @example DWM1000_GPIO_Init(DWM1000_GPIO_3 | DWM1000_GPIO_4, DWM1000_GPIO_OUTPUT);
 */
void DWM1000_GPIO_Init(DWM1000_GPIO_t GPIOx, DWM1000_GPIO_Mode_t mode){
    /* 使能DWM1000的GPIO时钟 */
    dwt_enablegpioclocks();
    /* 对输入的参数进行按位遍历，获取配置的GPIOx编号，并之后的赋值做准备 */
    uint32_t gpioNum = 0x00;
    uint32_t gpioDirection = 0x00;
    // /* 普通方法通过使用映射表实现赋值 */
    // for (uint8_t i = 0; i < 8; i++) {
    //     if (GPIOx & (0x01 << i)) {
    //         gpioNum |= DWM1000_GPIO_Map[i][0];
    //         gpioDirection |= DWM1000_GPIO_Map[i][1];
    //     }
    // }
    /* 
        快速方法，直接根据传入的参数计算出对应的gpioNum 和 gpioDirection 
        取出传入的GPIOx参数低4位的值，然后左移四位赋值给gpioNum
        然后取出GPIOx高四位的值，然后左移8位赋值给gpioNum
        同理，对于gpioDirection
        取出GPIOx低四位的值，直接赋值
        取出GPIOx高四位的值，左移4位赋值给gpioDirection
    */
    gpioDirection |= (GPIOx & 0x0F);
    gpioDirection |= ((GPIOx & 0xF0) << 4);
    // gpioNum |= ((GPIOx & 0x0F) << 4);
    // gpioNum |= ((GPIOx & 0xF0) << 8);
    gpioNum = gpioDirection << 4;
    /* 
        如果需要配置为输出模式，就需要将相应的引脚标志位设置为0
        如果配置位输入模式，那么将按上面的计算结果进行赋值即可
    */
    if(mode == DWM1000_GPIO_OUTPUT){
        gpioDirection = 0x00;
    }
    /* 根据之前遍历的配置，设置对应的引脚 */
    dwt_setgpiodirection(gpioNum, gpioDirection);
}

/**
 * @brief DWM1000 GPIO置位函数
 * @param[in] GPIOx  DWM1000的GPIO编号
 *          @arg DWM1000_GPIO_x: x取0到7
 * @note 传入的参数可以是引脚编号的 |
 * @example DWM1000_GPIO_SetBits(DWM1000_GPIO_3 | DWM1000_GPIO_4);
 */
void DWM1000_GPIO_SetBits(DWM1000_GPIO_t GPIOx){
    /* 对输入的参数进行按位遍历，获取配置的GPIOx编号，并之后的赋值做准备 */
    uint32_t gpioNum = 0x00;
    uint32_t gpioValue = 0x00;
    // /* 普通方法通过使用映射表实现赋值 */
    // for (uint8_t i = 0; i < 8; i++) {
    //     if (GPIOx & (0x01 << i)) {
    //         gpioNum |= DWM1000_GPIO_Map[i][0];
    //         gpioValue |= DWM1000_GPIO_Map[i][1];
    //     }
    // }
    /* 
        快速方法，直接根据传入的参数计算出对应的gpioNum 和 gpioValue 
        取出传入的GPIOx参数低4位的值，然后左移四位赋值给gpioNum
        然后取出GPIOx高四位的值，然后左移8位赋值给gpioNum
        同理，对于gpioValue
        取出GPIOx低四位的值，直接赋值
        取出GPIOx高四位的值，左移4位赋值给gpioValue
    */
    gpioValue |= (GPIOx & 0x0F);
    gpioValue |= ((GPIOx & 0xF0) << 4);
    // gpioNum |= ((GPIOx & 0x0F) << 4);
    // gpioNum |= ((GPIOx & 0xF0) << 8);
    gpioNum = gpioValue << 4;
    /* 根据之前遍历的配置，设置对应的引脚 */
    dwt_setgpiovalue(gpioNum, gpioValue);
}

/**
 * @brief DWM1000 GPIO清除位函数
 * @param[in] GPIOx  DWM1000的GPIO编号
 *          @arg DWM1000_GPIO_x: x取0到7
 * @note 传入的参数可以是引脚编号的 |
 * @example DWM1000_GPIO_ResetBits(DWM1000_GPIO_3 | DWM1000_GPIO_4);
 */
void DWM1000_GPIO_ResetBits(DWM1000_GPIO_t GPIOx){
    /* 对输入的参数进行按位遍历，获取配置的GPIOx编号，并之后的赋值做准备 */
    uint32_t gpioNum = 0x00;
    // /* 普通方法通过使用映射表实现赋值 */
    // for (uint8_t i = 0; i < 8; i++) {
    //     if (GPIOx & (0x01 << i)) {
    //         gpioNum |= DWM1000_GPIO_Map[i][0];
    //     }
    // }
    /* 
        快速方法，直接根据传入的参数计算出对应的gpioNum 和 gpioValue 
        取出传入的GPIOx参数低4位的值，然后左移四位赋值给gpioNum
        然后取出GPIOx高四位的值，然后左移8位赋值给gpioNum
    */
    gpioNum |= ((GPIOx & 0x0F) << 4);
    gpioNum |= ((GPIOx & 0xF0) << 8);
    /* 根据之前遍历的配置，设置对应的引脚 */
    dwt_setgpiovalue(gpioNum, 0);
}

/**
 * @brief DWM1000 GPIO位翻转函数
 * @param[in] GPIOx  DWM1000的GPIO编号
 *          @arg DWM1000_GPIO_x: x取0到7
 * @note  传入的参数只能是一个引脚的编号！！！
 */
void DWM1000_GPIO_ToggleBit(DWM1000_GPIO_t GPIOx){
    /* 对输入的参数进行按位遍历，获取配置的GPIOx编号，并之后的赋值做准备 */
    uint32_t gpioNum = 0x00;
    uint32_t gpioValue = 0x00;
    // /* 普通方法通过使用映射表实现赋值 */
    // for (uint8_t i = 0; i < 8; i++) {
    //     if (GPIOx & (0x01 << i)) {
    //         gpioNum |= DWM1000_GPIO_Map[i][0];
    //         gpioValue |= DWM1000_GPIO_Map[i][1];
    //     }
    // }
    /* 
        快速方法，直接根据传入的参数计算出对应的gpioNum 和 gpioValue 
        取出传入的GPIOx参数低4位的值，然后左移四位赋值给gpioNum
        然后取出GPIOx高四位的值，然后左移8位赋值给gpioNum
        同理，对于gpioValue
        取出GPIOx低四位的值，直接赋值
        取出GPIOx高四位的值，左移4位赋值给gpioValue
    */
    gpioValue |= (GPIOx & 0x0F);
    gpioValue |= ((GPIOx & 0xF0) << 4);
    // gpioNum |= ((GPIOx & 0x0F) << 4);
    // gpioNum |= ((GPIOx & 0xF0) << 8);
    gpioNum = gpioValue << 4;
    int value = dwt_getgpiovalue(gpioNum);
    if(value){
        dwt_setgpiovalue(gpioNum, gpioValue);
    }
    else{
        dwt_setgpiovalue(gpioNum, 0);
    }
}

/**
 * @brief DWM1000 GPIO写函数
 * @param[in] GPIOx  DWM1000的GPIO编号
 *      @arg DWM1000_GPIO_x: x取0到7
 * @param[in] value  GPIO引脚电平
 *      @arg DWM1000_GPIO_LEVEL_RESET: GPIO引脚低电平
 *      @arg DWM1000_GPIO_LEVEL_SET: GPIO引脚高电平
 * @note 传入的参数可以是引脚编号的 |
 * @example DWM1000_GPIO_Write(DWM1000_GPIO_3 | DWM1000_GPIO_4);
 */
void DWM1000_GPIO_Write(DWM1000_GPIO_t GPIOx, DWM1000_GPIO_Level_t value) {
    /* 对输入的参数进行按位遍历，获取配置的GPIOx编号，并之后的赋值做准备 */
    uint32_t gpioNum = 0x00;
    uint32_t gpioValue = 0x00;
    // /* 普通方法通过使用映射表实现赋值 */
    // for (uint8_t i = 0; i < 8; i++) {
    //     if (GPIOx & (0x01 << i)) {
    //         gpioNum |= DWM1000_GPIO_Map[i][0];
    //         gpioValue |= DWM1000_GPIO_Map[i][1];
    //     }
    // }
    /* 
        快速方法，直接根据传入的参数计算出对应的gpioNum 和 gpioValue 
        取出传入的GPIOx参数低4位的值，然后左移四位赋值给gpioNum
        然后取出GPIOx高四位的值，然后左移8位赋值给gpioNum
        同理，对于gpioValue
        取出GPIOx低四位的值，直接赋值
        取出GPIOx高四位的值，左移4位赋值给gpioValue
    */
    gpioValue |= (GPIOx & 0x0F);
    gpioValue |= ((GPIOx & 0xF0) << 4);
    // gpioNum |= ((GPIOx & 0x0F) << 4);
    // gpioNum |= ((GPIOx & 0xF0) << 8);
    gpioNum = gpioValue << 4;
    if(value == DWM1000_GPIO_LEVEL_RESET){
        gpioValue = 0;
    }
    /* 根据之前遍历的配置，设置对应的引脚 */
    dwt_setgpiovalue(gpioNum, gpioValue);
}

/**
 * @brief DWM1000 GPIO读引脚函数
 * @param[in] GPIOx  DWM1000的GPIO编号
 *      @arg DWM1000_GPIO_x: x取0到7
 * @return DWM1000_GPIO_Level_t 读取的引脚值、
 *      @arg DWM1000_GPIO_LEVEL_RESET: 读取到的引脚为低电平
 *      @arg DWM1000_GPIO_LEVLEL_SET: 读取到的引脚为高电平
 * @note  传入的参数只能是一个引脚的编号！！！
 */
DWM1000_GPIO_Level_t DWM1000_GPIO_ReadBit(DWM1000_GPIO_t GPIOx){
    uint32_t gpioNum = 0x00;
    gpioNum |= ((GPIOx & 0x0F) << 4);
    gpioNum |= ((GPIOx & 0xF0) << 8);
    return (uint8_t)dwt_getgpiovalue(gpioNum);
}

// /*! ------------------------------------------------------------------------------------------------------------------
//  * @fn rx_ok_cb()
//  *
//  * @brief Callback to process RX good frame events
//  *
//  * @param  cb_data  callback data
//  *
//  * @return  none
//  */
// static void rx_ok_cb(const dwt_cb_data_t* cb_data) {
//     DWM1000_WorkState = 1;
//     dwt_readrxdata(DWM1000_RxBuffer, cb_data->datalength, 0);

//     /* TESTING BREAKPOINT LOCATION #1 */
// }

// /**
//  * @brief 接收超时回调函数
//  * @param[in] cb_data  回调数据
//  * @note  当发生 接收器帧等待超时 或 前导码检测超时 时触发这个回调函数
//  */
// static void rx_to_cb(const dwt_cb_data_t* cb_data) {
//     DWM1000_WorkState = 3;// 切换为超时状态
//     /* TESTING BREAKPOINT LOCATION #2 */
// }


//  /**
//   * @brief 接收错误回调函数，处理 RX 错误事件
//   * @param[in] cb_data  回调数据
//   * @note  当发生 接收端PHY头超时、接收SFD超时、接收端里德-所罗门帧同步丢失、
//   *        接收端帧校验序列错误、边沿检测处理错误和自动帧过滤拒收时触发该回调函数
//   */
// static void rx_err_cb(const dwt_cb_data_t* cb_data) {
//     /* Activate reception immediately. */
//     dwt_rxenable(DWT_START_RX_IMMEDIATE);
//     /* TESTING BREAKPOINT LOCATION #3 */
// }

// /*! ------------------------------------------------------------------------------------------------------------------
//  * @fn tx_conf_cb()
//  *
//  * @brief Callback to process TX confirmation events
//  *
//  * @param  cb_data  callback data
//  *
//  * @return  none
//  */
// static void tx_conf_cb(const dwt_cb_data_t* cb_data) {
//     /* This callback has been defined so that a breakpoint can be put here to check it is correctly called but there is actually nothing specific to
//      * do on transmission confirmation in this example. Typically, we could activate reception for the response here but this is automatically handled
//      * by DW1000 using DWT_RESPONSE_EXPECTED parameter when calling dwt_starttx().
//      * An actual application that would not need this callback could simply not define it and set the corresponding field to NULL when calling
//      * dwt_setcallbacks(). The ISR will not call it which will allow to save some interrupt processing time. */

//     /* TESTING BREAKPOINT LOCATION #4 */
// }

/*
关于 DWM1000 工作状态说明（具体内容参考用户手册 2.3节 DW1000 工作状态）
DWM1000从断电到上电，首先进入 WAKEUP 状态，然后进入 INIT 状态。
CLKPLL锁定后就会进入 IDLE状态。
接着，当使能 TX 或 RX 后分别进入 TX 状态 或 RX 状态。
RX 或 TX 完成后，在默认情况下是返回 IDLE 状态
可以设置 TX 进入 IDLE 后再进入RX
在RX状态下，只要完成接收（不管是成功接收、超时还是错误）都会接收这次接收
返回IDLE状态
*/