#pragma once

#include "Macros.h"
#include <stdio.h>

#define logInfoInline(info, ...) fprintf(stdout, info, ##__VA_ARGS__)
#define logErrorInline(info, ...) fprintf(stderr, info, ##__VA_ARGS__)
#ifdef __DEBUG_MODE
#define logDebugInline(info, ...) fprintf(stdout, info, ##__VA_ARGS__)
#else 
#define logDebugInline(info, ...)
#endif
#define logInfo(info, ...) logInfoInline(info"\n", ##__VA_ARGS__)
#define logError(info, ...) logErrorInline(info"\n", ##__VA_ARGS__)
#define logDebug(info, ...) logDebugInline(info"\n", ##__VA_ARGS__)
