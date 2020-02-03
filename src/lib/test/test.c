#include <maeth.h>

#define LOGGER_NAME "Not-so-dumb-anymore Test"
#include <util/Logger.h>

int main(){
	logInfo("Test started");
	if(square(1) != 1) return 1;
	if(square(2) != 4) return 1;
	if(square(0) != 0) return 1;
	logInfo("Square OK!");
	if(sum0To5() != 1+2+3+4+5) return 1;
	logInfo("sum0To5 OK!");
	return 0;
}