function runTest() {
    printf $1 | ./a.exe > actual
    printf "Write word to search:\r\nNearest words are: {" > expected
    printf "$2" >> expected
    printf "}\r\n" >> expected
    cmp actual expected
    rm actual expected
}

g++ -g -std=c++11 -Wall -Wextra main.cpp VPTree.cpp Node.cpp metrics.cpp
runTest "kapa" "Kappa[1], Kippa[2], KAPOW[2], Kappu[2]"
runTest "Kppa" "Kippa[1], Kappa[1], Kappu[2], Keepo[3], KAPOW[3]"
runTest "SeemsGood" "SeemsGood[0], Keepo[6], SMOrc[6]"
runTest "SeemsGod" "SeemsGood[1], Keepo[5]"
runTest "Pogchamp" "PogChamp[0], PRChase[4], KonCha[4]"
runTest "OmegaLUL" "OneHand[5], Mau5[5], MVGame[5], LUL[5]"
runTest "KappaRos" "KappaRoss[1], Kappa[3]"
rm a.exe
