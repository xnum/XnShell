#include "Executor.h"

Executor::Executor(const Command& cmd)
{
    cmdHnd = cmd;
    done = false;
    fd[0][0] = -1;
    fd[0][1] = -1;
    fd[1][0] = -1;
    fd[1][1] = -1;
}

int Executor::PipeWith(Executor& rhs) {
    int pfd[2] = {};
    if( pipe(pfd) == 0 ) {
        fd[1][0] = pfd[0];
        fd[1][1] = pfd[1];
        rhs.fd[0][0] = pfd[0];
        rhs.fd[0][1] = pfd[1];
    }
    else {
        printf("pipe error");
    }
    return 0;
}

int Executor::Start()
{
    char* const* argv = cmdHnd.toArgv();

    pid_t rc = fork();

    if( rc < 0 ) { // fail
        cout << "Fork() error" << endl;
        return rc;
    }

    if( rc == 0 ) { // child
        if( cmdHnd.redirectStdout != "" )
            freopen(cmdHnd.redirectStdout.c_str(), "w+", stdout);
        if( cmdHnd.redirectStdin != "" )
            freopen(cmdHnd.redirectStdin.c_str(), "r+", stdin);
        if( fd[0][0] != -1 ) {
            close(0);
            dup2(fd[0][0],0);
            close(fd[0][1]);
        }
        if( fd[1][1] != -1 ) {
            close(1);
            dup2(fd[1][1],1);
            close(fd[1][0]);
        }
        execvp(argv[0],argv);
    }

    pid = rc;

    return 0;
}
