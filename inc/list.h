#ifndef     __LIST_H
#define     __LIST_H

#include <stdbool.h>
#include "my_type.h"

/* 初始化双向链表  */ 
DoubleLinkList double_list_init( data_t * data );

/* 双向链表插入节点 */
void list_add_pre( DoubleLinkList before, DoubleLinkList new);

/* 双向链表插入节点 */
void list_add( DoubleLinkList prev, DoubleLinkList new , DoubleLinkList next);

/* 有序插入, 自定义排序顺序 */
void list_add_order_defined(DoubleLinkList head, DoubleLinkList node, compare_node_funptr compare);

/* 头插 */
void list_add_first( DoubleLinkList head , DoubleLinkList new);

/* 输出双向链表 */
void list_print( DoubleLinkList head);

/* 销毁双向链表 */
void destroy(DoubleLinkList head);

/* size():返回链表元素个数*/
int list_size(DoubleLinkList head);

/* 获取链表头数据 */
data_t  list_get_first( DoubleLinkList head);

#endif