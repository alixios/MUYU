#ifndef __SCREEN_H
#define __SCREEN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define DOUBLE_CLICK_THRESHOLD 500 // 规定的双击间隔时间，单位为毫秒

#define NXET_CLICK   0   /* next单击事件 */

#define PREV_CLICK   1   /* prev单击事件 */

#define DOUBLE_CLICK 2   /* 双击事件 */

#define LEFT_SLIDE   3   /* 左滑动事件 */

#define RIGHT_SLIDE  4   /* 右滑动事件 */

#define UP_SLIDE     5   /* 主界面事件 */

#define DOWN_SLIDE   6   /* 主界面事件 */

#define ERROR        7   /* 出错 */

/* 初始化lcd */
void screen_init();

/* 主界面单击事件 */
int getTouchInput();

/* 按下单击事件，返回坐标 */
int getChessInput(int *x, int *y);

/* 应用单击事件 */
int screen_event();

/* 获得时间HH：MM */
unsigned long get_current_time();

#endif