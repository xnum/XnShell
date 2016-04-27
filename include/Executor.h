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
        Executor(const Command& cmd);
        int PipeWith(Executor&);
        int Start();

    private:
        Command cmdHnd;
        pid_t pid;
};
