#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <signal.h>

#include "Parser.h"
#include "InputHandler.h"
#include "ProcessController.h"

ProcessController procCtrl;

void waitProc()
{
	while( 1 ) {
		int status = 0;
		pid_t pid = waitpid(WAIT_ANY, &status, 0 | WUNTRACED);
		if( pid == -1 ) {
			// do until errno == ECHILD
			// means no more child 
			if(errno != ECHILD)
				printf("waitpid error: %s\n",strerror(errno));
			break;
		}

		printf("waitpid ok: %d\n",pid);

		if( WIFEXITED(status) ) {
			int rc = procCtrl.FreeProcess(pid);
			if( rc == ProcAllDone ) {
				procCtrl.TakeTerminalControl(Shell);
				printf("foreground process group all done");
				return;
			}
		}
		else if( WIFSTOPPED(status) ) {
			procCtrl.TakeTerminalControl(Shell);
			printf("waitpid got Stopped %d\n",pid);
			return;
		}
	}
}

void backToShell(int sig) {
	procCtrl.TakeTerminalControl(Shell);
	procCtrl.SendSignalToFG(SIGTSTP);
	return;
}

void bringToFront()
{
	procCtrl.TakeTerminalControl(ForeGround);
	procCtrl.SendSignalToFG(SIGCONT);
	return;
}

int main()
{
	procCtrl.SetShellPgid(getpgid(getpid()));
	
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	 signal(SIGTSTP, backToShell);
	string line;

	InputHandler InHnd;
	while( 1 ) {
		cout << "$ ";
		line = InHnd.Getline();
		if( line == "" ) {
			continue;
		}
		else if( line == "quit" || line == "exit" ) {
			exit(0);
		}
		else if( line == "fg" ) {
			bringToFront();
		}
		else {
			auto cmds = Parser::Parse(line);

			vector<Executor> exes;
			for( const auto& cmd : cmds ) {
				//cout << cmd;
				exes.emplace_back(Executor(cmd));
			}

			procCtrl.AddProcGroups(exes, line);
			if( Failure== procCtrl.StartProc() ) {
				continue;
			}
		}

		waitProc();
	}

	puts("dead");
	return 0;
}
