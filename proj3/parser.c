/////////////////////////////////////////////////////////////////////////////////
// Main:        main.c 
// File:        parser.c //
// Names:       Garret Huibregtse Tyler Patton
// Emails:      ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This file will read through the Make file and split the line
//              in an array of strings. It will check if the line begins with
//              a tab or regular character and filter out blank lines.
//
//              
///////////////////////////////////////////////////////////////////////////////

#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const int LINE_BUFFER = 1024;  // max size of line
const int ARRAY_BUFFER = 1024;  // max number of args/dependencies on a line
const int STRING_BUFFER = 1024;  // buffer for each depencecy or cmd arg

// Flushes out the rest of the line if it is too long
int FlushBuffer(FILE *fp, int error, int lineNumber, int i) {
    char ch;
    while ((ch = fgetc(fp)) != EOF && ch != '\n') {
        if (i == LINE_BUFFER) {  // checks if comment line is too long 
            fprintf(stderr, "%i: Invalid line: too long.\n", lineNumber);
            exit(1);
        }
        if (error)  // if flushing an invalid line, print out each char
            fprintf(stderr, "%c", ch);
        i++;
    }
    if (ch == EOF)
        return 1;
    return 0;
}

// flushes out invalid line to stderr and exits
void PrintError(int lineNumber, char **invalidLine, int size, FILE *fp) {
    fprintf(stderr, "%i: Invalid line: ", lineNumber);
    if (invalidLine != NULL) { 
        for (int i = 0; i < size; i++) {
            if (invalidLine[i] != NULL)
                fprintf(stderr, "%s", invalidLine[i]);
            if (i + 1 < size)
                fprintf(stderr, " ");
        }
    }
    FlushBuffer(fp, 1, lineNumber, 0); 
    fprintf(stderr, "\n");
    exit(1);
}

Node * Parse(FILE *fp) {
    Node *graph = GraphCreate();
    Node *node;
    char **arr = NULL;
    char ch = 0;
    int endOfFile = 0;  // 1 if end EOF
    int target = 0;  // 1 if line is a target
    int command = 0;  // 1 if the line is a command
    int hasColon = 0;  // 1 if target is followed by colon
    int i = 0;  // tracks line length
    int j = 0;  // tracks array indexing
    int k = 0;  // tracks string indexing
    int lineNumber = 1;  // tracks the line number
    int firstTarget = 0;  // asserted when first target has been parsed
    
    while (!endOfFile) {
        target = command = j = k = hasColon = 0;  // reset flags
        arr = (char**) calloc(ARRAY_BUFFER, sizeof(char*));
        for (i = 0; i < LINE_BUFFER; i++) {
            ch = fgetc(fp);
            if (i == 0 && ch != ' ') {  // check whether line is target/cmd
                if (ch != '\t' && ch != '#') {
                    target = 1;
                } else if (!firstTarget && ch != '#') {  // target before cmd
                    PrintError(lineNumber, arr, j + 1, fp);
                } else if (ch == '\t') {
                    command = 1; 
                }
            }
            if (ch == ':') {  // check if target is specfied
                if (k == 0) {
                    fprintf(stderr, "No target specified.\n");
                    PrintError(lineNumber, arr, j + 1, fp);
                }
                if (hasColon) {
                    fprintf(stderr, "Multiple colons detected.\n");
                    PrintError(lineNumber, arr, j + 1, fp);
                }
                hasColon = firstTarget = 1;
                k = 0;
                j++;
            } else if (ch == EOF || ch == '#' || ch == '\n') {  // comment line
                // if end of file, or if when we flush buffer due to comment,
                // set endOfFile
                if (ch == EOF)
                    endOfFile = 1;
                if (ch == '#')
                    endOfFile = FlushBuffer(fp, 0, lineNumber, i);
                if (j != 0 || k != 0) {
                    if (k == 0)  // add null terminator if not empty
                        j--;
                    if (target) {  // add node to graph if target
                         node = GraphAddNode(graph, arr, j + 1, lineNumber);
                    } else {  // set commands if command
                         if (j + 2 <= ARRAY_BUFFER || !command) {
                            GraphSetCommand(node, arr, j + 2);
                         } else {
                            PrintError(lineNumber, arr, j + 1, fp);
                         }
                    }
                    break;
                } else if (ch == EOF) {  // empty line at end of file
                    free(arr[j]);
                    free(arr);
                    break;
                } else {  // skip blank line
                    i = -1;
                    hasColon = target = 0;
                }
                lineNumber++;
            } else if (ch == ' ' || ch == '\t') {
                if (i == 0 && ch == ' ') {  // must begin with \t or char
                    fprintf(stderr, "Cannot begin line with a space.\n");
                    PrintError(lineNumber, arr, j + 1, fp);
                }   
                if (target && ch == '\t') {  // target lines cant have tab
                    PrintError(lineNumber, arr, j + 1, fp);
                }  // only increment to next index if the first string is done
                if (k > 0 && (!target || hasColon)) {
                    k = 0;
                    j++;
                }
            } else {  // add ch to the end of string at index j
                if (j == ARRAY_BUFFER) {
                    fprintf(stderr, "Too many targets or arguments\n");
                    PrintError(lineNumber, arr, j + 1, fp);
                } else if (k == STRING_BUFFER - 1) {
                    fprintf(stderr, "Target name or argument too long.\n");
                    PrintError(lineNumber, arr, j + 1, fp);
                } else if (k == 0) {
                    arr[j] = (char*) calloc(STRING_BUFFER, sizeof(char));
                    if (arr[j] == NULL) {
                        fprintf(stderr, "Cannot allocate enought memory.\n");
                        exit(1);
                    }
                }
                arr[j][k++] = ch;
            }
        }
        if (i == LINE_BUFFER) {
            PrintError(lineNumber, arr, j + 1, fp);
        }
        lineNumber++;
    }
    if (fclose(fp) == EOF) {
        fprintf(stderr, "Error closing file.\n");
        exit(1);
    }
    return graph;
}
