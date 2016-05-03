#pragma once

#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

namespace xnsh {
    class Command;
    class Parser;
};

using xnsh::Command;
using xnsh::Parser;

enum CommandResult {
    CmdRes_Ok = 0,
    CmdRes_PosErr,
    CmdRes_DupOutRe,
    CmdRes_DupInRe
};

class Command {
    public:
        string name;
        vector<string> args; 
        string redirectStdout;
        string redirectStdin;
        string originStr; // for debug
        int isSyntaxError;
        char **argv;

        Command() { isSyntaxError = CmdRes_Ok; argv = NULL; }
        Command(const string& nam,
                const vector<string>& arg,
                const string& reOut,
                const string& reIn,
                int synErr)
            : name(nam), args(arg), redirectStdout(reOut), redirectStdin(reIn), isSyntaxError(synErr) { argv = NULL; }
        ~Command();
        bool operator== (const Command& rhs) const;        
        char* const* toArgv();
};

ostream &operator<<(ostream &os, const Command &cmd);

class Parser {
    public:
        static vector<Command> Parse(string line,int &isfg);

    private:
        static vector<string> split(const string &source, const string &delim);
        static string trim(const string& str);

        static Command takeCommand(string);

        static const string PIPE_DELIM;
};

