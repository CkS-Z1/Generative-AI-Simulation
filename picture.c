#include "picture.h"

int outPicture(char genre, int storyNum, Picstate *state)
{
    int picNum;
    static int lastNum = 3;
    char bmpPath[100] = {0};
    char dbmPath[100] = {0};
    char titleText[50] = {0};
    int i;
    // ͼƬ��ʾ����������ԭʼλ�ò��䣩
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

    // ���Ʊ���
    setbkcolor(BLUE);
    cleardevice();

    // ���ƶ���������
    setfillstyle(SOLID_FILL, BLUE);
    setcolor(YELLOW);
    bar(0, 0, 639, 35);

    // ��̬���ɱ����ı�
    sprintf(titleText, "Visual Story - Genre: %c  Episode: %d", genre, storyNum);

    // ���ñ���������ʽ
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    outtextxy(320, 5, titleText);

    // ����ͼƬ�߿�
    setcolor(LIGHTCYAN);
    rectangle(imgX - 5 - 3, imgY - 5,
              imgX + imgWidth + 5 - 3, imgY + imgHeight + 5);
    rectangle(imgX - 5 - 5, imgY - 5 - 2,
              imgX + imgWidth + 5 - 1, imgY + imgHeight + 5 + 2);

    // ��ͼƬ��������ɫ����
    setfillstyle(SOLID_FILL, WHITE);
    bar(imgX - 4, imgY - 1,
        imgX + imgWidth + 1,
        imgY + imgHeight + 1);

    srand(time(NULL));
    // ���ڸ��ʷֲ�ѡ��ͼƬ
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

    // ��¼���ɵ�ͼƬ����
    if (state->genCnt < 2)
    {
        state->generatedPics[state->genCnt++] = picNum;
    }

    sprintf(bmpPath, "PICTURE\\%c\\%d\\%d.bmp", genre, storyNum, picNum);
    sprintf(dbmPath, "PICTURE\\%c\\%d\\%d.DBM", genre, storyNum, picNum);

    if (bmp_convert(bmpPath, dbmPath) == 0)
    {
        // ��ʾʱ����͸��Ч������
        show_dbm(imgX, imgY, dbmPath, 0);
    }

    // ���Ƶײ�װ����
    setfillstyle(INTERLEAVE_FILL, LIGHTCYAN);
    bar(0, 480 - 25, 639, 480);

    // �ײ�������Ϣ
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

    puthz(button1X + 5, buttonY + 5, "��������", 16, 16, WHITE);

    puthz(button2X + 5, buttonY + 5, "��", 16, 16, WHITE);
    puthz(button2X + 5 + 48, buttonY + 5, "��", 16, 16, WHITE);
    
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
                // ���Ʊ���
                setbkcolor(BLUE);
                // ���ƶ���������
                setfillstyle(SOLID_FILL, BLUE);
                setcolor(YELLOW);
                bar(0, 0, 639, 35);

                // ���ɱ�������
                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
                settextjustify(CENTER_TEXT, TOP_TEXT);
                outtextxy(320, 5, titleText);

                // ����ͼƬ�߿�����Ƕ�ױ߿�
                setcolor(LIGHTCYAN);
                rectangle(imgX - 5 - 3, imgY - 5,
                          imgX + imgWidth + 5 - 3, imgY + imgHeight + 5);
                rectangle(imgX - 5 - 5, imgY - 5 - 2,
                          imgX + imgWidth + 5 - 1, imgY + imgHeight + 5 + 2);

                // ��ͼƬ��������ɫ����
                setfillstyle(SOLID_FILL, WHITE);
                bar(imgX - 4, imgY - 1,
                    imgX + imgWidth + 1,
                    imgY + imgHeight + 1);

                // ��¼���εĹ��£����ߺ��������������ɣ���Ҫ�ܳ����ϴβ�ͬ��ͼƬ
                lastNum = picNum;
                while (picNum == lastNum)
                {
                    picNum = rand() % 3;
                }

                if (state->genCnt < 2)
                {
                    // ������������ɣ����ǵڶ���Ԫ��
                    state->generatedPics[state->genCnt] = picNum;
                    state->genCnt = (state->genCnt == 0) ? 1 : 2;
                }
                sprintf(bmpPath, "PICTURE\\%c\\%d\\%d.bmp", genre, storyNum, picNum);
                sprintf(dbmPath, "PICTURE\\%c\\%d\\%d.DBM", genre, storyNum, picNum);

                if (bmp_convert(bmpPath, dbmPath) == 0)
                {
                    show_dbm(imgX, imgY, dbmPath, 0);
                }

                // ���Ƶײ�װ����
                setfillstyle(INTERLEAVE_FILL, LIGHTCYAN);
                bar(0, 480 - 25, 639, 480);

                // �ײ�������Ϣ
                setcolor(DARKGRAY);
                settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);

                outtextxy(500, 455, "Nostalgia Viewer v1.0");

                setfillstyle(SOLID_FILL, LIGHTGRAY);
                setcolor(LIGHTGRAY);
                // �����˳���ťλ��
                button2X = (button1X + button2X) / 2;
                setcolor(LIGHTGRAY);
                bar(button2X, buttonY, button2X + buttonWidth, buttonY + buttonHeight);
                setcolor(DARKGRAY);
                rectangle(button2X, buttonY, button2X + buttonWidth, buttonY + buttonHeight);

                puthz(button2X + 5, buttonY + 5, "��", 16, 16, WHITE);
                puthz(button2X + 5 + 48, buttonY + 5, "��", 16, 16, WHITE);
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
    int startX = (640 - 4 * STAR_SPACE) / 2; // ������ʾ
    int startY1 = 180, startY2 = 280;
    int i;
    for (i = 0; i < 5; i++)
    {
        int x = startX + i * STAR_SPACE;
        // �Ȼ����ĵĴ��Ȧ
        setcolor(YELLOW);
        circle(x, y, STAR_SIZE);
        // �����ǰ�Ĵ��Ȧ��ѡ�з�Χ�ڣ����
        if (i < highlight)
        {
            setfillstyle(SOLID_FILL, YELLOW);
            fillellipse(x, y, STAR_SIZE - 5, STAR_SIZE - 5);
        }
    }
}

void ratingSystem(int cnt, Picstate *state)
{
    int startX = (640 - 4 * STAR_SPACE) / 2; // ������ʾ
    int startY1 = 180, startY2 = 280;
    int score1 = 0, score2 = 0;
    int activeLine = 0; // ��ǰ��������
    int line, i, j;
    char result[64] = {0};
    double coolingRate = 0.95;
    // ��ʼ������
    cleardevice();
    setbkcolor(WHITE);
    cleardevice();

    // ���Ʊ���
    puthz(30, 50, "����Ϊ������ͼ���֣����ǻ����ø��ã�", 32, 40, RED);
    puthz(startX - 95, startY1, "ͼƬһ��", 16, 16, DARKGRAY);

    // ���ƴ��Ȧ�߿�
    drawStarFrame(startY1, 0);
    if (cnt == 1)
    {
        puthz(startX - 95, startY2, "ͼƬ����", 16, 16, DARKGRAY);
        drawStarFrame(startY2, 0);
    }

    // �����ύ��ť
    setfillstyle(SOLID_FILL, GREEN);
    bar(250, 400, 350, 450);
    puthz(270, 415, "�ύ����", 16, 16, WHITE);

    // ����ѭ��
    while (1)
    {
        int nx, ny, nbuttons;
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        if (nbuttons == 1)
        {
            // ����һ�е��
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
                            // ���ԭ�д��
                            setfillstyle(SOLID_FILL, WHITE);
                            bar(startX - STAR_SIZE, currentY - STAR_SIZE,
                                startX + 5 * STAR_SPACE, currentY + STAR_SIZE);

                            // ���·������ػ�
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
            // ����ύ��ť���
            if (nx > 250 && nx < 350 && ny > 400 && ny < 450)
            {
                double sum = 0.0;
                // �������ָ��¸���
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

                // ���ʹ�һ��
                for (i = 0; i < 3; i++)
                {
                    sum += state->prob[i];
                }
                for (i = 0; i < 3; i++)
                {
                    state->prob[i] /= sum;
                }
                // �����¶�
                state->temperature *= coolingRate;

                // �Ƚϵ÷֣��������
                cleardevice();
                setcolor(WHITE);
                if (cnt == 0)
                {
                    if (score1 == 0)
                    {
                        sprintf(result, "�����ɹ���ͼƬ��ȫ�������û�Ԥ�ڣ������в�������");
                    }
                    else if (score1 <= 2)
                    {
                        sprintf(result, "�����ɹ���ͼƬ��̫�����û�Ԥ�ڣ������в�������");
                    }
                    else if (score1 == 3)
                    {
                        sprintf(result, "�����ɹ���ͼƬ�ȽϷ����û�Ԥ�ڣ������в�������");
                    }
                    else if (score1 > 3)
                    {
                        sprintf(result, "�����ɹ���ͼƬʮ�ַ����û�Ԥ�ڣ������в�������");
                    }
                }
                else
                {
                    if (score1 <= 2 && score2 <= 2)
                    {
                        sprintf(result, "�����ɹ�����������ͼ��������Ԥ�ڣ������в�������");
                    }
                    else if (score1 > score2)
                    {
                        sprintf(result, "�����ɹ���ͼƬһ����������Ԥ�ڣ������в�������");
                    }
                    else if (score2 > score1)
                    {
                        sprintf(result, "�����ɹ���ͼƬ������������Ԥ�ڣ������в�������");
                    }
                    else
                    {
                        sprintf(result, "�����ɹ�������ͼƬ������ͬ�������в�������");
                    }
                }
                puthz(40, 208, result, 24, 24, RED);
                delay(2000);
                return;
            }
        }
    }
}
