#include "BuiltinHelper.h"


bool BuiltinHelper::IsSupportCmd(string line)
{
	const string cmd[] = {
		"quit",
		"exit",
		"lsjob",
		"xenv",
		"fg",
        "bg"
	};

	for( const string& s : cmd ) {
		if( isStartWith(line, s) )
			return true;
	}

	return false;
}

int BuiltinHelper::RunBuiltinCmd(string line)
{
	if( isStartWith(line, "quit") || isStartWith(line, "exit") )
		GoExit();

	if( isStartWith(line, "xenv") ) {
		EnvHelper(line);
		return Success;
	}

	if( isStartWith(line, "lsjob") ) {
		procCtrl.printJobs();
		return Success;
	}

	if( isStartWith(line, "fg") ) {
		if( Failure == BringToFront(line) )
			return Failure;
		return Wait;
	}

	if( isStartWith(line, "bg") ) {
		if( Failure == BringToBack(line) )
			return Failure;
		return Success;
	}

	printf("no matching builtin command");
	exit(3);
}

bool BuiltinHelper::isStartWith(const string& str, const string& pat)
{
	if( pat.size() > str.size() )
		return false;
	for( size_t i = 0 ; i < pat.size() ; ++i ) {
		if( pat[i] != str[i] )
			return false;
	}
	return true;
}

void BuiltinHelper::GoExit()
{
	exit(0);
}

void BuiltinHelper::EnvHelper(const string& line)
{
	int fg;
	auto cmds = Parser::Parse(line,fg);
	const Command& cmd = cmds[0];

	if( cmd.name != "xenv" )
		goto GUIDE;
	if( cmd.args[0] == "add" && cmd.args.size() == 3 ) {
		if( -1 == setenv(cmd.args[1].c_str(), cmd.args[2].c_str(), 1) ) {
			printf("setenv error: %s\n",strerror(errno));
			return;
		}
	}
	else if( cmd.args[0] == "rm" && cmd.args.size() == 2 ) {
		if( -1 == unsetenv(cmd.args[1].c_str()) ) {
			printf("unsetenv error: %s\n",strerror(errno));
			return;
		}
	}
	else
		goto GUIDE;

		return;
GUIDE:
	puts("Command Example: ");
	puts("$ xenv add LANG C");
	puts("$ xenv rm LANG");
}

int BuiltinHelper::BringToFront(const string& line)
{
	int fg = 0;
	auto cmds = Parser::Parse(line,fg);
	int index = -1;
	if( cmds[0].args.size() == 1 ) {
		stringstream ss(cmds[0].args[0]);
		ss >> index;
	}	
	if( Failure == procCtrl.BringToFront(index) ) {
		return Failure;
	}
	return Success;
}

int BuiltinHelper::BringToBack(const string& line)
{
	int fg = 0;
	auto cmds = Parser::Parse(line,fg);
	int index = -1;
	if( cmds[0].args.size() == 1 ) {
		stringstream ss(cmds[0].args[0]);
		ss >> index;
	}	
	if( Failure == procCtrl.BringToBack(index) ) {
		return Failure;
	}
	return Success;
}
