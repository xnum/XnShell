#pragma once

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <termios.h>  // for tcxxxattr, ECHO, etc ..
#include <unistd.h>    // for STDIN_FILENO

using namespace std;

namespace xnsh {
    class InputHandler;
};

using xnsh::InputHandler;

class InputHandler {
    public:
        string Getline();
    private:
        vector<string> history;
};
