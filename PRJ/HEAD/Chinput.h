#ifndef _CHINPUT_H_
#define _CHINPUT_H_

#include <graphics.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "util_c.h"

#define MAX_INPUT 100
#define MAX_CANDIDATES 10 // 每页显示的候选字数量

// 左按键
#define x01 50
#define w01 30
#define y01 421
#define h01 49
// 右按键
#define x02 450
#define w02 30
#define y02 421
#define h02 49
// 候选框
#define x03 81
#define w03 368
#define y03 421
#define h03 49
// 输入框
#define x04 50
#define w04 430
#define y04 370
#define h04 50

void hzinput(char *output);
void drawInputBox(char *content);
void drawCandidateBox(const char candidates[MAX_CANDIDATES][3], int selected, int candidateCount);
#endif