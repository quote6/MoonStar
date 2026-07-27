/**
 * @file rm_linked_list.h
 * @author quote6 (2293127401@qq.com)
 * @brief 带哨兵位的双向环形链表
 *         参考链接：[链表进化论：C语言实现带哨兵位的双向循环链表，解锁O(1)删除的奥秘](https://blog.csdn.net/2501_93679849/article/details/155454023?fromshare=blogdetail&sharetype=blogdetail&sharerId=155454023&sharerefer=PC&sharesource=qq_37278781&sharefrom=from_link)
 * @version 0.1
 * @date 2026-04-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef __RM_LINKED_LIST_H
#define __RM_LINKED_LIST_h

#include <stdint.h>

#ifndef NULL
#define NULL ((void*) 0)
#endif

typedef struct RmNode_s {
    RmNode_t* prev; /*< 指向上一个节点 */
    RmNode_t* next; /*< 指向下一个节点 */
    uint8_t* data;  /*< 使用指针，赋值更灵活 */
    uint8_t size;   /*< 存储的数据大小*/
} RmNode_t;

void rm_node_deinit(RmNode_t* node);
void rm_linked_list_node_append(RmNode_t* head, RmNode_t* node);
void rm_linked_list_node_prepend(RmNode_t* head,RmNode_t* node);
void rm_linked_list_node_insert(RmNode_t* pos, RmNode_t* node);
void rm_linked_list_node_delete(RmNode_t* head);
void rm_linked_list_node_shift(RmNode_t* head, RmNode_t* node);
void rm_linked_list_node_remove(RmNode_t* node);
RmNode_t* rm_linked_list_node_find(RmNode_t* head,uint8_t* data);
#endif
