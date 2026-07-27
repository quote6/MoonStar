#include "rm_linked_list.h"

void rm_node_deinit(RmNode_t* node) {
    if (node == NULL) {
        return;
    } else {
        node->prev = node;
        node->next = node;
        node->data = NULL;
        node->size = 0;
    }
}

void rm_linked_list_node_append(RmNode_t* head, RmNode_t* node) {
    node->next = head;
    node->prev = head->prev;
    head->prev->next = node;
    head->prev = node;
}

void rm_linked_list_node_prepend(RmNode_t* head, RmNode_t* node) {
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}

void rm_linked_list_node_insert(RmNode_t* pos, RmNode_t* node) {
    node->next = pos->next;
    node->prev = pos;
    pos->next->prev = node;
    pos->next = node;
}

/**
 * @brief 删除链表尾部元素
 *
 * @param head
 */
void rm_linked_list_node_delete(RmNode_t* head) {
    if (head == NULL || head->next == head) {
        return;
    } else {
        RmNode_t* node = head->prev;
        node->prev->next = head;
        head->prev = node->prev;
    }
}

/**
 * @brief 删除环形列表头部元素
 *
 * @param head
 * @param node
 */
void rm_linked_list_node_shift(RmNode_t* head, RmNode_t* node) {
    if (head == NULL || head->next == head) {
        return;
    } else {
        RmNode_t* node = head->next;
        head->next = node->next;
        node->next->prev = head;
    }
}

/**
 * @brief 删除链表指定指定元素
 *
 * @param node
 */
void rm_linked_list_node_remove(RmNode_t* node) {
    if (node == NULL) {
        return;
    } else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
}

/**
 * @brief
 *
 * @param head
 * @param data
 */
RmNode_t* rm_linked_list_node_find(RmNode_t* head, uint8_t* data) {
    RmNode_t* node = head->next;
    while (node != head){
        if (node->data == data){
            return node;
        }
        node = node->next;
    }
    return NULL;
}