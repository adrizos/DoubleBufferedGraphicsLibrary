// Alex Drizos cs1550
//Double Buffered Graphics Library - Project 1
//this library file impliments the methods required by the project 1 specifications
//C standard library calls are commented out below and were used in the debugging process
//which I believe could be helpful to see as a part of my design process.

/**Imports**/
#include "graphics.h"
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h> //needed to print debugging statements to txt file
//potentially needed for getkey()
#include <stdio.h>

/**Global Variables**/
int framebuffer;
struct termios oldTerminalSettings;
typedef unsigned short color_t; //color_t type will be a color given as RGB(x,x,x) value converted to single unsigned char
struct fb_var_screeninfo virtual_resolution;
struct fb_fix_screeninfo fixed_resolution;
void* graphics_file_address;

/**Macros**/
#define RGB(r,g,b) ((r<<11) | (g<<5) | (b)) //converts three 16bit values to one unsigned 16bit value
#define ANSI_BLACKSCREEN "\033[2J" //ansi escape code for clearing screen by drawing rectangle of black pixels

void init_graphics() {
        //variables
        struct termios newTerminalSettings;
        int retval;

        if ((framebuffer = open("/dev/fb0", O_RDWR)) < 0) { //neg return value from open is an error
                // const char *text = "ERROR - Open failed in init_graphics";
                // fprintf(efile, "%s\n", text);
                //puts("ERROR - Open failed.");
                //exit(1);
        }

        //allocate memory for graphics mapping
        //determine screen size and bits per pixel

        //get virtual resolution
        retval = ioctl(framebuffer, FBIOGET_VSCREENINFO, &virtual_resolution);
        // if (retval == -1) {
        //         // const char *text = "ERROR - ioctl failed on FBIOGET_VSCREENINFO";
        //         // fprintf(efile, "%s\n", text);
        //         puts("ERROR - ioctl failed on FBIOGET_VSCREENINFO");
        //         exit(4);
        // }
        //next get bit depth
        retval = ioctl(framebuffer, FBIOGET_FSCREENINFO, &fixed_resolution);
        // if (retval == -1) {
        //         // const char *text = "ERROR - ioctl failed on FBIOGET_FSCREENINFO";
        //         // fprintf(efile, "%s\n", text);
        //         puts("ERROR - ioctl failed on FBIOGET_FSCREENINFO");
        //         exit(4);
        // }

        //allocate memory for graphics
        graphics_file_address = mmap(NULL, virtual_resolution.yres_virtual * fixed_resolution.line_length, PROT_READ | PROT_WRITE, MAP_SHARED, framebuffer, 0);
        // if (graphics_file_address == MAP_FAILED) {
        //         // const char *text = "ERROR - mmap failed.";
        //         // fprintf(efile, "%s\n", text);
        //         puts("ERROR - mmap failed.");
        //         exit(6);
        // }

        //clear screen with rectangle of black pixels using ansi escape code
        retval = write(framebuffer, ANSI_BLACKSCREEN, 4);
        //puts(ANSI_BLACKSCREEN);
        // if (retval == -1) {
        //         // const char *text = "ERROR - clear screen with black pixels failed.";
        //         // fprintf(efile, "%s\n", text);
        //         puts("ERROR - clear screen with black pixels failed.");
        //         exit(7);
        // }

        //shell input modifications
        retval = ioctl(0, TCGETS, &oldTerminalSettings); // get current mode / save current settings to be reassigned upon exiting program
        // if (retval == -1) {
        //         // const char *text = "ERROR - ioclt failed";
        //         // fprintf(efile, "%s\n", text);
        //         puts("ERROR - ioclt failed");
        //         exit(3);
        // }
        newTerminalSettings = oldTerminalSettings;
        newTerminalSettings.c_lflag &= ~ECHO; //turns echo off here
        newTerminalSettings.c_lflag &= ~ICANON; //one character at a time
        retval = ioctl(0,TCSETS, &newTerminalSettings); //set to new terminal mode
        // if (retval == -1) {
        //         // const char *text = "ERROR - ioclt failed on new terminal settings.";
        //         // fprintf(efile, "%s\n", text);
        //         puts("ERROR - ioclt failed on new terminal settings.");
        //         exit(3);
        // }
} //end of init_graphics()

void exit_graphics(){
        int retval;
        retval = ioctl(0,TCSETS, &oldTerminalSettings); // get previous mode
        // if (retval == -1) {
        //         // const char *text = "ERROR - ioclt failed restoring settings.";
        //         // fprintf(efile, "%s\n", text);
        //         puts("ERROR - ioclt failed restoring settings.");
        //         exit(3);
        // }
        //clear screen with rectangle of black pixels using ansi escape code
        write(1,"\033[2J",8);
        //fclose(efile);
}

char getkey(){
        char ret_key ='a';
        fd_set rfds;
        struct timeval tv;
        int retval;

        ssize_t bytesRead;

        /* Watch stdin (fd 0) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);

        //Pause as long as 1/10 second, waiting for key press.
        tv.tv_sec = 0;
        tv.tv_usec = 100000;

        retval = select(fileno(stdin)+1, &rfds, NULL, NULL, &tv);
        // if (retval == -1) {
        //         // const char *text = "ERROR - select() failed.";
        //         // fprintf(efile, "%s\n", text);
        //         puts("ERROR - select() failed."); //custom
        //         exit(2); //custom
        // }
        if (FD_ISSET(0, &rfds)) {

                if (retval) {
                        //data is now available so take user input
                        bytesRead = read(0, &ret_key, 1);
                        if (bytesRead == -1) {
                                // const char *text = "ERROR - read() failed.";
                                // fprintf(efile, "%s\n", text);
                                puts("ERROR - read() failed."); //custom
                                exit(3);
                        }
                        //debugging
                        //printf("bytes read: %d\n", bytesRead);
                        //printf("return key: %c\n", ret_key);
                        //exit(3);
                }
        }
        return(ret_key); //return user input
} //end of getkey()

void sleep_ms(long ms){
        nanosleep(ms*1000000, NULL);
}
void clear_screen(void *img){
        int i = 0;
        char* pimg = (char*) img;
        for (i = 0; i < virtual_resolution.yres_virtual * fixed_resolution.line_length; i++) {
                pimg[i] = 0;
        }
}
void draw_pixel(void *img, int x, int y, color_t color){
        unsigned short* pimg = (unsigned short *) img;
        pimg[x + (640*y)] = color;
}

int absValue(int x){
        if (x < 0) {
                x = -x;
        }
        return x;
} //helper function for draw_line

//citation: Bresenham's algorithm helper function outlined at: http://csourcecodes.blogspot.com/2016/06/bresenhams-line-drawing-algorithm-generalized-c-program.html
int sign(x)
{
        if(x>0)
            return 1;
        else if(x<0)
            return -1;
        else
            return 0;
}

//citation: modified version of Bresenham's algorithm outlined at: http://csourcecodes.blogspot.com/2016/06/bresenhams-line-drawing-algorithm-generalized-c-program.html
void draw_line(void *img, int x1, int y1, int x2, int y2, color_t color){

        //check for valid x,y bounds
        // if (x1>639 || x1 <0 || y1 >479 || y1 < 0 || x2>639 || x2 <0 || y2 >479 || y2 < 0){
        //     puts("out of bounds in draw line error (7).");
        //   return;
        // }
        int x,y,dx,dy,swap,temp,s1,s2,p,i;
        x = x1;
        y = y1;
        dx = absValue(x2-x1);
        dy = absValue(y2-y1);
        s1 = sign(x2-x1);
        s2 = sign(y2-y1);
        swap = 0;
        draw_pixel(img, x1,y1,color);

        if(dy>dx) {
                temp=dx;
                dx=dy;
                dy=temp;
                swap=1;
        }

        p = 2*dy-dx;
        for(i = 0; i<dx; i++)
        {
                draw_pixel(img, x,y,color);
                while(p >= 0)
                {
                        p = p-2*dx;
                        if(swap)
                                x+=s1;
                        else
                                y+=s2;
                }
                p = p+2*dy;
                if(swap)
                        y+=s2;
                else
                        x+=s1;
        }
        draw_pixel(img, x2,y2,color);
}
void *new_offscreen_buffer(){
        void* offscreen_buff_add = mmap(NULL, virtual_resolution.yres_virtual * fixed_resolution.line_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        // if (offscreen_buff_add == MAP_FAILED) {
        //         puts("ERROR - mmap offscreen buff failed.");
        //         exit(6);
        // }
        return offscreen_buff_add;
}
void blit(void *src){
        char* psrc = (char *) src;
        char* pdest = (char *) graphics_file_address;
        int i;
        for (i = 0; i < (virtual_resolution.yres_virtual * virtual_resolution.xres_virtual)*2; i ++) {
                pdest[i] = psrc[i];
        }
}
