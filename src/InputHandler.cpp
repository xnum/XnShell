#include "InputHandler.h"

int getch (void)
{
    int ch;
    struct termios oldt, newt;
 
    tcgetattr(STDIN_FILENO, &oldt);
    memcpy(&newt, &oldt, sizeof(newt));
    newt.c_lflag &= ~( ECHO | ICANON | ECHOE | ECHOK |
                       ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
 
    return ch;
}

enum KeyTable {
    KeyEnter = 10,
    KeyTab = 9,
    KeyBackSpace = 8
};

string InputHandler::Getline()
{
    string line;
    while(1)
    {
        int ch = getch();
		//printf("getting [%d]\n",ch);
        if(ch == KeyEnter) {
			putchar('\n');
            return line;
		}
		if(ch == '\033') {
			getch(); 
			ch = getch();
			if(ch == 'A')
				putchar('A');
			if(ch == 'B')
				putchar('B');
			if(ch == 'C')
				putchar('C');
			if(ch == 'D')
				putchar('D');
			continue;
		}
        if(ch != KeyTab && ch != KeyBackSpace) {
            putchar(ch);
            line += ch;
        }

        if(ch == KeyBackSpace) {
            printf("\b\033[K");
            if(line.size() > 0)
                line.pop_back();
        }
    }
}
