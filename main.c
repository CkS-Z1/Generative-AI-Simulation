#include "all.h"
#include "main.h" // �����������ͷ�ļ�

/*****************************************
main.c
COPYRIGHT:	ChenSihao
FUNCTION:	main
ABSTRACT:   ���й��ܵĴ�����ʹ��
DATE:2024/1/18
******************************************/

/*********************************************
FUNCTION: main
DESCRIPTION: ���������
INPUT: void
RETURN: (NULL)
***********************************************/
void main()
{
    // ��ʼ��ͼ��ģʽ
    int gd = VGA, gm = VGAHI;   // gd:ͼ������ gm:ͼ��ģʽ
    int page = 0;               // flag:���ڱ��ҳ�棬��ʼ 0 ����ӭ����
    initgraph(&gd, &gm, "BGI"); // ��ʼ��ͼ��ģʽ
    // _setcursortype(_NOCURSOR);
    // regs.x.ax = 0x00;        // ����0x00��Ӳ����λ���
    // int86(0x33, &regs, NULL);
    // ��ʼ�����
    // MouseS = 0;
    // clearEvents();
    mouseinit();   // ��������ʼ������
    clearEvents(); // �������¼�
    cleardevice(); // �����Ļ����

    // ������ѭ��
    loop(page);

    // �˳�ǰ�������겢��λ����
    // regs.x.ax = 0x00;        // ����0x00��Ӳ����λ���
    // int86(0x33, &regs, NULL);

    // regs.x.ax = 0x21;        // ����0x21�������λ������������Ҫ��
    // int86(0x33, &regs, NULL);

    // regs.x.ax = 0x02;        // ����0x02��ǿ�����ع��
    // int86(0x33, &regs, NULL);
    // ������Դ
    closegraph(); // �ر�ͼ��ģʽ
    printf("�������˳���\n");
}

/*********************************************
FUNCTION: loop
DESCRIPTION: ��ѭ��������ҳ��֮�����ת
INPUT: page
RETURN: (NULL)
***********************************************/
void loop(int page)
{
    int nx, ny, nbuttons = 0; // ���ڴ洢������λ�úͰ���״̬
    double sum;
    storyInput userInput = {0};
    int storyNum = 0;
    Picstate state;
    // ��ʼ�����ʷֲ�
    state.prob[0] = 1.0;
    state.prob[1] = 1.0;
    state.prob[2] = 1.0;
    // ��һ�����ʷֲ�
    sum = state.prob[0] + state.prob[1] + state.prob[2];
    state.prob[0] /= sum;
    state.prob[1] /= sum;
    state.prob[2] /= sum;
    // ���ó�ʼ�¶Ȳ���
    state.temperature = INITIAL_TEMP;
    // ��ʼ�����ɵ�ͼƬ�����ͼ���
    state.generatedPics[0] = -1;
    state.generatedPics[1] = -1;
    state.genCnt = 0;

    // InputState state;
    while (1)
    {
        // mread(&MouseX, &MouseY, &MouseS);
        // newmouse(&nx, &ny, &nbuttons); // ���ϸ������״̬
        delay(30); // �ӵ��ӳ٣���Ȼ���һֱϹ�İ���
        cleardevice();
        // IME_Update(&state, nx, ny);      // �������뷨״̬
        switch (page)
        {
        case 0:
        {
            delay(400);
            // ��ӭ����
            clearEvents();
            cleardevice();
            page = welcomePage();
        }
        break;

        case 1:
        {
            delay(400);
            // ��¼����
            clearEvents();
            cleardevice();
            page = loginPage();
        }
        break;

        case 2:
        {
            delay(400);
            // ע�����
            clearEvents();
            cleardevice();
            page = registerPage();
        }
        break;

        case 3:
        {
            delay(400);
            // �м���ת����
            clearEvents();
            cleardevice();
            page = turnPage();
        }
        break;

        case 4:
        {
            delay(400);
            // �����������
            // clearEvents();
            cleardevice();
            page = storyInPage(&userInput);
        }
        break;

        case 5:
        {
            // ������ͼ����
            delay(400);
            // �����������
            // clearEvents();
            cleardevice();
            storyNum = storyGen(userInput);
            page = 6;
        }
        break;

        case 6:
        {
            // ����ͼ����
            delay(400);
            // �����������
            // clearEvents();
            cleardevice();
            page = outPicture(userInput.genre, storyNum, &state);
        }
        break;

        case 7:
        {
            // ����ͼ����
            delay(400);
            // �����������
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
            printf("��������ֹ�����ش���༭ҳ�档\n");
            break; // �˳���ѭ��
        }
    }
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <graphics.h>
// #include "mouse.h"
// #include <time.h>
// #include "input.h"

// #define STRESS_TEST_CYCLES 500  // ѹ������ѭ������
// #define MEMORY_MONITOR_INTERVAL 50 // �ڴ��ؼ��

// // �ڴ��ؼ���ʵ�֣�����ݾ���ƽ̨ʵ�֣�
// void check_memory(const char *phase) {
//     // ����������ƽ̨�ض����ڴ������
//     // ����ʹ��GetProcessMemoryInfo��Windows����mallinfo��Linux��
//     printf("[Memory Check] %s\n", phase);
// }

// void critical_path_test() {
//     InputState state;
//     int i, j;
//     int nx, ny, nbuttons;
//     int gd = VGA, gm = VGAHI;             // gd:ͼ������ gm:ͼ��ģʽ
//     IME_Init(&state);
//     initgraph(&gd, &gm, "BGI");  // ��ʼ��ͼ��ģʽ
//     // mread(&MouseX, &MouseY, &MouseS);
//     // ���Թؼ�·��
//     for (i = 0; i < STRESS_TEST_CYCLES; i++) {
//         // 1. ���Գ���ƴ������
//         for (j = 0; j < 35; j++) { // ����32�ֽڻ�����
//             // newmouse(&nx, &ny, &nbuttons);
//             state.inputMode = 1;
//             handleInput(&state, 'a'); // ��Ҫ��handleInput��Ϊ�Ǿ�̬
//         }

//         // 2. �����ļ�·���߽�
//         strcpy(state.pinyin, "veryverylongpinyinstring");
//         getCandidates(&state); // ��Ҫ��getCandidates��Ϊ�Ǿ�̬

//         // 3. ���Ժ�ѡ��Խ�����
//         state.currentPage = 10; // ����ʵ��ҳ��
//         drawCandidates(&state); // ��Ҫ��drawCandidates��Ϊ�Ǿ�̬
//         // delay(30);
//         // 4. �ڴ����в�������
//         if (i % MEMORY_MONITOR_INTERVAL == 0) {
//             check_memory("During stress test");
//         }
//     }

//     closegraph();
// }

// void leak_test() {
//     int i, j;
//     int nx, ny, nbuttons;
//     int gd = VGA, gm = VGAHI;             // gd:ͼ������ gm:ͼ��ģʽ
//     // IME_Init(&state);
//     initgraph(&gd, &gm, "BGI");  // ��ʼ��ͼ��ģʽ
//     mread(&MouseX, &MouseY, &MouseS);
//     // �ظ���ʼ��-�ͷŲ���
//     for (i = 0; i < 100; i++) {
//         InputState* state = (InputState*)malloc(sizeof(InputState));
//         IME_Init(state);

//         // ģ������ʹ��
//         for (j = 0; j < 10; j++) {
//             newmouse(&nx, &ny, &nbuttons);
//             IME_Update(state, nx, ny);
//         }
//         delay(30);
//         // ǿ���ͷ���Դ
//         free(state);
//         check_memory("After free");
//     }

//     closegraph();
// }

// int main() {
//     int gd = VGA, gm = VGAHI;             // gd:ͼ������ gm:ͼ��ģʽ
//     // IME_Init(&state);
//     initgraph(&gd, &gm, "BGI");  // ��ʼ��ͼ��ģʽ
//     mouseinit();
//     check_memory("Before tests");

//     // �ؼ�·��ѹ������
//     critical_path_test();
//     check_memory("After critical test");

//     // �ڴ�й©ר�����
//     leak_test();
//     check_memory("After leak test");

//     return 0;
// }