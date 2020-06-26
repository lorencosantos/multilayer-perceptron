cd ../src
gcc -c *.c -I"../include"
gcc -o main.exe  *.o
del /f *o
move main.exe ../compile/
pause