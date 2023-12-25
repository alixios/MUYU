/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @Description  : 鱼乐终端
 * @version      : 1.0.0
 * @Author       : alan
 * @Date         : 2023-12-20 11:41:19
 * @copyright (c), 2023 by alan, All Rights Reserved.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdio.h>
#include <stdbool.h>
#include "my_type.h"
#include "lcd.h"
#include "screen.h"
#include "jpg2rgb.h"
#include "novel.h"
#include "game.h"

int main(int argc, char const *argv[])
{
    lcd_init();        /* 初始化显示器 */

    screen_init();     /* 初始化触摸屏 */

    archive_init();    /* 读取存档文件，初始化数据 */

    pic_animation("/alan/assets/open"); /* 开机动画 */

    int flag = 1;
    while (flag) {
        /* 初始化主界面 */
        showjpg(0, 0, "/alan/assets/image/theme.jpg");

        /* 显示时间功德 */
        show_time();
        show_beilf();
        
        /* 用户点击触摸屏，产生单击事件，根据单击的作用范围，返回4种事件 */       
        int userInput = getTouchInput();

        switch (userInput) {
            case PICTURE:/* 进行相册 */
                handle_picture("/alan/assets/Screenshots");
                break;
            case MUYU:   /* 进行木鱼 */
                handle_muyu();              
                break;
            case NOVEL:  /* 进行小说 */
                show_novel();              
                break;
            case GAME:   /* 进行五子棋游戏 */
                play_gobang_game();         
                break;
            case EXIT:   /* 终止程序 */
                flag = 0;
                break;
            default:
                break;
        }

    }

    /* 关机动画 */
    pic_animation("/alan/assets/down");

    /* 释放资源存档写入 */
    archive_exit();
    
    return 0;
}
