#ifndef __LCD_H
#define __LCD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>


#define LCD_PATH "/dev/fb0"
#define LCD_W    800
#define LCD_H    480
#define LCD_SIZE ((LCD_H)*(LCD_W)*4)

void lcd_init( void );
void lcd_close(void);

#endif