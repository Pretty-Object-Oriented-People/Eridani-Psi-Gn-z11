#include <maeth.h>

#include <json-c/json.h>
#include <string.h>

#include "Tests.h"
#include <TestSuiteMain.h>

testBegin(testSquare)
	assertNumDEqual(1, square(1));
	assertNumDEqual(4, square(2));
	assertNumDEqual(0, square(0));
testEnd()

testBegin(testSum0To5)
	assertNumDEqual(1+2+3+4+5, sum0To5());
testEnd()

testBegin(testJsonC)
	let i = 55;
	let d = 57.4; 
	let jsO = json_object_new_object();
	json_object_object_add(jsO, "i", json_object_new_int(i));
	json_object_object_add(jsO, "d", json_object_new_double(d));
	let jstr = json_object_to_json_string(jsO);
	
	let jsD = json_tokener_parse(jstr);
	assertNumIEqual(i, json_object_get_int(json_object_object_get(jsD, "i")));
	assertNumDEqual(d, json_object_get_double(json_object_object_get(jsD, "d")));
	
	json_object_put(jsD);
	json_object_put(jsO);
testEnd()

testsMainAll((testMacros, "Test Macros"), (testOpenGA, "Test Open GA"), (testSquare, "Test Square"), (testSum0To5, "Test Sum 0 to 5"))
