#include "save.h"
// ���ڰ��û�������������ļ�
void usersave(User user)
{
    FILE *file; // �����ļ�ָ��
    int i = 1;
    char path[100] = "USER\\"; // �û��ļ��洢·��
    char suffix[100] = ".txt"; // �ļ���׺
    strcat(path, user.account);
    strcat(path, suffix);

    // ע�����û�
    file = fopen(path, "wt"); // ��ֻдģʽ�����ļ�
    if (file == NULL)
    {
        clrmous(MouseX, MouseY);                        // �������¼�
        setfillstyle(1, YELLOW);                        // ���������ʽΪ��ɫ
        bar(150, 160, 490, 300);                        // ������ʾ��
        setcolor(BLACK);                                // ������ɫΪ��ɫ
        setlinestyle(0, 0, 3);                          // ����������ʽ
        rectangle(150 + 1, 160 + 1, 490 - 1, 300 - 1);  // ������ʾ��߿�
        puthz(203, 200, "�޷����ļ�", 24, 30, BLACK); // ��ʾ������Ϣ
        return;                                         // ����
    }
    fprintf(file, "%s", user.password1);

    fclose(file); // �ر��ļ�
}
