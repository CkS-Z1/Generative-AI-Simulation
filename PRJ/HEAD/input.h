/* input.h */
#ifndef _INPUT_H
#define _INPUT_H

#include "mouse.h"
#include "hz.h"
#include <stdlib.h>
#include <graphics.h>
#include <stdio.h>
#include <string.h>
// #include <bool.h>
#include <ctype.h>

// 屏幕尺寸
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_CONTENT_LENGTH 128

// 颜色定义
#define DARKBLUE 3569
#define LIGHTBLUE 627
#define WATERBLUE 46651
#define DARKGRAY 17224

// 输入法结构体
typedef struct
{
    // 输入状态
    char pinyin[32];        // 拼音缓冲区
    char candidate[110][3]; // 候选字（GBK编码）
    int totalCandidates;    // 总候选词数
    int currentPage;        // 当前候选页
    int isPinyin;           // 是否拼音输入
    int leftArrowDown;      // 左箭头按下状态
    int rightArrowDown;     // 右箭头按下状态
    int lastKey;            // 记录最后一次按下的键值

    // 界面位置
    struct
    {
        int x, y; // 候选框位置
    } candidatePos;

    // 输出结果
    struct
    {
        char content[32]; // 输出内容
        int width;        // 像素宽度
    } output;
    // 保留背景
    void *candidateBackground;
    int candidateBgX, candidateBgY;
} InputState;

// 输入框结构体
typedef struct
{
    int x, y;                 // 输入框位置
    int width, height;        // 输入框尺寸
    int isActive;             // 是否激活输入状态
    int cursorPos;            // 光标位置 / 字符偏移
    char content[128];        // 输入内容缓冲区
    unsigned int bgColor;     // 背景颜色
    unsigned int borderColor; // 边框颜色
    unsigned int textColor;   // 文字颜色
} InputBox;

// 输入法结果
typedef struct
{
    char content[32]; // 输出内容
    int width;        // 像素宽度（用于光标移动）
} IME_Result;

// 公共接口
void IME_Init(InputState *state);
// 输入框接口
void CreateInputBox(InputBox *box, int x, int y, int width, int height);
int DrawInputBox(InputBox *box);
void DestroyInputBox(InputBox *box);
int handleInputBox(InputBox *box, InputState *state, int mouseX, int mouseY, int button);
IME_Result IME_Update(InputState *state, int cursorX, int cursorY);

#endif