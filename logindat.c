#include "logindat.h"

int loginuser(char *name, char *password) // 用户登录函数
{
    FILE *file;
    int i = 1;
    char path[100] = "USER\\";
    char suffix[100] = ".txt"; // 文件后缀
    char filepassword[100];    // 存储文件中的密码

    strcat(path, name);
    strcat(path, suffix);

    if (strlen(name) == 0) // 用户名为空
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);                        // 设置填充颜色为白色
        bar(150, 160, 490, 300);                       // 绘制白色矩形
        setcolor(RED);                                 // 设置颜色为红色
        setlinestyle(0, 0, 3);                         // 设置线型
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1); // 绘制红色矩形边框
        puthz(218, 200, "请输入用户名", 24, 30, RED);  // 提示输入用户名
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press); // 更新鼠标位置
            delay(1);
        }
        return 1;
    }

    file = fopen(path, "rt"); // 打开文件
    if (file == NULL)         // 文件打开失败
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);
        bar(150, 160, 490, 300);
        setcolor(RED);
        setlinestyle(0, 0, 3);
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);
        puthz(188, 200, "用户不存在", 24, 30, RED); // 提示用户不存在
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press);
            delay(1);
        }
        fclose(file);
        return 2;
    }

    if (strlen(password) == 0) // 密码为空
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);
        bar(150, 160, 490, 300);
        setcolor(RED);
        setlinestyle(0, 0, 3);
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);
        puthz(233, 200, "请输入密码", 24, 30, RED); // 提示输入密码
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press);
            delay(1);
        }
        fclose(file);
        return 2;
    }

    fscanf(file, "%s", filepassword);        // 读取文件中的密码
    if (strcmp(password, filepassword) == 0) // 密码匹配
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);
        bar(150, 160, 490, 300);
        setcolor(RED);
        setlinestyle(0, 0, 3);
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);
        puthz(263, 200, "登录成功", 24, 30, RED); // 提示登录成功
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press);
            delay(1);
        }

        fclose(file); // 关闭文件
        return 3;
    }
    else // 密码不匹配
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);
        bar(150, 160, 490, 300);
        setcolor(RED);
        setlinestyle(0, 0, 3);
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);
        puthz(263, 200, "用户名或密码错误", 36, 30, RED); // 提示用户名或密码错误
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press);
            delay(1);
        }
        fclose(file);
        return 2;
    }
}