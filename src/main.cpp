#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <signal.h>

#include "Executor.h"
#include "Parser.h"
#include "InputHandler.h"
#include "ProcessGrouper.h"

vector<ProcessGrouper> pgrps;
bool ready = true;

void childExit(int sig) {
    int status = 0;
    pid_t pid = wait(&status);
    if( pid < 0 ) {
        printf("wait error\n");
    }
    else {
        printf("catch a child %d\n",pid);
    }

	for( size_t i = 0 ; i < pgrps.size() ; ++i ) {
		int rc = pgrps[i].NotifyTerminated(pid);
		if( rc != ProcNotMine ) {
			if( rc == ProcAllDone ) {
				pgrps.erase(pgrps.begin() + i);
				ready = true;
			}
			break;
		}
	}

    return;
}

void killSignal(int sig) {
	if(pgrps.size() >= 1) 
		pgrps[0].PassSignal(sig);
	else
		exit(0);
	return;
}

int main()
{
    signal(SIGCHLD, childExit);
    signal(SIGINT, killSignal);
    signal(SIGQUIT, killSignal);
    //char* const envp[] = { 0, NULL };
    string line;

    InputHandler InHnd;
    while( 1 ) {
		ready = false;
		vector<Executor> g_exes;
        cout << "$ ";
        line = InHnd.Getline();
		if( line == "" )
			continue;
        auto cmds = Parser::Parse(line);

        for( size_t i = 0 ; i < cmds.size() ; ++i ) {
            //cout << cmds[i] << endl;
            Executor exec(cmds[i]);
            g_exes.push_back(exec);
        }

		pgrps.emplace_back(ProcessGrouper(g_exes));
		ProcessGrouper &pgrp = *pgrps.rbegin();
		if( 0 != pgrp.Start()) {
			printf("Error: %s\n",strerror(errno));
			pgrp.PassSignal(SIGKILL);
			continue;
		}

		while( 1 ) {
			if( ready == false )
				usleep(10000);
			else
				break;
		}
    }
    return 0;
}
