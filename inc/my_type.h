#ifndef __MYTYPE_H
#define __MYTYPE_H

#include <stdlib.h>

/* 主界面枚举类型 */
enum event{
    PICTURE,
    MUYU,
    NOVEL,
    GAME,
    EXIT = 5
};

/* 存档结构体 */
typedef struct archive {
    int32_t novel_page;      /* 小说页码 */
    int32_t beilf_count;     /* 功德记录 */
                             /* 棋盘数组 */
}Archive;

/* 数据域 */
typedef struct data_type{
    char str[256];
}data_t;

/* 链表 */
typedef struct node{
    data_t data;
    struct node * pre;
    struct node * next;
}ListNode,* DoubleLinkList;

/* 自定义比较函数 */
typedef int (*compare_node_funptr) (const void  *, const void *);

#endif
