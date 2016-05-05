#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <signal.h>

#include "Parser.h"
#include "InputHandler.h"
#include "ProcessController.h"
#include "BuiltinHelper.h"

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

		if( WIFEXITED(status) ) {
			int rc = procCtrl.FreeProcess(pid);
			if( rc == ProcAllDone ) {
				procCtrl.TakeTerminalControl(Shell);
				return;
			}
		}
		else if( WIFSTOPPED(status) ) {
			procCtrl.TakeTerminalControl(Shell);
			return;
		}
	}
}

void backToShell(int sig __attribute__((unused))) {
	procCtrl.BackToShell();
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
		int fg=0;
		cout << "$ ";
		line = InHnd.Getline();
		if( line == "" ) {
			procCtrl.TakeTerminalControl(Shell);
			procCtrl.RefreshJobStatus();
			printf("\b\b  \b\b");
			continue;
		}
		else if( BuiltinHelper::isSupportCmd(line) ) {
			int rc = BuiltinHelper::RunBuiltinCmd(line);
			if( BH_IF_IS(rc, CONTINUE) )
				continue;
			if( BH_IF_IS(rc, RUN_FAIL) )
				continue;
		}
		else {
			auto cmds = Parser::Parse(line,fg);

			vector<Executor> exes;
			for( const auto& cmd : cmds ) {
				//cout << cmd;
				exes.emplace_back(Executor(cmd));
			}

			procCtrl.AddProcGroups(exes, line);
			if( Failure == procCtrl.StartProc(fg==0 ? true : false) ) {
				continue;
			}
		}

		if(fg == 0)waitProc();
	}

	puts("dead");
	return 0;
}
