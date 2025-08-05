/* output.c */
#include "output.h"

static int current_x = START_X;
static int current_y = START_Y;
static int current_page = 0;
static int max_page = 0;
static char page_content[MAX_CONTENT] = {0};

static void drawPageButtons();
static int checkButtonClick();
static void saveCurrentPage();
static int loadPage(int page_num);
static void pageTurn(int direction);
static void round_rect(int x1, int y1, int x2, int y2);

static void round_rect(int x1, int y1, int x2, int y2)
{
    // 绘制四角圆弧
    arc(x1 + BUTTON_RADIUS, y1 + BUTTON_RADIUS, 90, 180, BUTTON_RADIUS);
    arc(x2 - BUTTON_RADIUS, y1 + BUTTON_RADIUS, 0, 90, BUTTON_RADIUS);
    arc(x2 - BUTTON_RADIUS, y2 - BUTTON_RADIUS, 270, 360, BUTTON_RADIUS);
    arc(x1 + BUTTON_RADIUS, y2 - BUTTON_RADIUS, 180, 270, BUTTON_RADIUS);

    // 连接弧线
    line(x1 + BUTTON_RADIUS, y1, x2 - BUTTON_RADIUS, y1);
    line(x1 + BUTTON_RADIUS, y2, x2 - BUTTON_RADIUS, y2);
    line(x1, y1 + BUTTON_RADIUS, x1, y2 - BUTTON_RADIUS);
    line(x2, y1 + BUTTON_RADIUS, x2, y2 - BUTTON_RADIUS);
}

// 修改后的drawPageButtons
static void drawPageButtons()
{
    /* 清空原按钮区域 */
    setfillstyle(SOLID_FILL, CONTENT_BG);
    bar(0, BUTTON_Y - 10, 640, 480);

    // 绘制统一背景板
    setfillstyle(SOLID_FILL, BUTTON_BG);
    bar(0, BUTTON_Y - 5, 640, BUTTON_Y + BUTTON_HEIGHT + 5);

    // 上一页按钮
    setcolor(BUTTON_BORDER);
    if (current_page > 0)
    {
        setfillstyle(SOLID_FILL, YELLOW);
        round_rect(PREV_BUTTON_X, BUTTON_Y, PREV_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
        floodfill(PREV_BUTTON_X + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, BUTTON_BORDER);

        // 文字阴影效果
        setcolor(DARKGRAY);
        puthz(PREV_BUTTON_X + 12, BUTTON_Y + 10, "上一页", 16, 16, DARKGRAY);
        puthz(PREV_BUTTON_X + 10, BUTTON_Y + 8, "上一页", 16, 16, WHITE);
    }
    else
    {
        // 禁用状态
        setfillstyle(SOLID_FILL, BUTTON_BG);
        bar(PREV_BUTTON_X, BUTTON_Y, PREV_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
    }

    // 下一页按钮（代码类似上一页，省略部分重复内容）
    setcolor(BUTTON_BORDER);
    if (current_page < max_page)
    {
        // 判断鼠标悬停
        int hover = (MouseX >= NEXT_BUTTON_X && MouseX <= NEXT_BUTTON_X + BUTTON_WIDTH &&
                     MouseY >= BUTTON_Y && MouseY <= BUTTON_Y + BUTTON_HEIGHT);

        setfillstyle(SOLID_FILL, YELLOW);
        round_rect(NEXT_BUTTON_X, BUTTON_Y, NEXT_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
        floodfill(NEXT_BUTTON_X + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, BUTTON_BORDER);

        // 文字阴影效果
        setcolor(DARKGRAY);
        puthz(NEXT_BUTTON_X + 12, BUTTON_Y + 10, "下一页", 16, 16, DARKGRAY);
        puthz(NEXT_BUTTON_X + 10, BUTTON_Y + 8, "下一页", 16, 16, WHITE);
    }
    else
    {
        // 禁用状态
        setfillstyle(SOLID_FILL, BUTTON_BG);
        bar(NEXT_BUTTON_X, BUTTON_Y, NEXT_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
    }

    // 文生图按钮（3D效果）
    if (current_page == max_page)
    {
        setcolor(GEN_BUTTON_BG);
        setfillstyle(SOLID_FILL, GEN_BUTTON_BG);
        bar3d(GEN_BUTTON_X, BUTTON_Y, GEN_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT, 3, 1);

        // 渐变文字
        settextstyle(1, 0, 3);
        setcolor(WHITE);
        puthz(GEN_BUTTON_X + 12, BUTTON_Y + 10, "文生图", 16, 16, DARKGRAY);
        puthz(GEN_BUTTON_X + 10, BUTTON_Y + 8, "文生图", 16, 16, WHITE);
    }

    // 添加装饰线
    setcolor(BORDER_COLOR);
    line(0, BUTTON_Y - 10, 640, BUTTON_Y - 10);
}

// 修改后的checkButtonClick
static int checkButtonClick()
{
    static int lastPress = 0;
    int nx, ny, nbuttons;
    int result = 0;
    static int lastHover = 0;
    mread(&nx, &ny, &nbuttons);

    // 检测是否悬停
    if (nbuttons == 0)
    {
        // 三个按钮的Y区域是相同的
        if (ny >= BUTTON_Y && ny <= BUTTON_Y + BUTTON_HEIGHT)
        {
            // 判断是否在X区域内
            if ((nx >= PREV_BUTTON_X && nx <= PREV_BUTTON_X + BUTTON_WIDTH) || (nx >= NEXT_BUTTON_X && nx <= NEXT_BUTTON_X + BUTTON_WIDTH) || (nx >= GEN_BUTTON_X && nx <= GEN_BUTTON_X + BUTTON_WIDTH))
            {
                if (lastHover == 0)
                {
                    drawPageButtons(); // 重绘按钮
                    lastHover = 1;
                }
            }
            else
            {
                if (lastHover == 1)
                {
                    drawPageButtons(); // 重绘按钮
                    lastHover = 0;
                }
            }
        }
    }
    // clrmous(nx, ny);
    if (nbuttons == 0 && lastPress == 1)
    {
        // 文生图按钮检测
        if (nx >= GEN_BUTTON_X && nx <= GEN_BUTTON_X + BUTTON_WIDTH &&
            ny >= BUTTON_Y && ny <= BUTTON_Y + BUTTON_HEIGHT)
        {
            result = 2; // 新返回值表示文生图按钮
        }
        else if (current_page > 0 &&
                 nx >= PREV_BUTTON_X && nx <= PREV_BUTTON_X + BUTTON_WIDTH &&
                 ny >= BUTTON_Y && ny <= BUTTON_Y + BUTTON_HEIGHT)
        {
            result = -1;
        }
        else if (current_page < max_page && nx >= NEXT_BUTTON_X && nx <= NEXT_BUTTON_X + BUTTON_WIDTH &&
                 ny >= BUTTON_Y && ny <= BUTTON_Y + BUTTON_HEIGHT)
        {
            result = 1;
        }
    }

    lastPress = nbuttons;
    return result;
}

// 保存当前页面数据到文件
static void saveCurrentPage()
{
    char filename[MAX_PATH];
    FILE *fp;
    int has_gbk = 0; // 标记是否存在GBK汉字
    int i;

    if (current_page < 0)
    {
        return;
    }

    // 检查是否存在GBK汉字
    for (i = 0; i < strlen(page_content);)
    {
        if ((unsigned char)page_content[i] >= 0x80)
        {
            // 确保有连续两个字节且第二个字节合法
            if (i + 1 < strlen(page_content) &&
                (unsigned char)page_content[i + 1] >= 0x40)
            {
                has_gbk = 1;
                break;
            }
            i += 2; // 跳过双字节
        }
        else
        {
            i++; // ASCII字符
        }
    }

    // 无GBK内容时不保存
    if (!has_gbk)
    {
        return;
    }
    sprintf(filename, "STORY\\DATA\\%d.TXT", current_page);
    fp = fopen(filename, "w");
    if (fp)
    {
        fwrite(page_content, 1, strlen(page_content), fp);
        fclose(fp);
    }
}

// 加载指定页面内容
static int loadPage(int page_num)
{
    char filename[MAX_PATH];
    FILE *fp;
    int result = 0;
    static char line[1024] = {0}; // 假设有合适的行缓冲区定义
    if (page_num < 0)
    {
        return 0;
    }

    sprintf(filename, "STORY\\DATA\\%d.TXT", page_num);
    fp = fopen(filename, "r");

    if (fp)
    {
        cleardevice();
        reset_cursor();
        page_content[0] = '\0'; // 清空内容
        // 直接渲染文件内容，不使用outputText
        current_x = START_X;
        current_y = START_Y + 15;

        if (current_page == 0)
        {
            current_y += PARA_HEIGHT;
        }
        // 绘制内容区背景
        setfillstyle(SOLID_FILL, CONTENT_BG);
        bar(START_X - 10, START_Y - 10, 630, BUTTON_Y - 20);

        // 添加内容区边框
        setcolor(BORDER_COLOR);
        rectangle(START_X - 15, START_Y - 15, 635, BUTTON_Y - 15);
        while (fgets(line, sizeof(line), fp))
        {
            // 逐行直接绘制到屏幕
            char *ptr = line;
            char hanzi[3] = {0}, ascii[2] = {0};
            while (*ptr)
            {
                if ((unsigned char)*ptr >= 0x80)
                {
                    hanzi[0] = ptr[0];
                    hanzi[1] = ptr[1];
                    hanzi[2] = 0;
                    puthz(current_x, current_y, hanzi, 16, 0, DARKGRAY);
                    current_x += 18;
                    ptr += 2;
                }

                // 处理ASCII字符
                else
                {
                    ascii[0] = *ptr;
                    ascii[1] = 0;
                    settextstyle(8, 0, 2);
                    setcolor(DARKGRAY);
                    outtextxy(current_x, current_y - 8, ascii);
                    current_x += 12;
                    ptr++;
                }

                // 检查是否需要换行
                if (current_x + 18 + 15 >= 640 && *ptr)
                {
                    current_x = START_X;
                    current_y += LINE_HEIGHT;
                }
            }
            // 输出完一行以后要换段
            current_x = START_X;
            current_y += PARA_HEIGHT;
        }
        fclose(fp);
        strcpy(page_content, ""); // 或从文件重新加载到page_content
        result = 1;
    }
    else
    {
        outtextxy(300, 240, "FAIL TO OPEN FILE!!!");
    }
    drawPageButtons();
    return result;
}

// 翻页处理
static void pageTurn(int direction)
{
    int buttonNum;
    if (direction == 1)
    { // 下一页
        if (current_page < max_page)
        {
            current_page++;
            if (!loadPage(current_page))
            {
                cleardevice();          // 清除图形设备显示
                page_content[0] = '\0'; // 确保页面内容为空
                reset_cursor();
            }
        }
        else
        {
            // 已经是最后一页
            return;
        }
    }
    else if (direction == -1)
    { // 上一页
        if (current_page >= 1)
        {
            cleardevice(); // 清除图形设备显示
            current_page--;
            loadPage(current_page);
        }
        else
        {
            // 已经是第一页
            return;
        }
    }
}

void reset_cursor()
{
    current_x = START_X;
    current_y = START_Y;
}

void reset_pages()
{
    current_page = 0;
    max_page = 0;
    memset(page_content, 0, MAX_CONTENT);
    reset_cursor(); // 可选，确保光标也重置
}

// 修改函数实时输出文本内容
void outputText(const char *str, int isNewParagraph)
{
    static int len = 0;
    char hanzi[3];
    char ascii[2];
    const char indent[] = "    ";
    // 处理新段落
    if (isNewParagraph)
    {
        current_y += PARA_HEIGHT; // 增加段落高度
        current_x = START_X;

        // 如果页面内容不为空且最后一个字符不是换行符，则手动储存一次换行
        if (strlen(page_content) > 0 && page_content[strlen(page_content) - 1] != '\n' && page_content[strlen(page_content) - 1] != '\r')
        {
            strcat(page_content, "\n");
        }
        // 写入缩进
        strcat(page_content, indent);
        len += strlen(indent);
        outtextxy(current_x, current_y, indent);
        current_x += strlen(indent) * 12;
    }
    // strncat(page_content, str, MAX_CONTENT - strlen(page_content) - 1);

    // 写入逻辑
    while (*str)
    {
        // 检查是否需要翻页
        if (current_y + LINE_HEIGHT + 20 >= 480 - BUTTON_HEIGHT - MARGIN_BOTTOM)
        {
            saveCurrentPage();
            current_page++;
            max_page = current_page;
            // 翻页时，清空page_content
            len = 0;
            memset(page_content, 0, MAX_CONTENT);
            reset_cursor();
            cleardevice();
            drawPageButtons();
        }

        // 处理中文字符
        if ((unsigned char)*str >= 0x80)
        {
            hanzi[0] = str[0];
            hanzi[1] = str[1];
            hanzi[2] = 0;
            page_content[len++] = str[0];
            page_content[len++] = str[1];
            puthz(current_x, current_y, hanzi, 16, 0, DARKGRAY);
            current_x += 18;
            str += 2;
        }

        // 处理ASCII字符
        else
        {
            ascii[0] = *str;
            ascii[1] = 0;
            page_content[len++] = *str;
            settextstyle(8, 0, 2);
            setcolor(DARKGRAY);
            outtextxy(current_x, current_y - 8, ascii);
            current_x += 12;
            str++;
        }

        // 检查是否需要换行
        if (current_x + 18 + 15 >= 640 && *str)
        {
            current_x = START_X;
            current_y += LINE_HEIGHT;
        }
    }

    if (isNewParagraph == 2)
    {
        saveCurrentPage();
        max_page = current_page;
        // 翻页时，清空page_content
        len = 0;
        memset(page_content, 0, MAX_CONTENT);

        reset_cursor();
        cleardevice();
    }
    // 绘制翻页按钮
    drawPageButtons();
}

void show_story()
{
    int exit_flag = 0;
    int nx, ny, nbuttons;
    // getch();
    // saveCurrentPage();
    current_page = 0;
    reset_cursor();

    loadPage(current_page); // 加载第零页
    drawPageButtons();

    while (!exit_flag)
    {
        int buttonNum = checkButtonClick();
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        if (buttonNum == 2 && current_page == max_page)
        { // 在最后一页点击了文生图按钮
            exit_flag = 1;
        }
        else if (buttonNum == 1 || buttonNum == -1)
        {
            pageTurn(buttonNum);
        }
        delay(100);
    }

    current_page = 0;
    max_page = 0;
}