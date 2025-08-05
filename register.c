#include "register.h"

/*****************************************
login.c
COPYRIGHT:	ChenSihao
FUNCTION:	main
ABSTRACT:   进行注册界面的绘制，并在这个界面跳转到 欢迎界面 和 登录界面
DATE:2024/1/24
******************************************/

int registerPage()
{
    Box accountBox, passwordBox1, passwordBox2;
    Button registerButton, cancelButton;
    int accountLen = 0, passwordLen1 = 0, passwordLen2 = 0;
    User user = {{0}, {0}, {0}};

    drawPage_register(&accountBox, &passwordBox1, &passwordBox2, &registerButton, &cancelButton);

    // 等待用户输入
    while (1)
    {
        int nx, ny, nbuttons;
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        delay(30);

        if (nbuttons == 1)
        {
            // 用户名输入框被点击
            if (nx >= accountBox.x && nx <= accountBox.x + accountBox.width &&
                ny >= accountBox.y && ny <= accountBox.y + accountBox.height)
            {
                delay(30);
                putText(&accountBox, user.account, &accountLen);
            }
            // 密码输入框1被点击
            else if (nx >= passwordBox1.x && nx <= passwordBox1.x + passwordBox1.width &&
                     ny >= passwordBox1.y && ny <= passwordBox1.y + passwordBox1.height)
            {
                delay(30);
                putPassword(&passwordBox1, user.password1, &passwordLen1);
            }
            // 密码输入框2被点击
            else if (nx >= passwordBox2.x && nx <= passwordBox2.x + passwordBox2.width &&
                     ny >= passwordBox2.y && ny <= passwordBox2.y + passwordBox2.height)
            {
                delay(30);
                putPassword(&passwordBox2, user.password2, &passwordLen2);
            }
            // 注册按钮被点击
            else if (nx >= registerButton.x && nx <= registerButton.x + registerButton.width &&
                     ny >= registerButton.y && ny <= registerButton.y + registerButton.height)
            {
                // 检查密码是否一致
                delay(400);
                if (passwordLen1 == 0 && passwordLen2 == 0 || accountLen == 0)
                {
                    cleardevice();
                    puthz(150, 200, "账号密码不能为空！", 32, 40, RED);
                    delay(1000);
                    return 2; // 注册成功，跳转登录页面
                }
                else if (strcmp(user.password1, user.password2) == 0)
                {
                    usersave(user); // 储存用户信息

                    cleardevice();
                    puthz(170, 200, "注册成功！", 32, 40, RED);
                    delay(1000);
                    return 1; // 注册成功，跳转登录页面
                }
                else
                {
                    cleardevice();
                    puthz(170, 200, "密码输入不一致！", 32, 40, RED);
                    delay(1000);
                    return 2; // 注册成功，跳转登录页面
                }
            }
            // 取消按钮被点击
            else if (nx >= cancelButton.x && nx <= cancelButton.x + cancelButton.width &&
                     ny >= cancelButton.y && ny <= cancelButton.y + cancelButton.height)
            {
                delay(400);
                return 0; // 返回欢迎页面
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

    // 销毁输入框和按钮
    destroyBox(&accountBox);
    destroyBox(&passwordBox1);
    destroyBox(&passwordBox2);
    destroyButton(&registerButton);
    destroyButton(&cancelButton);

    return 0;
}

void drawPage_register(Box *accountBox, Box *passwordBox1, Box *passwordBox2, Button *registerButton, Button *cancelButton)
{
    int nx, ny, nbuttons;
    // 获取鼠标位置并清除光标
    mread(&nx, &ny, &nbuttons);
    clrmous(nx, ny);

    // 绘制背景
    setbkcolor(WHITE);
    cleardevice();

    // 显示注册信息
    setcolor(RED);
    settextstyle(8, 0, 5);
    outtextxy(200, 30, "Register");

    // 初始化输入框
    settextstyle(8, 0, 4);
    initBox(accountBox, 100, 100, 370, 80, "Account");
    initBox(passwordBox1, 100, 210, 370, 80, "Password");
    initBox(passwordBox2, 100, 320, 370, 80, "Confirm Password");

    // 初始化按钮
    initButton(registerButton, 510, 330, 100, 50, "注册");
    initButton(cancelButton, 510, 400, 100, 50, "取消");

    // 绘制输入框和按钮
    drawBox(*accountBox);
    drawBox(*passwordBox1);
    drawBox(*passwordBox2);
    drawButton(*registerButton);
    drawButton(*cancelButton);
}