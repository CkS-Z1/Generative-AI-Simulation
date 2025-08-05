#include "welcome.h"

/*****************************************
welcome.c
COPYRIGHT:	ChenSihao
FUNCTION:	main
ABSTRACT:   ���л�ӭ����Ļ��ƣ��������������ת�� ע����� �� ��½����
DATE:2024/1/18
******************************************/

// ��ʾ��ӭҳ��
int welcomePage()
{
    Button button[3];
    int flag;
    int i = 0;
    // ���Ʊ���
    setbkcolor(WHITE); // ���ñ�����ɫΪ��ɫ
    cleardevice();     // �����Ļ����

    // ��ʾ��ӭ��Ϣ
    setcolor(RED);                                   // ����������ɫΪ��ɫ
    settextstyle(8, 0, 3);                           // ����������ʽ�ʹ�С
    outtextxy(140, 100, "Welcome to Generative AI"); // Ӣ�Ļ�ӭ��Ϣ

    // ʹ�ú�����ʾ������ʾ������Ϣ
    puthz(185, 150, "��ӭʹ������ʽ", 24, 30, RED); // 24x24������
    outtextxy(398, 143, "AI");                      // AI

    // ������ť����¼��ע�ᣬ�˳���Ӧ����ͬһ�У���x�����y��ͬ
    // ��ť��ţ�xλ�ã�yλ�ã����---���߶�|����ť����
    initButton(&button[0], 100, 250, 100, 50, "��¼");
    initButton(&button[1], 250, 250, 100, 50, "ע��");
    initButton(&button[2], 400, 250, 100, 50, "�˳�");
    for (i = 0; i < 3; i++)
    {
        drawButton(button[i]);
        destroyButton(&button[i]);
    }

    while (1)
    {
        int nx, ny, nbuttons;
        newmouse(&nx, &ny, &nbuttons);
        delay(30);
        mread(&MouseX, &MouseY, &MouseS); // ��ȡ���λ�ú�״̬
        if (nbuttons == 1)
        {
            // ��¼��ť�����
            if (nx >= 100 && nx <= 200 && ny >= 250 && ny <= 300)
            {
                return BUTTON_LOGIN;
                // return 3;
            }
            // ע�ᰴť�����
            else if (nx >= 250 && nx <= 350 && ny >= 250 && ny <= 300)
            {
                return BUTTON_REGISTER;
            }
            // �˳���ť�����
            else if (nx >= 400 && nx <= 500 && ny >= 250 && ny <= 300)
            {
                return BUTTON_EXIT;
            }
        }
        if (kbhit())
        {                      // ����Ƿ��а�������
            int key = getch(); // ��ȡ����
            if (key == 27)
            { // Esc����ASCII��Ϊ27
                printf("��������ֹ�����ش���༭ҳ�档\n");
                break; // �˳���ѭ��
            }
        }
    }
}