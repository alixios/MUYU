#include "lcd.h"

int fd_lcd;
int * map;

void lcd_init(){
    // - 打开LCD并初始化（MMAP）
    fd_lcd = open(LCD_PATH, O_RDWR );
    if (fd_lcd == -1){
        perror("open fb0 file error");
        return  ;
    }
    
    // 内存映射
    map = mmap( NULL, LCD_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_lcd, 0);
    if (map == MAP_FAILED){
        perror("mmap error ");
        return  ;
    }
    return ;
}

void lcd_close(){
    munmap(map ,LCD_SIZE );
    close(fd_lcd);
}