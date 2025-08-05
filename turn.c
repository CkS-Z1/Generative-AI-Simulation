// turn.c 修改内容
#include "turn.h"

// 立体按钮绘制函数
void draw3dButton(int x, int y, int width, int height, char *text, int pressed)
{
    // 按钮主体
    setfillstyle(SOLID_FILL, pressed ? DARKGRAY : LIGHTCYAN);
    bar(x, y, x + width, y + height);

    // 边框效果
    setcolor(pressed ? DARKGRAY : WHITE);
    line(x, y, x + width - 1, y);  // 上边框
    line(x, y, x, y + height - 1); // 左边框

    setcolor(pressed ? WHITE : DARKGRAY);
    line(x + width, y, x + width, y + height);  // 右边框
    line(x, y + height, x + width, y + height); // 下边框

    // 文字居中
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
    int color = YELLOW; // 默认黄色
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
    // 10%的概率绘制更大的星星
    if (random(10) == 0)
    {
        // 画周围的像素，形成中十字
        setcolor(color);
        line(x - 3, y, x + 3, y);
        line(x, y - 3, x, y + 3);
    }
    if (random(25) == 0)
    {
        setcolor(color);
        // 画周围的像素，形成大十字
        line(x - 4, y, x + 4, y);
        line(x, y - 4, x, y + 4);
    }
        if (random(5) == 0)
    {
        setcolor(WHITE);
        // 像素点瞬时白光
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
    // 初始化界面
    cleardevice();
    cleardevice();
    // 背景设计：深蓝底色+装饰条纹
    setbkcolor(BLUE);
    setfillstyle(SOLID_FILL, BLUE);
    floodfill(0, 0, WHITE);

    // 顶部装饰条
    setcolor(CYAN);
    setfillstyle(SOLID_FILL, CYAN);
    bar(0, 0, 640, 5);
    bar(0, 45, 640, 50);
    bar(0, 105, 640, 110);
    bar(0, 150, 640, 155);

    // 装饰星星
    for (i = 0; i < 300; i++)
    {
        genStar();
    }

    // 标题文字带阴影
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 5);
    
    // 左对齐+顶对齐
    settextjustify(LEFT_TEXT, TOP_TEXT);  
    // 绘制暗灰色阴影文本
    setcolor(DARKGRAY);
    outtextxy(72, 52, "GEN AI Simulation System");

    // 绘制亮青色前景文本
    setcolor(LIGHTCYAN);
    outtextxy(70, 50, "GEN AI Simulation System");

    // 绘制三个立体按钮
    draw3dButton(BUTTON1_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Pics", 0);
    draw3dButton(BUTTON2_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Chat", 0);
    draw3dButton(BUTTON3_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Exit", 0);

    while (1)
    {
        int nx, ny, nbuttons;
        int hover = 0;
        mread(&MouseX, &MouseY, &MouseS);
        newmouse(&nx, &ny, &nbuttons);

        // 鼠标悬停效果
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

        // 按钮按下效果
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

        // 按钮释放触发
        if (nbuttons == 0 && last_buttons == 1)
        {
            if (active_button == hover && hover)
            {
                // 故事生图
                if (hover == 1)
                {
                    return 4;
                }
                // 文生聊天
                else if (hover == 2)
                {
                    return 7;
                }
                // 退出
                else if (hover == 3)
                {
                    return 999;
                }
            }
            // 恢复按钮状态
            draw3dButton(BUTTON1_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Pics", 0);
            draw3dButton(BUTTON2_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Chat", 0);
            draw3dButton(BUTTON3_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, "Exit", 0);
            active_button = 0;
        }

        last_buttons = nbuttons;
    }
}