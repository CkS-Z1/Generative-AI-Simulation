#include "chatcore.h"

extern char folderpath[50];

int chatCorePage()
{
    // �������
    // �����ͼ���
    Trigger sendTrigger;
    // ������Ϣ��
    //  char message[] = "һ�����������߰˾�ʮ";
    char message[MAX_INPUT] = {0}; // ������Ϣ�Ļ�����
    int inputlen = 0;
    FILE *message_file = NULL;
    char mf[40];

    // ������Ϣ��
    Body MsgBody;
    int cur_y = SHANGY;
    int is_page_full = 0;

    // ���������
    int nx, ny, nbuttons;

    // AI�ظ���Ϣ��
    char respond[MAX_ANSWER_LENGTH + 1];             // �ظ���Ϣ�Ļ�����
    const char rf[23] = "RULES\\rules.txt"; // �����ļ�·��

    // �������뷨��
    char result[MAX_INPUT] = {0};

    // ���������
    char ch;

    // ��ҳ��
    Trigger pgupTrigger;
    Trigger pgdnTrigger;
    int page_int = 1;   // ҳ�������
    char page_char[10]; // ҳ���ַ���
    char next_page_path[30];
    int maxpage = 1;

    //���Ʊ�����
    char bmpPath[] = "IMAGE\\chat.bmp";
    char dbmPath[] = "IMAGE\\chat.dbm";
    // �˵���
    Trigger menuTrigger;

    // ����ʱ�����
    char timestamp[20];

    // �����洢������ʷ���ļ���
    char folderName[9] = {0};
    char path[16] = "HISTORY\\";

    //�����˳�����
    Trigger exitTrigger;
    int ex = 5, ey = 420, ew = 40, eh = 50;

    setbkcolor(DARKGRAY);

    // ����һ��ʱ���
    generateTimestamp(timestamp, sizeof(timestamp));

    // ��ʼ���̶�Ԫ��
    initTrigger(&sendTrigger, 510, 420, 100, 50, "����");
    initTrigger(&pgupTrigger, 560, 370, 50, 50, "��ҳ");
    initTrigger(&pgdnTrigger, 510, 370, 50, 50, "��ҳ");
    initTrigger(&menuTrigger, 0, 230, 16, 20, ">");
    initTrigger(&exitTrigger, ex, ey, ew, eh, "�˳�");

    // ���ƹ̶�Ԫ��
    clrmous(nx, ny);
    clearEvents();
    cleardevice();
    //���Ʊ���
    if (bmp_convert(bmpPath, dbmPath) == 0)
    {
        // ��ʾʱ����͸��Ч������
        show_dbm(0, 0, dbmPath, 0);
    }
    drawTrigger(sendTrigger, HANZI);
    drawTrigger(pgupTrigger, HANZI);
    drawTrigger(pgdnTrigger, HANZI);
    drawTrigger(menuTrigger, FUHAO);
    drawInputBox("����������"); // ���������
    drawTrigger(exitTrigger, HANZI);//�����˳���
    setcolor (WHITE);
    rectangle (ZUOX, SHANGY, YOUX, XIAY);
                                // ���ƺ�ѡ��
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x03, y03, x03 + w03, y03 + h03);
    setcolor(BLUE);
    rectangle(x03, y03, x03 + w03, y03 + h03);
    // ��עҳ��
    setcolor(WHITE);
    settextstyle(8, 0, 1);
    outtextxy(620, 395, "1"); // ��ʾҳ��

    // �����洢�����¼���ļ���
    strncpy(folderName, timestamp + strlen(timestamp) - 8, 8); // ���ƺ�8λ
    folderName[8] = '\0';                                      // ȷ���ַ�����'\0'��β
    strcpy(folderpath, path);
    strcat(folderpath, folderName);
    mkdir(folderpath);
    strcat(folderpath, "\\");

    // �������������¼���ļ�
    strcpy(mf, folderpath);
    strcat(mf, "\\message.txt");

    // ��ѭ��
    while (1)
    {
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        delay(30);

        if (nbuttons == 1)
        {
            // ����򱻵��
            if (nx >= x04 && nx <= x04 + w04 && ny >= y04 && ny <= y04 + h04)
            {
                ////����
                // itoa (page_int, str1, 10);
                // itoa (maxpage, str2, 10);
                // setfillstyle (1, WHITE);
                // bar (10, 10, 50, 50);
                // outtextxy (10, 10, str1);
                // outtextxy (10, 30, str2);
                if (page_int < maxpage)
                {
                    showErrorPrompt("��ص�����һҳ");
                }
                else
                {
                    // �����Ϣ��
                    setfillstyle(1, WHITE);
                    bar(x04, y04, x04 + w04, y04 + h04); // ��������
                    setcolor(BLUE);
                    rectangle(x04, y04, x04 + w04, y04 + h04); // ���Ʊ߿�
                    // �������뷨
                    hzinput(message);           // �������뷨
                    inputlen = strlen(message); // �������볤��
                }
            }
            // ���ͼ������
            else if (nx >= sendTrigger.x && nx <= sendTrigger.x + sendTrigger.width &&
                     ny >= sendTrigger.y && ny <= sendTrigger.y + sendTrigger.height)
            {
                // ����Ϊ��
                if (inputlen == 0)
                {
                    showErrorPrompt(Error0100);
                }
                // ���벻Ϊ��
                else
                {
                    // �û�������Ϣ
                    saveMsg(mf, message);
                    Msg_preprocessing(&MsgBody, cur_y, message, yonghu);
                    addMessage(&MsgBody, &cur_y, &page_int, &maxpage); // ����folderPath
                    cur_y += MsgBody.height + 16;
                    freeBody(&MsgBody);
                    save_history(folderpath, page_int, cur_y);

                    if (find_answer(message, respond, rf))
                    {
                        // AI�ظ���Ϣ
                        saveMsg(mf, respond);
                        Msg_preprocessing(&MsgBody, cur_y, respond, AI);
                        addMessage(&MsgBody, &cur_y, &page_int, &maxpage); // ����folderPath
                        cur_y += MsgBody.height + 16;
                        freeBody(&MsgBody);
                        save_history(folderpath, page_int, cur_y);
                    }
                    else
                    {
                        showErrorPrompt("�Բ�����������һ�����");
                    }

                    // ����������������
                    //  ����������������
                    fflush(stdin);
                    fflush(stdout);

                    // ��� message �� respond ������
                    memset(message, 0, sizeof(message));
                    memset(respond, 0, sizeof(respond));
                }
                // �����Ϣ��
                drawInputBox("����������");
            }

            // ���ҳ
            else if (nx >= pgupTrigger.x && nx <= pgupTrigger.x + pgupTrigger.width &&
                     ny >= pgupTrigger.y && ny <= pgupTrigger.y + pgupTrigger.height)
            {
                sprintf(next_page_path, "%sImg_%d.dat", folderpath, page_int + 1);
                if (fileExists(next_page_path))
                {
                    clrmous(nx, ny);
                    FlipPage(PAGE_UP, &page_int, &cur_y, &maxpage); // ��ҳ
                }
                else
                {
                    clrmous(nx, ny);
                    showErrorPrompt("�Ѿ������һҳ");
                }
            }

            // ��ǰ��ҳ
            else if (nx >= pgdnTrigger.x && nx <= pgdnTrigger.x + pgdnTrigger.width &&
                     ny >= pgdnTrigger.y && ny <= pgdnTrigger.y + pgdnTrigger.height)
            {
                clrmous(nx, ny);
                FlipPage(PAGE_DOWN, &page_int, &cur_y, &maxpage); // ��ҳ
            }

            // �˵��������
            else if (nx >= menuTrigger.x && nx <= menuTrigger.x + menuTrigger.width &&
                     ny >= menuTrigger.y && ny <= menuTrigger.y + menuTrigger.height)
            {
                clrmous(nx, ny); // ������
                delay(30);
                handleMenuClick(&cur_y); // ����˵�����¼�
            }
           
            //�˳��������
           else if(nx >= ex && nx<= ex + ew && ny >= ey && ny <= ey + eh )
           {
               return 3;
           }
           
            while (nbuttons == 1) // �ȴ�����ͷ�
            {
                newmouse(&nx, &ny, &nbuttons);
                delay(30);
            }
        }
        
    }
    return 3;
}

