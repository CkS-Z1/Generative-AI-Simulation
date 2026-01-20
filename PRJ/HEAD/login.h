#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "box.h"
#include "hz.h"
#include "mouse.h"
#include "input.h"
#include "button.h"
#include <conio.h>
#include <graphics.h>
#include "User.h"
#include "logindat.h"

void drawPage_login(Box *accountBox, Box *passwordBox, Button *loginButton);
int loginPage(void);

#endif
