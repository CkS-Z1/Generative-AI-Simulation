#include "welcome.h"

/*****************************************
welcome.c
COPYRIGHT:	ChenSihao
FUNCTION:	main
ABSTRACT:   进行欢迎界面的绘制，并在这个界面跳转到 注册界面 和 登陆界面
DATE:2024/1/18
******************************************/

// 显示欢迎页面
int welcomePage()
{
    Button button[3];
    int flag;
    int i = 0;
    // 绘制背景
    setbkcolor(WHITE); // 设置背景颜色为白色
    cleardevice();     // 清除屏幕内容

    // 显示欢迎信息
    setcolor(RED);                                   // 设置文字颜色为黑色
    settextstyle(8, 0, 3);                           // 设置字体样式和大小
    outtextxy(140, 100, "Welcome to Generative AI"); // 英文欢迎信息

    // 使用汉字显示功能显示中文信息
    puthz(185, 150, "欢迎使用生成式", 24, 30, RED); // 24x24点阵汉字
    outtextxy(398, 143, "AI");                      // AI

    // 三个按钮，登录，注册，退出，应该在同一行，即x间隔，y相同
    // 按钮序号，x位置，y位置，宽度---，高度|，按钮名称
    initButton(&button[0], 100, 250, 100, 50, "登录");
    initButton(&button[1], 250, 250, 100, 50, "注册");
    initButton(&button[2], 400, 250, 100, 50, "退出");
    for (i = 0; i < 3; i++)
    {
        drawButton(button[i]);
        destroyButton(&button[i]);
    }

    while (1)
    {
        int nx, ny, nbuttons;
        newmouse(&nx, &ny, &nbuttons);
        delay(30);
        mread(&MouseX, &MouseY, &MouseS); // 获取鼠标位置和状态
        if (nbuttons == 1)
        {
            // 登录按钮被点击
            if (nx >= 100 && nx <= 200 && ny >= 250 && ny <= 300)
            {
                return BUTTON_LOGIN;
                // return 3;
            }
            // 注册按钮被点击
            else if (nx >= 250 && nx <= 350 && ny >= 250 && ny <= 300)
            {
                return BUTTON_REGISTER;
            }
            // 退出按钮被点击
            else if (nx >= 400 && nx <= 500 && ny >= 250 && ny <= 300)
            {
                return BUTTON_EXIT;
            }
        }
        if (kbhit())
        {                      // 检测是否有按键按下
            int key = getch(); // 获取按键
            if (key == 27)
            { // Esc键的ASCII码为27
                printf("程序已终止，返回代码编辑页面。\n");
                break; // 退出主循环
            }
        }
    }
}