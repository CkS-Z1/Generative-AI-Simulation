#ifndef UTIL_C
#define UTIL_C

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include <dirent.h>

#include "hz.h"
#include "mouse.h"
#include "Chinput.h"


#define MAX_LINE_WORDS 20       // 每行最大字符数
#define LINE_HEIGHT 16          // 每行高度
#define CHAR_WIDTH 16           // 每个字符宽度
#define MAX_MESSAGE_QUANTITY 40 // 消息数量
#define MAX_MESSAGE_WORDS 100   // 最大问题长度
#define CHAR_SPACING_OFFSET 16  // 字符间隔偏移量
#define MAX_FRAME_WIDTH 320     // 消息框宽度
#define MAX_PAGES 10            // 最大页数
#define MAX_LINE_LENGTH 1024

// 区分消息类型
#define yonghu 0
#define AI 1
// 区分按键上的字符类型
#define HANZI 0
#define FUHAO 1
// 消息区域的边界
#define ZUOX 50
#define YOUX 610
#define SHANGY 10
#define XIAY 350
// 翻页
#define PAGE_UP 1
#define PAGE_DOWN -1
// 报错

// 按键结构体
typedef struct
{
    int x, y, width, height;
    char *text;
} Trigger;

// 存储消息
typedef struct
{
    char *message; // 消息内容
    int num;
} Msg;

// 消息结构体
typedef struct
{
    int x, y, width, height;
    Msg message;
    int bottom;
    int duixiang;
    int row;
    int length;
} Body;

// 函数定义
void initTrigger(Trigger *trigger, int x, int y, int width, int height, char *text);
void drawTrigger(Trigger trigger, int zifu);
void freeBody(Body *body);
void showErrorPrompt(const char *Prompt);
void addMessage(Body *body, int *y, int *page_int, int *maxpage);
void Msg_preprocessing(Body *body, int y, char *text, int duixiang);
void clearWindow();
void drawWindow(Body **body, int count);
void drawMsgTxt(int x, int y, char *s, int part, int color, int words);
void saveMsg(const char *filename, char *message);
void readMsg(const char *filename, Msg *msg);
void Save_image(int x1, int y1, int x2, int y2, char *filepath);
void Load_image(int x1, int y1, int x2, int y2, char *filepath);
void FlipPage(int flip, int *page_count, int *cur_y, int *max_page);
int fileExists(const char *filepath);
void handleMenuClick(int *cur_y);
void generateTimestamp(char *timestamp, size_t size);
int listFolders(const char *folderPath, char folderNames[][50], int maxFolders);
void drawCurrentPage(int x1, int y1, int x2, int y2, Trigger HisTrigger[], char foldernames[][50], int current_page, int foldercount, Trigger *PrevBtn, Trigger *NextBtn);
void loadHistory(const char *folderpath, int *y);
void save_history(const char *folderpath, int pagecount, int y);
#endif