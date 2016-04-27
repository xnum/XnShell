#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "Executor.h"
#include "Parser.h"
#include "InputHandler.h"

int main()
{
    //char* const envp[] = { 0, NULL };
    string line;

    InputHandler InHnd;
    while( 1 ) {
        cout << "$ ";
        line = InHnd.Getline();
        auto cmds = Parser::Parse(line);
        for( size_t i = 0 ; i < cmds.size() ; ++i ) {
            cout << cmds[i] << endl;
            Executor exec(cmds[i]);
            exec.Start(); 
        }
    }
    return 0;
}
