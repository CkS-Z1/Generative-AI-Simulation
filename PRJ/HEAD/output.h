/* output.h */
#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hz.h"
#include "mouse.h"
#include "storyGen.h"

#define DARKBLUE 3569
#define LIGHTBLUE 627
#define WATERBLUE 46651
#define DARKGRAY 17224

// 按钮区域定义
#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 35
#define PREV_BUTTON_X 20
// #define MAX_PAGE 100
#define NEXT_BUTTON_X 560
#define BUTTON_Y 430
#define MARGIN_BOTTOM 40
#define GEN_BUTTON_X 280

// 颜色定义优化
#define BUTTON_BG LIGHTBLUE    // 常规按钮背景
#define BUTTON_BORDER DARKBLUE // 按钮边框
#define GEN_BUTTON_BG 0x2A     // 亮绿色文生图按钮
#define DISABLED_GRAY 0x07     // 禁用按钮颜色

// 布局参数优化
#define BUTTON_RADIUS 5   // 圆角半径
#define CONTENT_BG 0x17   // 浅蓝灰内容背景
#define BORDER_COLOR 0x01 // 深蓝色边框

/* 屏幕尺寸和布局参数 */
#define START_X 20
#define START_Y 20
#define LINE_HEIGHT 20 // 行高
#define PARA_HEIGHT 40 // 段落间距

/* 文本输出接口 */
void reset_cursor();
void outputText(const char *str, int isNewParagraph);
void show_story();
void reset_pages();

#endif