#include "Parser.h"

const string Parser::PIPE_DELIM = "|";

ostream &operator<<(ostream &os, const Command &cmd)
{
    return os << "[" + cmd.originStr + "]";
}

vector<Command> Parser::Parse(string line)
{
    vector<Command> ret;

    vector<string> commands = split(line, PIPE_DELIM);

    for( string str : commands ) {
        ret.push_back(takeCommand(str));
    }

    return ret;
}

/*
http://ericsilverblade.blogspot.tw/2013/03/csplit.html
*/
vector<string> Parser::split(const string &source, const string &delim)
{
    vector<string> ans;
    size_t begin_pos=0, end_pos=source.find(delim); 
    while(end_pos!=string::npos) {
        ans.push_back(source.substr(begin_pos, end_pos-begin_pos)); 
        begin_pos = end_pos+delim.size();
        end_pos = source.find(delim, begin_pos);  
    }
    ans.push_back(source.substr(begin_pos, end_pos-begin_pos));  
    return ans;
}

/*
http://stackoverflow.com/questions/25829143/c-trim-whitespace-from-a-string
*/
string Parser::trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

Command Parser::takeCommand(string str)
{
    Command ret;
    str = trim(str);
    ret.originStr = str;
    str += ' ';

    size_t begin_pos = 0, end_pos = 0;

    for( size_t i = 0 ; i < str.size() ; ++i ) {
        if( str[i] == ' ' || str[i] == '>' || str[i] == '<' ) {
            end_pos = i;
            if( begin_pos >= end_pos ) {
                fprintf(stderr, "Syntax error by position underflow");
                ret.isSyntaxError = true;
                return ret;
            }

            string pat = str.substr( begin_pos == 0 ? 0 : begin_pos+1
                    , end_pos-begin_pos);
            cout << pat << endl;
            if( str[i] == ' ' && begin_pos == 0 )
                ret.name = pat;
            if( str[i] == ' ' && begin_pos != 0 )
                ret.args.push_back(pat);
            if( str[i] == '>' && ret.redirectStdout == "" )
                ret.redirectStdout = pat;
            if( str[i] == '>' && ret.redirectStdout != "" ) {
                fprintf(stderr, "Syntax error by duplicate stdout redirection");
                ret.isSyntaxError = true;
                return ret;
            }
            if( str[i] == '<' && ret.redirectStdin == "" )
                ret.redirectStdin = pat;
            if( str[i] == '<' && ret.redirectStdin != "" ) {
                fprintf(stderr, "Syntax error by duplicate stdin redirection");
                ret.isSyntaxError = true;
                return ret;
            }

            begin_pos = i;
        }
    }

    cout << endl;

    return ret;
}
