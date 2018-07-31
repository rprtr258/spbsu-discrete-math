g++ main.cpp
time cat i | ./a.exe > actual
cmp actual o
rm actual
rm a.exe
