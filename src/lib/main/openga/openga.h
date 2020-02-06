#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void* OpenGAMat;
typedef unsigned int mdim;

OpenGAMat OpenGAMat_New(mdim m, mdim n);
void OpenGAMat_Destruct(OpenGAMat mat);

double OpenGAMat_Get(OpenGAMat mat, mdim m, mdim n);

#ifdef __cplusplus
}
#endif