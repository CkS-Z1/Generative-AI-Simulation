/* input.c */
#include "input.h"

// 常量定义
#define CANDIDATE_MARGIN 8       // 候选词边距
#define CANDIDATE_ARROW_SIZE 16  // 翻页箭头大小
#define CANDIDATE_ITEM_HEIGHT 32 // 候选词项高度
#define CANDIDATE_ITEM_WIDTH 50  // 候选词项宽度
#define CANDIDATE_NUM 5          // 每页显示候选词数量
#define PINYIN_DIR "PINYIN\\"    // 拼音词库文件路径

// 函数声明
static void handleInput(InputState *state);                  // 处理键盘输入
static void handleMouse(InputState *state);                  // 处理鼠标事件
static void drawCandidates(InputState *state);               // 绘制候选词窗口
static void getCandidates(InputState *state);                // 获取候选词
static int getCharWidth(char c);                             // 获取字符显示宽度
static int calculateWidth(const char *str, int n);           // 计算字符串显示宽度
static void strinsert(char *dest, const char *src, int pos); // 在指定位置插入字符串

// 初始化函数
void IME_Init(InputState *state)
{
    memset(state, 0, sizeof(InputState));
    state->totalCandidates = 0;
    state->isPinyin = 0;
    state->candidateBackground = NULL;
}

// 更新函数
IME_Result IME_Update(InputState *state, int cursorX, int cursorY)
{
    IME_Result result = {0};
    static char *prevBg = NULL;               // 保存背景图像的缓存
    static int prevWidth = 0, prevHeight = 0; // 保存上一次的候选窗口尺寸
    int i, start = state->currentPage * CANDIDATE_NUM;
    int frameWidth = 0, frameHeight = 0;
    int validCount = 0;
    int isPosChanged, isSizeChanged;
    int candidateSize;
    int nx, ny, nbuttons;
    mread(&MouseX, &MouseY, &MouseS);
    clrmous(nx, ny);

    // 恢复背景图像
    if (state->candidateBackground)
    {
        putimage(state->candidateBgX, state->candidateBgY, state->candidateBackground, COPY_PUT);
    }

    // 设置候选窗口位置
    state->candidatePos.x = cursorX;
    state->candidatePos.y = cursorY + 32;

    // 计算候选窗口尺寸
    for (i = 0; i < CANDIDATE_NUM; i++)
    {
        if (state->candidate[start + i][0])
            validCount++;
    }

    if (validCount > 0)
    {
        frameWidth = 2 * CANDIDATE_MARGIN + 2 * CANDIDATE_ARROW_SIZE + validCount * CANDIDATE_ITEM_WIDTH;
        frameHeight = CANDIDATE_ITEM_HEIGHT + CANDIDATE_MARGIN * 2;
    }

    // 调整候选窗口位置
    if (state->candidatePos.x + frameWidth > SCREEN_WIDTH - 15)
    {
        state->candidatePos.x = SCREEN_WIDTH - frameWidth - 15;
    }
    if (state->candidatePos.y + frameHeight > SCREEN_HEIGHT - 15)
    {
        state->candidatePos.y = SCREEN_HEIGHT - frameHeight - 15;
    }

    handleMouse(state);
    // 处理鼠标和键盘事件
    if (kbhit())
    {
        state->lastKey = getch(); // 记录按键值
        handleInput(state);       // 内部处理输入逻辑
        while (kbhit())
            getch(); // 清空缓冲区
    }
    else
    {
        state->lastKey = 0; // 无按键时重置
    }

    // 检测位置或尺寸是否变化
    isPosChanged = (state->candidateBgX != state->candidatePos.x ||
                    state->candidateBgY != state->candidatePos.y);
    isSizeChanged = (frameWidth != prevWidth || frameHeight != prevHeight);

    if (isPosChanged || isSizeChanged)
    {
        // 保存背景图像
        if (isSizeChanged && prevBg != NULL)
        { // 如果候选框尺寸发生过变化或者有未释放的内存，那么释放内存
            free(prevBg);
            prevBg = NULL;
        }
        // 计算候选窗口尺寸
        frameWidth = 2 * CANDIDATE_MARGIN + 2 * CANDIDATE_ARROW_SIZE + CANDIDATE_NUM * CANDIDATE_ITEM_WIDTH;
        // 获取候选窗口区域的图像
        candidateSize = imagesize(state->candidatePos.x, state->candidatePos.y,
                                  state->candidatePos.x + frameWidth,
                                  state->candidatePos.y + frameHeight);
        if (candidateSize > 0)
        {
            if (!prevBg || candidateSize != prevWidth * prevHeight)
            {                                           // 如果存在未释放的背景，或者尺寸发生变化
                prevBg = (char *)malloc(candidateSize); // 则重新分配内存
            }
            if (prevBg)
            {
                getimage(state->candidatePos.x, state->candidatePos.y,
                         state->candidatePos.x + frameWidth,
                         state->candidatePos.y + frameHeight, prevBg); // 保存背景
                state->candidateBackground = prevBg;                   // 背景指针储存
                state->candidateBgX = state->candidatePos.x;           // 背景X位置储存
                state->candidateBgY = state->candidatePos.y;           // 背景Y位置储存
                prevWidth = frameWidth;                                // 背景宽度储存
                prevHeight = frameHeight;                              // 背景长度储存
            }
        }
    }

    // 绘制候选窗口
    if (frameWidth > 0 && frameHeight > 0)
    {
        drawCandidates(state);
    }

    // 返回输入结果
    if (state->output.content[0])
    {
        strcpy(result.content, state->output.content);
        result.width = state->output.width;
        memset(&state->output, 0, sizeof(state->output));
    }

    return result;
}

// 创建输入框
void CreateInputBox(InputBox *box, int x, int y, int width, int height)
{
    memset(box, 0, sizeof(InputBox));
    box->x = x;
    box->y = y;
    box->width = width;
    box->height = height;
    box->bgColor = LIGHTGRAY;
    box->borderColor = DARKGRAY;
    box->textColor = WHITE;
}

// 绘制输入框
int DrawInputBox(InputBox *box)
{
    int textX = box->x + 5;
    int textY;                      // 文本Y坐标
    int maxWidth = box->width - 10; // 可用宽度
    int currentWidth = 0;
    const char *p = box->content;
    char ch[2] = {0}, hz[3] = {0};

    // 绘制背景
    setfillstyle(SOLID_FILL, box->bgColor);
    bar(box->x, box->y, box->x + box->width, box->y + box->height);

    // 绘制边框
    setcolor(box->borderColor);
    rectangle(box->x, box->y, box->x + box->width, box->y + box->height);

    // 绘制"[中]"提示
    if (box->width >= 10 && box->height >= 16)
    {
        setcolor(RED);
        settextstyle(4, 0, 2);
        outtextxy(box->x + 5, box->y, "[");
        puthz(box->x + 12, box->y + 7, "中", 16, 2, RED);
        outtextxy(box->x + 30, box->y, "]");
    }

    textX = box->x + 40;

    while (*p && currentWidth < maxWidth)
    {
        int charWidth;
        if ((unsigned char)*p >= 0xB0)
        { // 中文字符
            textY = box->y + (box->height - 16) / 2;
            hz[0] = *p;
            hz[1] = *(p + 1);
            hz[2] = '\0';
            if (currentWidth + 18 > maxWidth)
            {
                break;
            }
            puthz(textX, textY, hz, 16, 2, box->textColor);
            textX += 18;
            currentWidth += 18;
            p += 2;
        }
        else
        { // ASCII字符
            textY = box->y;
            charWidth = getCharWidth(*p);
            if (currentWidth + charWidth > maxWidth)
            {
                break;
            }
            settextstyle(6, 0, 2);
            ch[0] = *p;
            ch[1] = '\0';
            setcolor(box->textColor);
            outtextxy(textX, textY, ch);

            textX += charWidth + 5;
            currentWidth += charWidth + 5;
            p++;
        }
    }

    // 绘制光标
    if (box->isActive)
    {
        int cursorX = textX + 5;
        setcolor(DARKGRAY);
        textY = box->y + (box->height - 16) / 2;
        line(cursorX, textY, cursorX, textY + 16);
    }
    return textX + 5;
}

// 处理输入框事件
int handleInputBox(InputBox *box, InputState *state, int nx, int ny, int nbuttons)
{
    int isInsideBox = 0;
    int isInCandidateArea = 0;
    int frameWidth, frameHeight;
    int candidateX1, candidateY1, candidateX2, candidateY2;
    // mread(&nx, &ny, &nbuttons);
    if (nx >= box->x + 20 && nx <= box->x + box->width &&
        ny >= box->y && ny <= box->y + box->height)
    {
        isInsideBox = 1;
    }

    // 计算候选框区域
    if (state->totalCandidates > 0)
    {
        frameWidth = 2 * CANDIDATE_MARGIN + 2 * CANDIDATE_ARROW_SIZE +
                     CANDIDATE_NUM * CANDIDATE_ITEM_WIDTH;
        frameHeight = CANDIDATE_ITEM_HEIGHT + CANDIDATE_MARGIN;

        candidateX1 = state->candidatePos.x;
        candidateY1 = state->candidatePos.y;
        candidateX2 = candidateX1 + frameWidth;
        candidateY2 = candidateY1 + frameHeight;

        isInCandidateArea = (nx >= candidateX1 && nx <= candidateX2 &&
                             ny >= candidateY1 && ny <= candidateY2);
    }

    if (nbuttons == 1)
    {
        if (isInsideBox)
        {
            if (!box->isActive)
            {
                box->isActive = 1;
                return 1;
            }
        }
        else
        {
            // 只有当点击既不在输入框也不在候选框时才取消激活
            if (!isInCandidateArea && box->isActive)
            {
                memset(state->pinyin, 0, sizeof(state->pinyin));       // 清空拼音缓冲区
                memset(state->candidate, 0, sizeof(state->candidate)); // 清空候选词列表
                state->currentPage = 0;                                // 重置当前页码
                state->totalCandidates = 0;                            // 重置候选词总数
                state->isPinyin = 0;                                   // 重置拼音模式标志
                box->isActive = 0;
                return 1;
            }
        }
    }

    if (box->isActive)
    {
        IME_Result result = IME_Update(state, box->x + 5 + calculateWidth(box->content, box->cursorPos), box->y - 6);
        if (result.content[0])
        {
            int current_len = strlen(box->content);
            int insert_len = strlen(result.content);
            if (current_len + insert_len <= 14)
            { // 总长度不超过14字节
                strinsert(box->content, result.content, box->cursorPos);
                box->cursorPos += insert_len;
            }
            else
            {
                return -1;
            }
        }
        if (state->lastKey)
        {
            static int canDelete = 0;
            if (state->lastKey == 13)
            { // Enter 键
                box->isActive = 0;
                return 1;
            }

            if (state->lastKey == '\b' && box->cursorPos > 0 && canDelete)
            {
                int isChinese = 0, len;
                if (box->cursorPos >= 2)
                {
                    unsigned char prev1 = box->content[box->cursorPos - 1];
                    unsigned char prev2 = box->content[box->cursorPos - 2];
                    if (prev2 >= 0xB0 && prev2 <= 0xF7 && prev1 >= 0xA1)
                    {
                        isChinese = 1;
                    }
                }
                len = isChinese ? 2 : 1;
                if (len == 1)
                {
                    box->content[box->cursorPos - 1] = '\0';
                }
                if (len == 2)
                {
                    box->content[box->cursorPos - 1] = '\0';
                    box->content[box->cursorPos - 2] = '\0';
                }
                box->cursorPos -= len;
                return 1;
            }

            if (!state->pinyin[0])
            {
                canDelete = 1;
            }
            else
            {
                canDelete = 0;
            }
        }
    }
    return 0;
}

// 销毁输入框
void DestroyInputBox(InputBox *box)
{
    memset(box->content, 0, sizeof(box->content));
    box->cursorPos = 0;
    box->isActive = 0;
}

// 处理键盘输入
static void handleInput(InputState *state)
{
    int key, isNum;
    int tmp[2];

    key = state->lastKey; // 直接使用已存储的按键值
    isNum = key - '0';

    if (key == '\b')
    {
        if (strlen(state->pinyin) > 0)
        {
            state->pinyin[strlen(state->pinyin) - 1] = '\0';
            getCandidates(state);
        }
    }
    else if (isalpha(key))
    {
        if ('a' <= key <= 'z')
        {
            key = key;
        }
        else if ('A' <= key <= 'Z')
        {
            key = key - 'A' + 'a';
        }
        if (!state->isPinyin)
        {
            memset(state->candidate, 0, sizeof(state->candidate));
            state->totalCandidates = 0;
        }
        state->isPinyin = 1;
        strncat(state->pinyin, (char *)&key, 1);
        getCandidates(state);
    }
    else if (isdigit(key))
    {
        int idx = key - '1' + state->currentPage * CANDIDATE_NUM;
        if (isNum >= 6)
        {
            if (!state->pinyin[0])
            {
                tmp[0] = key;
                tmp[1] = 0;
                strcat(state->pinyin, tmp);
                strcpy(state->output.content, state->pinyin);
                state->output.width = calculateWidth(state->pinyin, -1);
                memset(state->pinyin, 0, sizeof(state->pinyin));
                memset(state->candidate, 0, sizeof(state->candidate));
                state->totalCandidates = 0;
            }
        }
        else if (state->isPinyin && key == '1')
        {
            strcpy(state->output.content, state->pinyin);
            state->output.width = calculateWidth(state->pinyin, -1);
            memset(state->pinyin, 0, sizeof(state->pinyin));
            memset(state->candidate, 0, sizeof(state->candidate));
        }
        else if (!state->isPinyin && idx < state->totalCandidates && state->candidate[idx][0])
        {
            strcpy(state->output.content, state->candidate[idx]);
            state->output.width = calculateWidth(state->candidate[idx], -1);
            memset(state->pinyin, 0, sizeof(state->pinyin));
            memset(state->candidate, 0, sizeof(state->candidate));
        }
        else if (!state->pinyin[0])
        {
            tmp[0] = key;
            tmp[1] = 0;
            strcat(state->pinyin, tmp);
            strcpy(state->output.content, state->pinyin);
            state->output.width = calculateWidth(state->pinyin, -1);
            memset(state->pinyin, 0, sizeof(state->pinyin));
            memset(state->candidate, 0, sizeof(state->candidate));
            state->totalCandidates = 0;
        }
    }
    else if (key == ' ' || key == '\r')
    {
        if (state->totalCandidates > 0)
        {
            int firstIdx = state->currentPage * CANDIDATE_NUM;
            if (firstIdx < state->totalCandidates && state->candidate[firstIdx][0])
            {
                strcpy(state->output.content, state->candidate[firstIdx]);
                state->output.width = calculateWidth(state->candidate[firstIdx], -1);
                memset(state->pinyin, 0, sizeof(state->pinyin));
                memset(state->candidate, 0, sizeof(state->candidate));
                state->totalCandidates = 0;
                state->currentPage = 0;
                state->isPinyin = 0;
            }
        }
    }
    else if (key == '+' || key == '=')
    {
        if ((state->currentPage + 1) * CANDIDATE_NUM < state->totalCandidates)
        {
            state->currentPage++;
        }
    }
    else if (key == '-')
    {
        if (state->currentPage > 0)
        {
            state->currentPage--;
        }
    }
    else
    {
        if (strlen(state->pinyin) >= 0)
        {
            tmp[0] = key;
            tmp[1] = 0;
            strcat(state->pinyin, tmp);
            strcpy(state->output.content, state->pinyin);
            state->output.width = calculateWidth(state->pinyin, -1);
            memset(state->pinyin, 0, sizeof(state->pinyin));
            memset(state->candidate, 0, sizeof(state->candidate));
            state->totalCandidates = 0;
        }
    }
}

// 绘制候选词窗口
static void drawCandidates(InputState *state)
{
    int start = state->currentPage * CANDIDATE_NUM;
    int nx, ny, nbuttons;
    int frameWidth, frameHeight, frameX, frameY;
    int arrowY;
    int itemStartX, itemY;
    int validCount = 0;
    int i;

    for (i = 0; i < CANDIDATE_NUM; i++)
    {
        if (state->candidate[start + i][0])
        {
            validCount++;
        }
    }
    if (validCount == 0)
    {
        return;
    }

    if (state->isPinyin)
    {
        frameWidth = 2 * CANDIDATE_MARGIN + strlen(state->pinyin) * 12 + 2 * CANDIDATE_MARGIN;
        frameHeight = CANDIDATE_ITEM_HEIGHT + 2 * CANDIDATE_MARGIN;
    }
    else
    {
        frameWidth = 2 * CANDIDATE_MARGIN + 2 * CANDIDATE_ARROW_SIZE +
                     validCount * CANDIDATE_ITEM_WIDTH;
        frameHeight = CANDIDATE_ITEM_HEIGHT + CANDIDATE_MARGIN * 2;
    }

    if (state->candidatePos.x + frameWidth > SCREEN_WIDTH)
    {
        state->candidatePos.x = SCREEN_WIDTH - frameWidth;
    }
    if (state->candidatePos.y + frameHeight > SCREEN_HEIGHT)
    {
        state->candidatePos.y = SCREEN_HEIGHT - frameHeight;
    }

    frameX = state->candidatePos.x;
    frameY = state->candidatePos.y;
    setfillstyle(SOLID_FILL, WHITE);
    bar(frameX, frameY, frameX + frameWidth, frameY + frameHeight);
    setcolor(DARKBLUE);
    rectangle(frameX, frameY, frameX + frameWidth, frameY + frameHeight);

    if (!state->isPinyin)
    {
        arrowY = frameY + (frameHeight - CANDIDATE_ARROW_SIZE) / 3 - 1;

        if (state->currentPage > 0)
        {
            setfillstyle(SOLID_FILL, state->leftArrowDown ? DARKGRAY : LIGHTGRAY);
            bar(frameX + CANDIDATE_MARGIN - 4, frameY + CANDIDATE_MARGIN + (state->leftArrowDown ? 2 : 0),
                frameX + CANDIDATE_MARGIN + CANDIDATE_ARROW_SIZE - 2,
                frameY + CANDIDATE_MARGIN + CANDIDATE_ITEM_HEIGHT + (state->leftArrowDown ? 2 : 0));
            setcolor(state->leftArrowDown ? WHITE : DARKGRAY);
            outtextxy(frameX + CANDIDATE_MARGIN - 1,
                      arrowY + (state->leftArrowDown ? 2 : 0),
                      "<");
        }

        if ((state->currentPage + 1) * CANDIDATE_NUM < state->totalCandidates)
        {
            setfillstyle(SOLID_FILL, state->rightArrowDown ? DARKGRAY : LIGHTGRAY);
            bar(frameX + frameWidth - CANDIDATE_ARROW_SIZE - CANDIDATE_MARGIN + 2, frameY + CANDIDATE_MARGIN + (state->rightArrowDown ? 2 : 0),
                frameX + frameWidth - CANDIDATE_MARGIN + 4,
                frameY + CANDIDATE_MARGIN + CANDIDATE_ITEM_HEIGHT + (state->rightArrowDown ? 2 : 0));

            setcolor(state->rightArrowDown ? WHITE : DARKGRAY);
            outtextxy(frameX + frameWidth - CANDIDATE_ARROW_SIZE - CANDIDATE_MARGIN + 7,
                      arrowY + (state->rightArrowDown ? 2 : 0),
                      ">");
        }
    }

    if (state->isPinyin)
    {
        int x = frameX + CANDIDATE_MARGIN;
        int y = frameY + CANDIDATE_MARGIN + 4;
        setcolor(DARKGRAY);
        for (i = 0; i < strlen(state->pinyin); i++)
        {
            char *c = state->candidate[start + i];
            int wordWidth = calculateWidth(c, -1);
            outtextxy(x, y, c);
            x += wordWidth + 5;
        }
    }
    else
    {
        itemStartX = frameX + CANDIDATE_MARGIN + CANDIDATE_ARROW_SIZE;
        itemY = frameY + CANDIDATE_MARGIN;
        mread(&nx, &ny, &nbuttons);

        for (i = 0; i < validCount; i++)
        {
            char num[4];
            int itemX = itemStartX + i * CANDIDATE_ITEM_WIDTH;
            int itemRight = itemX + CANDIDATE_ITEM_WIDTH;
            int itemBottom = itemY + CANDIDATE_ITEM_HEIGHT;
            int COLOR_HZ;

            int isHover = (nx >= itemX && nx <= itemRight &&
                           ny >= itemY && ny <= itemBottom);

            if (i == 0)
            {
                setfillstyle(SOLID_FILL, isHover ? LIGHTBLUE : DARKBLUE);
                bar(itemX, itemY, itemRight, itemBottom);
                COLOR_HZ = isHover ? DARKGRAY : WHITE;
                setcolor(COLOR_HZ);
            }
            else
            {
                setfillstyle(SOLID_FILL, isHover ? WATERBLUE : WHITE);
                bar(itemX, itemY, itemRight, itemBottom);
                COLOR_HZ = DARKGRAY;
                setcolor(COLOR_HZ);
            }
            if (isHover == 1 && nbuttons == 1)
            {
                state->output.width = calculateWidth(state->candidate[start + i], -1);
                strcpy(state->output.content, state->candidate[start + i]);
                memset(state->pinyin, 0, sizeof(state->pinyin));
                memset(state->candidate, 0, sizeof(state->candidate));
            }

            settextstyle(6, 0, 2);
            sprintf(num, "%d.", i + 1);
            outtextxy(itemX + 5, itemY + 4, num);
            puthz(itemX + 25, itemY + 4,
                  state->candidate[start + i],
                  16, 2, COLOR_HZ);
        }
    }
}

// 获取候选词
static void getCandidates(InputState *state)
{
    char path[128], line[256];
    FILE *fp;
    int len, i, idx;

    sprintf(path, "%s%s.txt", PINYIN_DIR, state->pinyin);
    state->currentPage = 0;
    if ((fp = fopen(path, "rb")) == NULL)
    {
        int i, idx;
        int pinyin_len = strlen(state->pinyin);
        memset(state->candidate, 0, sizeof(state->candidate));

        for (i = 0, idx = 0; i < pinyin_len && idx < pinyin_len; i++, idx++)
        {
            state->candidate[idx][0] = state->pinyin[i];
            state->candidate[idx][1] = '\0';
        }
        state->totalCandidates = idx;
        state->isPinyin = 1;
        return;
    }

    len = fread(line, 1, sizeof(line) - 1, fp);
    fclose(fp);
    line[len] = '\0';
    state->isPinyin = 0;

    memset(state->candidate, 0, sizeof(state->candidate));
    idx = 0;
    for (i = 0; i < len;)
    {
        if ((unsigned char)line[i] >= 0xB0 &&
            (unsigned char)line[i] <= 0xF7)
        {
            if (i + 1 < len && (unsigned char)line[i + 1] >= 0xA1)
            {
                memcpy(state->candidate[idx], &line[i], 2);
                idx++;
                i += 2;
            }
            else
            {
                i++;
            }
        }
        else
        {
            state->candidate[idx][0] = line[i];
            state->candidate[idx][1] = '\0';
            idx++;
            i++;
        }
    }
    state->totalCandidates = idx;
}

// 获取字符宽度
static int getCharWidth(char c)
{
    static const unsigned char widths[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             // 0-15
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             // 16-31
        4, 4, 6, 8, 8, 8, 8, 4, 5, 5, 8, 8, 4, 8, 4, 8,             // 32-47
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 8, 8, 8, 8,             // 48-63
        8, 10, 10, 10, 10, 10, 10, 10, 10, 4, 8, 10, 8, 13, 11, 10, // 64-79
        10, 10, 10, 10, 10, 10, 10, 12, 10, 10, 10, 5, 8, 5, 8, 8,  // 80-95
        4, 8, 8, 8, 8, 8, 5, 8, 8, 4, 4, 8, 4, 13, 9, 8,            // 96-111
        8, 8, 5, 8, 5, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 0             // 112-127
    };

    if ((unsigned char)c >= 128)
    {
        return 16;
    }
    return widths[(unsigned char)c];
}

// 计算字符串宽度
static int calculateWidth(const char *str, int n)
{
    int width = 0;
    const char *p = str;
    int count = 0;

    while (*p && (n == -1 || count < n))
    {
        if ((unsigned char)*p >= 0xB0 && (unsigned char)*p <= 0xF7 &&
            *(p + 1) &&
            (unsigned char)*(p + 1) >= 0xA1 && (unsigned char)*(p + 1) <= 0xFE)
        {
            width += 16;
            p += 2;
            count++;
        }
        else
        {
            width += getCharWidth(*p);
            p += 1;
            count++;
        }
    }
    return width;
}

// 在指定位置插入字符串
static void strinsert(char *dest, const char *src, int pos)
{
    int destLen = strlen(dest);
    int srcLen = 0;
    const char *p = src;

    srcLen = strlen(src);

    if (pos > 0)
    {
        unsigned char prev = dest[pos - 1];
        if (prev >= 0xB0 && prev <= 0xF7 && dest[pos] >= 0xA1)
        {
            pos--;
        }
    }

    memmove(dest + pos + srcLen, dest + pos, destLen - pos + 1);
    memcpy(dest + pos, src, srcLen);
}

// 处理鼠标事件
static void handleMouse(InputState *state)
{
    int nx, ny, nbuttons;
    static int arrowPressed = 0;
    mread(&nx, &ny, &nbuttons);

    if (nbuttons == 1 && state->pinyin[0])
    {
        int start = state->currentPage * CANDIDATE_NUM;
        int validCount = 0;
        int frameX, frameY, frameWidth;
        int i;

        state->rightArrowDown = 0;
        state->leftArrowDown = 0;

        for (i = 0; i < CANDIDATE_NUM; i++)
        {
            if (state->candidate[start + i][0])
            {
                validCount++;
            }
        }
        if (validCount == 0)
        {
            return;
        }
        frameX = state->candidatePos.x;
        frameY = state->candidatePos.y;
        frameWidth = CANDIDATE_MARGIN * 2 + CANDIDATE_ARROW_SIZE * 2 +
                     validCount * CANDIDATE_ITEM_WIDTH;

        if (nx >= frameX && nx <= frameX + CANDIDATE_MARGIN + CANDIDATE_ARROW_SIZE &&
            ny >= frameY && ny <= frameY + (CANDIDATE_ITEM_HEIGHT + CANDIDATE_MARGIN * 2))
        {
            state->leftArrowDown = 1;
            arrowPressed = 1;
        }
        else if (nx >= frameX + frameWidth - CANDIDATE_ARROW_SIZE - CANDIDATE_MARGIN &&
                 nx <= frameX + frameWidth &&
                 ny >= frameY && ny <= frameY + (CANDIDATE_ITEM_HEIGHT + 2 * CANDIDATE_MARGIN))
        {
            state->rightArrowDown = 1;
            arrowPressed = 2;
        }
    }
    else if (nbuttons == 0)
    {
        if (arrowPressed)
        {
            if (arrowPressed == 1 && state->leftArrowDown)
            {
                state->currentPage--;
                if (state->currentPage < 0)
                    state->currentPage = 0;
            }
            else if (arrowPressed == 2 && state->rightArrowDown)
            {
                if ((state->currentPage + 1) * CANDIDATE_NUM < state->totalCandidates)
                {
                    state->currentPage++;
                }
            }
            arrowPressed = 0;
            state->leftArrowDown = 0;
            state->rightArrowDown = 0;
        }
    }
}