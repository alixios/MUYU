#ifndef __NOVEL_H
#define __NOVEL_H

#include "font.h"

/* 木鱼功能 */
void handle_muyu();

/* 读取txt文件 */
int show_novel();

/* 小说功能 */
int handle_novel();

/* 初始化存档 */
void archive_init();

/* 画板显示时间 */
void show_time();

/* 画板功德时间 */
void show_beilf();

/* 获得时间 */
char * get_local_time();

/* 显示文件 */
void show_font_str( char * src, int width, int height);

/* 显示文本 */
void display_text(font *f,  char *text) ;

/* 读取文件  */
char *read_text_from_file(const char *filename);

/* 存档， 释放lcd ,map */
void archive_exit();

#endif
