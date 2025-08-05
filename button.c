#include "button.h" // 包含接口的头文件
#include "hz.h"     // 包含汉字显示功能
#include "mouse.h"  // 包含鼠标功能
/*****************************************
button.c
COPYRIGHT:	ChenSihao
FUNCTION:	button
ABSTRACT:   用来建立按钮，不然每次都要重新写那么多东西，太烦了
DATE:2024/1/24
******************************************/
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 30
/*
typedef struct {
    int x, y, width, height;
    char *text;
} Button;
*/

/*********************************************
FUNCTION: initButton
DESCRIPTION: 初始化按钮，将传进来的数据写入按钮中
INPUT: button -> x,y,width,height,*text
RETURN: (NULL)
***********************************************/
void initButton(Button *button, int x, int y, int width, int height, char *text)
{
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    button->text = malloc(strlen(text) + 1); // 分配足够的内存
    if (button->text == NULL)
    {
        printf("Out of memory!");
        exit(1);
    }
    strcpy(button->text, text); // 拷贝字符串
}

/*********************************************
FUNCTION: drawButton
DESCRIPTION: 绘制按钮，读取传进来的按钮结构，按格式绘制按钮
INPUT: button
RETURN: (NULL)
***********************************************/
void drawButton(Button button)
{
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(button.x, button.y, button.x + button.width, button.y + button.height);
    setcolor(BROWN);
    rectangle(button.x, button.y, button.x + button.width, button.y + button.height);
    setcolor(WHITE);
    puthz(button.x + 25, button.y + 15, button.text, 24, 30, WHITE);
}

/*********************************************
FUNCTION: destroyButton
DESCRIPTION: 由于绘制按钮时使用了malloc，所以需要释放动态内存，销毁按钮
INPUT: button
RETURN: (NULL)
***********************************************/
void destroyButton(Button *button)
{
    if (button)
    {
        free(button->text);  // 释放动态分配的文本内存
        button->text = NULL; // 将指针置为 NULL，避免悬垂指针
    }
}