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
    // �����Ľ�Բ��
    arc(x1 + BUTTON_RADIUS, y1 + BUTTON_RADIUS, 90, 180, BUTTON_RADIUS);
    arc(x2 - BUTTON_RADIUS, y1 + BUTTON_RADIUS, 0, 90, BUTTON_RADIUS);
    arc(x2 - BUTTON_RADIUS, y2 - BUTTON_RADIUS, 270, 360, BUTTON_RADIUS);
    arc(x1 + BUTTON_RADIUS, y2 - BUTTON_RADIUS, 180, 270, BUTTON_RADIUS);

    // ���ӻ���
    line(x1 + BUTTON_RADIUS, y1, x2 - BUTTON_RADIUS, y1);
    line(x1 + BUTTON_RADIUS, y2, x2 - BUTTON_RADIUS, y2);
    line(x1, y1 + BUTTON_RADIUS, x1, y2 - BUTTON_RADIUS);
    line(x2, y1 + BUTTON_RADIUS, x2, y2 - BUTTON_RADIUS);
}

// �޸ĺ��drawPageButtons
static void drawPageButtons()
{
    /* ���ԭ��ť���� */
    setfillstyle(SOLID_FILL, CONTENT_BG);
    bar(0, BUTTON_Y - 10, 640, 480);

    // ����ͳһ������
    setfillstyle(SOLID_FILL, BUTTON_BG);
    bar(0, BUTTON_Y - 5, 640, BUTTON_Y + BUTTON_HEIGHT + 5);

    // ��һҳ��ť
    setcolor(BUTTON_BORDER);
    if (current_page > 0)
    {
        setfillstyle(SOLID_FILL, YELLOW);
        round_rect(PREV_BUTTON_X, BUTTON_Y, PREV_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
        floodfill(PREV_BUTTON_X + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, BUTTON_BORDER);

        // ������ӰЧ��
        setcolor(DARKGRAY);
        puthz(PREV_BUTTON_X + 12, BUTTON_Y + 10, "��һҳ", 16, 16, DARKGRAY);
        puthz(PREV_BUTTON_X + 10, BUTTON_Y + 8, "��һҳ", 16, 16, WHITE);
    }
    else
    {
        // ����״̬
        setfillstyle(SOLID_FILL, BUTTON_BG);
        bar(PREV_BUTTON_X, BUTTON_Y, PREV_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
    }

    // ��һҳ��ť������������һҳ��ʡ�Բ����ظ����ݣ�
    setcolor(BUTTON_BORDER);
    if (current_page < max_page)
    {
        // �ж������ͣ
        int hover = (MouseX >= NEXT_BUTTON_X && MouseX <= NEXT_BUTTON_X + BUTTON_WIDTH &&
                     MouseY >= BUTTON_Y && MouseY <= BUTTON_Y + BUTTON_HEIGHT);

        setfillstyle(SOLID_FILL, YELLOW);
        round_rect(NEXT_BUTTON_X, BUTTON_Y, NEXT_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
        floodfill(NEXT_BUTTON_X + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, BUTTON_BORDER);

        // ������ӰЧ��
        setcolor(DARKGRAY);
        puthz(NEXT_BUTTON_X + 12, BUTTON_Y + 10, "��һҳ", 16, 16, DARKGRAY);
        puthz(NEXT_BUTTON_X + 10, BUTTON_Y + 8, "��һҳ", 16, 16, WHITE);
    }
    else
    {
        // ����״̬
        setfillstyle(SOLID_FILL, BUTTON_BG);
        bar(NEXT_BUTTON_X, BUTTON_Y, NEXT_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
    }

    // ����ͼ��ť��3DЧ����
    if (current_page == max_page)
    {
        setcolor(GEN_BUTTON_BG);
        setfillstyle(SOLID_FILL, GEN_BUTTON_BG);
        bar3d(GEN_BUTTON_X, BUTTON_Y, GEN_BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT, 3, 1);

        // ��������
        settextstyle(1, 0, 3);
        setcolor(WHITE);
        puthz(GEN_BUTTON_X + 12, BUTTON_Y + 10, "����ͼ", 16, 16, DARKGRAY);
        puthz(GEN_BUTTON_X + 10, BUTTON_Y + 8, "����ͼ", 16, 16, WHITE);
    }

    // ���װ����
    setcolor(BORDER_COLOR);
    line(0, BUTTON_Y - 10, 640, BUTTON_Y - 10);
}

// �޸ĺ��checkButtonClick
static int checkButtonClick()
{
    static int lastPress = 0;
    int nx, ny, nbuttons;
    int result = 0;
    static int lastHover = 0;
    mread(&nx, &ny, &nbuttons);

    // ����Ƿ���ͣ
    if (nbuttons == 0)
    {
        // ������ť��Y��������ͬ��
        if (ny >= BUTTON_Y && ny <= BUTTON_Y + BUTTON_HEIGHT)
        {
            // �ж��Ƿ���X������
            if ((nx >= PREV_BUTTON_X && nx <= PREV_BUTTON_X + BUTTON_WIDTH) || (nx >= NEXT_BUTTON_X && nx <= NEXT_BUTTON_X + BUTTON_WIDTH) || (nx >= GEN_BUTTON_X && nx <= GEN_BUTTON_X + BUTTON_WIDTH))
            {
                if (lastHover == 0)
                {
                    drawPageButtons(); // �ػ水ť
                    lastHover = 1;
                }
            }
            else
            {
                if (lastHover == 1)
                {
                    drawPageButtons(); // �ػ水ť
                    lastHover = 0;
                }
            }
        }
    }
    // clrmous(nx, ny);
    if (nbuttons == 0 && lastPress == 1)
    {
        // ����ͼ��ť���
        if (nx >= GEN_BUTTON_X && nx <= GEN_BUTTON_X + BUTTON_WIDTH &&
            ny >= BUTTON_Y && ny <= BUTTON_Y + BUTTON_HEIGHT)
        {
            result = 2; // �·���ֵ��ʾ����ͼ��ť
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

// ���浱ǰҳ�����ݵ��ļ�
static void saveCurrentPage()
{
    char filename[MAX_PATH];
    FILE *fp;
    int has_gbk = 0; // ����Ƿ����GBK����
    int i;

    if (current_page < 0)
    {
        return;
    }

    // ����Ƿ����GBK����
    for (i = 0; i < strlen(page_content);)
    {
        if ((unsigned char)page_content[i] >= 0x80)
        {
            // ȷ�������������ֽ��ҵڶ����ֽںϷ�
            if (i + 1 < strlen(page_content) &&
                (unsigned char)page_content[i + 1] >= 0x40)
            {
                has_gbk = 1;
                break;
            }
            i += 2; // ����˫�ֽ�
        }
        else
        {
            i++; // ASCII�ַ�
        }
    }

    // ��GBK����ʱ������
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

// ����ָ��ҳ������
static int loadPage(int page_num)
{
    char filename[MAX_PATH];
    FILE *fp;
    int result = 0;
    static char line[1024] = {0}; // �����к��ʵ��л���������
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
        page_content[0] = '\0'; // �������
        // ֱ����Ⱦ�ļ����ݣ���ʹ��outputText
        current_x = START_X;
        current_y = START_Y + 15;

        if (current_page == 0)
        {
            current_y += PARA_HEIGHT;
        }
        // ��������������
        setfillstyle(SOLID_FILL, CONTENT_BG);
        bar(START_X - 10, START_Y - 10, 630, BUTTON_Y - 20);

        // ����������߿�
        setcolor(BORDER_COLOR);
        rectangle(START_X - 15, START_Y - 15, 635, BUTTON_Y - 15);
        while (fgets(line, sizeof(line), fp))
        {
            // ����ֱ�ӻ��Ƶ���Ļ
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

                // ����ASCII�ַ�
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

                // ����Ƿ���Ҫ����
                if (current_x + 18 + 15 >= 640 && *ptr)
                {
                    current_x = START_X;
                    current_y += LINE_HEIGHT;
                }
            }
            // �����һ���Ժ�Ҫ����
            current_x = START_X;
            current_y += PARA_HEIGHT;
        }
        fclose(fp);
        strcpy(page_content, ""); // ����ļ����¼��ص�page_content
        result = 1;
    }
    else
    {
        outtextxy(300, 240, "FAIL TO OPEN FILE!!!");
    }
    drawPageButtons();
    return result;
}

// ��ҳ����
static void pageTurn(int direction)
{
    int buttonNum;
    if (direction == 1)
    { // ��һҳ
        if (current_page < max_page)
        {
            current_page++;
            if (!loadPage(current_page))
            {
                cleardevice();          // ���ͼ���豸��ʾ
                page_content[0] = '\0'; // ȷ��ҳ������Ϊ��
                reset_cursor();
            }
        }
        else
        {
            // �Ѿ������һҳ
            return;
        }
    }
    else if (direction == -1)
    { // ��һҳ
        if (current_page >= 1)
        {
            cleardevice(); // ���ͼ���豸��ʾ
            current_page--;
            loadPage(current_page);
        }
        else
        {
            // �Ѿ��ǵ�һҳ
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
    reset_cursor(); // ��ѡ��ȷ�����Ҳ����
}

// �޸ĺ���ʵʱ����ı�����
void outputText(const char *str, int isNewParagraph)
{
    static int len = 0;
    char hanzi[3];
    char ascii[2];
    const char indent[] = "    ";
    // �����¶���
    if (isNewParagraph)
    {
        current_y += PARA_HEIGHT; // ���Ӷ���߶�
        current_x = START_X;

        // ���ҳ�����ݲ�Ϊ�������һ���ַ����ǻ��з������ֶ�����һ�λ���
        if (strlen(page_content) > 0 && page_content[strlen(page_content) - 1] != '\n' && page_content[strlen(page_content) - 1] != '\r')
        {
            strcat(page_content, "\n");
        }
        // д������
        strcat(page_content, indent);
        len += strlen(indent);
        outtextxy(current_x, current_y, indent);
        current_x += strlen(indent) * 12;
    }
    // strncat(page_content, str, MAX_CONTENT - strlen(page_content) - 1);

    // д���߼�
    while (*str)
    {
        // ����Ƿ���Ҫ��ҳ
        if (current_y + LINE_HEIGHT + 20 >= 480 - BUTTON_HEIGHT - MARGIN_BOTTOM)
        {
            saveCurrentPage();
            current_page++;
            max_page = current_page;
            // ��ҳʱ�����page_content
            len = 0;
            memset(page_content, 0, MAX_CONTENT);
            reset_cursor();
            cleardevice();
            drawPageButtons();
        }

        // ���������ַ�
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

        // ����ASCII�ַ�
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

        // ����Ƿ���Ҫ����
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
        // ��ҳʱ�����page_content
        len = 0;
        memset(page_content, 0, MAX_CONTENT);

        reset_cursor();
        cleardevice();
    }
    // ���Ʒ�ҳ��ť
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

    loadPage(current_page); // ���ص���ҳ
    drawPageButtons();

    while (!exit_flag)
    {
        int buttonNum = checkButtonClick();
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        if (buttonNum == 2 && current_page == max_page)
        { // �����һҳ���������ͼ��ť
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