/*** includes ***/

#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** data ***/
struct termios orig_termios;

/*** terminal ***/
void die(const char *s) {
	perror(s);
	exit(1);
}

void disableRawMode() {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		die("tcsetattr");
}

void enableRawMode() {
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
		die("tcgetattr");
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("tcsetattr");
}

/*** init ***/

int main() {
	/* Start off in raw mode. This allows text to be 
	 * input one character at a time as opposed to 
	 * waiting for <return> key
	 */
	enableRawMode();

        // Read in characters from terminal until q
	char c;
	while (1) {
		char c = '\0';
	        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
			die("read");
		//Print characters to terminal
	       	if (iscntrl(c)) {
		       printf("%d\n",c);
	       	} else {
		       printf("%d ('%c')\r\n", c, c);
	       	}
	       	if (c=='q') break;
	}

	return 0;
}
