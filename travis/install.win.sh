#!/bin/bash
set -ev
choco install make cmake

#Copy DLLs to build
powershell copy "lib/Windows/*.dll" "build/"
powershell copy "lib/Windows/*.dll.a" "build/"