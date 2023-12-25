#include <time.h>
#include "screen.h"


int fd_ts;
/*
type:
    #define EV_SYN			0x00  分割
    #define EV_KEY			0x01  键盘事件
    #define EV_REL			0x02
    #define EV_ABS			0x03  触摸屏事件
code:

*/

/* 初始化触摸屏 */
void screen_init(){
    fd_ts = open("/dev/input/event0", O_RDWR);
    if(fd_ts == -1){
        perror("open error:");
        return ;
    }
}


int __event(int x_in, int y_in){
    printf("__event坐标:(%d, %d)\n", x_in,y_in);
    if(x_in >= 5 && x_in <= 173 && y_in >= 240 && y_in <= 480){
        return 3;
    }else if(x_in >= 174 && x_in <= 346 && y_in >= 240 && y_in <= 480){
        return 2;
    }else if(x_in >= 347 && x_in <= 519 && y_in >= 240 && y_in <= 480){
        printf("%s\n","进入相册");
        return 0;
    }else if(x_in >= 520 && x_in <= 800 && y_in >= 0   && y_in <= 480){
        return 1;
    }else{
        return 7;
    }
}

/* 范围触发单击事件 */
int getTouchInput(){
    struct input_event ts_event;
    bool xdone = false;
    bool ydone = false;

    int x_in = 0;
    int y_in = 0;

    int x_out = 0;
    int y_out = 0;

    int count = 0;

    while(1){
        read(fd_ts, &ts_event, sizeof(ts_event));     

        /* 范围触发单击事件 */
        if(ts_event.type == EV_ABS ){ /* 第一次按下记录 */
            if(ts_event.code == ABS_X && xdone == false) {
                count++;
                x_in = x_out = ts_event.value * 800  / 1024 ;
                xdone = true;
                printf("y_in:%d\n", x_in);
            }
            if(ts_event.code == ABS_Y && ydone == false) {
                count++;
                y_in = y_out = ts_event.value * 480 / 600;
                ydone = true;
                printf("y_in:%d\n", y_in);
            }
        }
        if(count < 2){
            continue;
        }

        if(ts_event.type == EV_ABS){/* 如果有移动 */
            if(ts_event.code == ABS_X) {
                x_out = ts_event.value * 800  / 1024;
            }else if(ts_event.code == ABS_Y) {
                y_out = ts_event.value * 480 / 600;
          
            }
        }  

        /* 松手 */
        if(ts_event.type == EV_KEY && ts_event.code == BTN_TOUCH && ts_event.value == 0){
            int x_offset = x_in - x_out;
            int y_offset = y_in - y_out;

            if(y_in >= 430 && y_in <= 480 && abs(y_offset) > 80 && abs(x_offset) < abs(y_offset)){
                if(y_offset < 0){
                    printf("4.下滑事件\n");
                    return DOWN_SLIDE;
                }else{
                    printf("4.上滑事件\n");
                    return UP_SLIDE;
                }
            }

            if(abs(x_offset) <= 20 && abs(y_offset) <= 20){
                return __event(x_in, y_in);            
            }else{
                xdone = false;
                ydone = false;
                continue;
            }                          
        } 

    }
}

/* 应用点击滑动事件 */
int screen_event(){
    struct input_event ts_event;
    bool xdone = false;
    bool ydone = false;

    int x_in = 0;
    int y_in = 0;

    int x_out = 0;
    int y_out = 0;

    while(1){
        read(fd_ts, &ts_event, sizeof(ts_event));

        /* 第一次按下记录 */
        if(ts_event.type == EV_ABS ){
            if(ts_event.code == ABS_X && xdone == false) {
                x_in = x_out = ts_event.value * 800  / 1024;
                xdone = true;
            }
            if(ts_event.code == ABS_Y && ydone == false) {
                y_in = y_out = ts_event.value * 480 / 600;
                ydone = true; 
            }
        }

        /* 如果有移动 */
        if(ts_event.type == EV_ABS){
            if(ts_event.code == ABS_X) {
                x_out = ts_event.value * 800  / 1024;  
            }else if(ts_event.code == ABS_Y) {
                y_out = ts_event.value* 480 / 600;
            }
        } 
        
        /* 松手触发单击事件 */
        if(ts_event.type == EV_KEY && ts_event.code == BTN_TOUCH && ts_event.value == 0){
            printf("in坐标: (%d, %d)\n",  x_in,y_in );
            printf("out坐标:(%d, %d)\n", x_out,y_out);
            int x_offset = x_in - x_out;
            int y_offset = y_in - y_out;
            printf("x_offset:%d,y_offset: %d)\n", x_offset,y_offset);

            xdone = false;              
            ydone = false; 

            if( abs(x_offset) > 80 && abs(x_offset) > abs(y_offset)){
                if(x_in >= 0 && x_in <= 50 && x_offset < 0){
                    printf("4.右滑事件\n");
                    return LEFT_SLIDE;
                }else if(x_in >= 750 && x_in <= 800 && x_offset > 0){
                    printf("4.左滑事件\n");
                    return RIGHT_SLIDE;
                }
                 
            }
            if(y_in >= 430 && y_in <= 480 && abs(y_offset) > 80 && abs(x_offset) < abs(y_offset)){
                if(y_offset < 0){
                    printf("4.下滑事件\n");
                    return DOWN_SLIDE;
                }else{
                    printf("4.上滑事件\n");
                    return UP_SLIDE;
                }
            } 
            /* 松手单击 */
            if(x_in >= 0 && x_in <= 400 ){
                return PREV_CLICK;
            }else {
                return NXET_CLICK;
            }
        }
    }
    return 0;
}

/* 按下单击事件，返回坐标 */
int getChessInput(int *x, int *y){
    struct input_event ts_event;
    bool xdone = false;
    bool ydone = false;

    int x_in = 0;
    int y_in = 0;

    int x_out = 0;
    int y_out = 0;

    int count = 0;

    while(1){
        read(fd_ts, &ts_event, sizeof(ts_event));

        /* 第一次按下记录 */
        if(ts_event.type == EV_ABS ){
            if(ts_event.code == ABS_X && xdone == false) {
                count++;
                x_in = x_out = ts_event.value * 800  / 1024;
                xdone = true;
            }
            if(ts_event.code == ABS_Y && ydone == false) {
                count++;
                y_in = y_out = ts_event.value * 480 / 600;
                ydone = true; 
            }
        }

        if(count < 2){
            continue;
        }

        /* 如果有移动 */
        if(ts_event.type == EV_ABS){
            if(ts_event.code == ABS_X) {
                x_out = ts_event.value * 800  / 1024;  
            }else if(ts_event.code == ABS_Y) {
                y_out = ts_event.value* 480 / 600;
            }
        } 
        
        /* 松手触发单击事件 */
        if(ts_event.type == EV_KEY && ts_event.code == BTN_TOUCH && ts_event.value == 0){
            printf("in坐标: (%d, %d)\n",  x_in,y_in );
            printf("out坐标:(%d, %d)\n", x_out,y_out);
            int x_offset = x_in - x_out;
            int y_offset = y_in - y_out;
            printf("x_offset:%d,y_offset: %d)\n", x_offset,y_offset);

            xdone = false;              
            ydone = false; 

            if( abs(x_offset) > 80 && abs(x_offset) > abs(y_offset)){
                if(x_in >= 0 && x_in <= 50 && x_offset < 0){
                    printf("4.右滑事件\n");
                    return RIGHT_SLIDE;
  
                }else if(x_in >= 750 && x_in <= 800 && x_offset > 0){
                    printf("4.左滑事件\n");
                    return LEFT_SLIDE;
                }
                 
            }
            if(y_in >= 430 && y_in <= 480 && abs(y_offset) > 80 && abs(x_offset) < abs(y_offset)){
                if(y_offset < 0){
                    printf("4.下滑事件\n");
                    return DOWN_SLIDE;
                }else{
                    printf("4.上滑事件\n");
                    return UP_SLIDE;
                }
            } 

            /* 松手单击 */
            *x = x_in;
            *y = y_in;
            return 0;
        }
    }
    return 0;
}

unsigned long get_current_time(){
    return (unsigned long)time(NULL);
}
