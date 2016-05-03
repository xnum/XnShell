#include "Parser.h"

const string Parser::PIPE_DELIM = "|";

Command::~Command()
{
    if(argv!=NULL)
    {
        char **ptr = argv;
        while( *ptr != NULL )
            free(*ptr++);
        free(argv);
    }
}

bool Command::operator== (const Command& rhs) const
{
    if( isSyntaxError != rhs.isSyntaxError )
        return false;
    if( isSyntaxError == rhs.isSyntaxError && isSyntaxError != CmdRes_Ok )
        return true;
    if( name != rhs.name )
        return false;
    if( redirectStdout != rhs.redirectStdout )
        return false;
    if( redirectStdin != rhs.redirectStdin )
        return false;
    if( args.size() != rhs.args.size() )
        return false;
    for( size_t i = 0 ; i < args.size() ; ++i )
        if( args[i] != rhs.args[i] )
            return false;
    return true;
}

char* const* Command::toArgv()
{
    argv = (char**)calloc(args.size()+2, sizeof(char*));
    argv[0] = strdup(name.c_str());
    for(size_t i = 0 ; i < args.size() ; ++i )
        argv[i+1] = strdup(args[i].c_str());
    argv[args.size()+1] = NULL;

    return argv;
}

ostream &operator<<(ostream &os, const Command &cmd)
{
    string out = "\n======\n";
    out += "name: " + cmd.name + "\n";
    for( size_t i = 0 ; i < cmd.args.size() ; ++i ) {
        out += "arg[" + std::to_string(i);
        out += "] = ";
        out += cmd.args[i];
        out += "\n";
    }
    out += "reOut: " + cmd.redirectStdout + "\n";
    out += "reIn: " + cmd.redirectStdin + "\n";
    out += "Err: ";
    out += std::to_string(cmd.isSyntaxError);
    out += "\n======\n";
    return os << out;
}

vector<Command> Parser::Parse(string line,int &isfg)
{
    vector<Command> ret;

    vector<string> commands = split(line, PIPE_DELIM);

    for( string str : commands ) {
        str = trim(str);
        if(str!="")
            ret.push_back(takeCommand(str));
    }

	if( ret.size() == 0 )
		return ret;
	if( ret[0].args.size() == 0 )
		return ret;

	Command& last = *ret.rbegin();

	if( *(last.args.rbegin()) == "&" ) {
		isfg = 1;
		(last.args).pop_back();
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
    if(str.size()==0)
        return "";
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

Command Parser::takeCommand(string str)
{
    Command ret;
    ret.originStr = str;

    string reBuildStr = "";

    bool emitSyntax = false;
    for( size_t i = 0 ; i < str.size() ; ++i ) {
        if( str[i] == '>' || str[i] == '<' ) {
            emitSyntax = true;
            reBuildStr += ' ';
        }
        else if( str[i] != ' ') {
            emitSyntax = false;
        }

        if(!( str[i] == ' ' && emitSyntax == true ))
            reBuildStr += str[i];
    }

    stringstream ss(reBuildStr);

    string token;
    while( ss >> token ) {
        if( ret.name == "" )
            ret.name = token;
        else if( token[0] == '>' ) {
            if( ret.redirectStdout != "" ) {
                ret.isSyntaxError = CmdRes_DupOutRe;
                return ret;
            }
            token.erase(token.begin());
            ret.redirectStdout = token;
        }
        else if( token[0] == '<' ) {
            if( ret.redirectStdin != "" ) {
                ret.isSyntaxError = CmdRes_DupInRe;
                return ret;
            }
            token.erase(token.begin());
            ret.redirectStdin = token;
        }
        else {
            ret.args.push_back(token);
        }
    }

    return ret;
}
