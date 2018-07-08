/*
Alex Drizos
Cs1550
Project1
myDriver.c
Dependencies: graphics.h library.c

This program illustrates another use of the required methods in the graphics
library, library.c file and associated graphics.h header file.

Instructions:
User may enter 'd' to draw a diagonal line across the entire screen making an "X"

User may enter 's' to draw repeating 1 pixel wide square lines around the entire
screen that loops and draws consecutive inner circles until screen is filled.
    The colors change every 16 iterations in repeating order of the ROYGBIV
    color palette.

User may enter 'q' to safely quit the program
*/

//includes
#include "graphics.h"

//global variables
char userKey; //variable to store user input
int counter = 1;
int direction = 0;
int curr_x = 0;
int curr_y = 0;

//functions

//converts rgb values to 5 bit equivalents
convertRGB5bit(int x) {
    x  = (x / 8);
    return x;
}
void drawCircle(void *img, int counter, int r, int g, int b) {
}//end of drawCircle

void drawDiagonal(void *img, int counter, int r, int g, int b){
    draw_line(img, 0, 0, 639, 479, RGB(r,g,b));
    draw_line(img, 639, 0, 0, 479, RGB(r,g,b));

}//end of drawDiagonal
void drawSquare(void *img, int counter, int r, int g, int b, int i) {
    //i is passed here from the for loop counter and will increment at a slope
    //from the origin at 0,0 to the middle of the display
    curr_x = i;
    curr_y = i;

    //declare variables for second coorindate of the line
    int new_x, new_y;
    //iterate 4 times to complete square from lines
    int p;
    for (p = 0; p < 4; p++) {
        //update coordinates
        if (p == 0){
            //the first iteration already has correct (x1,y1)
            new_x = 639 - i;
            new_y = i;

            draw_line(img, curr_x, curr_y, new_x, new_y, RGB(r,g,b));
        }
        //subsequent iterations need both cooridinates updated
        //move cooridnate clockwise around points of the square
        else if (p == 1) {
            //top right most point
            curr_x = new_x;
            curr_y = new_y;
            new_x = 639 - i;
            new_y = 479 - i;
            draw_line(img, curr_x, curr_y, new_x, new_y, RGB(r,g,b));
        }
        else if (p == 2) {
            //bottom right point
            curr_x = new_x;
            curr_y = new_y;
            new_x = i;
            new_y = 479 - i;
            draw_line(img, curr_x, curr_y, new_x, new_y, RGB(r,g,b));
        }
        else if (p == 3) {
            //bottom left point
            curr_x = new_x;
            curr_y = new_y;
            new_x = i;
            new_y = i;
            draw_line(img, curr_x, curr_y, new_x, new_y, RGB(r,g,b));
        }
    }//end of for loop
}//end of drawSquare


void userMode(){
    //initialize graphics library
    init_graphics();
    //create offscreen buffer for drawing to
    void *buff = new_offscreen_buffer();
    //color variable declarations
    //each index represents their value for ROYGBIV color palatte
    int r[7] = {31, 31, 31, 1, 1, 24, 14};
    int g[7] = {0, 14, 31, 30, 5, 1, 1};
    int b[7] = {0, 0, 0, 3, 30, 30, 30};
    //loop for user interaction
    do {
        //coordinate declarations
        curr_x = 0;
        curr_y = 0;

        userKey = getkey();
        if (userKey == 'q') {
            break; //user wants to quit
        }
        else if (userKey == 's') {
            //draws circles in various colors from the outside of the display
            //to the inside of the display
            //colors cycle through roygbiv over and over again via % operator
            int colorCounter = 0; //keeps track of the color index for R G and B
                                  //for each of the 5 iterations that it stays the same
            int i;
            for (i = 0; i < 240; i++) {
                //only change color each 16 iterations to give thicker blocks of each color
                if (i == 0){
                    //start with red, without this it will change on iteration zero and
                    //start with an orange block
                    drawSquare(buff, counter, r[(colorCounter)], g[colorCounter], b[colorCounter], i);
                }
                else if ((i % 16) == 0){
                    //change color
                    drawSquare(buff, counter, r[(i%7)], g[i%7], b[i%7], i);
                    colorCounter = (colorCounter + 1) % 7;
                }
                else {
                    //keep previous color
                    drawSquare(buff, counter, r[(colorCounter)], g[colorCounter], b[colorCounter], i);
                }
                blit(buff);
                sleep_ms(500);
            }
        }
        else if (userKey == 'd') {
          //draw two diagonal lines to show proper screen canvas size is used
          drawDiagonal(buff, counter, r[0], g[0], b[0]);
          blit(buff);
        }
        //increment counter, this helps change the location of the next shape
        counter++;
        //delay
        sleep_ms(200);
    }
    while (1);
    exit_graphics(); //clean up and exit
}//end of userMode

int main (int argc, char **argv) {
    //paint canvas black to clear screen
    write(1,"\033[2J",8);
    //start userMode
    userMode();
    return 0;
} //end of main
