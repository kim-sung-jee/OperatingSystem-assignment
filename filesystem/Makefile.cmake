CC=gcc
TARGET=final
OBJS= disk.o fs.o testcase.o



$(TARGET): $(OBJS)
	$(CC) -no-pie  -o $@ $(OBJS) validate.o

disk.o : disk.c
	$(CC) -c -o disk.o disk.c

fs.o : fs.c
	$(CC) -c -o fs.o fs.c

testcase.o : testcase.c
	$(CC) -c -o testcase.o testcase.c

