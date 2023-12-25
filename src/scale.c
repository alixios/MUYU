#include "scale.h"
/*****************************************************************************************
*	功能：获取放大缩小后的图片像素点数组
*	参数 pColorData 原图像素点数组
*		   o_w o_h 原图宽度 和 高度
*		   pColorDataMid 目标图片像素点数组
*		   d_w d_h 目标图片宽度和高度
*****************************************************************************************/
void ScalePicture(unsigned char pColorData[],int o_w,int o_h,unsigned char pColorDataMid[],int d_w,int d_h)
{
	int hnum,wnum;
	for ( hnum = 0; hnum < d_h; hnum++)
		for ( wnum = 0; wnum < d_w; wnum++)
		{
			double d_original_img_hnum = (hnum+0.5)*o_h / (double)d_h-0.5;
			double d_original_img_wnum = (wnum+0.5)*o_w / (double)d_w-0.5;
			int i_original_img_hnum = d_original_img_hnum;
			int i_original_img_wnum = d_original_img_wnum;
			double distance_to_a_x = d_original_img_wnum - i_original_img_wnum;//在原图像中与a点的水平距离  
			double distance_to_a_y = d_original_img_hnum - i_original_img_hnum;//在原图像中与a点的垂直距离  
	 
			int original_point_a = i_original_img_hnum*o_w*3 + i_original_img_wnum * 3;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点A    
			int original_point_b = i_original_img_hnum*o_w*3 + (i_original_img_wnum + 1) * 3;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点B  
			int original_point_c = (i_original_img_hnum + 1)*o_w*3 + i_original_img_wnum * 3;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点C   
			int original_point_d = (i_original_img_hnum + 1)*o_w*3 + (i_original_img_wnum + 1) * 3;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点D   
			
			if (i_original_img_hnum >= o_h - 1)
			{
				original_point_c = original_point_a;
				original_point_d = original_point_b;
			}
			if (i_original_img_wnum >= o_w - 1)
			{
				original_point_b = original_point_a;
				original_point_d = original_point_c;
			}
			int pixel_point = hnum*d_w*3 + wnum * 3;//映射尺度变换图像数组位置偏移量  
			pColorDataMid[pixel_point] =
				pColorData[original_point_a] * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				pColorData[original_point_b] * distance_to_a_x*(1 - distance_to_a_y) +
				pColorData[original_point_c] * distance_to_a_y*(1 - distance_to_a_x) +
				pColorData[original_point_c] * distance_to_a_y*distance_to_a_x;
			pColorDataMid[pixel_point + 1] =
				pColorData[original_point_a + 1] * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				pColorData[original_point_b + 1] * distance_to_a_x*(1 - distance_to_a_y) +
				pColorData[original_point_c + 1] * distance_to_a_y*(1 - distance_to_a_x) +
				pColorData[original_point_c + 1] * distance_to_a_y*distance_to_a_x;
			pColorDataMid[pixel_point + 2] =
				pColorData[original_point_a + 2] * (1 - distance_to_a_x)*(1 - distance_to_a_y) +
				pColorData[original_point_b + 2] * distance_to_a_x*(1 - distance_to_a_y) +
				pColorData[original_point_c + 2] * distance_to_a_y*(1 - distance_to_a_x) +
				pColorData[original_point_c + 2] * distance_to_a_y*distance_to_a_x;
	 
		}
}
