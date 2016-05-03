#include "ProcessController.h"

int ProcessController::AddProcGroups(const vector<Executor>& exes, const string& cmd)
{
	pgrps.emplace_back(ProcessGrouper(exes));
	ProcessGrouper &pgrp = *pgrps.rbegin();
	pgrp.originCmds = cmd;
	return 0;
}

int ProcessController::StartProc()
{
	if( pgrps.size() == 0 ) {
		printf("Fatel Error: No processes could be start\n");
		exit(1);
	}

	ProcessGrouper &pgrp = *pgrps.rbegin();
	if( pgrp.Start() != 0 ) {
		printf("Error: %s\n",strerror(errno));
		pgrp.PassSignal(SIGKILL);
		return Failure;
	}

	fgIndex = pgrps.size() -1;
	TakeTerminalControl(pgrp.GetPgid());

	return Success;
}

int ProcessController::TakeTerminalControl(pid_t pgid)
{
	pid_t target = pgid;
	if( pgid == Shell ) {
		target = shellPgid;
	}

	if( pgid == ForeGround ) {
		//printf("current fg process group index: %d\n",fgIndex);
		if( fgIndex != Shell )
			target = pgrps[fgIndex].GetPgid();
		else
			target = shellPgid;
	}

	//printf("Take Terminal Control setpgrp(%d)\n",target);
	fflush(stdout);

	if( 0 != tcsetpgrp(0, target) ) {
		puts("setpgrp error");
		puts(strerror(errno));
	}
	if( 0 != tcsetpgrp(1, target) ) {
		puts("setpgrp error");
		puts(strerror(errno));
	}
	if( 0 != tcsetpgrp(2, target) ) {
		puts("setpgrp error");
		puts(strerror(errno));
	}

	return 0;
}

int ProcessController::FreeProcess(pid_t pid) 
{
	for( size_t i = 0 ; i < pgrps.size() ; ++i ) {
		int rc = pgrps[i].NotifyTerminated(pid);
		if( rc != ProcNotMine ) {
			if( rc == ProcAllDone ) {
				pgrps.erase(pgrps.begin() + i);
				if( fgIndex == i ) {
					fgIndex = pgrps.size()-1;
					return ProcAllDone;
				}
			}
			break;
		}
	}
	return ProcNotAllDone;
}

int ProcessController::SendSignalToFG(int sig) 
{
	kill(pgrps[fgIndex].GetPgid(),sig);
	return 0;
}

void ProcessController::BackToShell(int sig)
{
	TakeTerminalControl(Shell);
	SendSignalToFG(SIGTSTP);
}

void ProcessController::BringToFront(int index)
{
	if( index == -1 ) {
		//printf("FG default\n");
		TakeTerminalControl(ForeGround);
		SendSignalToFG(SIGCONT);
	}
	else if( 0 <= index && index < pgrps.size() ) {
		//printf("FG spec\n");
		fgIndex = index;
		TakeTerminalControl(ForeGround);
		SendSignalToFG(SIGCONT);
	}
	else {
		printf("Error: index out of range\n");
	}
}

void ProcessController::printJobs()
{
	for( size_t i = 0 ; i < pgrps.size() ; ++i ) {
		printf("\n=== Process Group %lu ===\n",i);
		if(fgIndex == i)puts("Foreground Process Group");
		else			puts("Background Process Group");
		printf("Cmd: %s\n",pgrps[i].originCmds.c_str());
		printf("pgid: %d\n",pgrps[i].GetPgid());
	}
}







