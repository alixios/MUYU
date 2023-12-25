#ifndef __JPG2RGB_H
#define __JPG2RGB_H

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdbool.h>

#include <sys/mman.h>
#include <sys/ioctl.h>

#include "jpeglib.h"
#include "my_type.h"

#define SCALE_WIDTH  200    /* 缩放宽度 */
#define SCALE_HEIGHT 120    /* 缩放高度 */

typedef struct tagBITMAPFILEHEADER {
    u_int16_t bfType;   // 位图的类型
    u_int32_t bfSize;  // 位图的大小
    u_int16_t bfReserved1;  // 保留字节
    u_int16_t bfReserved2;  // 保留字节 
    u_int32_t bfOffBits;   // 位图信息的偏移量
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    u_int32_t biSize;      // 位图大小
    u_int32_t biWidth;      // 位图宽度
    u_int32_t biHeight;     // 位图高度
    u_int16_t biPlanes;     // 为目标设备说明位面数，其值将总是被设为1
    u_int16_t biBitCount;   // 位图的色深  其值为1、4、8、16、24、或32
    u_int32_t biCompression;   // 说明图像数据压缩的类型    BI_RGB \ BI_RLE8 \ BI_RLE4 \ BI_BITFIELDS
    u_int32_t biSizeImage; // 图像大小
    u_int32_t biXPelsPerMeter;  // 水平分辨率
    u_int32_t biYPelsPerMeter;  // 垂直分辨率
    u_int32_t biClrUsed;      // 说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）
    u_int32_t biClrImportant; // 说明对图像显示有重要影响的颜色索引的数目，如果是0，表示都重要。
}__attribute__((packed))  BITMAPINFOHEADER;

/* 图片数据 */
struct RGBimage{
	int width;
	int height;
	int pixel_size; 

	char *rgb;
};

/* 图片显示 */
int handle_picture(char *path_name);

/* 动画 */
void pic_animation(char * path);

/* 图片居中显示 */
int show_jpg_center(const char *jpg_path);

int show_bmp_center(const char *bmp_path);

/* 坐标显示图片 */
int show_pic_position(DoubleLinkList head, int num);

/* 链表显示图片 */
int show_jpg_for_list(char *jpg_path_name, DoubleLinkList head);

/* 展示缩放图片 */
void show_jpg_scale(int start_x,int start_y, DoubleLinkList head);

void show_bmp_scale(int start_x,int start_y, DoubleLinkList head);

void showbmp( int x_s, int y_s, const char * FilePaht );

int showjpg(unsigned int start_x, unsigned int start_y, const char *jpg_path);

/* 将jpeg文件的压缩图像数据读出，放到jpg_buffer中去等待解压 */
unsigned long read_image_from_file(int fd,
				   unsigned char *jpg_buffer,
				   unsigned long jpg_size);

int Stat(const char *filename, struct stat *file_info);

int Open(const char *filename, int mode);

struct RGBimage *jpg2rgb(const char *jpgfile);

void release(struct RGBimage *p);


#endif
