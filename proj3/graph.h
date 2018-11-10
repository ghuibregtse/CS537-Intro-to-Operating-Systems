////////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        graph.h
//
// Names:       Garret Huibregtse Tyler Patton
// Email:       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This is the header for the graph module.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __node_h__
#define __node_h__

// This node structure holds the target name, commands of the target,
// and edges leading from the target to targets it depends on.
typedef struct Node {
    int shouldExecute;
    int checked;
    int numTargets;
    int lineNumber;
    char **targetList;
    struct CommandList *CommandList;
    struct Node **edges;
    struct Node *next;
} Node;

typedef struct CommandList {
    char **command;
    int numCmds;
    struct CommandList *next;
} CommandList;

//  @brief  This function is a constructor for the graph data structure.
Node * GraphCreate();

//  @brief  This function finds a node in the graph.
//  @return The node if it exists, NULL if it does not.
Node * GraphFindNode(Node *graph, char *target);

//  @brief  This function adds a target to the graph.
Node * GraphAddNode(Node *graph, char **targetList, int numTargets, 
                    int lineNumber);

//  @brief This function sets the command of a node.
void GraphSetCommand(Node *node, char **commandList, int numCmds);

//  @brief  This function adds an edge between two nodes.
int GraphAddEdge(Node *graph, Node *start);

//  @brief This function frees the memory allocated by the graph.
void GraphDestroy(Node *graph);

//  @brief  This function implements a post order traversal of the graph
//          in order to access each dependency of a target.
void GraphTraverseExecute(Node *graph, char *target, char *fileName);

//  @brief This function will check if the graph has a cycle
void CheckCycles(Node *graph);

#endif  // __node_h___
