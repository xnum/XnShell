#pragma once

#include "Executor.h"

namespace xnsh {
	class ProcessGrouper;
};

using xnsh::ProcessGrouper;

enum StatusResult {
	ProcAllDone,
	ProcNotAllDone,
	ProcNotMine
};

class ProcessGrouper {
	public:

		ProcessGrouper(vector<Executor> exes) :
			executors(exes) {}

		int Start();
		int NotifyTerminated(pid_t);
		int PassSignal(int sig);

	private:
		vector<Executor> executors;
};
