/**
 * ************************************************************************
 * 
 * @file QueueRM.h
 * @author quote6 (2293127401@qq.com)
 * @brief 本文件实现的静态队列，队列的最大长度由程序指定，并保持不变 \n
 *        最优的办法是实现动态创建，使用链表实现 \n
 *        队列取名为 RM 是为了防止命名重复而取的名字 Queue_RM \n
 *        为了方便进行队列相关的运算，设定队列的长度为 2 的幂次 
 * @date 2025-6-13
 * @note 2025-6-13: 修改 Queue_RM_DeInit 和 Queue_RM_Clear 声明 \n
 *                  添加返回值类型 uint8_t
 * ************************************************************************
 * @copyright Copyright (c) 2025 quote6 
 * For study and research only, no reprinting
 * ************************************************************************
 */
#ifndef __QUEUE_RM_H
#define __QUEUE_RM_H

#include <stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

/* 定义队列函数参数检查开关，需要时可以解除注释开启，默认关闭 */
// #define QUEUE_RM_PARAMETER_CHECK

/* 宏函数 位数转换对应的进制数 */
#define bitToNum(x)            (1 << x)

/**
 * @brief 队列类型结构体
 * @param mask 掩码，用于 & 运算, 相较于 % 运算开支少, 同时 mask 的值表示队列的实际最大值
 * @param head 队头指针 该指针指向的位置为当前要出队列的元素
 * @param tail 队尾指针 该指针指向的位置为当前要入队的位置
 * @param buff 队列缓冲区指针，保存的数据存储在此地址，缓冲区的大小事先指定，大小不变
 * @note 队列的缓冲区的所占的内存长度为 mask + 1, 因为队列需要空一个单元用于进行队列满判断
 * @attention 队列的长度不要超过 16位宽, 即 mask < 2^16
 */
typedef struct QueueRM_s
{
    uint16_t mask;        // 掩码，用于 & 运算
    uint16_t head;        // 头指针 出队列指针
    uint16_t tail;        // 尾指针 入队列指针
    uint16_t elementSize; // 队列中单个元素大小
    uint8_t* buff;        // 缓冲区
} QueueRM_t;



/* 定义队列函数执行的相关状态 */
//队列当前状态为空
#define QUEUE_RM_STATUS_EMPTY        0
//队列当前状态为满
#define QUEUE_RM_STATUS_FULL         1
//队列当前状态为有空余
#define QUEUE_RM_STATUS_SPARE        2
//队列状态成功
#define QUEUE_RM_STATUS_SUCCESS      3
//队列状态失败
#define QUEUE_RM_STATUS_FAILURE      4
//队列状态 任务未完全执行
#define QUEUE_RM_STATUS_INCOMPLETE   5
//队列状态 错误
#define QUEUE_RM_STATUS_ERROR        6


/* 供外部使用的函数 */

//因为是静态创建队列，这两个函数使用不上
// Queue_RM_t* Queue_RM_Create(void);
// void Queue_RM_Free(Queue_RM_t* queue);

/**
 * @brief 创建静态队列宏函数
 * @param[in] name: 队列名称
 * @param[in] width: 队列宽度（不能超过16）
 * @param[in] eSize: 队列中单个元素尺寸
 */
//
#define QUEUE_RM_CREATE_STATIC_QUEUE(name, width, eSize) \
    uint8_t QueueRM_Buff_##name[((uint16_t)bitToNum(width))*(eSize)];\
    QueueRM_t QueueRM_##name = {\
        .mask = (uint16_t) (bitToNum(width) - 1),\
        .head = 0,\
        .tail = 0,\
        .buff =QueueRM_Buff_##name,\
        .elementSize = eSize \
    };
#define QUEUE_RM_GET_STATIC_QUEUE_NAME(name) QueueRM_##name

uint8_t QueueRM_Init(QueueRM_t* const queue, uint8_t const queueWidth, uint8_t* buff, uint8_t const elementSize); // 队列初始化函数
uint8_t QueueRM_DeInit(QueueRM_t* const queue);                                                                   // 队列默认初始化函数
uint8_t QueueRM_Clear(QueueRM_t* const queue);                                                                    // 队列清空函数

uint16_t QueueRM_GetLength(QueueRM_t* const queue); // 获取队列当前实际长度函数

uint8_t QueueRM_PutIn(QueueRM_t* const queue, const void* const data);                                                               // 队列插入元素函数
uint8_t QueueRM_TakeOut(QueueRM_t* const queue, void* const data);                                                                   // 队列取出元素函数
uint8_t QueueRM_PutArrayIn(QueueRM_t* const queue, const void* const source, uint16_t lengthTarget, uint16_t* const lengthActual);   // 队列插入数组函数
uint8_t QueueRM_TakeArrayOut(QueueRM_t* const queue, void* const destination, uint16_t lengthTarget, uint16_t* const lengthActual);  // 队列取出数组函数
uint8_t QueueRM_TransferData(QueueRM_t* const queueD, QueueRM_t* const queueS, uint16_t lengthTarget, uint16_t* const lengthActual); // 将队列中的数据搬运到另一个队列

uint8_t QueueRM_Discard(QueueRM_t* const queue, uint16_t length, uint16_t* const lengthActual); // 队列丢弃若干数据函数
uint8_t QueueRM_Overwrite(QueueRM_t* const queue, const void* const data);                      // 队列覆写函数

uint8_t QueueRM_ViewHead(const QueueRM_t* const queue, void* const data);                                                         // 查看队头元素，不改变队头元素的值
uint8_t QueueRM_ViewTail(const QueueRM_t* const queue, void* const data);                                                         // 查看队尾元素，不改变队尾元素的值
uint8_t QueueRM_Peep(const QueueRM_t* const queue, void* const destination, uint16_t lengthTarget, uint16_t* const lengthActual); // 偷看队列，从队列中取值，但不会改变队头指针的值

#endif

/*
 队列，是一种先进先出（first in first out 简称FIFO）的数据结构，
 是只允许在一端进行插入数据操作，在另一端进行删除数据操作的特殊线性表，
 在队列中，进行插入的一端称为队尾 (tail)，进行删除的一端称为队头 (head)。

 当尾指针和头指针相等时，表示此时队列空
 当队尾指针的下一个是队头指针时，表示此时队列已满
 入队操作过程：先将队尾指针指向的位置放入要插入的元素，然后队尾指针增 1
 出队操作过程：先将队头指针指向的位置的元素提取出来，然后队头指针增 1
*/

