#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

typedef struct {
    unsigned char r, g, b;
} RGB;

RGB palette[256];

void reset_palette() {
    int i;
    outportb(0x3C8, 0);
    for (i = 0; i < 256; i++) {
        outportb(0x3C9, 0);
        outportb(0x3C9, 0);
        outportb(0x3C9, 0);
    }
}

void get_vga_palette() {
    int i;
    for (i = 0; i < 256; i++) {
        outportb(0x3C7, i);
        palette[i].r = inportb(0x3C9);
        palette[i].g = inportb(0x3C9);
        palette[i].b = inportb(0x3C9);
    }
}

void display_palette() {
    int i;
    int x = 0, y = 0;
    for (i = 0; i < 256; i++) {
        setfillstyle(SOLID_FILL, i);
        bar(x, y, x + 20, y + 20);
        x += 25;
        if (x + 25 > getmaxx()) {
            x = 0;
            y += 25;
        }
    }
}

void main() {
    int gd = VGA, gm = VGAMED;
    initgraph(&gd, &gm, "C:\\PRJ\\BGI"); // 确保路径正确

    reset_palette();
    get_vga_palette();
    display_palette();

    getch();
    display_palette();
    getch();
    display_palette();
    getch();
    display_palette();
    closegraph();
}