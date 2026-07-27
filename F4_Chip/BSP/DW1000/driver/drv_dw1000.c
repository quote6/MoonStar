/**
 * @file drv_dw1000.c
 * @author quote6 (HeXiHeNian@qq.com)
 * @brief
 * @version 0.1
 * @date 2026-07-24
 *
 * @copyright Copyright (c) 2026
 *
 */
#include "drv_dw1000.h"

#ifndef NULL
#define NULL ((void*) 0)
#endif

/* 给寄存器地址添加读/写命令 */
// #define DW1000_SPI_CMD_READ(addr)  DW1000_CLEAR_BITS(addr, DW1000_SPI_CMD_MASK)
#define DW1000_SPI_CMD_READ(addr)       (addr & 0x7F)
// #define DW1000_SPI_CMD_WRITE(addr) DW1000_SET_BITS(addr, DW1000_SPI_CMD_MASK)
#define DW1000_SPI_CMD_WRITE(addr)      (addr | 0x80)

/* 给寄存器地址添加是否存在子地址标志 */
// #define DW1000_SPI_NOT_SUB_ADDR(addr) DW1000_CLEAR_BITS(addr, DW1000_SPI_SUB_ADDR_MASK)
#define DW1000_SPI_NOT_SUB_ADDR(addr)   (addr & 0xBF)
// #define DW1000_SPI_HAS_SUB_ADDR(addr) DW1000_SET_BITS(addr, DW1000_SPI_SUB_ADDR_MASK)
#define DW1000_SPI_HAS_SUB_ADDR(addr)   (addr | 0x40)


/* 给子地址添加是否为扩展子地址标志 */
// #define DW1000_SPI_NOT_EXT_SUB_ADDR(sa) DW1000_CLEAR_BITS(sa, DW1000_SPI_EXT_SUB_ADDR_MASK)
#define DW1000_SPI_NOT_EXT_SUB_ADDR(sa) (sa & 0x7F)
// #define DW1000_SPI_HAS_EXT_SUB_ADDR(sa) DW1000_SET_BITS(sa, DW1000_SPI_EXT_SUB_ADDR_MASK)
#define DW1000_SPI_HAS_EXT_SUB_ADDR(sa) (sa | 0x80)


static drv_dwx_err_t _dw1000_spi_read(drv_dwx_t* const self,
                                     uint8_t          reg,
                                     uint16_t         offset,
                                     uint8_t* const   data,
                                     uint16_t         len) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(reg <= 0x3F);
    DW1000_ASSERT_PARAM(self != NULL);
    DW1000_ASSERT_PARAM(self->ops->spi.read != NULL);
    DW1000_ASSERT_PARAM(self->ops->spi.write != NULL);
    DW1000_ASSERT_PARAM(self->ops->irq.disable != NULL);
    DW1000_ASSERT_PARAM(self->ops->irq.enable != NULL);
    DW1000_ASSERT_PARAM(self->ops->spi.nss.write != NULL);
    DW1000_ASSERT_PARAM(data != NULL);
    DW1000_ASSERT_PARAM(len != 0);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 定义头部数组 */
    uint8_t       header[3];
    uint8_t       len_header;
    drv_dwx_err_t ret = DRV_DWX_ERR_OK;
    /* 如果偏移地址为0，即只存在寄存器地址，头部长度为1 */
    if (0 == offset) {
        header[0]  = reg;
        len_header = 1;
    }
    /* 如果偏移不超过127可用7bit表示，则不需要扩展子地址 */
    else if (0x7F >= offset) {
        header[0]  = DW1000_SPI_HAS_SUB_ADDR(reg);
        header[1]  = offset;
        len_header = 2;
    }
    /* 本次spi通信为读操作且有子地址与扩展子地址 */
    else {
        header[0]  = DW1000_SPI_HAS_SUB_ADDR(reg);
        header[1]  = DW1000_SPI_HAS_EXT_SUB_ADDR(offset);
        header[2]  = (offset >> 7);
        len_header = 3;
    }
    /* 对SPI通信进行保护（关闭DW1000相关中断） */
    self->ops->irq.disable();
    /* 拉低spi片选线 */
    self->ops->spi.nss.write(0);
    /* 先发送头部数据 */
    ret = self->ops->spi.write(header, len_header);
    if (DRV_DWX_ERR_OK != ret) {
        goto err_spi;
    }
    /* 开始读取数据 */
    ret = self->ops->spi.read(data, len);
    if (DRV_DWX_ERR_OK != ret) {
        goto err_spi;
    }
err_spi:
    /* 拉高spi片选 */
    self->ops->spi.nss.write(1);
    /* 接收spi通信保护，恢复中断 */
    self->ops->irq.enable();
    return ret;
}

static drv_dwx_err_t _dw1000_spi_write(drv_dwx_t* const     self,
                                       uint8_t              reg,
                                       uint16_t             offset,
                                       const uint8_t* const data,
                                       uint16_t             len) {
#if (USE_DW1000_FULL_ASSERT == 1U)
    DW1000_ASSERT_PARAM(reg <= 0x3F);
    DW1000_ASSERT_PARAM(self != NULL);
    DW1000_ASSERT_PARAM(self->ops->spi.write != NULL);
    DW1000_ASSERT_PARAM(self->ops->irq.disable != NULL);
    DW1000_ASSERT_PARAM(self->ops->irq.enable != NULL);
    DW1000_ASSERT_PARAM(self->ops->spi.nss.write != NULL);
    DW1000_ASSERT_PARAM(data != NULL);
    DW1000_ASSERT_PARAM(len != 0);
#endif /* USE_DW1000_FULL_ASSERT */
    /* 定义头部数组 */
    uint8_t       header[3];
    uint8_t       len_header;
    drv_dwx_err_t ret = DRV_DWX_ERR_OK;
    /* 如果偏移为0，即只有寄存器地址，头部字段长度为1字节 */
    if (offset == 0) {
        /* 设置本次spi为写操作且无子地址 */
        header[0]  = DW1000_SPI_CMD_WRITE(reg);
        len_header = 1;
    }
    /* 如果偏移不超过127，也就是用7bit可以表示时，说明不需要扩展子地址 */
    else if (offset <= 0x7F) {
        /* 设置本次spi为写操作且有子地址，但无扩展子地址 */
        header[0]  = DW1000_SPI_HAS_SUB_ADDR(DW1000_SPI_CMD_WRITE(reg));
        header[1]  = offset;
        len_header = 2;
    } else {
        /* 设置本次spi为写操作且有子地址，且有扩展子地址 */
        header[0]  = DW1000_SPI_HAS_SUB_ADDR(DW1000_SPI_CMD_WRITE(reg));
        header[1]  = DW1000_SPI_HAS_EXT_SUB_ADDR(offset);
        header[2]  = (offset >> 7);
        len_header = 3;
    }
    /* 对SPI通信进行保护（关闭DW1000相关中断） */
    self->ops->irq.disable();
    /* 拉低spi片选线 */
    self->ops->spi.nss.write(0);
    /* 先发送头部数据 */
    ret = self->ops->spi.write(header, len_header);
    if (DRV_DWX_ERR_OK != ret) {
        goto err_spi;
    }
    /* 开始读取数据 */
    ret = self->ops->spi.write(data, len);
    if (DRV_DWX_ERR_OK != ret) {
        goto err_spi;
    }
err_spi:
    /* 拉高spi片选 */
    self->ops->spi.nss.write(1);
    /* 接收spi通信保护，恢复中断 */
    self->ops->irq.enable();
    return ret;
}

drv_dwx_err_t drv_dwx_reset(drv_dwx_t* const self) {
    if (NULL == self ||
        NULL == self->ops->delay_ms ||
        NULL == self->ops->delay_us ||
        NULL == self->ops->rst.init) {
        return DRV_DWX_ERR_PARAM;
    }
    drv_dwx_err_t ret = DRV_DWX_ERR_OK;
    /* RST引脚去初始化 */
    ret = self->ops->rst.deinit();
    if (DRV_DWX_ERR_OK != ret) {
        return ret;
    }
    /* 配置RST引脚为开漏输出 */
    ret = self->ops->rst.init(DRV_DWX_RST_MODE_OD);
    if (DRV_DWX_ERR_OK != ret) {
        return ret;
    }
    /* RST引脚输出低电平 */
    self->ops->rst.write(0);
    self->ops->delay_us(1);
    /* RST引脚输出高电平 */
    self->ops->rst.write(1);
    self->ops->delay_ms(2);
    return ret;
}

drv_dwx_err_t drv_dwx_read_dev_id(drv_dwx_t* const self,
                                  uint32_t* const  id) {
    if (NULL == self || NULL == id ||
        NULL == self->ops->spi.write ||
        NULL == self->ops->spi.read) {
        return DRV_DWX_ERR_PARAM;
    }
    return _dw1000_spi_read(self,);
}

drv_dwx_err_t drv_dwx_init(drv_dwx_t* const self,
                           uint8_t          mode) {
    if (NULL == self) {
        return DRV_DWX_ERR_PARAM;
    }
    /** @todo */
}