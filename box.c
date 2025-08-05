#include "box.h"
#include <conio.h>
/*****************************************
box.c
COPYRIGHT:	ChenSihao
FUNCTION:	box
ABSTRACT:   用来建立文本框，加上文本框点击显示功能
DATE:2024/2/21
******************************************/

/*********************************************
FUNCTION: initBox
DESCRIPTION: 初始化文本框
INPUT: box -> x,y,width,height,*text
RETURN: (NULL)
***********************************************/
void initBox(Box *box, int x, int y, int width, int height, char *text)
{
    box->x = x;
    box->y = y;
    box->width = width;
    box->height = height;
    box->text = malloc(strlen(text) + 1);
    if (box->text == NULL)
    {
        printf("Out of memory!");
        exit(1);
    }
    strcpy(box->text, text);
}

/*********************************************
FUNCTION: drawBox
DESCRIPTION: 绘制文本框，读取传进来的结构，按格式绘制
INPUT: box
RETURN: (NULL)
***********************************************/
void drawBox(Box box)
{
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(box.x, box.y, box.x + box.width, box.y + box.height);
    setcolor(BROWN);
    rectangle(box.x, box.y, box.x + box.width, box.y + box.height);
    setcolor(WHITE);
    outtextxy(box.x + 10, box.y + 5, box.text);
}

/*********************************************
FUNCTION: destroyBox
DESCRIPTION: 由于绘制时使用了malloc，所以需要释放动态内存，销毁文本框内容
INPUT: box
RETURN: (NULL)
***********************************************/
void destroyBox(Box *box)
{
    if (box)
    {
        free(box->text);
        box->text = NULL;
    }
}

/*********************************************
FUNCTION: putText
DESCRIPTION: 模拟输入账户，在指定位置按字符输入显示
INPUT: box input inputLen
RETURN: (NULL)
***********************************************/
void putText(Box *box, char *input, int *inputLen)
{
    static char display[16]; // 使用静态内存
    char ch;
    int len = *inputLen;
    int nx, ny, nbuttons;
    mread(&nx, &ny, &nbuttons);
    clrmous(nx, ny);
    // 点击交互窗口后变换显示
    setfillstyle(SOLID_FILL, YELLOW);
    bar(box->x, box->y, box->x + box->width, box->y + box->height);
    setcolor(BLUE);
    rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
    settextstyle(8, 0, 4); // 4号字体
    outtextxy(box->x + 10, box->y + 5, input);

    while (1)
    {
        // 更新鼠标状态
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        // 检查鼠标点击是否在窗口外
        if (MouseS == 1 && (MouseX < box->x || MouseX > box->x + box->width || MouseY < box->y || MouseY > box->y + box->height))
        {
            break;
        }

        // 非阻塞读取字符
        if (kbhit())
        {
            ch = getch();
            if (ch == 13)
            { // 读取到回车时停止输入
                clrmous(nx, ny);
                break;
            }
            // 读到退格时删除，覆盖原来的内容
            if (ch == 8 && len > 0)
            {
                len--;
                input[len] = '\0';
                setfillstyle(SOLID_FILL, YELLOW);
                bar(box->x, box->y, box->x + box->width, box->y + box->height);
                setcolor(BLUE);
                rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
            }
            // 用户名最长为8位
            else if (len >= 8)
            {
                clrmous(nx, ny);
                strncpy(display, input, len);
                display[len] = '\0';
                setfillstyle(SOLID_FILL, CYAN);
                bar(box->x, box->y, box->x + box->width, box->y + box->height);
                setcolor(DARKGRAY);
                rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
                settextstyle(8, 0, 4); // 4号字体
                outtextxy(box->x + 10, box->y + 5, display);
                break;
            }
            // 否则判断是否继续输入
            else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            {
                input[len++] = ch;
                input[len] = '\0';
            }

            // 显示输入内容
            strncpy(display, input, len);
            display[len] = '\0';
            outtextxy(box->x + 10, box->y + 5, display);
        }

        // 绘制鼠标
        // drawmous(nx, ny);
        // delay(30);  // 添加延迟以减少CPU占用
    }

    // 输入结束后变换窗口和文字颜色
    setfillstyle(SOLID_FILL, CYAN);
    bar(box->x, box->y, box->x + box->width, box->y + box->height);
    setcolor(DARKGRAY);
    rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
    settextstyle(8, 0, 4); // 4号字体
    outtextxy(box->x + 10, box->y + 5, input);
    clrmous(nx, ny);
    *inputLen = len;
}

/*********************************************
FUNCTION: putPassword
DESCRIPTION: 模拟输入密码，输入密码时显示为“*”
INPUT: box password passwordLen
RETURN: (NULL)
***********************************************/
void putPassword(Box *box, char *password, int *passwordLen)
{
    static char display[16]; // 使用静态内存
    int i = 0;
    char ch;
    int len = *passwordLen;
    int nx, ny, nbuttons;
    mread(&nx, &ny, &nbuttons);
    clrmous(nx, ny);
    // 点击交互窗口后变换显示
    for (i = 0; i < len; i++)
    {
        display[i] = '*';
    }
    display[len] = '\0';
    setfillstyle(SOLID_FILL, YELLOW);
    bar(box->x, box->y, box->x + box->width, box->y + box->height);
    setcolor(BLUE);
    rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
    settextstyle(8, 0, 4); // 4号字体
    outtextxy(box->x + 10, box->y + 5, display);

    while (1)
    {
        // 更新鼠标状态
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        // 检查鼠标点击是否在窗口外
        if (MouseS == 1 && (MouseX < box->x || MouseX > box->x + box->width || MouseY < box->y || MouseY > box->y + box->height))
        {
            break;
        }

        // 非阻塞读取字符
        if (kbhit())
        {
            ch = getch();
            if (ch == 13)
            { // 读取到回车时停止输入
                clrmous(nx, ny);
                break;
            }
            // 读到退格时删除
            if (ch == 8 && len > 0)
            {
                len--;
                password[len] = '\0';
                setfillstyle(SOLID_FILL, YELLOW);
                bar(box->x, box->y, box->x + box->width, box->y + box->height);
                setcolor(BLUE);
                rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
            }
            // 密码长度超过15位时停止输入
            else if (len >= 15)
            {
                clrmous(nx, ny);
                for (i = 0; i < len; i++)
                {
                    display[i] = '*';
                }
                display[len] = '\0';
                setfillstyle(SOLID_FILL, CYAN);
                bar(box->x, box->y, box->x + box->width, box->y + box->height);
                setcolor(DARKGRAY);
                rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
                settextstyle(8, 0, 4); // 4号字体
                outtextxy(box->x + 10, box->y + 5, display);
                break;
            }
            else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            {
                password[len++] = ch;
                password[len] = '\0';
            }

            // 显示输入内容为“*”
            for (i = 0; i < len; i++)
            {
                display[i] = '*';
            }
            display[len] = '\0';
            outtextxy(box->x + 10, box->y + 5, display);
        }
        // delay(30);  // 添加延迟以减少CPU占用
    }

    // 输入结束后变换窗口和文字颜色
    for (i = 0; i < len; i++)
    {
        display[i] = '*';
    }
    display[len] = '\0';
    setfillstyle(SOLID_FILL, CYAN);
    bar(box->x, box->y, box->x + box->width, box->y + box->height);
    setcolor(DARKGRAY);
    rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
    settextstyle(8, 0, 4); // 4号字体
    outtextxy(box->x + 10, box->y + 5, display);
    clrmous(nx, ny);
    *passwordLen = len;
}