#include "bitmap.h"
#include "font8x8.h"
#include "vesa.h"

void bitmap_draw_char(char ch, int x, int y, int color) {
    int temp = 0, pix_data = 0;

    for (int i = 0; i < BITMAP_SIZE; i++) {
        temp = x;
        x += BITMAP_SIZE;
        pix_data = font8x8_basic[(int)ch][i];
        while (pix_data > 0) {
            if (pix_data & 1) {
                vbe_putpixel(x, y, color);
            }
            pix_data >>= 1;
            x++;
        }
        x = temp;
        y++;
    }
}

void bitmap_draw_string(const char *str, int x, int y, int color) {
    int new_x = x;
    int new_y = y;
    while (*str) {
        bitmap_draw_char(*str, new_x, new_y, color);
        str++;
        new_x += 14;
        if (new_x > (int)vbe_get_width()) {
            new_y += 17;
            new_x = x;
        }
    }
}