/* storyIn.c */
#include "storyIn.h"

// 全局组件方便绘制
InputBox nameBox, traitBox, locationBox;
DropDown genreBox;
Button generateBox, exitBox;

// 初始化下拉框
void InitDropDown(DropDown *dropDown, int x, int y)
{
    dropDown->x = x;
    dropDown->y = y;
    dropDown->width = 150;
    dropDown->height = 25;
    strcpy(dropDown->options[0], "科幻图景");
    strcpy(dropDown->options[1], "奇幻冒险");
    strcpy(dropDown->options[2], "都市迷域");
    strcpy(dropDown->options[3], "玄幻异界");
    strcpy(dropDown->options[4], "悬疑迷雾");
    dropDown->selected = 0;
    dropDown->isOpen = 0;
}

// 绘制下拉框
void DrawDropDown(DropDown *dropDown)
{
    int i;
    int yPos;
    // 绘制主框
    if (dropDown->x + dropDown->width > 633 || dropDown->y + dropDown->height > 473)
    {
        dropDown->x = 633 - dropDown->width - 5;
        dropDown->y = 473 - dropDown->height - 5;
    }
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(dropDown->x, dropDown->y, dropDown->x + dropDown->width, dropDown->y + dropDown->height);
    setcolor(DARKGRAY);
    rectangle(dropDown->x, dropDown->y, dropDown->x + dropDown->width, dropDown->y + dropDown->height);

    // 显示选中项
    puthz(dropDown->x + 5, dropDown->y + 4, dropDown->options[dropDown->selected], 16, 15, BLACK);

    // 绘制箭头
    line(dropDown->x + dropDown->width - 15, dropDown->y + 10,
         dropDown->x + dropDown->width - 10, dropDown->y + 15);
    line(dropDown->x + dropDown->width - 10, dropDown->y + 15,
         dropDown->x + dropDown->width - 5, dropDown->y + 10);

    // 展开选项
    if (dropDown->isOpen)
    {
        int totalHeight = 25 * 5;
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(dropDown->x, dropDown->y + dropDown->height,
            dropDown->x + dropDown->width, dropDown->y + dropDown->height + totalHeight);
        setcolor(DARKGRAY);
        rectangle(dropDown->x, dropDown->y + dropDown->height,
                  dropDown->x + dropDown->width, dropDown->y + dropDown->height + totalHeight);

        for (i = 0; i < 5; i++)
        {
            yPos = dropDown->y + dropDown->height + i * 25;
            if (i == dropDown->selected)
            {
                setfillstyle(SOLID_FILL, BLUE);
                bar(dropDown->x, yPos, dropDown->x + dropDown->width, yPos + 25);
                puthz(dropDown->x + 5, yPos + 4, dropDown->options[i], 16, 32, WHITE);
            }
            else
            {
                puthz(dropDown->x + 5, yPos + 4, dropDown->options[i], 16, 32, WHITE);
            }
        }
    }
}

// 绘制界面元素
void DrawInterface()
{
    // 标题居中计算修正
    int titleX = 210;
    int titleY = 60;

    // 组件基准坐标使用百分比
    int baseX = 96;  // 15%宽度
    int baseY = 115; // 20%高度
    int i, y;

    // 标签对齐
    char *labels[] = {"主角名称", "角色性格", "故事地点", "生图类型"};

    // 绘制羊皮纸背景
    setfillstyle(SOLID_FILL, YELLOW);
    bar(50, 50, 640 - 50, 480 - 50);
    setcolor(DARKGRAY);
    rectangle(49, 49, 640 - 51, 480 - 51);

    
    // 强制恢复左对齐+顶对齐
    settextjustify(LEFT_TEXT, TOP_TEXT);  
    // 添加英文装饰
    settextstyle(3, 0, 6);
    setcolor(WHITE);
    outtextxy(320 - 80, 20, "Story Weaver");
    setcolor(GREEN);
    outtextxy(320 - 78, 18, "Story Weaver");

    // 标题居中
    puthz(titleX, titleY, "故事生图器", 32, 38, DARKGRAY);
    setcolor(RED);
    settextstyle(8, 0, 3);
    for (i = 0; i < 4; i++)
    {
        y = baseY + i * 70;
        puthz(baseX, y, labels[i], 16, 16, RED);
        outtextxy(baseX + 4 * 16 + 5, y - 13, ":"); // 精确计算冒号位置
    }

    // 绘制组件
    DrawInputBox(&nameBox);
    DrawInputBox(&traitBox);
    DrawInputBox(&locationBox);
    drawButton(generateBox);
    drawButton(exitBox);
    DrawDropDown(&genreBox);
}

// 处理组件交互
Bool HandleComponents(int nx, int ny, int nbuttons)
{
    Bool changed = False;
    static int lastSelected = -1;
    static char lastContent[3][256] = {0};
    static int isArrow = 0;
    int i;
    int optionY;
    // 处理下拉框
    if (nbuttons == 1)
    {
        if (nx > genreBox.x && nx < genreBox.x + genreBox.width &&
            ny > genreBox.y && ny < genreBox.y + genreBox.height)
        {
            isArrow = 1;
        }
        if (genreBox.isOpen)
        {
            for (i = 0; i < 5; i++)
            {
                optionY = genreBox.y + genreBox.height + i * 25;
                if (ny > optionY && ny < optionY + 25)
                {
                    genreBox.selected = i;
                    changed = True;
                    genreBox.isOpen = 0;
                }
            }
        }
    }
    else if (nbuttons == 0 && isArrow)
    {
        isArrow = 0;
        if (nx > genreBox.x && nx < genreBox.x + genreBox.width &&
            ny > genreBox.y && ny < genreBox.y + genreBox.height)
        {
            genreBox.isOpen = !genreBox.isOpen;
            changed = True;
        }
    }

    // 处理输入变化
    if (strcmp(nameBox.content, lastContent[0]) != 0)
    {
        strcpy(lastContent[0], nameBox.content);
        changed = True;
    }
    if (strcmp(traitBox.content, lastContent[1]) != 0)
    {
        strcpy(lastContent[1], traitBox.content);
        changed = True;
    }
    if (strcmp(locationBox.content, lastContent[2]) != 0)
    {
        strcpy(lastContent[2], locationBox.content);
        changed = True;
    }
    return changed;
}

// 判断输入内容是否是GBK编码
int isGBK(const char *str)
{
    int i = 0;
    while (str[i])
    {
        if ((unsigned char)str[i] >= 0xB0)
        { // GBK汉字
            if (!str[i + 1] || (unsigned char)str[i + 1] < 0xA1)
                return 0;
            i += 2;
        }
        else
        { // 非汉字字符
            return 0;
        }
    }
    return 1;
}

// 在traits.txt中寻找“personality”是否存在
int findTraits(const char *trait, char *code)
{
    FILE *fp = NULL;
    char path[128];
    int i;
    const char *dirs[] = {"00", "01", "10", "11", "20", "21", "30", "31", "40", "41"};

    for (i = 0; i < 10; i++)
    {
        sprintf(path, "STORY\\traits\\%s\\traits.txt", dirs[i]);
        fp = fopen(path, "r");
        if (fp)
        {
            char line[256];
            while (fgets(line, sizeof(line), fp))
            {
                // 去除掉每行行末的换行符方便匹配
                line[strcspn(line, "\r\n")] = '\0';
                // 精确匹配整行内容
                if (strcmp(line, trait) == 0)
                {
                    strcpy(code, dirs[i]);
                    fclose(fp);
                    return 1;
                }
            }
            fclose(fp);
        }
    }
    return 0;
}

void showError(const char *msg)
{
    int left = 120, top = 140;     // 窗口位置
    int width = 400, height = 200; // 窗口尺寸
    int titleHeight = 30;          // 标题栏高度

    // 绘制窗口阴影
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(left + 5, top + 5, left + width + 5, top + height + 5);

    // 标题栏
    if (strcmp(msg, "正在为您生成故事") == 0)
    {
        // 主窗口背景
        setfillstyle(SOLID_FILL, LIGHTGREEN);
        bar(left, top, left + width, top + height);

        // 窗口边框
        setcolor(LIGHTGRAY);
        rectangle(left, top, left + width, top + height);

        setfillstyle(SOLID_FILL, WATERBLUE);
        bar(left + 1, top + 1, left + width - 1, top + titleHeight);
        setcolor(WHITE);
        puthz(left + width / 2 - 55, top + 4, "通过检测", 24, 26, BLUE);
    }
    else
    {
        // 主窗口背景
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(left, top, left + width, top + height);

        // 窗口边框
        setcolor(DARKBLUE);
        rectangle(left, top, left + width, top + height);

        setfillstyle(SOLID_FILL, WATERBLUE);
        bar(left + 1, top + 1, left + width - 1, top + titleHeight);
        setcolor(WHITE);
        puthz(left + width / 2 - 55, top + 4, "错误提示", 24, 26, BLUE);
        // 警告图标（16x16）
        setcolor(YELLOW);
        circle(left + 70, top + titleHeight + 50, 12);
        line(left + 70, top + titleHeight + 45, left + 70, top + titleHeight + 52);
        line(left + 70, top + titleHeight + 54, left + 70, top + titleHeight + 55);
    }

    // 消息正文
    puthz(left + 100, top + titleHeight + 38, msg, 24, 24, RED);

    // 分隔线
    setcolor(DARKGRAY);
    line(left + 20, top + height - 50, left + width - 20, top + height - 50);

    // 提示文字
    setcolor((strcmp(msg, "正在为您生成故事") == 0) ? DARKGRAY : WHITE);
    puthz(left + width / 2 - 80, top + height - 35, "按任意键继续", 16, 20, (strcmp(msg, "正在为您生成故事") == 0) ? DARKGRAY : WHITE);
    outtextxy(left + width / 2 - 80 + 120, top + height - 50, "......");
    // 等待按键
    getch();
}

// 故事输入页面
int storyInPage(storyInput *userInput)
{
    InputState input, tmp;
    int nx, ny, nbuttons;
    int isError;
    int isGenerate = 0;
    Bool needRedraw = True;
    int key;
    setbkcolor(WHITE);
    // 初始化输入框位置
    CreateInputBox(&nameBox, 183, 107, 300, 30);
    CreateInputBox(&traitBox, 183, 107 + 70, 300, 30);
    CreateInputBox(&locationBox, 183, 107 + 140, 300, 30);
    InitDropDown(&genreBox, 183, 107 + 210);
    initButton(&generateBox, 460, 395, 100, 50, "生成");
    initButton(&exitBox, 80, 395, 100, 50, "退出");
    // 初始化输入法
    IME_Init(&input);

    cleardevice();
    DrawInterface();

    while (1)
    {
        // 处理鼠标事件
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        
        if (needRedraw)
        {
            // 先清除旧鼠标图形
            clrmous(nx, ny);
            cleardevice();
            // 完整绘制界面
            DrawInterface();
            // 保存当前鼠标位置的新背景
            save_bk_mou(nx, ny);
            // 绘制新鼠标图形
            drawmous(nx, ny);
            needRedraw = False;
        }
        if (nbuttons == 1)
        {
            isGenerate = 0;
            // 点击了生成按钮，则鼠标抬起时开始生成故事
            if (nx >= generateBox.x && ny >= generateBox.y &&
                nx <= generateBox.x + generateBox.width &&
                ny <= generateBox.y + generateBox.height)
            {
                // 点击了生成按钮，那么就开始生成故事
                isGenerate = 1;
                // needRedraw = True;
            }
            // 点击了退出按钮，则退出程序
            else if (nx >= exitBox.x && nx <= exitBox.x + exitBox.width &&
                     ny >= exitBox.y && ny <= exitBox.y + exitBox.height)
            {
                // 立即退出程序
                DestroyInputBox(&nameBox);
                DestroyInputBox(&traitBox);
                DestroyInputBox(&locationBox);
                destroyButton(&generateBox);
                destroyButton(&exitBox);
                return 3;
            }
        }
        else if (nbuttons == 0)
        {
            // 判断是否开始生成故事
            if (isGenerate)
            {
                char traitCode[3] = {0};
                char tmp[3] = {0};
                int storyNum;
                isGenerate = 0;
                if (nameBox.content[0] == '\0' || traitBox.content[0] == '\0' || locationBox.content[0] == '\0')
                {
                    showError("输入不可以为空哦");
                    needRedraw = True;
                    continue;
                }

                // 验证GBK
                if (!isGBK(nameBox.content) || !isGBK(traitBox.content) || !isGBK(locationBox.content))
                {
                    showError("输入必须为纯中文");
                    needRedraw = True;
                    continue;
                }

                // 查找性格编码
                if (!findTraits(traitBox.content, traitCode))
                {
                    showError("请勿输入无效性格");
                    needRedraw = True;
                    continue;
                }
                showError("正在为您生成故事");
                // 组装参数
                strncpy(userInput->mainName, nameBox.content, 31);
                strncpy(userInput->personality, traitCode, 3);
                userInput->personality[2] = '\0'; // 确保终止符
                strncpy(userInput->location, locationBox.content, 31);
                userInput->genre = genreBox.selected + 'a'; // 获取故事储存方式：a科幻,b冒险,c都市,d玄幻,e悬疑
                userInput->gender = rand() % 2;

                DestroyInputBox(&nameBox);
                DestroyInputBox(&traitBox);
                DestroyInputBox(&locationBox);
                destroyButton(&generateBox);
                return 5;
            }
        }

        // 处理输入框事件
        clrmous(nx, ny);
        isError = handleInputBox(&nameBox, &input, nx, ny, nbuttons);
        if (isError == 1)
        {
            needRedraw = True;
        }
        else if (isError == -1)
        {
            showError("输入超过十四字节");
            delay(1000);
            needRedraw = True;
            continue;
        }
        isError = handleInputBox(&traitBox, &input, nx, ny, nbuttons);
        if (isError == 1)
        {
            needRedraw = True;
        }
        else if (isError == -1)
        {
            showError("输入超过十四字节");
            delay(1000);
            needRedraw = True;
            continue;
        }
        isError = handleInputBox(&locationBox, &input, nx, ny, nbuttons);
        if (isError == 1)
        {
            needRedraw = True;
        }
        else if (isError == -1)
        {
            showError("输入超过十四字节");
            delay(1000);
            needRedraw = True;
            continue;
        }
        save_bk_mou(nx,ny);
        drawmous(nx, ny);
        // 处理下拉框事件
        if (HandleComponents(nx, ny, nbuttons))
        {
            needRedraw = True;
        }
        delay(50); // 优化刷新率
    }
}