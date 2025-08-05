#include "login.h"

/*****************************************
login.c
COPYRIGHT:	ChenSihao
FUNCTION:	main
ABSTRACT:   实现先登陆界面的绘制和功能
******************************************/

int loginPage()
{
    Box accountBox, passwordBox;
    Button loginButton;
    int passwordLen = 0, accountLen = 0;
    User user = {{0}, {0}};
    drawPage_login(&accountBox, &passwordBox, &loginButton);

    while (1)
    {
        int nx, ny, nbuttons;
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        delay(30);

        if (nbuttons == 1)
        {
            if (nx >= accountBox.x && nx <= accountBox.x + accountBox.width &&
                ny >= accountBox.y && ny <= accountBox.y + accountBox.height)
            {
                delay(30);
                putText(&accountBox, user.account, &accountLen);
            }
            else if (nx >= passwordBox.x && nx <= passwordBox.x + passwordBox.width &&
                     ny >= passwordBox.y && ny <= passwordBox.y + passwordBox.height)
            {
                delay(30);
                putPassword(&passwordBox, user.password1, &passwordLen);
            }

            else if (nx >= loginButton.x && nx <= loginButton.x + loginButton.width &&
                     ny >= loginButton.y && ny <= loginButton.y + loginButton.height)
            {
                return loginuser(user.account, user.password1);
            }
        }

        if (kbhit())
        {
            int key = getch();
            if (key == 27)
            {
                printf("退出登录\n");
                break;
            }
        }
    }

    destroyBox(&accountBox);
    destroyBox(&passwordBox);
    free(loginButton.text);
    loginButton.text = NULL;
    return 0;
}

void drawPage_login(Box *accountBox, Box *passwordBox, Button *loginButton)
{
    int nx, ny, nbuttons;
    mread(&nx, &ny, &nbuttons);
    clrmous(nx, ny);

    setbkcolor(WHITE);
    cleardevice();

    setcolor(RED);
    settextstyle(8, 0, 5);
    outtextxy(220, 60, "Login");

    initBox(accountBox, 100, 150, 370, 80, "Account");
    initBox(passwordBox, 100, 300, 370, 80, "Password");
    initButton(loginButton, 510, 330, 100, 50, "登录");

    drawBox(*accountBox);
    drawBox(*passwordBox);
    drawButton(*loginButton);
}