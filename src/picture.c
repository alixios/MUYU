#include "lcd.h"
#include "jpg2rgb.h"
#include "list.h"
#include "build_list.h"
#include "screen.h"
#include "scale.h"

extern int fd_lcd;
extern int * map;

void pic_animation(char * path){
 	/* 读取后缀 .jpg文件，生成链表 */
    DoubleLinkList head = double_list_init(NULL);

    build_list_dirpath(path,".jpg", head);

	show_jpg_for_list(path, head);

	/* 销毁链表 */
	destroy(head);
}

int handle_picture(char *path_name)
{
    /* 读取后缀 .jpg文件，生成链表 */
    DoubleLinkList head = double_list_init(NULL);

    build_list_pic(path_name, head);

    list_print(head);
	// 获取链表长度
    int total_images = list_size(head);
	printf("链表长度%d\n", total_images);

	DoubleLinkList node = head->next;
	DoubleLinkList tmp = NULL;

	int picture_page = 0;
    /* 通过图片链表，展示图片 */
	while(1){
		tmp = node;
		showjpg(0,0,"/alan/assets/image/background.jpg");

		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){				
				if(node == head){
					break;
				}

				int start_x = j * SCALE_WIDTH + 5;
				int start_y = i * SCALE_HEIGHT+ 5;
				// 检查文件后缀
				const char *extension = strrchr(node->data.str, '.'); 
				if (extension != NULL) {
					if(strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
						show_jpg_scale(start_x, start_y, node);
					}else if(strcmp(extension, ".bmp") == 0) {
						show_bmp_scale( start_x, start_y, node);
					}
				}
				node = node->next;
			}
		}
		
		/* lcd事件 */
		int x_pos = 0;
		int y_pos = 0;
		int pic_event = getChessInput(&x_pos,&y_pos);
		printf("坐标：（%d, %d)\n", x_pos, y_pos);

		if(pic_event == RIGHT_SLIDE){
			picture_page--;
			if (picture_page < 0) {  // 判断是否到达第一页之前
                picture_page = 0;
            }
			 // 调整链表节点指针位置，向前移动 32 个单位
            for (int i = 0; i < 32; i++) {
                if (node->pre != head) {
                    node = node->pre;
                } else {
                    break;
                }
            }
			continue;
		}else if(pic_event == LEFT_SLIDE){			
			if (node == head) {  // 判断是否到达最后一页
                node = tmp;
				continue;
            }
			picture_page++;
			continue;
		}else if(pic_event == UP_SLIDE){
			break;
		}

		int row = y_pos / (SCALE_HEIGHT + 10); // 计算行
		int col = x_pos / (SCALE_WIDTH  + 10); // 计算列

		int num = (picture_page * 16) + col + row * 4;

		printf("第%d张\n", num);
		if(num > total_images){
			node = tmp;
			continue;
		}

		show_pic_position(head, num);

		node = head->next;
	}


	/* 销毁链表 */
	destroy(head);

    return 0;
}
int show_pic_position(DoubleLinkList head, int num){
	

	DoubleLinkList tmp = head->next;

	while(num-- && tmp != head){
		tmp = tmp->next;
	}

	while (1){
		showjpg(0,0,"/alan/assets/image/background.jpg");

		// 检查文件后缀
		const char *extension = strrchr(tmp->data.str, '.'); 
		if (extension != NULL) {
			if(strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
				printf("center%s\n", tmp->data.str);
				show_jpg_center(tmp->data.str);
			}else if(strcmp(extension, ".bmp") == 0) {
				printf("center%s\n", tmp->data.str);
				show_bmp_center(tmp->data.str);
			}
		}		

		int event = screen_event(); 

        if(event == UP_SLIDE){/* 返回主界面 */
            return 1;
        } 

        if (event == NXET_CLICK && tmp->next != head) {
            tmp = tmp->next; // 显示下一页
        } else if (event == PREV_CLICK  && tmp->pre != head) {
            tmp = tmp->pre; // 显示上一页
        }

	}
	return 0;
}

void show_bmp_scale(int start_x,int start_y,DoubleLinkList head)
{
	int bmp_fd,i,x,y;
	unsigned short heigh,weight;
	unsigned char head_buff[54];//存储头信息的buff
	//打开屏幕
	bmp_fd = open(head->data.str,O_RDWR);
	if(bmp_fd == -1)
	{
		printf("open bmp failure!\n");
		return ;
	}
	
	read(bmp_fd,head_buff,54);
	//得到图片宽度
	weight = head_buff[19]<<8 | head_buff[18];
	
	//得到图片高度
	heigh = head_buff[23]<<8 | head_buff[22];

	if((190+start_x) > 800)
	{
		printf("bmp 190+start_x big 800\n");
		return ;
	}
	
	if((110+start_y) > 480)
	{
		printf("bmp 110+start_y big 480\n");
		return ;
	}
	
	//定义用于存放图片rgb的buff
	unsigned char bmp_buff[weight*heigh*3];
	
	unsigned int buff[190*110];

	//将原图图片像素读到buffer当中
	read(bmp_fd,bmp_buff,sizeof(bmp_buff));
	
	unsigned char bmp_scalebuff[190*110*3];
	
	ScalePicture(bmp_buff,weight,heigh,bmp_scalebuff,190,110);
	
	
	//将rgb合成一个像素点unsigned int
	for(i=0; i<190*110; i++)
		buff[i] = bmp_scalebuff[3*i+0] | bmp_scalebuff[3*i+1]<<8 | bmp_scalebuff[3*i+2]<<16;
	
	
	//图像倒序及显示
	for(y=0; y<110; y++)
		for(x=0; x<190; x++)
			*(map + (y+start_y)*800+ start_x+x) = buff[(110-1-y)*190+x];
		
	close(bmp_fd);
	
}

void show_jpg_scale(int start_x,int start_y, DoubleLinkList head){
	DoubleLinkList tmp = head;

	struct RGBimage * image = jpg2rgb(tmp->data.str);

				
	ScalePicture(image->rgb, image->width, image->height, image->rgb, 190, 110) ;//缩放图片展示800->200 24					
	// 把bgr 数据转换位  argb 数据
	int argbBuf[480][800] = {0};
	for (int y = 0; y < 480; y++) {
		for (int x = 0; x < 800; x++) {
									
			unsigned char r = image->rgb[(x + y * 190) * 3 + 0];
			unsigned char g = image->rgb[(x + y * 190) * 3 + 1];
			unsigned char b = image->rgb[(x + y * 190) * 3 + 2];
						
			// 将RGB数据合成为ARGB格式
			int argbPixel = (0xFF << 24) | (r << 16) | (g << 8) | b;
				
			// 将ARGB数据写入argbBuf
			argbBuf[y][x] = argbPixel;
		}
	}
									

	// - 写入到LCD的设备文件即可
	// memcpy( map , argbBuf , sizeof(argbBuf) );
	for (int y = start_y; y < 110 + start_y; y++){
		for (int x = start_x; x < 190 + start_x; x++){
			if (x < 800 && y < 480){
				*(map+x+y*800) = argbBuf[y-start_y][x-start_x];
			}
		}
	}
	release(image);	
}

int show_jpg_for_list(char *jpg_path_name, DoubleLinkList head)
{
    DoubleLinkList tmp = head->next;
    while (1){       
        if(tmp == head){   /* 跳过链表头 */
           break;
        }

        /* 拼接路径 */
		char jpg[256];
		snprintf(jpg, 255, "%s/%s", jpg_path_name, tmp->data.str);

        showjpg(0, 0, jpg);

        tmp = tmp->next;    
		
		usleep(1000);
    }
}


/* 图片居中显示 */
int show_bmp_center(const char *bmp_path){
	 // - 打开图像文件
    FILE * fp = fopen ( bmp_path , "r" );
    if (fp == NULL)
    {
        fprintf( stderr , "open [%s] error :%s \n" ,bmp_path , strerror(errno) );
        return  -1;
    }

	// 读取文件头
    BITMAPFILEHEADER fileHead = {0};  // 定义一个文件头结构体
    
    // 把文件中的文件头数据读取到文件头结构体中
    fread( &fileHead , sizeof(BITMAPFILEHEADER) , 1 , fp  );

    // 读取信息头
    BITMAPINFOHEADER infoHead = {0};

    // 把信息头读取到结构体中
    fread( &infoHead , sizeof(BITMAPINFOHEADER) , 1 , fp );

	int width = infoHead.biWidth;
	int height = infoHead.biHeight;

	// 计算图片在屏幕上的起始坐标，使其居中显示
    int start_x = (800 - width) / 2;
    int start_y = (480 - height) / 2;

    // 显示图片
    showbmp(start_x, start_y, bmp_path);
    
    // - 关闭、释放等工作
    fclose(fp);
	return 0;
}

/* 图片居中显示 */
int show_jpg_center(const char *jpg_path){
	//把jpgdata图像的原始数据转换bgr颜色数据后返回
    struct RGBimage * image =  jpg2rgb(jpg_path);

	int width = image->width;
	int height = image->height;
	// 确保不会超出图像边界
	if (width > 800) {
		width = 800;
	}
	if (height > 480) {
		height = 480;
	}

	// 计算图片在屏幕上的起始坐标，使其居中显示
    int start_x = (800 - width) / 2;
    int start_y = (480 - height) / 2;

    // 显示图片
    showjpg(start_x, start_y, jpg_path);

    release(image);
}


void showbmp( int x_s, int y_s, const char * FilePaht )
{
    // - 打开图像文件
    FILE * fp = fopen ( FilePaht , "r" );
    if (fp == NULL)
    {
        fprintf( stderr , "open [%s] error :%s \n" , FilePaht , strerror(errno) );
        return  ;
    }

    // - 读取图像的信息
        // 读取文件头
    BITMAPFILEHEADER fileHead = {0};  // 定义一个文件头结构体
    
    // 把文件中的文件头数据读取到文件头结构体中
    fread( &fileHead , sizeof(BITMAPFILEHEADER) , 1 , fp  );
    printf( "文件类型：%c%c 文件大小：%d 偏移量：%d\n" , 
            ((char *)&fileHead.bfType)[0],((char *)&fileHead.bfType)[1],
                fileHead.bfSize , fileHead.bfOffBits );

        // 读取信息头
    BITMAPINFOHEADER infoHead = {0};

    // 把信息头读取到结构体中
    fread( &infoHead , sizeof(BITMAPINFOHEADER) , 1 , fp );
    printf("图像尺寸[%dx%d] \n图像色深：%d\n 图像大小：%d\n" , 
            infoHead.biWidth , infoHead.biHeight , infoHead.biBitCount , infoHead.biSizeImage);


    // 计算图像为了满足行宽为4的倍数补齐的字节数
    int count = (4 - (infoHead.biWidth * 3 % 4))%4;

    // - 读取图像的BGR颜色
    // 定义一个数组准备用于存储图像的BGR数据
    char bgrBuf[ infoHead.biWidth * infoHead.biHeight * 3 ];
    bzero( bgrBuf , sizeof(bgrBuf));
    // 一次性读入所有的BGR数据
    fread( bgrBuf , 3 * infoHead.biWidth * infoHead.biHeight , 1 , fp );

    // - 把 24 位BGR颜色转换 32位 ARGB
    int argbBuf [ infoHead.biHeight ] [ infoHead.biWidth ];
    bzero(argbBuf , sizeof(argbBuf));

    for (int y = 0; y < infoHead.biHeight; y++)
    {
        for (int x = 0; x < infoHead.biWidth ; x++)
        {
            argbBuf[ infoHead.biHeight - y - 1 ][x]  =    
                                        bgrBuf[(x+y*infoHead.biWidth)*3 + 0+count*y] << 0 |   // 蓝色
                                        bgrBuf[(x+y*infoHead.biWidth)*3 + 1+count*y] << 8 |        // 绿色
                                        bgrBuf[(x+y*infoHead.biWidth)*3 + 2+count*y] << 16 ;      // 红色
        }

    }
    

    // - 写入到LCD的设备文件即可
    // memcpy( map , argbBuf , sizeof(argbBuf) );
    for (int y = y_s; y < infoHead.biHeight+y_s; y++){
        for (int x = x_s; x < infoHead.biWidth+x_s; x++){
            if (x < 800 && y < 480){
                *(map+x+y*LCD_W) = argbBuf[y-y_s][x-x_s];
            }
        }
    }
    
    // - 关闭、释放等工作
    fclose(fp);
}



int showjpg(unsigned int start_x, unsigned int start_y, const char *jpg_path){
	printf("图片：%s\n", jpg_path);

	//把jpgdata图像的原始数据转换bgr颜色数据后返回
    struct RGBimage * image =  jpg2rgb(jpg_path);

	int width = image->width;
	int height = image->height;

	printf("图片宽高：%d*%d\n", width,height);
	
	// 确保不会超出图像边界
	if (width > 800) {
		width = 800;
	}
	if (height > 480) {
		height = 480;
	}

	// 把bgr 数据转换位  argb 数据
	int argbBuf[480][800] = {0};
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			unsigned char r = image->rgb[(x + y * image->width) * 3 + 0];
			unsigned char g = image->rgb[(x + y * image->width) * 3 + 1];
			unsigned char b = image->rgb[(x + y * image->width) * 3 + 2];

			// 将RGB数据合成为ARGB格式
			int argbPixel = (0xFF << 24) | (r << 16) | (g << 8) | b;

			// 将ARGB数据写入argbBuf
			argbBuf[y][x] = argbPixel;
		}
	}

    // - 写入到LCD的设备文件即可
    // memcpy( map , argbBuf , sizeof(argbBuf) );
    for (int y = start_y; y < image->height + start_y; y++){
        for (int x = start_x; x < image->width + start_x; x++){
            if (x < 800 && y < 480){
                *(map+x+y*800) = argbBuf[y-start_y][x-start_x];
            }
        }
    }

	release(image);
}

int Stat(const char *filename, struct stat *file_info)
{
	int ret = stat(filename, file_info);

	if(ret == -1)
	{
		fprintf(stderr, "[%d]: stat failed: "
			"%s\n", __LINE__, strerror(errno));
		exit(1);
	}

	return ret;
}

int Open(const char *filename, int mode)
{
	int fd = open(filename, mode);
	if(fd == -1)
	{
		fprintf(stderr, "[%d]: open failed: "
			"%s\n", __LINE__, strerror(errno));
		exit(1);
	}

	return fd;
}

// 将jpeg文件的压缩图像数据读出，放到jpg_buffer中去等待解码
unsigned long read_image_from_file(int fd,
				   unsigned char *jpg_buffer,
				   unsigned long jpg_size)
{
	unsigned long nread = 0;
	unsigned long total = 0;

	while(jpg_size > 0)
	{
		nread = read(fd, jpg_buffer, jpg_size);

		jpg_size -= nread;
		jpg_buffer += nread;
		total += nread;
	}
	close(fd);

	return total;
}

struct RGBimage * jpg2rgb(const char *jpgfile)
{
	// 读取图片文件属性信息
	// 并根据其大小分配内存缓冲区jpg_buffer
	struct stat file_info = {0};
	stat(jpgfile, &file_info);
	int fd = Open(jpgfile, O_RDONLY);

	unsigned char *jpg_buffer = {0};
	jpg_buffer = (unsigned char *)calloc(1, file_info.st_size);
	read_image_from_file(fd, jpg_buffer, file_info.st_size);

	// 声明解码缩结构体，以及错误管理结构体
	struct jpeg_decompress_struct cinfo= {0};
	struct jpeg_error_mgr jerr= {0};

	// 使用缺省的出错处理来初始化解码缩结构体
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	// 配置该cinfo，使其从jpg_buffer中读取jpg_size个字节
	// 这些数据必须是完整的JPEG数据
	jpeg_mem_src(&cinfo, jpg_buffer, file_info.st_size);

	// 读取JPEG文件的头，并判断其格式是否合法
	int ret = jpeg_read_header(&cinfo, true);
	if(ret != 1)
	{
		fprintf(stderr, "[%d]: jpeg_read_header failed: "
			"%s\n", __LINE__, strerror(errno));
        return NULL;
	}

	// 开始解码
	jpeg_start_decompress(&cinfo);

    struct RGBimage * minfo = calloc(1, sizeof(struct RGBimage));
	minfo->width = cinfo.output_width;
	minfo->height = cinfo.output_height;
	minfo->pixel_size = cinfo.output_components;

	int row_stride = minfo->width * minfo->pixel_size;

	// 根据图片的尺寸大小，分配一块相应的内存rgb_buffer
	// 用来存放从jpg_buffer解码出来的图像数据
	unsigned long rgb_size;
	rgb_size = minfo->width * minfo->height * minfo->pixel_size;
	minfo->rgb = (char *)calloc(1, rgb_size);

	// 循环地将图片的每一行读出并解码到rgb_buffer中
	int line = 0;
	while(cinfo.output_scanline < cinfo.output_height)
	{
		unsigned char *buffer_array[1];
		buffer_array[0] = minfo->rgb + (cinfo.output_scanline) * row_stride;
		jpeg_read_scanlines(&cinfo, buffer_array, 1);
	}

	// 解码完了，将jpeg相关的资源释放掉
 	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(jpg_buffer);
	return minfo;
}

void release(struct RGBimage *p){
	if(p == NULL)
		return;

	free(p->rgb);
	free(p);
}