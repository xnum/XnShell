#include "Executor.h"

Executor::Executor(const Command& cmd)
{
    cmdHnd = cmd;
}

int Executor::Start()
{
    char* const* argv = cmdHnd.toArgv();

    pid_t rc = fork();

    if( rc < 0 ) { // fail
        cout << "Fork() error" << endl;
        return rc;
    }

    if( rc > 0 ) { // parent
        if( waitpid(rc, NULL, 0) != rc ) {
            cout << "Wait child error" << endl;
            return -1;
        }
    }

    if( rc == 0 ) { // child
        if( cmdHnd.redirectStdout != "" )
            freopen(cmdHnd.redirectStdout.c_str(), "w+", stdout);
        if( cmdHnd.redirectStdin != "" )
            freopen(cmdHnd.redirectStdin.c_str(), "r+", stdin);
        execvp(argv[0],argv);
    }

    return 0;
}
