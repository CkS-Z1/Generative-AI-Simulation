#include "box.h"
#include <conio.h>
/*****************************************
box.c
COPYRIGHT:	ChenSihao
FUNCTION:	box
ABSTRACT:   ���������ı��򣬼����ı�������ʾ����
DATE:2024/2/21
******************************************/

/*********************************************
FUNCTION: initBox
DESCRIPTION: ��ʼ���ı���
INPUT: box -> x,y,width,height,*text
RETURN: (NULL)
***********************************************/
void initBox(Box *box, int x, int y, int width, int height, char *text)
{
    box->x = x;
    box->y = y;
    box->width = width;
    box->height = height;
    box->text = malloc(strlen(text) + 1);
    if (box->text == NULL)
    {
        printf("Out of memory!");
        exit(1);
    }
    strcpy(box->text, text);
}

/*********************************************
FUNCTION: drawBox
DESCRIPTION: �����ı��򣬶�ȡ�������Ľṹ������ʽ����
INPUT: box
RETURN: (NULL)
***********************************************/
void drawBox(Box box)
{
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(box.x, box.y, box.x + box.width, box.y + box.height);
    setcolor(BROWN);
    rectangle(box.x, box.y, box.x + box.width, box.y + box.height);
    setcolor(WHITE);
    outtextxy(box.x + 10, box.y + 5, box.text);
}

/*********************************************
FUNCTION: destroyBox
DESCRIPTION: ���ڻ���ʱʹ����malloc��������Ҫ�ͷŶ�̬�ڴ棬�����ı�������
INPUT: box
RETURN: (NULL)
***********************************************/
void destroyBox(Box *box)
{
    if (box)
    {
        free(box->text);
        box->text = NULL;
    }
}

/*********************************************
FUNCTION: putText
DESCRIPTION: ģ�������˻�����ָ��λ�ð��ַ�������ʾ
INPUT: box input inputLen
RETURN: (NULL)
***********************************************/
void putText(Box *box, char *input, int *inputLen)
{
    static char display[16]; // ʹ�þ�̬�ڴ�
    char ch;
    int len = *inputLen;
    int nx, ny, nbuttons;
    mread(&nx, &ny, &nbuttons);
    clrmous(nx, ny);
    // ����������ں�任��ʾ
    setfillstyle(SOLID_FILL, YELLOW);
    bar(box->x, box->y, box->x + box->width, box->y + box->height);
    setcolor(BLUE);
    rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
    settextstyle(8, 0, 4); // 4������
    outtextxy(box->x + 10, box->y + 5, input);

    while (1)
    {
        // �������״̬
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        // ���������Ƿ��ڴ�����
        if (MouseS == 1 && (MouseX < box->x || MouseX > box->x + box->width || MouseY < box->y || MouseY > box->y + box->height))
        {
            break;
        }

        // ��������ȡ�ַ�
        if (kbhit())
        {
            ch = getch();
            if (ch == 13)
            { // ��ȡ���س�ʱֹͣ����
                clrmous(nx, ny);
                break;
            }
            // �����˸�ʱɾ��������ԭ��������
            if (ch == 8 && len > 0)
            {
                len--;
                input[len] = '\0';
                setfillstyle(SOLID_FILL, YELLOW);
                bar(box->x, box->y, box->x + box->width, box->y + box->height);
                setcolor(BLUE);
                rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
            }
            // �û����Ϊ8λ
            else if (len >= 8)
            {
                clrmous(nx, ny);
                strncpy(display, input, len);
                display[len] = '\0';
                setfillstyle(SOLID_FILL, CYAN);
                bar(box->x, box->y, box->x + box->width, box->y + box->height);
                setcolor(DARKGRAY);
                rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
                settextstyle(8, 0, 4); // 4������
                outtextxy(box->x + 10, box->y + 5, display);
                break;
            }
            // �����ж��Ƿ��������
            else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            {
                input[len++] = ch;
                input[len] = '\0';
            }

            // ��ʾ��������
            strncpy(display, input, len);
            display[len] = '\0';
            outtextxy(box->x + 10, box->y + 5, display);
        }

        // �������
        // drawmous(nx, ny);
        // delay(30);  // ����ӳ��Լ���CPUռ��
    }

    // ���������任���ں�������ɫ
    setfillstyle(SOLID_FILL, CYAN);
    bar(box->x, box->y, box->x + box->width, box->y + box->height);
    setcolor(DARKGRAY);
    rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
    settextstyle(8, 0, 4); // 4������
    outtextxy(box->x + 10, box->y + 5, input);
    clrmous(nx, ny);
    *inputLen = len;
}

/*********************************************
FUNCTION: putPassword
DESCRIPTION: ģ���������룬��������ʱ��ʾΪ��*��
INPUT: box password passwordLen
RETURN: (NULL)
***********************************************/
void putPassword(Box *box, char *password, int *passwordLen)
{
    static char display[16]; // ʹ�þ�̬�ڴ�
    int i = 0;
    char ch;
    int len = *passwordLen;
    int nx, ny, nbuttons;
    mread(&nx, &ny, &nbuttons);
    clrmous(nx, ny);
    // ����������ں�任��ʾ
    for (i = 0; i < len; i++)
    {
        display[i] = '*';
    }
    display[len] = '\0';
    setfillstyle(SOLID_FILL, YELLOW);
    bar(box->x, box->y, box->x + box->width, box->y + box->height);
    setcolor(BLUE);
    rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
    settextstyle(8, 0, 4); // 4������
    outtextxy(box->x + 10, box->y + 5, display);

    while (1)
    {
        // �������״̬
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        // ���������Ƿ��ڴ�����
        if (MouseS == 1 && (MouseX < box->x || MouseX > box->x + box->width || MouseY < box->y || MouseY > box->y + box->height))
        {
            break;
        }

        // ��������ȡ�ַ�
        if (kbhit())
        {
            ch = getch();
            if (ch == 13)
            { // ��ȡ���س�ʱֹͣ����
                clrmous(nx, ny);
                break;
            }
            // �����˸�ʱɾ��
            if (ch == 8 && len > 0)
            {
                len--;
                password[len] = '\0';
                setfillstyle(SOLID_FILL, YELLOW);
                bar(box->x, box->y, box->x + box->width, box->y + box->height);
                setcolor(BLUE);
                rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
            }
            // ���볤�ȳ���15λʱֹͣ����
            else if (len >= 15)
            {
                clrmous(nx, ny);
                for (i = 0; i < len; i++)
                {
                    display[i] = '*';
                }
                display[len] = '\0';
                setfillstyle(SOLID_FILL, CYAN);
                bar(box->x, box->y, box->x + box->width, box->y + box->height);
                setcolor(DARKGRAY);
                rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
                settextstyle(8, 0, 4); // 4������
                outtextxy(box->x + 10, box->y + 5, display);
                break;
            }
            else if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            {
                password[len++] = ch;
                password[len] = '\0';
            }

            // ��ʾ��������Ϊ��*��
            for (i = 0; i < len; i++)
            {
                display[i] = '*';
            }
            display[len] = '\0';
            outtextxy(box->x + 10, box->y + 5, display);
        }
        // delay(30);  // ����ӳ��Լ���CPUռ��
    }

    // ���������任���ں�������ɫ
    for (i = 0; i < len; i++)
    {
        display[i] = '*';
    }
    display[len] = '\0';
    setfillstyle(SOLID_FILL, CYAN);
    bar(box->x, box->y, box->x + box->width, box->y + box->height);
    setcolor(DARKGRAY);
    rectangle(box->x, box->y, box->x + box->width, box->y + box->height);
    settextstyle(8, 0, 4); // 4������
    outtextxy(box->x + 10, box->y + 5, display);
    clrmous(nx, ny);
    *passwordLen = len;
}