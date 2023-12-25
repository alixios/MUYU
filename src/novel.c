#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "novel.h"
#include "font.h"
#include "screen.h"
#include "lcd.h"
#include "my_type.h"
#include "jpg2rgb.h"

#define MAX_PAGE_FRONT 11*20*3 // 假设屏幕每行最多能显示字符
#define TIME_FORMAT_LEN 6 // 时间格式的长度 HH:mm，加上结束符 \0

extern int fd_lcd;
extern int * map;

int beilf_count;
int novel_page;

int handle_novel(){
    show_novel();
    return 0;
}

void archive_init(){
     /* 读取存档文件 */
    FILE *fp = fopen("assets/archive/archive.dat", "rb");
    if(fp == NULL){
        printf("Failed to open the archive file!\n");
        return;
    }

    Archive archive_data; // 创建一个 Archive 结构体变量

    // 从文件中读取 Archive 结构体的数据
    fread(&archive_data, sizeof(Archive), 1, fp);

    beilf_count = archive_data.beilf_count;
    novel_page =  archive_data.novel_page;

    // 关闭文件
    fclose(fp);
}

void handle_muyu(){
    /* 敲击图片 */
    showjpg(520, 0, "/alan/assets/image/muyu2.jpg");

    /* 功德+1 */
    beilf_count++;
}

/* 显示文档功德计数 */
void show_beilf(){
        //打开字体	
	font *f = fontLoad("/usr/share/fonts/simhei.ttf");
	  
	fontSetSize(f,60); 

    //创建一个画板（点阵图）
    bitmap *bm = createBitmapWithInit(450, 80, 4, getColor(0, 98, 51, 0));

    /* 获得系统时间并转换格式为：HH:mm */
    char buf[20];
    sprintf( buf, "功德：+%d ", beilf_count);

    /* 显示时间 */
    fontPrint(f, bm, 0, 0, buf, getColor(0,255,255,255),450);
    
    //把字体框输出到LCD屏幕上
    show_font_to_lcd(map, 60, 130, bm);
	
	//关闭字体，关闭画板
	fontUnload(f);
    destroyBitmap(bm); 
}

/* 显示时间 */
void show_time(){
    //打开字体	
	font *f = fontLoad("/usr/share/fonts/simhei.ttf");
	  
	fontSetSize(f, 100); 

    //创建一个画板（点阵图）
    bitmap *bm = createBitmapWithInit(450, 100, 4, getColor(0, 98, 51, 0));

    /* 获得系统时间并转换格式为：HH:mm */
    char *buf = get_local_time();
    printf("%s\n", buf);

    /* 显示时间 */
    fontPrint(f, bm, 0, 0, buf, getColor(0,255,255,255),450);
    
    //把字体框输出到LCD屏幕上
    show_font_to_lcd(map, 60, 30, bm);
	
	//关闭字体，关闭画板
	fontUnload(f);
    destroyBitmap(bm); 
}

/** 
 * @brief: 使用font画板居中显示文字
 * @param {char *} src 文字
 * @param {int} width  画板宽
 * @param {int} height 画板高
 * @return {*}
 */
void show_font_str(char * src, int width, int height){
    //打开字体	
	font *f = fontLoad("/usr/share/fonts/simhei.ttf");
	  
	fontSetSize(f, 40); /* 一行能显示 二十个中文 */

    //创建一个画板（点阵图）
    bitmap *bm = createBitmapWithInit(width, height, 4, getColor(0,255,255,255));

    /* 计算屏幕居中显示 */
    int start_x = (LCD_W - width) / 2;
    int start_y = (LCD_H - height) / 2;

    fontPrint(f, bm, 0, 0, src, getColor(0,255,0,0),width);
    
    //把字体框输出到LCD屏幕上
    show_font_to_lcd(map, start_x, start_y, bm);
	
	//关闭字体，关闭画板
	fontUnload(f);
    destroyBitmap(bm); 
}

int show_novel()
{
	//打开字体	
	font *f = fontLoad("/usr/share/fonts/simhei.ttf");
	  
	fontSetSize(f, 40); /* 一行能显示 二十个中文 */

    char * buf = read_text_from_file("/alan/assets/txt/b.txt");
    display_text(f, buf);
	
	//关闭字体，关闭画板
	fontUnload(f);
    
}

// 从文件中读取文本内容
char * read_text_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "无法打开文件 %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 为文本内容分配内存空间
    char *text = (char *)malloc(size + 1);
    if (text == NULL) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }

    // 读取文件内容到缓冲区
    size_t bytesRead = fread(text, 1, size, file);
    if (bytesRead != size) {
        fprintf(stderr, "读取文件失败\n");
        exit(EXIT_FAILURE);
    }

    // 添加字符串结束符
    text[size] = '\0';

    // 关闭文件
    fclose(file);

    return text;
}

// 根据文本内容和屏幕尺寸，进行文本分页和显示
void display_text(font *f,  char *text) {
    int total_page = strlen(text) / MAX_PAGE_FRONT; // 

    char buf[MAX_PAGE_FRONT + 1]; // 用于存储当前页的文本内容
    while (1) {
        //创建一个画板（点阵图）
	    bitmap *bm = createBitmapWithInit(800, 480, 4, getColor(0,255,255,255));
        
        // 显示当前页文本内容到LCD屏幕上
        memset(buf, 0, sizeof(buf));

        // 截取当前页文本内容
        strncpy(buf, text + (novel_page * MAX_PAGE_FRONT), MAX_PAGE_FRONT);        
        fontPrint(f, bm, 0, 0, buf, getColor(0,255,0,0),800);
        
        //把字体框输出到LCD屏幕上
        show_font_to_lcd(map, 0, 0, bm);

        int event = screen_event(); 

        if(event == UP_SLIDE){/* 返回主界面 */
            destroyBitmap(bm); 
            return ;
        } 

        if (event == NXET_CLICK && (novel_page + 1) < total_page) {
            ++novel_page; // 显示下一页
        } else if (event == PREV_CLICK && novel_page > 0) {
            --novel_page; // 显示上一页
        }

        destroyBitmap(bm); 

        // 退出条件
        if ((novel_page + 1) >= total_page) {
            break;
        }
     
    }
}

char *get_local_time() {
    time_t rawtime;
    struct tm *timeinfo;
    static char buffer[TIME_FORMAT_LEN]; // 静态字符数组用于存储时间

    time(&rawtime); // 获取当前时间的时间戳
    timeinfo = localtime(&rawtime); // 转换为本地时间

    strftime(buffer, TIME_FORMAT_LEN, "%H:%M", timeinfo); // 格式化时间为 HH:mm

    return buffer;
}

void archive_exit(){
    /* 读取存档文件 */
    FILE *fp = fopen("assets/archive/archive.dat", "wb");
    if(fp == NULL){
        printf("Failed to open the archive file!\n");
        return;
    }

    Archive archive_data; // 创建一个 Archive 结构体变量
    archive_data.beilf_count = beilf_count;
    archive_data.novel_page = novel_page;

    // 从文件中读取 Archive 结构体的数据
    fwrite(&archive_data, sizeof(Archive), 1, fp);

    // 关闭文件
    fclose(fp);

    /* 关闭映射, lcd */
    lcd_close();
}


