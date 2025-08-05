/* storyIn.c */
#include "storyIn.h"

// ȫ������������
InputBox nameBox, traitBox, locationBox;
DropDown genreBox;
Button generateBox, exitBox;

// ��ʼ��������
void InitDropDown(DropDown *dropDown, int x, int y)
{
    dropDown->x = x;
    dropDown->y = y;
    dropDown->width = 150;
    dropDown->height = 25;
    strcpy(dropDown->options[0], "�ƻ�ͼ��");
    strcpy(dropDown->options[1], "���ð��");
    strcpy(dropDown->options[2], "��������");
    strcpy(dropDown->options[3], "�������");
    strcpy(dropDown->options[4], "��������");
    dropDown->selected = 0;
    dropDown->isOpen = 0;
}

// ����������
void DrawDropDown(DropDown *dropDown)
{
    int i;
    int yPos;
    // ��������
    if (dropDown->x + dropDown->width > 633 || dropDown->y + dropDown->height > 473)
    {
        dropDown->x = 633 - dropDown->width - 5;
        dropDown->y = 473 - dropDown->height - 5;
    }
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(dropDown->x, dropDown->y, dropDown->x + dropDown->width, dropDown->y + dropDown->height);
    setcolor(DARKGRAY);
    rectangle(dropDown->x, dropDown->y, dropDown->x + dropDown->width, dropDown->y + dropDown->height);

    // ��ʾѡ����
    puthz(dropDown->x + 5, dropDown->y + 4, dropDown->options[dropDown->selected], 16, 15, BLACK);

    // ���Ƽ�ͷ
    line(dropDown->x + dropDown->width - 15, dropDown->y + 10,
         dropDown->x + dropDown->width - 10, dropDown->y + 15);
    line(dropDown->x + dropDown->width - 10, dropDown->y + 15,
         dropDown->x + dropDown->width - 5, dropDown->y + 10);

    // չ��ѡ��
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

// ���ƽ���Ԫ��
void DrawInterface()
{
    // ������м�������
    int titleX = 210;
    int titleY = 60;

    // �����׼����ʹ�ðٷֱ�
    int baseX = 96;  // 15%���
    int baseY = 115; // 20%�߶�
    int i, y;

    // ��ǩ����
    char *labels[] = {"��������", "��ɫ�Ը�", "���µص�", "��ͼ����"};

    // ������Ƥֽ����
    setfillstyle(SOLID_FILL, YELLOW);
    bar(50, 50, 640 - 50, 480 - 50);
    setcolor(DARKGRAY);
    rectangle(49, 49, 640 - 51, 480 - 51);

    
    // ǿ�ƻָ������+������
    settextjustify(LEFT_TEXT, TOP_TEXT);  
    // ���Ӣ��װ��
    settextstyle(3, 0, 6);
    setcolor(WHITE);
    outtextxy(320 - 80, 20, "Story Weaver");
    setcolor(GREEN);
    outtextxy(320 - 78, 18, "Story Weaver");

    // �������
    puthz(titleX, titleY, "������ͼ��", 32, 38, DARKGRAY);
    setcolor(RED);
    settextstyle(8, 0, 3);
    for (i = 0; i < 4; i++)
    {
        y = baseY + i * 70;
        puthz(baseX, y, labels[i], 16, 16, RED);
        outtextxy(baseX + 4 * 16 + 5, y - 13, ":"); // ��ȷ����ð��λ��
    }

    // �������
    DrawInputBox(&nameBox);
    DrawInputBox(&traitBox);
    DrawInputBox(&locationBox);
    drawButton(generateBox);
    drawButton(exitBox);
    DrawDropDown(&genreBox);
}

// �����������
Bool HandleComponents(int nx, int ny, int nbuttons)
{
    Bool changed = False;
    static int lastSelected = -1;
    static char lastContent[3][256] = {0};
    static int isArrow = 0;
    int i;
    int optionY;
    // ����������
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

    // ��������仯
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

// �ж����������Ƿ���GBK����
int isGBK(const char *str)
{
    int i = 0;
    while (str[i])
    {
        if ((unsigned char)str[i] >= 0xB0)
        { // GBK����
            if (!str[i + 1] || (unsigned char)str[i + 1] < 0xA1)
                return 0;
            i += 2;
        }
        else
        { // �Ǻ����ַ�
            return 0;
        }
    }
    return 1;
}

// ��traits.txt��Ѱ�ҡ�personality���Ƿ����
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
                // ȥ����ÿ����ĩ�Ļ��з�����ƥ��
                line[strcspn(line, "\r\n")] = '\0';
                // ��ȷƥ����������
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
    int left = 120, top = 140;     // ����λ��
    int width = 400, height = 200; // ���ڳߴ�
    int titleHeight = 30;          // �������߶�

    // ���ƴ�����Ӱ
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(left + 5, top + 5, left + width + 5, top + height + 5);

    // ������
    if (strcmp(msg, "����Ϊ�����ɹ���") == 0)
    {
        // �����ڱ���
        setfillstyle(SOLID_FILL, LIGHTGREEN);
        bar(left, top, left + width, top + height);

        // ���ڱ߿�
        setcolor(LIGHTGRAY);
        rectangle(left, top, left + width, top + height);

        setfillstyle(SOLID_FILL, WATERBLUE);
        bar(left + 1, top + 1, left + width - 1, top + titleHeight);
        setcolor(WHITE);
        puthz(left + width / 2 - 55, top + 4, "ͨ�����", 24, 26, BLUE);
    }
    else
    {
        // �����ڱ���
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(left, top, left + width, top + height);

        // ���ڱ߿�
        setcolor(DARKBLUE);
        rectangle(left, top, left + width, top + height);

        setfillstyle(SOLID_FILL, WATERBLUE);
        bar(left + 1, top + 1, left + width - 1, top + titleHeight);
        setcolor(WHITE);
        puthz(left + width / 2 - 55, top + 4, "������ʾ", 24, 26, BLUE);
        // ����ͼ�꣨16x16��
        setcolor(YELLOW);
        circle(left + 70, top + titleHeight + 50, 12);
        line(left + 70, top + titleHeight + 45, left + 70, top + titleHeight + 52);
        line(left + 70, top + titleHeight + 54, left + 70, top + titleHeight + 55);
    }

    // ��Ϣ����
    puthz(left + 100, top + titleHeight + 38, msg, 24, 24, RED);

    // �ָ���
    setcolor(DARKGRAY);
    line(left + 20, top + height - 50, left + width - 20, top + height - 50);

    // ��ʾ����
    setcolor((strcmp(msg, "����Ϊ�����ɹ���") == 0) ? DARKGRAY : WHITE);
    puthz(left + width / 2 - 80, top + height - 35, "�����������", 16, 20, (strcmp(msg, "����Ϊ�����ɹ���") == 0) ? DARKGRAY : WHITE);
    outtextxy(left + width / 2 - 80 + 120, top + height - 50, "......");
    // �ȴ�����
    getch();
}

// ��������ҳ��
int storyInPage(storyInput *userInput)
{
    InputState input, tmp;
    int nx, ny, nbuttons;
    int isError;
    int isGenerate = 0;
    Bool needRedraw = True;
    int key;
    setbkcolor(WHITE);
    // ��ʼ�������λ��
    CreateInputBox(&nameBox, 183, 107, 300, 30);
    CreateInputBox(&traitBox, 183, 107 + 70, 300, 30);
    CreateInputBox(&locationBox, 183, 107 + 140, 300, 30);
    InitDropDown(&genreBox, 183, 107 + 210);
    initButton(&generateBox, 460, 395, 100, 50, "����");
    initButton(&exitBox, 80, 395, 100, 50, "�˳�");
    // ��ʼ�����뷨
    IME_Init(&input);

    cleardevice();
    DrawInterface();

    while (1)
    {
        // ��������¼�
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        
        if (needRedraw)
        {
            // ����������ͼ��
            clrmous(nx, ny);
            cleardevice();
            // �������ƽ���
            DrawInterface();
            // ���浱ǰ���λ�õ��±���
            save_bk_mou(nx, ny);
            // ���������ͼ��
            drawmous(nx, ny);
            needRedraw = False;
        }
        if (nbuttons == 1)
        {
            isGenerate = 0;
            // ��������ɰ�ť�������̧��ʱ��ʼ���ɹ���
            if (nx >= generateBox.x && ny >= generateBox.y &&
                nx <= generateBox.x + generateBox.width &&
                ny <= generateBox.y + generateBox.height)
            {
                // ��������ɰ�ť����ô�Ϳ�ʼ���ɹ���
                isGenerate = 1;
                // needRedraw = True;
            }
            // ������˳���ť�����˳�����
            else if (nx >= exitBox.x && nx <= exitBox.x + exitBox.width &&
                     ny >= exitBox.y && ny <= exitBox.y + exitBox.height)
            {
                // �����˳�����
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
            // �ж��Ƿ�ʼ���ɹ���
            if (isGenerate)
            {
                char traitCode[3] = {0};
                char tmp[3] = {0};
                int storyNum;
                isGenerate = 0;
                if (nameBox.content[0] == '\0' || traitBox.content[0] == '\0' || locationBox.content[0] == '\0')
                {
                    showError("���벻����Ϊ��Ŷ");
                    needRedraw = True;
                    continue;
                }

                // ��֤GBK
                if (!isGBK(nameBox.content) || !isGBK(traitBox.content) || !isGBK(locationBox.content))
                {
                    showError("�������Ϊ������");
                    needRedraw = True;
                    continue;
                }

                // �����Ը����
                if (!findTraits(traitBox.content, traitCode))
                {
                    showError("����������Ч�Ը�");
                    needRedraw = True;
                    continue;
                }
                showError("����Ϊ�����ɹ���");
                // ��װ����
                strncpy(userInput->mainName, nameBox.content, 31);
                strncpy(userInput->personality, traitCode, 3);
                userInput->personality[2] = '\0'; // ȷ����ֹ��
                strncpy(userInput->location, locationBox.content, 31);
                userInput->genre = genreBox.selected + 'a'; // ��ȡ���´��淽ʽ��a�ƻ�,bð��,c����,d����,e����
                userInput->gender = rand() % 2;

                DestroyInputBox(&nameBox);
                DestroyInputBox(&traitBox);
                DestroyInputBox(&locationBox);
                destroyButton(&generateBox);
                return 5;
            }
        }

        // ����������¼�
        clrmous(nx, ny);
        isError = handleInputBox(&nameBox, &input, nx, ny, nbuttons);
        if (isError == 1)
        {
            needRedraw = True;
        }
        else if (isError == -1)
        {
            showError("���볬��ʮ���ֽ�");
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
            showError("���볬��ʮ���ֽ�");
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
            showError("���볬��ʮ���ֽ�");
            delay(1000);
            needRedraw = True;
            continue;
        }
        save_bk_mou(nx,ny);
        drawmous(nx, ny);
        // �����������¼�
        if (HandleComponents(nx, ny, nbuttons))
        {
            needRedraw = True;
        }
        delay(50); // �Ż�ˢ����
    }
}