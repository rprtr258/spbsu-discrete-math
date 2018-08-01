g++ -O3 main.cpp
time  ./a.exe -in i -out actual -f
cat actual
cmp actual o
rm actual
rm a.exe
