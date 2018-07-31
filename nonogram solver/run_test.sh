g++ -O3 main.cpp
time cat i | ./a.exe | tee actual
cmp actual o
rm actual
rm a.exe
