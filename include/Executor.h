#pragma once

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Parser.h"

namespace xnsh {
    class Executor;
};

using xnsh::Executor;

class Executor {
    public:
        bool done;
        pid_t pid;

        Executor(const Command& cmd);
        int PipeWith(Executor&);
        int Start();

    protected:
        Command cmdHnd;
};
