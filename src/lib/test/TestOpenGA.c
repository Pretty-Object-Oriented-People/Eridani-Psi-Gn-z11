#include "Tests.h"

#include <openga/openga.h>

testBegin(testOpenGA)
	let M = 3;
	let N = 5;
	let mat = OpenGAMat_New(M, N);
	for(let m = 0; m < M; m++) for(let n = 0; n < N; n++) assertNumDEqual(0.0, OpenGAMat_Get(mat, m, n));
	OpenGAMat_Destruct(mat);
testEnd()
