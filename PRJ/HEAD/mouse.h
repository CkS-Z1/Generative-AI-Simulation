#ifndef _mouse_h_ // 防止头文件重复包�?
#define _mouse_h_

// 函数声明
int mouse_press(int x1, int y1, int x2, int y2); // 检测鼠标是否在指定区域内点�?
void mouse(int, int);                            // 绘制鼠标光标
void mouseinit(void);                            // 初始化鼠�?
void mread(int *nx, int *ny, int *nbuttons);     // 读取鼠标当前位置和按钮状�?
void save_bk_mou(int x, int y);                  // 保存鼠标当前位置的背�?
void clrmous(int x, int y);                      // 清除鼠标光标
void clearEvents();                              // 清除鼠标事件队列
void drawmous(int x, int y);                     // 绘制鼠标光标
void newmouse(int *nx, int *ny, int *nbuttons);  // 更新鼠标位置和状�?

// 全局变量声明
extern int MouseX;      // 鼠标当前X坐标
extern int MouseY;      // 鼠标当前Y坐标
extern int MouseS;      // 鼠标状态（按钮状态等�?
extern int press;       // 鼠标按键状态（是否按下�?
extern union REGS regs; // DOS中断寄存器结�?

#endif