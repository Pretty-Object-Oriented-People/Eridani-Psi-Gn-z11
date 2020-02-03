#pragma once
#include <stddef.h>
#include "Macros.h"

#define structAlloc(type) M_structAlloc(sizeof(type))
#define structConstruct(type, ...) ({ type* _var = structAlloc(type); *_var = (type) {__VA_ARGS__}; _var; })

#define structCopyTo(type, dst, src) M_structCopy(dst, src, sizeof(type))
#define structCopy(type, src) ({ type* _var = structAlloc(type); structCopyTo(type, _var, src); })

#define structFree(stru) M_structFree(stru)
#define structDestruct(stru) structFree(stru)

void* M_memcpy(void* dst, void* src, size_t size);

void* M_structAlloc(size_t size);
void* M_structCopy(void* dst, void* src, size_t size);
void M_structFree(void* stru);

#define arrayAlloc(type, size) M_arrayAlloc(size, sizeof(type))
#define arrayConstruct(type, ...) ({ type* _arr = arrayAlloc(type, VARARG_COUNT(__VA_ARGS__)); type _vals[VARARG_COUNT(__VA_ARGS__)] = {__VA_ARGS__}; M_memcpy(_arr, &_vals, sizeof(_vals)); _arr; })

#define arrayCopyTo(type, dst, src, size) M_arrayCopy(dst, src, size, sizeof(type))
#define arrayCopyToOffset(type, dst, di, src, si, c) arrayCopyTo(type, dst+di, src+si, c)
#define arrayCopy(type, src, size) ({ type* _dst = arrayAlloc(type, size); arrayCopyTo(type, _dst, src, size); _dst; })
#define arrayCopyOffset(type, src, si, c) ({ type* _dst = arrayAlloc(type, c); arrayCopyToOffset(type, _dst, 0, src, si, c); _dst; })

#define arrayFree(arr) M_arrayFree(arr)
#define arrayDestruct(arr) arrayFree(arr)

void* M_arrayAlloc(int num, size_t els);
void* M_arrayCopy(void* dst, void* src, int num, size_t els);
void* M_arrayCopyOffset(void* dst, int di, void* src, int si, int num, size_t els);
void M_arrayFree(void* arr);

#define arrayResize(type, arr, num) do{ arr = M_arrayRealloc(arr, num, sizeof(type)); }while(0)

void* M_arrayRealloc(void* arr, size_t num, size_t els);
