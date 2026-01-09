#ifndef __DRV_DW1000_REG_H
#define __DRV_DW1000_REG_H

// 宏函数 产生指定bit位为1的数值
#define DW1000_REG_BIT(bit)                             (1UL << bit)
// 宏函数 产生指定bit范围内全1的数值
#define DW1000_REG_BIT_RANGE(end, start)                (((1UL << ((end) - (start) + 1)) - 1) << (start))


#define DW1000_SPI_CMD_POS                              (7U)
#define DW1000_SPI_CMD_MASK                             (0x1UL << DW1000_SPI_CMD_POS)
// #define DW1000_SPI_CMD_MASK                            DW1000_REG_BIT(7)
#define DW1000_SPI_CMD                                  DW1000_SPI_CMD_MASK

#define DW1000_SPI_SUB_ADDR_POS                         (6U)
#define DW1000_SPI_SUB_ADDR_MASK                        (0x1UL << DW1000_SPI_SUB_ADDR_POS)
// #define DW1000_SPI_SUB_ADDR_MASK                       DW1000_REG_BIT(6)
#define DW1000_SPI_SUB_ADDR                             DW1000_SPI_SUB_ADDR_MASK

#define DW1000_SPI_EXT_SUB_ADDR_POS                     (7U)
#define DW1000_SPI_EXT_SUB_ADDR_MASK                    (0x1UL << DW1000_SPI_EXT_SUB_ADDR_POS)
// #define DW1000_SPI_EXT_SUB_ADDR_MASK                   DW1000_REG_BIT(7)
#define DW1000_SPI_EXT_SUB_ADDR                         DW1000_SPI_EXT_SUB_ADDR_MASK


/***********************************************************************************************
 * @brief 寄存器文件 0x00 - DEV_ID 设备标识符 [只读]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_DEV_ID                               0x00
#define DW1000_REG_DEV_ID_LEN                           (4) /* 字节 */
/*--------------------------------------- DEV_ID 位定义 ---------------------------------------*/
/**
 * @brief Bit[31:16]: RIDTAG - Register Identification Tag 寄存器标识标签
 * @note  计划对对所有的Decadewave器件使用相同的值。该值为0xDECA。
 */
#define DW1000_REG_DEV_ID_BITS_RIDTAG_POS               (16U)
#define DW1000_REG_DEV_ID_BITS_RIDTAG_LEN               (16U) /* 位 */
#define DW1000_REG_DEV_ID_BITS_RIDTAG_MSK               (0xFFFFUL << DW1000_REG_DEV_ID_BITS_RIDTAG_POS)
#define DW1000_REG_DEV_ID_BITS_RIDTAG                   DW1000_REG_DEV_ID_BITS_RIDTAG_MSK

/**
 * @brief Bit[15:8]: MODEL - 型号标识设备
 * @note  DW1000的设备类型为0x01。
 */
#define DW1000_REG_DEV_ID_BITS_MODEL_POS                (8U)
#define DW1000_REG_DEV_ID_BITS_MODEL_LEN                (8U) /* 位 */
#define DW1000_REG_DEV_ID_BITS_MODEL_MSK                (0xFFUL << DW1000_REG_DEV_ID_BITS_MODEL_POS)
#define DW1000_REG_DEV_ID_BITS_MODEL                    DW1000_REG_DEV_ID_BITS_MODEL_MSK

/**
 * @brief Bit[7:4]: VER - Verison 版本
 * @note  如果生产的新版本与之前的版本有重大差异，则此数字将被更新。
 */
#define DW1000_REG_DEV_ID_BITS_VER_POS                  (4U)
#define DW1000_REG_DEV_ID_BITS_VER_LEN                  (4U) /* 位 */
#define DW1000_REG_DEV_ID_BITS_VER_MSK                  (0xFUL << DW1000_REG_DEV_ID_BITS_VER_POS)
#define DW1000_REG_DEV_ID_BITS_VER                      DW1000_REG_DEV_ID_BITS_VER_MSK

/**
 * @brief Bit[3:0]: REV - Revision 修订
 * @note  当进行小幅更正和操作更改时会更新此编号
 */
#define DW1000_REG_DEV_ID_BITS_REV_POS                  (0U)
#define DW1000_REG_DEV_ID_BITS_REV_LEN                  (4U) /* 位 */
#define DW1000_REG_DEV_ID_BITS_REV_MSK                  (0xFUL << DW1000_REG_DEV_ID_BITS_REV_POS)
#define DW1000_REG_DEV_ID_BITS_REV                      DW1000_REG_DEV_ID_BITS_REV_MSK


/***********************************************************************************************
 * @brief 寄存器文件 0x01 - EUI 扩展唯一标识符 [读写]
 * @note  该寄存器可以按字节访问，也就是可以作为8个8字节的子寄存器进行访问
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_EUI                                  0x01
#define DW1000_REG_EUI_LEN                              (8) /* 字节 */
/*====================================== 子寄存器偏移地址 ======================================*/
#define DW1000_SUB_REG_EUI_0_OFFSET                     0x00 /* 扩展标识符的Bit[7:0] */
#define DW1000_SUB_REG_EUI_1_OFFSET                     0x01 /* 扩展标识符的Bit[15:8] */
#define DW1000_SUB_REG_EUI_2_OFFSET                     0x02 /* 扩展标识符的Bit[23:16] */
#define DW1000_SUB_REG_EUI_3_OFFSET                     0x03 /* 扩展标识符的Bit[31:24] */
#define DW1000_SUB_REG_EUI_4_OFFSET                     0x04 /* 扩展标识符的Bit[39:32] */
#define DW1000_SUB_REG_EUI_5_OFFSET                     0x05 /* 制造商ID的Bit[7:0] */
#define DW1000_SUB_REG_EUI_6_OFFSET                     0x06 /* 制造商ID的Bit[15:8] */
#define DW1000_SUB_REG_EUI_7_OFFSET                     0x07 /* 制造商ID的Bit[23:16] */


/***********************************************************************************************
 * @brief 寄存器文件 0x02
 * @note  保留
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x03 - PANADR PAN标识符和短地址 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_PANADR                               0x03
#define DW1000_REG_PANADR_LEN                           (4) /* 字节 */
/*--------------------------------------- PANADR 位定义 ---------------------------------------*/
#define DW1000_REG_PANADR_BITS_PAN_ID_POS               (16U)
#define DW1000_REG_PANADR_BITS_PAN_ID_LEN               (16U) /* 位 */
#define DW1000_REG_PANADR_BITS_PAN_ID_MSK               (0xFFFFUL << DW1000_REG_PANADR_BITS_PAN_ID_POS)
#define DW1000_REG_PANADR_BITS_PAN_ID                   DW1000_REG_PANADR_BITS_PAN_ID_MSK

#define DW1000_REG_PANADR_BITS_SHORT_ADDR_POS           (0U)
#define DW1000_REG_PANADR_BITS_SHORT_ADDR_LEN           (16U) /* 位 */
#define DW1000_REG_PANADR_BITS_SHORT_ADDR_MSK           (0xFFFFUL << DW1000_REG_PANADR_BITS_SHORT_ADDR_POS)
#define DW1000_REG_PANADR_BITS_SHORT_ADDR               DW1000_REG_PANADR_BITS_SHORT_ADDR_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x04 - SYS_CFG 系统配置位图 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_SYS_CFG                              0x04
#define DW1000_REG_SYS_CFG_LEN                          (4) /* 字节 */
/*--------------------------------------- SYS_CFG 位定义 ---------------------------------------*/
#define DW1000_REG_SYS_CFG_BIT_AACKPEND_POS             (31U)
#define DW1000_REG_SYS_CFG_BIT_AACKPEND_MSK             (0x1UL << DW1000_REG_SYS_CFG_BIT_AACKPEND_POS)
#define DW1000_REG_SYS_CFG_BIT_AACKPEND                 DW1000_REG_SYS_CFG_BIT_AACKPEND_MSK

#define DW1000_REG_SYS_CFG_BIT_AUTOACK_POS              (30U)
#define DW1000_REG_SYS_CFG_BIT_AUTOACK_MSK              (0x1UL << DW1000_REG_SYS_CFG_BIT_AUTOACK_POS)
#define DW1000_REG_SYS_CFG_BIT_AUTOACK                  DW1000_REG_SYS_CFG_BIT_AUTOACK_MSK

#define DW1000_REG_SYS_CFG_BIT_RXAUTR_POS               (29U)
#define DW1000_REG_SYS_CFG_BIT_RXAUTR_MSK               (0x1UL << DW1000_REG_SYS_CFG_BIT_RXAUTR_POS)
#define DW1000_REG_SYS_CFG_BIT_RXAUTR                   DW1000_REG_SYS_CFG_BIT_RXAUTR_MSK

#define DW1000_REG_SYS_CFG_BIT_RXWTOE_POS               (28U)
#define DW1000_REG_SYS_CFG_BIT_RXWTOE_MSK               (0x1UL << DW1000_REG_SYS_CFG_BIT_RXWTOE_POS)
#define DW1000_REG_SYS_CFG_BIT_RXWTOE                   DW1000_REG_SYS_CFG_BIT_RXWTOE_MSK

#define DW1000_REG_SYS_CFG_BIT_RXM110K_POS              (22U)
#define DW1000_REG_SYS_CFG_BIT_RXM110K_MSK              (0x1UL << DW1000_REG_SYS_CFG_BIT_RXM110K_POS)
#define DW1000_REG_SYS_CFG_BIT_RXM110K                  DW1000_REG_SYS_CFG_BIT_RXM110K_MSK

#define DW1000_REG_SYS_CFG_BIT_DIS_STXP_POS             (18U)
#define DW1000_REG_SYS_CFG_BIT_DIS_STXP_MSK             (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_STXP_POS)
#define DW1000_REG_SYS_CFG_BIT_DIS_STXP                 DW1000_REG_SYS_CFG_BIT_DIS_STXP_MSK

#define DW1000_REG_SYS_CFG_BITS_PHR_MODE_POS            (16U)
#define DW1000_REG_SYS_CFG_BITS_PHR_MODE_LEN            (2U) /* 位 */
#define DW1000_REG_SYS_CFG_BITS_PHR_MODE_MSK            (0x3UL << DW1000_REG_SYS_CFG_BITS_PHR_MODE_POS)
#define DW1000_REG_SYS_CFG_BITS_PHR_MODE                DW1000_REG_SYS_CFG_BITS_PHR_MODE_MSK

#define DW1000_REG_SYS_CFG_BIT_FCS_INIT2F_POS           (15U)
#define DW1000_REG_SYS_CFG_BIT_FCS_INIT2F_MSK           (0x1UL << DW1000_REG_SYS_CFG_BIT_FCS_INIT2F_POS)
#define DW1000_REG_SYS_CFG_BIT_FCS_INIT2F               DW1000_REG_SYS_CFG_BIT_FCS_INIT2F_MSK

#define DW1000_REG_SYS_CFG_BIT_DIS_RSDE_POS             (14U)
#define DW1000_REG_SYS_CFG_BIT_DIS_RSDE_MSK             (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_RSDE_POS)
#define DW1000_REG_SYS_CFG_BIT_DIS_RSDE                 DW1000_REG_SYS_CFG_BIT_DIS_RSDE_MSK

#define DW1000_REG_SYS_CFG_BIT_DIS_PHE_POS              (13U)
#define DW1000_REG_SYS_CFG_BIT_DIS_PHE_MSK              (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_PHE_POS)
#define DW1000_REG_SYS_CFG_BIT_DIS_PHE                  DW1000_REG_SYS_CFG_BIT_DIS_PHE_MSK

#define DW1000_REG_SYS_CFG_BIT_DIS_DRXB_POS             (12U)
#define DW1000_REG_SYS_CFG_BIT_DIS_DRXB_MSK             (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_DRXB_POS)
#define DW1000_REG_SYS_CFG_BIT_DIS_DRXB                 DW1000_REG_SYS_CFG_BIT_DIS_DRXB_MSK

#define DW1000_REG_SYS_CFG_BIT_DIS_FCE_POS              (11U)
#define DW1000_REG_SYS_CFG_BIT_DIS_FCE_MSK              (0x1UL << DW1000_REG_SYS_CFG_BIT_DIS_FCE_POS)
#define DW1000_REG_SYS_CFG_BIT_DIS_FCE                  DW1000_REG_SYS_CFG_BIT_DIS_FCE_MSK

#define DW1000_REG_SYS_CFG_BIT_SPI_EDGE_POS             (10U)
#define DW1000_REG_SYS_CFG_BIT_SPI_EDGE_MSK             (0x1UL << DW1000_REG_SYS_CFG_BIT_SPI_EDGE_POS)
#define DW1000_REG_SYS_CFG_BIT_SPI_EDGE                 DW1000_REG_SYS_CFG_BIT_SPI_EDGE_MSK

#define DW1000_REG_SYS_CFG_BIT_HIRQ_POL_POS             (9U)
#define DW1000_REG_SYS_CFG_BIT_HIRQ_POL_MSK             (0x1UL << DW1000_REG_SYS_CFG_BIT_HIRQ_POL_POS)
#define DW1000_REG_SYS_CFG_BIT_HIRQ_POL                 DW1000_REG_SYS_CFG_BIT_HIRQ_POL_MSK

#define DW1000_REG_SYS_CFG_BIT_FFA5_POS                 (8U)
#define DW1000_REG_SYS_CFG_BIT_FFA5_MSK                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFA5_POS)
#define DW1000_REG_SYS_CFG_BIT_FFA5                     DW1000_REG_SYS_CFG_BIT_FFA5_MSK

#define DW1000_REG_SYS_CFG_BIT_FFA4_POS                 (7U)
#define DW1000_REG_SYS_CFG_BIT_FFA4_MSK                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFA4_POS)
#define DW1000_REG_SYS_CFG_BIT_FFA4                     DW1000_REG_SYS_CFG_BIT_FFA4_MSK

#define DW1000_REG_SYS_CFG_BIT_FFAR_POS                 (6U)
#define DW1000_REG_SYS_CFG_BIT_FFAR_MSK                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAR_POS)
#define DW1000_REG_SYS_CFG_BIT_FFAR                     DW1000_REG_SYS_CFG_BIT_FFAR_MSK

#define DW1000_REG_SYS_CFG_BIT_FFAM_POS                 (5U)
#define DW1000_REG_SYS_CFG_BIT_FFAM_MSK                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAM_POS)
#define DW1000_REG_SYS_CFG_BIT_FFAM                     DW1000_REG_SYS_CFG_BIT_FFAM_MSK

#define DW1000_REG_SYS_CFG_BIT_FFAA_POS                 (4U)
#define DW1000_REG_SYS_CFG_BIT_FFAA_MSK                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAA_POS)
#define DW1000_REG_SYS_CFG_BIT_FFAA                     DW1000_REG_SYS_CFG_BIT_FFAA_MSK

#define DW1000_REG_SYS_CFG_BIT_FFAD_POS                 (3U)
#define DW1000_REG_SYS_CFG_BIT_FFAD_MSK                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAD_POS)
#define DW1000_REG_SYS_CFG_BIT_FFAD                     DW1000_REG_SYS_CFG_BIT_FFAD_MSK

#define DW1000_REG_SYS_CFG_BIT_FFAB_POS                 (2U)
#define DW1000_REG_SYS_CFG_BIT_FFAB_MSK                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFAB_POS)
#define DW1000_REG_SYS_CFG_BIT_FFAB                     DW1000_REG_SYS_CFG_BIT_FFAB_MSK

#define DW1000_REG_SYS_CFG_BIT_FFBC_POS                 (1U)
#define DW1000_REG_SYS_CFG_BIT_FFBC_MSK                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFBC_POS)
#define DW1000_REG_SYS_CFG_BIT_FFBC                     DW1000_REG_SYS_CFG_BIT_FFBC_MSK

#define DW1000_REG_SYS_CFG_BIT_FFEN_POS                 (0U)
#define DW1000_REG_SYS_CFG_BIT_FFEN_MSK                 (0x1UL << DW1000_REG_SYS_CFG_BIT_FFEN_POS)
#define DW1000_REG_SYS_CFG_BIT_FFEN                     DW1000_REG_SYS_CFG_BIT_FFEN_MSK
/*====================================== 子寄存器偏移地址 ======================================*/

#define DW1000_SUB_REG_SYS_CFG_3_OFFSET                 0x03
/*++++++++++++++++++++++++++++++++++++++ 子寄存器位的定义 ++++++++++++++++++++++++++++++++++++++*/

#define DW1000_SUB_REG_SYS_CFG_3_BIT_RXWTOE_POS         (4U)
#define DW1000_SUB_REG_SYS_CFG_3_BIT_RXWTOE_MSK         (0x1UL << DW1000_SUB_REG_SYS_CFG_3_BIT_RXWTOE_POS)
#define DW1000_SUB_REG_SYS_CFG_3_BIT_RXWTOE             DW1000_SUB_REG_SYS_CFG_3_BIT_RXWTOE_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x05 - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x06 - SYS_TIME 系统时间计数器 [只读]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_SYS_TIME                             0x06
#define DW1000_REG_SYS_TIME_LEN                         (5) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x07 - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x08 - TX_FCTRL 发送帧控制 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_TX_FCTRL                             0x08
#define DW1000_REG_TX_FCTRL_LEN                         (5) /* 字节 */
/*-------------------------------------- TX_FCTRL 位定义 --------------------------------------*/
#define DW1000_REG_TX_FCTRL_BITS_TXBOFFS_POS            (22U)
#define DW1000_REG_TX_FCTRL_BITS_TXBOFFS_LEN            (10U) /* 位 */
#define DW1000_REG_TX_FCTRL_BITS_TXBOFFS_MSK            (0x3FFUL << DW1000_REG_TX_FCTRL_BITS_TXBOFFS_POS)
#define DW1000_REG_TX_FCTRL_BITS_TXBOFFS                DW1000_REG_TX_FCTRL_BITS_TXBOFFS_MSK

#define DW1000_REG_TX_FCTRL_BITS_PE_POS                 (20U)
#define DW1000_REG_TX_FCTRL_BITS_PE_LEN                 (2U) /* 位 */
#define DW1000_REG_TX_FCTRL_BITS_PE_MSK                 (0x3UL << DW1000_REG_TX_FCTRL_BITS_PE_POS)
#define DW1000_REG_TX_FCTRL_BITS_PE                     DW1000_REG_TX_FCTRL_BITS_PE_MSK

#define DW1000_REG_TX_FCTRL_BITS_TXPSR_POS              (18U)
#define DW1000_REG_TX_FCTRL_BITS_TXPSR_LEN              (2U) /* 位 */
#define DW1000_REG_TX_FCTRL_BITS_TXPSR_MSK              (0x3UL << DW1000_REG_TX_FCTRL_BITS_TXPSR_POS)
#define DW1000_REG_TX_FCTRL_BITS_TXPSR                  DW1000_REG_TX_FCTRL_BITS_TXPSR_MSK

#define DW1000_REG_TX_FCTRL_BITS_TXPRF_POS              (16U)
#define DW1000_REG_TX_FCTRL_BITS_TXPRF_LEN              (2U) /* 位 */
#define DW1000_REG_TX_FCTRL_BITS_TXPRF_MSK              (0x3UL << DW1000_REG_TX_FCTRL_BITS_TXPRF_POS)
#define DW1000_REG_TX_FCTRL_BITS_TXPRF                  DW1000_REG_TX_FCTRL_BITS_TXPRF_MSK

#define DW1000_REG_TX_FCTRL_BIT_TR_POS                  (15U)
#define DW1000_REG_TX_FCTRL_BIT_TR_MSK                  (0x1UL << DW1000_REG_TX_FCTRL_BIT_TR_POS)
#define DW1000_REG_TX_FCTRL_BIT_TR                      DW1000_REG_TX_FCTRL_BIT_TR_MSK

#define DW1000_REG_TX_FCTRL_BITS_R_POS                  (10U)
#define DW1000_REG_TX_FCTRL_BITS_R_LEN                  (3U) /* 位 */
#define DW1000_REG_TX_FCTRL_BITS_R_MSK                  (0x7UL << DW1000_REG_TX_FCTRL_BITS_R_POS)
#define DW1000_REG_TX_FCTRL_BITS_R                      DW1000_REG_TX_FCTRL_BITS_R_MSK

#define DW1000_REG_TX_FCTRL_BITS_TFLEN_POS              (7U)
#define DW1000_REG_TX_FCTRL_BITS_TFLEN_LEN              (3U) /* 位 */
#define DW1000_REG_TX_FCTRL_BITS_TFLEN_MSK              (0x7UL << DW1000_REG_TX_FCTRL_BITS_TFLEN_POS)
#define DW1000_REG_TX_FCTRL_BITS_TFLEN                  DW1000_REG_TX_FCTRL_BITS_TFLEN_MSK

#define DW1000_REG_TX_FCTRL_BITS_TFLEN_POS              (0U)
#define DW1000_REG_TX_FCTRL_BITS_TFLEN_LEN              (7U) /* 位 */
#define DW1000_REG_TX_FCTRL_BITS_TFLEN_MSK              (0x7FUL << DW1000_REG_TX_FCTRL_BITS_TFLEN_POS)
#define DW1000_REG_TX_FCTRL_BITS_TFLEN                  DW1000_REG_TX_FCTRL_BITS_TFLEN_MSK

/*======================================= 子寄存偏移地址 =======================================*/
/**
 * @brief DW1000_SUB_REG_TX_FCTRL_4 偏移地址
 * @note  在DW1000的用户手册中并没有这个子寄存器的定义，但是由于该寄存器的长度超过32bit
 *        为了方便访问，所以将其划分出一个子寄存器地址进行访问
 * @todo 验证这种子寄存器访问是否合理
 */
#define DW1000_SUB_REG_TX_FCTRL_4_OFFSET                0x04
/*++++++++++++++++++++++++++++++++++++++ 子寄存器位的定义 ++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_TX_FCTRL_4_BITS_TXBOFFS_POS      (0U)
#define DW1000_SUB_REG_TX_FCTRL_4_BITS_TXBOFFS_LEN      (8U) /* 位 */
#define DW1000_SUB_REG_TX_FCTRL_4_BITS_TXBOFFS_MSK      (0xFFUL << DW1000_SUB_REG_TX_FCTRL_4_BITS_TXBOFFS_POS)
#define DW1000_SUB_REG_TX_FCTRL_4_BITS_IFSDELAY         DW1000_SUB_REG_TX_FCTRL_4_BITS_TXBOFFS_MSK
// #define DW1000_REG_TX_FCTRL_BITS_IFSDELAY   DW1000_REG_BIT_RANGE(39,32)

/***********************************************************************************************
 * @brief 寄存器文件 0x09 - TX_BUFFER 发送数据缓冲区 [只写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_TX_BUFFER                            0x09
#define DW1000_REG_TX_BUFFER_LEN                        (1024) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x0A - DX_TIME 延迟发送或接收时间 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_DX_TIME                              0x0A
#define DW1000_REG_DX_TIME_LEN                          (5) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x0B - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x0C - RX_FWTO 接收帧等待超时时间 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_RX_FWTO                              0x0C
#define DW1000_REG_RX_FWTO_LEN                          (2) /* 字节 */
/*--------------------------- RX_FWTO 位定义（本质上是一个16位寄存器） ---------------------------*/
#define DW1000_REG_RX_FWTO_BITS_RXFWTO_POS              (0U)
#define DW1000_REG_RX_FWTO_BITS_RXFWTO_LEN              (16U) /* 位 */
#define DW1000_REG_RX_FWTO_BITS_RXFWTO_MSK              (0xFFFFUL << DW1000_REG_RX_FWTO_BITS_RXFWTO_POS)
#define DW1000_REG_RX_FWTO_BITS_RXFWTO                  DW1000_REG_RX_FWTO_BITS_RXFWTO_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x0D - SYS_CTRL 系统控制寄存器 [特殊读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_SYS_CTRL                             0x0D
#define DW1000_REG_SYS_CTRL_LEN                         (4) /* 字节 */
/*-------------------------------------- SYS_CTRL 位定义 --------------------------------------*/

#define DW1000_REG_SYS_CTRL_BIT_HRBPT_POS               (24U)
#define DW1000_REG_SYS_CTRL_BIT_HRBPT_MSK               (0x1UL << DW1000_REG_SYS_CTRL_BIT_HRBPT_POS)
#define DW1000_REG_SYS_CTRL_BIT_HRBPT                   DW1000_REG_SYS_CTRL_BIT_HRBPT_MSK

#define DW1000_REG_SYS_CTRL_BIT_RXDLYE_POS              (9U)
#define DW1000_REG_SYS_CTRL_BIT_RXDLYE_MSK              (0x1UL << DW1000_REG_SYS_CTRL_BIT_RXDLYE_POS)
#define DW1000_REG_SYS_CTRL_BIT_RXDLYE                  DW1000_REG_SYS_CTRL_BIT_RXDLYE_MSK

#define DW1000_REG_SYS_CTRL_BIT_RXENAB_POS              (8U)
#define DW1000_REG_SYS_CTRL_BIT_RXENAB_MSK              (0x1UL << DW1000_REG_SYS_CTRL_BIT_RXENAB_POS)
#define DW1000_REG_SYS_CTRL_BIT_RXENAB                  DW1000_REG_SYS_CTRL_BIT_RXENAB_MSK

#define DW1000_REG_SYS_CTRL_BIT_WAIT4RESP_POS           (7U)
#define DW1000_REG_SYS_CTRL_BIT_WAIT4RESP_MSK           (0x1UL << DW1000_REG_SYS_CTRL_BIT_WAIT4RESP_POS)
#define DW1000_REG_SYS_CTRL_BIT_WAIT4RESP               DW1000_REG_SYS_CTRL_BIT_WAIT4RESP_MSK

#define DW1000_REG_SYS_CTRL_BIT_TRXOFF_POS              (6U)
#define DW1000_REG_SYS_CTRL_BIT_TRXOFF_MSK              (0x1UL << DW1000_REG_SYS_CTRL_BIT_TRXOFF_POS)
#define DW1000_REG_SYS_CTRL_BIT_TRXOFF                  DW1000_REG_SYS_CTRL_BIT_TRXOFF_MSK

#define DW1000_REG_SYS_CTRL_BIT_CANSFCS_POS             (3U)
#define DW1000_REG_SYS_CTRL_BIT_CANSFCS_MSK             (0x1UL << DW1000_REG_SYS_CTRL_BIT_CANSFCS_POS)
#define DW1000_REG_SYS_CTRL_BIT_CANSFCS                 DW1000_REG_SYS_CTRL_BIT_CANSFCS_MSK

#define DW1000_REG_SYS_CTRL_BIT_TXDLYS_POS              (2U)
#define DW1000_REG_SYS_CTRL_BIT_TXDLYS_MSK              (0x1UL << DW1000_REG_SYS_CTRL_BIT_TXDLYS_POS)
#define DW1000_REG_SYS_CTRL_BIT_TXDLYS                  DW1000_REG_SYS_CTRL_BIT_TXDLYS_MSK

#define DW1000_REG_SYS_CTRL_BIT_TXSTRT_POS              (1U)
#define DW1000_REG_SYS_CTRL_BIT_TXSTRT_MSK              (0x1UL << DW1000_REG_SYS_CTRL_BIT_TXSTRT_POS)
#define DW1000_REG_SYS_CTRL_BIT_TXSTRT                  DW1000_REG_SYS_CTRL_BIT_TXSTRT_MSK

#define DW1000_REG_SYS_CTRL_BIT_SFCST_POS               (0U)
#define DW1000_REG_SYS_CTRL_BIT_SFCST_MSK               (0x1UL << DW1000_REG_SYS_CTRL_BIT_SFCST_POS)
#define DW1000_REG_SYS_CTRL_BIT_SFCST                   DW1000_REG_SYS_CTRL_BIT_SFCST_MSK
/*======================================= 子寄存偏移地址 =======================================*/
/**
 * @brief DW1000_SUB_REG_SYS_CTRL_3 偏移地址
 * @note  在DW1000的用户手册中并没有这个子寄存器的定义
 *        为了方便访问，所以将其划分出一个子寄存器地址进行访问
 * @todo 验证这种子寄存器访问是否合理
 */
#define DW1000_SUB_REG_SYS_CTRL_3_OFFSET                0x03
/*++++++++++++++++++++++++++++++++++++++ 子寄存器位的定义 ++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_SYS_CTRL_3_BIT_HRBPT_POS         (0U)
#define DW1000_SUB_REG_SYS_CTRL_3_BIT_HRBPT_MSK         (0x1UL << DW1000_SUB_REG_SYS_CTRL_3_BIT_HRBPT_POS)
#define DW1000_SUB_REG_SYS_CTRL_3_BIT_HRBPT             DW1000_SUB_REG_SYS_CTRL_3_BIT_HRBPT_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x0E - 系统事件掩码寄存器 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_SYS_MASK                             0x0E
#define DW1000_REG_SYS_MASK_LEN                         (4) /* 字节 */
/*-------------------------------------- SYS_MASK 位定义 --------------------------------------*/

#define DW1000_REG_SYS_MASK_BIT_MAFFREJ_POS             (29U)
#define DW1000_REG_SYS_MASK_BIT_MAFFREJ_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MAFFREJ_POS)
#define DW1000_REG_SYS_MASK_BIT_MAFFREJ                 DW1000_REG_SYS_MASK_BIT_MAFFREJ_MSK

#define DW1000_REG_SYS_MASK_BIT_MTXBERR_POS             (28U)
#define DW1000_REG_SYS_MASK_BIT_MTXBERR_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MTXBERR_POS)
#define DW1000_REG_SYS_MASK_BIT_MTXBERR                 DW1000_REG_SYS_MASK_BIT_MTXBERR_MSK

#define DW1000_REG_SYS_MASK_BIT_MHPDWARN_POS            (27U)
#define DW1000_REG_SYS_MASK_BIT_MHPDWARN_MSK            (0x1UL << DW1000_REG_SYS_MASK_BIT_MHPDWARN_POS)
#define DW1000_REG_SYS_MASK_BIT_MHPDWARN                DW1000_REG_SYS_MASK_BIT_MHPDWARN_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXSFDTO_POS            (26U)
#define DW1000_REG_SYS_MASK_BIT_MRXSFDTO_MSK            (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXSFDTO_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXSFDTO                DW1000_REG_SYS_MASK_BIT_MRXSFDTO_MSK

#define DW1000_REG_SYS_MASK_BIT_MCPLLLL_POS             (25U)
#define DW1000_REG_SYS_MASK_BIT_MCPLLLL_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MCPLLLL_POS)
#define DW1000_REG_SYS_MASK_BIT_MCPLLLL                 DW1000_REG_SYS_MASK_BIT_MCPLLLL_MSK

#define DW1000_REG_SYS_MASK_BIT_MRFPLLLL_POS            (24U)
#define DW1000_REG_SYS_MASK_BIT_MRFPLLLL_MSK            (0x1UL << DW1000_REG_SYS_MASK_BIT_MRFPLLLL_POS)
#define DW1000_REG_SYS_MASK_BIT_MRFPLLLL                DW1000_REG_SYS_MASK_BIT_MRFPLLLL_MSK

#define DW1000_REG_SYS_MASK_BIT_MSLP2INIT_POS           (23U)
#define DW1000_REG_SYS_MASK_BIT_MSLP2INIT_MSK           (0x1UL << DW1000_REG_SYS_MASK_BIT_MSLP2INIT_POS)
#define DW1000_REG_SYS_MASK_BIT_MSLP2INIT               DW1000_REG_SYS_MASK_BIT_MSLP2INIT_MSK

#define DW1000_REG_SYS_MASK_BIT_MGPIOIRQ_POS            (22U)
#define DW1000_REG_SYS_MASK_BIT_MGPIOIRQ_MSK            (0x1UL << DW1000_REG_SYS_MASK_BIT_MGPIOIRQ_POS)
#define DW1000_REG_SYS_MASK_BIT_MGPIOIRQ                DW1000_REG_SYS_MASK_BIT_MGPIOIRQ_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXPTO_POS              (21U)
#define DW1000_REG_SYS_MASK_BIT_MRXPTO_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXPTO_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXPTO                  DW1000_REG_SYS_MASK_BIT_MRXPTO_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXOVRR_POS             (20U)
#define DW1000_REG_SYS_MASK_BIT_MRXOVRR_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXOVRR_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXOVRR                 DW1000_REG_SYS_MASK_BIT_MRXOVRR_MSK

#define DW1000_REG_SYS_MASK_BIT_MLDEERR_POS             (18U)
#define DW1000_REG_SYS_MASK_BIT_MLDEERR_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MLDEERR_POS)
#define DW1000_REG_SYS_MASK_BIT_MLDEERR                 DW1000_REG_SYS_MASK_BIT_MLDEERR_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXRFTO_POS             (17U)
#define DW1000_REG_SYS_MASK_BIT_MRXRFTO_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXRFTO_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXRFTO                 DW1000_REG_SYS_MASK_BIT_MRXRFTO_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXRFSL_POS             (16U)
#define DW1000_REG_SYS_MASK_BIT_MRXRFSL_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXRFSL_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXRFSL                 DW1000_REG_SYS_MASK_BIT_MRXRFSL_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXFCE_POS              (15U)
#define DW1000_REG_SYS_MASK_BIT_MRXFCE_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXFCE_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXFCE                  DW1000_REG_SYS_MASK_BIT_MRXFCE_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXFCG_POS              (14U)
#define DW1000_REG_SYS_MASK_BIT_MRXFCG_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXFCG_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXFCG                  DW1000_REG_SYS_MASK_BIT_MRXFCG_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXDFR_POS              (13U)
#define DW1000_REG_SYS_MASK_BIT_MRXDFR_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXDFR_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXDFR                  DW1000_REG_SYS_MASK_BIT_MRXDFR_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXPHE_POS              (12U)
#define DW1000_REG_SYS_MASK_BIT_MRXPHE_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXPHE_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXPHE                  DW1000_REG_SYS_MASK_BIT_MRXPHE_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXPHD_POS              (11U)
#define DW1000_REG_SYS_MASK_BIT_MRXPHD_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXPHD_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXPHD                  DW1000_REG_SYS_MASK_BIT_MRXPHD_MSK

#define DW1000_REG_SYS_MASK_BIT_MLDEDONE_POS            (10U)
#define DW1000_REG_SYS_MASK_BIT_MLDEDONE_MSK            (0x1UL << DW1000_REG_SYS_MASK_BIT_MLDEDONE_POS)
#define DW1000_REG_SYS_MASK_BIT_MLDEDONE                DW1000_REG_SYS_MASK_BIT_MLDEDONE_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXSFDD_POS             (9U)
#define DW1000_REG_SYS_MASK_BIT_MRXSFDD_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXSFDD_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXSFDD                 DW1000_REG_SYS_MASK_BIT_MRXSFDD_MSK

#define DW1000_REG_SYS_MASK_BIT_MRXPRD_POS              (8U)
#define DW1000_REG_SYS_MASK_BIT_MRXPRD_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MRXPRD_POS)
#define DW1000_REG_SYS_MASK_BIT_MRXPRD                  DW1000_REG_SYS_MASK_BIT_MRXPRD_MSK

#define DW1000_REG_SYS_MASK_BIT_MTXFRS_POS              (7U)
#define DW1000_REG_SYS_MASK_BIT_MTXFRS_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MTXFRS_POS)
#define DW1000_REG_SYS_MASK_BIT_MTXFRS                  DW1000_REG_SYS_MASK_BIT_MTXFRS_MSK

#define DW1000_REG_SYS_MASK_BIT_MTXPHS_POS              (6U)
#define DW1000_REG_SYS_MASK_BIT_MTXPHS_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MTXPHS_POS)
#define DW1000_REG_SYS_MASK_BIT_MTXPHS                  DW1000_REG_SYS_MASK_BIT_MTXPHS_MSK

#define DW1000_REG_SYS_MASK_BIT_MTXPRS_POS              (5U)
#define DW1000_REG_SYS_MASK_BIT_MTXPRS_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MTXPRS_POS)
#define DW1000_REG_SYS_MASK_BIT_MTXPRS                  DW1000_REG_SYS_MASK_BIT_MTXPRS_MSK

#define DW1000_REG_SYS_MASK_BIT_MTXFRB_POS              (4U)
#define DW1000_REG_SYS_MASK_BIT_MTXFRB_MSK              (0x1UL << DW1000_REG_SYS_MASK_BIT_MTXFRB_POS)
#define DW1000_REG_SYS_MASK_BIT_MTXFRB                  DW1000_REG_SYS_MASK_BIT_MTXFRB_MSK

#define DW1000_REG_SYS_MASK_BIT_MAAT_POS                (3U)
#define DW1000_REG_SYS_MASK_BIT_MAAT_MSK                (0x1UL << DW1000_REG_SYS_MASK_BIT_MAAT_POS)
#define DW1000_REG_SYS_MASK_BIT_MAAT                    DW1000_REG_SYS_MASK_BIT_MAAT_MSK

#define DW1000_REG_SYS_MASK_BIT_MESYNCR_POS             (2U)
#define DW1000_REG_SYS_MASK_BIT_MESYNCR_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MESYNCR_POS)
#define DW1000_REG_SYS_MASK_BIT_MESYNCR                 DW1000_REG_SYS_MASK_BIT_MESYNCR_MSK

#define DW1000_REG_SYS_MASK_BIT_MCPLOCK_POS             (1U)
#define DW1000_REG_SYS_MASK_BIT_MCPLOCK_MSK             (0x1UL << DW1000_REG_SYS_MASK_BIT_MCPLOCK_POS)
#define DW1000_REG_SYS_MASK_BIT_MCPLOCK                 DW1000_REG_SYS_MASK_BIT_MCPLOCK_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x0F - SYS_STATUS 系统事件状态寄存器 [特殊读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_SYS_STATUS                           0x0F
#define DW1000_REG_SYS_STATUS_LEN                       (5) /* 字节 */
/*------------------------------------- SYS_STATUS 位定义 -------------------------------------*/

#define DW1000_REG_SYS_STATUS_BIT_ICRBP_POS             (31U)
#define DW1000_REG_SYS_STATUS_BIT_ICRBP_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_ICRBP_POS)
#define DW1000_REG_SYS_STATUS_BIT_ICRBP                 DW1000_REG_SYS_STATUS_BIT_ICRBP_MSK

#define DW1000_REG_SYS_STATUS_BIT_HSRB_POS              (30U)
#define DW1000_REG_SYS_STATUS_BIT_HSRB_MSK              (0x1UL << DW1000_REG_SYS_STATUS_BIT_HSRB_POS)
#define DW1000_REG_SYS_STATUS_BIT_HSRBP                 DW1000_REG_SYS_STATUS_BIT_HSRB_MSK

#define DW1000_REG_SYS_STATUS_BIT_AFFREJ_POS            (29U)
#define DW1000_REG_SYS_STATUS_BIT_AFFREJ_MSK            (0x1UL << DW1000_REG_SYS_STATUS_BIT_AFFREJ_POS)
#define DW1000_REG_SYS_STATUS_BIT_AFFREJ                DW1000_REG_SYS_STATUS_BIT_AFFREJ_MSK

#define DW1000_REG_SYS_STATUS_BIT_TXBERR_POS            (28U)
#define DW1000_REG_SYS_STATUS_BIT_TXBERR_MSK            (0x1UL << DW1000_REG_SYS_STATUS_BIT_TXBERR_POS)
#define DW1000_REG_SYS_STATUS_BIT_TXBERR                DW1000_REG_SYS_STATUS_BIT_TXBERR_MSK

#define DW1000_REG_SYS_STATUS_BIT_HPDWARN_POS           (27U)
#define DW1000_REG_SYS_STATUS_BIT_HPDWARN_MSK           (0x1UL << DW1000_REG_SYS_STATUS_BIT_HPDWARN_POS)
#define DW1000_REG_SYS_STATUS_BIT_HPDWARN               DW1000_REG_SYS_STATUS_BIT_HPDWARN_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXSFDTO_POS           (26U)
#define DW1000_REG_SYS_STATUS_BIT_RXSFDTO_MSK           (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXSFDTO_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXSFDTO               DW1000_REG_SYS_STATUS_BIT_RXSFDTO_MSK

#define DW1000_REG_SYS_STATUS_BIT_CLKPLL_LL_POS         (25U)
#define DW1000_REG_SYS_STATUS_BIT_CLKPLL_LL_MSK         (0x1UL << DW1000_REG_SYS_STATUS_BIT_CLKPLL_LL_POS)
#define DW1000_REG_SYS_STATUS_BIT_CLKPLL_LL             DW1000_REG_SYS_STATUS_BIT_CLKPLL_LL_MSK

#define DW1000_REG_SYS_STATUS_BIT_RFPLL_LL_POS          (24U)
#define DW1000_REG_SYS_STATUS_BIT_RFPLL_LL_MSK          (0x1UL << DW1000_REG_SYS_STATUS_BIT_RFPLL_LL_POS)
#define DW1000_REG_SYS_STATUS_BIT_RFPLL_LL              DW1000_REG_SYS_STATUS_BIT_RFPLL_LL_MSK

#define DW1000_REG_SYS_STATUS_BIT_SLP2INIT_POS          (23U)
#define DW1000_REG_SYS_STATUS_BIT_SLP2INIT_MSK          (0x1UL << DW1000_REG_SYS_STATUS_BIT_SLP2INIT_POS)
#define DW1000_REG_SYS_STATUS_BIT_SLP2INIT              DW1000_REG_SYS_STATUS_BIT_SLP2INIT_MSK

#define DW1000_REG_SYS_STATUS_BIT_GPIOIRQ_POS           (22U)
#define DW1000_REG_SYS_STATUS_BIT_GPIOIRQ_MSK           (0x1UL << DW1000_REG_SYS_STATUS_BIT_GPIOIRQ_POS)
#define DW1000_REG_SYS_STATUS_BIT_GPIOIRQ               DW1000_REG_SYS_STATUS_BIT_GPIOIRQ_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXPTO_POS             (21U)
#define DW1000_REG_SYS_STATUS_BIT_RXPTO_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXPTO_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXPTO                 DW1000_REG_SYS_STATUS_BIT_RXPTO_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXOVRR_POS            (20U)
#define DW1000_REG_SYS_STATUS_BIT_RXOVRR_MSK            (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXOVRR_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXOVRR                DW1000_REG_SYS_STATUS_BIT_RXOVRR_MSK

#define DW1000_REG_SYS_STATUS_BIT_LDEERR_POS            (18U)
#define DW1000_REG_SYS_STATUS_BIT_LDEERR_MSK            (0x1UL << DW1000_REG_SYS_STATUS_BIT_LDEERR_POS)
#define DW1000_REG_SYS_STATUS_BIT_LDEERR                DW1000_REG_SYS_STATUS_BIT_LDEERR_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXRFTO_POS            (17U)
#define DW1000_REG_SYS_STATUS_BIT_RXRFTO_MSK            (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXRFTO_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXRFTO                DW1000_REG_SYS_STATUS_BIT_RXRFTO_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXRFSL_POS            (16U)
#define DW1000_REG_SYS_STATUS_BIT_RXRFSL_MSK            (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXRFSL_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXRFSL                DW1000_REG_SYS_STATUS_BIT_RXRFSL_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXFCE_POS             (15U)
#define DW1000_REG_SYS_STATUS_BIT_RXFCE_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXFCE_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXFCE                 DW1000_REG_SYS_STATUS_BIT_RXFCE_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXFCG_POS             (14U)
#define DW1000_REG_SYS_STATUS_BIT_RXFCG_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXFCG_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXFCG                 DW1000_REG_SYS_STATUS_BIT_RXFCG_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXDFR_POS             (13U)
#define DW1000_REG_SYS_STATUS_BIT_RXDFR_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXDFR_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXDFR                 DW1000_REG_SYS_STATUS_BIT_RXDFR_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXPHE_POS             (12U)
#define DW1000_REG_SYS_STATUS_BIT_RXPHE_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXPHE_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXPHE                 DW1000_REG_SYS_STATUS_BIT_RXPHE_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXPHD_POS             (11U)
#define DW1000_REG_SYS_STATUS_BIT_RXPHD_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXPHD_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXPHD                 DW1000_REG_SYS_STATUS_BIT_RXPHD_MSK

#define DW1000_REG_SYS_STATUS_BIT_LDEDONE_POS           (10U)
#define DW1000_REG_SYS_STATUS_BIT_LDEDONE_MSK           (0x1UL << DW1000_REG_SYS_STATUS_BIT_LDEDONE_POS)
#define DW1000_REG_SYS_STATUS_BIT_LDEDONE               DW1000_REG_SYS_STATUS_BIT_LDEDONE_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXSFDD_POS            (9U)
#define DW1000_REG_SYS_STATUS_BIT_RXSFDD_MSK            (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXSFDD_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXSFDD                DW1000_REG_SYS_STATUS_BIT_RXSFDD_MSK

#define DW1000_REG_SYS_STATUS_BIT_RXPRD_POS             (8U)
#define DW1000_REG_SYS_STATUS_BIT_RXPRD_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_RXPRD_POS)
#define DW1000_REG_SYS_STATUS_BIT_RXPRD                 DW1000_REG_SYS_STATUS_BIT_RXPRD_MSK

#define DW1000_REG_SYS_STATUS_BIT_TXFRS_POS             (7U)
#define DW1000_REG_SYS_STATUS_BIT_TXFRS_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_TXFRS_POS)
#define DW1000_REG_SYS_STATUS_BIT_TXFRS                 DW1000_REG_SYS_STATUS_BIT_TXFRS_MSK

#define DW1000_REG_SYS_STATUS_BIT_TXPHS_POS             (6U)
#define DW1000_REG_SYS_STATUS_BIT_TXPHS_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_TXPHS_POS)
#define DW1000_REG_SYS_STATUS_BIT_TXPHS                 DW1000_REG_SYS_STATUS_BIT_TXPHS_MSK

#define DW1000_REG_SYS_STATUS_BIT_TXPRS_POS             (5U)
#define DW1000_REG_SYS_STATUS_BIT_TXPRS_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_TXPRS_POS)
#define DW1000_REG_SYS_STATUS_BIT_TXPRS                 DW1000_REG_SYS_STATUS_BIT_TXPRS_MSK

#define DW1000_REG_SYS_STATUS_BIT_TXFRB_POS             (4U)
#define DW1000_REG_SYS_STATUS_BIT_TXFRB_MSK             (0x1UL << DW1000_REG_SYS_STATUS_BIT_TXFRB_POS)
#define DW1000_REG_SYS_STATUS_BIT_TXFRB                 DW1000_REG_SYS_STATUS_BIT_TXFRB_MSK

#define DW1000_REG_SYS_STATUS_BIT_AAT_POS               (3U)
#define DW1000_REG_SYS_STATUS_BIT_AAT_MSK               (0x1UL << DW1000_REG_SYS_STATUS_BIT_AAT_POS)
#define DW1000_REG_SYS_STATUS_BIT_AAT                   DW1000_REG_SYS_STATUS_BIT_AAT_MSK

#define DW1000_REG_SYS_STATUS_BIT_ESYNCR_POS            (2U)
#define DW1000_REG_SYS_STATUS_BIT_ESYNCR_MSK            (0x1UL << DW1000_REG_SYS_STATUS_BIT_ESYNCR_POS)
#define DW1000_REG_SYS_STATUS_BIT_ESYNCR                DW1000_REG_SYS_STATUS_BIT_ESYNCR_MSK

#define DW1000_REG_SYS_STATUS_BIT_CPLOCK_POS            (1U)
#define DW1000_REG_SYS_STATUS_BIT_CPLOCK_MSK            (0x1UL << DW1000_REG_SYS_STATUS_BIT_CPLOCK_POS)
#define DW1000_REG_SYS_STATUS_BIT_CPLOCK                DW1000_REG_SYS_STATUS_BIT_CPLOCK_MSK

#define DW1000_REG_SYS_STATUS_BIT_IRQS_POS              (0U)
#define DW1000_REG_SYS_STATUS_BIT_IRQS_MSK              (0x1UL << DW1000_REG_SYS_STATUS_BIT_IRQS_POS)
#define DW1000_REG_SYS_STATUS_BIT_IRQS                  DW1000_REG_SYS_STATUS_BIT_IRQS_MSK
/*====================================== 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_SYS_STATUS_3_OFFSET              0x03
#define DW1000_SUB_REG_SYS_STATUS_4_OFFSET              0x04
/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/

#define DW1000_SUB_REG_SYS_STATUS_3_BIT_ICRBP_POS       (7U)
#define DW1000_SUB_REG_SYS_STATUS_3_BIT_ICRBP_MSK       (0x1UL << DW1000_SUB_REG_SYS_STATUS_3_BIT_ICRBP_POS)
#define DW1000_SUB_REG_SYS_STATUS_3_BIT_ICRBP           DW1000_SUB_REG_SYS_STATUS_3_BIT_ICRBP_MSK

#define DW1000_SUB_REG_SYS_STATUS_3_BIT_HSRBP_POS       (6U)
#define DW1000_SUB_REG_SYS_STATUS_3_BIT_HSRBP_MSK       (0x1UL << DW1000_SUB_REG_SYS_STATUS_3_BIT_HSRBP_POS)
#define DW1000_SUB_REG_SYS_STATUS_3_BIT_HSRBP           DW1000_SUB_REG_SYS_STATUS_3_BIT_HSRBP_MSK

#define DW1000_SUB_REG_SYS_STATUS_3_BIT_HPDWARN_POS     (3U)
#define DW1000_SUB_REG_SYS_STATUS_3_BIT_HPDWARN_MSK     (0x1UL << DW1000_SUB_REG_SYS_STATUS_3_BIT_HPDWARN_POS)
#define DW1000_SUB_REG_SYS_STATUS_3_BIT_HPDWARN         DW1000_SUB_REG_SYS_STATUS_3_BIT_HPDWARN_MSK

#define DW1000_SUB_REG_SYS_STATUS_4_BIT_TXPUTE_POS      (2U)
#define DW1000_SUB_REG_SYS_STATUS_4_BIT_TXPUTE_MSK      (0x1UL << DW1000_SUB_REG_SYS_STATUS_4_BIT_TXPUTE_POS)
#define DW1000_SUB_REG_SYS_STATUS_4_BIT_TXPUTE          DW1000_SUB_REG_SYS_STATUS_4_BIT_TXPUTE_MSK
// #define DW1000_REG_SYS_STATUS_BIT_TXPUTE    DW1000_REG_BIT(34)

#define DW1000_SUB_REG_SYS_STATUS_4_BIT_RXPREJ_POS      (1U)
#define DW1000_SUB_REG_SYS_STATUS_4_BIT_RXPREJ_MSK      (0x1UL << DW1000_SUB_REG_SYS_STATUS_4_BIT_RXPREJ_POS)
#define DW1000_SUB_REG_SYS_STATUS_4_BIT_RXPREJ          DW1000_SUB_REG_SYS_STATUS_4_BIT_RXPREJ_MSK
// #define DW1000_REG_SYS_STATUS_BIT_RXPREJ    DW1000_REG_BIT(33)

#define DW1000_SUB_REG_SYS_STATUS_4_BIT_RXRSCS_POS      (0U)
#define DW1000_SUB_REG_SYS_STATUS_4_BIT_RXRSCS_MSK      (0x1UL << DW1000_SUB_REG_SYS_STATUS_4_BIT_RXRSCS_POS)
#define DW1000_SUB_REG_SYS_STATUS_4_BIT_RXRSCS          DW1000_SUB_REG_SYS_STATUS_4_BIT_RXRSCS_MSK
// #define DW1000_REG_SYS_STATUS_BIT_RXRSCS    DW1000_REG_BIT(32)

/***********************************************************************************************
 * @brief 寄存器文件 0x10 - RX_FINFO 接收帧信息寄存器 [只读双缓冲]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_RX_FINFO                             0x10
#define DW1000_REG_RX_FINFO_LEN                         (4) /* 字节 */
/*-------------------------------------- RX_FINFO 位定义 --------------------------------------*/
#define DW1000_REG_RX_FINFO_BITS_RXPACC_POS             (20U)
#define DW1000_REG_RX_FINFO_BITS_RXPACC_LEN             (12U) /* 位 */
#define DW1000_REG_RX_FINFO_BITS_RXPACC_MSK             (0xFFFUL << DW1000_REG_RX_FINFO_BITS_RXPACC_POS)
#define DW1000_REG_RX_FINFO_BITS_RXPACC                 DW1000_REG_RX_FINFO_BITS_RXPACC_MSK

#define DW1000_REG_RX_FINFO_BITS_RXPSR_POS              (18U)
#define DW1000_REG_RX_FINFO_BITS_RXPSR_LEN              (2U) /* 位 */
#define DW1000_REG_RX_FINFO_BITS_RXPSR_MSK              (0x3UL << DW1000_REG_RX_FINFO_BITS_RXPSR_POS)
#define DW1000_REG_RX_FINFO_BITS_RXPSR                  DW1000_REG_RX_FINFO_BITS_RXPSR_MSK

#define DW1000_REG_RX_FINFO_BITS_RXPRFR_POS             (16U)
#define DW1000_REG_RX_FINFO_BITS_RXPRFR_LEN             (2U) /* 位 */
#define DW1000_REG_RX_FINFO_BITS_RXPRFR_MSK             (0x3UL << DW1000_REG_RX_FINFO_BITS_RXPRFR_POS)
#define DW1000_REG_RX_FINFO_BITS_RXPRFR                 DW1000_REG_RX_FINFO_BITS_RXPRFR_MSK

#define DW1000_REG_RX_FINFO_BIT_RNG_POS                 (15U)
#define DW1000_REG_RX_FINFO_BIT_RNG_MSK                 (0x1UL << DW1000_REG_RX_FINFO_BIT_RNG_POS)
#define DW1000_REG_RX_FINFO_BIT_RNG                     DW1000_REG_RX_FINFO_BIT_RNG_MSK

#define DW1000_REG_RX_FINFO_BITS_RXBR_POS               (13U)
#define DW1000_REG_RX_FINFO_BITS_RXBR_LEN               (2U) /* 位 */
#define DW1000_REG_RX_FINFO_BITS_RXBR_MSK               (0x3UL << DW1000_REG_RX_FINFO_BITS_RXBR_POS)
#define DW1000_REG_RX_FINFO_BITS_RXBR                   DW1000_REG_RX_FINFO_BITS_RXBR_MSK

#define DW1000_REG_RX_FINFO_BITS_RXNSPL_POS             (11U)
#define DW1000_REG_RX_FINFO_BITS_RXNSPL_LEN             (2U) /* 位 */
#define DW1000_REG_RX_FINFO_BITS_RXNSPL_MSK             (0x3UL << DW1000_REG_RX_FINFO_BITS_RXNSPL_POS)
#define DW1000_REG_RX_FINFO_BITS_RXNSPL                 DW1000_REG_RX_FINFO_BITS_RXNSPL_MSK

#define DW1000_REG_RX_FINFO_BITS_RXFLE_POS              (7U)
#define DW1000_REG_RX_FINFO_BITS_RXFLE_LEN              (3U) /* 位 */
#define DW1000_REG_RX_FINFO_BITS_RXFLE_MSK              (0x7UL << DW1000_REG_RX_FINFO_BITS_RXFLE_POS)
#define DW1000_REG_RX_FINFO_BITS_RXFLE                  DW1000_REG_RX_FINFO_BITS_RXFLE_MSK

#define DW1000_REG_RX_FINFO_BITS_RXFLEN_POS             (0U)
#define DW1000_REG_RX_FINFO_BITS_RXFLEN_LEN             (7U) /* 位 */
#define DW1000_REG_RX_FINFO_BITS_RXFLEN_MSK             (0x7FUL << DW1000_REG_RX_FINFO_BITS_RXFLEN_POS)
#define DW1000_REG_RX_FINFO_BITS_RXFLEN                 DW1000_REG_RX_FINFO_BITS_RXFLEN_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x11 - RX_BUFFER 接收帧数据 [只读双缓冲]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_RX_BUFFER                            0x11
#define DW1000_REG_RX_BUFFER_LEN                        (1024) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x12 - Rx_FQUAL 接收帧质量信息 [只读双缓冲]
 * @note  用户手册中并未有4个子寄存器，但是为了方便访问，这里划分为4个子寄存器
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_RX_FQUAL                             0x12
#define DW1000_REG_RX_FQUAL_LEN                         (8) /* 字节 */
/*-------------------------------------- RX_FQUAL 位定义 --------------------------------------*/
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_STD_NOISE_OFFSET                 0x00
#define DW1000_SUB_REG_STD_NOISE_LEN                    (2) /* 字节 */

#define DW1000_SUB_REG_FP_AMPL2_OFFSET                  0x02
#define DW1000_SUB_REG_FP_AMPL2_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_PP_AMPL3_OFFSET                  0x04
#define DW1000_SUB_REG_PP_AMPL3_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_CIR_PWR_OFFSET                   0x06
#define DW1000_SUB_REG_CIR_PWR_LEN                      (2) /* 字节 */
/*++++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 ++++++++++++++++++++++++++++++++++++++*/


/***********************************************************************************************
 * @brief 寄存器文件 0x13 - RX_TTCKI 接收器时间跟踪间隔 [只读双缓冲]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_RX_TTCKI                             0x13
#define DW1000_REG_RX_TTCKI_LEN                         (4) /* 字节 */
/*-------------------------------------- RX_TTCKI 位定义 --------------------------------------*/
// #define DW1000_REG_RX_TTCKI_BITS_RXTTCKI DW1000_REG_BIT_RANGE(31,0)


/***********************************************************************************************
 * @brief 寄存器文件 0x14 - RX_TTCKO 接收器时间跟踪偏移量 [只读双缓冲]
 * @note  用户手册中并未有4个子寄存器，但是为了方便访问，这里划分为4个子寄存器
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_RX_TTCKO                             0x14
#define DW1000_REG_RX_TTCKO_LEN                         (5) /* 字节 */
/*-------------------------------------- RX_TTCKO 位定义 --------------------------------------*/

#define DW1000_REG_RX_TTCKO_BITS_RSMPDEL_POS            (24U)
#define DW1000_REG_RX_TTCKO_BITS_RSMPDEL_LEN            (8U) /* 位 */
#define DW1000_REG_RX_TTCKO_BITS_RSMPDEL_MSK            (0xFFUL << DW1000_REG_RX_TTCKO_BITS_RSMPDEL_POS)
#define DW1000_REG_RX_TTCKO_BITS_RSMPDEL                DW1000_REG_RX_TTCKO_BITS_RSMPDEL_MSK

#define DW1000_REG_RX_TTCKO_BITS_RXTOFS_POS             (0U)
#define DW1000_REG_RX_TTCKO_BITS_RXTOFS_LEN             (19U) /* 位 */
#define DW1000_REG_RX_TTCKO_BITS_RXTOFS_MSK             (0x7FFFFUL << DW1000_REG_RX_TTCKO_BITS_RXTOFS_POS)
#define DW1000_REG_RX_TTCKO_BITS_RXTOFS                 DW1000_REG_RX_TTCKO_BITS_RXTOFS_MSK
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_RX_TTCKO_4_OFFSET                0x04
/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_RX_TTCKO_4_BITS_RCPHASE_POS      (0U)
#define DW1000_SUB_REG_RX_TTCKO_4_BITS_RCPHASE_LEN      (6U) /* 位 */
#define DW1000_SUB_REG_RX_TTCKO_4_BITS_RCPHASE_MSK      (0x3FUL << DW1000_SUB_REG_RX_TTCKO_4_BITS_RCPHASE_POS)
#define DW1000_SUB_REG_RX_TTCKO_4_BITS_RCPHASE          DW1000_SUB_REG_RX_TTCKO_4_BITS_RCPHASE_MSK
// #define DW1000_REG_RX_TTCKO_BITS_RCPHASE        DW1000_REG_BIT_RANGE(38, 32)

/***********************************************************************************************
 * @brief 寄存器文件 0x15 - RX_TIME 接收时间戳 [只读双缓冲]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_RX_TIME                              0x15
#define DW1000_REG_RX_TIME_LEN                          (14) /* 字节 */
/*-------------------------------------- RX_TIME 位定义 --------------------------------------*/
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_RX_STAMP_OFFSET                  0x00
#define DW1000_SUB_REG_RX_STAMP_LEN                     (5) /* 字节 */

#define DW1000_SUB_REG_FP_INDEX_OFFSET                  0x05
#define DW1000_SUB_REG_FP_INDEX_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_FP_AMPL1_OFFSET                  0x07
#define DW1000_SUB_REG_FP_AMPL1_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_RX_RAWST_OFFSET                  0x09
#define DW1000_SUB_REG_RX_RAWST_LEN                     (5) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x16 - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x17 - TX_TIME 消息发送时间戳 [只读]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_TX_TIME                              0x17
#define DW1000_REG_TX_TIME_LEN                          (10) /* 字节 */
/*-------------------------------------- TX_TIME 位定义 --------------------------------------*/
// #define DW1000_REG_TX_IME_BITS_TX_STAMP     DW1000_REG_BIT_RANGE(39,0)
// #define DW1000_REG_TX_TIME_BITS_TX_RAWST    DW1000_REG_BIT_RANGE(79,40)
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_TX_STAMP_OFFSET                  0x00
#define DW1000_SUB_REG_TX_STAMP_LEN                     (5) /* 字节 */

#define DW1000_SUB_REG_TX_RAWST_OFFSET                  0x05
#define DW1000_SUB_REG_TX_RAWST_LEN                     (5) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x18 - TX_ANTD 发送至天线时延 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_TX_ANTD                              0x18
#define DW1000_REG_TX_ANTD_LEN                          (4) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x19 - SYS_STATE DW1000系统状态信息 [只读]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_SYS_STATE                            0x19
#define DW1000_REG_SYS_STATE_LEN                        (4) /* 字节 */
/*-------------------------------------- SYS_STATE 位定义 --------------------------------------*/

#define DW1000_REG_SYS_STATE_BITS_PMSC_STATE_POS        (16U)
#define DW1000_REG_SYS_STATE_BITS_PMSC_STATE_LEN        (4U) /* 位 */
#define DW1000_REG_SYS_STATE_BITS_PMSC_STATE_MSK        (0xFUL << DW1000_REG_SYS_STATE_BITS_PMSC_STATE_POS)
#define DW1000_REG_SYS_STATE_BITS_PMSC_STATE            DW1000_REG_SYS_STATE_BITS_PMSC_STATE_MSK

#define DW1000_REG_SYS_STATE_BITS_RX_STATE_POS          (8U)
#define DW1000_REG_SYS_STATE_BITS_RX_STATE_LEN          (5U) /* 位 */
#define DW1000_REG_SYS_STATE_BITS_RX_STATE_MSK          (0x1FUL << DW1000_REG_SYS_STATE_BITS_RX_STATE_POS)
#define DW1000_REG_SYS_STATE_BITS_RX_STATE              DW1000_REG_SYS_STATE_BITS_RX_STATE_MSK

#define DW1000_REG_SYS_STATE_BITS_TX_STATE_POS          (0U)
#define DW1000_REG_SYS_STATE_BITS_TX_STATE_LEN          (4U) /* 位 */
#define DW1000_REG_SYS_STATE_BITS_TX_STATE_MSK          (0xFUL << DW1000_REG_SYS_STATE_BITS_TX_STATE_POS)
#define DW1000_REG_SYS_STATE_BITS_TX_STATE              DW1000_REG_SYS_STATE_BITS_TX_STATE_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x1A - ACK_RESP_T 应答时间和响应时间 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_ACK_RESP_T                           0x1A
#define DW1000_REG_ACK_RESP_T_LEN                       (4) /* 字节 */
/*------------------------------------- ACK_RESP_T 位定义 -------------------------------------*/
#define DW1000_REG_ACK_RESP_T_BITS_ACK_TIM_POS          (24U)
#define DW1000_REG_ACK_RESP_T_BITS_ACK_TIM_LEN          (8U) /* 位 */
#define DW1000_REG_ACK_RESP_T_BITS_ACK_TIM_MSK          (0xFFUL << DW1000_REG_ACK_RESP_T_BITS_ACK_TIM_POS)
#define DW1000_REG_ACK_RESP_T_BITS_ACK_TIM              DW1000_REG_ACK_RESP_T_BITS_ACK_TIM_MSK

#define DW1000_REG_ACK_RESP_T_BITS_W4R_TIM_POS          (0U)
#define DW1000_REG_ACK_RESP_T_BITS_W4R_TIM_LEN          (20U) /* 位 */
#define DW1000_REG_ACK_RESP_T_BITS_W4R_TIM_MSK          (0xFFFFFUL << DW1000_REG_ACK_RESP_T_BITS_W4R_TIM_POS)
#define DW1000_REG_ACK_RESP_T_BITS_W4R_TIM              DW1000_REG_ACK_RESP_T_BITS_W4R_TIM_MSK
/*======================================= 子寄存器偏移地址 =====================================*/
/* 这个子寄存器是自己定义的，手册里并没有给其划分 */
#define DW1000_SUB_REG_ACK_TIM_OFFSET                   0x03

/***********************************************************************************************
 * @brief 寄存器文件 0x1B - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x1C - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x1D - RX_SNIFF 嗅探模式配置 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_RX_SNIFF                             0x1D
#define DW1000_REG_RX_SNIFF_LEN                         (4) /* 字节 */
/*-------------------------------------- RX_SNIFF 位定义 --------------------------------------*/

#define DW1000_REG_RX_SNIFF_BITS_SNIFF_OFFT_POS         (8U)
#define DW1000_REG_RX_SNIFF_BITS_SNIFF_OFFT_LEN         (8U) /* 位 */
#define DW1000_REG_RX_SNIFF_BITS_SNIFF_OFFT_MSK         (0xFFUL << DW1000_REG_RX_SNIFF_BITS_SNIFF_OFFT_POS)
// #define DW1000_REG_RX_SNIFF_BITS_SNIFF_OFFT_MSK       DW1000_REG_BIT_RANGE(15, 8)
#define DW1000_REG_RX_SNIFF_BITS_SNIFF_OFFT             DW1000_REG_RX_SNIFF_BITS_SNIFF_OFFT_MSK

#define DW1000_REG_RX_SNIFF_BITS_SNIFF_ONT_POS          (0U)
#define DW1000_REG_RX_SNIFF_BITS_SNIFF_ONT_LEN          (4U) /* 位 */
#define DW1000_REG_RX_SNIFF_BITS_SNIFF_ONT_MSK          (0xFUL << DW1000_REG_RX_SNIFF_BITS_SNIFF_ONT_POS)
// #define DW1000_REG_RX_SNIFF_BITS_SNIFF_ONT_MSK        DW1000_REG_BIT_RANGE(3, 0)
#define DW1000_REG_RX_SNIFF_BITS_SNIFF_ONT              DW1000_REG_RX_SNIFF_BITS_SNIFF_ONT_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x1E - TX_POWER TX功率控制 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_TX_POWER                             0x1E
#define DW1000_REG_TX_POWER_LEN                         (4) /* 字节 */
/*-------------------------------------- TX_POWER 位定义 --------------------------------------*/
/**
 * @brief
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 根据用户手册来补充
 */
#define DW1000_REG_TX_POWER_BITS_BOOSTP125_POS          (24U)
#define DW1000_REG_TX_POWER_BITS_BOOSTP125_LEN          (8U) /* 位 */
#define DW1000_REG_TX_POWER_BITS_BOOSTP125_MSK          (0xFFUL << DW1000_REG_TX_POWER_BITS_BOOSTP125_POS)
// #define DW1000_REG_TX_POWER_BITS_BOOSTP125_MSK        DW1000_REG_BIT_RANGE(31, 24)
#define DW1000_REG_TX_POWER_BITS_BOOSTP125              DW1000_REG_TX_POWER_BITS_BOOSTP125_MSK

#define DW1000_REG_TX_POWER_BITS_BOOSTP250_POS          (16U)
#define DW1000_REG_TX_POWER_BITS_BOOSTP250_LEN          (8U) /* 位 */
#define DW1000_REG_TX_POWER_BITS_BOOSTP250_MSK          (0xFFUL << DW1000_REG_TX_POWER_BITS_BOOSTP250_POS)
// #define DW1000_REG_TX_POWER_BITS_BOOSTP250_MSK        DW1000_REG_BIT_RANGE(23, 16)
#define DW1000_REG_TX_POWER_BITS_BOOSTP250              DW1000_REG_TX_POWER_BITS_BOOSTP250_MSK

#define DW1000_REG_TX_POWER_BITS_BOOSTP500_POS          (8U)
#define DW1000_REG_TX_POWER_BITS_BOOSTP500_LEN          (8U) /* 位 */
#define DW1000_REG_TX_POWER_BITS_BOOSTP500_MSK          (0xFFUL << DW1000_REG_TX_POWER_BITS_BOOSTP500_POS)
// #define DW1000_REG_TX_POWER_BITS_BOOSTP500_MSK        DW1000_REG_BIT_RANGE(15, 8)
#define DW1000_REG_TX_POWER_BITS_BOOSTP500              DW1000_REG_TX_POWER_BITS_BOOSTP500_MSK

#define DW1000_REG_TX_POWER_BITS_BOOSTPNORM_POS         (0U)
#define DW1000_REG_TX_POWER_BITS_BOOSTPNORM_LEN         (8U) /* 位 */
#define DW1000_REG_TX_POWER_BITS_BOOSTPNORM_MSK         (0xFFUL << DW1000_REG_TX_POWER_BITS_BOOSTPNORM_POS)
// #define DW1000_REG_TX_POWER_BITS_BOOSTPNORM_MSK       DW1000_REG_BIT_RANGE(7, 0)
#define DW1000_REG_TX_POWER_BITS_BOOSTPNORM             DW1000_REG_TX_POWER_BITS_BOOSTPNORM_MSK

/**
 * @brief
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 * @todo 根据用户手册来补充
 */
#define DW1000_REG_TX_POWER_BITS_TXPOWSD_POS            (16U)
#define DW1000_REG_TX_POWER_BITS_TXPOWSD_LEN            (8U) /* 位 */
#define DW1000_REG_TX_POWER_BITS_TXPOWSD_MSK            (0xFFUL << DW1000_REG_TX_POWER_BITS_TXPOWSD_POS)
// #define DW1000_REG_TX_POWER_BITS_TXPOWSD_MSK          DW1000_REG_BIT_RANGE(23, 16)
#define DW1000_REG_TX_POWER_BITS_TXPOWSD                DW1000_REG_TX_POWER_BITS_TXPOWSD_MSK

#define DW1000_REG_TX_POWER_BITS_TXPOWPHR_POS           (8U)
#define DW1000_REG_TX_POWER_BITS_TXPOWPHR_LEN           (8U) /* 位 */
#define DW1000_REG_TX_POWER_BITS_TXPOWPHR_MSK           (0xFFUL << DW1000_REG_TX_POWER_BITS_TXPOWPHR_POS)
// #define DW1000_REG_TX_POWER_BITS_TXPOWPHR_MSK         DW1000_REG_BIT_RANGE(15, 8)
#define DW1000_REG_TX_POWER_BITS_TXPOWPHR               DW1000_REG_TX_POWER_BITS_TXPOWPHR_MSK
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_BOOSTNORM_OFFSET                 0x00
#define DW1000_SUB_REG_BOOSTNORM_LEN                    (1) /* 字节 */

#define DW1000_SUB_REG_BOOSTP500_OFFSET                 0x01
#define DW1000_SUB_REG_BOOSTP500_LEN                    (1) /* 字节 */

#define DW1000_SUB_REG_BOOSTP250_OFFSET                 0x02
#define DW1000_SUB_REG_BOOSTP250_LEN                    (1) /* 字节 */

#define DW1000_SUB_REG_BOOSTP125_OFFSET                 0x03
#define DW1000_SUB_REG_BOOSTP125_LEN                    (1) /* 字节 */

#define DW1000_SUB_REG_TXPOWPHR_OFFSET                  0x01
#define DW1000_SUB_REG_TXPOWPHR_LEN                     (1) /* 字节 */

#define DW1000_SUB_REG_TXPOWSD_OFFSET                   0x02
#define DW1000_SUB_REG_TXPOWSD_LEN                      (1) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x1F - CHAN_CTRL 通道控制 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_CHAN_CTRL                            0x1F
#define DW1000_REG_CHAN_CTRL_LEN                        (4) /* 字节 */
/*------------------------------------- CHAN_CTRL 位定义 -------------------------------------*/
#define DW1000_REG_CHAN_CTRL_BITS_RX_PCODE_POS          (27U)
#define DW1000_REG_CHAN_CTRL_BITS_RX_PCODE_LEN          (5U) /* 位 */
#define DW1000_REG_CHAN_CTRL_BITS_RX_PCODE_MSK          (0x1FUL << DW1000_REG_CHAN_CTRL_BITS_RX_PCODE_POS)
// #define DW1000_REG_CHAN_CTRL_BITS_RX_PCODE_MSK        DW1000_REG_BIT_RANGE(31, 27)
#define DW1000_REG_CHAN_CTRL_BITS_RX_PCODE              DW1000_REG_CHAN_CTRL_BITS_RX_PCODE_MSK

#define DW1000_REG_CHAN_CTRL_BITS_TX_PCODE_POS          (22U)
#define DW1000_REG_CHAN_CTRL_BITS_TX_PCODE_LEN          (5U) /* 位 */
#define DW1000_REG_CHAN_CTRL_BITS_TX_PCODE_MSK          (0x1FUL << DW1000_REG_CHAN_CTRL_BITS_TX_PCODE_POS)
// #define DW1000_REG_CHAN_CTRL_BITS_TX_PCODE_MSK        DW1000_REG_BIT_RANGE(26, 22)
#define DW1000_REG_CHAN_CTRL_BITS_TX_PCODE              DW1000_REG_CHAN_CTRL_BITS_TX_PCODE_MSK

#define DW1000_REG_CHAN_CTRL_BIT_RNSSFD_POS             (21U)
#define DW1000_REG_CHAN_CTRL_BIT_RNSSFD_MSK             (0x1UL << DW1000_REG_CHAN_CTRL_BIT_RNSSFD_POS)
// #define DW1000_REG_CHAN_CTRL_BIT_RNSSFD_MSK          DW1000_REG_BIT(21)
#define DW1000_REG_CHAN_CTRL_BIT_RNSSFD                 DW1000_REG_CHAN_CTRL_BIT_RNSSFD_MSK

#define DW1000_REG_CHAN_CTRL_BIT_TNSSFD_POS             (20U)
#define DW1000_REG_CHAN_CTRL_BIT_TNSSFD_MSK             (0x1UL << DW1000_REG_CHAN_CTRL_BIT_TNSSFD_POS)
// #define DW1000_REG_CHAN_CTRL_BIT_TNSSFD_MSK           DW1000_REG_BIT(20)
#define DW1000_REG_CHAN_CTRL_BIT_TNSSFD                 DW1000_REG_CHAN_CTRL_BIT_TNSSFD_MSK

#define DW1000_REG_CHAN_CTRL_BITS_RXPRF_POS             (18U)
#define DW1000_REG_CHAN_CTRL_BITS_RXPRF_LEN             (2U) /* 位 */
#define DW1000_REG_CHAN_CTRL_BITS_RXPRF_MSK             (0x3UL << DW1000_REG_CHAN_CTRL_BITS_RXPRF_POS)
// #define DW1000_REG_CHAN_CTRL_BITS_RXPRF_MSK           DW1000_REG_BIT_RANGE(19, 18)
#define DW1000_REG_CHAN_CTRL_BITS_RXPRF                 DW1000_REG_CHAN_CTRL_BITS_RXPRF_MSK

#define DW1000_REG_CHAN_CTRL_BIT_DWSFD_POS              (17U)
#define DW1000_REG_CHAN_CTRL_BIT_DWSFD_MSK              (0x1UL << DW1000_REG_CHAN_CTRL_BIT_DWSFD_POS)
// #define DW1000_REG_CHAN_CTRL_BIT_DWSFD                DW1000_REG_BIT(17)
#define DW1000_REG_CHAN_CTRL_BIT_DWSFD                  DW1000_REG_CHAN_CTRL_BIT_DWSFD_MSK

#define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_POS           (4U)
#define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_LEN           (4U) /* 位 */
#define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_MSK           (0xFUL << DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_POS)
// #define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN             DW1000_REG_BIT_RANGE(7, 4)
#define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN               DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_MSK

#define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_POS           (0U)
#define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_LEN           (4U) /* 位 */
#define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_MSK           (0xFUL << DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_POS)
// #define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN             DW1000_REG_BIT_RANGE(3, 0)
#define DW1000_REG_CHAN_CTRL_BITS_RX_CHAN               DW1000_REG_CHAN_CTRL_BITS_RX_CHAN_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x20 - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */

/***********************************************************************************************
 * @brief 寄存器文件 0x21 - USR_SFD 用户定义SFD序列 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_USR_SFD                              0x21
#define DW1000_REG_USR_SFD_LEN                          (41) /* 字节 */
/*--------------------------------------- USR_SFD 位定义 ---------------------------------------*/
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_SFD_LENGTH_OFFSET                0x00
#define DW1000_SUB_REG_SFD_LENGTH_LEN                   (1) /* 字节 */

#define DW1000_SUB_REG_TX_SSFD_MAGL_OFFSET              0x01
#define DW1000_SUB_REG_TX_SSFD_MAGL_LEN                 (1) /* 字节 */

#define DW1000_SUB_REG_TX_SSFD_MAGH_OFFSET              0x02
#define DW1000_SUB_REG_TX_SSFD_MAGH_LEN                 (1) /* 字节 */

#define DW1000_SUB_REG_TX_SSFD_SGNL_OFFSET              0x03
#define DW1000_SUB_REG_TX_SSFD_SGNL_LEN                 (1) /* 字节 */

#define DW1000_SUB_REG_TX_SSFD_SGNH_OFFSET              0x04
#define DW1000_SUB_REG_TX_SSFD_SGNH_LEN                 (1) /* 字节 */

#define DW1000_SUB_REG_RX_SSFD_MAGL_OFFSET              0x05
#define DW1000_SUB_REG_RX_SSFD_MAGL_LEN                 (1) /* 字节 */

#define DW1000_SUB_REG_RX_SSFD_MAGH_OFFSET              0x06
#define DW1000_SUB_REG_RX_SSFD_MAGH_LEN                 (1) /* 字节 */

#define DW1000_SUB_REG_RX_SSFD_SGNL_OFFSET              0x07
#define DW1000_SUB_REG_RX_SSFD_SGNL_LEN                 (1) /* 字节 */

#define DW1000_SUB_REG_RX_SSFD_SGNH_OFFSET              0x08
#define DW1000_SUB_REG_RX_SSFD_SGNH_LEN                 (1) /* 字节 */

#define DW1000_SUB_REG_TX_LSFD_MAG_OFFSET               0x09
#define DW1000_SUB_REG_TX_LSFD_MAG_LEN                  (8) /* 字节 */

#define DW1000_SUB_REG_TX_LSFD_SGN_OFFSET               0x11
#define DW1000_SUB_REG_TX_LSFD_SGN_LEN                  (8) /* 字节 */

#define DW1000_SUB_REG_RX_LSFD_MAG_OFFSET               0x19
#define DW1000_SUB_REG_RX_LSFD_MAG_LEN                  (8) /* 字节 */

#define DW1000_SUB_REG_RX_LSFD_SGN_OFFSET               0x21
#define DW1000_SUB_REG_RX_LSFD_SGN_LEN                  (8) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x22 - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x23 - AGC_CTRL AGC配置与控制 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_AGC_CTRL                             0x23
#define DW1000_REG_AGC_CTRL_LEN                         (33) /* 字节 */
/*====================================== 子寄存器偏移地址 ======================================*/
/* 偏移地址 0x00 保留 */

#define DW1000_SUB_REG_AGC_CTRL1_OFFSET                 0x02
#define DW1000_SUB_REG_AGC_CTRL1_LEN                    (2) /* 字节 */

#define DW1000_SUB_REG_AGC_TUNE1_OFFSET                 0x04
#define DW1000_SUB_REG_AGC_TUNE1_LEN                    (2) /* 字节 */

/* 偏移地址 0x06 保留 */

#define DW1000_SUB_REG_AGC_TUNE2_OFFSET                 0x0C
#define DW1000_SUB_REG_AGC_TUNE2_LEN                    (4) /* 字节 */

/* 偏移地址 0x10 保留 */

#define DW1000_SUB_REG_AGC_TUNE3_OFFSET                 0x10
#define DW1000_SUB_REG_AGC_TUNE3_LEN                    (2) /* 字节 */

/* 偏移地址 0x14 保留 */

#define DW1000_SUB_REG_AGC_STAT1_OFFSET                 0x1E
#define DW1000_SUB_REG_AGC_STAT1_LEN                    (3) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_AGC_CTRL1_BIT_DIS_AM_POS         (0U)
#define DW1000_SUB_REG_AGC_CTRL1_BIT_DIS_AM_MSK         (0x1UL << DW1000_SUB_REG_AGC_CTRL1_BIT_DIS_AM_POS)
// #define DW1000_SUB_REG_AGC_CTRL1_BIT_DIS_AM_MSK       DW1000_REG_BIT(0)
#define DW1000_SUB_REG_AGC_CTRL1_BIT_DIS_AM             DW1000_SUB_REG_AGC_CTRL1_BIT_DIS_AM_MSK

#define DW1000_SUB_REG_AGC_STAT1_BITS_EDV2_POS          (11U)
#define DW1000_SUB_REG_AGC_STAT1_BITS_EDV2_LEN          (9) /* 位 */
#define DW1000_SUB_REG_AGC_STAT1_BITS_EDV2_MSK          (0x1FF << DW1000_SUB_REG_AGC_STAT1_BITS_EDV2_POS)
// #define DW1000_SUB_REG_AGC_STAT1_BITS_EDV2_MSK        DW1000_REG_BIT_RANGE(19, 11)
#define DW1000_SUB_REG_AGC_STAT1_BITS_EDV2              DW1000_SUB_REG_AGC_STAT1_BITS_EDV2_MSK

#define DW1000_SUB_REG_AGC_STAT1_BITS_EDG1_POS          (6U)
#define DW1000_SUB_REG_AGC_STAT1_BITS_EDG1_LEN          (5) /* 位 */
#define DW1000_SUB_REG_AGC_STAT1_BITS_EDG1_MSK          (0x1FUL << DW1000_SUB_REG_AGC_STAT1_BITS_EDG1_POS)
// #define DW1000_SUB_REG_AGC_STAT1_BITS_EDG1_MSK DW1000_REG_BIT_RANGE(10,6)
#define DW1000_SUB_REG_AGC_STAT1_BITS_EDG1              DW1000_SUB_REG_AGC_STAT1_BITS_EDG1_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x24 - EXT_SYNC 外部同步控制 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_EXT_SYNC                             0x24
#define DW1000_REG_EXT_SYNC_LEN                         (12) /* 字节 */
/*====================================== 子寄存器偏移地址 ======================================*/
#define DW1000_SUB_REG_EC_CTRL_OFFSET                   0x00
#define DW1000_SUB_REG_EC_CTRL_LEN                      (4) /* 字节 */

#define DW1000_SUB_REG_EC_RXTC_OFFSET                   0x04
#define DW1000_SUB_REG_EC_RXTC_LEN                      (4) /* 字节 */

#define DW1000_SUB_REG_EC_GOLP_OFFSET                   0x08
#define DW1000_SUB_REG_EC_GOLP_LEN                      (4) /* 字节 */
/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_EC_CTRL_BIT_OSTRM_POS            (11U)
#define DW1000_SUB_REG_EC_CTRL_BIT_OSTRM_MSK            (0x1UL << DW1000_SUB_REG_EC_CTRL_BIT_OSTRM_POS)
// #define DW1000_SUB_REG_EC_CTRL_BIT_OSTRM_MSK          DW1000_REG_BIT(11)
#define DW1000_SUB_REG_EC_CTRL_BIT_OSTRM                DW1000_SUB_REG_EC_CTRL_BIT_OSTRM_MSK

#define DW1000_SUB_REG_EC_CTRL_BITS_WAIT_POS            (3U)
#define DW1000_SUB_REG_EC_CTRL_BITS_WAIT_LEN            (8U) /* 位 */
#define DW1000_SUB_REG_EC_CTRL_BITS_WAIT_MSK            (0xFFUL << DW1000_SUB_REG_EC_CTRL_BITS_WAIT_POS)
// #define DW1000_SUB_REG_EC_CTRL_BITS_WAIT_MSK    DW1000_REG_BIT_RANGE(10,3)
#define DW1000_SUB_REG_EC_CTRL_BITS_WAIT                DW1000_SUB_REG_EC_CTRL_BITS_WAIT_MSK

#define DW1000_SUB_REG_EC_CTRL_BIT_PLLLDT_POS           (2U)
#define DW1000_SUB_REG_EC_CTRL_BIT_PLLLDT_MSK           (0x1UL << DW1000_SUB_REG_EC_CTRL_BIT_PLLLDT_POS)
// #define DW1000_SUB_REG_EC_CTRL_BIT_PLLLDT_MSK         DW1000_REG_BIT(2)
#define DW1000_SUB_REG_EC_CTRL_BIT_PLLLDT               DW1000_SUB_REG_EC_CTRL_BIT_PLLLDT_MSK

#define DW1000_SUB_REG_EC_CTRL_BIT_OSRSM_POS            (1U)
#define DW1000_SUB_REG_EC_CTRL_BIT_OSRSM_MSK            (0x1UL << DW1000_SUB_REG_EC_CTRL_BIT_OSRSM_POS)
// #define DW1000_SUB_REG_EC_CTRL_BIT_OSRSM_MSK          DW1000_REG_BIT(1)
#define DW1000_SUB_REG_EC_CTRL_BIT_OSRSM                DW1000_SUB_REG_EC_CTRL_BIT_OSRSM_MSK

#define DW1000_SUB_REG_EC_CTRL_BIT_OSTSM_POS            (0U)
#define DW1000_SUB_REG_EC_CTRL_BIT_OSTSM_MSK            (0x1UL << DW1000_SUB_REG_EC_CTRL_BIT_OSTSM_POS)
// #define DW1000_SUB_REG_EC_CTRL_BIT_OSTSM_MSK          DW1000_REG_BIT(0)
#define DW1000_SUB_REG_EC_CTRL_BIT_OSTSM                DW1000_SUB_REG_EC_CTRL_BIT_OSTSM_MSK

#define DW1000_SUB_REG_EC_RXTC_BITS_RX_TX_EST_POS       (0U)
#define DW1000_SUB_REG_EC_RXTC_BITS_RX_TX_EST_LEN       (32U) /* 位 */
#define DW1000_SUB_REG_EC_RXTC_BITS_RX_TX_EST_MSK       (0xFFFFFFFFUL << DW1000_SUB_REG_EC_RXTC_BITS_RX_TX_EST_POS)
// #define DW1000_SUB_REG_EC_RXTC_BITS_RX_TX_EST_MSK     DW1000_REG_BIT_RANGE(31, 0)
#define DW1000_SUB_REG_EC_RXTC_BITS_RX_TX_EST           DW1000_SUB_REG_EC_RXTC_BITS_RX_TX_EST_MSK

#define DW1000_SUB_REG_EC_GOLP_BITS_OFFSET_EXT_POS      (0U)
#define DW1000_SUB_REG_EC_GOLP_BITS_OFFSET_EXT_LEN      (6U) /* 位 */
#define DW1000_SUB_REG_EC_GOLP_BITS_OFFSET_EXT_MSK      (0x3FUL << DW1000_SUB_REG_EC_GOLP_BITS_OFFSET_EXT_POS)
// #define DW1000_SUB_REG_EC_GOLP_BITS_OFFSET_EXT_MSK    DW1000_REG_BIT_RANGE(5,0)
#define DW1000_SUB_REG_EC_GOLP_BITS_OFFSET_EXT          DW1000_SUB_REG_EC_GOLP_BITS_OFFSET_EXT_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x25 - ACC_MEM CIR累加器内存
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_ACC_MEM                              0x25
#define DW1000_REG_ACC_MEM_LEN                          (4064) /* 字节 */
/*====================================== 子寄存器偏移地址 ======================================*/
#define DW1000_SUB_REG_CIRx_REAL_OFFSET(x)              (0x00 + ((x) * 4)) /* x 取值 0-1015 */
#define DW1000_SUB_REG_CIRx_IMAG_OFFSET(x)              (0x02 + ((x) * 4))
#define DW1000_SUB_REG_CIRx_REAL_LEN                    (2) /* 字节 */
#define DW1000_SUB_REG_CIRx_IMAG_LEN                    (2) /* 字节 */

/***********************************************************************************************
 * @brief 寄存器文件 0x26 - GPIO_CTRL GPIO控制和状态 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_GPIO_CTRL                            0x26
#define DW1000_REG_GPIO_CTRL_LEN                        (44) /* 字节 */
/*====================================== 子寄存器偏移地址 ======================================*/

#define DW1000_SUB_REG_GPIO_MODE_OFFSET                 0x00
#define DW1000_SUB_REG_GPIO_MODE_LEN                    (4) /* 字节 */

/* 偏移地址 0x04 保留 */

#define DW1000_SUB_REG_GPIO_DIR_OFFSET                  0x08
#define DW1000_SUB_REG_GPIO_DIR_LEN                     (4) /* 字节 */

#define DW1000_SUB_REG_GPIO_DOUT_OFFSET                 0x0C
#define DW1000_SUB_REG_GPIO_DOUT_LEN                    (4) /* 字节 */

#define DW1000_SUB_REG_GPIO_IRQE_OFFSET                 0x10
#define DW1000_SUB_REG_GPIO_IRQE_LEN                    (4) /* 字节 */

#define DW1000_SUB_REG_GPIO_ISEN_OFFSET                 0x14
#define DW1000_SUB_REG_GPIO_ISEN_LEN                    (4) /* 字节 */

#define DW1000_SUB_REG_GPIO_IMODE_OFFSET                0x18
#define DW1000_SUB_REG_GPIO_IMODE_LEN                   (4) /* 字节 */

#define DW1000_SUB_REG_GPIO_IBES_OFFSET                 0x1C
#define DW1000_SUB_REG_GPIO_IBES_LEN                    (4) /* 字节 */

#define DW1000_SUB_REG_GPIO_ICLR_OFFSET                 0x20
#define DW1000_SUB_REG_GPIO_ICLR_LEN                    (4) /* 字节 */

#define DW1000_SUB_REG_GPIO_IDBE_OFFSET                 0x24
#define DW1000_SUB_REG_GPIO_IDBE_LEN                    (4) /* 字节 */

#define DW1000_SUB_REG_GPIO_RAW_OFFSET                  0x28
#define DW1000_SUB_REG_GPIO_RAW_LEN                     (4) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGPx_POS(x)      (6U + ((x) * 2)) /* x 取值 0-8 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGPx_LEN         (2U)             /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGPx_MSK(x)      (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGPx_POS(x))
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGPx(x)          DW1000_SUB_REG_GPIO_MODE_BITS_MSGPx_MSK(x)

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP8_POS         (22U)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP8_LEN         (2U) /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP8_MSK         (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGP8_POS)
// #define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP8_MSK       DW1000_REG_BIT_RANGE(23, 22)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP8             DW1000_SUB_REG_GPIO_MODE_BITS_MSGP8_MSK

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP7_POS         (20U)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP7_LEN         (2U) /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP7_MSK         (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGP7_POS)
// #define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP7_MSK       DW1000_REG_BIT_RANGE(21, 20)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP7             DW1000_SUB_REG_GPIO_MODE_BITS_MSGP7_MSK

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP6_POS         (18U)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP6_LEN         (2U) /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP6_MSK         (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGP6_POS)
// #define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP6_MSK       DW1000_REG_BIT_RANGE(19, 18)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP6             DW1000_SUB_REG_GPIO_MODE_BITS_MSGP6_MSK

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP5_POS         (16U)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP5_LEN         (2U) /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP5_MSK         (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGP5_POS)
// #define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP5_MSK       DW1000_REG_BIT_RANGE(17, 16)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP5             DW1000_SUB_REG_GPIO_MODE_BITS_MSGP5_MSK

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP4_POS         (14U)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP4_LEN         (2U) /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP4_MSK         (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGP4_POS)
// #define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP4_MSK       DW1000_REG_BIT_RANGE(15, 14)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP4             DW1000_SUB_REG_GPIO_MODE_BITS_MSGP4_MSK

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP3_POS         (12U)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP3_LEN         (2U) /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP3_MSK         (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGP3_POS)
// #define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP3_MSK       DW1000_REG_BIT_RANGE(13, 12)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP3             DW1000_SUB_REG_GPIO_MODE_BITS_MSGP3_MSK

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP2_POS         (10U)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP2_LEN         (2U) /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP2_MSK         (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGP2_POS)
// #define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP2_MSK       DW1000_REG_BIT_RANGE(11, 10)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP2             DW1000_SUB_REG_GPIO_MODE_BITS_MSGP2_MSK

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP1_POS         (8U)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP1_LEN         (2U) /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP1_MSK         (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGP1_POS)
// #define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP1_MSK       DW1000_REG_BIT_RANGE(9, 8)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP1             DW1000_SUB_REG_GPIO_MODE_BITS_MSGP1_MSK

#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP0_POS         (6U)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP0_LEN         (2U) /* 位 */
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP0_MSK         (0x3UL << DW1000_SUB_REG_GPIO_MODE_BITS_MSGP0_POS)
// #define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP0_MSK       DW1000_REG_BIT_RANGE(7, 6)
#define DW1000_SUB_REG_GPIO_MODE_BITS_MSGP0             DW1000_SUB_REG_GPIO_MODE_BITS_MSGP0_MSK


#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM8_POS            (20U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM8_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDM8_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDM8_MSK       DW1000_REG_BIT(20)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM8                DW1000_SUB_REG_GPIO_DIR_BIT_GDM8_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP8_POS            (16U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP8_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDP8_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDP8_MSK          DW1000_REG_BIT(16)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP8                DW1000_SUB_REG_GPIO_DIR_BIT_GDP8_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM7_POS            (15U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM7_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDM7_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDM7_MSK       DW1000_REG_BIT(15)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM7                DW1000_SUB_REG_GPIO_DIR_BIT_GDM7_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM6_POS            (14U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM6_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDM6_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDM6_MSK       DW1000_REG_BIT(14)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM6                DW1000_SUB_REG_GPIO_DIR_BIT_GDM6_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM5_POS            (13U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM5_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDM5_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDM5_MSK       DW1000_REG_BIT(13)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM5                DW1000_SUB_REG_GPIO_DIR_BIT_GDM5_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM4_POS            (12U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM4_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDM4_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDM4_MSK       DW1000_REG_BIT(12)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM4                DW1000_SUB_REG_GPIO_DIR_BIT_GDM4_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP7_POS            (11U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP7_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDP7_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDP7_MSK       DW1000_REG_BIT(11)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP7                DW1000_SUB_REG_GPIO_DIR_BIT_GDP7_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP6_POS            (10U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP6_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDP6_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDP6_MSK       DW1000_REG_BIT(10)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP6                DW1000_SUB_REG_GPIO_DIR_BIT_GDP6_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP5_POS            (9U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP5_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDP5_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDP5_MSK       DW1000_REG_BIT(9)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP5                DW1000_SUB_REG_GPIO_DIR_BIT_GDP5_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP4_POS            (8U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP4_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDP4_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDP4_MSK       DW1000_REG_BIT(8)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP4                DW1000_SUB_REG_GPIO_DIR_BIT_GDP4_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM3_POS            (7U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM3_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDM3_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDM3_MSK       DW1000_REG_BIT(7)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM3                DW1000_SUB_REG_GPIO_DIR_BIT_GDM3_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM2_POS            (6U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM2_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDM2_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDM2_MSK       DW1000_REG_BIT(6)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM2                DW1000_SUB_REG_GPIO_DIR_BIT_GDM2_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM1_POS            (5U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM1_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDM1_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDM1_MSK       DW1000_REG_BIT(5)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM1                DW1000_SUB_REG_GPIO_DIR_BIT_GDM1_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM0_POS            (4U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM0_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDM0_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDM0_MSK       DW1000_REG_BIT(4)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDM0                DW1000_SUB_REG_GPIO_DIR_BIT_GDM0_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP3_POS            (3U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP3_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDP3_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDP3_MSK       DW1000_REG_BIT(3)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP3                DW1000_SUB_REG_GPIO_DIR_BIT_GDP3_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP2_POS            (2U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP2_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDP2_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDP2_MSK       DW1000_REG_BIT(2)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP2                DW1000_SUB_REG_GPIO_DIR_BIT_GDP2_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP1_POS            (1U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP1_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDP1_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDP1_MSK       DW1000_REG_BIT(1)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP1                DW1000_SUB_REG_GPIO_DIR_BIT_GDP1_MSK

#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP0_POS            (0U)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP0_MSK            (0x1UL << DW1000_SUB_REG_GPIO_DIR_BIT_GDP0_POS)
// #define DW1000_SUB_REG_GPIO_DIR_BIT_GDP0_MSK       DW1000_REG_BIT(0)
#define DW1000_SUB_REG_GPIO_DIR_BIT_GDP0                DW1000_SUB_REG_GPIO_DIR_BIT_GDP0_MSK


#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM8_POS           (20U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM8_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOM8_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM8_MSK         DW1000_REG_BIT(20)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM8               DW1000_SUB_REG_GPIO_DOUT_BIT_GOM8_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP8_POS           (16U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP8_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOP8_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP8_MSK         DW1000_REG_BIT(16)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP8               DW1000_SUB_REG_GPIO_DOUT_BIT_GOP8_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM7_POS           (15U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM7_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOM7_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM7_MSK         DW1000_REG_BIT(15)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM7               DW1000_SUB_REG_GPIO_DOUT_BIT_GOM7_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM6_POS           (14U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM6_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOM6_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM6_MSK         DW1000_REG_BIT(14)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM6               DW1000_SUB_REG_GPIO_DOUT_BIT_GOM6_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM5_POS           (13U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM5_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOM5_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM5_MSK         DW1000_REG_BIT(13)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM5               DW1000_SUB_REG_GPIO_DOUT_BIT_GOM5_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM4_POS           (12U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM4_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOM4_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM4_MSK         DW1000_REG_BIT(12)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM4               DW1000_SUB_REG_GPIO_DOUT_BIT_GOM4_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP7_POS           (11U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP7_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOP7_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP7_MSK         DW1000_REG_BIT(11)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP7               DW1000_SUB_REG_GPIO_DOUT_BIT_GOP7_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP6_POS           (10U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP6_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOP6_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP6_MSK         DW1000_REG_BIT(10)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP6               DW1000_SUB_REG_GPIO_DOUT_BIT_GOP6_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP5_POS           (9U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP5_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOP5_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP5_MSK         DW1000_REG_BIT(9)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP5               DW1000_SUB_REG_GPIO_DOUT_BIT_GOP5_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP4_POS           (8U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP4_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOP4_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP4_MSK         DW1000_REG_BIT(8)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP4               DW1000_SUB_REG_GPIO_DOUT_BIT_GOP4_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM3_POS           (7U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM3_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOM3_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM3_MSK         DW1000_REG_BIT(7)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM3               DW1000_SUB_REG_GPIO_DOUT_BIT_GOM3_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM2_POS           (6U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM2_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOM2_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM2_MSK         DW1000_REG_BIT(6)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM2               DW1000_SUB_REG_GPIO_DOUT_BIT_GOM2_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM1_POS           (5U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM1_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOM1_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM1_MSK         DW1000_REG_BIT(5)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM1               DW1000_SUB_REG_GPIO_DOUT_BIT_GOM1_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM0_POS           (4U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM0_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOM0_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM0_MSK         DW1000_REG_BIT(4)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOM0               DW1000_SUB_REG_GPIO_DOUT_BIT_GOM0_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP3_POS           (3U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP3_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOP3_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP3_MSK         DW1000_REG_BIT(3)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP3               DW1000_SUB_REG_GPIO_DOUT_BIT_GOP3_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP2_POS           (2U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP2_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOP2_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP2_MSK         DW1000_REG_BIT(2)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP2               DW1000_SUB_REG_GPIO_DOUT_BIT_GOP2_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP1_POS           (1U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP1_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOP1_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP1_MSK         DW1000_REG_BIT(1)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP1               DW1000_SUB_REG_GPIO_DOUT_BIT_GOP1_MSK

#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP0_POS           (0U)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP0_MSK           (0x1UL << DW1000_SUB_REG_GPIO_DOUT_BIT_GOP0_POS)
// #define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP0_MSK         DW1000_REG_BIT(0)
#define DW1000_SUB_REG_GPIO_DOUT_BIT_GOP0               DW1000_SUB_REG_GPIO_DOUT_BIT_GOP0_MSK


#define DW1000_SUB_REG_GPIO_TRQE_BIT_GIRQEx_POS(x)      (x)
#define DW1000_SUB_REG_GPIO_TRQE_BIT_GIRQEx_MSK(x)      (0x1UL << DW1000_SUB_REG_GPIO_TRQE_BIT_GIRQEx_POS(x))
#define DW1000_SUB_REG_GPIO_TRQE_BIT_GIRQEx(x)          DW1000_SUB_REG_GPIO_TRQE_BIT_GIRQEx_MSK(x)

#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE8_POS         (8U)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE8_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE8_POS)
// #define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE8_MSK       DW1000_REG_BIT(8)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE8             DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE8_MSK

#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE7_POS         (7U)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE7_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE7_POS)
// #define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE7_MSK       DW1000_REG_BIT(7)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE7             DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE7_MSK

#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE6_POS         (6U)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE6_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE6_POS)
// #define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE6_MSK       DW1000_REG_BIT(6)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE6             DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE6_MSK

#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE5_POS         (5U)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE5_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE5_POS)
// #define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE5_MSK       DW1000_REG_BIT(5)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE5             DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE5_MSK

#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE4_POS         (4U)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE4_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE4_POS)
// #define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE4_MSK       DW1000_REG_BIT(4)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE4             DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE4_MSK

#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE3_POS         (3U)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE3_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE3_POS)
// #define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE3_MSK       DW1000_REG_BIT(3)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE3             DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE3_MSK

#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE2_POS         (2U)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE2_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE2_POS)
// #define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE2_MSK       DW1000_REG_BIT(2)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE2             DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE2_MSK

#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE1_POS         (1U)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE1_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE1_POS)
// #define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE1_MSK       DW1000_REG_BIT(1)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE1             DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE1_MSK

#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE0_POS         (0U)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE0_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE0_POS)
// #define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE0_MSK       DW1000_REG_BIT(0)
#define DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE0             DW1000_SUB_REG_GPIO_IRQE_BIT_GIRQE0_MSK


#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISENx_POS(x)      (x)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISENx_MSK(x)      (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISENx_POS(x))
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISENx(x)          DW1000_SUB_REG_GPIO_ISEN_BIT_GISENx_MSK(x)

#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN8_POS         (8U)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN8_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN8_POS)
// #define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN8_MSK       DW1000_REG_BIT(8)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN8             DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN8_MSK

#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN7_POS         (7U)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN7_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN7_POS)
// #define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN7_MSK       DW1000_REG_BIT(7)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN7             DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN7_MSK

#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN6_POS         (6U)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN6_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN6_POS)
// #define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN6_MSK       DW1000_REG_BIT(6)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN6             DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN6_MSK

#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN5_POS         (5U)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN5_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN5_POS)
// #define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN5_MSK       DW1000_REG_BIT(5)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN5             DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN5_MSK

#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN4_POS         (4U)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN4_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN4_POS)
// #define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN4_MSK       DW1000_REG_BIT(4)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN4             DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN4_MSK

#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN3_POS         (3U)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN3_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN3_POS)
// #define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN3_MSK       DW1000_REG_BIT(3)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN3             DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN3_MSK

#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN2_POS         (2U)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN2_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN2_POS)
// #define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN2_MSK       DW1000_REG_BIT(2)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN2             DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN2_MSK

#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN1_POS         (1U)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN1_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN1_POS)
// #define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN1_MSK       DW1000_REG_BIT(1)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN1             DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN1_MSK

#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN0_POS         (0U)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN0_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN0_POS)
// #define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN0_MSK       DW1000_REG_BIT(0)
#define DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN0             DW1000_SUB_REG_GPIO_ISEN_BIT_GISEN0_MSK


#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMODNx_POS(x)     (x)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMODx_MSK(x)      (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMODNx_POS(x))
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMODx(x)          DW1000_SUB_REG_GPIO_IMOD_BIT_GIMODx_MSK(x)

#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD8_POS         (8U)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD8_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD8_POS)
// #define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD8_MSK       DW1000_REG_BIT(8)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD8             DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD8_MSK

#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD7_POS         (7U)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD7_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD7_POS)
// #define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD7_MSK       DW1000_REG_BIT(7)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD7             DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD7_MSK

#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD6_POS         (6U)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD6_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD6_POS)
// #define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD6_MSK       DW1000_REG_BIT(6)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD6             DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD6_MSK

#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD5_POS         (5U)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD5_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD5_POS)
// #define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD5_MSK       DW1000_REG_BIT(5)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD5             DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD5_MSK

#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD4_POS         (4U)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD4_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD4_POS)
// #define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD4_MSK       DW1000_REG_BIT(4)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD4             DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD4_MSK

#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD3_POS         (3U)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD3_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD3_POS)
// #define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD3_MSK       DW1000_REG_BIT(3)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD3             DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD3_MSK

#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD2_POS         (2U)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD2_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD2_POS)
// #define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD2_MSK       DW1000_REG_BIT(2)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD2             DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD2_MSK

#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD1_POS         (1U)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD1_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD1_POS)
// #define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD1_MSK       DW1000_REG_BIT(1)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD1             DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD1_MSK

#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD0_POS         (0U)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD0_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD0_POS)
// #define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD0_MSK       DW1000_REG_BIT(0)
#define DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD0             DW1000_SUB_REG_GPIO_IMOD_BIT_GIMOD0_MSK

#define DW1000_SUB_REG_GPPIO_IBES_BIT_GIBESx_POS(x)     (x)
#define DW1000_SUB_REG_GPPIO_IBES_BIT_GIBESx_MSK(x)     (0x1UL << DW1000_SUB_REG_GPPIO_IBES_BIT_GIBESx_POS(x))
#define DW1000_SUB_REG_GPPIO_IBES_BIT_GIBESx(x)         DW1000_SUB_REG_GPPIO_IBES_BIT_GIBESx_MSK(x)

#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES8_POS         (8U)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES8_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IBES_BIT_GIBES8_POS)
// #define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES8_MSK        DW1000_REG_BIT(8)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES8             DW1000_SUB_REG_GPIO_IBES_BIT_GIBES8_MSK

#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES7_POS         (7U)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES7_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IBES_BIT_GIBES7_POS)
// #define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES7_MSK        DW1000_REG_BIT(7)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES7             DW1000_SUB_REG_GPIO_IBES_BIT_GIBES7_MSK

#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES6_POS         (6U)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES6_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IBES_BIT_GIBES6_POS)
// #define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES6_MSK        DW1000_REG_BIT(6)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES6             DW1000_SUB_REG_GPIO_IBES_BIT_GIBES6_MSK

#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES5_POS         (5U)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES5_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IBES_BIT_GIBES5_POS)
// #define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES5_MSK        DW1000_REG_BIT(5)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES5             DW1000_SUB_REG_GPIO_IBES_BIT_GIBES5_MSK

#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES4_POS         (4U)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES4_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IBES_BIT_GIBES4_POS)
// #define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES4_MSK        DW1000_REG_BIT(4)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES4             DW1000_SUB_REG_GPIO_IBES_BIT_GIBES4_MSK

#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES3_POS         (3U)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES3_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IBES_BIT_GIBES3_POS)
// #define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES3_MSK        DW1000_REG_BIT(3)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES3             DW1000_SUB_REG_GPIO_IBES_BIT_GIBES3_MSK

#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES2_POS         (2U)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES2_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IBES_BIT_GIBES2_POS)
// #define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES2_MSK        DW1000_REG_BIT(2)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES2             DW1000_SUB_REG_GPIO_IBES_BIT_GIBES2_MSK

#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES1_POS         (1U)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES1_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IBES_BIT_GIBES1_POS)
// #define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES1_MSK        DW1000_REG_BIT(1)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES1             DW1000_SUB_REG_GPIO_IBES_BIT_GIBES1_MSK

#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES0_POS         (0U)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES0_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IBES_BIT_GIBES0_POS)
// #define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES0_MSK        DW1000_REG_BIT(0)
#define DW1000_SUB_REG_GPIO_IBES_BIT_GIBES0             DW1000_SUB_REG_GPIO_IBES_BIT_GIBES0_MSK

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLRx_POS(x)      (x)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLRx_MSK(x)      (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLRx_POS(x))
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLRx(x)          DW1000_SUB_REG_GPIO_ICLR_BIT_GICLRx_MSK(x)

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR8_POS         (8U)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR8_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR8_POS)
// #define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR8_MSK       DW1000_REG_BIT(8)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR8             DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR8_MSK

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR7_POS         (7U)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR7_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR7_POS)
// #define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR7_MSK       DW1000_REG_BIT(7)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR7             DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR7_MSK

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR6_POS         (6U)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR6_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR6_POS)
// #define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR6_MSK       DW1000_REG_BIT(6)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR6             DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR6_MSK

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR5_POS         (5U)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR5_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR5_POS)
// #define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR5_MSK       DW1000_REG_BIT(5)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR5             DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR5_MSK

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR4_POS         (4U)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR4_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR4_POS)
// #define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR4_MSK       DW1000_REG_BIT(4)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR4             DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR4_MSK

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR3_POS         (3U)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR3_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR3_POS)
// #define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR3_MSK       DW1000_REG_BIT(3)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR3             DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR3_MSK

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR2_POS         (2U)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR2_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR2_POS)
// #define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR2_MSK       DW1000_REG_BIT(2)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR2             DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR2_MSK

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR1_POS         (1U)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR1_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR1_POS)
// #define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR1_MSK       DW1000_REG_BIT(1)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR1             DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR1_MSK

#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR0_POS         (0U)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR0_MSK         (0x1UL << DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR0_POS)
// #define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR0_MSK       DW1000_REG_BIT(0)
#define DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR0             DW1000_SUB_REG_GPIO_ICLR_BIT_GICLR0_MSK


#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBEx_POS(x)      (x)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBEx_MSK(x)      (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBEx_POS(x))
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBEx(x)          DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBEx_MSK(x)

#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE8_POS         (8U)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE8_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE8_POS)
// #define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE8_MSK       DW1000_REG_BIT(8)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE8             DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE8_MSK

#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE7_POS         (7U)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE7_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE7_POS)
// #define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE7_MSK        DW1000_REG_BIT(7)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE7             DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE7_MSK

#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE6_POS         (6U)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE6_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE6_POS)
// #define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE6_MSK        DW1000_REG_BIT(6)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE6             DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE6_MSK

#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE5_POS         (5U)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE5_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE5_POS)
// #define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE5_MSK        DW1000_REG_BIT(5)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE5             DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE5_MSK

#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE4_POS         (4U)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE4_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE4_POS)
// #define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE4_MSK        DW1000_REG_BIT(4)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE4             DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE4_MSK

#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE3_POS         (3U)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE3_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE3_POS)
// #define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE3_MSK        DW1000_REG_BIT(3)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE3             DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE3_MSK

#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE2_POS         (2U)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE2_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE2_POS)
// #define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE2_MSK        DW1000_REG_BIT(2)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE2             DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE2_MSK

#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE1_POS         (1U)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE1_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE1_POS)
// #define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE1_MSK        DW1000_REG_BIT(1)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE1             DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE1_MSK

#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE0_POS         (0U)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE0_MSK         (0x1UL << DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE0_POS)
// #define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE0_MSK        DW1000_REG_BIT(0)
#define DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE0             DW1000_SUB_REG_GPIO_IDBE_BIT_GIDBE0_MSK


#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWPx_POS(x)       (x)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWPx_MSK(x)       (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWPx_POS(x))
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWPx              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWPx_MSK(x)

#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP8_POS          (8U)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP8_MSK          (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP8_POS)
// #define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP8_MSK         DW1000_REG_BIT(8)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP8              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP8_MSK

#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP7_POS          (7U)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP7_MSK          (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP7_POS)
// #define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP7_MSK         DW1000_REG_BIT(7)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP7              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP7_MSK

#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP6_POS          (6U)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP6_MSK          (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP6_POS)
// #define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP6_MSK         DW1000_REG_BIT(6)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP6              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP6_MSK

#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP5_POS          (5U)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP5_MSK          (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP5_POS)
// #define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP5_MSK         DW1000_REG_BIT(5)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP5              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP5_MSK

#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP4_POS          (4U)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP4_MSK          (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP4_POS)
// #define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP4_MSK         DW1000_REG_BIT(4)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP4              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP4_MSK

#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP3_POS          (3U)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP3_MSK          (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP3_POS)
// #define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP3_MSK         DW1000_REG_BIT(3)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP3              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP3_MSK

#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP2_POS          (2U)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP2_MSK          (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP2_POS)
// #define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP2_MSK         DW1000_REG_BIT(2)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP2              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP2_MSK

#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP1_POS          (1U)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP1_MSK          (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP1_POS)
// #define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP1_MSK         DW1000_REG_BIT(1)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP1              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP1_MSK

#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP0_POS          (0U)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP0_MSK          (0x1UL << DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP0_POS)
// #define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP0_MSK         DW1000_REG_BIT(0)
#define DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP0              DW1000_SUB_REG_GPIO_RAW_BIT_GRAWP0_MSK


/***********************************************************************************************
 * @brief 寄存器文件 0x27 - DRX_CONF 数字接收器配置 [不定]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_DRX_CONF                             0x27
#define DW1000_REG_DRX_CONF_LEN                         (46) /* 字节 */
/*======================================= 子寄存器偏移地址 =====================================*/
/* 偏移地址 0x00 保留 */

#define DW1000_SUB_REG_DRX_TUNE0b_OFFSET                0x02
#define DW1000_SUB_REG_DRX_TUNE0b_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_DRX_TUNE1a_OFFSET                0x04
#define DW1000_SUB_REG_DRX_TUNE1a_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_DRX_TUNE1b_OFFSET                0x06
#define DW1000_SUB_REG_DRX_TUNE1b_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_DRX_TUNE2_OFFSET                 0x08
#define DW1000_SUB_REG_DRX_TUNE2_LEN                    (4) /* 字节 */

/* 偏移地址 0x0C 保留 */

#define DW1000_SUB_REG_DRX_SFDTOC_OFFSET                0x20
#define DW1000_SUB_REG_DRX_SFDTOC_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_DRX_PRETOC_OFFSET                0x24
#define DW1000_SUB_REG_DRX_PRETOC_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_DRX_TUNE4H_OFFSET                0x26
#define DW1000_SUB_REG_DRX_TUNE4H_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_DRX_CAR_INT_OFFSET               0x28
#define DW1000_SUB_REG_DRX_CAR_INT_LEN                  (3) /* 字节 */

#define DW1000_SUB_REG_RXPACC_NOSAT_OFFSET              0x2C
#define DW1000_SUB_REG_RXPACC_NOSAT_LEN                 (2) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/
// 【待补充】
#define DW1000_SUB_REG_DRX_CAR_INT_BITS_DRX_CAR_INT_POS (0U)
#define DW1000_SUB_REG_DRX_CAR_INT_BITS_DRX_CAR_INT_LEN (21U) /* 位 */
#define DW1000_SUB_REG_DRX_CAR_INT_BITS_DRX_CAR_INT_MSK (0x1FFFFFUL << DW1000_SUB_REG_DRX_CAR_INT_BITS_DRX_CAR_INT_POS)
// #define DW1000_SUB_REG_DRX_CAR_INT_BITS_DRX_CAR_INT_MSK  DW1000_REG_BIT_RANGE(20,0)
#define DW1000_SUB_REG_DRX_CAR_INT_BITS_DRX_CAR_INT     DW1000_SUB_REG_DRX_CAR_INT_BITS_DRX_CAR_INT_MSK


/***********************************************************************************************
 * @brief 寄存器文件 0x28 - RF_CONF 模拟射频配置块 [不定]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_RF_CONF                              0x28
#define DW1000_REG_RF_CONF_LEN                          (58) /* 字节 */
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_RF_CONF_OFFSET                   0x00
#define DW1000_SUB_REG_RF_CONF_LEN                      (4) /* 字节 */

/* 偏移地址 0x04 保留 */

#define DW1000_SUB_REG_RF_RXCTRLH_OFFSET                0x0B
#define DW1000_SUB_REG_RF_RXCTRLH_LEN                   (1) /* 字节 */

#define DW1000_SUB_REG_RF_TXCTRL_OFFSET                 0x0C
#define DW1000_SUB_REG_RF_TXCTRL_LEN                    (3) /* 字节 */

/* 偏移地址 0x10 保留 */

#define DW1000_SUB_REG_RF_STATUS_OFFSET                 0x2C
#define DW1000_SUB_REG_RF_STATUS_LEN                    (4) /* 字节 */

#define DW1000_SUB_REG_LDOTUNE_OFFSET                   0x30
#define DW1000_SUB_REG_LDOTUNE_LEN                      (5) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_POS          (21U)
#define DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_LEN          (2U) /* 位 */
#define DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_MSK          (0x3UL << DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_POS)
// #define DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_MSK         DW1000_REG_BIT_RANGE(22, 21)
#define DW1000_SUB_REG_RF_CONF_BITS_TXRXSW              DW1000_SUB_REG_RF_CONF_BITS_TXRXSW_MSK

#define DW1000_SUB_REG_RF_CONF_BITS_LDOFEN_POS          (16U)
#define DW1000_SUB_REG_RF_CONF_BITS_LDOFEN_LEN          (5U) /* 位 */
#define DW1000_SUB_REG_RF_CONF_BITS_LDOFEN_MSK          (0x1FUL << DW1000_SUB_REG_RF_CONF_BITS_LDOFEN_POS)
// #define DW1000_SUB_REG_RF_CONF_BITS_LDOFEN_MSK         DW1000_REG_BIT_RANGE(20, 16)
#define DW1000_SUB_REG_RF_CONF_BITS_LDOFEN              DW1000_SUB_REG_RF_CONF_BITS_LDOFEN_MSK

#define DW1000_SUB_REG_RF_CONF_BITS_PLLFEN_POS          (13U)
#define DW1000_SUB_REG_RF_CONF_BITS_PLLFEN_LEN          (3U) /* 位 */
#define DW1000_SUB_REG_RF_CONF_BITS_PLLFEN_MSK          (0x7UL << DW1000_SUB_REG_RF_CONF_BITS_PLLFEN_POS)
// #define DW1000_SUB_REG_RF_CONF_BITS_PLLFEN_MSK         DW1000_REG_BIT_RANGE(15, 13)
#define DW1000_SUB_REG_RF_CONF_BITS_PLLFEN              DW1000_SUB_REG_RF_CONF_BITS_PLLFEN_MSK

#define DW1000_SUB_REG_RF_CONF_BITS_TXFEN_POS           (8U)
#define DW1000_SUB_REG_RF_CONF_BITS_TXFEN_LEN           (5U) /* 位 */
#define DW1000_SUB_REG_RF_CONF_BITS_TXFEN_MSK           (0x1FUL << DW1000_SUB_REG_RF_CONF_BITS_TXFEN_POS)
// #define DW1000_SUB_REG_RF_CONF_BITS_TXFEN_MSK          DW1000_REG_BIT_RANGE(12, 8)
#define DW1000_SUB_REG_RF_CONF_BITS_TXFEN               DW1000_SUB_REG_RF_CONF_BITS_TXFEN_MSK

#define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMQ_POS          (9U)
#define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMQ_LEN          (3U) /* 位 */
#define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMQ_MSK          (0x7UL << DW1000_SUB_REG_RF_TXCTRL_BITS_TXMQ_POS)
// #define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMQ_MSK         DW1000_REG_BIT_RANGE(11, 9)
#define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMQ              DW1000_SUB_REG_RF_TXCTRL_BITS_TXMQ_MSK

#define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMTUNE_POS       (5U)
#define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMTUNE_LEN       (4U) /* 位 */
#define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMTUNE_MSK       (0xFUL << DW1000_SUB_REG_RF_TXCTRL_BITS_TXMTUNE_POS)
// #define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMTUNE_MSK      DW1000_REG_BIT_RANGE(8, 5)
#define DW1000_SUB_REG_RF_TXCTRL_BITS_TXMTUNE           DW1000_SUB_REG_RF_TXCTRL_BITS_TXMTUNE_MSK


#define DW1000_SUB_REG_RF_STATUS_BIT_RFPLLLOCK_POS      (3U)
#define DW1000_SUB_REG_RF_STATUS_BIT_RFPLLLOCK_MSK      (0x1UL << DW1000_SUB_REG_RF_STATUS_BIT_RFPLLLOCK_POS)
// #define DW1000_SUB_REG_RF_STATUS_BIT_RFPLLLOCK_MSK     DW1000_REG_BIT(3)
#define DW1000_SUB_REG_RF_STATUS_BIT_RFPLLLOCK          DW1000_SUB_REG_RF_STATUS_BIT_RFPLLLOCK_MSK

#define DW1000_SUB_REG_RF_STATUS_BIT_CPLLHIGH_POS       (2U)
#define DW1000_SUB_REG_RF_STATUS_BIT_CPLLHIGH_MSK       (0x1UL << DW1000_SUB_REG_RF_STATUS_BIT_CPLLHIGH_POS)
// #define DW1000_SUB_REG_RF_STATUS_BIT_CPLLHIGH_MSK      DW1000_REG_BIT(2)
#define DW1000_SUB_REG_RF_STATUS_BIT_CPLLHIGH           DW1000_SUB_REG_RF_STATUS_BIT_CPLLHIGH_MSK

#define DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOW_POS        (1U)
#define DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOW_MSK        (0x1UL << DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOW_POS)
// #define DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOW_MSK       DW1000_REG_BIT(1)
#define DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOW            DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOW_MSK

#define DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOCK_POS       (0U)
#define DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOCK_MSK       (0x1UL << DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOCK_POS)
// #define DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOCK_MSK       DW1000_REG_BIT(0)
#define DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOCK           DW1000_SUB_REG_RF_STATUS_BIT_CPLLLOCK_MSK


/***********************************************************************************************
 * @brief 寄存器文件 0x29 - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x2A - TX_CAL 发送器校准模块 [读写]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_TX_CAL                               0x2A
#define DW1000_REG_TX_CAL_LEN                           (1) /* 字节 */
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_TC_SARC_OFFSET                   0x00
#define DW1000_SUB_REG_TC_SARC_LEN                      (2) /* 字节 */

/* 偏移地址 0x02 保留 */

#define DW1000_SUB_REG_TC_SARL_OFFSET                   0x03
#define DW1000_SUB_REG_TC_SARL_LEN                      (3) /* 字节 */

#define DW1000_SUB_REG_SAR_LVBAT_OFFSET                 0x03
#define DW1000_SUB_REG_SAR_LVBAT_LEN                    (1) /* 字节 */

#define DW1000_SUB_REG_SAR_LTEMP_OFFSET                 0x04
#define DW1000_SUB_REG_SAR_LTEMP_LEN                    (1) /* 字节 */

#define DW1000_SUB_REG_TC_SARW_OFFSET                   0x06
#define DW1000_SUB_REG_TC_SARW_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_TC_PG_CTRL_OFFSET                0x08
#define DW1000_SUB_REG_TC_PG_CTRL_LEN                   (1) /* 字节 */

#define DW1000_SUB_REG_TC_PG_STATUS_OFFSET              0x09
#define DW1000_SUB_REG_TC_PG_STATUS_LEN                 (2) /* 字节 */

#define DW1000_SUB_REG_TC_PGDELAY_OFFSET                0x0B
#define DW1000_SUB_REG_TC_PGDELAY_LEN                   (1) /* 字节 */

#define DW1000_SUB_REG_TC_PGTEST_OFFSET                 0x0C
#define DW1000_SUB_REG_TC_PGTEST_LEN                    (1) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_TC_SARC_BIT_SAR_CTRL_POS         (0U)
#define DW1000_SUB_REG_TC_SARC_BIT_SAR_CTRL_MSK         (0x1UL << DW1000_SUB_REG_TC_SARC_BIT_SAR_CTRL_POS)
// #define DW1000_SUB_REG_TC_SARC_BIT_SAR_CTRL_MSK         DW1000_REG_BIT(0)
#define DW1000_SUB_REG_TC_SARC_BIT_SAR_CTRL             DW1000_SUB_REG_TC_SARC_BIT_SAR_CTRL_MSK

#define DW1000_SUB_REG_TC_SARL_BITS_SAR_LTEMP_POS       (8U)
#define DW1000_SUB_REG_TC_SARL_BITS_SAR_LTEMP_LEN       (8U) /* 位 */
#define DW1000_SUB_REG_TC_SARL_BITS_SAR_LTEMP_MSK       (0xFFUL << DW1000_SUB_REG_TC_SARL_BITS_SAR_LTEMP_POS)
// #define DW1000_SUB_REG_TC_SARL_BITS_SAR_LTEMP_MSK        DW1000_REG_BIT_RANGE(15, 8)
#define DW1000_SUB_REG_TC_SARL_BITS_SAR_LTEMP           DW1000_SUB_REG_TC_SARL_BITS_SAR_LTEMP_MSK

#define DW1000_SUB_REG_TC_SARL_BITS_SAR_LVBAT_POS       (0U)
#define DW1000_SUB_REG_TC_SARL_BITS_SAR_LVBAT_LEN       (8U) /* 位 */
#define DW1000_SUB_REG_TC_SARL_BITS_SAR_LVBAT_MSK       (0xFFUL << DW1000_SUB_REG_TC_SARL_BITS_SAR_LVBAT_POS)
// #define DW1000_SUB_REG_TC_SARL_BITS_SAR_LVBAT_MSK        DW1000_REG_BIT_RANGE(7, 0)
#define DW1000_SUB_REG_TC_SARL_BITS_SAR_LVBAT           DW1000_SUB_REG_TC_SARL_BITS_SAR_LVBAT_MSK


#define DW1000_SUB_REG_TC_SARW_BITS_SAR_WTEMP_POS       (8U)
#define DW1000_SUB_REG_TC_SARW_BITS_SAR_WTEMP_LEN       (8U) /* 位 */
#define DW1000_SUB_REG_TC_SARW_BITS_SAR_WTEMP_MSK       (0xFFUL << DW1000_SUB_REG_TC_SARW_BITS_SAR_WTEMP_POS)
// #define DW1000_SUB_REG_TC_SARW_BITS_SAR_WTEMP_MSK        DW1000_REG_BIT_RANGE(15, 8)
#define DW1000_SUB_REG_TC_SARW_BITS_SAR_WTEMP           DW1000_SUB_REG_TC_SARW_BITS_SAR_WTEMP_MSK

#define DW1000_SUB_REG_TC_SARW_BITS_SAR_WVBAT_POS       (0U)
#define DW1000_SUB_REG_TC_SARW_BITS_SAR_WVBAT_LEN       (8U) /* 位 */
#define DW1000_SUB_REG_TC_SARW_BITS_SAR_WVBAT_MSK       (0xFFUL << DW1000_SUB_REG_TC_SARW_BITS_SAR_WVBAT_POS)
// #define DW1000_SUB_REG_TC_SARW_BITS_SAR_WVBAT_MSK        DW1000_REG_BIT_RANGE(7, 0)
#define DW1000_SUB_REG_TC_SARW_BITS_SAR_WVBAT           DW1000_SUB_REG_TC_SARW_BITS_SAR_WVBAT_MSK

/* 【该位手册中没有，根据官方的库推出来的】 */
#define DW1000_SUB_REG_TC_PG_CTRL_BIT_DIR_CONV_POS      (7U)
#define DW1000_SUB_REG_TC_PG_CTRL_BIT_DIR_CONV_MSK      (0x1UL << DW1000_SUB_REG_TC_PG_CTRL_BIT_DIR_CONV_POS)
// #define DW1000_SUB_REG_TC_PG_CTRL_BIT_DIR_CONV_MSK       DW1000_REG_BIT(7)
#define DW1000_SUB_REG_TC_PG_CTRL_BIT_DIR_CONV          DW1000_SUB_REG_TC_PG_CTRL_BIT_DIR_CONV_MSK

#define DW1000_SUB_REG_TC_PG_CTRL_BITS_PG_TMEAS_POS     (2U)
#define DW1000_SUB_REG_TC_PG_CTRL_BITS_PG_TMEAS_LEN     (4U) /* 位 */
#define DW1000_SUB_REG_TC_PG_CTRL_BITS_PG_TMEAS_MSK     (0xFUL << DW1000_SUB_REG_TC_PG_CTRL_BITS_PG_TMEAS_POS)
// #define DW1000_SUB_REG_TC_PG_CTRL_BITS_PG_TMEAS_MSK      DW1000_REG_BIT_RANGE(5, 2)
#define DW1000_SUB_REG_TC_PG_CTRL_BITS_PG_TMEAS         DW1000_SUB_REG_TC_PG_CTRL_BITS_PG_TMEAS_MSK

#define DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START_POS      (0U)
#define DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START_MSK      (0x1UL << DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START_POS)
// #define DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START_MSK       DW1000_REG_BIT(0)
#define DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START          DW1000_SUB_REG_TC_PG_CTRL_BIT_PG_START_MSK

#define DW1000_SUB_REG_TC_PG_STATUS_BITS_DELAY_CNT_POS  (0U)
#define DW1000_SUB_REG_TC_PG_STATUS_BITS_DELAY_CNT_LEN  (12U) /* 位 */
#define DW1000_SUB_REG_TC_PG_STATUS_BITS_DELAY_CNT_MSK  (0xFFFUL << DW1000_SUB_REG_TC_PG_STATUS_BITS_DELAY_CNT_POS)
// #define DW1000_SUB_REG_TC_PG_STATUS_BITS_DELAY_CNT_MSK    DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_TC_PG_STATUS_BITS_DELAY_CNT      DW1000_SUB_REG_TC_PG_STATUS_BITS_DELAY_CNT_MSK


/***********************************************************************************************
 * @brief 寄存器文件 0x2B - FS_CTRL 频率合成器控制模块 [不定]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_FS_CTRL                              0x2B
#define DW1000_REG_FS_CTRL_LEN                          (21) /* 字节 */
/*======================================= 子寄存器偏移地址 =====================================*/
/* 偏移地址 0x00 保留 */

#define DW1000_SUB_REG_FS_PLLCFG_OFFSET                 0x07
#define DW1000_SUB_REG_FS_PLLCFG_LEN                    (4) /* 字节 */

#define DW1000_SUB_REG_FS_PLLTUNE_OFFSET                0x0B
#define DW1000_SUB_REG_FS_PLLTUNE_LEN                   (1) /* 字节 */

/* 偏移地址 0x0C 保留 */

#define DW1000_SUB_REG_FS_XTALT_OFFSET                  0x0E
#define DW1000_SUB_REG_FS_XTALT_LEN                     (1) /* 字节 */

/* 偏移地址 0x0F 保留 */
/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_FS_XTALT_BITS_XTALT_POS          (0U)
#define DW1000_SUB_REG_FS_XTALT_BITS_XTALT_LEN          (5U) /* 位 */
#define DW1000_SUB_REG_FS_XTALT_BITS_XTALT_MSK          (0x1FUL << DW1000_SUB_REG_FS_XTALT_BITS_XTALT_POS)
// #define DW1000_SUB_REG_FS_XTALT_BITS_XTALT_MSK           DW1000_REG_BIT_RANGE(4, 0)
#define DW1000_SUB_REG_FS_XTALT_BITS_XTALT              DW1000_SUB_REG_FS_XTALT_BITS_XTALT_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x2C - AON 常开系统控制接口 [不定]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_AON                                  0x2C
#define DW1000_REG_AON_LEN                              (12) /* 字节 */
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_AON_WCFG_OFFSET                  0x00
#define DW1000_SUB_REG_AON_WCFG_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_AON_CTRL_OFFSET                  0x02
#define DW1000_SUB_REG_AON_CTRL_LEN                     (1) /* 字节 */

#define DW1000_SUB_REG_AON_RDAT_OFFSET                  0x03
#define DW1000_SUB_REG_AON_RDAT_LEN                     (1) /* 字节 */

#define DW1000_SUB_REG_AON_ADDR_OFFSET                  0x04
#define DW1000_SUB_REG_AON_ADDR_LEN                     (1) /* 字节 */

/* 偏移地址 0x05 保留 */

#define DW1000_SUB_REG_AON_CFG0_OFFSET                  0x06
#define DW1000_SUB_REG_AON_CFG0_LEN                     (4) /* 字节 */

/* 这里为了方便，将4字节的子寄存器拆出一个2字节的子寄存器用于单独设置 */
#define DW1000_SUB_REG_SLEEP_TIM_OFFSET                 0x08
#define DW1000_SUB_REG_SLEEP_TIM_LEN                    (2) /* 字节 */

#define DW1000_SUB_REG_AON_CFG1_OFFSET                  0x0A
#define DW1000_SUB_REG_AON_CFG1_LEN                     (2) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/

#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLD_POS         (12U)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLD_MSK         (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLD_POS)
// #define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLD_MSK          DW1000_REG_BIT(12)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLD             DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLD_MSK

#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDE_POS        (11U)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDE_MSK        (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDE_POS)
// #define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDE_MSK         DW1000_REG_BIT(11)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDE            DW1000_SUB_REG_AON_WCFG_BIT_ONW_LLDE_MSK

#define DW1000_SUB_REG_AON_WCFG_BIT_PRES_SLEEP_POS      (8U)
#define DW1000_SUB_REG_AON_WCFG_BIT_PRES_SLEEP_MSK      (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_PRES_SLEEP_POS)
// #define DW1000_SUB_REG_AON_WCFG_BIT_PRES_SLEEP_MSK        DW1000_REG_BIT(8)
#define DW1000_SUB_REG_AON_WCFG_BIT_PRES_SLEEP          DW1000_SUB_REG_AON_WCFG_BIT_PRES_SLEEP_MSK

#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_L64P_POS        (7U)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_L64P_MSK        (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_L64P_POS)
// #define DW1000_SUB_REG_AON_WCFG_BIT_ONW_L64P_MSK          DW1000_REG_BIT(7)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_L64P            DW1000_SUB_REG_AON_WCFG_BIT_ONW_L64P_MSK

#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LDC_POS         (6U)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LDC_MSK         (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_LDC_POS)
// #define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LDC_MSK          DW1000_REG_BIT(6)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LDC             DW1000_SUB_REG_AON_WCFG_BIT_ONW_LDC_MSK

#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LEUI_POS        (3U)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LEUI_MSK        (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_LEUI_POS)
// #define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LEUI_MSK         DW1000_REG_BIT(3)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_LEUI            DW1000_SUB_REG_AON_WCFG_BIT_ONW_LEUI_MSK

#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_RX_POS          (1U)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_RX_MSK          (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_RX_POS)
// #define DW1000_SUB_REG_AON_WCFG_BIT_ONW_RX_MSK           DW1000_REG_BIT(1)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_RX              DW1000_SUB_REG_AON_WCFG_BIT_ONW_RX_MSK

#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_RADC_POS        (0U)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_RADC_MSK        (0x1UL << DW1000_SUB_REG_AON_WCFG_BIT_ONW_RADC_POS)
// #define DW1000_SUB_REG_AON_WCFG_BIT_ONW_RADC_MSK          DW1000_REG_BIT(0)
#define DW1000_SUB_REG_AON_WCFG_BIT_ONW_RADC            DW1000_SUB_REG_AON_WCFG_BIT_ONW_RADC_MSK

#define DW1000_SUB_REG_AON_CTRL_BIT_DCA_ENAB_POS        (7U)
#define DW1000_SUB_REG_AON_CTRL_BIT_DCA_ENAB_MSK        (0x1UL << DW1000_SUB_REG_AON_CTRL_BIT_DCA_ENAB_POS)
// #define DW1000_SUB_REG_AON_CTRL_BIT_DCA_ENAB_MSK         DW1000_REG_BIT(7)
#define DW1000_SUB_REG_AON_CTRL_BIT_DCA_ENAB            DW1000_SUB_REG_AON_CTRL_BIT_DCA_ENAB_MSK

#define DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_POS        (3U)
#define DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_MSK        (0x1UL << DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_POS)
// #define DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_MSK         DW1000_REG_BIT(3)
#define DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ            DW1000_SUB_REG_AON_CTRL_BIT_DCA_READ_MSK

#define DW1000_SUB_REG_AON_CTRL_BIT_UPL_CFG_POS         (2U)
#define DW1000_SUB_REG_AON_CTRL_BIT_UPL_CFG_MSK         (0x1UL << DW1000_SUB_REG_AON_CTRL_BIT_UPL_CFG_POS)
// #define DW1000_SUB_REG_AON_CTRL_BIT_UPL_CFG_MSK          DW1000_REG_BIT(2)
#define DW1000_SUB_REG_AON_CTRL_BIT_UPL_CFG             DW1000_SUB_REG_AON_CTRL_BIT_UPL_CFG_MSK

#define DW1000_SUB_REG_AON_CTRL_BIT_SAVE_POS            (1U)
#define DW1000_SUB_REG_AON_CTRL_BIT_SAVE_MSK            (0x1UL << DW1000_SUB_REG_AON_CTRL_BIT_SAVE_POS)
// #define DW1000_SUB_REG_AON_CTRL_BIT_SAVE_MSK             DW1000_REG_BIT(1)
#define DW1000_SUB_REG_AON_CTRL_BIT_SAVE                DW1000_SUB_REG_AON_CTRL_BIT_SAVE_MSK

#define DW1000_SUB_REG_AON_CTRL_BIT_RESTORE_POS         (0U)
#define DW1000_SUB_REG_AON_CTRL_BIT_RESTORE_MSK         (0x1UL << DW1000_SUB_REG_AON_CTRL_BIT_RESTORE_POS)
// #define DW1000_SUB_REG_AON_CTRL_BIT_RESTORE_MSK          DW1000_REG_BIT(0)
#define DW1000_SUB_REG_AON_CTRL_BIT_RESTORE             DW1000_SUB_REG_AON_CTRL_BIT_RESTORE_MSK


#define DW1000_SUB_REG_AON_CFG0_BITS_SLEEP_TIM_POS      (16U)
#define DW1000_SUB_REG_AON_CFG0_BITS_SLEEP_TIM_LEN      (16U) /* 位 */
#define DW1000_SUB_REG_AON_CFG0_BITS_SLEEP_TIM_MSK      (0xFFFFUL << DW1000_SUB_REG_AON_CFG0_BITS_SLEEP_TIM_POS)
// #define DW1000_SUB_REG_AON_CFG0_BITS_SLEEP_TIM_MSK       DW1000_REG_BIT_RANGE(31, 16)
#define DW1000_SUB_REG_AON_CFG0_BITS_SLEEP_TIM          DW1000_SUB_REG_AON_CFG0_BITS_SLEEP_TIM_MSK

#define DW1000_SUB_REG_AON_CFG0_BITS_LPCLKDIVA_POS      (5U)
#define DW1000_SUB_REG_AON_CFG0_BITS_LPCLKDIVA_LEN      (11) /* 位 */
#define DW1000_SUB_REG_AON_CFG0_BITS_LPCLKDIVA_MSK      (0x7FFUL << DW1000_SUB_REG_AON_CFG0_BITS_LPCLKDIVA_POS)
// #define DW1000_SUB_REG_AON_CFG0_BITS_LPCLKDIVA_MSK       DW1000_REG_BIT_RANGE(15, 5)
#define DW1000_SUB_REG_AON_CFG0_BITS_LPCLKDIVA          DW1000_SUB_REG_AON_CFG0_BITS_LPCLKDIVA_MSK

#define DW1000_SUB_REG_AON_CFG0_BIT_LPDIV_EN_POS        (4U)
#define DW1000_SUB_REG_AON_CFG0_BIT_LPDIV_EN_MSK        (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_LPDIV_EN_POS)
// #define DW1000_SUB_REG_AON_CFG0_BIT_LPDIV_EN_MSK        DW1000_REG_BIT(4)
#define DW1000_SUB_REG_AON_CFG0_BIT_LPDIV_EN            DW1000_SUB_REG_AON_CFG0_BIT_LPDIV_EN_MSK

#define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_CNT_POS        (3U)
#define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_CNT_MSK        (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_WAKE_CNT_POS)
// #define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_CNT_MSK        DW1000_REG_BIT(3)
#define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_CNT            DW1000_SUB_REG_AON_CFG0_BIT_WAKE_CNT_MSK

#define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_SPI_POS        (2U)
#define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_SPI_MSK        (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_WAKE_SPI_POS)
// #define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_SPI_MSK        DW1000_REG_BIT(2)
#define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_SPI            DW1000_SUB_REG_AON_CFG0_BIT_WAKE_SPI_MSK

#define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_PIN_POS        (1U)
#define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_PIN_MSK        (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_WAKE_PIN_POS)
// #define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_PIN_MSK        DW1000_REG_BIT(1)
#define DW1000_SUB_REG_AON_CFG0_BIT_WAKE_PIN            DW1000_SUB_REG_AON_CFG0_BIT_WAKE_PIN_MSK

#define DW1000_SUB_REG_AON_CFG0_BIT_SLEEP_EN_POS        (0U)
#define DW1000_SUB_REG_AON_CFG0_BIT_SLEEP_EN_MSK        (0x1UL << DW1000_SUB_REG_AON_CFG0_BIT_SLEEP_EN_POS)
// #define DW1000_SUB_REG_AON_CFG0_BIT_SLEEP_EN_MSK        DW1000_REG_BIT(0)
#define DW1000_SUB_REG_AON_CFG0_BIT_SLEEP_EN            DW1000_SUB_REG_AON_CFG0_BIT_SLEEP_EN_MSK

#define DW1000_SUB_REG_AON_CFG1_BIT_LPOSC_CAL_POS       (2U)
#define DW1000_SUB_REG_AON_CFG1_BIT_LPOSC_CAL_MSK       (0x1UL << DW1000_SUB_REG_AON_CFG1_BIT_LPOSC_CAL_POS)
// #define DW1000_SUB_REG_AON_CFG1_BIT_LPOSC_CAL_MSK          DW1000_REG_BIT(2)
#define DW1000_SUB_REG_AON_CFG1_BIT_LPOSC_CAL           DW1000_SUB_REG_AON_CFG1_BIT_LPOSC_CAL_MSK

#define DW1000_SUB_REG_AON_CFG1_BIT_SMXX_POS            (1U)
#define DW1000_SUB_REG_AON_CFG1_BIT_SMXX_MSK            (0x1UL << DW1000_SUB_REG_AON_CFG1_BIT_SMXX_POS)
// #define DW1000_SUB_REG_AON_CFG1_BIT_SMXX_MSK             DW1000_REG_BIT(1)
#define DW1000_SUB_REG_AON_CFG1_BIT_SMXX                DW1000_SUB_REG_AON_CFG1_BIT_SMXX_MSK

#define DW1000_SUB_REG_AON_CFG1_BIT_SLEEP_CEN_POS       (0U)
#define DW1000_SUB_REG_AON_CFG1_BIT_SLEEP_CEN_MSK       (0x1UL << DW1000_SUB_REG_AON_CFG1_BIT_SLEEP_CEN_POS)
// #define DW1000_SUB_REG_AON_CFG1_BIT_SLEEP_CEN_MSK         DW1000_REG_BIT(0)
#define DW1000_SUB_REG_AON_CFG1_BIT_SLEEP_CEN           DW1000_SUB_REG_AON_CFG1_BIT_SLEEP_CEN_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x2D - OTP_IF OTP内存接口 [不定]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_OTP_IF                               0x2D
#define DW1000_REG_OTP_IF_LEN                           (18) /* 字节 */
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_OTP_WDAT_OFFSET                  0x00
#define DW1000_SUB_REG_OTP_WDAT_LEN                     (4) /* 字节 */

#define DW1000_SUB_REG_OTP_ADDR_OFFSET                  0x04
#define DW1000_SUB_REG_OTP_ADDR_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_OTP_CTRL_OFFSET                  0x06
#define DW1000_SUB_REG_OTP_CTRL_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_OTP_STAT_OFFSET                  0x08
#define DW1000_SUB_REG_OTP_STAT_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_OTP_RDAT_OFFSET                  0x0A
#define DW1000_SUB_REG_OTP_RDAT_LEN                     (4) /* 字节 */

#define DW1000_SUB_REG_OTP_SRDAT_OFFSET                 0x0E
#define DW1000_SUB_REG_OTP_SRDAT_LEN                    (4) /* 字节 */

#define DW1000_SUB_REG_OTP_SF_OFFSET                    0x12
#define DW1000_SUB_REG_OTP_SF_LEN                       (1) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_OTP_ADDR_BITS_OTPADDR_POS        (0U)
#define DW1000_SUB_REG_OTP_ADDR_BITS_OTPADDR_LEN        (11U) /* 位*/
#define DW1000_SUB_REG_OTP_ADDR_BITS_OTPADDR_MSK        (0x7FFUL << DW1000_SUB_REG_OTP_ADDR_BITS_OTPADDR_POS)
// #define DW1000_SUB_REG_OTP_ADDR_BITS_OTPADDR_MSK         DW1000_REG_BIT_RANGE(10, 0)
#define DW1000_SUB_REG_OTP_ADDR_BITS_OTPADDR            DW1000_SUB_REG_OTP_ADDR_BITS_OTPADDR_MSK

#define DW1000_SUB_REG_OTP_CTRL_BIT_LDELOAD_POS         (15U)
#define DW1000_SUB_REG_OTP_CTRL_BIT_LDELOAD_MSK         (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_LDELOAD_POS)
// #define DW1000_SUB_REG_OTP_CTRL_BIT_LDELOAD_MSK           DW1000_REG_BIT(15)
#define DW1000_SUB_REG_OTP_CTRL_BIT_LDELOAD             DW1000_SUB_REG_OTP_CTRL_BIT_LDELOAD_MSK


/* 【该位根据官方配套库代码推算出来实际只有两位】 */
#define DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_POS          (9U)
#define DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_LEN          (2U) /* 位 */
#define DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_MSK          (0x3UL << DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_POS)
// #define DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_POS          (7U)
// #define DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_LEN          (4U) /* 位 */
// #define DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_MSK          (0xFUL << DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_POS)
// // #define DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_MSK            DW1000_REG_BIT_RANGE(10, 7)
#define DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR              DW1000_SUB_REG_OTP_CTRL_BITS_OTPMR_MSK

/* 【该位根据官方配套库代码推算出来的，不在用户手册中】 */
#define DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_POS         (8U)
#define DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_MSK         (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_POS)
// #define DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_MSK         DW1000_REG_BIT(8)
#define DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL             DW1000_SUB_REG_OTP_CTRL_BIT_MODESEL_MSK

/* 【该位根据官方配套库代码推算出来的，不在用户手册中】 */
#define DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_POS          (7U)
#define DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK          (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_POS)
// #define DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK          DW1000_REG_BIT(7)
#define DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD              DW1000_SUB_REG_OTP_CTRL_BIT_AUXUPD_MSK


#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPPROG_POS         (6U)
#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPPROG_MSK         (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_OTPPROG_POS)
// #define DW1000_SUB_REG_OTP_CTRL_BIT_OTPPROG_MSK           DW1000_REG_BIT(6)
#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPPROG             DW1000_SUB_REG_OTP_CTRL_BIT_OTPPROG_MSK

#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_POS         (3U)
#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK         (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_POS)
// #define DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK          DW1000_REG_BIT(3)
#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR             DW1000_SUB_REG_OTP_CTRL_BIT_OTPMRWR_MSK

#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPREAD_POS         (1U)
#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPREAD_MSK         (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_OTPREAD_POS)
// #define DW1000_SUB_REG_OTP_CTRL_BIT_OTPREAD_MSK          DW1000_REG_BIT(1)
#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPREAD             DW1000_SUB_REG_OTP_CTRL_BIT_OTPREAD_MSK

#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPRDEN_POS         (0U)
#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPRDEN_MSK         (0x1UL << DW1000_SUB_REG_OTP_CTRL_BIT_OTPRDEN_POS)
// #define DW1000_SUB_REG_OTP_CTRL_BIT_OTPRDEN_MSK          DW1000_REG_BIT(0)
#define DW1000_SUB_REG_OTP_CTRL_BIT_OTPRDEN             DW1000_SUB_REG_OTP_CTRL_BIT_OTPRDEN_MSK

#define DW1000_SUB_REG_OTP_STAT_BIT_OTPVPOK_POS         (1U)
#define DW1000_SUB_REG_OTP_STAT_BIT_OTPVPOK_MSK         (0x1UL << DW1000_SUB_REG_OTP_STAT_BIT_OTPVPOK_POS)
// #define DW1000_SUB_REG_OTP_STAT_BIT_OTPVPOK_MSK          DW1000_REG_BIT(1)
#define DW1000_SUB_REG_OTP_STAT_BIT_OTPVPOK             DW1000_SUB_REG_OTP_STAT_BIT_OTPVPOK_MSK

#define DW1000_SUB_REG_OTP_STAT_BIT_OTPPRGD_POS         (0U)
#define DW1000_SUB_REG_OTP_STAT_BIT_OTPPRGD_MSK         (0x1UL << DW1000_SUB_REG_OTP_STAT_BIT_OTPPRGD_POS)
// #define DW1000_SUB_REG_OTP_STAT_BIT_OTPPRGD_MSK          DW1000_REG_BIT(0)
#define DW1000_SUB_REG_OTP_STAT_BIT_OTPPRGD             DW1000_SUB_REG_OTP_STAT_BIT_OTPPRGD_MSK

#define DW1000_SUB_REG_OTP_SF_BITS_OPS_SEL_POS          (5U)
#define DW1000_SUB_REG_OTP_SF_BITS_OPS_SEL_LEN          (2U) /* 位 */
#define DW1000_SUB_REG_OTP_SF_BITS_OPS_SEL_MSK          (0x3UL << DW1000_SUB_REG_OTP_SF_BITS_OPS_SEL_POS)
// #define DW1000_SUB_REG_OTP_SF_BITS_OPS_SEL_MSK           DW1000_REG_BIT_RANGE(6, 5)
#define DW1000_SUB_REG_OTP_SF_BITS_OPS_SEL              DW1000_SUB_REG_OTP_SF_BITS_OPS_SEL_MSK

#define DW1000_SUB_REG_OTP_SF_BIT_LDO_KICK_POS          (1U)
#define DW1000_SUB_REG_OTP_SF_BIT_LDO_KICK_MSK          (0x1UL << DW1000_SUB_REG_OTP_SF_BIT_LDO_KICK_POS)
// #define DW1000_SUB_REG_OTP_SF_BIT_LDO_KICK_MSK           DW1000_REG_BIT(1)
#define DW1000_SUB_REG_OTP_SF_BIT_LDO_KICK              DW1000_SUB_REG_OTP_SF_BIT_LDO_KICK_MSK

#define DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK_POS          (0U)
#define DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK_MSK          (0x1UL << DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK_POS)
// #define DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK_MSK           DW1000_REG_BIT(0)
#define DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK              DW1000_SUB_REG_OTP_SF_BIT_OPS_KICK_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x2E - LDE_IF 上升沿检测接口 [不定]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_LDE_IF                               0x2E
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_LDE_THRESH_OFFSET                0x0000
#define DW1000_SUB_REG_LDE_THRESH_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_LDE_CFG1_OFFSET                  0x0806
#define DW1000_SUB_REG_LDE_CFG1_LEN                     (1) /* 字节 */

#define DW1000_SUB_REG_LDE_PPINDX_OFFSET                0x1000
#define DW1000_SUB_REG_LDE_PPINDX_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_LDE_PPAMPL_OFFSET                0x1002
#define DW1000_SUB_REG_LDE_PPAMPL_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_LDE_RXANTD_OFFSET                0x1804
#define DW1000_SUB_REG_LDE_RXANTD_LEN                   (2) /* 字节 */

#define DW1000_SUB_REG_LDE_CFG2_OFFSET                  0x1806
#define DW1000_SUB_REG_LDE_CFG2_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_LDE_REPC_OFFSET                  0x2804
#define DW1000_SUB_REG_LDE_REPC_LEN                     (2) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/
#define DW1000_SUB_REG_LDE_CFG1_BITS_PMULT_POS          (5U)
#define DW1000_SUB_REG_LDE_CFG1_BITS_PMULT_LEN          (3U) /* 位 */
#define DW1000_SUB_REG_LDE_CFG1_BITS_PMULT_MSK          (0x7UL << DW1000_SUB_REG_LDE_CFG1_BITS_PMULT_POS)
// #define DW1000_SUB_REG_LDE_CFG1_BITS_PMULT_MSK           DW1000_REG_BIT_RANGE(7, 5)
#define DW1000_SUB_REG_LDE_CFG1_BITS_PMULT              DW1000_SUB_REG_LDE_CFG1_BITS_PMULT_MSK

#define DW1000_SUB_REG_LDE_CFG1_BITS_NTM_POS            (0U)
#define DW1000_SUB_REG_LDE_CFG1_BITS_NTM_LEN            (5U)
#define DW1000_SUB_REG_LDE_CFG1_BITS_NTM_MSK            (0x1FUL << DW1000_SUB_REG_LDE_CFG1_BITS_NTM_POS)
// #define DW1000_SUB_REG_LDE_CFG1_BITS_NTM_MSK             DW1000_REG_BIT_RANGE(4, 0)
#define DW1000_SUB_REG_LDE_CFG1_BITS_NTM                DW1000_SUB_REG_LDE_CFG1_BITS_NTM_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x2F - DIG_DIAG 数字诊断接口 [不定]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_DIG_DIAG                             0x2F
#define DW1000_REG_DIG_DIAG_LEN                         (41) /* 字节 */
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_EVC_CTRL_OFFSET                  0x00
#define DW1000_SUB_REG_EVC_CTRL_LEN                     (4) /* 字节 */

#define DW1000_SUB_REG_EVC_PHE_OFFSET                   0x04
#define DW1000_SUB_REG_EVC_PHE_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_EVC_RSE_OFFSET                   0x06
#define DW1000_SUB_REG_EVC_RSE_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_EVC_FCG_OFFSET                   0x08
#define DW1000_SUB_REG_EVC_FCG_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_EVC_FCE_OFFSET                   0x0A
#define DW1000_SUB_REG_EVC_FCE_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_EVC_FFR_OFFSET                   0x0C
#define DW1000_SUB_REG_EVC_FFR_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_EVC_OVR_OFFSET                   0x0E
#define DW1000_SUB_REG_EVC_OVR_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_EVC_STO_OFFSET                   0x10
#define DW1000_SUB_REG_EVC_STO_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_EVC_PTO_OFFSET                   0x12
#define DW1000_SUB_REG_EVC_PTO_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_EVC_FWTO_OFFSET                  0x14
#define DW1000_SUB_REG_EVC_FWTO_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_EVC_TXFS_OFFSET                  0x16
#define DW1000_SUB_REG_EVC_TXFS_LEN                     (2) /* 字节 */

#define DW1000_SUB_REG_EVC_HPW_OFFSET                   0x18
#define DW1000_SUB_REG_EVC_HPW_LEN                      (2) /* 字节 */

#define DW1000_SUB_REG_EVC_TPW_OFFSET                   0x1A
#define DW1000_SUB_REG_EVC_TPW_LEN                      (2) /* 字节 */

/* 偏移地址 0x1C 保留 */

#define DW1000_SUB_REG_DIAG_TMC_OFFSET                  0x24
#define DW1000_SUB_REG_DIAG_TMC_LEN                     (2) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/

#define DW1000_SUB_REG_EVC_CTRL_BIT_EVC_CLR_POS         (1U)
#define DW1000_SUB_REG_EVC_CTRL_BIT_EVC_CLR_MSK         (0x1UL << DW1000_SUB_REG_EVC_CTRL_BIT_EVC_CLR_POS)
// #define DW1000_SUB_REG_EVC_CTRL_BIT_EVC_CLR_MSK          DW1000_REG_BIT(1)
#define DW1000_SUB_REG_EVC_CTRL_BIT_EVC_CLR             DW1000_SUB_REG_EVC_CTRL_BIT_EVC_CLR_MSK

#define DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN_POS          (0U)
#define DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN_MSK          (0x1UL << DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN_POS)
// #define DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN_MSK           DW1000_REG_BIT(0)
#define DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN              DW1000_SUB_REG_EVC_CTRL_BIT_EVC_EN_MSK

#define DW1000_SUB_REG_EVC_PHE_BITS_EVC_PHE_POS         (0U)
#define DW1000_SUB_REG_EVC_PHE_BITS_EVC_PHE_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_PHE_BITS_EVC_PHE_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_PHE_BITS_EVC_PHE_POS)
// #define DW1000_SUB_REG_EVC_PHE_BITS_EVC_PHE_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_PHE_BITS_EVC_PHE             DW1000_SUB_REG_EVC_PHE_BITS_EVC_PHE_MSK

#define DW1000_SUB_REG_EVC_RSE_BITS_EVC_RSE_POS         (0U)
#define DW1000_SUB_REG_EVC_RSE_BITS_EVC_RSE_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_RSE_BITS_EVC_RSE_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_RSE_BITS_EVC_RSE_POS)
// #define DW1000_SUB_REG_EVC_RSE_BITS_EVC_RSE_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_RSE_BITS_EVC_RSE             DW1000_SUB_REG_EVC_RSE_BITS_EVC_RSE_MSK

#define DW1000_SUB_REG_EVC_FCG_BITS_EVC_FCG_POS         (0U)
#define DW1000_SUB_REG_EVC_FCG_BITS_EVC_FCG_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_FCG_BITS_EVC_FCG_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_FCG_BITS_EVC_FCG_POS)
// #define DW1000_SUB_REG_EVC_FCG_BITS_EVC_FCG_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_FCG_BITS_EVC_FCG             DW1000_SUB_REG_EVC_FCG_BITS_EVC_FCG_MSK

#define DW1000_SUB_REG_EVC_FCE_BITS_EVC_FCE_POS         (0U)
#define DW1000_SUB_REG_EVC_FCE_BITS_EVC_FCE_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_FCE_BITS_EVC_FCE_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_FCE_BITS_EVC_FCE_POS)
// #define DW1000_SUB_REG_EVC_FCE_BITS_EVC_FCE_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_FCE_BITS_EVC_FCE             DW1000_SUB_REG_EVC_FCE_BITS_EVC_FCE_MSK

#define DW1000_SUB_REG_EVC_FFR_BITS_EVC_FFR_POS         (0U)
#define DW1000_SUB_REG_EVC_FFR_BITS_EVC_FFR_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_FFR_BITS_EVC_FFR_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_FFR_BITS_EVC_FFR_POS)
// #define DW1000_SUB_REG_EVC_FFR_BITS_EVC_FFR_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_FFR_BITS_EVC_FFR             DW1000_SUB_REG_EVC_FFR_BITS_EVC_FFR_MSK

#define DW1000_SUB_REG_EVC_OVR_BITS_EVC_OVR_POS         (0U)
#define DW1000_SUB_REG_EVC_OVR_BITS_EVC_OVR_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_OVR_BITS_EVC_OVR_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_OVR_BITS_EVC_OVR_POS)
// #define DW1000_SUB_REG_EVC_OVR_BITS_EVC_OVR_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_OVR_BITS_EVC_OVR             DW1000_SUB_REG_EVC_OVR_BITS_EVC_OVR_MSK

#define DW1000_SUB_REG_EVC_STO_BITS_EVC_STO_POS         (0U)
#define DW1000_SUB_REG_EVC_STO_BITS_EVC_STO_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_STO_BITS_EVC_STO_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_STO_BITS_EVC_STO_POS)
// #define DW1000_SUB_REG_EVC_STO_BITS_EVC_STO_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_STO_BITS_EVC_STO             DW1000_SUB_REG_EVC_STO_BITS_EVC_STO_MSK

#define DW1000_SUB_REG_EVC_PTO_BITS_EVC_PTO_POS         (0U)
#define DW1000_SUB_REG_EVC_PTO_BITS_EVC_PTO_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_PTO_BITS_EVC_PTO_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_PTO_BITS_EVC_PTO_POS)
// #define DW1000_SUB_REG_EVC_PTO_BITS_EVC_PTO_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_PTO_BITS_EVC_PTO             DW1000_SUB_REG_EVC_PTO_BITS_EVC_PTO_MSK

#define DW1000_SUB_REG_EVC_FWTO_BITS_EVC_FWTO_POS       (0U)
#define DW1000_SUB_REG_EVC_FWTO_BITS_EVC_FWTO_LEN       (12U) /* 位 */
#define DW1000_SUB_REG_EVC_FWTO_BITS_EVC_FWTO_MSK       (0xFFFUL << DW1000_SUB_REG_EVC_FWTO_BITS_EVC_FWTO_POS)
// #define DW1000_SUB_REG_EVC_FWTO_BITS_EVC_FWTO_MSK        DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_FWTO_BITS_EVC_FWTO           DW1000_SUB_REG_EVC_FWTO_BITS_EVC_FWTO_MSK

#define DW1000_SUB_REG_EVC_TXFS_BITS_EVC_TXFS_POS       (0U)
#define DW1000_SUB_REG_EVC_TXFS_BITS_EVC_TXFS_LEN       (12U) /* 位 */
#define DW1000_SUB_REG_EVC_TXFS_BITS_EVC_TXFS_MSK       (0xFFFUL << DW1000_SUB_REG_EVC_TXFS_BITS_EVC_TXFS_POS)
// #define DW1000_SUB_REG_EVC_TXFS_BITS_EVC_TXFS_MSK        DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_TXFS_BITS_EVC_TXFS           DW1000_SUB_REG_EVC_TXFS_BITS_EVC_TXFS_MSK

#define DW1000_SUB_REG_EVC_HPW_BITS_EVC_HPW_POS         (0U)
#define DW1000_SUB_REG_EVC_HPW_BITS_EVC_HPW_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_HPW_BITS_EVC_HPW_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_HPW_BITS_EVC_HPW_POS)
// #define DW1000_SUB_REG_EVC_HPW_BITS_EVC_HPW_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_HPW_BITS_EVC_HPW             DW1000_SUB_REG_EVC_HPW_BITS_EVC_HPW_MSK

#define DW1000_SUB_REG_EVC_TPW_BITS_EVC_TPW_POS         (0U)
#define DW1000_SUB_REG_EVC_TPW_BITS_EVC_TPW_LEN         (12U) /* 位 */
#define DW1000_SUB_REG_EVC_TPW_BITS_EVC_TPW_MSK         (0xFFFUL << DW1000_SUB_REG_EVC_TPW_BITS_EVC_TPW_POS)
// #define DW1000_SUB_REG_EVC_TPW_BITS_EVC_TPW_MSK          DW1000_REG_BIT_RANGE(11, 0)
#define DW1000_SUB_REG_EVC_TPW_BITS_EVC_TPW             DW1000_SUB_REG_EVC_TPW_BITS_EVC_TPW_MSK

#define DW1000_SUB_REG_DIAG_TMC_BIT_TX_PSTM_POS         (4U)
#define DW1000_SUB_REG_DIAG_TMC_BIT_TX_PSTM_MSK         (0x1UL << DW1000_SUB_REG_DIAG_TMC_BIT_TX_PSTM_POS)
// #define DW1000_SUB_REG_DIAG_TMC_BIT_TX_PSTM_MSK          DW1000_REG_BIT(4)
#define DW1000_SUB_REG_DIAG_TMC_BIT_TX_PSTM             DW1000_SUB_REG_DIAG_TMC_BIT_TX_PSTM_MSK

/***********************************************************************************************
 * @brief 寄存器文件 0x30 - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */

/***********************************************************************************************
 * @brief 寄存器文件 0x31 - CRTR 【名字未知，用户手册未更新】
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_CRTR                                 0x31
/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_CRTR_GEAR_OFFSET                 0x28

/***********************************************************************************************
 * @brief 寄存器文件 0x32 到 0x35 - 保留
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */


/***********************************************************************************************
 * @brief 寄存器文件 0x36 - PMSC 电源管理系统控制块 [不定]
 * @note  备注
 * @attention  特别需要注意的地方进行说明
 * @warning  对函数的警告说明
 * @example  函数使用示例
 */
#define DW1000_REG_PMSC                                 0x36
#define DW1000_REG_PMSC_LEN                             (44) /* 字节 */

/*======================================= 子寄存器偏移地址 =====================================*/
#define DW1000_SUB_REG_PMSC_CTRL0_OFFSET                0x00
#define DW1000_SUB_REG_PMSC_CTRL0_LEN                   (4) /* 字节 */

#define DW1000_SUB_REG_PMSC_CTRL0_3_OFFSET              0x03

#define DW1000_SUB_REG_PMSC_CTRL1_OFFSET                0x04
#define DW1000_SUB_REG_PMSC_CTRL1_LEN                   (4) /* 字节 */

/* 偏移地址 0x08 保留 */

#define DW1000_SUB_REG_PMSC_SNOZT_OFFSET                0x0C
#define DW1000_SUB_REG_PMSC_SNOZT_LEN                   (1) /* 字节 */

/* 偏移地址 0x10 保留 */

#define DW1000_SUB_REG_PMSC_TXFSEQ_OFFSET               0x26
#define DW1000_SUB_REG_PMSC_TXFSEQ_LEN                  (2) /* 字节 */

#define DW1000_SUB_REG_PMSC_LEDC_OFFSET                 0x28
#define DW1000_SUB_REG_PMSC_LEDC_LEN                    (4) /* 字节 */

/*+++++++++++++++++++++++++++++++++++++++ 子寄存器位定义 +++++++++++++++++++++++++++++++++++++++*/

#define DW1000_SUB_REG_PMSC_CTRL0_3_BITS_SOFTRESET_POS  (4U)
#define DW1000_SUB_REG_PMSC_CTRL0_3_BITS_SOFTRESET_LEN  (4U) /* 位 */
#define DW1000_SUB_REG_PMSC_CTRL0_3_BITS_SOFTRESET_MSK  (0xFUL << DW1000_SUB_REG_PMSC_CTRL0_3_BITS_SOFTRESET_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_3_BITS_SOFTRESET_MSK   DW1000_REG_BIT_RANGE(7, 4)
#define DW1000_SUB_REG_PMSC_CTRL0_3_BITS_SOFTRESET      DW1000_SUB_REG_PMSC_CTRL0_3_BITS_SOFTRESET_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BITS_SOFTRESET_POS    (28U)
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_SOFTRESET_LEN    (4U) /* 位 */
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_SOFTRESET_MSK    (0xFUL << DW1000_SUB_REG_PMSC_CTRL0_BITS_SOFTRESET_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BITS_SOFTRESET_MSK     DW1000_REG_BIT_RANGE(31, 28)
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_SOFTRESET        DW1000_SUB_REG_PMSC_CTRL0_BITS_SOFTRESET_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_POS   (24U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_MSK   (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_MSK    DW1000_REG_BIT(24)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN       DW1000_SUB_REG_PMSC_CTRL0_BIT_PLL2_SEQ_EN_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BIT_KHZCLKEN_POS      (23U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_KHZCLKEN_MSK      (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_KHZCLKEN_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_KHZCLKEN_MSK       DW1000_REG_BIT(23)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_KHZCLKEN          DW1000_SUB_REG_PMSC_CTRL0_BIT_KHZCLKEN_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDRN_POS         (19U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDRN_MSK         (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDRN_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDRN_MSK          DW1000_REG_BIT(19)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDRN             DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDRN_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDCE_POS         (18U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDCE_MSK         (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDCE_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDCE_MSK          DW1000_REG_BIT(18)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDCE             DW1000_SUB_REG_PMSC_CTRL0_BIT_GPDCE_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN_POS          (17U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN_MSK          (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN_MSK           DW1000_REG_BIT(17)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN              DW1000_SUB_REG_PMSC_CTRL0_BIT_GPRN_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE_POS          (16U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE_MSK          (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE_MSK           DW1000_REG_BIT(16)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE              DW1000_SUB_REG_PMSC_CTRL0_BIT_GPCE_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE_POS          (15U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE_MSK          (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE_MSK           DW1000_REG_BIT(15)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE              DW1000_SUB_REG_PMSC_CTRL0_BIT_AMCE_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE_POS         (10U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE_MSK         (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE_MSK          DW1000_REG_BIT(10)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE             DW1000_SUB_REG_PMSC_CTRL0_BIT_ADCCE_MSK

/* 【该寄存器的位是根据官方的代码猜的，用户手册中并没有】 */
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE_POS        (9U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE_MSK        (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE_MSK          DW1000_REG_BIT(9)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE            DW1000_SUB_REG_PMSC_CTRL0_BIT_FOTPCE_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_POS          (6U)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_MSK          (0x1UL << DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_MSK           DW1000_REG_BIT(6)
#define DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE              DW1000_SUB_REG_PMSC_CTRL0_BIT_FACE_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_POS       (4U)
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_LEN       (2U) /* 位 */
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_MSK       (0x3UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_MSK        DW1000_REG_BIT_RANGE(5, 4)
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS           DW1000_SUB_REG_PMSC_CTRL0_BITS_TXCLKS_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_POS       (2U)
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_LEN       (2U) /* 位 */
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_MSK       (0x3UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_MSK        DW1000_REG_BIT_RANGE(3, 2)
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS           DW1000_SUB_REG_PMSC_CTRL0_BITS_RXCLKS_MSK

#define DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_POS      (0U)
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_LEN      (2U) /* 位 */
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_MSK      (0x3UL << DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_POS)
// #define DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_MSK       DW1000_REG_BIT_RANGE(1, 0)
#define DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS          DW1000_SUB_REG_PMSC_CTRL0_BITS_SYSCLKS_MSK

#define DW1000_SUB_REG_PMSC_CTRL1_BITS_KHZCLKDIV_POS    (26U)
#define DW1000_SUB_REG_PMSC_CTRL1_BITS_KHZCLKDIV_LEN    (6U) /* 位 */
#define DW1000_SUB_REG_PMSC_CTRL1_BITS_KHZCLKDIV_MSK    (0x3FUL << DW1000_SUB_REG_PMSC_CTRL1_BITS_KHZCLKDIV_POS)
// #define DW1000_SUB_REG_PMSC_CTRL1_BITS_KHZCLKDIV_MSK     DW1000_REG_BIT_RANGE(31, 26)
#define DW1000_SUB_REG_PMSC_CTRL1_BITS_KHZCLKDIV        DW1000_SUB_REG_PMSC_CTRL1_BITS_KHZCLKDIV_MSK

#define DW1000_SUB_REG_PMSC_CTRL1_BIT_LDERUNE_POS       (17U)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_LDERUNE_MSK       (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_LDERUNE_POS)
// #define DW1000_SUB_REG_PMSC_CTRL1_BIT_LDERUNE_MSK        DW1000_REG_BIT(17)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_LDERUNE           DW1000_SUB_REG_PMSC_CTRL1_BIT_LDERUNE_MSK

#define DW1000_SUB_REG_PMSC_CTRL1_BIT_PLLSYN_POS        (15U)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_PLLSYN_MSK        (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_PLLSYN_POS)
// #define DW1000_SUB_REG_PMSC_CTRL1_BIT_PLLSYN_MSK         DW1000_REG_BIT(15)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_PLLSYN            DW1000_SUB_REG_PMSC_CTRL1_BIT_PLLSYN_MSK

#define DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZR_POS         (14U)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZR_MSK         (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZR_POS)
// #define DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZR_MSK          DW1000_REG_BIT(14)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZR             DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZR_MSK

#define DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_POS         (13U)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_MSK         (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_POS)
// #define DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_MSK          DW1000_REG_BIT(13)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE             DW1000_SUB_REG_PMSC_CTRL1_BIT_SNOZE_MSK

#define DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_POS        (12U)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_MSK        (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_POS)
// #define DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_MSK         DW1000_REG_BIT(12)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP            DW1000_SUB_REG_PMSC_CTRL1_BIT_ARXSLP_MSK

#define DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP_POS        (11U)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP_MSK        (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP_POS)
// #define DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP_MSK         DW1000_REG_BIT(11)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP            DW1000_SUB_REG_PMSC_CTRL1_BIT_ATXSLP_MSK

#define DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_POS       (3U)
#define DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_LEN       (8U) /* 位 */
#define DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_MSK       (0xFFUL << DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_POS)
// #define DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_MSK        DW1000_REG_BIT_RANGE(10, 3)
#define DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ           DW1000_SUB_REG_PMSC_CTRL1_BITS_PKTSEQ_MSK

#define DW1000_SUB_REG_PMSC_CTRL1_BIT_ARX2INIT_POS      (1U)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_ARX2INIT_MSK      (0x1UL << DW1000_SUB_REG_PMSC_CTRL1_BIT_ARX2INIT_POS)
// #define DW1000_SUB_REG_PMSC_CTRL1_BIT_ARX2INIT_MSK       DW1000_REG_BIT(1)
#define DW1000_SUB_REG_PMSC_CTRL1_BIT_ARX2INIT          DW1000_SUB_REG_PMSC_CTRL1_BIT_ARX2INIT_MSK

#define DW1000_SUB_REG_PMSC_SNOZT_BITS_SNOZ_TIM_POS     (0U)
#define DW1000_SUB_REG_PMSC_SNOZT_BITS_SNOZ_TIM_LEN     (8U) /* 位 */
#define DW1000_SUB_REG_PMSC_SNOZT_BITS_SNOZ_TIM_MSK     (0xFFUL << DW1000_SUB_REG_PMSC_SNOZT_BITS_SNOZ_TIM_POS)
// #define DW1000_SUB_REG_PMSC_SNOZT_BITS_SNOZ_TIM_MSK      DW1000_REG_BIT_RANGE(7, 0)
#define DW1000_SUB_REG_PMSC_SNOZT_BITS_SNOZ_TIM         DW1000_SUB_REG_PMSC_SNOZT_BITS_SNOZ_TIM_MSK

#define DW1000_SUB_REG_PMSC_TXFSEQ_BITS_TXFINESEQ_POS   (0U)
#define DW1000_SUB_REG_PMSC_TXFSEQ_BITS_TXFINESEQ_LEN   (16U) /* 位 */
#define DW1000_SUB_REG_PMSC_TXFSEQ_BITS_TXFINESEQ_MSK   (0xFFFFUL << DW1000_SUB_REG_PMSC_TXFSEQ_BITS_TXFINESEQ_POS)
// #define DW1000_SUB_REG_PMSC_TXFSEQ_BITS_TXFINESEQ_MSK    DW1000_REG_BIT_RANGE(15, 0)
#define DW1000_SUB_REG_PMSC_TXFSEQ_BITS_TXFINESEQ       DW1000_SUB_REG_PMSC_TXFSEQ_BITS_TXFINESEQ_MSK

#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKNOW_POS       (16U)
#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKNOW_LEN       (4U) /* 位 */
#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKNOW_MSK       (0xFUL << DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKNOW_POS)
// #define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKNOW_MSK        DW1000_REG_BIT_RANGE(19, 16)
#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKNOW           DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKNOW_MSK

#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKEN_POS        (8U)
#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKEN_MSK        (0x1UL << DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKEN_POS)
// #define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKEN_MSK         DW1000_REG_BIT(8)
#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKEN            DW1000_SUB_REG_PMSC_LDEC_BITS_BLNKEN_MSK

#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNK_TIM_POS      (0U)
#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNK_TIM_LEN      (8U) /* 位 */
#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNK_TIM_MSK      (0xFFUL << DW1000_SUB_REG_PMSC_LDEC_BITS_BLNK_TIM_POS)
// #define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNK_TIM_MSK       DW1000_REG_BIT_RANGE(7, 0)
#define DW1000_SUB_REG_PMSC_LDEC_BITS_BLNK_TIM          DW1000_SUB_REG_PMSC_LDEC_BITS_BLNK_TIM_MSK


/*
寄存器文件 0x37 到 0x3F 均保留
*/


#endif
