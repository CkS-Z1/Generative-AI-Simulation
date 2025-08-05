#include "Chinput.h"

// ���������
void drawInputBox(char *content)
{
    Trigger leftTrigger;
    Trigger rightTrigger;
    // ��ʼ������
    initTrigger(&leftTrigger, x01, y01, w01, h01, "<");
    initTrigger(&rightTrigger, x02, y02, w02, h02, ">");
    // ���ư���
    drawTrigger(leftTrigger, FUHAO);
    drawTrigger(rightTrigger, FUHAO);

    setfillstyle(SOLID_FILL, WHITE);
    bar(x04, y04, x04 + w04, y04 + h04); // ���Ʊ���
    setcolor(BLUE);
    setlinestyle(SOLID_LINE, 0, 1);
    rectangle(x04, y04, x04 + w04, y04 + h04); // ���Ʊ߿�
    setcolor(DARKGRAY);
    drawMsgTxt(x04, y04 + 5, content, 16, DARKGRAY, 28);
}

// ���ƺ�ѡ��
void drawCandidateBox(const char candidates[MAX_CANDIDATES][3], int selected, int candidateCount)
{
    int i;
    int color = BLACK;
    int space = 16;
    char s[2];

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x03, y03, x03 + w03, y03 + h03); // ���Ʊ���
    setcolor(BLUE);
    rectangle(x03, y03, x03 + w03, y03 + h03); // ���Ʊ߿�

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
            // ����ÿ����ѡ���ֵĻ���λ��Ϊ��������
            drawMsgTxt(x03 + 25 + 16 + i * (CHAR_WIDTH + space), y03 + 5, candidates[i], 16, color, MAX_CANDIDATES);
        }
    }
}

// ��ƴ���ļ��м��غ�ѡ����
int loadCandidates(const char *pinyin, char candidates[MAX_CANDIDATES][3], int startIndex)
{
    int i = 0;
    char filepath[30];
    FILE *file;
    char line[256]; // ���ڴ洢��������
    int count = 0;

    sprintf(filepath, "PINYIN\\%s.txt", pinyin); // ƴ��ƴ���ļ�·��
    file = fopen(filepath, "r");
    if (file == NULL)
    {
        return 0; // �ļ������ڻ��޷���
    }

    // ��ȡ��������
    if (fgets(line, sizeof(line), file) != NULL)
    {
        for (i = startIndex * 2; i < strlen(line) && count < MAX_CANDIDATES; i += 2)
        {
            if (line[i] == '\n' || line[i] == '\0')
            {
                break; // �������з����ַ���������
            }
            candidates[count][0] = line[i];
            candidates[count][1] = line[i + 1];
            candidates[count][2] = '\0'; // ȷ���ַ����� '\0' ��β
            count++;
        }
        i = 0;
        // ���ʣ��λ��
        for (i = count; i < MAX_CANDIDATES; i++)
        {
            candidates[i][0] = '\0';
        }
    }

    fclose(file);
    return count; // ���غ�ѡ��������
}

// �����뷨�߼�
void hzinput(char *output)
{
    int nx, ny, nbuttons;
    char input[MAX_INPUT] = {0};                   // �û������ƴ��
    char candidates[MAX_CANDIDATES][3] = {{'\0'}}; // ��ѡ����
    char finalOutput[MAX_INPUT] = {0};             // ��������ĺ�������
    int selected = 0;                              // ��ǰѡ�еĺ�ѡ����
    int inputLen = 0;                              // ��ǰƴ�����볤��
    int outputLen = 0;                             // ����������ݳ���
    int ch, candidateCount = 0;
    int startIndex = 0; // ��ѡ���ֵ���ʼ����
    int i = 0;
    int charX, charY;
    int needRedraw = 0; // ����Ƿ���Ҫ�ػ�
    int forceRedraw = 0;
    static int lastPageButton = 0;
    int currentPageButton = 0;
    int tempCount = 0;                      // ��ʱ����
    char tempCandidates[MAX_CANDIDATES][3]; // ��ʱ��ѡ����
    int hasMorePages = 0;                   // ����Ƿ��и���ҳ
    int isInInputArea = 0;                  // ����Ƿ�������������
    char combine [MAX_INPUT *2 + 1] = {0};  //������ݻ�����

    drawCandidateBox(candidates, selected, candidateCount); // ����ʵ�ʵĺ�ѡ������

    while (1)
    {
        //��������
        if (forceRedraw == 1)
        {
            clrmous(nx, ny);
            drawInputBox(combine); // ������������ݻ��Ƶ������
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

        //����߼�
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        delay(30);

        //�����������
        memset (combine, 0, sizeof (combine));
        sprintf (combine, "%s%s", finalOutput, input);

        /* �ж��Ƿ������������� */
        isInInputArea = 0;
        // ����Ƿ����������
        if (nx >= x04 && nx <= x04 + w04 && ny >= y04 && ny <= y04 + h04)
        {
            isInInputArea = 1;
        }
        // ����Ƿ��ں�ѡ����
        if (nx >= x03 && nx <= x03 + w03 && ny >= y03 && ny <= y03 + h03)
        {
            isInInputArea = 1;
        }
        // ����Ƿ������ҷ�ҳ��ť��
        if ((nx >= x01 && nx <= x01 + w01 && ny >= y01 && ny <= y01 + h01) ||
            (nx >= x02 && nx <= x02 + w02 && ny >= y02 && ny <= y02 + h02))
        {
            isInInputArea = 1;
        }

        /* ������������¼� */
        if (nbuttons == 1) // ���������
        {
            if (!isInInputArea)
            {
                // ��������������⣬�˳�����ģʽ
                clrmous(nx, ny);
                strcpy(output, finalOutput);
                return;
            }

            if (candidateCount > 0) // ֻ�к�ѡ��������ʱ�Ŵ���
            {
                for (i; i < candidateCount; i++) // ֻ����к�ѡ�ֵ���
                {
                    // ����ÿ����ѡ�ֵĵ�����򣨰������ֺͺ��֣�
                    int charX = x03 + 25 + i * (CHAR_WIDTH + 16);
                    int charY = y03;
                    int charWidth = CHAR_WIDTH * 3; // ����+���ֵĿ��
                    int charHeight = 30;            // �ʵ��ĸ߶�

                    if (nx >= charX && nx <= charX + charWidth &&
                        ny >= charY && ny <= charY + charHeight)
                    {
                        // ֱ������ѡ�еĺ��ֵ��������
                        strcat(finalOutput, candidates[i]);
                        outputLen += strlen(candidates[i]);

                        // ���ƴ������ͺ�ѡ��
                        inputLen = 0;
                        input[0] = '\0';
                        candidateCount = 0;
                        startIndex = 0;

                        // �����ػ����
                        clrmous(nx, ny);
                        drawInputBox(finalOutput);
                        setfillstyle(SOLID_FILL, WHITE);
                        bar(x03, y03, x03 + w03, y03 + h03);
                        break;
                    }
                }
                i = 0;
            }

            // ����Ƿ����˷�ҳ��ť
            currentPageButton = 0;
            if (nx >= x01 && nx <= x01 + w01 && ny >= y01 && ny <= y01 + h01)
            {
                currentPageButton = 1;
            }
            else if (nx >= x02 && nx <= x02 + w02 && ny >= y02 && ny <= y02 + h02)
            {
                currentPageButton = 2;
            }

            // ����ҳ�߼�
            if (currentPageButton != 0)
            {
                if (currentPageButton == 1 && startIndex > 0)
                { // ��ҳ
                    startIndex -= MAX_CANDIDATES;
                    if (startIndex < 0)
                        startIndex = 0;
                    candidateCount = loadCandidates(input, candidates, startIndex);
                    selected = (candidateCount > 0) ? 0 : -1;
                    needRedraw = 1;
                }
                else if (currentPageButton == 2)
                { // �ҷ�ҳ
                    // ���Լ�����һҳ
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

                // �����Ҫ�ػ棬�������½���
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

                // �ȴ�����ͷ�
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
            // ����������
            ch = getch();
            forceRedraw = 1;

            if (ch == 13)
            { // �س������������
                clrmous(nx, ny);
                delay(30);
                break;
            }

            else if (ch == '-' || ch == '=') 
            {
                if (ch == '-') 
                {
                    // ��һҳ��ͬ���ͷ�߼���
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
                    // ��һҳ��ͬ�Ҽ�ͷ�߼���
                    tempCount = loadCandidates(input, tempCandidates, startIndex + MAX_CANDIDATES);
                    if (tempCount > 0) 
                    {
                        startIndex += MAX_CANDIDATES;
                        memcpy(candidates, tempCandidates, sizeof(candidates));
                        candidateCount = tempCount;
                        selected = 0;
                    }
                }
                
                // ǿ��ˢ�½���
                sprintf(combine, "%s%s", finalOutput, input);
                forceRedraw = 1;
                continue;
            }

            //�˸����ɾ��ƴ������
            else if (ch == 8)
            { 
                if (inputLen > 0)
                {
                    input[--inputLen] = '\0';
                    startIndex = 0;                                                 // ���ú�ѡ������ʼ����
                    candidateCount = loadCandidates(input, candidates, startIndex); // ���¼��غ�ѡ����
                    selected = 0;                                                   // ����ѡ����
                }
                else if (outputLen > 0)
                {
                    int deleteBytes = 1;
                    int chLength = outputLen -1 -i;
                    if (outputLen >= 2 && (finalOutput [outputLen - 1] & 0x80))
                    {
                        //��ǰ���Һ�����ʼ�ֽ�
                        int i = outputLen - 2;
                        while (i >=0 && (finalOutput[i] & 0x80))
                        {
                            i--;
                        }
                        //������������ռ�õ��ֽ���
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

            // ESC����ȡ������
            else if (ch == 27)
            { 
                output[0] = '\0';
                return;
            }
            if (ch == 75)
            { // ���ͷ
                if (selected > 0)
                {
                    selected--;
                }
                else if (startIndex > 0)
                {
                    startIndex -= MAX_CANDIDATES;
                    if (startIndex < 0)
                    {
                        startIndex = 0; // ��ֹԽ��
                    }
                    candidateCount = loadCandidates(input, candidates, startIndex / 2); // ���¼��غ�ѡ����
                    selected = (candidateCount > 0) ? candidateCount - 1 : -1;          // ����ѡ����
                }
            }
            else if (ch == 77)
            { // �Ҽ�ͷ
                if (selected < candidateCount - 1)
                {
                    selected++;
                }
                else
                {
                    // ����Ƿ��и���ҳ
                    tempCount = loadCandidates(input, tempCandidates, startIndex + MAX_CANDIDATES);
                    if (tempCount > 0)
                    {
                        // ����һҳ��������
                        startIndex += MAX_CANDIDATES;
                        memcpy(candidates, tempCandidates, sizeof(candidates));
                        candidateCount = tempCount;
                        selected = 0;
                    }
                    else
                    {
                        // û����һҳ�������ڵ�ǰ���һ����ѡ��
                        selected = candidateCount - 1;
                    }
                }
                if (candidateCount == 0)
                {
                    memset(candidates, '\0', sizeof(candidates)); // ��պ�ѡ����
                }
            }

            // ���ּ�1-9ѡ���ѡ����
            else if (ch >= '1' && ch <= '9')
            {
                int index = ch - '1'; // ת��Ϊ0-8������
                if (index < candidateCount && candidates[index][0] != '\0')
                {
                    strcat(finalOutput, candidates[index]); // ��ѡ�еĺ��ּ����������
                    outputLen += strlen(candidates[index]);
                    inputLen = 0; // ���ƴ������
                    input[0] = '\0';
                    candidateCount = 0; // ��պ�ѡ����
                    startIndex = 0;     // ���ú�ѡ������ʼ����
                    sprintf(combine, "%s", finalOutput);  // �������ƴ����ʾ
                    forceRedraw = 1;
                    continue;  // ������������ֱ�ӻ���
                }
            }
            // ���ּ�0ѡ���10����ѡ����
            else if (ch == '0')
            {
                if (candidateCount >= 10 && candidates[9][0] != '\0')
                {
                    strcat(finalOutput, candidates[9]); // ����10�����ּ����������
                    outputLen += strlen(candidates[9]);
                    inputLen = 0; // ���ƴ������
                    input[0] = '\0';
                    candidateCount = 0; // ��պ�ѡ����
                    startIndex = 0;     // ���ú�ѡ������ʼ����
                    sprintf(combine, "%s", finalOutput);  // �������ƴ����ʾ
                    forceRedraw = 1;
                    continue;  // ������������ֱ�ӻ���
                }
            }

            // ƴ������
            else if (ch >= 'a' && ch <= 'z')
            {
                if (inputLen < MAX_INPUT - 1)
                {
                    input[inputLen++] = ch;
                    input[inputLen] = '\0';
                    startIndex = 0;                                                 // ���ú�ѡ������ʼ����
                    candidateCount = loadCandidates(input, candidates, startIndex); // ���غ�ѡ����
                    selected = 0;         
                    sprintf(combine, "%s%s", finalOutput, input);  // ʵʱ����
                    forceRedraw = 1;  // ���������Ҫ�ػ�                                          // ����ѡ����
                }
            }
            else if (ch == ' ')
            { // �ո����ѡ���ѡ����
                if (candidateCount > 0 && candidates[selected][0] != '\0')
                {
                    strcat(finalOutput, candidates[selected]); // ��ѡ�еĺ��ּ����������
                    outputLen += strlen(candidates[selected]);
                    inputLen = 0; // ���ƴ������
                    input[0] = '\0';
                    candidateCount = 0; // ��պ�ѡ����
                    startIndex = 0;     // ���ú�ѡ������ʼ����
                    sprintf(combine, "%s", finalOutput);  // ����������ʾ����
                    forceRedraw = 1;  // ǿ�������ػ�
                    continue;  // ֱ����ת�����ƻ���
                }
            }

            // ���������ͺ�ѡ��
            clrmous(nx, ny);
            drawInputBox(combine); // ������������ݻ��Ƶ������
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

    strcpy(output, finalOutput); // ���������ݴ���
}