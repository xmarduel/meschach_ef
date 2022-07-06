gcc -I /Users/xavier/DEVELOPMENT/MESCHACH_WORK -c test_meschach.c
gcc -L /Users/xavier/x_local/lib  -o test_meschach test_meschach.o -lmeschach_threaded -lmeschach
./test_meschach
