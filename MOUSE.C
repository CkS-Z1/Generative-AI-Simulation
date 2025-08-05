#include <conio.h>    // 包含标准输入输出函数
#include <graphics.h> // 包含图形库函数
#include <dos.h>      // 包含DOS中断调用函数
#include <stdio.h>    // 包含标准输入输出
#include <stdlib.h>   // 包含标准库函数
#include <time.h>
#include "mouse.h" // 包含鼠标头文件

// 全局变量定义
int MouseX;      // 鼠标当前X坐标
int MouseY;      // 鼠标当前Y坐标
int MouseS;      // 鼠标状态（按钮状态等）
int press;       // 鼠标按键状态（是否按下）
void *buffer;    // 用于保存鼠标背景的缓冲区
union REGS regs; // DOS中断寄存器结构
int flag = 0;    // 标志变量，用于判断鼠标是否初始化成功

// 初始化鼠标
void mouseinit()
{
    int retcode;                  // 返回值q
    int xmin, xmax, ymin, ymax;   // 鼠标活动区域
    int x_max = 625, y_max = 480; // 屏幕最大坐标
    int size;

    // 设置鼠标活动区域
    xmin = 2;
    xmax = x_max - 1;
    ymin = 8;
    ymax = y_max - 2;

    // 调用 DOS中断 初始化鼠标
    regs.x.ax = 0;           // 初始化鼠标
    int86(51, &regs, &regs); // 调用中断
    retcode = regs.x.ax;     // 获取返回值

    // 检查鼠标是否初始化成功
    if (retcode == 0)
    {
        printf("Mouse or Mouse Driver Missing, Please Install!\n");
        delay(5000); // 延时5秒
    }
    else
    {
        // 设置鼠标水平活动范围
        regs.x.ax = 7;
        regs.x.cx = xmin;
        regs.x.dx = xmax;
        int86(51, &regs, &regs);

        // 设置鼠标垂直活动范围
        regs.x.ax = 8;
        regs.x.cx = ymin;
        regs.x.dx = ymax;
        int86(51, &regs, &regs);

        // 初始化鼠标状态
        MouseS = 0;
        MouseX = 320;          // 鼠标初始X坐标
        MouseY = 240;          // 鼠标初始Y坐标
        save_bk_mou(320, 240); // 保存鼠标初始位置的背景
        mouse(MouseX, MouseY); // 绘制鼠标光标
        flag = 1;              // 标志设置为1，表示鼠标初始化成功
    }
}

// 绘制鼠标光标
void mouse(int x, int y)
{
    switch (MouseS)
    {
    case 1: // 鼠标左键按下
    {
        // 绘制鼠标光标图形（具体图形由line和arc函数定义）
        setcolor(WHITE);       // 设置颜色为白色
        setlinestyle(0, 0, 1); // 设置线条样式为实线

        // 绘制鼠标光标的轮廓（白色部分）
        line(x - 1, y + 9, x - 1, y + 8);
        line(x, y + 7, x, y + 11);
        line(x + 1, y + 6, x + 1, y + 13);
        line(x + 2, y + 8, x + 2, y + 14);
        line(x + 3, y - 1, x + 3, y + 15);
        arc(x + 4, y - 1, 0, 180, 1); // 绘制上半圆
        line(x + 4, y - 2, x + 4, y + 15);
        line(x + 5, y - 1, x + 5, y + 16);
        arc(x + 6, y + 3, 0, 180, 1); // 绘制上半圆
        line(x + 6, y + 2, x + 6, y + 16);
        line(x + 7, y + 3, x + 7, y + 17);
        arc(x + 8, y + 5, 0, 180, 1); // 绘制上半圆
        line(x + 8, y + 4, x + 8, y + 17);
        line(x + 9, y + 5, x + 9, y + 16);
        arc(x + 10, y + 7, 0, 180, 1); // 绘制上半圆
        line(x + 10, y + 6, x + 10, y + 16);
        line(x + 11, y + 7, x + 11, y + 13);

        // 绘制鼠标光标的阴影部分（深灰色部分）
        setcolor(DARKGRAY); // 设置颜色为深灰色
        line(x - 1, y + 9, x - 1, y + 8);
        line(x - 1, y + 8, x + 1, y + 6);
        line(x + 1, y + 6, x + 3, y + 10);
        line(x + 3, y + 10, x + 3, y - 1);
        arc(x + 4, y - 1, 0, 180, 1); // 绘制上半圆
        line(x + 5, y - 1, x + 5, y + 5);
        arc(x + 6, y + 3, 0, 180, 1); // 绘制上半圆
        line(x + 7, y + 3, x + 7, y + 7);
        arc(x + 8, y + 5, 0, 180, 1); // 绘制上半圆
        line(x + 9, y + 5, x + 9, y + 9);
        arc(x + 10, y + 7, 0, 180, 1); // 绘制上半圆
        line(x + 11, y + 7, x + 11, y + 13);
        arc(x + 7, y + 13, -90, 0, 4); // 绘制右上角的圆弧
        line(x + 7, y + 17, x + 3, y + 15);
        line(x + 3, y + 15, x + 1, y + 13);
        line(x + 1, y + 13, x - 1, y + 9);
    }
    break;

    case 2: // 鼠标右键按下
    {
        // 绘制鼠标光标图形（具体图形由line和arc函数定义）
        setcolor(DARKGRAY);    // 设置颜色为深灰色
        setlinestyle(0, 0, 1); // 设置线条样式
        // 绘制鼠标光标的具体线条和弧形
        line(x + 1, y - 1, x + 9, y - 1);
        line(x + 1, y + 15, x + 9, y + 15);
        line(x + 5, y - 1, x + 5, y + 15);
    }
    break;

    case 3: // 鼠标中键按下
    {
        // 绘制鼠标光标图形（具体图形由line和arc函数定义）
        setcolor(WHITE);       // 设置颜色为白色
        setlinestyle(0, 0, 1); // 设置线条样式
        // 绘制鼠标光标的具体线条和弧形
        line(x - 1, y + 7, x + 11, y + 7);
        line(x + 5, y - 1, x + 5, y + 15);
    }
    break;

    default: // 鼠标未按下
    {
        // 设置线条样式和颜色
        setlinestyle(0, 0, 1); // 设置为实线
        setcolor(WHITE);       // 设置颜色为白色

        // 绘制鼠标光标的主体部分（白色线条）
        line(x, y, x, y + 13);              // 垂直线
        line(x + 1, y + 1, x + 1, y + 12);  // 垂直线
        line(x + 2, y + 2, x + 2, y + 11);  // 垂直线
        line(x + 3, y + 3, x + 3, y + 10);  // 垂直线
        line(x + 4, y + 4, x + 4, y + 12);  // 垂直线
        line(x + 5, y + 5, x + 5, y + 9);   // 垂直线
        line(x + 5, y + 11, x + 5, y + 14); // 垂直线
        line(x + 6, y + 6, x + 6, y + 9);   // 垂直线
        line(x + 6, y + 13, x + 6, y + 15); // 垂直线
        line(x + 7, y + 7, x + 7, y + 9);   // 垂直线
        line(x + 8, y + 8, x + 8, y + 9);   // 垂直线
        line(x + 9, y + 9, x + 9, y + 9);   // 垂直线

        // 切换颜色绘制阴影部分
        setcolor(DARKGRAY); // 设置颜色为深灰色

        // 绘制鼠标光标的阴影部分（深灰色线条）
        line(x - 1, y - 1, x - 1, y + 14);   // 左侧阴影
        line(x - 1, y + 14, x + 3, y + 11);  // 底部阴影
        line(x + 3, y + 11, x + 3, y + 12);  // 右侧阴影
        line(x + 3, y + 12, x + 4, y + 13);  // 右侧阴影
        line(x + 4, y + 13, x + 4, y + 14);  // 右侧阴影
        line(x + 4, y + 14, x + 7, y + 17);  // 右侧阴影
        line(x + 7, y + 17, x + 7, y + 13);  // 右侧阴影
        line(x + 7, y + 13, x + 6, y + 12);  // 右侧阴影
        line(x + 6, y + 12, x + 6, y + 11);  // 右侧阴影
        line(x + 6, y + 11, x + 5, y + 10);  // 右侧阴影
        line(x + 5, y + 10, x + 11, y + 10); // 右侧阴影
        line(x + 11, y + 10, x - 1, y - 2);  // 右侧阴影
    }
    break;
    }
}

// 读取鼠标状态
void mread(int *nx, int *ny, int *nbuttons)
{
    // 调用DOS中断获取鼠标状态
    regs.x.ax = 3; // 获取鼠标位置和按钮状态
    int86(51, &regs, &regs);
    *nx = regs.x.cx;       // 获取鼠标X坐标
    *ny = regs.x.dx;       // 获取鼠标Y坐标
    *nbuttons = regs.x.bx; // 获取鼠标按钮状态
}

void newmouse(int *nx, int *ny, int *nbuttons)
{
    int xn, yn, buttonsn;                         // 新的鼠标位置和按钮状态
    int x0 = *nx, y0 = *ny, buttons0 = *nbuttons; // 旧的鼠标位置和按钮状态

    // 读取新的鼠标状态
    mread(&xn, &yn, &buttonsn);
    *nx = xn;
    *ny = yn;
    *nbuttons = buttonsn;

    // 判断鼠标位置和按钮状态是否发生变化
    if (buttons0 != buttonsn)
    {
        // 如果按钮状态发生变化，则更新按钮状态
        *nbuttons = buttonsn;
    }
    if (xn == x0 && yn == y0 && buttons0 == buttonsn)
    {
        return; // 如果鼠标状态未改变，则直接返回
    }

    // 清除旧的鼠标光标
    clrmous(x0, y0);
    // 保存新位置的背景
    save_bk_mou(*nx, *ny);
    // 绘制新的鼠标光标
    drawmous(*nx, *ny);
}

// 保存鼠标背景
void save_bk_mou(int nx, int ny)
{
    int size; // 背景图像大小
    // 计算鼠标光标区域的大小
    size = imagesize(nx - 1, ny - 2, nx + 11, ny + 17);
    buffer = malloc(size); // 分配内存用于保存背景
    if (buffer != NULL)
    {
        getimage(nx - 1, ny - 2, nx + 11, ny + 17, buffer); // 保存鼠标光标区域的背景
    }
    else
    {
        printf("Error: Unable to allocate memory for mouse background.\n");
    }
}

// 清除鼠标光标
void clrmous(int nx, int ny)
{
    if (flag == 1)
    {
        setwritemode(XOR_PUT);                      // 设置写模式为异或模式
        mouse(nx, ny);                              // 绘制鼠标光标（异或模式下会清除光标）
        putimage(nx - 1, ny - 2, buffer, COPY_PUT); // 恢复背景
        free(buffer);                               // 释放背景内存
        flag = 0;                                   // 标志设置为0，表示鼠标光标已清除
        setwritemode(COPY_PUT);                     // 恢复默认写模式
    }
}

// 清除鼠标状态
void clearEvents()
{
    union REGS inregs, outregs; // 定义寄存器结构
    // 调用鼠标中断 0x33，功能号 0x0A：清除鼠标事件队列
    inregs.h.ah = 0x0A;
    int86(0x33, &inregs, &outregs); // 调用中断
}

// 绘制鼠标光标
void drawmous(int nx, int ny)
{
    if (flag == 0)
    {
        setwritemode(COPY_PUT); // 设置写模式为复制模式
        mouse(nx, ny);          // 绘制鼠标光标
        flag = 1;               // 标志设置为1，表示鼠标光标已绘制
    }
}

// 检测鼠标是否在指定区域内点击
int mouse_press(int x1, int y1, int x2, int y2)
{
    // 如果鼠标在区域内且左键按下
    if (MouseX > x1 && MouseX < x2 && MouseY > y1 && MouseY < y2 && press == 1)
    {
        return 1; // 返回1表示左键点击
    }
    // 如果鼠标在区域内但未按下
    else if (MouseX > x1 && MouseX < x2 && MouseY > y1 && MouseY < y2 && press == 0)
    {
        return 2; // 返回2表示鼠标悬停
    }
    // 如果鼠标在区域内且右键按下
    else if (MouseX > x1 && MouseX < x2 && MouseY > y1 && MouseY < y2 && press == 2)
    {
        return 3; // 返回3表示右键点击
    }
    // 如果鼠标不在区域内
    else
    {
        return 0; // 返回0表示无操作
    }
}