/**
 * @file drv_dw1000.h
 * @author quote6 (HeXiHeNian@qq.com)
 * @brief
 * @version 0.1
 * @date 2026-07-15
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    DRV_DWX_ERR_OK              = 0,
    DRV_DWX_ERR_GENERAL         = 1,
    DRV_DWX_ERR_TIMEOUT         = 2,
    DRV_DWX_ERR_PARAM           = 3,
    DRV_DWX_ERR_NO_MEMORY       = 4,
    DRV_DWX_ERR_NO_RESOURCE     = 5,
    DRV_DWX_ERR_NOT_SUPPORTED   = 6,
    DRV_DWX_ERR_NOT_INITIALIZED = 7,
    DRV_DWX_ERR_ALREADY_INIT    = 8,
    DRV_DWX_ERR_BUSY            = 9,
    DRV_DWX_ERR_FAIL            = 10,
    DRV_DWX_ERR_RESERVED        = 0x7FFFFF
} drv_dwx_err_t;

typedef enum {
    DRV_DWX_RST_MODE_OD,
    DRV_DWX_RST_MODE_IT,
} drv_dwx_rst_mode_t;

/**
 * @brief
 *
 */
typedef enum {
    DRV_DWX_CHANNEL_1           = 0,
    DRV_DWX_CHANNEL_2           = 1,
    DRV_DWX_CHANNEL_3           = 2,
    DRV_DWX_CHANNEL_4           = 3,
    DRV_DWX_CHANNEL_5           = 4,
    DRV_DWX_CHANNEL_6           = 5, /**< DW1000不支持频道6 */
    DRV_DWX_CHANNEL_7           = 6,
    DRV_DWX_CHANNEL_SUPPORT_NUM = 6 /**< 支持频道数 */
} drv_dwx_chan_t;

/**
 * @brief
 *
 */
typedef enum {
    DRV_DWX_DATA_RATE_110K        = 0,
    DRV_DWX_DATA_RATE_850K        = 1,
    DRV_DWX_DATA_RATE_6M8         = 2,
    DRV_DWX_DATA_RATE_SUPPORT_NUM = 3 /**< 支持的数据速率数 */
} drv_dwx_data_rate_t;

/**
 * @brief
 *
 */
typedef enum {
    DRV_DWX_PAC_8           = 0,
    DRV_DWX_PAC_16          = 1,
    DRV_DWX_PAC_32          = 2,
    DRV_DWX_PAC_64          = 3,
    DRV_DWX_PAC_SUPPORT_NUM = 4,
} drv_dwx_pac_t;

typedef enum {
    DRV_DWX_PRF_4M          = 0, /**< DW1000不支持该频率 */
    DRV_DWX_PRF_16M         = 1,
    DRV_DWX_PRF_64M         = 2,
    DRV_DWX_PRF_SUPPORT_NUM = 2,
} drv_dwx_prf_t;

typedef enum {
    DRV_DWX_PLEN_4096 = 0x03U,
    DRV_DWX_PLEN_2048 = 0x0AU,
    DRV_DWX_PLEN_1536 = 0x06U,
    DRV_DWX_PLEN_1024 = 0x02U,
    DRV_DWX_PLEN_512  = 0x0DU,
    DRV_DWX_PLEN_256  = 0x09U,
    DRV_DWX_PLEN_128  = 0x05U,
    DRV_DWX_PLEN_64   = 0x01U,
    DRV_DWX_PLEN
} drv_dwx_plen_t;

typedef enum {
    DRV_DWX_PHR_TYPE_STD = 0x0U,
    DRV_DWX_PHR_TYPE_EXT = 0x1U,
} drv_dwx_phr_type_t;

typedef enum {
    DRV_DWX_RX_MODE_IMMEDIATED                    = 0x00,
    DRV_DWX_RX_MODE_DELAYED                       = 0x01,
    DRV_DWX_RX_MODE_IDLE_ON_DELAY_ERR             = 0x02,
    DRV_DWX_RX_MODE_NO_SYNC_POINTERS              = 0x04,
    DRV_DWX_RX_MODE_DELAYED_AND_IDLE_ON_DELAY_ERR = 0x03,
    DRV_DWX_RX_MODE_IMMEDIATE_AND_NO_SYNC_PTRS    = 0x04, /**< 【需要对检查该参数是否正确】 */
} drv_dwx_rx_mode_t;

typedef enum {
    DRV_DWX_SPI_SPEED_LOW,
    DRV_DWX_SPI_SPEED_HIGH,
} drv_dwx_spi_speed_t;

typedef struct {
    uint8_t* data;
    uint16_t size;
    uint16_t count;
} drv_dwx_array_t;


/**
 * @brief
 *
 */
typedef struct {
    /**
     * @brief 在发送完成后是否自动开启接收（通常用于接收应答信息）
     */
    uint8_t is_rx_on_after_tx : 1;
    /**
     * @brief 是否从OTP中加载partID
     */
    uint8_t should_load_part_id_from_otp : 1;
    /**
     * @brief 是否从OTP中加载LOT ID
     */
    uint8_t should_load_lot_id_from_otp : 1;
    /**
     * @brief 是否从OTP中加载参考电压值
     */
    uint8_t should_load_ref_volt_from_otp : 1;
    /**
     * @brief 是否从OTP中加载参考温度值
     */
    uint8_t should_load_ref_temp_from_otp : 1;
    /**
     * @brief 是否从OTP中加载微码
     */
    uint8_t should_load_ucode_from_otp : 1;
    /**
     * @brief 是否在唤醒后从OTP中加载微码
     */
    uint8_t should_load_ucode_from_otp_after_wakeup : 1;
    /**
     * @brief 是否在唤醒后从OTP中加载OTP修订号
     */
    uint8_t should_load_rev_from_otp_after_wakeup : 1;
    /**
     * @brief 是否使用非标 SFD（非标SFD可获得更好的性能）
     */
    uint8_t use_nstd_sfd : 1;
    uint8_t use_double_buff : 1;    /**< 是否为双缓冲模式 */
    uint8_t use_low_pow_listen : 1; /**< 是否使用低功耗监听 */
    /**
     * @brief 频道号
     * @see drv_dwx_chan_t
     */
    uint8_t channel : 3;
    /**
     * @brief 数据速率
     * @see drv_dwx_data_rate_t
     */
    uint8_t data_rate : 2;
    /**
     * @brief 脉冲重复频率
     * @see drv_dwx_prf_t
     */
    uint8_t prf : 2;
    /**
     * @brief PHR模式
     * @see drv_dwx_phr_type_t
     */
    uint8_t phrType : 1;
    uint8_t : 5; /**< 保留位 */
    /**
     * @brief 发送前导码长度
     * @see drv_dwx_plen_t
     */
    uint8_t tx_preamble_len;
    /**
     * @brief 前导码查询块大小（与接收前导码长度相关）
     */
    uint8_t rx_pac;
    /**
     * @brief 发送前导码
     */
    uint8_t tx_preamble_code;
    /**
     * @brief 接收前导码
     */
    uint8_t rx_preamble_code;
    /**
     * @brief SFD超时值（单位 符号）
     *
     * SFD超时 = (前导码长度 + 1 + SFD长度 - PAC大小)
     */
    uint16_t sfd_timeout;
    /**
     * @brief 接收超时时间（在超时时间内没有接收到帧），设置为0表示禁用超时
     */
    uint16_t rx_timeout;
    /**
     * @brief 发送帧完后开启接收器的延迟时间（最短6.2us，即使设置为0）
     */
    uint32_t rx_delay_time;
} drv_dwx_cfg_t;

typedef struct {

} drv_dwx_data_t;

typedef struct {
    /* data */
} drv_dwx_ctx_t;


typedef struct {
    struct {
        struct {
            void (*write)(bool);
            bool (*read)(void);
        } nss;
        // struct {
        //     drv_dwx_err_t (*read)(uint8_t* const data,
        //                           uint16_t       size);
        // } dma;
        drv_dwx_err_t (*init)(void);
        drv_dwx_err_t (*deinit)(void);
        drv_dwx_err_t (*set_speed)(drv_dwx_spi_speed_t speed);
        drv_dwx_err_t (*write)(const uint8_t* const data,
                               uint16_t             size);
        drv_dwx_err_t (*read)(uint8_t* const data, uint16_t size);
    } spi;
    struct {
        drv_dwx_err_t (*init)(void);
        drv_dwx_err_t (*deinit)(void);
        void (*enable)(void);
        void (*disable)(void);
    } irq;
    struct {
        drv_dwx_err_t (*init)(drv_dwx_rst_mode_t mode);
        drv_dwx_err_t (*deinit)(void);
        void (*write)(bool);
        bool (*read)(void);
    } rst;
    struct {
        drv_dwx_err_t (*init)(void);
        drv_dwx_err_t (*deinit)(void);
        void (*write)(bool);
        bool (*read)(void);
    } wakeup;
    void (*delay_ms)(uint32_t ms);
    void (*delay_us)(uint32_t us);
    void (*debug_print)(const char* fmt, ...);
    uint32_t (*get_tick)(void);
} drv_dwx_ops_t;

typedef struct {
    const drv_dwx_cfg_t* const  cfg;
    /* private */ drv_dwx_ctx_t ctx;
    /* public */ drv_dwx_data_t data;
    const drv_dwx_ops_t* const  ops;
} drv_dwx_t;

drv_dwx_err_t drv_dwx_ctor(drv_dwx_t* const           self,
                           const drv_dwx_ops_t* const ops);

drv_dwx_err_t drv_dwx_dtor(drv_dwx_t* self);

drv_dwx_err_t drv_dwx_reset(drv_dwx_t* const self);

drv_dwx_err_t drv_dwx_init(drv_dwx_t* const self,
                           uint8_t          mode);