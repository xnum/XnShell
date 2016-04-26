#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace xnsh {
    class Command;
    class Parser;
};

using xnsh::Command;
using xnsh::Parser;

class Command {
    public:
        string name;
        vector<string> args; 
        string redirectStdout;
        string redirectStdin;
        string originStr; // for debug
        bool isSyntaxError;

        Command() {}
        
};

ostream &operator<<(ostream &os, const Command &cmd);

class Parser {
    public:
        static vector<Command> Parse(string line);

    private:
        static vector<string> split(const string &source, const string &delim);
        static string trim(const string& str);

        static Command takeCommand(string);

        static const string PIPE_DELIM;
};

