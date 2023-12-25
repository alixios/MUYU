#include <stdio.h>
#include <stdlib.h>

#include "my_type.h"

/**
 *初始化双向链表 
 */ 
DoubleLinkList double_list_init( data_t * data ){
    DoubleLinkList node = malloc(sizeof(ListNode));

    if( node != NULL ){
        node->next = node->pre = node;
    }

    if( data != NULL ){
        node->data = *data;
    }

    return  node;
}

/**
 * 双向链表插入节点
*/
void list_add_pre( DoubleLinkList before, DoubleLinkList new ){
    new->next = before->next;
    new->pre = before;
    before->next = new;
    new->next->pre = new;
}
/**
 * 双向链表插入节点
 * -> -> <- <-
*/
void list_add( DoubleLinkList prev, DoubleLinkList new , DoubleLinkList next){
    prev->next = new;
    new->next  = next;
    next->pre = new;
    new->pre = prev;
}

/* 有序插入, 自定义排序顺序 */
void list_add_order_defined(DoubleLinkList head, DoubleLinkList node, compare_node_funptr compare){
    DoubleLinkList tmp = head->next;

    for (tmp = head->next; tmp != head && compare(tmp, node); tmp = tmp->next);

    list_add(tmp->pre, node, tmp);
}

/**
 * 头插
*/
void list_add_first(DoubleLinkList head ,DoubleLinkList new){
    list_add_pre(head, new);
}

data_t  list_get_first(DoubleLinkList head){
    return head->next->data;
}
/**
 * size():返回链表元素个数
*/
int list_size(DoubleLinkList head){
    int size = 0;
    DoubleLinkList p = head;
    while ( p->next != head){
        p = p->next;
        size++;
    }
    return size;
}

/**
 * 输出双向链表
*/
void list_print(DoubleLinkList head){
    DoubleLinkList p = head->next;

    printf("[");
    while(p != head){     
        if( p->next == head){
            printf("%s", p->data.str);
        }else{
            printf("%s, ", p->data.str);
        }        
        p = p->next;
    }
    printf("]\n");
}

/**
 * 销毁双向链表
*/
void destroy(DoubleLinkList head){
    DoubleLinkList next, current = head->next;

    while( current != head ){
        next = current->next;
        free(current);
        current = next;
    }

    free(head);
}


