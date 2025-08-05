#include "Chinput.h"

// 绘制输入框
void drawInputBox(char *content)
{
    Trigger leftTrigger;
    Trigger rightTrigger;
    // 初始化按键
    initTrigger(&leftTrigger, x01, y01, w01, h01, "<");
    initTrigger(&rightTrigger, x02, y02, w02, h02, ">");
    // 绘制按键
    drawTrigger(leftTrigger, FUHAO);
    drawTrigger(rightTrigger, FUHAO);

    setfillstyle(SOLID_FILL, WHITE);
    bar(x04, y04, x04 + w04, y04 + h04); // 绘制背景
    setcolor(BLUE);
    setlinestyle(SOLID_LINE, 0, 1);
    rectangle(x04, y04, x04 + w04, y04 + h04); // 绘制边框
    setcolor(DARKGRAY);
    drawMsgTxt(x04, y04 + 5, content, 16, DARKGRAY, 28);
}

// 绘制候选框
void drawCandidateBox(const char candidates[MAX_CANDIDATES][3], int selected, int candidateCount)
{
    int i;
    int color = BLACK;
    int space = 16;
    char s[2];

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x03, y03, x03 + w03, y03 + h03); // 绘制背景
    setcolor(BLUE);
    rectangle(x03, y03, x03 + w03, y03 + h03); // 绘制边框

    for (i = 0; i < candidateCount && i < MAX_CANDIDATES; i++)
    {
        if (i < 9)
        {
            sprintf(s, "%d", i + 1);
            drawMsgTxt(x03 + 25 + i * (CHAR_WIDTH + space), y03 + 10, s, 16, BLACK, MAX_CANDIDATES);
        }
        else
        {
            drawMsgTxt(x03 + 25 + i * (CHAR_WIDTH + space), y03 + 10, "0", 16, BLACK, MAX_CANDIDATES);
        }
        if (candidates[i][0] != '\0')
        {
            if (i == selected)
            {
                color = RED;
            }
            else
            {
                color = BLACK;
            }
            // 调整每个候选汉字的绘制位置为横向排列
            drawMsgTxt(x03 + 25 + 16 + i * (CHAR_WIDTH + space), y03 + 5, candidates[i], 16, color, MAX_CANDIDATES);
        }
    }
}

// 从拼音文件中加载候选汉字
int loadCandidates(const char *pinyin, char candidates[MAX_CANDIDATES][3], int startIndex)
{
    int i = 0;
    char filepath[30];
    FILE *file;
    char line[256]; // 用于存储整行内容
    int count = 0;

    sprintf(filepath, "PINYIN\\%s.txt", pinyin); // 拼接拼音文件路径
    file = fopen(filepath, "r");
    if (file == NULL)
    {
        return 0; // 文件不存在或无法打开
    }

    // 读取整行内容
    if (fgets(line, sizeof(line), file) != NULL)
    {
        for (i = startIndex * 2; i < strlen(line) && count < MAX_CANDIDATES; i += 2)
        {
            if (line[i] == '\n' || line[i] == '\0')
            {
                break; // 遇到换行符或字符串结束符
            }
            candidates[count][0] = line[i];
            candidates[count][1] = line[i + 1];
            candidates[count][2] = '\0'; // 确保字符串以 '\0' 结尾
            count++;
        }
        i = 0;
        // 清空剩余位置
        for (i = count; i < MAX_CANDIDATES; i++)
        {
            candidates[i][0] = '\0';
        }
    }

    fclose(file);
    return count; // 返回候选汉字数量
}

// 主输入法逻辑
void hzinput(char *output)
{
    int nx, ny, nbuttons;
    char input[MAX_INPUT] = {0};                   // 用户输入的拼音
    char candidates[MAX_CANDIDATES][3] = {{'\0'}}; // 候选汉字
    char finalOutput[MAX_INPUT] = {0};             // 最终输出的汉字内容
    int selected = 0;                              // 当前选中的候选汉字
    int inputLen = 0;                              // 当前拼音输入长度
    int outputLen = 0;                             // 最终输出内容长度
    int ch, candidateCount = 0;
    int startIndex = 0; // 候选汉字的起始索引
    int i = 0;
    int charX, charY;
    int needRedraw = 0; // 标记是否需要重绘
    int forceRedraw = 0;
    static int lastPageButton = 0;
    int currentPageButton = 0;
    int tempCount = 0;                      // 临时变量
    char tempCandidates[MAX_CANDIDATES][3]; // 临时候选数组
    int hasMorePages = 0;                   // 标记是否有更多页
    int isInInputArea = 0;                  // 标记是否在输入区域内
    char combine [MAX_INPUT *2 + 1] = {0};  //组合内容缓冲区

    drawCandidateBox(candidates, selected, candidateCount); // 传入实际的候选字数量

    while (1)
    {
        //立即绘制
        if (forceRedraw == 1)
        {
            clrmous(nx, ny);
            drawInputBox(combine); // 将最终输出内容绘制到输入框
            if (inputLen > 0 && candidateCount > 0)
            {
                drawCandidateBox(candidates, selected, candidateCount);
            }
            else
            {
                setfillstyle(SOLID_FILL, WHITE);
                bar(x03, y03, x03 + w03, y03 + h03);
            }
            forceRedraw = 0;
        }

        //鼠标逻辑
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        delay(30);

        //更新组合内容
        memset (combine, 0, sizeof (combine));
        sprintf (combine, "%s%s", finalOutput, input);

        /* 判断是否在输入区域内 */
        isInInputArea = 0;
        // 检查是否在输入框内
        if (nx >= x04 && nx <= x04 + w04 && ny >= y04 && ny <= y04 + h04)
        {
            isInInputArea = 1;
        }
        // 检查是否在候选框内
        if (nx >= x03 && nx <= x03 + w03 && ny >= y03 && ny <= y03 + h03)
        {
            isInInputArea = 1;
        }
        // 检查是否在左右翻页按钮内
        if ((nx >= x01 && nx <= x01 + w01 && ny >= y01 && ny <= y01 + h01) ||
            (nx >= x02 && nx <= x02 + w02 && ny >= y02 && ny <= y02 + h02))
        {
            isInInputArea = 1;
        }

        /* 独立处理鼠标事件 */
        if (nbuttons == 1) // 鼠标左键点击
        {
            if (!isInInputArea)
            {
                // 点击了输入区域外，退出输入模式
                clrmous(nx, ny);
                strcpy(output, finalOutput);
                return;
            }

            if (candidateCount > 0) // 只有候选框有内容时才处理
            {
                for (i; i < candidateCount; i++) // 只检查有候选字的项
                {
                    // 计算每个候选字的点击区域（包括数字和汉字）
                    int charX = x03 + 25 + i * (CHAR_WIDTH + 16);
                    int charY = y03;
                    int charWidth = CHAR_WIDTH * 3; // 数字+汉字的宽度
                    int charHeight = 30;            // 适当的高度

                    if (nx >= charX && nx <= charX + charWidth &&
                        ny >= charY && ny <= charY + charHeight)
                    {
                        // 直接输入选中的汉字到最终输出
                        strcat(finalOutput, candidates[i]);
                        outputLen += strlen(candidates[i]);

                        // 清空拼音输入和候选框
                        inputLen = 0;
                        input[0] = '\0';
                        candidateCount = 0;
                        startIndex = 0;

                        // 立即重绘界面
                        clrmous(nx, ny);
                        drawInputBox(finalOutput);
                        setfillstyle(SOLID_FILL, WHITE);
                        bar(x03, y03, x03 + w03, y03 + h03);
                        break;
                    }
                }
                i = 0;
            }

            // 检查是否点击了翻页按钮
            currentPageButton = 0;
            if (nx >= x01 && nx <= x01 + w01 && ny >= y01 && ny <= y01 + h01)
            {
                currentPageButton = 1;
            }
            else if (nx >= x02 && nx <= x02 + w02 && ny >= y02 && ny <= y02 + h02)
            {
                currentPageButton = 2;
            }

            // 处理翻页逻辑
            if (currentPageButton != 0)
            {
                if (currentPageButton == 1 && startIndex > 0)
                { // 左翻页
                    startIndex -= MAX_CANDIDATES;
                    if (startIndex < 0)
                        startIndex = 0;
                    candidateCount = loadCandidates(input, candidates, startIndex);
                    selected = (candidateCount > 0) ? 0 : -1;
                    needRedraw = 1;
                }
                else if (currentPageButton == 2)
                { // 右翻页
                    // 尝试加载下一页
                    tempCount = loadCandidates(input, tempCandidates, startIndex + MAX_CANDIDATES);
                    if (tempCount > 0)
                    {
                        startIndex += MAX_CANDIDATES;
                        memcpy(candidates, tempCandidates, sizeof(candidates));
                        candidateCount = tempCount;
                        selected = 0;
                        needRedraw = 1;
                    }
                }

                // 如果需要重绘，立即更新界面
                if (needRedraw)
                {
                    clrmous(nx, ny);
                    drawInputBox(combine);
                    if (inputLen > 0 || candidateCount > 0)
                    {
                        drawCandidateBox(candidates, selected, candidateCount);
                    }
                    else
                    {
                        setfillstyle(SOLID_FILL, WHITE);
                        bar(x03, y03, x03 + w03, y03 + h03);
                    }
                    needRedraw = 0;
                }

                // 等待鼠标释放
                while (nbuttons == 1)
                {
                    newmouse(&nx, &ny, &nbuttons);
                    delay(30);
                }
                continue;
            }
            lastPageButton = currentPageButton;
        }

        if (kbhit())
        {
            // 检测键盘输入
            ch = getch();
            forceRedraw = 1;

            if (ch == 13)
            { // 回车键，完成输入
                clrmous(nx, ny);
                delay(30);
                break;
            }

            else if (ch == '-' || ch == '=') 
            {
                if (ch == '-') 
                {
                    // 上一页（同左箭头逻辑）
                    if (startIndex > 0) 
                    {
                        startIndex -= MAX_CANDIDATES;
                        if (startIndex < 0) 
                        {
                            startIndex = 0;
                        }
                        
                        candidateCount = loadCandidates(input, candidates, startIndex);
                        selected = (candidateCount > 0) ? candidateCount - 1 : -1;
                    }
                } 
                else if (ch == '=') 
                {
                    // 下一页（同右箭头逻辑）
                    tempCount = loadCandidates(input, tempCandidates, startIndex + MAX_CANDIDATES);
                    if (tempCount > 0) 
                    {
                        startIndex += MAX_CANDIDATES;
                        memcpy(candidates, tempCandidates, sizeof(candidates));
                        candidateCount = tempCount;
                        selected = 0;
                    }
                }
                
                // 强制刷新界面
                sprintf(combine, "%s%s", finalOutput, input);
                forceRedraw = 1;
                continue;
            }

            //退格键，删除拼音或汉字
            else if (ch == 8)
            { 
                if (inputLen > 0)
                {
                    input[--inputLen] = '\0';
                    startIndex = 0;                                                 // 重置候选汉字起始索引
                    candidateCount = loadCandidates(input, candidates, startIndex); // 重新加载候选汉字
                    selected = 0;                                                   // 重置选中项
                }
                else if (outputLen > 0)
                {
                    int deleteBytes = 1;
                    int chLength = outputLen -1 -i;
                    if (outputLen >= 2 && (finalOutput [outputLen - 1] & 0x80))
                    {
                        //向前查找汉字起始字节
                        int i = outputLen - 2;
                        while (i >=0 && (finalOutput[i] & 0x80))
                        {
                            i--;
                        }
                        //计算完整汉字占用的字节数
                        if (chLength % 2 == 0)
                        {
                            deleteBytes = 2;
                        }
                    }
                    outputLen = (outputLen - deleteBytes >= 0) ? outputLen - deleteBytes : 0;
                    finalOutput[--outputLen] = '\0';
                }
                sprintf (combine, "%s%s", finalOutput, input);
                forceRedraw = 1;
                continue;
            }

            // ESC键，取消输入
            else if (ch == 27)
            { 
                output[0] = '\0';
                return;
            }
            if (ch == 75)
            { // 左箭头
                if (selected > 0)
                {
                    selected--;
                }
                else if (startIndex > 0)
                {
                    startIndex -= MAX_CANDIDATES;
                    if (startIndex < 0)
                    {
                        startIndex = 0; // 防止越界
                    }
                    candidateCount = loadCandidates(input, candidates, startIndex / 2); // 重新加载候选汉字
                    selected = (candidateCount > 0) ? candidateCount - 1 : -1;          // 重置选中项
                }
            }
            else if (ch == 77)
            { // 右箭头
                if (selected < candidateCount - 1)
                {
                    selected++;
                }
                else
                {
                    // 检查是否有更多页
                    tempCount = loadCandidates(input, tempCandidates, startIndex + MAX_CANDIDATES);
                    if (tempCount > 0)
                    {
                        // 有下一页，加载它
                        startIndex += MAX_CANDIDATES;
                        memcpy(candidates, tempCandidates, sizeof(candidates));
                        candidateCount = tempCount;
                        selected = 0;
                    }
                    else
                    {
                        // 没有下一页，保持在当前最后一个候选字
                        selected = candidateCount - 1;
                    }
                }
                if (candidateCount == 0)
                {
                    memset(candidates, '\0', sizeof(candidates)); // 清空候选汉字
                }
            }

            // 数字键1-9选择候选汉字
            else if (ch >= '1' && ch <= '9')
            {
                int index = ch - '1'; // 转换为0-8的索引
                if (index < candidateCount && candidates[index][0] != '\0')
                {
                    strcat(finalOutput, candidates[index]); // 将选中的汉字加入最终输出
                    outputLen += strlen(candidates[index]);
                    inputLen = 0; // 清空拼音输入
                    input[0] = '\0';
                    candidateCount = 0; // 清空候选汉字
                    startIndex = 0;     // 重置候选汉字起始索引
                    sprintf(combine, "%s", finalOutput);  // 立即清空拼音显示
                    forceRedraw = 1;
                    continue;  // 跳过后续处理直接绘制
                }
            }
            // 数字键0选择第10个候选汉字
            else if (ch == '0')
            {
                if (candidateCount >= 10 && candidates[9][0] != '\0')
                {
                    strcat(finalOutput, candidates[9]); // 将第10个汉字加入最终输出
                    outputLen += strlen(candidates[9]);
                    inputLen = 0; // 清空拼音输入
                    input[0] = '\0';
                    candidateCount = 0; // 清空候选汉字
                    startIndex = 0;     // 重置候选汉字起始索引
                    sprintf(combine, "%s", finalOutput);  // 立即清空拼音显示
                    forceRedraw = 1;
                    continue;  // 跳过后续处理直接绘制
                }
            }

            // 拼音输入
            else if (ch >= 'a' && ch <= 'z')
            {
                if (inputLen < MAX_INPUT - 1)
                {
                    input[inputLen++] = ch;
                    input[inputLen] = '\0';
                    startIndex = 0;                                                 // 重置候选汉字起始索引
                    candidateCount = loadCandidates(input, candidates, startIndex); // 加载候选汉字
                    selected = 0;         
                    sprintf(combine, "%s%s", finalOutput, input);  // 实时更新
                    forceRedraw = 1;  // 立即标记需要重绘                                          // 重置选中项
                }
            }
            else if (ch == ' ')
            { // 空格键，选择候选汉字
                if (candidateCount > 0 && candidates[selected][0] != '\0')
                {
                    strcat(finalOutput, candidates[selected]); // 将选中的汉字加入最终输出
                    outputLen += strlen(candidates[selected]);
                    inputLen = 0; // 清空拼音输入
                    input[0] = '\0';
                    candidateCount = 0; // 清空候选汉字
                    startIndex = 0;     // 重置候选汉字起始索引
                    sprintf(combine, "%s", finalOutput);  // 立即更新显示内容
                    forceRedraw = 1;  // 强制立即重绘
                    continue;  // 直接跳转到绘制环节
                }
            }

            // 绘制输入框和候选框
            clrmous(nx, ny);
            drawInputBox(combine); // 将最终输出内容绘制到输入框
            if (inputLen > 0 && candidateCount > 0)
            {
                drawCandidateBox(candidates, selected, candidateCount);
            }
            else
            {
                setfillstyle(SOLID_FILL, WHITE);
                bar(x03, y03, x03 + w03, y03 + h03);
            }
        }
    }

    strcpy(output, finalOutput); // 将最终内容传出
}