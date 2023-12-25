#ifndef __SCALE_H_
#define __SCALE_H_
#include <stdio.h>

//pColorData是原图像素RGP数组
extern void ScalePicture(unsigned char pColorData[],int o_w,int o_h,unsigned char pColorDataMid[],int d_w,int d_h);

#endif