#!/bin/bash

function check() {
    cmp decoded.txt file.txt
    rm file.txt
    rm encoded.txt
    rm decoded.txt
}


function runTest() {
    printf "%s" $1 > file.txt
    ./a.exe
    check
}

make
#runTest "" #not working
#runTest "a" #not working
#runTest "aaa" #not working
runTest "aab"
runTest "%^&"
rm a.exe
