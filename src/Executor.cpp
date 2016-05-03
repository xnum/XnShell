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

int Executor::Start(const vector<Executor>& pgrp, int index)
{
    char* const* argv = cmdHnd.toArgv();

    pid_t rc = fork();

    if( rc < 0 ) { // fail
        cout << "Fork() error" << endl;
        return rc;
    }

    if( rc == 0 ) { // child
		if(index==0)setpgid(0,0);
        if( cmdHnd.redirectStdout != "" )
            freopen(cmdHnd.redirectStdout.c_str(), "w+", stdout);
        if( cmdHnd.redirectStdin != "" )
            freopen(cmdHnd.redirectStdin.c_str(), "r+", stdin);
        if( fd[0][0] != -1 ) {
            dup2(fd[0][0],0);
            close(fd[0][1]);
			fd[0][1] = -1;
        }
        if( fd[1][1] != -1 ) {
            dup2(fd[1][1],1);
            close(fd[1][0]);
			fd[1][0] = -1;
        }
		CloseAllPipe(pgrp);
        execvp(argv[0],argv);
    }

	if( kill(pid, 0) ) {
		exit(1);
	}


    pid = rc;

    return 0;
}

void xnsh::CloseAllPipe(const vector<Executor> &g_exes)
{
	set<int> closed_fd;
	for( size_t i = 0 ; i < g_exes.size() ; ++i ) {
		for( int j = 0 ; j < 2 ; ++j ) {
			for( int k = 0 ; k < 2 ; ++k ) {
				const bool is_in = closed_fd.find(g_exes[i].fd[j][k]) != closed_fd.end();
				if( g_exes[i].fd[j][k] != -1 && !is_in ) {
					close(g_exes[i].fd[j][k]);
					closed_fd.insert(g_exes[i].fd[j][k]);
				}
			}
		}
	}
}
