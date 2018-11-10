///////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        main.c
//
// Names:       Garret Huibregtse Tyler Patton
// Emails       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This file will start the 537make program. It will parse 
//              a Makefile and run the correct commands or detect a problem
//              and throw an error.
//              
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "graph.h"
#include "parser.h"
#include "execute.h"

int main(int argc, char *argv[]) {
    char opt;  // for processing arguments
    Node *graph = NULL;  // data structure of makefile targets
    char *fileName = NULL;  // name of makefile being used
    char *target = NULL;  // target being made in makefile
    
    if (argc > 4) {
        fprintf(stderr, "Usage: 537Make [target] [-f <makefile>]\n");
        exit(1);
    }
    // Gets the optional target to be made, if it doesn't exist then just run
    // the first target.
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            i++;
        } else {
            target = argv[i];
            break;
        }
    }
    // Gets the makefile file name if it is specified
    while ((opt = getopt(argc, argv, ":f:")) != -1) {
        switch (opt) {  // determine which flag is present and sets status T/F
            case 'f' :
                if (optarg == NULL) {
                    fprintf(stderr, "Null argument passed.\n");
                    fprintf(stderr, "Usage: 537Make [target] ");
                    fprintf(stderr, "[-f <makefile>]\n");
                    exit(1);
                }
                fileName = optarg;
                break;
            case ':' :
                fprintf(stderr, "Option -%c is missing ", optopt);
                fprintf(stderr, "makefile name.\n");
                fprintf(stderr, "Usage: 537Make [target] [-f <makefile>]\n");
                exit(1);
                break;
            default  :
                break;
        }
    }
    
    FILE *fp;
    Node *tmp;
    if (fileName != NULL) {
        if ((fp = fopen(fileName, "r")) != NULL) {
        graph = Parse(fp);
    } else {
        fprintf(stderr, "Usage: 537Make [target] ");
        fprintf(stderr, "[-f <makefile>]\n");
        }   
    } else if ((fp = fopen("makefile", "r")) != NULL) {
        fileName = "makefile";
        graph = Parse(fp);
    } else if ((fp = fopen("Makefile", "r")) != NULL) {
        fileName = "Makefile";
        graph = Parse(fp);
    } else {
        fprintf(stderr, "Could not open makefile\n");
        exit(1);
    }
    if (graph == NULL || graph->next == NULL) {
        fprintf(stderr, "Failed to create graph of targets.\n");
        exit(1);
    }
    if (target != NULL) {
        int targetExists = 0;
        tmp = graph->next;
        while (tmp != NULL) {
            if (strcmp(tmp->targetList[0], target) == 0)
                targetExists = 1;
            tmp = tmp->next;
        }
        if (!targetExists) {
            fprintf(stderr, "537Make: *** No rule to make '%s'", target);
            fprintf(stderr, ". Stop.\n"); 
            exit(1);
        }
    }
    // ADD EDGES
    tmp = graph->next;
    while (tmp != NULL) {
       GraphAddEdge(graph, tmp); 
       tmp = tmp->next;
    }
    
    // Check for cycles
    CheckCycles(graph);

    // Traverse graph in post order matter and build the make target
    GraphTraverseExecute(graph, target, fileName);
    GraphDestroy(graph);
}
