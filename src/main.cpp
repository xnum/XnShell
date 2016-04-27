#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "Executor.h"
#include "Parser.h"

int main()
{
    //char* const envp[] = { 0, NULL };
    string line;

    cout << "$ ";
    while( getline(cin,line) ) {
        auto cmds = Parser::Parse(line);
        for( size_t i = 0 ; i < cmds.size() ; ++i ) {
            cout << cmds[i] << endl;
            Executor exec(cmds[i]);
            exec.Start(); 
        }
        cout << "$ ";
    }
    return 0;
}
