#include "maeth.h"
#include <util/Macros.h>

double square(double d){
	return d*d;
}

#define addToVarD(n) do {d += n;} while(0);

double sum0To5(){
	double d = 0;
	FOREACH_ARG(addToVarD, 0, 1, 2, 3, 4, 5);
	return d;
}