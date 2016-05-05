#pragma once
#include "ProcessController.h"

extern ProcessController procCtrl;

enum BHResult {
	CONTINUE = 1 << 2,
	RUN_FAIL = 1 << 3,
	RUN_OK = 1 << 4
};

#define BH_IF_IS(rc,param) ((rc&param))

namespace xnsh {
	class BuiltinHelper;
};

using xnsh::BuiltinHelper;

class BuiltinHelper {
public:
	static bool isSupportCmd(string);
	static int RunBuiltinCmd(string);
private:
	static bool isStartWith(const string& str, const string& pat);

	static void GoExit();
	static void EnvHelper(const string&);
	static int BringToFront(const string&);
};


