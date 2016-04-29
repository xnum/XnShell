#pragma once

#include <cstdio>
#include <cstdlib>
#include <set>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Parser.h"


namespace xnsh {
    class Executor;

	void CloseAllPipe();
};

using xnsh::Executor;

class Executor {
    public:
        bool done;
        pid_t pid;
        int fd[2][2];

        Executor(const Command& cmd);
        int PipeWith(Executor&);
        int Start();

    protected:
        Command cmdHnd;
};


extern vector<Executor> g_exes;
