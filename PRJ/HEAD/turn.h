#ifndef _TURN_H_
#define _TURN_H_

#include <graphics.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <time.h>
#include <stdio.h>
#include "hz.h"
#include "mouse.h"

#define BUTTON1_X 100
#define BUTTON2_X 270
#define BUTTON3_X 440
#define BOX_Y 250
#define BOX_WIDTH 120
#define BOX_HEIGHT 60
#define BUTTON_SPACING 30

int turnPage();
void draw3dButton(int x, int y, int width, int height, char *text, int pressed);

#endif
