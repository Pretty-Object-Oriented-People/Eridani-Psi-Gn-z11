#pragma once

#include "UnitTest.h"

extern String currentlyRunningTest;
extern int TestS_Depth;

#define subTest(testf, testname) ({\
	let parentTest = currentlyRunningTest; \
	currentlyRunningTest = testname; \
	logInfo("Testing: %s...", testname); \
	TestS_Depth++; \
	bool t_res = testf(); \
	TestS_Depth--; \
	if(t_res) logInfo("OK."); \
	else logError("Fail."); \
	currentlyRunningTest = parentTest; \
	t_res; \
	})

#define compoTestDeclare(fname) testDeclare(fname)

#define compoTestBegin(fname) \
bool fname(){ \
	bool _b = true;

#define compoTestSubTest(testf, testname) \
	_b &= subTest(testf, testname);

#define compoTestSubTestPacked(arg) compoTestSubTest arg

#define compoTestSubTestMany(...) \
	FOREACH_ARG(compoTestSubTestPacked, __VA_ARGS__)

#define compoTestEnd() \
	return _b; \
}

#define compoTestAll(fname, ...) \
compoTestBegin(fname)\
compoTestSubTestMany(##__VA_ARGS__)\
compoTestEnd()
