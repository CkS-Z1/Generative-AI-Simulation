#ifndef _BOX_H_
#define _BOX_H_

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> /* 控制台输入输出 */
#include <graphics.h>
#include "hz.h"
#include "mouse.h"

typedef struct
{
    int x, y, width, height;
    char *text;
} Box;

void initBox(Box *box, int x, int y, int width, int height, char *text);
void drawBox(Box box);
void destroyBox(Box *box);
void putText(Box *box, char *input, int *inputLen);
void putPassword(Box *box, char *password, int *passwordLen);

#endif