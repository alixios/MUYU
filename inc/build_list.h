#ifndef     __BULIDLIST_H
#define     __BULIDLIST_H

#define  IS_DIR   4   
#define  IS_REG   8   

#include "my_type.h"

/* 扫描指定路径下所以指定格式文件生成链表 */
int build_list_dirpath(const char * pathname, char * type, DoubleLinkList head);

int build_list_pic(const char * pathname, DoubleLinkList head);
#endif