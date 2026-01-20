#ifndef _REGISTER_H_
#define _REGISTER_H_

#include <graphics.h>
#include "box.h"
#include "save.h"
#include "User.h"
#include "button.h"
#include "hz.h"
#include "mouse.h"

int registerPage(void);
void drawPage_register(Box *accountBox, Box *passwordBox1, Box *passwordBox2, Button *registerButton, Button *cancelButton);

#endif