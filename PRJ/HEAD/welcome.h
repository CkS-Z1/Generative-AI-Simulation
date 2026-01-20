#ifndef _WELCOME_H_
#define _WELCOME_H_

#include <graphics.h>
#include <dos.h>
#include "hz.h"
#include "mouse.h"
#include "button.h"

#define BUTTON_LOGIN 1    // 转到登陆界面
#define BUTTON_REGISTER 2 // 转到注册界面
#define BUTTON_EXIT 999   // 退出值

int welcomePage(void); // 绘制欢迎界面

#endif