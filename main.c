#include "all.h"
#include "main.h" // 包含主程序的头文件

/*****************************************
main.c
COPYRIGHT:	ChenSihao
FUNCTION:	main
ABSTRACT:   进行功能的串联与使用
DATE:2024/1/18
******************************************/

/*********************************************
FUNCTION: main
DESCRIPTION: 主程序入口
INPUT: void
RETURN: (NULL)
***********************************************/
void main()
{
    // 初始化图形模式
    int gd = VGA, gm = VGAHI;   // gd:图形驱动 gm:图形模式
    int page = 0;               // flag:用于标记页面，初始 0 代表欢迎界面
    initgraph(&gd, &gm, "BGI"); // 初始化图形模式
    // _setcursortype(_NOCURSOR);
    // regs.x.ax = 0x00;        // 功能0x00：硬件复位鼠标
    // int86(0x33, &regs, NULL);
    // 初始化鼠标
    // MouseS = 0;
    // clearEvents();
    mouseinit();   // 调用鼠标初始化函数
    clearEvents(); // 清除鼠标事件
    cleardevice(); // 清除屏幕内容

    // 进入主循环
    loop(page);

    // 退出前清除鼠标光标并复位驱动
    // regs.x.ax = 0x00;        // 功能0x00：硬件复位鼠标
    // int86(0x33, &regs, NULL);

    // regs.x.ax = 0x21;        // 功能0x21：软件复位（部分驱动需要）
    // int86(0x33, &regs, NULL);

    // regs.x.ax = 0x02;        // 功能0x02：强制隐藏光标
    // int86(0x33, &regs, NULL);
    // 清理资源
    closegraph(); // 关闭图形模式
    printf("程序已退出。\n");
}

/*********************************************
FUNCTION: loop
DESCRIPTION: 主循环，处理页面之间的跳转
INPUT: page
RETURN: (NULL)
***********************************************/
void loop(int page)
{
    int nx, ny, nbuttons = 0; // 用于存储鼠标的新位置和按键状态
    double sum;
    storyInput userInput = {0};
    int storyNum = 0;
    Picstate state;
    // 初始化概率分布
    state.prob[0] = 1.0;
    state.prob[1] = 1.0;
    state.prob[2] = 1.0;
    // 归一化概率分布
    sum = state.prob[0] + state.prob[1] + state.prob[2];
    state.prob[0] /= sum;
    state.prob[1] /= sum;
    state.prob[2] /= sum;
    // 设置初始温度参数
    state.temperature = INITIAL_TEMP;
    // 初始化生成的图片索引和计数
    state.generatedPics[0] = -1;
    state.generatedPics[1] = -1;
    state.genCnt = 0;

    // InputState state;
    while (1)
    {
        // mread(&MouseX, &MouseY, &MouseS);
        // newmouse(&nx, &ny, &nbuttons); // 不断更新鼠标状态
        delay(30); // 加点延迟，不然鼠标一直瞎寄吧闪
        cleardevice();
        // IME_Update(&state, nx, ny);      // 更新输入法状态
        switch (page)
        {
        case 0:
        {
            delay(400);
            // 欢迎界面
            clearEvents();
            cleardevice();
            page = welcomePage();
        }
        break;

        case 1:
        {
            delay(400);
            // 登录界面
            clearEvents();
            cleardevice();
            page = loginPage();
        }
        break;

        case 2:
        {
            delay(400);
            // 注册界面
            clearEvents();
            cleardevice();
            page = registerPage();
        }
        break;

        case 3:
        {
            delay(400);
            // 中间跳转界面
            clearEvents();
            cleardevice();
            page = turnPage();
        }
        break;

        case 4:
        {
            delay(400);
            // 故事输入界面
            // clearEvents();
            cleardevice();
            page = storyInPage(&userInput);
        }
        break;

        case 5:
        {
            // 故事生图界面
            delay(400);
            // 故事输入界面
            // clearEvents();
            cleardevice();
            storyNum = storyGen(userInput);
            page = 6;
        }
        break;

        case 6:
        {
            // 文生图界面
            delay(400);
            // 故事输入界面
            // clearEvents();
            cleardevice();
            page = outPicture(userInput.genre, storyNum, &state);
        }
        break;

        case 7:
        {
            // 文生图界面
            delay(400);
            // 故事输入界面
            // clearEvents();
            cleardevice();
            page = chatCorePage();
        }
        break;

        default:
        {
            nbuttons = 0;
            // clearEvents();
            cleardevice();
            page = 111;
        }
        break;
        }
        if (page == 999)
        {
            // clearEvents();
            MouseS = 0;
            printf("程序已终止，返回代码编辑页面。\n");
            break; // 退出主循环
        }
    }
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <graphics.h>
// #include "mouse.h"
// #include <time.h>
// #include "input.h"

// #define STRESS_TEST_CYCLES 500  // 压力测试循环次数
// #define MEMORY_MONITOR_INTERVAL 50 // 内存监控间隔

// // 内存监控简易实现（需根据具体平台实现）
// void check_memory(const char *phase) {
//     // 这里可以添加平台特定的内存检查代码
//     // 例如使用GetProcessMemoryInfo（Windows）或mallinfo（Linux）
//     printf("[Memory Check] %s\n", phase);
// }

// void critical_path_test() {
//     InputState state;
//     int i, j;
//     int nx, ny, nbuttons;
//     int gd = VGA, gm = VGAHI;             // gd:图形驱动 gm:图形模式
//     IME_Init(&state);
//     initgraph(&gd, &gm, "BGI");  // 初始化图形模式
//     // mread(&MouseX, &MouseY, &MouseS);
//     // 测试关键路径
//     for (i = 0; i < STRESS_TEST_CYCLES; i++) {
//         // 1. 测试超长拼音输入
//         for (j = 0; j < 35; j++) { // 超过32字节缓冲区
//             // newmouse(&nx, &ny, &nbuttons);
//             state.inputMode = 1;
//             handleInput(&state, 'a'); // 需要将handleInput改为非静态
//         }

//         // 2. 测试文件路径边界
//         strcpy(state.pinyin, "veryverylongpinyinstring");
//         getCandidates(&state); // 需要将getCandidates改为非静态

//         // 3. 测试候选词越界访问
//         state.currentPage = 10; // 超出实际页数
//         drawCandidates(&state); // 需要将drawCandidates改为非静态
//         // delay(30);
//         // 4. 内存敏感操作后检查
//         if (i % MEMORY_MONITOR_INTERVAL == 0) {
//             check_memory("During stress test");
//         }
//     }

//     closegraph();
// }

// void leak_test() {
//     int i, j;
//     int nx, ny, nbuttons;
//     int gd = VGA, gm = VGAHI;             // gd:图形驱动 gm:图形模式
//     // IME_Init(&state);
//     initgraph(&gd, &gm, "BGI");  // 初始化图形模式
//     mread(&MouseX, &MouseY, &MouseS);
//     // 重复初始化-释放测试
//     for (i = 0; i < 100; i++) {
//         InputState* state = (InputState*)malloc(sizeof(InputState));
//         IME_Init(state);

//         // 模拟正常使用
//         for (j = 0; j < 10; j++) {
//             newmouse(&nx, &ny, &nbuttons);
//             IME_Update(state, nx, ny);
//         }
//         delay(30);
//         // 强制释放资源
//         free(state);
//         check_memory("After free");
//     }

//     closegraph();
// }

// int main() {
//     int gd = VGA, gm = VGAHI;             // gd:图形驱动 gm:图形模式
//     // IME_Init(&state);
//     initgraph(&gd, &gm, "BGI");  // 初始化图形模式
//     mouseinit();
//     check_memory("Before tests");

//     // 关键路径压力测试
//     critical_path_test();
//     check_memory("After critical test");

//     // 内存泄漏专项测试
//     leak_test();
//     check_memory("After leak test");

//     return 0;
// }