#include "picture.h"

int outPicture(char genre, int storyNum, Picstate *state)
{
    int picNum;
    static int lastNum = 3;
    char bmpPath[100] = {0};
    char dbmPath[100] = {0};
    char titleText[50] = {0};
    int i;
    // 图片显示参数（保持原始位置不变）
    const int imgWidth = 580, imgHeight = 434;
    const int imgX = 32, imgY = 40;

    int buttonWidth = 75;
    int buttonHeight = 25;
    int buttonY = 480 - buttonHeight - 1;
    int spacing = 80;
    int windowWidth = 640;
    int button1X = 40;
    int button2X = button1X + buttonWidth + spacing;
    int nx, ny, nbuttons;
    mread(&nx, &ny, &nbuttons);
    clrmous(nx, ny);

    cleardevice();

    // 绘制背景
    setbkcolor(BLUE);
    cleardevice();

    // 绘制顶部标题栏
    setfillstyle(SOLID_FILL, BLUE);
    setcolor(YELLOW);
    bar(0, 0, 639, 35);

    // 动态生成标题文本
    sprintf(titleText, "Visual Story - Genre: %c  Episode: %d", genre, storyNum);

    // 设置标题文字样式
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(320, 5, titleText);

    // 绘制图片边框
    setcolor(LIGHTCYAN);
    rectangle(imgX - 5 - 3, imgY - 5,
              imgX + imgWidth + 5 - 3, imgY + imgHeight + 5);
    rectangle(imgX - 5 - 5, imgY - 5 - 2,
              imgX + imgWidth + 5 - 1, imgY + imgHeight + 5 + 2);

    // 在图片区域填充白色背景
    setfillstyle(SOLID_FILL, WHITE);
    bar(imgX - 4, imgY - 1,
        imgX + imgWidth + 1,
        imgY + imgHeight + 1);

    srand(time(NULL));
    // 基于概率分布选择图片
    {
        double r = (double)rand() / (RAND_MAX + 1.0);
        double cumulative = 0.0;
        picNum = 0;
        for (i = 0; i < 3; i++)
        {
            cumulative += state->prob[i];
            if (r <= cumulative)
            {
                picNum = i;
                break;
            }
        }
    }

    // 记录生成的图片索引
    if (state->genCnt < 2)
    {
        state->generatedPics[state->genCnt++] = picNum;
    }

    sprintf(bmpPath, "PICTURE\\%c\\%d\\%d.bmp", genre, storyNum, picNum);
    sprintf(dbmPath, "PICTURE\\%c\\%d\\%d.DBM", genre, storyNum, picNum);

    if (bmp_convert(bmpPath, dbmPath) == 0)
    {
        // 显示时增加透明效果参数
        show_dbm(imgX, imgY, dbmPath, 0);
    }

    // 绘制底部装饰条
    setfillstyle(INTERLEAVE_FILL, LIGHTCYAN);
    bar(0, 480 - 25, 639, 480);

    // 底部文字信息
    setcolor(DARKGRAY);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    outtextxy(500, 455, "Nostalgia Viewer v1.0");

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    setcolor(LIGHTGRAY);
    bar(button1X, buttonY, button1X + buttonWidth, buttonY + buttonHeight);
    setcolor(DARKGRAY);
    rectangle(button1X, buttonY, button1X + buttonWidth, buttonY + buttonHeight);

    setcolor(LIGHTGRAY);
    bar(button2X, buttonY, button2X + buttonWidth, buttonY + buttonHeight);
    setcolor(DARKGRAY);
    rectangle(button2X, buttonY, button2X + buttonWidth, buttonY + buttonHeight);

    puthz(button1X + 5, buttonY + 5, "重新生成", 16, 16, WHITE);

    puthz(button2X + 5, buttonY + 5, "退", 16, 16, WHITE);
    puthz(button2X + 5 + 48, buttonY + 5, "出", 16, 16, WHITE);
    
    while (1)
    {
        int nx, ny, nbuttons;
        static int cnt = 0;
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        if (nbuttons == 1)
        {
            if (nx >= button1X && ny >= buttonY && nx <= button1X + buttonWidth && ny <= buttonY + buttonHeight)
            {
                if (cnt >= 1)
                {
                    continue;
                }
                cnt++;
                cleardevice();
                clrmous(nx, ny);
                // 绘制背景
                setbkcolor(BLUE);
                // 绘制顶部标题栏
                setfillstyle(SOLID_FILL, BLUE);
                setcolor(YELLOW);
                bar(0, 0, 639, 35);

                // 生成标题文字
                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
                settextjustify(CENTER_TEXT, TOP_TEXT);
                outtextxy(320, 5, titleText);

                // 绘制图片边框（三层嵌套边框）
                setcolor(LIGHTCYAN);
                rectangle(imgX - 5 - 3, imgY - 5,
                          imgX + imgWidth + 5 - 3, imgY + imgHeight + 5);
                rectangle(imgX - 5 - 5, imgY - 5 - 2,
                          imgX + imgWidth + 5 - 1, imgY + imgHeight + 5 + 2);

                // 在图片区域填充白色背景
                setfillstyle(SOLID_FILL, WHITE);
                bar(imgX - 4, imgY - 1,
                    imgX + imgWidth + 1,
                    imgY + imgHeight + 1);

                // 记录本次的故事，告诉函数是在重新生成，需要跑出和上次不同的图片
                lastNum = picNum;
                while (picNum == lastNum)
                {
                    picNum = rand() % 3;
                }

                if (state->genCnt < 2)
                {
                    // 如果是重新生成，覆盖第二个元素
                    state->generatedPics[state->genCnt] = picNum;
                    state->genCnt = (state->genCnt == 0) ? 1 : 2;
                }
                sprintf(bmpPath, "PICTURE\\%c\\%d\\%d.bmp", genre, storyNum, picNum);
                sprintf(dbmPath, "PICTURE\\%c\\%d\\%d.DBM", genre, storyNum, picNum);

                if (bmp_convert(bmpPath, dbmPath) == 0)
                {
                    show_dbm(imgX, imgY, dbmPath, 0);
                }

                // 绘制底部装饰条
                setfillstyle(INTERLEAVE_FILL, LIGHTCYAN);
                bar(0, 480 - 25, 639, 480);

                // 底部文字信息
                setcolor(DARKGRAY);
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);

                outtextxy(500, 455, "Nostalgia Viewer v1.0");

                setfillstyle(SOLID_FILL, LIGHTGRAY);
                setcolor(LIGHTGRAY);
                // 更新退出按钮位置
                button2X = (button1X + button2X) / 2;
                setcolor(LIGHTGRAY);
                bar(button2X, buttonY, button2X + buttonWidth, buttonY + buttonHeight);
                setcolor(DARKGRAY);
                rectangle(button2X, buttonY, button2X + buttonWidth, buttonY + buttonHeight);

                puthz(button2X + 5, buttonY + 5, "退", 16, 16, WHITE);
                puthz(button2X + 5 + 48, buttonY + 5, "出", 16, 16, WHITE);
            }

            if (nx >= button2X && ny >= buttonY && nx <= button2X + buttonWidth && ny <= buttonY + buttonHeight)
            {
                ratingSystem(cnt, state);
                state->genCnt = 0;
                state->generatedPics[0] = -1;
                state->generatedPics[1] = -1;
                cnt = 0;
                break;
            }
        }
        delay(10);
    }
    setbkcolor(WHITE);
    return 4;
}

void drawStarFrame(int y, int highlight)
{
    int startX = (640 - 4 * STAR_SPACE) / 2; // 居中显示
    int startY1 = 180, startY2 = 280;
    int i;
    for (i = 0; i < 5; i++)
    {
        int x = startX + i * STAR_SPACE;
        // 先画空心的打分圈
        setcolor(YELLOW);
        circle(x, y, STAR_SIZE);
        // 如果当前的打分圈在选中范围内，填充
        if (i < highlight)
        {
            setfillstyle(SOLID_FILL, YELLOW);
            fillellipse(x, y, STAR_SIZE - 5, STAR_SIZE - 5);
        }
    }
}

void ratingSystem(int cnt, Picstate *state)
{
    int startX = (640 - 4 * STAR_SPACE) / 2; // 居中显示
    int startY1 = 180, startY2 = 280;
    int score1 = 0, score2 = 0;
    int activeLine = 0; // 当前操作的行
    int line, i, j;
    char result[64] = {0};
    double coolingRate = 0.95;
    // 初始化界面
    cleardevice();
    setbkcolor(WHITE);
    cleardevice();

    // 绘制标题
    puthz(30, 50, "请你为故事生图评分，我们会做得更好！", 32, 40, RED);
    puthz(startX - 95, startY1, "图片一：", 16, 16, DARKGRAY);

    // 绘制打分圈边框
    drawStarFrame(startY1, 0);
    if (cnt == 1)
    {
        puthz(startX - 95, startY2, "图片二：", 16, 16, DARKGRAY);
        drawStarFrame(startY2, 0);
    }

    // 绘制提交按钮
    setfillstyle(SOLID_FILL, GREEN);
    bar(250, 400, 350, 450);
    puthz(270, 415, "提交评分", 16, 16, WHITE);

    // 交互循环
    while (1)
    {
        int nx, ny, nbuttons;
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        if (nbuttons == 1)
        {
            // 检测第一行点击
            for (line = 0; line < (cnt == 1 ? 2 : 1); line++)
            {
                int currentY = line ? startY2 : startY1;
                if (ny > currentY - STAR_SIZE && ny < currentY + STAR_SIZE)
                {
                    activeLine = line;
                    for (i = 0; i < 5; i++)
                    {
                        int starX = startX + i * STAR_SPACE;
                        if (nx > starX - STAR_SIZE && nx < starX + STAR_SIZE)
                        {
                            // 清除原有打分
                            setfillstyle(SOLID_FILL, WHITE);
                            bar(startX - STAR_SIZE, currentY - STAR_SIZE,
                                startX + 5 * STAR_SPACE, currentY + STAR_SIZE);

                            // 更新分数并重绘
                            if (line == 0)
                            {
                                score1 = i + 1;
                            }
                            else
                            {
                                score2 = i + 1;
                            }
                            clrmous(nx, ny);
                            drawStarFrame(currentY, i + 1);
                            save_bk_mou(nx, ny);
                            drawmous(nx, ny);
                            break;
                        }
                    }
                }
            }
            // 检测提交按钮点击
            if (nx > 250 && nx < 350 && ny > 400 && ny < 450)
            {
                double sum = 0.0;
                // 根据评分更新概率
                if (state->genCnt >= 1)
                {
                    double reward = score1 / 5.0;
                    int pic = state->generatedPics[0];
                    state->prob[pic] *= exp(reward / state->temperature);
                }
                if (cnt == 1 && state->genCnt >= 2)
                {
                    double reward = score2 / 5.0;
                    int pic = state->generatedPics[1];
                    state->prob[pic] *= exp(reward / state->temperature);
                }

                // 概率归一化
                for (i = 0; i < 3; i++)
                {
                    sum += state->prob[i];
                }
                for (i = 0; i < 3; i++)
                {
                    state->prob[i] /= sum;
                }
                // 降低温度
                state->temperature *= coolingRate;

                // 比较得分，输出反馈
                cleardevice();
                setcolor(WHITE);
                if (cnt == 0)
                {
                    if (score1 == 0)
                    {
                        sprintf(result, "反馈成功！图片完全不符合用户预期，将进行参数调整");
                    }
                    else if (score1 <= 2)
                    {
                        sprintf(result, "反馈成功！图片不太符合用户预期，将进行参数调整");
                    }
                    else if (score1 == 3)
                    {
                        sprintf(result, "反馈成功！图片比较符合用户预期，将进行参数调整");
                    }
                    else if (score1 > 3)
                    {
                        sprintf(result, "反馈成功！图片十分符合用户预期，将进行参数调整");
                    }
                }
                else
                {
                    if (score1 <= 2 && score2 <= 2)
                    {
                        sprintf(result, "反馈成功，两张生成图都不符合预期，将进行参数调整");
                    }
                    else if (score1 > score2)
                    {
                        sprintf(result, "反馈成功，图片一更符合生成预期，将进行参数调整");
                    }
                    else if (score2 > score1)
                    {
                        sprintf(result, "反馈成功，图片二更符合生成预期，将进行参数调整");
                    }
                    else
                    {
                        sprintf(result, "反馈成功，两张图片期望相同，将进行参数调整");
                    }
                }
                puthz(40, 208, result, 24, 24, RED);
                delay(2000);
                return;
            }
        }
    }
}
