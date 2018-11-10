CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537ps

all: 537ps.o procList.o optProc.o parser.o
	$(CC) -o $(EXE) -g 537ps.o procList.o optProc.o parser.o

537ps.o: 537ps.c procList.h optProc.h parser.h
	$(CC) $(WARNING_FLAGS) -c 537ps.c

procList.o: procList.c procList.h
	$(CC) $(WARNING_FLAGS) -c procList.c

optProc.o: optProc.c optProc.h
	$(CC) $(WARNING_FLAGS) -c optProc.c

parser.o: parser.c parser.h
	$(CC) $(WARNING_FLAGS) -c parser.c

clean:
	rm $(EXE) *.o
