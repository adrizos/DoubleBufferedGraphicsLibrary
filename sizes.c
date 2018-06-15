//helper file to check type sizes in the particular QEMU Linux VM 
#include <stdlib.h>

int main(int argc, char **argv){

  int integerV = 0;
  short shortV = 0;
  char charV = 0;

  printf("int %d, short %d, char %d \n",sizeof(integerV), sizeof(shortV), sizeof(charV) );
}
