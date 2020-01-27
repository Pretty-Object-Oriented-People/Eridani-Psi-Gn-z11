#!/bin/bash
set -ev
choco install make gnuwin32-coreutils.portable sed

cd lib
#JSON-C
mv "Windows/json-c/libjson-c.dll.a" "libjson-c.dll.a"
powershell copy "Windows/json-c/*.dll" "../run/"
