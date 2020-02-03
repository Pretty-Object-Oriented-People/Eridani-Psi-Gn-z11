#include <maeth.h>

int main(){
	if(square(1) != 1) return 1;
	if(square(2) != 4) return 1;
	if(square(0) != 0) return 1;
	if(sum0To5() != 1+2+3+4+5) return 1;
	return 0;
}