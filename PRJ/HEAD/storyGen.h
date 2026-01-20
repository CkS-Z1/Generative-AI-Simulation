/* storyGen.h */
#ifndef _STORYGEN_H_
#define _STORYGEN_H_

#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include "box.h"
#include "output.h"
#include "mouse.h"

#define STORY_ROOT "STORY\\"
#define MAX_PATH 80
#define MAX_CONTENT 1200

// 用户输入数据结构
typedef struct
{
    char mainName[32];    // 主角名称
    char personality[32]; // 性格特征
    char location[32];    // 故事地点
    char genre;           // 故事类型（a-e）
    int gender;           // 随机生成性别（0/1）
} storyInput;

int generateStory(storyInput input);
void replacePlaceholder(char *str, storyInput input);
void processBranch(char *str, storyInput input, int *score, char type, char *branchPart);
void extractContent(const char *source, const char *startTag, const char *endTag, char *output);
void replaceTag(char *dest, const char *tag, const char *value);
int storyGen(storyInput input);

#endif