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

	void CloseAllPipe(const vector<Executor>&);
};

using xnsh::Executor;

class Executor {
    public:
        bool done;
        pid_t pid;
        int fd[2][2];
        Command cmdHnd;

        Executor(const Command& cmd);
        int PipeWith(Executor&);
        int Start(const vector<Executor>&,int);

};

