// turn.c �޸�����
#include "turn.h"

// ���尴ť���ƺ���
void draw3dButton(int x, int y, int width, int height, char *text, int pressed)
{
    // ��ť����
    setfillstyle(SOLID_FILL, pressed ? DARKGRAY : LIGHTCYAN);
    bar(x, y, x + width, y + height);

    // �߿�Ч��
    setcolor(pressed ? DARKGRAY : WHITE);
    line(x, y, x + width - 1, y);  // �ϱ߿�
    line(x, y, x, y + height - 1); // ��߿�

    setcolor(pressed ? WHITE : DARKGRAY);
    line(x + width, y, x + width, y + height);  // �ұ߿�
    line(x, y + height, x + width, y + height); // �±߿�

    // ���־���
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setcolor(pressed ? YELLOW : DARKGRAY);
    outtextxy(x + width / 2, y + 25, text);
    settextjustify(LEFT_TEXT, TOP_TEXT);
}

void genStar()
{
    int i;
    int x = random(610) + 10;
    int y = min(min(random(440), random(440)), min(random(440), random(440))) + 10;
    int color = YELLOW; // Ĭ�ϻ�ɫ
    int r = random(10);
    setcolor(YELLOW);
    if (r < 4)
    {
        color = WHITE;
    }
    else if (r < 6)
    {
        color = LIGHTGRAY;
    }
    
    setcolor(color);
    line(x - 2, y, x + 2, y);
    line(x, y - 2, x, y + 2);
    // 10%�ĸ��ʻ��Ƹ��������
    if (random(10) == 0)
    {
        // ����Χ�����أ��γ���ʮ��
        setcolor(color);
        line(x - 3, y, x + 3, y);
        line(x, y - 3, x, y + 3);
    }
    if (random(25) == 0)
    {
        setcolor(color);
        // ����Χ�����أ��γɴ�ʮ��
        line(x - 4, y, x + 4, y);
        line(x, y - 4, x, y + 4);
    }
        if (random(5) == 0)
    {
        setcolor(WHITE);
        // ���ص�˲ʱ�׹�
        putpixel(x, y, 15);
    }
}

int turnPage()
{
    int last_buttons = 0;
    int pressed_x = 0, pressed_y = 0;
    int active_button = 0;
    int i;
    int nx, ny, nbuttons;
    mread(&nx, &ny, &nbuttons);
    clrmous(nx, ny);
    // ��ʼ������
    cleardevice();
    cleardevice();
    // ������ƣ�������ɫ+װ������
    setbkcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    floodfill(0, 0, WHITE);

    // ����װ����
    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    bar(0, 0, 640, 5);
    bar(0, 45, 640, 50);
    bar(0, 105, 640, 110);
    bar(0, 150, 640, 155);

    // װ������
    for (i = 0; i < 300; i++)
    {
        genStar();
    }

    // �������ִ���Ӱ
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 5);
    
    // �����+������
    settextjustify(LEFT_TEXT, TOP_TEXT);  
    // ���ư���ɫ��Ӱ�ı�
    setcolor(DARKGRAY);
    outtextxy(72, 52, "GEN AI Simulation System");

    // ��������ɫǰ���ı�
    setcolor(LIGHTCYAN);
    outtextxy(70, 50, "GEN AI Simulation System");

    // �����������尴ť
    draw3dButton(BUTTON1_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Pics", 0);
    draw3dButton(BUTTON2_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Chat", 0);
    draw3dButton(BUTTON3_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Exit", 0);

    while (1)
    {
        int nx, ny, nbuttons;
        int hover = 0;
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        // �����ͣЧ��
        if (nx >= BUTTON1_X && nx <= BUTTON1_X + BOX_WIDTH &&
            ny >= BOX_Y && ny <= BOX_Y + BOX_HEIGHT)
        {
            hover = 1;
        }
        else if (nx >= BUTTON2_X && nx <= BUTTON2_X + BOX_WIDTH &&
                 ny >= BOX_Y && ny <= BOX_Y + BOX_HEIGHT)
        {
            hover = 2;
        }
        else if (nx >= BUTTON3_X && nx <= BUTTON3_X + BOX_WIDTH &&
                 ny >= BOX_Y && ny <= BOX_Y + BOX_HEIGHT)
        {
            hover = 3;
        }

        // ��ť����Ч��
        if (nbuttons == 1 && !last_buttons)
        {
            if (hover)
            {
                active_button = hover;
                if (hover == 1)
                {
                    draw3dButton(BUTTON1_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Pics", 1);
                }
                else if (hover == 2)
                {
                    draw3dButton(BUTTON2_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Chat", 1);
                }
                else if (hover == 3)
                {
                    draw3dButton(BUTTON3_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Exit", 1);
                }
            }
        }

        // ��ť�ͷŴ���
        if (nbuttons == 0 && last_buttons == 1)
        {
            if (active_button == hover && hover)
            {
                // ������ͼ
                if (hover == 1)
                {
                    return 4;
                }
                // ��������
                else if (hover == 2)
                {
                    return 7;
                }
                // �˳�
                else if (hover == 3)
                {
                    return 999;
                }
            }
            // �ָ���ť״̬
            draw3dButton(BUTTON1_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Pics", 0);
            draw3dButton(BUTTON2_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Chat", 0);
            draw3dButton(BUTTON3_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Exit", 0);
            active_button = 0;
        }

        last_buttons = nbuttons;
    }
}