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
		if( fgIndex != Shell )
			target = pgrps[fgIndex].GetPgid();
		else
			target = shellPgid;
	}

	printf("setpgrp(%d)\n",target);
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
				if( fgIndex == i )
					return ProcAllDone;
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









