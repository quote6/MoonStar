/**
 * @file drv_dw1000_reg_params.h
 * @author quote6 (2293127401@qq.com)
 * @date 2026-01-27
 * @brief
 *
 * @copyright Copyright (c) 2026 quote6
 * For study and research only, no reprinting
 *
 * @par 修改日志
 * <table>
 * <tr><th>Date        <th>Version      <th>Author    <th>Description
 * <tr><td>2026-01-27      <td>{version}   <td>quote6   <td>{Description}
 * </table>
 * ***************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_DW1000_REG_PARAMS_H
#define __DRV_DW1000_REG_PARAMS_H
/* Includes ------------------------------------------------------------------*/
#include "drv_dw1000_reg.h"
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/**
 * @}
 */
/* Exported constants --------------------------------------------------------*/

/**
 * @brief 定义DW1000设备ID
 * @note  位于寄存器 DEV_ID - 0x00
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_DEV_ID                              0xDECA0130UL /**< DW1000 device ID */

/**
 * @brief 定义帧过滤使能控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @param DW1000_FRAME_FILTER_ENABLE: 使能帧过滤功能
 * @param DW1000_FRAME_FILTER_DISABLE: 禁用帧过滤功能
 */
#define DW1000_FRAME_FILTER_ENABLE                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFEN_POS)
#define DW1000_FRAME_FILTER_DISABLE                (0x0UL << DW1000_REG_SYS_CFG_BIT_FFEN_POS)
#define DW1000_FRAME_FILTER_DEFAULT                DW1000_FRAME_FILTER_DISABLE

/**
 * @brief 定义帧过滤行为控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FF_BEHAVE_COORDINATOR               (0x1UL << DW1000_REG_SYS_CFG_BIT_FFBC_POS)
#define DW1000_FF_BEHAVE_NORMAL_NODE               (0x0UL << DW1000_REG_SYS_CFG_BIT_FFBC_POS)
#define DW1000_FF_BEHAVE_DEFAULT                   DW1000_FF_BEHAVE_NORMAL_NODE

/**
 * @brief 定义帧过滤信标帧控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FF_BEACON_FRAME_ALLOW               (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAB_POS)
#define DW1000_FF_BEACON_FRAME_REJECT              (0x0UL << DW1000_REG_SYS_CFG_BIT_FFAB_POS)
#define DW1000_FF_BEACON_FRAME_DEFAULT             DW1000_FF_BEACON_FRAME_REJECT

/**
 * @brief 定义帧过滤数据帧控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FF_DATA_FRAME_ALLOW                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAD_POS)
#define DW1000_FF_DATA_FRAME_REJECT                (0x0UL << DW1000_REG_SYS_CFG_BIT_FFAD_POS)
#define DW1000_FF_DATA_FRAME_DEFAULT               DW1000_FF_DATA_FRAME_REJECT

/**
 * @brief 定义帧过滤ACK帧控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FF_ACK_FRAME_ALLOW                  (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAA_POS)
#define DW1000_FF_ACK_FRAME_REJECT                 (0x0UL << DW1000_REG_SYS_CFG_BIT_FFAA_POS)
#define DW1000_FF_ACK_FRAME_DEFAULT                DW1000_FF_ACK_FRAME_REJECT

/**
 * @brief 定义帧过滤MAC命令帧控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FF_MAC_CMD_FRAME_ALLOW              (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAM_POS)
#define DW1000_FF_MAC_CMD_FRAME_REJECT             (0x0UL << DW1000_REG_SYS_CFG_BIT_FFAM_POS)
#define DW1000_FF_MAC_CMD_FRAME_DEFAULT            DW1000_FF_MAC_CMD_FRAME_REJECT

/**
 * @brief 定义帧过滤保留帧类型控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FF_RESERVED_FRAME_ALLOW             (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAR_POS)
#define DW1000_FF_RESERVED_FRAME_REJECT            (0x0UL << DW1000_REG_SYS_CFG_BIT_FFAR_POS)
#define DW1000_FF_RESERVED_FRAME_DEFAULT           DW1000_FF_RESERVED_FRAME_REJECT

/**
 * @brief 定义帧过滤帧类型字段4控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FF_FRAME_TYPE_FIELD4_ALLOW          (0x1UL << DW1000_REG_SYS_CFG_BIT_FFA4_POS)
#define DW1000_FF_FRAME_TYPE_FIELD4_REJECT         (0x0UL << DW1000_REG_SYS_CFG_BIT_FFA4_POS)
#define DW1000_FF_FRAME_TYPE_FIELD4_DEFAULT        DW1000_FF_FRAME_TYPE_FIELD4_REJECT

/**
 * @brief 定义帧过滤帧类型字段5控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FF_FRAME_TYPE_FIELD5_ALLOW          (0x1UL << DW1000_REG_SYS_CFG_BIT_FFA5_POS)
#define DW1000_FF_FRAME_TYPE_FIELD5_REJECT         (0x0UL << DW1000_REG_SYS_CFG_BIT_FFA5_POS)
#define DW1000_FF_FRAME_TYPE_FIELD5_DEFAULT        DW1000_FF_FRAME_TYPE_FIELD5_REJECT


// #define DW1000_FRAME_FILTER_ALL_TYPE_ALLOW         \
//     (DW1000_FF_BEACON_FRAME_ALLOW |      \
//      DW1000_FF_DATA_FRAME_ALLOW |        \
//      DW1000_FF_ACK_FRAME_ALLOW |         \
//      DW1000_FF_MAC_CMD_FRAME_ALLOW |     \
//      DW1000_FF_RESERVED_FRAME_ALLOW |    \
//      DW1000_FF_FRAME_TYPE_FIELD4_ALLOW | \
//      DW1000_FF_FRAME_TYPE_FIELD5_ALLOW)

#define DW1000_FRAME_FILTER_MSK                    (0x000000FFUL)
#define DW1000_FF_ALL_TYPE_ALLOW                   (0x000001FEUL)
#define DW1000_FF_ALL_TYPE_REJECT                  (0x00000000UL)

/**
 * @brief 定义主机中断极性控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_HOST_INTERRUPT_POLARITY_ACTIVE_HIGH (0x1UL << DW1000_REG_SYS_CFG_BIT_HIRQ_POL_POS)
#define DW1000_HOST_INTERRUPT_POLARITY_ACTIVE_LOW  (0x0UL << DW1000_REG_SYS_CFG_BIT_HIRQ_POL_POS)
#define DW1000_HOST_INTERRUPT_POLARITY_DEFAULT     DW1000_HOST_INTERRUPT_POLARITY_ACTIVE_HIGH

/**
 * @brief 定义SPI数据启动边沿控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SPI_DATA_LAUNCH_EDGE_SAMPLE_EDGE    (0x1UL << DW1000_REG_SYS_CFG_BIT_SPI_EDGE_POS)
#define DW1000_SPI_DATA_LAUNCH_EDGE_OPPOSITE_EDGE  (0x0UL << DW1000_REG_SYS_CFG_BIT_SPI_EDGE_POS)
#define DW1000_SPI_DATA_LAUNCH_EDGE_DEFAULT        DW1000_SPI_DATA_LAUNCH_EDGE_SAMPLE_EDGE

/**
 * @brief 定义帧校验错误处理控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FRAME_CHECK_ERROR_HANDLE_ENABLE     (0x0UL << DW1000_REG_SYS_CFG_BIT_DIS_FCE_POS)
#define DW1000_FRAME_CHECK_ERROR_HANDLE_DISABLE    (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_FCE_POS)
#define DW1000_FRAME_CHECK_ERROR_HANDLE_DEFAULT    DW1000_FRAME_CHECK_ERROR_HANDLE_ENABLE

/**
 * @brief 定义双重接收缓冲区控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_DOUBLE_RX_BUFFER_ENABLE             (0x0UL << DW1000_REG_SYS_CFG_BIT_DIS_DRXB_POS)
#define DW1000_DOUBLE_RX_BUFFER_DISABLE            (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_DRXB_POS)
#define DW1000_DOUBLE_RX_BUFFER_DEFAULT            DW1000_DOUBLE_RX_BUFFER_DISABLE

/**
 * @brief 定义在检测到PHR错误时中止接收控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_RX_ABORT_ON_PHR_ERROR_ENABLE        (0x0UL << DW1000_REG_SYS_CFG_BIT_DIS_PHE_POS)
#define DW1000_RX_ABORT_ON_PHR_ERROR_DISABLE       (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_PHE_POS)
#define DW1000_RX_ABORT_ON_PHR_ERROR_DEFAULT       DW1000_RX_ABORT_ON_PHR_ERROR_ENABLE

/**
 * @brief 定义在检测到RSD错误时中止接收控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_RX_ABORT_ON_RSD_ERROR_ENABLE        (0x0UL << DW1000_REG_SYS_CFG_BIT_DIS_RSDE_POS)
#define DW1000_RX_ABORT_ON_RSD_ERROR_DISABLE       (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_RSDE_POS)
#define DW1000_RX_ABORT_ON_RSD_ERROR_DEFAULT       DW1000_RX_ABORT_ON_RSD_ERROR_ENABLE

/**
 * @brief 定义帧校验初始化种子值控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_FCS_INIT_SEED_VALUE_0X0000          (0x0UL << DW1000_REG_SYS_CFG_BIT_FCS_INIT2F_POS)
#define DW1000_FCS_INIT_SEED_VALUE_0XFFFF          (0x1UL << DW1000_REG_SYS_CFG_BIT_FCS_INIT2F_POS)
#define DW1000_FCS_INIT_SEED_VALUE_DEFAULT         DW1000_FCS_INIT_SEED_VALUE_0X0000

/**
 * @brief 定义帧头模式控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_PHR_MODE_STANDARD                   (0x0 << DW1000_REG_SYS_CFG_BITS_PHR_MODE_POS)
#define DW1000_PHR_MODE_EXTENDED                   (0x3 << DW1000_REG_SYS_CFG_BITS_PHR_MODE_POS)
#define DW1000_PHR_MODE_DEFAULT                    DW1000_PHR_MODE_STANDARD

/**
 * @brief 定义智能发射功率控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SMART_TX_POWER_CONTROL_ENABLE       (0x0UL << DW1000_REG_SYS_CFG_BIT_DIS_STXP_POS)
#define DW1000_SMART_TX_POWER_CONTROL_DISABLE      (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_STXP_POS)
#define DW1000_SMART_TX_POWER_CONTROL_DEFAULT      DW1000_SMART_TX_POWER_CONTROL_ENABLE

/**
 * @brief 定义接收模式数据速率控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_RX_MODE_DATA_RATE_AUTO_DETERMINE    (0x0UL << DW1000_REG_SYS_CFG_BIT_RXM110K_POS)
#define DW1000_RX_MODE_DATA_RATE_110kbps           (0x1UL << DW1000_REG_SYS_CFG_BIT_RXM110K_POS)
#define DW1000_RX_MODE_DATA_RATE_DEFAULT           DW1000_RX_MODE_DATA_RATE_AUTO_DETERMINE

/**
 * @brief 定义接收等待超时控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_RX_WAIT_TIMEOUT_ENABLE              (0x1UL << DW1000_REG_SYS_CFG_BIT_RXWTOE_POS)
#define DW1000_RX_WAIT_TIMEOUT_DISABLE             (0x0UL << DW1000_REG_SYS_CFG_BIT_RXWTOE_POS)
#define DW1000_RX_WAIT_TIMEOUT_DEFAULT             DW1000_RX_WAIT_TIMEOUT_DISABLE

/**
 * @brief 定义接收自动重使能控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_RX_AUTO_RE_ENABLE                   (0x1UL << DW1000_REG_SYS_CFG_BIT_RXAUTR_POS)
#define DW1000_RX_AUTO_RE_DISABLE                  (0x0UL << DW1000_REG_SYS_CFG_BIT_RXAUTR_POS)
#define DW1000_RX_AUTO_RE_DEFAULT                  DW1000_RX_AUTO_RE_DISABLE

/**
 * @brief 定义自动应答控制
 * @note  位于寄存器 SYS_CFG - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_AUTO_ACK_ENABLE                     (0x1UL << DW1000_REG_SYS_CFG_BIT_AUTOACK_POS)
#define DW1000_AUTO_ACK_DISABLE                    (0x0UL << DW1000_REG_SYS_CFG_BIT_AUTOACK_POS)
#define DW1000_AUTO_ACK_DEFAULT                    DW1000_AUTO_ACK_DISABLE

/**
 * @brief 定义自动应答挂起位控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_AUTO_ACK_PEND_BIT_SET               (0x1UL << DW1000_REG_SYS_CFG_BIT_AACKPEND_POS)
#define DW1000_AUTO_ACK_PEND_BIT_CLEAR             (0x0UL << DW1000_REG_SYS_CFG_BIT_AACKPEND_POS)
#define DW1000_AUTO_ACK_PEND_BIT_DEFAULT           DW1000_AUTO_ACK_PEND_BIT_CLEAR

/**
 * @brief 发送开始，立刻进入发送状态（如果没有设置延迟发送的话）
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SYS_CTRL_TX_START                   0x01

/**
 * @brief 收发器关闭，DW1000立刻返回空闲状态
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SYS_CTRL_TRX_OFF                    0x01


/**
 * @brief 定义DW1000所有接收成功状态位掩码
 * @note  位于寄存器 SYS_STATUS - 0x0F
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SYS_STATUS_ALL_RX_GOOD_MSK          ( \
    DW1000_REG_SYS_STATUS_BIT_RXDFR |                \
    DW1000_REG_SYS_STATUS_BIT_RXFCG |                \
    DW1000_REG_SYS_STATUS_BIT_RXPRD |                \
    DW1000_REG_SYS_STATUS_BIT_RXSFDD |               \
    DW1000_REG_SYS_STATUS_BIT_RXPHD |                \
    DW1000_REG_SYS_STATUS_BIT_LDEDONE)

#define DW1000_SYS_STATUS_ALL_DOUBLE_BUFF_MSK ( \
    DW1000_REG_SYS_STATUS_BIT_RXDFR |           \
    DW1000_REG_SYS_STATUS_BIT_RXFCG)

#define DW1000_SYS_STATUS_ALL_RX_ERR_MSK ( \
    DW1000_REG_SYS_STATUS_BIT_RXPHE |      \
    DW1000_REG_SYS_STATUS_BIT_RXFCE |      \
    DW1000_REG_SYS_STATUS_BIT_RXRFSL |     \
    DW1000_REG_SYS_STATUS_BIT_RXSFDTO |    \
    DW1000_REG_SYS_STATUS_BIT_AFFREJ |     \
    DW1000_REG_SYS_STATUS_BIT_LDEERR)

#define DW1000_SYS_STATUS_ALL_RX_TO_MSK ( \
    DW1000_REG_SYS_STATUS_BIT_RXRFTO |    \
    DW1000_REG_SYS_STATUS_BIT_RXPTO)

#define DW1000_SYS_STATUS_ALL_TX_MSK ( \
    DW1000_REG_SYS_STATUS_BIT_AAT |    \
    DW1000_REG_SYS_STATUS_BIT_TXFRB |  \
    DW1000_REG_SYS_STATUS_BIT_TXPRS |  \
    DW1000_REG_SYS_STATUS_BIT_TXPHS |  \
    DW1000_REG_SYS_STATUS_BIT_TXFRS)

#define DW1000_RX_FINFO_RXFL_1023_MSK                (0x000003FFUL)


#define DW1000_SYS_STATUS_O3_L16_TXERR_MSK           (0x0408)

/**
 * @brief 定义DW1000的USR_SFD寄存器在不同速率下的非标SFD长度
 * @note  位于寄存器 USR_SFD - 0x21
 */
#define DW1000_USR_SFD_NSTD_110K_LEN                 64
#define DW1000_USR_SFD_NSTD_850K_LEN                 16
#define DW1000_USR_SFD_NSTD_6M8_LEN                  8


/**
 * @brief 定义DW1000的AGC调谐寄存器1在不同PRF下的值
 * @note  位于寄存器 AGC_CTRL - 0x23
 *        子寄存器 AGC_TUNE1 - 0x04
 */
#define DW1000_AGC_TUNE1_PRF16M                      0x8870
#define DW1000_AGC_TUNE1_PRF64M                      0x889B

/**
 * @brief 定义DW1000的AGC调谐寄存器2的默认值
 * @note  位于寄存器 AGC_CTRL - 0x23
 *        子寄存器 AGC_TUNE2 - 0x0C
 */
#define DW1000_AGC_TUNE2_DEFAULT                     0x2502A907


/**
 * @brief PLL时钟锁定检测使能
 * @note  位于寄存器 EXT_SYNC - 0x24
 *        子寄存器 EC_CTRL - 0x00
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_PLL_LOCK_DETECT_ENABLE                (0x1UL << DW1000_SUB_REG_EC_CTRL_BIT_PLLLDT_POS)


/**
 * @brief 定义GPIO模式
 * @note  默认为DW1000_GPIO_MODE_DEFAULT
 * @attention  根据用户手册，GPIO模式2和模式3保留不应使用
 */
#define DW1000_GPIO_MODE_0                           0x00
#define DW1000_GPIO_MODE_1                           0x01
// #define DW1000_GPIO_MODE_2                  0x02 /* 保留不应使用 */
// #define DW1000_GPIO_MODE_3                  0x03 /* 保留不应使用 */
#define DW1000_GPIO_MODE_DEFAULT                     DW1000_GPIO_MODE_0

/**
 * @brief 配置GPIO引脚的模式
 * @note 位于寄存器 GPIO_CTRL - 0x26
 *       子寄存器 GPIO_MODE - 0x00
 * @param[in] pin  引脚编号，取值0-8
 * @param[in] mode 模式
 *  @arg DW1000_GPIO_MODE_x： 模式x，x取值0-3
 * @warning  目前只能使用模式0-1，模式2-3保留不应使用
 */
#define DW1000_GPIO_MODE_SELECT(pin, mode)           ((mode) << (2 * (pin) + 6UL))
#define DW1000_GPIO_MODE_PIN_MASK(pin)               (0x03 << (2 * (pin) + 6UL))


#define DW1000_LNA_PA_MODE_MASK                      (DW1000_SUB_REG_GPIO_MODE_BITS_MSGP4 | DW1000_SUB_REG_GPIO_MODE_BITS_MSGP5 | DW1000_SUB_REG_GPIO_MODE_BITS_MSGP6)
// #define DW1000_LNA_PA_MODE_MASK                                            DW1000_REG_BIT_RANGE(19, 14)

#define DW1000_GPIO_MODE_P2_RXLED                    DW1000_GPIO_MODE_SELECT(2, DW1000_GPIO_MODE_1)
#define DW1000_GPIO_MODE_P3_TXLED                    DW1000_GPIO_MODE_SELECT(3, DW1000_GPIO_MODE_1)
#define DW1000_GPIO_MODE_P4_EXTPA                    DW1000_GPIO_MODE_SELECT(4, DW1000_GPIO_MODE_1)
#define DW1000_GPIO_MODE_P5_EXTTXE                   DW1000_GPIO_MODE_SELECT(5, DW1000_GPIO_MODE_1)
#define DW1000_GPIO_MODE_P6_EXTRXE                   DW1000_GPIO_MODE_SELECT(6, DW1000_GPIO_MODE_1)


/**
 * @brief 定义GPIO方向
 * @note  位于寄存器 GPIO_CTRL - 0x26
 *        子寄存器 GPIO_DIR - 0x08
 * @param DW1000_GPIO_DIRECTION_OUTPUT_Px: 编号x引脚为输出，x取值0-8
 * @param DW1000_GPIO_DIRECTION_INPUT_Px: 编号x引脚为输入，x取值0-8
 */
#define DW1000_GPIO_DIRECTION_OUTPUT_P0              0x00000010
#define DW1000_GPIO_DIRECTION_OUTPUT_P1              0x00000020
#define DW1000_GPIO_DIRECTION_OUTPUT_P2              0x00000040
#define DW1000_GPIO_DIRECTION_OUTPUT_P3              0x00000080
#define DW1000_GPIO_DIRECTION_OUTPUT_P4              0x00001000
#define DW1000_GPIO_DIRECTION_OUTPUT_P5              0x00002000
#define DW1000_GPIO_DIRECTION_OUTPUT_P6              0x00004000
#define DW1000_GPIO_DIRECTION_OUTPUT_P7              0x00008000
#define DW1000_GPIO_DIRECTION_OUTPUT_P8              0x00100000
#define DW1000_GPIO_DIRECTION_INPUT_P0               0x00000011
#define DW1000_GPIO_DIRECTION_INPUT_P1               0x00000022
#define DW1000_GPIO_DIRECTION_INPUT_P2               0x00000044
#define DW1000_GPIO_DIRECTION_INPUT_P3               0x00000088
#define DW1000_GPIO_DIRECTION_INPUT_P4               0x00001100
#define DW1000_GPIO_DIRECTION_INPUT_P5               0x00002200
#define DW1000_GPIO_DIRECTION_INPUT_P6               0x00004400
#define DW1000_GPIO_DIRECTION_INPUT_P7               0x00008800
#define DW1000_GPIO_DIRECTION_INPUT_P8               0x00110000

/**
 * @brief 定义GPIO方向
 * @note  位于寄存器 GPIO_CTRL - 0x26
 *        子寄存器 GPIO_DOUT - 0x0C
 * @param DW1000_GPIO_OUTPUT_STATE_LOW_Px: 编号x引脚输出低电平，x取值0-8
 * @param DW1000_GPIO_OUTPUT_STATE_HIGH_Px: 编号x引脚输出高电平，x取值0-8
 */
#define DW1000_GPIO_OUTPUT_STATE_LOW_P0              0x00000010
#define DW1000_GPIO_OUTPUT_STATE_LOW_P1              0x00000020
#define DW1000_GPIO_OUTPUT_STATE_LOW_P2              0x00000040
#define DW1000_GPIO_OUTPUT_STATE_LOW_P3              0x00000080
#define DW1000_GPIO_OUTPUT_STATE_LOW_P4              0x00001000
#define DW1000_GPIO_OUTPUT_STATE_LOW_P5              0x00002000
#define DW1000_GPIO_OUTPUT_STATE_LOW_P6              0x00004000
#define DW1000_GPIO_OUTPUT_STATE_LOW_P7              0x00008000
#define DW1000_GPIO_OUTPUT_STATE_LOW_P8              0x00100000
#define DW1000_GPIO_OUTPUT_STATE_HIGH_P0             0x00000011
#define DW1000_GPIO_OUTPUT_STATE_HIGH_P1             0x00000022
#define DW1000_GPIO_OUTPUT_STATE_HIGH_P2             0x00000044
#define DW1000_GPIO_OUTPUT_STATE_HIGH_P3             0x00000088
#define DW1000_GPIO_OUTPUT_STATE_HIGH_P4             0x00001100
#define DW1000_GPIO_OUTPUT_STATE_HIGH_P5             0x00002200
#define DW1000_GPIO_OUTPUT_STATE_HIGH_P6             0x00004400
#define DW1000_GPIO_OUTPUT_STATE_HIGH_P7             0x00008800
#define DW1000_GPIO_OUTPUT_STATE_HIGH_P8             0x00110000

/**
 * @brief 定义GPIO原始状态原始值
 * @note  位于寄存器 GPIO_CTRL - 0x26
 *        子寄存器 GPIO_RAW - 0x28
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_GPIO_RAW_STATE_P0                     DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP0
#define DW1000_GPIO_RAW_STATE_P1                     DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP1
#define DW1000_GPIO_RAW_STATE_P2                     DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP2
#define DW1000_GPIO_RAW_STATE_P3                     DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP3
#define DW1000_GPIO_RAW_STATE_P4                     DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP4
#define DW1000_GPIO_RAW_STATE_P5                     DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP5
#define DW1000_GPIO_RAW_STATE_P6                     DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP6
#define DW1000_GPIO_RAW_STATE_P7                     DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP7
#define DW1000_GPIO_RAW_STATE_P8                     DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP8

/**
 * @brief 定义DW1000数字调谐寄存器不同数据速率和前导码长度下的值
 * @note  位于寄存器 DRX_CONF - 0x27
 *        子寄存器 DRX_TUNE1b - 0x06
 * @note 后缀PL表示前导码长度大于1024个符号
 *       后缀PM表示前导码长度处于128到1024个符号
 *       后缀PS表示前导码长度处为64个符号
 */
#define DW1000_DRX_TUNE1B_110K_PL                    0x0064
#define DW1000_DRX_TUNE1B_850K_6M8_PM                0x0020
#define DW1000_DRX_TUNE1B_6M8_PS                     0x0010


/**
 * @brief 定义DW1000 接收调谐2寄存器不同PAC和PRF对应的值
 * @note  位于寄存器 DRX_CONF - 0x27
 *        子寄存器 DRX_TUN2 - 0x08
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_DRX_TUNE2_PAC8_PRF16M                 0x311A003CUL /* 这个值与用户手册的不一样，0x311A002D */
#define DW1000_DRX_TUNE2_PAC8_PRF64M                 0x313B006BUL
#define DW1000_DRX_TUNE2_PAC16_PRF16M                0x331A0052UL
#define DW1000_DRX_TUNE2_PAC16_PRF64M                0x333B00BEUL
#define DW1000_DRX_TUNE2_PAC32_PRF16M                0x351A009AUL
#define DW1000_DRX_TUNE2_PAC32_PRF64M                0x353B015EUL
#define DW1000_DRX_TUNE2_PAC64_PRF16M                0x371A011DUL
#define DW1000_DRX_TUNE2_PAC64_PRF64M                0x373B0296UL

/* 这两个值跟优化有关，暂时不修改 */
#define DW1000_DRX_TUN2_UNCONF_SFD_TH_PRF16          0x0E
#define DW1000_DRX_TUN2_UNCONF_SFD_TH_PRF64          0x20

/**
 * @brief 定义DW1000 DRX_SFDTOC寄存器默认值
 * @note  位于寄存器 DRX_CONF - 0x27
 *        子寄存器 DRX_SFDTOC - 0x20
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_DRX_SFDTOC_DEFAULT                    0x1041
#define DW1000_SFD_TIMEOUT_DEFAULT                   DW1000_DRX_SFDTOC_DEFAULT

/**
 * @brief 定义DW1000数字调谐寄存器4H
 *        预期接收的前导码长度（符号个数）对应的值
 * @note  位于寄存器 DRX_CONF - 0x27
 *        子寄存器 DRX_TUNE4H - 0x26
 */
#define DW1000_DRX_TUNE4H_PLEN64                     0x0010
#define DW1000_DRX_TUNE4H_PLEN128_PLUS               0x0028


/**
 * @brief
 * @note  位于寄存器 RF_CONF - 0x28
 *        子寄存器 RF_CONF - 0x00
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_RF_CFG_SWITCH_TX                      (0x02UL << DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_POS)
#define DW1000_RF_CFG_SWITCH_RX                      (0x01UL << DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_POS)

#define DW1000_RF_CFG_ALL_LDO_ENABLE                 (0x1FUL << DW1000_SUB_REG_RF_CONF_BITS_LDOFEN_POS)

#define DW1000_RF_CFG_CLK_PLL_ENABLE                 (0x5UL << DW1000_SUB_REG_RF_CONF_BITS_PLLFEN_POS)
#define DW1000_RF_CFG_ALL_PLL_ENABLE                 (0x7UL << DW1000_SUB_REG_RF_CONF_BITS_PLLFEN_POS)
#define DW1000_RF_CFG_PULSE_GEN_ENABLE               (0x5UL << DW1000_SUB_REG_RF_CONF_BITS_PLLFEN_POS)

#define DW1000_RF_CFG_TX_ENABLE                      (0x2UL << DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_POS)
#define DW1000_RF_CFG_RX_ENABLE                      (0x1UL << DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_POS)

#define DW1000_RF_CFG_TXB_ALL_ENABLE                 (0x1FUL << DW1000_SUB_REG_RF_CONF_BITS_TXFEN_POS)
#define DW1000_RF_CFG_TXB_MIXER_BIAS_ENABLE          (0x7UL << DW1000_SUB_REG_RF_CONF_BITS_TXFEN_POS)
#define DW1000_RF_CFG_TXB_DISABLE                    (0x0UL << DW1000_SUB_REG_RF_CONF_BITS_TXFEN_POS)

/**
 * @brief LDO调谐寄存器默认值
 * @note  位于寄存器 RF_CONF - 0x28
 *        子寄存器 LDOTUNE - 0x30
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_LDO_TUNE_DEFAULT                      0x88888888UL

/**
 * @brief
 * @note  位于寄存器 RF_CONF - 0x2A
 *        子寄存器 TC_PG_CTRL - 0x08
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_PG_CTRL_DIR_CONV_HIGHER               (0x1UL << DW1000_SUB_REG_TC_PG_CTRL_BIT_DIR_CONV_POS)
#define DW1000_PG_CTRL_DIR_CONV_LOWER                (0x0UL << DW1000_SUB_REG_TC_PG_CTRL_BIT_DIR_CONV_POS)

#define DW1000_PG_TMEAS_MAX                          (0xFUL << DW1000_SUB_REG_TC_PG_CTRL_BITS_PG_TMEAS_POS)

/**
 * @brief 定义DW1000 发送校准测试模式
 * @note  位于寄存器 TX_CAL - 0x2A
 *        子寄存器 TC_PGTEST - 0x0C
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_TC_PGTEST_MODE_CW                     0x13
#define DW1000_TC_PGTEST_MODE_NORMAL                 0x00


/**
 * @brief 定义DW1000 保持睡眠配置
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_WCFG - 0x00
 * @note  DW1000当配置了接收和（或）发送完成之后进入睡眠模式之后，
 *        默认是当从睡眠模式中唤醒后会自动清除配置。
 *        如果配置了保持睡眠，则DW1000会在从睡眠模式中唤醒后保持
 *        之前的接收和（或）发送完成之后进入睡眠模式的配置状态。
 *        实现从唤醒后完成接收和（或）发送再次进入睡眠模式的效果。
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_PRESERVE_SLEEP                        (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_PRES_SLEEP_POS)
#define DW1000_PRESERVE_SLEEP_NOT                    (0x0UL << DW1000_SUB_REG_AON_WCFG_BIT_PRES_SLEEP_POS)
#define DW1000_PRESERVE_SLEEP_DEFAULT                DW1000_PRESERVE_SLEEP_NOT

/**
 * @brief 定义DW1000唤醒时加载Length64接收器操作参数
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_WCFG - 0x00
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_ONW_RX_OPER_PARAM_SET_LOAD_L64        (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_L64P_POS)
#define DW1000_ONW_RX_OPER_PARAM_SET_RESET           (0x0UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_L64P_POS)
#define DW1000_ONW_RX_OPER_PARAM_SET_DEFAULT         DW1000_ONW_RX_OPER_PARAM_SET_RESET

/**
 * @brief 定义DW1000唤醒时从 AON内存 上传配置到 主机接口寄存器组
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_WCFG - 0x00
 * @note  当ONW_LDC位设为1时，用户配置寄存器的值将恢复至睡眠前的配置值。
 *        当ONW_LDC位为0时，DW1000从SLEEP或DEEPSLEEP唤醒后，用户配置寄存器的值将恢复至上电复位值。
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_ONW_HIF_REG_SET_CONFIG_RESTORE        (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_LDC_POS)
#define DW1000_ONW_HIF_REG_SET_CONFIG_RESET          (0x0UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_LDC_POS)
#define DW1000_ONW_HIF_REG_SET_CONFIG_DEFAULT        DW1000_ONW_HIF_REG_SET_CONFIG_RESET

/**
 * @brief 定义DW1000唤醒时加载从OTP读取EUI到寄存器REG
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_WCFG - 0x00
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_ONW_EUI_LOAD_FROM_OTP                 (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_LEUI_POS)
#define DW1000_ONW_EUI_LOAD_RESET_VAL                (0x0UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_LEUI_POS)
#define DW1000_ONW_EUI_LOAD_DEFAULT                  DW1000_ONW_EUI_LOAD_RESET_VAL

/**
 * @brief 定义DW1000唤醒时开启接收控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_WCFG - 0x00
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_ONW_RX_TURN_ON                        (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_RX_POS)
#define DW1000_ONW_RX_NOP                            (0x0UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_RX_POS)
#define DW1000_ONW_RX_DEFAULT                        DW1000_ONW_RX_NOP

/**
 * @brief 定义DW1000唤醒时开启ADC控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_WCFG - 0x00
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_ONW_ADC_RUN                           (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_RADC_POS)
#define DW1000_ONW_ADC_NOP                           (0x0UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_RADC_POS)
#define DW1000_ONW_ADC_DEFAULT                       DW1000_ONW_ADC_NOP

/**
 * @brief 定义DW1000 直接AON内存访问使能控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CTRL - 0x02
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_DIRECT_AON_MEM_ACCESS_ENABLE_SET      (0x1UL << DW1000_SUB_REG_AON_CTRL_BIT_DCA_ENAB_POS)
#define DW1000_DIRECT_AON_MEM_ACCESS_ENABLE_CLEAR    (0x0UL << DW1000_SUB_REG_AON_CTRL_BIT_DCA_ENAB_POS)
#define DW1000_DIRECT_AON_MEM_ACCESS_ENABLE_DEFAULT  DW1000_DIRECT_AON_MEM_ACCESS_ENABLE_CLEAR

/**
 * @brief 定义DW1000 直接AON内存访问读控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CTRL - 0x02
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_DIRECT_AON_MEM_ACCESS_READ_SET        (0x1UL << DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_POS)
#define DW1000_DIRECT_AON_MEM_ACCESS_READ_CLEAR      (0x0UL << DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_POS)
#define DW1000_DIRECT_AON_MEM_ACCESS_READ_DEFAULT    DW1000_DIRECT_AON_MEM_ACCESS_READ_CLEAR

/**
 * @brief 定义DW1000 AON控制上传配置控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CTRL - 0x02
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_AON_CTRL_UPLOAD_CONFIGURATION_SET     (0x1UL << DW1000_SUB_REG_AON_CTRL_BIT_UPL_CFG_POS)
#define DW1000_AON_CTRL_UPLOAD_CONFIGURATION_CLEAR   (0x0UL << DW1000_SUB_REG_AON_CTRL_BIT_UPL_CFG_POS)
#define DW1000_AON_CTRL_UPLOAD_CONFIGURATION_DEFAULT DW1000_AON_CTRL_UPLOAD_CONFIGURATION_CLEAR

/**
 * @brief 定义DW1000 AON控制保存控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CTRL - 0x02
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_AON_CTRL_SAVE_SET                     (0x1UL << DW1000_SUB_REG_AON_CTRL_BIT_SAVE_POS)
#define DW1000_AON_CTRL_SAVE_CLEAR                   (0x0UL << DW1000_SUB_REG_AON_CTRL_BIT_SAVE_POS)
#define DW1000_AON_CTRL_SAVE_DEFAULT                 DW1000_AON_CTRL_SAVE_CLEAR

/**
 * @brief 定义 AON直接地址寄存器相关地址定义
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_ADDR - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_AON_ADDR_LPOSC_CAL_LOWER              (0x75)
#define DW1000_AON_ADDR_LPOSC_CAL_UPPER              (0x76)


/**
 * @brief 定义睡眠使能控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CFG0 - 0x06
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SLEEP_CONFIG_ENABLE                   (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_SLEEP_EN_POS)
#define DW1000_SLEEP_CONFIG_DISABLE                  (0x0UL << DW1000_SUB_REG_AON_CFG0_BIT_SLEEP_EN_POS)
#define DW1000_SLEEP_CONFIG_DEFAULT                  DW1000_SLEEP_CONFIG_DISABLE

/**
 * @brief 定义唤醒引脚控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CFG0 - 0x06
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_WAKEUP_BY_PIN_ENABLE                  (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_WAKE_PIN_POS)
#define DW1000_WAKEUP_BY_PIN_DISABLE                 (0x0UL << DW1000_SUB_REG_AON_CFG0_BIT_WAKE_PIN_POS)
#define DW1000_WAKEUP_BY_PIN_DEFAULT                 DW1000_WAKEUP_BY_PIN_ENABLE

/**
 * @brief 定义唤醒SPI访问控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_WAKEUP_BY_SPI_ACCESS_ENABLE           (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_WAKE_SPI_POS)
#define DW1000_WAKEUP_BY_SPI_ACCESS_DISABLE          (0x0UL << DW1000_SUB_REG_AON_CFG0_BIT_WAKE_SPI_POS)
#define DW1000_WAKEUP_BY_SPI_ACCESS_DEFAULT          DW1000_WAKEUP_BY_SPI_ACCESS_ENABLE

/**
 * @brief 定义唤醒睡眠计数器控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CFG0 - 0x06
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_WAKEUP_BY_SLEEP_COUTNTER_ENABLE       (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_WAKE_CNT_POS)
#define DW1000_WAKEUP_BY_SLEEP_COUTNTER_DISABLE      (0x0UL << DW1000_SUB_REG_AON_CFG0_BIT_WAKE_CNT_POS)
#define DW1000_WAKEUP_BY_SLEEP_COUTNTER_DEFAULT      DW1000_WAKEUP_BY_SLEEP_COUTNTER_ENABLE

/**
 * @brief 定义低功耗分频器使能控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CFG0 - 0x06
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_LOW_POWER_DIVIDER_ENABLE              (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_LPDIV_EN_POS)
#define DW1000_LOW_POWER_DIVIDER_DISABLE             (0x0UL << DW1000_SUB_REG_AON_CFG0_BIT_LPDIV_EN_POS)
#define DW1000_LOW_POWER_DIVIDER_DEFAULT             DW1000_LOW_POWER_DIVIDER_DISABLE

/**
 * @brief 定义睡眠计数器使能控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CFG1 - 0x0A
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SLEEP_COUNTER_ENABLE                  (0x1UL << DW1000_SUB_REG_AON_CFG1_BIT_SLEEP_CEN_POS)
#define DW1000_SLEEP_COUNTER_DISABLE                 (0x0UL << DW1000_SUB_REG_AON_CFG1_BIT_SLEEP_CEN_POS)
#define DW1000_SLEEP_COUNTER_DEFAULT                 DW1000_SLEEP_COUNTER_ENABLE

/**
 * @brief 定义低功耗振荡器校准使能控制
 * @note  位于寄存器AON - 0x2C
 *        子寄存器 AON_CFG1 - 0x0A
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_LOW_POWER_OSC_CALIBRATE_ENABLE        (0x1UL << DW1000_SUB_REG_AON_CFG1_BIT_LPOSC_CAL_POS)
#define DW1000_LOW_POWER_OSC_CALIBRATE_DISABLE       (0x0UL << DW1000_SUB_REG_AON_CFG1_BIT_LPOSC_CAL_POS)
#define DW1000_LOW_POWER_OSC_CALIBRATE_DEFAULT       DW1000_LOW_POWER_OSC_CALIBRATE_ENABLE

/**
 * @brief 定义OTP中LDO调谐值地址
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_ADDR - 0x04
 */
#define DW1000_OTP_ADDRESS_LDO_TUNE                  0x04
#define DW1000_OTP_ADDRESS_PART_ID                   0x06
#define DW1000_OTP_ADDRESS_LOT_ID                    0x07
#define DW1000_OTP_ADDRESS_REF_VOLT                  0x08
#define DW1000_OTP_ADDRESS_REF_TEMP                  0x09
#define DW1000_OTP_ADDRESS_OTP_REV                   0x1E
#define DW1000_OTP_ADDRESS_XTAL_TRIM                 0x1E
#define DW1000_OTP_ADDRESS_OTP_REV_XTAL_TRIM         0x1E


/**
 * @brief 定义OTP模式选择控制
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_CTRL - 0x06
 *        【根据官方库代码推测出来的】
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_OTP_MODE_REGISTER_MR                  (0x0UL << DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_POS)
#define DW1000_OTP_MODE_REGISTER_MRA                 (0x1UL << DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_POS)
#define DW1000_OTP_MODE_REGISTER_MRB                 (0x2UL << DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_POS)
#define DW1000_OTP_MODE_REGISTER_DEFAULT             DW1000_OTP_MODE_REGISTER_MR

/**
 * @brief 定义OTP模式选择控制
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_CTRL - 0x06
 *        【根据官方库代码推测出来的】
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_OTP_MODE_SELECT_SET                   (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_POS)
#define DW1000_OTP_MODE_SELECT_CLEAR                 (0x0UL << DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_POS)
#define DW1000_OTP_MODE_SELECT_DEFAULT               DW1000_OTP_MODE_SELECT_CLEAR

/**
 * @brief 定义OTP辅助更新控制
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_CTRL - 0x06
 *        【根据官方库代码推测出来的】
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_OTP_AUX_UPDATE_SET                    (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_POS)
#define DW1000_OTP_AUX_UPDATE_CLEAR                  (0x0UL << DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_POS)
#define DW1000_OTP_AUX_UPDATE_DEFAULT                DW1000_OTP_AUX_UPDATE_CLEAR

/**
 * @brief 定义OTP编程控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_OTP_PROGRAM_SET                       (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_OTPPROG_POS)
#define DW1000_OTP_PROGRAM_CLEAR                     (0x0UL << DW1000_SUB_REG_OTP_CTRL_BIT_OTPPROG_POS)
#define DW1000_OTP_PROGRAM_DEFAULT                   DW1000_OTP_PROGRAM_CLEAR

/**
 * @brief 定义OTP模式写入控制
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_CTRL - 0x06
 *        【根据官方库代码推测出来的】
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_OTP_MODE_REGISTER_WRITE_SET           (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_POS)
#define DW1000_OTP_MODE_REGISTER_WRITE_CLEAR         (0x0UL << DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_POS)
#define DW1000_OTP_MODE_REGISTER_WRITE_DEFAULT       DW1000_OTP_MODE_REGISTER_WRITE_CLEAR

/**
 * @brief 定义OTP编程状态
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_STAT - 0x08
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_OTP_STATUS_PROGRAMMING_DONE           (0x1UL << DW1000_SUB_REG_OTP_STAT_BIT_OTPPRGD_POS)
#define DW1000_OTP_STATUS_PROGRAMMING_NOT            (0x0UL << DW1000_SUB_REG_OTP_STAT_BIT_OTPPRGD_POS)
#define DW1000_OTP_STATUS_PROGRAMMING_DEFAULT        DW1000_OTP_STATUS_PROGRAMMING_NOT

#define DW1000_IS_OTP_PROGRAMMING_DONE(reg)          ((DW1000_READ_BITS(reg, DW1000_SUB_REG_OTP_STAT_BIT_OTPPRGD_MSK) == (DW1000_OTP_STATUS_PROGRAMMING_DONE)) ? 1UL : 0UL)

/**
 * @brief 定义DW1000 LDE微码加载控制
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_CTRL - 0x06
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_LDE_LOAD_UCODE_ENABLE                 (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_LDELOAD_POS)
#define DW1000_LDE_LOAD_UCODE_DISABLE                (0x0UL << DW1000_SUB_REG_OTP_CTRL_BIT_LDELOAD_POS)
#define DW1000_LDE_LOAD_UCODE_DEFAULT                DW1000_LDE_LOAD_UCODE_DISABLE

/**
 * @brief 定义DW1000 运行参数集加载控制
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_SF - 0x12
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_OPER_PARAM_SET_LOAD_ENABLE            (0x1UL << DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK_POS)
#define DW1000_OPER_PARAM_SET_LOAD_DISABLE           (0x0UL << DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK_POS)
#define DW1000_OPER_PARAM_SET_LOAD_DEFAULT           DW1000_OPER_PARAM_SET_LOAD_DISABLE

/**
 * @brief 定义DW1000 LDO调谐参数加载控制
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_SF - 0x12
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_LDO_TUNE_CAL_PARAM_LOAD_ENABLE        (0x1UL << DW1000_SUB_REG_OTP_SF_BIT_LDOTUNE_POS)
#define DW1000_LDO_TUNE_CAL_PARAM_LOAD_DISABLE       (0x0UL << DW1000_SUB_REG_OTP_SF_BIT_LDOTUNE_POS)
#define DW1000_LDO_TUNE_CAL_PARAM_LOAD_DEFAULT       DW1000_LDO_TUNE_CAL_PARAM_LOAD_DISABLE

/**
 * @brief 定义DW1000 运行参数集选择
 * @note  位于寄存器 OTP_IF - 0x2D
 *        子寄存器 OTP_SF - 0x12
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_OPER_PARAM_SET_SELECT_L64             (0x0UL << DW1000_SUB_REG_OTP_SF_BIT_OPS_SEL_POS)
#define DW1000_OPER_PARAM_SET_SELECT_TIGHT           (0x1UL << DW1000_SUB_REG_OTP_SF_BIT_OPS_SEL_POS)
#define DW1000_OPER_PARAM_SET_SELECT_DEFAULT         (0x2UL << DW1000_SUB_REG_OTP_SF_BIT_OPS_SEL_POS)
// #define DW1000_OPER_PARAM_SET_SELECT_REVERSED                                       (0x3UL << DW1000_SUB_REG_OTP_SF_BIT_OPS_SEL_POS) /* 保留至，不应该选择该值 */


/**
 * @brief 定义DW1000 LDE参数配置
 * @note  位于寄存器 LDE_IF - 0x2E
 *        子寄存器 LDE_CFG1 - 0x0806
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_PEAK_MULTIPLIER_DEFAULT               (0x3UL << DW1000_SUB_REG_LDE_CFG1_BITS_PMULT_POS)
#define DW1000_NOISE_THRES_MULTIPLIER_DEFAULT        (0xCUL << DW1000_SUB_REG_LDE_CFG1_BITS_NT_MULT_POS)
#define DW1000_LDE_PARAM1                            (0x6D)

/**
 * @brief 定义DW1000 LDE调谐参数配置
 * @note  位于寄存器 LDE_IF - 0x2E
 *       子寄存器 LDE_CFG2 - 0x1806
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_LDE_TUNE_PARAM_IN_PRF16MHz            (0x1607)
#define DW1000_LDE_TUNE_PARAM_IN_PRF64MHz            (0x0607)

/**
 * @brief
 * @note  位于寄存器 DIG_DIAG - 0x2F
 *        子寄存器 EVC_CTRL - 0x00
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_EVENT_COUNT_ENABLE                    (0x1UL << DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN_POS)
#define DW1000_EVENT_COUNT_DISABLE                   (0x0UL << DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN_POS)
#define DW1000_EVENT_COUNT_DEFAULT                   DW1000_EVENT_COUNT_DISABLE

#define DW1000_EVENT_COUNT_CLEAR                     (0x1UL << DW1000_SUB_REG_EVC_CTRL_BIT_EVC_CLR_POS)
#define DW1000_EVENT_COUNT_NOT_CLEAR                 (0x0UL << DW1000_SUB_REG_EVC_CTRL_BIT_EVC_CLR_POS)
#define DW1000_EVENT_COUNT_CLEAR_DEFAULT             DW1000_EVENT_COUNT_NOT_CLEAR

/**
 * @brief
 * @note  位于寄存器 DIG_DIAG - 0x2F
 *        子寄存器 DIAG_TMC - 0x24
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_TEST_MODE_TX_PWR_SPECTRUM_ENABLE      (0x1UL << DW1000_SUB_REG_DIAG_TMC_BIT_TX_PSTM_POS)
#define DW1000_TEST_MODE_TX_PWR_SPECTRUM_DISABLE     (0x0UL << DW1000_SUB_REG_DIAG_TMC_BIT_TX_PSTM_POS)

/**
 * @brief 定义系统时钟选择
 * @note  位于寄存器PMSC - 0x36
 *        子寄存器 PMSC_CTRL0 - 0x00
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SYSTEM_CLOCK_SELECT_AUTO              (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_POS)
#define DW1000_SYSTEM_CLOCK_SELECT_FORCE_XTI         (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_POS)
#define DW1000_SYSTEM_CLOCK_SELECT_FORCE_PLL         (0x2UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_POS)
// #define DW1000_SYSTEM_CLOCK_ELECT_RESERVED                       (0x3UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_POS) /* 该值不应使用 */

/**
 * @brief 定义接收器时钟选择
 * @note  位于寄存器PMSC - 0x36
 *        子寄存器 PMSC_CTRL0 - 0x00
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_RX_CLOCK_SELECT_AUTO                  (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_POS)
#define DW1000_RX_CLOCK_SELECT_FORCE_XTI             (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_POS)
#define DW1000_RX_CLOCK_SELECT_FORCE_PLL             (0x2UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_POS)
#define DW1000_RX_CLOCK_SELECT_FORCE_OFF             (0x3UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_POS)

/**
 * @brief 定义发送器时钟选择
 * @note  位于寄存器PMSC - 0x36
 *        子寄存器 PMSC_CTRL0 - 0x00
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_TX_CLOCK_SELECT_AUTO                  (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_POS)
#define DW1000_TX_CLOCK_SELECT_FORCE_XTI             (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_POS)
#define DW1000_TX_CLOCK_SELECT_FORCE_PLL             (0x2UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_POS)
#define DW1000_TX_CLOCK_SELECT_FORCE_OFF             (0x3UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_POS)

/**
 * @brief 定义累加器时钟控制
 * @note  位于寄存器PMSC - 0x36
 *        子寄存器 PMSC_CTRL0 - 0x00
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_ACC_CLOCK_FORCE_ENABLE                (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_POS)
#define DW1000_ACC_CLOCK_FORCE_DISABLE               (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_POS)
#define DW1000_ACC_CLOCK_DEFAULT                     DW1000_ACC_CLOCK_FORCE_DISABLE

/**
 * @brief 定义ADC转换时钟控制
 * @note  位于寄存器PMSC - 0x36
 *       子寄存器 PMSC_CTRL0 - 0x00
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_ADC_CONVERT_CLOCK_ENABLE              (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE_POS)
#define DW1000_ADC_CONVERT_CLOCK_DISABLE             (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE_POS)
#define DW1000_ADC_CONVERT_CLOCK_DEFAULT             DW1000_ADC_CONVERT_CLOCK_DISABLE

/**
 * @brief 定义累加器存储时钟控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_ACC_MEM_CLOCK_ENABLE                  (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE_POS)
#define DW1000_ACC_MEM_CLOCK_DISABLE                 (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE_POS)
#define DW1000_ACC_MEM_CLOCK_DEFAULT                 DW1000_ACC_MEM_CLOCK_DISABLE

/**
 * @brief 定义GIO时钟控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_GPIO_CLOCK_ENABLE                     (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE_POS)
#define DW1000_GPIO_CLOCK_DISABLE                    (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE_POS)
#define DW1000_GPIO_CLOCK_DEFAULT                    DW1000_GPIO_CLOCK_DISABLE

/**
 * @brief 定义GPIO复位控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_GPIO_RESET_ACTIVE                     (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN_POS)
#define DW1000_GPIO_RESET_DEACTIVE                   (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN_POS)
#define DW1000_GPIO_RESET_DEFAULT                    DW1000_GPIO_RESET_ACTIVE

/**
 * @brief 定义GPIO消抖时钟控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_GPIO_DEBOUNCE_CLOCK_ENABLE            (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDCE_POS)
#define DW1000_GPIO_DEBOUNCE_CLOCK_DISABLE           (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDCE_POS)
#define DW1000_GPIO_DEBOUNCE_CLOCK_DEFAULT           DW1000_GPIO_DEBOUNCE_CLOCK_DISABLE

/**
 * @brief 定义GPIO消抖复位控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_GPIO_DEBOUNCE_RESET_ACTIVE            (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDRN_POS)
#define DW1000_GPIO_DEBOUNCE_RESET_DEACTIVE          (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDRN_POS)
#define DW1000_GPIO_DEBOUNCE_RESET_DEFAULT           DW1000_GPIO_DEBOUNCE_RESET_ACTIVE

/**
 * @brief 定义千赫兹时钟控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_KHZ_CLOCK_ENABLE                      (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_KHZCE_POS)
#define DW1000_KHZ_CLOCK_DISABLE                     (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_KHZCE_POS)
#define DW1000_KHZ_CLOCK_DEFAULT                     DW1000_KHZ_CLOCK_DISABLE

/**
 * @brief 定义PLL2序列控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_PLL2_SEQ_NORMAL                       (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_POS)
#define DW1000_PLL2_SEQ_TX_SEQ_CTRL                  (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_POS)
#define DW1000_PLL2_SEQ_RX_SNIFF                     (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_POS)
#define DW1000_PLL2_SEQ_DEFAULT                      DW1000_PLL2_SEQ_NORMAL

/**
 * @brief 定义软件复位控制
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SOFTRESET_SET                         (0x0UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_SOFTRESET_POS)
#define DW1000_SOFTRESET_CLEAR                       (0xFUL << DW1000_SUB_REG_PMSC_CTRL0_BITS_SOFTRESET_POS)
#define DW1000_SOFTRESET_DEFAULT                     DW1000_SOFTRESET_CLEAR

/**
 * @brief 定义尝试接收后自动进入INIT状态
 * @note  位于寄存器PMSC - 0x36
 *       子寄存器 PMSC_CTRL1 - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_STATE_AFTER_RX_AUTO_TO_INIT           (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_ARX2INIT_POS)
#define DW1000_STATE_AFTER_RX_NOT_TO_INIT            (0x0UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_ARX2INIT_POS)
#define DW1000_STATE_AFTER_RX_DEFAULT                DW1000_STATE_AFTER_RX_NOT_TO_INIT

/**
 * @brief 定义DW1000 模拟射频电路控制
 * @note  位于寄存器 PMSC - 0x36
 *       子寄存器 PMSC_CTRL1 - 0x04
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_PMSC_ANALOG_RF_CTRL_DISABLE           (0x0UL << DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_POS)
#define DW1000_PMSC_ANALOG_RF_CTRL_ENABLE            (0xE7UL << DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_POS)
#define DW1000_PMSC_ANALOG_RF_CTRL_DEFAULT           DW1000_PMSC_ANALOG_RF_CTRL_ENABLE

/**
 * @brief 定义DW1000 发送后自动进入睡眠状态
 * @note  位于寄存器 PMSC - 0x36
 *       子寄存器 PMSC_CTRL1 - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_STATE_AFTER_TX_AUTO_TO_SLEEP          (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP_POS)
#define DW1000_STATE_AFTER_TX_NOT_TO_SLEEP           (0x0UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP_POS)
#define DW1000_STATE_AFTER_TX_DEFAULT                DW1000_STATE_AFTER_TX_NOT_TO_SLEEP

/**
 * @brief 定义DW1000 接收后自动进入睡眠状态
 * @note  位于寄存器 PMSC - 0x36
 *       子寄存器 PMSC_CTRL1 - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_STATE_AFTER_RX_AUTO_TO_SLEEP          (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_POS)
#define DW1000_STATE_AFTER_RX_NOT_TO_SLEEP           (0x0UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_POS)
#define DW1000_STATE_AFTER_RX_SLEEP_DEFAULT          DW1000_STATE_AFTER_RX_NOT_TO_SLEEP

/**
 * @brief 定义DW1000 SNOOZE模式使能控制
 * @note  位于寄存器 PMSC - 0x36
 *       子寄存器 PMSC_CTRL1 - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_MODE_SNOOZE_ENABLE                    (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_POS)
#define DW1000_MODE_SNOOZE_DISABLE                   (0x0UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_POS)
#define DW1000_MODE_SNOOZE_DEFAULT                   DW1000_MODE_SNOOZE_DISABLE

/**
 * @brief 定义DW1000 1GHz时钟使能控制
 * @note  位于寄存器 PMSC - 0x36
 *       子寄存器 PMSC_CTRL1 - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_1GHz_CLOCK_ENABLE                     (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_PLLSYN_POS)
#define DW1000_1GHz_CLOCK_DISABLE                    (0x0UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_PLLSYN_POS)
#define DW1000_1GHz_CLOCK_DEFAULT                    DW1000_1GHz_CLOCK_DISABLE

/**
 * @brief 定义DW1000 SNOOZE计时器重复触发控制
 * @note  位于寄存器 PMSC - 0x36
 *       子寄存器 PMSC_CTRL1 - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_SNOOZE_TIMER_REPEAT                   (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZR_POS)
#define DW1000_SNOOZE_TIMER_ONCE                     (0x0UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZR_POS)
#define DW1000_SNOOZE_TIMER_DEFAULT                  DW1000_SNOOZE_TIMER_ONCE

/**
 * @brief 定义DW1000 接收复位控制
 * @note  位于寄存器 PMSC - 0x36
 *       子寄存器 PMSC_CTRL1 - 0x04
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_RX_RESET_ENABLE_32BIT                 (0x0UL << 28U)
#define DW1000_RX_RESET_DISABLE_32BIT                (0x1UL << 28U)

#define DW1000_RX_RESET_ENABLE_8BIT                  (0x0UL << 4U)
#define DW1000_RX_RESET_DISABLE_8BIT                 (0x1UL << 4U)

#define DW1000_LOW_POWER_LISTENING_MODE_MASK         (DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_MSK | DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_MSK)
#define DW1000_LOW_POWER_LISTENING_MODE_ENABLE       (DW1000_STATE_AFTER_RX_AUTO_TO_SLEEP | DW1000_MODE_SNOOZE_ENABLE)
#define DW1000_LOW_POWER_LISTENING_MODE_DISABLE      (DW1000_STATE_AFTER_RX_NOT_TO_SLEEP | DW1000_MODE_SNOOZE_DISABLE)

#define DW1000_PMSC_CTRL_RF_SUB_SYS_DISABLE          (0x00UL << DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_POS)
#define DW1000_PMSC_CTRL_RF_SUB_SYS_ENABLE           (0xE7UL << DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_POS)
#define DW1000_PMSC_CTRL_RF_SUB_SYS_DEFAULT          DW1000_PMSC_CTRL_RF_SUB_SYS_ENABLE


/**
 * @brief
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_TX_MODE_IMMEDIATE                     0x00
#define DW1000_TX_MODE_DELAYED                       0x01
#define DW1000_TX_MODE_EXPECT_RESPONSE               0x02


#define DW1000_INT_EVENT_FRAME_SENT                  DW1000_REG_SYS_MASK_BIT_MTXFRB
#define DW1000_INT_EVENT_FRAME_RECIVED               DW1000_REG_SYS_MASK_BIT_MRXFCG
#define DW1000_INT_EVENT_RX_PHR_ERROR                DW1000_REG_SYS_MASK_BIT_MRXPHE
#define DW1000_INT_EVENT_RX_CRC_ERROR                DW1000_REG_SYS_MASK_BIT_MRXFCE
#define DW1000_INT_EVENT_RX_SYNC_LOST                DW1000_REG_SYS_MASK_BIT_MRXRFSL
#define DW1000_INT_EVENT_RX_TIMEOUT                  DW1000_REG_SYS_MASK_BIT_MRXRFTO
#define DW1000_INT_EVENT_SFD_TIMEOUT                 DW1000_REG_SYS_MASK_BIT_MRXSFDTO
#define DW1000_INT_EVENT_FRAME_REJECTED              DW1000_REG_SYS_MASK_BIT_MAFFREJ

#endif
