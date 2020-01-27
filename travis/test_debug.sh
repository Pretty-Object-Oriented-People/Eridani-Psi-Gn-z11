#!/bin/bash
gdb ../build/test -q -x ../travis/test_debug.gdbinit
if grep -q 'SIGSEGV' test_debug.gdb.log; then
	echo "Sefault detected. Log:"
	cat test_debug.gdb.log
	exit 1
fi
echo "No Segfaults detected"
exit 0
