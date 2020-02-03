CC ?= gcc
CSTD ?= gnu99
CMAIN ?= build/main
CFLAGS += -Wreturn-type -Wcast-qual -Warray-bounds -Waddress -Wchar-subscripts
LIBS=m json-c
LIBSp=

LIBNAME=Eridani
