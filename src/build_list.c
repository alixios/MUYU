#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "my_type.h"
#include "build_list.h"
#include "list.h"

/* 提取数字转换为整数 */
int extract_number(const char *str) {
    int num = 0;
    const char *start = str; // 指向数字的起始位置
    const char *end = str;   // 指向数字的结束位置

    // 找到第一个数字字符
    while (*start && !isdigit(*start)) {
        ++start;
    }

    // 找到最后一个数字字符
    end = start;
    while (*end && isdigit(*end)) {
        ++end;
    }

    // 提取数字
    char number[end - start + 1];
    strncpy(number, start, end - start);
    number[end - start] = '\0';

    // 将字符串转换为整数
    num = atoi(number);
    return num;
}

/* 自定义比较函数 */ 
int compare_frame(const void * tmp, const void * node) {

    // 获取文件名中的数字部分
    int num_a = extract_number(((DoubleLinkList)tmp )->data.str);
    int num_b = extract_number(((DoubleLinkList)node)->data.str);

    // 比较数字并返回结果
    return num_a < num_b;
}

/** 
 * @brief: 读取指定路径下所以指定格式的文件并生成链表
 * @param {char *} dir_path
 * @param {char *} type
 * @param {LinkList} head
 * @return {*}
 */
int build_list_dirpath(const char * pathname, char * type, DoubleLinkList head)
{
    printf("%s\n",pathname);

    /* 打开目录 */
    DIR * directory = opendir(pathname);
    if(directory == NULL){
        perror("open directory error:");
        return -1;
    }

    /* 读取目录项 */
    char child_path_name[512] = {0};
    while(1){
        struct dirent * dirent = readdir(directory);

        if(dirent == NULL){
            perror("readdir error:");
            break;
        }

        if(dirent->d_type == IS_REG ){
    
            char * types = strrchr(dirent->d_name, '.');
            if (types == NULL) {// 如果找不到则表示给文件没有设置后缀名（忽略它）                
                continue;
            }
            
            if ( strcmp(types, type) == 0) {              
                data_t filename = {0};

                //snprintf(filename.str, 255, "%s%s", pathname, dirent->d_name);
                strcpy(filename.str, dirent->d_name);
                DoubleLinkList new_node = double_list_init(&filename);
                list_add_order_defined(head, new_node, compare_frame);
            }
        }
    }
    return 0;
}

/** 
 * @brief: 读取指定路径下所以图片文件并生成链表
 * @param {char *} dir_path
 * @param {char *} type
 * @param {LinkList} head
 * @return {*}
 */
int build_list_pic(const char * pathname, DoubleLinkList head)
{
    printf("%s\n",pathname);

    /* 打开目录 */
    DIR * directory = opendir(pathname);
    if(directory == NULL){
        perror("open directory error:");
        return -1;
    }

    /* 读取目录项 */
    char child_path_name[512] = {0};
    while(1){
        struct dirent * dirent = readdir(directory);

        if(dirent == NULL){
            perror("readdir error:");
            break;
        }

        if(dirent->d_type == IS_REG ){
    
            char * types = strrchr(dirent->d_name, '.');
            if (types == NULL) {// 如果找不到则表示给文件没有设置后缀名（忽略它）                
                continue;
            }
            
            if(!strcmp(types, ".jpg")||!strcmp(types, ".jepg")||!strcmp(types, ".bmp")) {              
                data_t filename = {0};

                snprintf(filename.str, 255, "%s/%s", pathname, dirent->d_name);
        
                DoubleLinkList new_node = double_list_init(&filename);

                list_add_order_defined(head, new_node, compare_frame);
            }
        }
    }
    return 0;
}