#include "register.h"

/*****************************************
login.c
COPYRIGHT:	ChenSihao
FUNCTION:	main
ABSTRACT:   ����ע�����Ļ��ƣ��������������ת�� ��ӭ���� �� ��¼����
DATE:2024/1/24
******************************************/

int registerPage()
{
    Box accountBox, passwordBox1, passwordBox2;
    Button registerButton, cancelButton;
    int accountLen = 0, passwordLen1 = 0, passwordLen2 = 0;
    User user = {{0}, {0}, {0}};

    drawPage_register(&accountBox, &passwordBox1, &passwordBox2, &registerButton, &cancelButton);

    // �ȴ��û�����
    while (1)
    {
        int nx, ny, nbuttons;
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        delay(30);

        if (nbuttons == 1)
        {
            // �û�������򱻵��
            if (nx >= accountBox.x && nx <= accountBox.x + accountBox.width &&
                ny >= accountBox.y && ny <= accountBox.y + accountBox.height)
            {
                delay(30);
                putText(&accountBox, user.account, &accountLen);
            }
            // ���������1�����
            else if (nx >= passwordBox1.x && nx <= passwordBox1.x + passwordBox1.width &&
                     ny >= passwordBox1.y && ny <= passwordBox1.y + passwordBox1.height)
            {
                delay(30);
                putPassword(&passwordBox1, user.password1, &passwordLen1);
            }
            // ���������2�����
            else if (nx >= passwordBox2.x && nx <= passwordBox2.x + passwordBox2.width &&
                     ny >= passwordBox2.y && ny <= passwordBox2.y + passwordBox2.height)
            {
                delay(30);
                putPassword(&passwordBox2, user.password2, &passwordLen2);
            }
            // ע�ᰴť�����
            else if (nx >= registerButton.x && nx <= registerButton.x + registerButton.width &&
                     ny >= registerButton.y && ny <= registerButton.y + registerButton.height)
            {
                // ��������Ƿ�һ��
                delay(400);
                if (passwordLen1 == 0 && passwordLen2 == 0 || accountLen == 0)
                {
                    cleardevice();
                    puthz(150, 200, "�˺����벻��Ϊ�գ�", 32, 40, RED);
                    delay(1000);
                    return 2; // ע��ɹ�����ת��¼ҳ��
                }
                else if (strcmp(user.password1, user.password2) == 0)
                {
                    usersave(user); // �����û���Ϣ

                    cleardevice();
                    puthz(170, 200, "ע��ɹ���", 32, 40, RED);
                    delay(1000);
                    return 1; // ע��ɹ�����ת��¼ҳ��
                }
                else
                {
                    cleardevice();
                    puthz(170, 200, "�������벻һ�£�", 32, 40, RED);
                    delay(1000);
                    return 2; // ע��ɹ�����ת��¼ҳ��
                }
            }
            // ȡ����ť�����
            else if (nx >= cancelButton.x && nx <= cancelButton.x + cancelButton.width &&
                     ny >= cancelButton.y && ny <= cancelButton.y + cancelButton.height)
            {
                delay(400);
                return 0; // ���ػ�ӭҳ��
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

    // ���������Ͱ�ť
    destroyBox(&accountBox);
    destroyBox(&passwordBox1);
    destroyBox(&passwordBox2);
    destroyButton(&registerButton);
    destroyButton(&cancelButton);

    return 0;
}

void drawPage_register(Box *accountBox, Box *passwordBox1, Box *passwordBox2, Button *registerButton, Button *cancelButton)
{
    int nx, ny, nbuttons;
    // ��ȡ���λ�ò�������
    mread(&nx, &ny, &nbuttons);
    clrmous(nx, ny);

    // ���Ʊ���
    setbkcolor(WHITE);
    cleardevice();

    // ��ʾע����Ϣ
    setcolor(RED);
    settextstyle(8, 0, 5);
    outtextxy(200, 30, "Register");

    // ��ʼ�������
    settextstyle(8, 0, 4);
    initBox(accountBox, 100, 100, 370, 80, "Account");
    initBox(passwordBox1, 100, 210, 370, 80, "Password");
    initBox(passwordBox2, 100, 320, 370, 80, "Confirm Password");

    // ��ʼ����ť
    initButton(registerButton, 510, 330, 100, 50, "ע��");
    initButton(cancelButton, 510, 400, 100, 50, "ȡ��");

    // ���������Ͱ�ť
    drawBox(*accountBox);
    drawBox(*passwordBox1);
    drawBox(*passwordBox2);
    drawButton(*registerButton);
    drawButton(*cancelButton);
}