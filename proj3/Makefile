#
#    scan-build will "make" your program and run CSA at the same time.
#
#    scan-view will do a scan-build (if necessary) and then open a web browser
#      window to review the results.
#
CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537Make
SCAN_BUILD_DIR = scan-build-out

all:  main.o graph.o parser.o execute.o 
	$(CC) -O0 -o  $(EXE) -g main.o graph.o parser.o execute.o
main.o: main.c
	$(CC) $(WARNING_FLAGS) -c main.c -g -O0

graph.o: graph.c graph.h
	$(CC) $(WARNING_FLAGS) -c graph.c -g -O0

parser.o: parser.c parser.h
	$(CC) $(WARNING_FLAGS) -c parser.c -g -O0

execute.o: execute.c execute.h
	$(CC) $(WARNING_FLAGS) -c execute.c -g -O0

clean:
	rm $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
