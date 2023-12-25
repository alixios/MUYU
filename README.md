======================================================
 * @Description  : 鱼乐终端
 * @version      : 1.0.0
 * @Author       : alan
 * @Date         : 2023-12-20 11:41:19
 * @copyright (c), 2023 by alan, All Rights Reserved.
======================================================

## 项目介绍
本项目由C编写,使用JPEG库、字体库结合双向链表实现图片缩放、放大、展示，
五子棋小游戏，小说，木鱼等功能。

## 环境依赖
arm-linux
make

## 部署步骤
1. 执行 make

2. 运行 ./bin/main

## 目录结构描述

├── README.md             // help       
├── makefile              // 构建项目
├── assets                //资源文件
├── bin                   //执行文件
├── inc                   //头文件
├── lib
│   ├── libfont.a         //字体库
│   └── libjpeg.so.9.4.0  //jpeg库
└── src
    ├── build_list.c      //构建链表
    ├── game.c            //五子棋
    ├── lcd.c             //lcd
    ├── list.c            //链表
    ├── main.c            //main
    ├── novel.c           //小说
    ├── picture.c         //图片
    ├── scale.c           //缩放
    └── screen.c          //屏幕事件


## 使用说明
点击操作，上滑返回上一界面 

### V1.0.0 版本内容更新


