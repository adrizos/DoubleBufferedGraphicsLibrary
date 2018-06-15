// Project 1: fix - turn echo on that was left on due to program crash.
#include <termios.h>
#include <stdlib.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
  int retval;
	struct termios oldTerminalSettings,
				   newTerminalSettings;

	retval = ioctl(0,TCGETS,&oldTerminalSettings); // get current mode
	if (retval == -1) {
		puts("ERROR--ioctl failed.");
        exit(3);
    }
	newTerminalSettings = oldTerminalSettings;
	newTerminalSettings.c_lflag |= ECHO; /* echo on */
	newTerminalSettings.c_lflag |= ICANON; /* full line */
	retval = ioctl(0,TCSETS,&newTerminalSettings); /* set new terminal mode */
	if (retval == -1) {
		puts("ERROR--ioctl failed on newTerminalSettings.");
        exit(4);
    }

	return 0;
}
