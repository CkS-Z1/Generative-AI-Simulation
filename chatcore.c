#include "chatcore.h"

extern char folderpath[50];

int chatCorePage()
{
    // 定义变量
    // 处理发送键用
    Trigger sendTrigger;
    // 处理消息用
    //  char message[] = "一二三四五六七八九十";
    char message[MAX_INPUT] = {0}; // 输入消息的缓冲区
    int inputlen = 0;
    FILE *message_file = NULL;
    char mf[40];

    // 绘制消息用
    Body MsgBody;
    int cur_y = SHANGY;
    int is_page_full = 0;

    // 调用鼠标用
    int nx, ny, nbuttons;

    // AI回复消息用
    char respond[MAX_ANSWER_LENGTH + 1];             // 回复消息的缓冲区
    const char rf[23] = "RULES\\rules.txt"; // 规则文件路径

    // 中文输入法用
    char result[MAX_INPUT] = {0};

    // 处理键盘用
    char ch;

    // 翻页用
    Trigger pgupTrigger;
    Trigger pgdnTrigger;
    int page_int = 1;   // 页码计数器
    char page_char[10]; // 页码字符串
    char next_page_path[30];
    int maxpage = 1;

    //绘制背景用
    char bmpPath[] = "IMAGE\\chat.bmp";
    char dbmPath[] = "IMAGE\\chat.dbm";
    // 菜单用
    Trigger menuTrigger;

    // 生成时间戳用
    char timestamp[20];

    // 创建存储聊天历史的文件夹
    char folderName[9] = {0};
    char path[16] = "HISTORY\\";

    //绘制退出按键
    Trigger exitTrigger;
    int ex = 5, ey = 420, ew = 40, eh = 50;

    setbkcolor(DARKGRAY);

    // 生成一个时间戳
    generateTimestamp(timestamp, sizeof(timestamp));

    // 初始化固定元素
    initTrigger(&sendTrigger, 510, 420, 100, 50, "发送");
    initTrigger(&pgupTrigger, 560, 370, 50, 50, "下页");
    initTrigger(&pgdnTrigger, 510, 370, 50, 50, "上页");
    initTrigger(&menuTrigger, 0, 230, 16, 20, ">");
    initTrigger(&exitTrigger, ex, ey, ew, eh, "退出");

    // 绘制固定元素
    clrmous(nx, ny);
    clearEvents();
    cleardevice();
    //绘制背景
    if (bmp_convert(bmpPath, dbmPath) == 0)
    {
        // 显示时增加透明效果参数
        show_dbm(0, 0, dbmPath, 0);
    }
    drawTrigger(sendTrigger, HANZI);
    drawTrigger(pgupTrigger, HANZI);
    drawTrigger(pgdnTrigger, HANZI);
    drawTrigger(menuTrigger, FUHAO);
    drawInputBox("请输入文字"); // 绘制输入框
    drawTrigger(exitTrigger, HANZI);//绘制退出键
    setcolor (WHITE);
    rectangle (ZUOX, SHANGY, YOUX, XIAY);
                                // 绘制候选框
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(x03, y03, x03 + w03, y03 + h03);
    setcolor(BLUE);
    rectangle(x03, y03, x03 + w03, y03 + h03);
    // 标注页码
    setcolor(WHITE);
    settextstyle(8, 0, 1);
    outtextxy(620, 395, "1"); // 显示页码

    // 创建存储聊天记录的文件夹
    strncpy(folderName, timestamp + strlen(timestamp) - 8, 8); // 复制后8位
    folderName[8] = '\0';                                      // 确保字符串以'\0'结尾
    strcpy(folderpath, path);
    strcat(folderpath, folderName);
    mkdir(folderpath);
    strcat(folderpath, "\\");

    // 创建储存聊天记录的文件
    strcpy(mf, folderpath);
    strcat(mf, "\\message.txt");

    // 主循环
    while (1)
    {
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);
        delay(30);

        if (nbuttons == 1)
        {
            // 输入框被点击
            if (nx >= x04 && nx <= x04 + w04 && ny >= y04 && ny <= y04 + h04)
            {
                ////调试
                // itoa (page_int, str1, 10);
                // itoa (maxpage, str2, 10);
                // setfillstyle (1, WHITE);
                // bar (10, 10, 50, 50);
                // outtextxy (10, 10, str1);
                // outtextxy (10, 30, str2);
                if (page_int < maxpage)
                {
                    showErrorPrompt("请回到最新一页");
                }
                else
                {
                    // 清空消息框
                    setfillstyle(1, WHITE);
                    bar(x04, y04, x04 + w04, y04 + h04); // 清空输入框
                    setcolor(BLUE);
                    rectangle(x04, y04, x04 + w04, y04 + h04); // 绘制边框
                    // 调用输入法
                    hzinput(message);           // 调用输入法
                    inputlen = strlen(message); // 更新输入长度
                }
            }
            // 发送键被点击
            else if (nx >= sendTrigger.x && nx <= sendTrigger.x + sendTrigger.width &&
                     ny >= sendTrigger.y && ny <= sendTrigger.y + sendTrigger.height)
            {
                // 输入为空
                if (inputlen == 0)
                {
                    showErrorPrompt(Error0100);
                }
                // 输入不为空
                else
                {
                    // 用户发送消息
                    saveMsg(mf, message);
                    Msg_preprocessing(&MsgBody, cur_y, message, yonghu);
                    addMessage(&MsgBody, &cur_y, &page_int, &maxpage); // 传入folderPath
                    cur_y += MsgBody.height + 16;
                    freeBody(&MsgBody);
                    save_history(folderpath, page_int, cur_y);

                    if (find_answer(message, respond, rf))
                    {
                        // AI回复消息
                        saveMsg(mf, respond);
                        Msg_preprocessing(&MsgBody, cur_y, respond, AI);
                        addMessage(&MsgBody, &cur_y, &page_int, &maxpage); // 传入folderPath
                        cur_y += MsgBody.height + 16;
                        freeBody(&MsgBody);
                        save_history(folderpath, page_int, cur_y);
                    }
                    else
                    {
                        showErrorPrompt("对不起这个问题我还不会");
                    }

                    // 清空输入输出缓冲区
                    //  清空输入输出缓冲区
                    fflush(stdin);
                    fflush(stdout);

                    // 清空 message 和 respond 缓冲区
                    memset(message, 0, sizeof(message));
                    memset(respond, 0, sizeof(respond));
                }
                // 清空消息框
                drawInputBox("请输入文字");
            }

            // 向后翻页
            else if (nx >= pgupTrigger.x && nx <= pgupTrigger.x + pgupTrigger.width &&
                     ny >= pgupTrigger.y && ny <= pgupTrigger.y + pgupTrigger.height)
            {
                sprintf(next_page_path, "%sImg_%d.dat", folderpath, page_int + 1);
                if (fileExists(next_page_path))
                {
                    clrmous(nx, ny);
                    FlipPage(PAGE_UP, &page_int, &cur_y, &maxpage); // 翻页
                }
                else
                {
                    clrmous(nx, ny);
                    showErrorPrompt("已经是最后一页");
                }
            }

            // 向前翻页
            else if (nx >= pgdnTrigger.x && nx <= pgdnTrigger.x + pgdnTrigger.width &&
                     ny >= pgdnTrigger.y && ny <= pgdnTrigger.y + pgdnTrigger.height)
            {
                clrmous(nx, ny);
                FlipPage(PAGE_DOWN, &page_int, &cur_y, &maxpage); // 翻页
            }

            // 菜单键被点击
            else if (nx >= menuTrigger.x && nx <= menuTrigger.x + menuTrigger.width &&
                     ny >= menuTrigger.y && ny <= menuTrigger.y + menuTrigger.height)
            {
                clrmous(nx, ny); // 清除鼠标
                delay(30);
                handleMenuClick(&cur_y); // 处理菜单点击事件
            }
           
            //退出键被点击
           else if(nx >= ex && nx<= ex + ew && ny >= ey && ny <= ey + eh )
           {
               return 3;
           }
           
            while (nbuttons == 1) // 等待鼠标释放
            {
                newmouse(&nx, &ny, &nbuttons);
                delay(30);
            }
        }
        
    }
    return 3;
}

