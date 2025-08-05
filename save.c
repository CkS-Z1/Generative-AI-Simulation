#include "save.h"
// 用于把用户名和密码存入文件
void usersave(User user)
{
    FILE *file; // 定义文件指针
    int i = 1;
    char path[100] = "USER\\"; // 用户文件存储路径
    char suffix[100] = ".txt"; // 文件后缀
    strcat(path, user.account);
    strcat(path, suffix);

    // 注册新用户
    file = fopen(path, "wt"); // 以只写模式创建文件
    if (file == NULL)
    {
        clrmous(MouseX, MouseY);                        // 清除鼠标事件
        setfillstyle(1, YELLOW);                        // 设置填充样式为黄色
        bar(150, 160, 490, 300);                        // 绘制提示框
        setcolor(BLACK);                                // 设置颜色为黑色
        setlinestyle(0, 0, 3);                          // 设置线条样式
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);  // 绘制提示框边框
        puthz(203, 200, "无法打开文件", 24, 30, BLACK); // 显示错误信息
        return;                                         // 返回
    }
    fprintf(file, "%s", user.password1);

    fclose(file); // 关闭文件
}
