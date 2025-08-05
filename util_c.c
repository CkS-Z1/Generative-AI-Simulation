#include "util_c.h"

char folderpath[50];

// 初始化按钮
void initTrigger(Trigger *trigger, int x, int y, int width, int height, char *text)
{
    memset(trigger, 0, sizeof(Trigger));
    trigger->x = x;
    trigger->y = y;
    trigger->width = width;
    trigger->height = height;
    trigger->text = text;
}

// 绘制按钮
void drawTrigger(Trigger trigger, int zifu)
{
    setfillstyle(1, LIGHTGRAY);
    bar(trigger.x, trigger.y, trigger.x + trigger.width, trigger.y + trigger.height);
    setcolor(BROWN);
    rectangle(trigger.x, trigger.y, trigger.x + trigger.width, trigger.y + trigger.height);
    if (zifu == HANZI)
    {
        puthz(trigger.x + trigger.width / 2 - 24, trigger.y + 15, trigger.text, 24, 24, WHITE);
    }
    else
    {
        settextstyle(8, 0, 1); 
        outtextxy(trigger.x + trigger.width / 2 - 8, trigger.y + trigger.height / 2 - 15, trigger.text);
    }
}

// 消息预处理
void Msg_preprocessing(Body *body, int y, char *text, int duixiang)
{
    int i, j;
    int char_count = 0;
    char *ptr = NULL;
    body->message.message = malloc(strlen(text) + 1); // 分配内存存储文本内容
    if (body->message.message == NULL)
    {
        cleardevice();
        outtextxy(10, 10, "Error code 0010.Press any key to exit.");
        getch();
        exit(1);
    }
    strcpy(body->message.message, text); // 复制文本内容

    ptr = body->message.message; // 指向消息内容的指针
    // 计算消息的字数
    while (*ptr != '\0')
    {
        if ((unsigned char)*ptr >= 0xA1 && (unsigned char)*ptr <= 0xFE)
        {
            // 汉字占两个字节
            char_count++;
            ptr += 2;
        }
        else
        {
            // 英文或数字占一个字节
            char_count++;
            ptr++;
        }
    }
    body->length = char_count; // 将字数存储到 body->length

    body->row = (body->length + MAX_LINE_WORDS - 1) / MAX_LINE_WORDS;
    body->height = body->row * LINE_HEIGHT;
    // 计算消息框宽度
    if (body->length <= MAX_LINE_WORDS) // 单行消息
    {
        body->width = CHAR_WIDTH * body->length;
    }
    else // 多行消息
    {
        body->width = MAX_FRAME_WIDTH;
    }
    // 设置x坐标
    if (duixiang == yonghu) // 用户输入消息
    {
        body->x = YOUX - body->width;
    }
    else if (duixiang == AI) // AI输出消息
    {
        body->x = ZUOX;
    }

    body->y = y;
    body->bottom = y + body->height;
    body->duixiang = duixiang;
}

// 释放内存
void freeBody(Body *body)
{
    free(body->message.message);
    body->message.message = NULL;
}

// 绘制新消息
void drawNewMessage(Body *body)
{
    int color;
    if (body->duixiang == yonghu)
    {
        color = GREEN;
    }
    else
    {
        color = WHITE;
    }
    settextstyle(8, 0, 1);
    // 绘制文本和背景
    setfillstyle(1,color);
    bar(body->x,body->y,body->x+body->width,body->y+body->height);
    setcolor(WHITE);
    rectangle(body->x, body->y, body->x + body->width, body->y + body->height);
    drawMsgTxt(body->x, body->y, body->message.message, CHAR_SPACING_OFFSET, DARKGRAY, MAX_LINE_WORDS);
}

// 添加新消息
void addMessage(Body *body, int *y, int *page_int, int *maxpage)
{
    // 屏幕没有填满时
    if (body->bottom <= XIAY)
    {
        drawNewMessage(body);
    }
    // 屏幕被填满时
    else
    {
        // 向后翻页
        FlipPage(PAGE_UP, page_int, y, maxpage); // 传入folderpath参数
        *y = SHANGY;                             // 重置y坐标
        body->y = *y;                            // 更新消息的y坐标
        body->bottom = *y + body->height;        // 更新消息的底部坐标
        drawNewMessage(body);                    // 绘制新消息
    }
}

// 显示错误信息提示
void showErrorPrompt(const char *Prompt)
{
    const char *Img = "IMAGE\\tip.dat";
    // 保存屏幕
    Save_image(148, 158, 494, 304, Img);

    setcolor(RED);
    setlinestyle(0, 0, 3);
    rectangle(150, 160, 490, 300);
    setfillstyle(1, WHITE);
    bar(150, 160, 490, 300);
    drawMsgTxt(218, 200, Prompt, 16, RED, 10); // 绘制提示文本
    delay(500);

    Load_image(148, 158, 494, 304, Img);
}

// 清除消息区域
void clearWindow()
{
    setfillstyle(1, WHITE);
    bar(ZUOX, SHANGY, YOUX, XIAY);
}

// 绘制消息文本
void drawMsgTxt(int x, int y, char *s, int part, int color, int words)
{
    FILE *hzk_p = NULL;                                                      // 定义汉字库文件指针
    unsigned char quma, weima;                                               // 定义汉字的区码和位码
    unsigned long offset;                                                    // 定义汉字在字库中的偏移量
    unsigned char mask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; // 功能数组，用于显示汉字点阵中的亮点
    int i, j, pos;
    char temp[2] = {0};

    char mat[32]; // 16*16的汉字需要32个字节的数组来存储
    int y0 = y;
    int x0 = x;
    int count = 0;                    // 用于计数每行输出的字符数量
    hzk_p = fopen("HZK\\HZ16", "rb"); // 使用相对路径

    if (hzk_p == NULL)
    {
        settextjustify(LEFT_TEXT, TOP_TEXT);     // 左部对齐，顶部对齐
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1); // 黑体笔划输出，水平输出，24*24点阵
        outtextxy(10, 10, "Can't open hzk16 file!Press any key to quit...");
        getch();
        exit(1);
    }

    while (*s != '\0')
    {
        // 每行最多输出20个字符，或者x坐标超出屏幕宽度
        while (count < words && *s != '\0' && x < 640 - 16)
        {
            y = y0;

            if ((unsigned char)*s >= 0xA1 && (unsigned char)*s <= 0xFE) // 判断是否为汉字
            {
                quma = s[0] - 0xA0;                             // 求出区码
                weima = s[1] - 0xA0;                            // 求出位码
                offset = (94 * (quma - 1) + (weima - 1)) * 32L; // 计算偏移量
                fseek(hzk_p, offset, SEEK_SET);                 // 重定位文件指针
                fread(mat, 32, 1, hzk_p);                       // 读取点阵数据

                for (i = 0; i < 16; i++)
                {
                    pos = 2 * i; // 每行有2个字节
                    for (j = 0; j < 16; j++)
                    {
                        if ((mask[j % 8] & mat[pos + j / 8]) != 0)
                        {
                            putpixel(x + j, y, color);
                        }
                    }
                    y++;
                }

                x += part; // 水平方向偏移16个像素
                s += 2;    // 汉字内码占2个字节
            }
            else // 英文或数字
            {
                setcolor(color);
                temp[0] = *s;               // 取出当前字符
                temp[1] = '\0';             // 字符串结束符
                outtextxy(x, y - 12, temp); // 调整 y 坐标，+4 是为了对齐中文
                x += part;                  // 水平方向偏移8个像素
                s++;                        // 英文或数字占1个字节
            }

            count++; // 计数器加1
        }

        // 重置x坐标，增加y坐标，重置计数器
        x = x0;
        y0 += 16;  // 换行，增加垂直方向偏移
        count = 0; // 重置计数器
    }
    fclose(hzk_p); // 关闭文件
    hzk_p = NULL;  // 将指针置为NULL，避免悬空指针
}

// 将消息以GBK编码形式存储进入文件
void saveMsg(const char *filename, char *message)
{
    static int Msg_count = 1; // 消息计数器
    FILE *file = fopen(filename, "ab");
    if (file == NULL)
    {
        cleardevice();
        outtextxy(10, 10, "Error code 0021.Press any key to exit.");
        getch();
        exit(1);
    }

    // 写入字符串
    fprintf(file, "%04d%s\n", Msg_count, message); // 添加换行符，每一条消息占一行
    fclose(file);

    Msg_count++; // 增加消息计数器
}

// 读取文件中的消息
void readMsg(const char *filename, Msg *msg)
{
    FILE *file = fopen(filename, "rb");
    char buffer[256];
    if (file == NULL)
    {
        cleardevice();
        outtextxy(10, 10, "Error code 0023. Press any key to exit.");
        getch();
        exit(1);
    }

    if (fgets(buffer, sizeof(buffer), file) != NULL) // 读取一行
    {
        sscanf(buffer, "%04d%[^\n]", &msg->num, msg->message); // 分离编号和消息
    }
    else
    {
        msg->num = -1; // 如果读取失败，设置编号为 -1 表示无效
        strcpy(msg->message, "");
    }

    fclose(file);
}

// 保存矩形图像
void Save_image(int x1, int y1, int x2, int y2, char *filepath)
{
    int i = 0, j = 0;
    int height = y2 - y1, width = x2 - x1;
    int color = 0;
    FILE *fp = NULL;

    fp = fopen(filepath, "wb");
    if (fp == NULL)
    {
        perror("无法打开该 image 文件");
        exit(1);
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            color = getpixel(x1 + j, y1 + i);   // 使用 getpixel 获取像素颜色
            fwrite(&color, sizeof(int), 1, fp); // 写入文件
        }
    }
    fclose(fp);
}

// 加载矩形图像
void Load_image(int x1, int y1, int x2, int y2, char *filepath)
{
    int i = 0, j = 0;
    int height = y2 - y1, width = x2 - x1;
    int color = 0;
    FILE *fp = NULL;

    fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        perror("无该 image 存储路径");
        exit(1);
    }
    else
    {
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                fread(&color, sizeof(int), 1, fp); // 从文件读取颜色
                putpixel(x1 + j, y1 + i, color);   // 使用 putpixel 设置像素颜色
            }
        }
        fclose(fp);
    }
}

// 翻页函数
void FlipPage(int flip, int *page_count, int *cur_y, int *max_page)
{
    char target_page_name[10];
    char target_page_path[50];
    int exists;
    char *page_str;
    const char *default_path = "IMAGE\\BLANK.DAT";

    if (*page_count + flip < 1)
    {
        showErrorPrompt("已经是第一页");
        return;
    }

    // 转换页码为字符串
    itoa(*page_count + flip, target_page_name, 10);

    // 处理目标页
    strcpy(target_page_path, folderpath);
    strcat(target_page_path, "Img_");
    strcat(target_page_path, target_page_name);
    strcat(target_page_path, ".dat");

    // 检查目标页是否存在
    exists = fileExists(target_page_path);
    if (!exists)
    {
        // 检查默认路径长度是否超出缓冲区大小
        if (strlen(default_path) >= sizeof(target_page_path))
        {
            // 处理路径过长的情况
            showErrorPrompt("默认页面路径过长");
            exit(1);
        }
        strcpy(target_page_path, default_path);
        *cur_y = SHANGY;
    }
    // 清除窗口并加载新页
    clearWindow();
    Load_image(ZUOX, SHANGY, YOUX, XIAY, target_page_path);

    // 更新页码
    *page_count += flip;
    if (*max_page < *page_count)
    {
        *max_page = *page_count;
    }

    // 更新页码显示
    itoa(*page_count, page_str, 10);
    Load_image (620, 370, 700, 420, "IMAGE\\pagenum.dat");
    
    setcolor(WHITE);
    settextstyle(8, 0, 1);
    outtextxy(620, 395, page_str);

    /* 恢复目标页面状态 */
    cur_y = SHANGY;
}

// 保存聊天记录
void save_history(const char *folderpath, int pagecount, int y)
{
    char *page_char[4];
    char current_page_path[50];
    char cur_y[5];
    char yf_path[50];
    FILE *yf = NULL;

    itoa(pagecount, page_char, 10);
    strcpy(current_page_path, folderpath);
    strcat(current_page_path, "Img_");
    strcat(current_page_path, page_char);
    strcat(current_page_path, ".dat");
    Save_image(ZUOX, SHANGY, YOUX, XIAY, current_page_path);

    itoa(y, cur_y, 10);
    strcpy(yf_path, folderpath);
    strcat(yf_path, "Y.txt");
    yf = fopen(yf_path, "wt");
    fprintf(yf, cur_y);
    fclose(yf);
}

// 检查文件是否存在
int fileExists(const char *filepath)
{
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        // 文件不存在
        return 0;
    }
    else
    {
        // 文件存在
        fclose(fp);
        return 1;
    }
}

// 绘制当前页的文件夹列表和翻页按钮
void drawCurrentPage(int x1, int y1, int x2, int y2, Trigger HisTrigger[],
                     char foldernames[][50], int current_page, int foldercount, Trigger *PrevBtn, Trigger *NextBtn)
{
    int i;
    int display_count = (foldercount - current_page * 8) > 8 ? 8 : (foldercount - current_page * 8);

    // 绘制菜单区域
    setfillstyle(1, WHITE);
    bar(x1, y1, x2, y2); // 清除窗口
    setcolor(BLUE);
    rectangle(x1, y1, x2, y2); // 绘制边框

    // 绘制当前页的文件夹按钮
    for (i = 0; i < display_count; i++)
    {
        initTrigger(&HisTrigger[i], x1, y1 + i * 50, 213, 40, foldernames[current_page * 8 + i]);
        drawTrigger(HisTrigger[i], FUHAO);
    }

    // 绘制翻页按钮
    if (current_page > 0)
    {
        drawTrigger(*PrevBtn, HANZI); // 绘制"上一页"按钮
    }
    if (foldercount > (current_page + 1) * 8)
    {
        drawTrigger(*NextBtn, HANZI); // 绘制"下一页"按钮
    }
}

// 处理菜单键点击
void handleMenuClick(int *cur_y)
{
    // 必须在函数开头定义所有变量
    int x1 = 0, y1 = 20, x2 = 213, y2 = 460;
    int nx, ny, nbuttons;
    int i = 0;
    Trigger HisTrigger[8];
    Trigger PrevBtn, NextBtn;
    char foldernames[16][50];
    int foldercount = listFolders("HISTORY", foldernames, 16);
    int current_page = 0;
    const char *Imgf = "IMAGE\\menu.dat";
    char selected_folder[100];
    int clicked = 0; // 新增标志变量

    newmouse(&nx, &ny, &nbuttons);
    clrmous(nx, ny);
    delay(30);

    // 保存被菜单区域遮挡的图像
    Save_image(x1, y1 - 2, x2 + 2, y2 + 2, Imgf);

    // 初始化翻页按钮
    initTrigger(&PrevBtn, x1, y2 - 40, 100, 40, "上一页");
    initTrigger(&NextBtn, x1 + 113, y2 - 40, 100, 40, "下一页");

    // 初始绘制第一页
    drawCurrentPage(x1, y1, x2, y2, HisTrigger, foldernames, current_page, foldercount, &PrevBtn, &NextBtn);

    while (!clicked) // 修改循环条件
    {
        newmouse(&nx, &ny, &nbuttons);

        if (nbuttons == 1)
        {
            clrmous(nx, ny);

            // 检查是否点击了历史记录按钮
            for (i = 0; i < 8 && i < foldercount - current_page * 8; i++)
            {
                if (nx >= HisTrigger[i].x && nx <= HisTrigger[i].x + HisTrigger[i].width &&
                    ny >= HisTrigger[i].y && ny <= HisTrigger[i].y + HisTrigger[i].height)
                {
                    // 构建选中文件夹的完整路径
                    strcpy(selected_folder, "HISTORY\\");
                    strcat(selected_folder, foldernames[current_page * 8 + i]);
                    strcat(selected_folder, "\\");

                    clicked = 1; // 设置标志位
                    break;       // 退出循环
                }
            }

            if (clicked)
            {
                break; // 如果点击了历史记录，退出主循环
            }
            // 检查是否点击了"上一页"按钮
            if (current_page > 0 && nx >= PrevBtn.x && nx <= PrevBtn.x + PrevBtn.width &&
                ny >= PrevBtn.y && ny <= PrevBtn.y + PrevBtn.height)
            {
                current_page--; // 切换到上一页
                drawCurrentPage(x1, y1, x2, y2, HisTrigger, foldernames, current_page, foldercount, &PrevBtn, &NextBtn);
                continue;
            }

            // 检查是否点击了"下一页"按钮
            if (foldercount > (current_page + 1) * 8 && nx >= NextBtn.x && nx <= NextBtn.x + NextBtn.width &&
                ny >= NextBtn.y && ny <= NextBtn.y + NextBtn.height)
            {
                current_page++; // 切换到下一页
                drawCurrentPage(x1, y1, x2, y2, HisTrigger, foldernames, current_page, foldercount, &PrevBtn, &NextBtn);
                continue;
            }

            // 鼠标在菜单区域外
            if (nx < x1 || nx > x2 || ny < y1 || ny > y2)
            {
                break;
            }
        }
    }

    // 先恢复屏幕（关闭菜单）
    Load_image(x1, y1 - 2, x2 + 2, y2 + 2, Imgf);

    // 再加载选中的历史记录（如果有）
    if (clicked)
    {
        loadHistory(selected_folder, cur_y);
        strcpy(folderpath, selected_folder);
        current_page = 0;
    }
}

// 生成时间戳
void generateTimestamp(char *timestamp, size_t size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // 格式化时间戳为 "YYYYMMDD_HHMMSS"
    strftime(timestamp, size, "%Y%m%d%H%M%S", t);
}

// 列出指定目录下的所有文件夹，并存储到数组中
int listFolders(const char *folderPath, char folderNames[][50], int maxFolders)
{
    struct dirent *entry;
    DIR *dp = opendir(folderPath); // 打开目录
    int count = 0;                 // 文件计数器

    if (dp == NULL)
    {
        showErrorPrompt("目录不存在");
        return 0;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        // 排除 "." 和 ".." 目录
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            if (count < maxFolders)
            {
                strncpy(folderNames[count], entry->d_name, 50); // 复制文件名
                folderNames[count][49] = '\0';                  // 确保字符串以 '\0' 结尾
                count++;
            }
            else
            {
                showErrorPrompt("文件数量超标");
                break;
            }
        }
    }

    closedir(dp); // 关闭目录
    return count; // 返回文件数量
}

// 加载历史记录函数 - 只处理图像文件
void loadHistory(const char *folderpath, int *y)
{
    // 定义变量
    char max_page_file[40]; // 存储最大页码文件名
    char temp_file[40];
    int max_page = 0;
    int current_page;
    struct dirent *entry;
    DIR *dp;
    char page_str[10];
    char cur_y[5];
    FILE *yf = NULL;
    char yfpath[50];

    // 查找最大的页码图像文件
    dp = opendir(folderpath);
    if (dp)
    {
        while ((entry = readdir(dp)) != NULL)
        {
            // 检查是否是Img_开头的.dat文件
            if (strstr(entry->d_name, "IMG_") == entry->d_name &&
                strstr(entry->d_name, ".DAT") != NULL)
            {
                // 提取页码数字
                sscanf(entry->d_name, "IMG_%d.DAT", &current_page);
                if (current_page > max_page)
                {
                    max_page = current_page;
                    // 构建完整文件路径
                    strcpy(temp_file, folderpath);
                    strcat(temp_file, entry->d_name);
                }
            }
        }
        closedir(dp);
    }

    // 读取当前消息的cur_y
    sprintf(yfpath, "%sY.TXT", folderpath);
    yf = fopen(yfpath, "rt");
    if (fgets(cur_y, sizeof(cur_y), yf) != NULL)
    {
        // 将字符串转换为整数
        *y = atoi(cur_y);
    }
    fclose(yf);

    // 加载最大页码的图像
    if (max_page > 0)
    {
        // 清空当前窗口
        clearWindow();
        delay(100);
        // 加载图像
        Load_image(ZUOX, SHANGY, YOUX, XIAY, temp_file);
        delay(100);
        // 更新页码显示
        itoa(max_page, page_str, 10);
        setfillstyle(1, WHITE);
        bar(620, 370, 700, 420);
        setcolor(BLUE);
        settextstyle(8, 0, 1);
        outtextxy(620, 395, page_str);
    }
    else
    {
        showErrorPrompt("未找到历史记录图像");
    }
}
