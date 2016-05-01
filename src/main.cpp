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

void takeTermCtrl()
{
	pid_t pgid = getpgid(0);
	printf("taking control of pgid = %d\n",pgid);

	if( 0 != tcsetpgrp(0, pgid) )
		puts("setpgrp error");
	if( 0 != tcsetpgrp(1, pgid) )
		puts("setpgrp error");
	if( 0 != tcsetpgrp(2, pgid) )
		puts("setpgrp error");
	ready = true;
}

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
				takeTermCtrl();
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

void backToShell(int sig) {
	printf("[0] %s Passing\n",pgrps[0].originCmds.c_str());
	pgrps[0].PassSignal(SIGSTOP);
	printf("[0] %s Paused\n",pgrps[0].originCmds.c_str());
	takeTermCtrl();
	return;
}

void bringToFront()
{
	pgrps[0].PassSignal(SIGCONT);
	printf("[0] %s Resumed\n",pgrps[0].originCmds.c_str());
	return;
}

int main()
{
	signal(SIGTTOU, SIG_IGN);
    signal(SIGCHLD, childExit);
    signal(SIGINT, killSignal);
    signal(SIGQUIT, killSignal);
	signal(SIGTSTP, backToShell);
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
		if( line != "fg" ) {
			auto cmds = Parser::Parse(line);

			for( size_t i = 0 ; i < cmds.size() ; ++i ) {
				cout << cmds[i] << endl;
				Executor exec(cmds[i]);
				g_exes.push_back(exec);
			}

			pgrps.emplace_back(ProcessGrouper(g_exes));
			ProcessGrouper &pgrp = *pgrps.rbegin();
			pgrp.originCmds = line;
			if( 0 != pgrp.Start()) {
				printf("Error: %s\n",strerror(errno));
				pgrp.PassSignal(SIGKILL);
				continue;
			}
		}
		else {
			//bringToFront();
		}

		while( 1 ) {
			if( ready == false ) {
				usleep(10000);
				//puts("wait...");
			}
			else
				break;
		}
		//puts("next");
    }

	puts("dead");
    return 0;
}
