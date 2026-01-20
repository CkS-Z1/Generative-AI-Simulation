#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdio.h> // 内存不足时用printf输出提示要用
#include <dos.h>
#include <stdlib.h>   // 动态分配内存要用
#include <string.h>   // 拷贝内容进入 button.text 要用
#include <graphics.h> // 伟大，无需多言

typedef struct
{
    int x, y, width, height;
    char *text;
} Button;

void initButton(Button *button, int x, int y, int width, int height, char *text); // 初始化按钮
void drawButton(Button button);                                                   // 绘制按钮
void destroyButton(Button *button);                                               // 销毁按钮

#endif