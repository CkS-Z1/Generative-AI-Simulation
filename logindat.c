#include "logindat.h"

int loginuser(char *name, char *password) // �û���¼����
{
    FILE *file;
    int i = 1;
    char path[100] = "USER\\";
    char suffix[100] = ".txt"; // �ļ���׺
    char filepassword[100];    // �洢�ļ��е�����

    strcat(path, name);
    strcat(path, suffix);

    if (strlen(name) == 0) // �û���Ϊ��
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);                        // ���������ɫΪ��ɫ
        bar(150, 160, 490, 300);                       // ���ư�ɫ����
        setcolor(RED);                                 // ������ɫΪ��ɫ
        setlinestyle(0, 0, 3);                         // ��������
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1); // ���ƺ�ɫ���α߿�
        puthz(218, 200, "�������û���", 24, 30, RED);  // ��ʾ�����û���
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press); // �������λ��
            delay(1);
        }
        return 1;
    }

    file = fopen(path, "rt"); // ���ļ�
    if (file == NULL)         // �ļ���ʧ��
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);
        bar(150, 160, 490, 300);
        setcolor(RED);
        setlinestyle(0, 0, 3);
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);
        puthz(188, 200, "�û�������", 24, 30, RED); // ��ʾ�û�������
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press);
            delay(1);
        }
        fclose(file);
        return 2;
    }

    if (strlen(password) == 0) // ����Ϊ��
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);
        bar(150, 160, 490, 300);
        setcolor(RED);
        setlinestyle(0, 0, 3);
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);
        puthz(233, 200, "����������", 24, 30, RED); // ��ʾ��������
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press);
            delay(1);
        }
        fclose(file);
        return 2;
    }

    fscanf(file, "%s", filepassword);        // ��ȡ�ļ��е�����
    if (strcmp(password, filepassword) == 0) // ����ƥ��
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);
        bar(150, 160, 490, 300);
        setcolor(RED);
        setlinestyle(0, 0, 3);
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);
        puthz(263, 200, "��¼�ɹ�", 24, 30, RED); // ��ʾ��¼�ɹ�
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press);
            delay(1);
        }

        fclose(file); // �ر��ļ�
        return 3;
    }
    else // ���벻ƥ��
    {
        clrmous(MouseX, MouseY);
        setfillstyle(1, WHITE);
        bar(150, 160, 490, 300);
        setcolor(RED);
        setlinestyle(0, 0, 3);
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);
        puthz(263, 200, "�û������������", 36, 30, RED); // ��ʾ�û������������
        for (i = 1; i < 500; i++)
        {
            newmouse(&MouseX, &MouseY, &press);
            delay(1);
        }
        fclose(file);
        return 2;
    }
}