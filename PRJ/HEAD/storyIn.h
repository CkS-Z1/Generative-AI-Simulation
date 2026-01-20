/* storyIn.h */
#ifndef _STORYIN_H_
#define _STORYIN_H_

#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
// #include <queue.h>
#include "input.h"
#include "box.h"
#include "button.h"
#include "mouse.h"
#include "storyGen.h"
#include "picture.h"

// extern std::queue<int> g_inputQueue;  // 全局键盘事件队列

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// 预定义颜色常量
#define TITLE_COLOR WHITE
#define BORDER_COLOR LIGHTCYAN
#define BUTTON_COLOR RED
#define TEXT_COLOR BLACK
#define BG_COLOR LIGHTGRAY

// Bool类型定义
typedef int Bool;
#define True 1
#define False 0

// 下拉框结构体
typedef struct
{
    int x, y;
    int width, height;
    char options[5][20];
    int selected;
    int isOpen;
} DropDown;

// 定义全局变量
extern InputState imeState;
extern InputBox nameBox, traitBox, locationBox;
extern DropDown genreBox;

// 函数定义
int storyInPage(storyInput *userInput);
void InitDropDown(DropDown *dd, int x, int y);
void DrawDropDown(DropDown *dd);
void DrawInterface();
Bool HandleComponents(int mx, int my, int mb);
int isGBK(const char *str);
int findTraits(const char *trait, char *code);
void showError(const char *msg);

#endif