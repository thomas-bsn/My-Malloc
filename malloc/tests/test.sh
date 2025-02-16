#!/bin/bash

function run_command {
LD_PRELOAD=./libmalloc.so $1 > /dev/null
if [ $? -eq 0 ]; then
echo "OK"
else
echo "FAIL"
fi
}

run_command "factor 20 30 40 50 60 70 80 90"
run_command "ls"
run_command "ls -la"
run_command "od libmalloc.so"
run_command "tar -cf malloc.tar libmalloc.so"
run_command "git status"
run_command "less Makefile"
run_command "ip a"
run_command "cat Makefile"