#include <maeth.h>

#include <TestSuiteMain.h>

testBegin(testSquare)
	assertNumDEqual(1, square(1));
	assertNumDEqual(4, square(2));
	assertNumDEqual(0, square(0));
testEnd()

testBegin(testSum0To5)
	assertNumDEqual(1+2+3+4+5, sum0To5());
testEnd()

testsMainAll((testSquare, "Test Square"), (testSum0To5, "Test Sum 0 to 5"))
