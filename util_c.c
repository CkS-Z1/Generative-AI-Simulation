#include "util_c.h"

char folderpath[50];

// ��ʼ����ť
void initTrigger(Trigger *trigger, int x, int y, int width, int height, char *text)
{
    memset(trigger, 0, sizeof(Trigger));
    trigger->x = x;
    trigger->y = y;
    trigger->width = width;
    trigger->height = height;
    trigger->text = text;
}

// ���ư�ť
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

// ��ϢԤ����
void Msg_preprocessing(Body *body, int y, char *text, int duixiang)
{
    int i, j;
    int char_count = 0;
    char *ptr = NULL;
    body->message.message = malloc(strlen(text) + 1); // �����ڴ�洢�ı�����
    if (body->message.message == NULL)
    {
        cleardevice();
        outtextxy(10, 10, "Error code 0010.Press any key to exit.");
        getch();
        exit(1);
    }
    strcpy(body->message.message, text); // �����ı�����

    ptr = body->message.message; // ָ����Ϣ���ݵ�ָ��
    // ������Ϣ������
    while (*ptr != '\0')
    {
        if ((unsigned char)*ptr >= 0xA1 && (unsigned char)*ptr <= 0xFE)
        {
            // ����ռ�����ֽ�
            char_count++;
            ptr += 2;
        }
        else
        {
            // Ӣ�Ļ�����ռһ���ֽ�
            char_count++;
            ptr++;
        }
    }
    body->length = char_count; // �������洢�� body->length

    body->row = (body->length + MAX_LINE_WORDS - 1) / MAX_LINE_WORDS;
    body->height = body->row * LINE_HEIGHT;
    // ������Ϣ����
    if (body->length <= MAX_LINE_WORDS) // ������Ϣ
    {
        body->width = CHAR_WIDTH * body->length;
    }
    else // ������Ϣ
    {
        body->width = MAX_FRAME_WIDTH;
    }
    // ����x����
    if (duixiang == yonghu) // �û�������Ϣ
    {
        body->x = YOUX - body->width;
    }
    else if (duixiang == AI) // AI�����Ϣ
    {
        body->x = ZUOX;
    }

    body->y = y;
    body->bottom = y + body->height;
    body->duixiang = duixiang;
}

// �ͷ��ڴ�
void freeBody(Body *body)
{
    free(body->message.message);
    body->message.message = NULL;
}

// ��������Ϣ
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
    // �����ı��ͱ���
    setfillstyle(1,color);
    bar(body->x,body->y,body->x+body->width,body->y+body->height);
    setcolor(WHITE);
    rectangle(body->x, body->y, body->x + body->width, body->y + body->height);
    drawMsgTxt(body->x, body->y, body->message.message, CHAR_SPACING_OFFSET, DARKGRAY, MAX_LINE_WORDS);
}

// �������Ϣ
void addMessage(Body *body, int *y, int *page_int, int *maxpage)
{
    // ��Ļû������ʱ
    if (body->bottom <= XIAY)
    {
        drawNewMessage(body);
    }
    // ��Ļ������ʱ
    else
    {
        // ���ҳ
        FlipPage(PAGE_UP, page_int, y, maxpage); // ����folderpath����
        *y = SHANGY;                             // ����y����
        body->y = *y;                            // ������Ϣ��y����
        body->bottom = *y + body->height;        // ������Ϣ�ĵײ�����
        drawNewMessage(body);                    // ��������Ϣ
    }
}

// ��ʾ������Ϣ��ʾ
void showErrorPrompt(const char *Prompt)
{
    const char *Img = "IMAGE\\tip.dat";
    // ������Ļ
    Save_image(148, 158, 494, 304, Img);

    setcolor(RED);
    setlinestyle(0, 0, 3);
    rectangle(150, 160, 490, 300);
    setfillstyle(1, WHITE);
    bar(150, 160, 490, 300);
    drawMsgTxt(218, 200, Prompt, 16, RED, 10); // ������ʾ�ı�
    delay(500);

    Load_image(148, 158, 494, 304, Img);
}

// �����Ϣ����
void clearWindow()
{
    setfillstyle(1, WHITE);
    bar(ZUOX, SHANGY, YOUX, XIAY);
}

// ������Ϣ�ı�
void drawMsgTxt(int x, int y, char *s, int part, int color, int words)
{
    FILE *hzk_p = NULL;                                                      // ���庺�ֿ��ļ�ָ��
    unsigned char quma, weima;                                               // ���庺�ֵ������λ��
    unsigned long offset;                                                    // ���庺�����ֿ��е�ƫ����
    unsigned char mask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; // �������飬������ʾ���ֵ����е�����
    int i, j, pos;
    char temp[2] = {0};

    char mat[32]; // 16*16�ĺ�����Ҫ32���ֽڵ��������洢
    int y0 = y;
    int x0 = x;
    int count = 0;                    // ���ڼ���ÿ��������ַ�����
    hzk_p = fopen("HZK\\HZ16", "rb"); // ʹ�����·��

    if (hzk_p == NULL)
    {
        settextjustify(LEFT_TEXT, TOP_TEXT);     // �󲿶��룬��������
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1); // ����ʻ������ˮƽ�����24*24����
        outtextxy(10, 10, "Can't open hzk16 file!Press any key to quit...");
        getch();
        exit(1);
    }

    while (*s != '\0')
    {
        // ÿ��������20���ַ�������x���곬����Ļ���
        while (count < words && *s != '\0' && x < 640 - 16)
        {
            y = y0;

            if ((unsigned char)*s >= 0xA1 && (unsigned char)*s <= 0xFE) // �ж��Ƿ�Ϊ����
            {
                quma = s[0] - 0xA0;                             // �������
                weima = s[1] - 0xA0;                            // ���λ��
                offset = (94 * (quma - 1) + (weima - 1)) * 32L; // ����ƫ����
                fseek(hzk_p, offset, SEEK_SET);                 // �ض�λ�ļ�ָ��
                fread(mat, 32, 1, hzk_p);                       // ��ȡ��������

                for (i = 0; i < 16; i++)
                {
                    pos = 2 * i; // ÿ����2���ֽ�
                    for (j = 0; j < 16; j++)
                    {
                        if ((mask[j % 8] & mat[pos + j / 8]) != 0)
                        {
                            putpixel(x + j, y, color);
                        }
                    }
                    y++;
                }

                x += part; // ˮƽ����ƫ��16������
                s += 2;    // ��������ռ2���ֽ�
            }
            else // Ӣ�Ļ�����
            {
                setcolor(color);
                temp[0] = *s;               // ȡ����ǰ�ַ�
                temp[1] = '\0';             // �ַ���������
                outtextxy(x, y - 12, temp); // ���� y ���꣬+4 ��Ϊ�˶�������
                x += part;                  // ˮƽ����ƫ��8������
                s++;                        // Ӣ�Ļ�����ռ1���ֽ�
            }

            count++; // ��������1
        }

        // ����x���꣬����y���꣬���ü�����
        x = x0;
        y0 += 16;  // ���У����Ӵ�ֱ����ƫ��
        count = 0; // ���ü�����
    }
    fclose(hzk_p); // �ر��ļ�
    hzk_p = NULL;  // ��ָ����ΪNULL����������ָ��
}

// ����Ϣ��GBK������ʽ�洢�����ļ�
void saveMsg(const char *filename, char *message)
{
    static int Msg_count = 1; // ��Ϣ������
    FILE *file = fopen(filename, "ab");
    if (file == NULL)
    {
        cleardevice();
        outtextxy(10, 10, "Error code 0021.Press any key to exit.");
        getch();
        exit(1);
    }

    // д���ַ���
    fprintf(file, "%04d%s\n", Msg_count, message); // ��ӻ��з���ÿһ����Ϣռһ��
    fclose(file);

    Msg_count++; // ������Ϣ������
}

// ��ȡ�ļ��е���Ϣ
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

    if (fgets(buffer, sizeof(buffer), file) != NULL) // ��ȡһ��
    {
        sscanf(buffer, "%04d%[^\n]", &msg->num, msg->message); // �����ź���Ϣ
    }
    else
    {
        msg->num = -1; // �����ȡʧ�ܣ����ñ��Ϊ -1 ��ʾ��Ч
        strcpy(msg->message, "");
    }

    fclose(file);
}

// �������ͼ��
void Save_image(int x1, int y1, int x2, int y2, char *filepath)
{
    int i = 0, j = 0;
    int height = y2 - y1, width = x2 - x1;
    int color = 0;
    FILE *fp = NULL;

    fp = fopen(filepath, "wb");
    if (fp == NULL)
    {
        perror("�޷��򿪸� image �ļ�");
        exit(1);
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            color = getpixel(x1 + j, y1 + i);   // ʹ�� getpixel ��ȡ������ɫ
            fwrite(&color, sizeof(int), 1, fp); // д���ļ�
        }
    }
    fclose(fp);
}

// ���ؾ���ͼ��
void Load_image(int x1, int y1, int x2, int y2, char *filepath)
{
    int i = 0, j = 0;
    int height = y2 - y1, width = x2 - x1;
    int color = 0;
    FILE *fp = NULL;

    fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        perror("�޸� image �洢·��");
        exit(1);
    }
    else
    {
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                fread(&color, sizeof(int), 1, fp); // ���ļ���ȡ��ɫ
                putpixel(x1 + j, y1 + i, color);   // ʹ�� putpixel ����������ɫ
            }
        }
        fclose(fp);
    }
}

// ��ҳ����
void FlipPage(int flip, int *page_count, int *cur_y, int *max_page)
{
    char target_page_name[10];
    char target_page_path[50];
    int exists;
    char *page_str;
    const char *default_path = "IMAGE\\BLANK.DAT";

    if (*page_count + flip < 1)
    {
        showErrorPrompt("�Ѿ��ǵ�һҳ");
        return;
    }

    // ת��ҳ��Ϊ�ַ���
    itoa(*page_count + flip, target_page_name, 10);

    // ����Ŀ��ҳ
    strcpy(target_page_path, folderpath);
    strcat(target_page_path, "Img_");
    strcat(target_page_path, target_page_name);
    strcat(target_page_path, ".dat");

    // ���Ŀ��ҳ�Ƿ����
    exists = fileExists(target_page_path);
    if (!exists)
    {
        // ���Ĭ��·�������Ƿ񳬳���������С
        if (strlen(default_path) >= sizeof(target_page_path))
        {
            // ����·�����������
            showErrorPrompt("Ĭ��ҳ��·������");
            exit(1);
        }
        strcpy(target_page_path, default_path);
        *cur_y = SHANGY;
    }
    // ������ڲ�������ҳ
    clearWindow();
    Load_image(ZUOX, SHANGY, YOUX, XIAY, target_page_path);

    // ����ҳ��
    *page_count += flip;
    if (*max_page < *page_count)
    {
        *max_page = *page_count;
    }

    // ����ҳ����ʾ
    itoa(*page_count, page_str, 10);
    Load_image (620, 370, 700, 420, "IMAGE\\pagenum.dat");
    
    setcolor(WHITE);
    settextstyle(8, 0, 1);
    outtextxy(620, 395, page_str);

    /* �ָ�Ŀ��ҳ��״̬ */
    cur_y = SHANGY;
}

// ���������¼
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

// ����ļ��Ƿ����
int fileExists(const char *filepath)
{
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        // �ļ�������
        return 0;
    }
    else
    {
        // �ļ�����
        fclose(fp);
        return 1;
    }
}

// ���Ƶ�ǰҳ���ļ����б�ͷ�ҳ��ť
void drawCurrentPage(int x1, int y1, int x2, int y2, Trigger HisTrigger[],
                     char foldernames[][50], int current_page, int foldercount, Trigger *PrevBtn, Trigger *NextBtn)
{
    int i;
    int display_count = (foldercount - current_page * 8) > 8 ? 8 : (foldercount - current_page * 8);

    // ���Ʋ˵�����
    setfillstyle(1, WHITE);
    bar(x1, y1, x2, y2); // �������
    setcolor(BLUE);
    rectangle(x1, y1, x2, y2); // ���Ʊ߿�

    // ���Ƶ�ǰҳ���ļ��а�ť
    for (i = 0; i < display_count; i++)
    {
        initTrigger(&HisTrigger[i], x1, y1 + i * 50, 213, 40, foldernames[current_page * 8 + i]);
        drawTrigger(HisTrigger[i], FUHAO);
    }

    // ���Ʒ�ҳ��ť
    if (current_page > 0)
    {
        drawTrigger(*PrevBtn, HANZI); // ����"��һҳ"��ť
    }
    if (foldercount > (current_page + 1) * 8)
    {
        drawTrigger(*NextBtn, HANZI); // ����"��һҳ"��ť
    }
}

// ����˵������
void handleMenuClick(int *cur_y)
{
    // �����ں�����ͷ�������б���
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
    int clicked = 0; // ������־����

    newmouse(&nx, &ny, &nbuttons);
    clrmous(nx, ny);
    delay(30);

    // ���汻�˵������ڵ���ͼ��
    Save_image(x1, y1 - 2, x2 + 2, y2 + 2, Imgf);

    // ��ʼ����ҳ��ť
    initTrigger(&PrevBtn, x1, y2 - 40, 100, 40, "��һҳ");
    initTrigger(&NextBtn, x1 + 113, y2 - 40, 100, 40, "��һҳ");

    // ��ʼ���Ƶ�һҳ
    drawCurrentPage(x1, y1, x2, y2, HisTrigger, foldernames, current_page, foldercount, &PrevBtn, &NextBtn);

    while (!clicked) // �޸�ѭ������
    {
        newmouse(&nx, &ny, &nbuttons);

        if (nbuttons == 1)
        {
            clrmous(nx, ny);

            // ����Ƿ�������ʷ��¼��ť
            for (i = 0; i < 8 && i < foldercount - current_page * 8; i++)
            {
                if (nx >= HisTrigger[i].x && nx <= HisTrigger[i].x + HisTrigger[i].width &&
                    ny >= HisTrigger[i].y && ny <= HisTrigger[i].y + HisTrigger[i].height)
                {
                    // ����ѡ���ļ��е�����·��
                    strcpy(selected_folder, "HISTORY\\");
                    strcat(selected_folder, foldernames[current_page * 8 + i]);
                    strcat(selected_folder, "\\");

                    clicked = 1; // ���ñ�־λ
                    break;       // �˳�ѭ��
                }
            }

            if (clicked)
            {
                break; // ����������ʷ��¼���˳���ѭ��
            }
            // ����Ƿ�����"��һҳ"��ť
            if (current_page > 0 && nx >= PrevBtn.x && nx <= PrevBtn.x + PrevBtn.width &&
                ny >= PrevBtn.y && ny <= PrevBtn.y + PrevBtn.height)
            {
                current_page--; // �л�����һҳ
                drawCurrentPage(x1, y1, x2, y2, HisTrigger, foldernames, current_page, foldercount, &PrevBtn, &NextBtn);
                continue;
            }

            // ����Ƿ�����"��һҳ"��ť
            if (foldercount > (current_page + 1) * 8 && nx >= NextBtn.x && nx <= NextBtn.x + NextBtn.width &&
                ny >= NextBtn.y && ny <= NextBtn.y + NextBtn.height)
            {
                current_page++; // �л�����һҳ
                drawCurrentPage(x1, y1, x2, y2, HisTrigger, foldernames, current_page, foldercount, &PrevBtn, &NextBtn);
                continue;
            }

            // ����ڲ˵�������
            if (nx < x1 || nx > x2 || ny < y1 || ny > y2)
            {
                break;
            }
        }
    }

    // �Ȼָ���Ļ���رղ˵���
    Load_image(x1, y1 - 2, x2 + 2, y2 + 2, Imgf);

    // �ټ���ѡ�е���ʷ��¼������У�
    if (clicked)
    {
        loadHistory(selected_folder, cur_y);
        strcpy(folderpath, selected_folder);
        current_page = 0;
    }
}

// ����ʱ���
void generateTimestamp(char *timestamp, size_t size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // ��ʽ��ʱ���Ϊ "YYYYMMDD_HHMMSS"
    strftime(timestamp, size, "%Y%m%d%H%M%S", t);
}

// �г�ָ��Ŀ¼�µ������ļ��У����洢��������
int listFolders(const char *folderPath, char folderNames[][50], int maxFolders)
{
    struct dirent *entry;
    DIR *dp = opendir(folderPath); // ��Ŀ¼
    int count = 0;                 // �ļ�������

    if (dp == NULL)
    {
        showErrorPrompt("Ŀ¼������");
        return 0;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        // �ų� "." �� ".." Ŀ¼
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            if (count < maxFolders)
            {
                strncpy(folderNames[count], entry->d_name, 50); // �����ļ���
                folderNames[count][49] = '\0';                  // ȷ���ַ����� '\0' ��β
                count++;
            }
            else
            {
                showErrorPrompt("�ļ���������");
                break;
            }
        }
    }

    closedir(dp); // �ر�Ŀ¼
    return count; // �����ļ�����
}

// ������ʷ��¼���� - ֻ����ͼ���ļ�
void loadHistory(const char *folderpath, int *y)
{
    // �������
    char max_page_file[40]; // �洢���ҳ���ļ���
    char temp_file[40];
    int max_page = 0;
    int current_page;
    struct dirent *entry;
    DIR *dp;
    char page_str[10];
    char cur_y[5];
    FILE *yf = NULL;
    char yfpath[50];

    // ��������ҳ��ͼ���ļ�
    dp = opendir(folderpath);
    if (dp)
    {
        while ((entry = readdir(dp)) != NULL)
        {
            // ����Ƿ���Img_��ͷ��.dat�ļ�
            if (strstr(entry->d_name, "IMG_") == entry->d_name &&
                strstr(entry->d_name, ".DAT") != NULL)
            {
                // ��ȡҳ������
                sscanf(entry->d_name, "IMG_%d.DAT", &current_page);
                if (current_page > max_page)
                {
                    max_page = current_page;
                    // ���������ļ�·��
                    strcpy(temp_file, folderpath);
                    strcat(temp_file, entry->d_name);
                }
            }
        }
        closedir(dp);
    }

    // ��ȡ��ǰ��Ϣ��cur_y
    sprintf(yfpath, "%sY.TXT", folderpath);
    yf = fopen(yfpath, "rt");
    if (fgets(cur_y, sizeof(cur_y), yf) != NULL)
    {
        // ���ַ���ת��Ϊ����
        *y = atoi(cur_y);
    }
    fclose(yf);

    // �������ҳ���ͼ��
    if (max_page > 0)
    {
        // ��յ�ǰ����
        clearWindow();
        delay(100);
        // ����ͼ��
        Load_image(ZUOX, SHANGY, YOUX, XIAY, temp_file);
        delay(100);
        // ����ҳ����ʾ
        itoa(max_page, page_str, 10);
        setfillstyle(1, WHITE);
        bar(620, 370, 700, 420);
        setcolor(BLUE);
        settextstyle(8, 0, 1);
        outtextxy(620, 395, page_str);
    }
    else
    {
        showErrorPrompt("δ�ҵ���ʷ��¼ͼ��");
    }
}
