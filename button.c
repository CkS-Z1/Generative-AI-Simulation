#include "button.h" // �����ӿڵ�ͷ�ļ�
#include "hz.h"     // ����������ʾ����
#include "mouse.h"  // ������깦��
/*****************************************
button.c
COPYRIGHT:	ChenSihao
FUNCTION:	button
ABSTRACT:   ����������ť����Ȼÿ�ζ�Ҫ����д��ô�ණ����̫����
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
DESCRIPTION: ��ʼ����ť����������������д�밴ť��
INPUT: button -> x,y,width,height,*text
RETURN: (NULL)
***********************************************/
void initButton(Button *button, int x, int y, int width, int height, char *text)
{
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    button->text = malloc(strlen(text) + 1); // �����㹻���ڴ�
    if (button->text == NULL)
    {
        printf("Out of memory!");
        exit(1);
    }
    strcpy(button->text, text); // �����ַ���
}

/*********************************************
FUNCTION: drawButton
DESCRIPTION: ���ư�ť����ȡ�������İ�ť�ṹ������ʽ���ư�ť
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
DESCRIPTION: ���ڻ��ư�ťʱʹ����malloc��������Ҫ�ͷŶ�̬�ڴ棬���ٰ�ť
INPUT: button
RETURN: (NULL)
***********************************************/
void destroyButton(Button *button)
{
    if (button)
    {
        free(button->text);  // �ͷŶ�̬������ı��ڴ�
        button->text = NULL; // ��ָ����Ϊ NULL����������ָ��
    }
}