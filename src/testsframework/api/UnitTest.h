#pragma once

#include <stdbool.h>
#include <string.h>
#include <util/Macros.h>

//Logging

extern String currentlyRunningTest;
extern int TestS_Depth;

#define LOGGER_FORMAT(info) "[Test: %s][" LOGGER_TIMEFMT "] %.*s" info
#define LOGGER_FORMAT_VA(ttm, ...) currentlyRunningTest, LOGGER_TIMEFMTARGS(_lt), TestS_Depth, "																																", ##__VA_ARGS__

#include <util/Logger.h>

//Assert

#define assertTrue(condition, str, ...) if(!(condition)){ logError("Assertion failed [%s:%d]: " #str, __FILE__, __LINE__, ##__VA_ARGS__); return false;}
#define assertFalse(condition, str, ...) if(condition){ logError("Assertion failed [%s:%d]: " #str, __FILE__, __LINE__, ##__VA_ARGS__); return false;}


#if __GENERIC_CONCAT_WORKS

#define assertNumEqual(expected, actual) { __auto_type e = (expected); __auto_type a = (actual); assertTrue(e == a, "Expected "printf_fmt(e)", Got "printf_fmt(a), e, a); }
#define assertNumIEqual(expected, actual) assertNumEqual(expected, actual)
#define assertNumUIEqual(expected, actual) assertNumEqual(expected, actual)
#define assertNumLEqual(expected, actual) assertNumEqual(expected, actual)
#define assertNumULEqual(expected, actual) assertNumEqual(expected, actual)
#define assertNumLLEqual(expected, actual) assertNumEqual(expected, actual)
#define assertNumULLEqual(expected, actual) assertNumEqual(expected, actual)
#define assertNumFEqual(expected, actual) assertNumEqual(expected, actual)
#define assertNumDEqual(expected, actual) assertNumEqual(expected, actual)
#define assertStrEqual(expected, actual) { char* e = expected; char* a = actual; assertTrue(strcmp(e, a) == 0, "Expected %s, Got %s", e, a); }

#define assertNumDWEqual(expected, actual, delta) { __auto_type e = (expected); __auto_type a = (actual); assertTrue(absm(e-a) <= absm(delta*e), "Expected %f [+-%f], Got %f", e, delta, a); }
#define assertDoubleEqual(expected, actual) assertNumDWEqual(expected, actual, 1E-5)

#define assertNumNotEqual(expected, actual) { __auto_type e = (expected); __auto_type a = (actual); assertTrue(e != a, "Expected not "printf_fmt(e)", Got "printf_fmt(e), e, a); }
#define assertNumINotEqual(expected, actual) assertNumNotEqual(expected, actual)
#define assertNumUINotEqual(expected, actual) assertNumNotEqual(expected, actual)
#define assertNumLNotEqual(expected, actual) assertNumNotEqual(expected, actual)
#define assertNumULNotEqual(expected, actual) assertNumNotEqual(expected, actual)
#define assertNumLLNotEqual(expected, actual) assertNumNotEqual(expected, actual)
#define assertNumULLNotEqual(expected, actual) assertNumNotEqual(expected, actual)
#define assertNumFNotEqual(expected, actual) assertNumNotEqual(expected, actual)
#define assertNumDNotEqual(expected, actual) assertNumNotEqual(expected, actual)
#define assertStrNotEqual(expected, actual) { char* e = expected; char* a = actual; assertTrue(strcmp(e, a) != 0, "Expected not %s, Got %s", e, a); }

#else

#define assertNumEqual(expected, actual, type, fmt) { type e = expected; type a = actual; assertTrue(e == a, "Expected "#fmt", Got "#fmt, e, a); }
#define assertNumIEqual(expected, actual) assertNumEqual(expected, actual, int, "%i")
#define assertNumUIEqual(expected, actual) assertNumEqual(expected, actual, unsigned int, "%ui")
#define assertNumLEqual(expected, actual) assertNumEqual(expected, actual, long int, "%li")
#define assertNumULEqual(expected, actual) assertNumEqual(expected, actual, unsigned long int, "%uli")
#define assertNumLLEqual(expected, actual) assertNumEqual(expected, actual, long long int, "%lli")
#define assertNumULLEqual(expected, actual) assertNumEqual(expected, actual, unsigned long long int, "%ulli")
#define assertNumFEqual(expected, actual) assertNumEqual(expected, actual, float, "%f")
#define assertNumDEqual(expected, actual) assertNumEqual(expected, actual, double, "%f")
#define assertStrEqual(expected, actual) { char* e = expected; char* a = actual; assertTrue(strcmp(e, a) == 0, "Expected %s, Got %s", e, a); }

#define assertNumDWEqual(expected, actual, delta, type) { type e = expected; type a = actual; assertTrue(dapproxeqd(e, a, delta), "Expected %f [+-%f], Got %f", e, delta, a); }
#define assertDoubleEqual(expected, actual) assertNumDWEqual(expected, actual, 1E-5, double)

#define assertNumNotEqual(expected, actual, type, fmt) { type e = expected; type a = actual; assertTrue(e != a, "Expected not "#fmt", Got "#fmt, e, a); }
#define assertNumINotEqual(expected, actual) assertNumNotEqual(expected, actual, int, "%i")
#define assertNumUINotEqual(expected, actual) assertNumNotEqual(expected, actual, unsigned int, "%ui")
#define assertNumLNotEqual(expected, actual) assertNumNotEqual(expected, actual, long int, "%li")
#define assertNumULNotEqual(expected, actual) assertNumNotEqual(expected, actual, unsigned long int, "%uli")
#define assertNumLLNotEqual(expected, actual) assertNumNotEqual(expected, actual, long long int, "%lli")
#define assertNumULLNotEqual(expected, actual) assertNumNotEqual(expected, actual, unsigned long long int, "%ulli")
#define assertNumFNotEqual(expected, actual) assertNumNotEqual(expected, actual, float, "%f")
#define assertNumDNotEqual(expected, actual) assertNumNotEqual(expected, actual, double, "%f")
#define assertStrNotEqual(expected, actual) { char* e = expected; char* a = actual; assertTrue(strcmp(e, a) != 0, "Expected not %s, Got %s", e, a); }

#endif

//Test

#define testDeclare(fname) \
bool fname();

#define testBegin(fname) \
bool fname(){

#define testEnd() \
	return true; \
}
