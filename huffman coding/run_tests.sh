function runTest() {
    echo $1 > file.txt
    ./a.exe
    cmp decoded.txt file.txt
    #rm decoded.txt
    #rm encoded.txt
    #rm file.txt
}

make
#runTest "" #not working
#runTest "a"
#runTest "aaa"
#runTest "aab"
runTest "%^&" #not working
rm a.exe
