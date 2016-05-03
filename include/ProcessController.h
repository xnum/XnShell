#pragma once

enum SimpleRetVal {
	Failure,
	Success
};

enum PgidTarget {
	Shell = -1,
	ForeGround = 0
};

namespace xnsh {
	class ProcessController;
};

#include "Executor.h"
#include "ProcessGrouper.h"

using xnsh::ProcessController;

class ProcessController {
	public:
		int AddProcGroups(const vector<Executor>&, const string& cmd);
		int StartProc();
		int TakeTerminalControl(pid_t);
		void SetShellPgid(pid_t p) { shellPgid = p; }
		int FreeProcess(pid_t);
		int SendSignalToFG(int sig);
		void BackToShell(int sig);
		void BringToFront(int index);
		void printJobs();
	private:
		vector<ProcessGrouper> pgrps;
		int fgIndex;
		pid_t shellPgid;
};
