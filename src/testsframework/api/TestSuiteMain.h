#pragma once

#include "ComposedUnitTest.h"

extern String currentlyRunningTest;

#define testsMainBegin() \
int main(){ \
	bool _b = true; \
	currentlyRunningTest = "Root"; \

// Use composed test methods, they are valid here

#define testsMainEnd() \
	if(_b){ logInfo("All tests successfull!"); return 0; } \
	else { logInfo("Tests failed."); return 1; } \
}

#define testsMainAll(...) \
testsMainBegin()\
compoTestSubTestMany(__VA_ARGS__)\
testsMainEnd()
