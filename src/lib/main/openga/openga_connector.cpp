#include <cstdlib>

#include "openga.hpp"
#include "openga.h"

#ifdef __cplusplus
extern "C" {
#endif

OpenGAMat OpenGAMat_New(mdim m, mdim n){
	EA::Matrix* mat = new EA::Matrix(m, n);
	return (void*)mat;
}
void OpenGAMat_Destruct(OpenGAMat omat){
	EA::Matrix* mat = (EA::Matrix*) omat;
	delete mat;
}

double OpenGAMat_Get(OpenGAMat omat, mdim m, mdim n){
	EA::Matrix* mat = (EA::Matrix*) omat;
	return (*mat)(m, n);
}

#ifdef __cplusplus
}
#endif