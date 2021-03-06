#include "InputHandler.h"

int getch (void)
{
    int ch;
    struct termios oldt, newt;
 
    if( -1 == tcgetattr(STDIN_FILENO, &oldt)) {
		printf("tcgetattr error: %s\n",strerror(errno));
		exit(1);
	}
    memcpy(&newt, &oldt, sizeof(newt));
    newt.c_lflag &= ~( ECHO | ICANON | ECHOE | ECHOK |
                       ECHONL | ECHOPRT | ECHOKE | ICRNL);
    if( -1 == tcsetattr(STDIN_FILENO, TCSANOW, &newt)) {
		printf("tcgetattr error: %s\n",strerror(errno));
		exit(1);
	}
    ch = getchar();
    if( -1 == tcsetattr(STDIN_FILENO, TCSANOW, &oldt)) {
		printf("tcgetattr error: %s\n",strerror(errno));
		exit(1);
	}

    return ch;
}

enum KeyTable {
    KeyEnter = 10,
    KeyTab = 9,
    KeyBackSpace = 8
};

string InputHandler::Getline()
{
    history.push_back(string());
    auto line = history.rbegin();

    while(1)
    {
        int ch = getch();
		if(ch == EOF)
			return "";
        if(ch == KeyEnter) {
			putchar('\n');
            return *line;
		}
		if(ch == '\033') {
			getch(); 
			ch = getch();
			if(ch == 'A') // up
            {
                if(line+1 != history.rend()) {
                    putchar('\r');
                    int length = line->size();
                    line++;
                    printf("$ %-*s",length,line->c_str());
                    for( size_t i = 0 ; i < length - line->size() + 2 ; ++i ) {
                        putchar('\b');
                    }
                }
            }
			if(ch == 'B') // down
				putchar('B');
			if(ch == 'C') // right
				putchar('C');
			if(ch == 'D') // left
				putchar('D');
			continue;
		}
        if(ch != KeyTab && ch != KeyBackSpace) {
            putchar(ch);
            *line += ch;
        }

        if(ch == KeyBackSpace) {
            printf("\b\033[K");
            if(line->size() > 0)
                line->pop_back();
        }
    }
}
