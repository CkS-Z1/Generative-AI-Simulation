#ifndef _PICTURE_H_
#define _PICTURE_H_

#include "IMAGE.h"
#include "hz.h"
#include "mouse.h"
#include <graphics.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// 常量定义
#define INITIAL_TEMP 1.0 // 初始温度
#define STAR_SIZE 25
#define STAR_SPACE 50
#define MAX_PICTURES 3 // 图片数量

// 概率分布结构体
typedef struct
{
    double prob[MAX_PICTURES]; // 三个图片的概率分布
    double temperature;        // 软最大策略的温度参数
    int generatedPics[2];      // 最近两次生成的图片索引
    int genCnt;                // 已生成图片计数
} Picstate;

// 函数声明
int outPicture(char genre, int storyNum, Picstate *state);
void ratingSystem(int cnt, Picstate *state);
void drawStarFrame(int y, int highlight);

#endif