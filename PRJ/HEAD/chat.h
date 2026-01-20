#ifndef CHAT_H
#define CHAT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "util_c.h"
// 定义常量
#define MAX_QUESTION_LENGTH 248
#define MAX_ANSWER_LENGTH 248
#define MAX_MATCHED_QUESTIONS 5

typedef struct
{
    char name[20];
} IDX;

// 内置虚词表（常用中文虚词）
static const char *PARTICLES[] = {
    "的", "了", "吗", "呢", "吧", "啊", "呀", "哇", "哦", "啦",
    "么", "嘛", "呐", "咯", "喽", "咧", "哩", "啰", "哟", "哉",
    NULL // 结束标记
};

// 函数声明
int find_answer(char *input, char *response, char *filename);
void trim(char *str);
unsigned short fast_hash(const char *str);
int are_synonyms(const char *word1, const char *word2);
void filter_particles(const char *input, char *output);

#endif // CHAT_H