//Alex Drizos cs1550 Project 1
//graphics library header file

typedef unsigned short color_t;
#define RGB(a, b, c) ((a<<11) | (b<<5) | (c))


void init_graphics();
void exit_graphics();
char getkey();
void sleep_ms(long ms);
void *new_offscreen_buffer();
void clear_screen(void *img);
void draw_pixel(void *img, int x, int y, color_t color);
void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c);
void blit(void *src);
