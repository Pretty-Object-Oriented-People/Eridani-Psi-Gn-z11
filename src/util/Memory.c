#include "Memory.h"
#include <stdlib.h>
#include <string.h>

void* M_memcpy(void* dst, void* src, size_t size){
	return memcpy(dst, src, size);
}

void* M_structAlloc(size_t size){
	return calloc(1, size);
}
void* M_structCopy(void* dst, void* src, size_t size){
	return M_memcpy(dst, src, size);
}
void M_structFree(void* stru){
	free(stru);
}

void* M_arrayAlloc(int num, size_t els){
	return calloc(num, els);
}
void* M_arrayCopy(void* dst, void* src, int num, size_t els){
	return M_memcpy(dst, src, num*els);
}
void* M_arrayCopyOffset(void* dst, int di, void* src, int si, int num, size_t els){
	M_arrayCopy(dst+di, src+si, num, els);
	return dst;
}
void M_arrayFree(void* arr){
	return free(arr);
}

void* M_arrayRealloc(void* arr, size_t num, size_t els){
	return realloc(arr, num*els);
}