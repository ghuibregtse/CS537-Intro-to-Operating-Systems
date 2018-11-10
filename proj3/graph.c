///////////////////////////////////////////////////////////////////////////////
// Main:        main.c
// File:        graph.c
//
// Names:       Garret Huibregtse Tyler Patton
// Email:       ghuibregtse@wisc.edu tpatton4@wisc.edu
//
// Description: This is the source file for the graph module that contains
//              functions to create a graph object, add targets to it,
//              add edges between the targets, and traverse through the graph.
//
////////////////////////////////////////////////////////////////////////////////

#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include <sys/types.h>
#include <unistd.h>
#include "execute.h"


/*  @brief  This function is a constructor for the graph structure.
 *  @return a reference to the graph created.
 */
Node * GraphCreate() {
    Node *graph = (Node *) calloc(1,sizeof(Node)); 
    if (graph == NULL) {
        fprintf(stderr, "Not enough memory for graph.\n");
        exit(1);
    }
    return graph;
}


/*  @brief  This function adds a target to the graph and sets its data members.
 *          such as its target name, and commands.
 *  @param  graph is a reference to the graph structure.
 *  @param  target is the name of the node being added.
 *  @param  command is the command that is executed by the target.
 *  @return reference to the created node, NULL if already added.
 */
Node * GraphAddNode(Node *graph, char **targetList, 
                    int numTargets, int lineNumber) {
    Node *node = (Node *) calloc(1, sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Not enough memory for node.\n");
        exit(1);
    }
    node->numTargets = numTargets;
    node->lineNumber = lineNumber;
    node->targetList = targetList;
    if (graph->next == NULL) {
        graph->next = node;
    } else {
        Node *tmp = graph->next;
        while (tmp->next != NULL) {
            if (strcmp(tmp->targetList[0], targetList[0]) == 0) {
                fprintf(stderr, "Target: %s ", targetList[0]);
                fprintf(stderr, "has already been created.\n");
                exit(1);
            }
            tmp = tmp->next;
        }
        tmp->next = node;
    }
    return node;
}
/*  @brief This function sets the command of a node.
 *  @param graph is a reference to the graph structure.
 *  @param command is the command being set.
 *  @return 0 if successful, -1 if unsuccessful
 */
void GraphSetCommand(Node *node, char **commandList, int numCmds) {
    if (node == NULL) {
        fprintf(stderr, "Unable to set the commands of the target.\n");
        exit(1);
    }
    CommandList *newCommand = (CommandList *) calloc(1, sizeof(CommandList));
    newCommand->command = commandList;
    newCommand->numCmds = numCmds;
    if (node->CommandList == NULL) {
        node->CommandList = newCommand;
    } else {
        CommandList *list = node->CommandList;
        while (list->next != NULL) {
            list = list->next;
        }
        list->next = newCommand;
    }
}

/*  @brief  This function finds a node in the graph. 
 *  @param  graph is a reference to the graph structure.
 *  @param  target is the name of the node being found.
 *  @return The node if it exists, NULL if it does not.
 */
Node * GraphFindNode(Node *graph, char *target) {
    if (graph == NULL || target == NULL)
        return NULL;
    Node *tmp = graph->next;
    while (tmp != NULL) {
        if (strcmp(tmp->targetList[0], target) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

/*  @brief  This function adds all the edges originated at a target node.
 *  @param  graph is a reference to the graph object.
 *  @param  start is a reference to the target node.
 *  @param  edgeList is a reference to the list of dependent nodes.
 *  @param  edges is the number of edges being added to the node.
 *  @return 0 if successful, -1 if start node doesnt exist
 */
int GraphAddEdge(Node *graph, Node *start) {
    struct stat stTarget, stDependency;
    if (graph == NULL || start == NULL) {
        return -1;
    } else if (start->numTargets > 1) {
        start->edges = (Node **) calloc(start->numTargets -1, sizeof(Node*));
        Node *tmp = NULL;
        for (int i = 1; i < start->numTargets; i++) {  // add edges if it exists
            if ((tmp = GraphFindNode(graph, start->targetList[i])) != NULL) {
                start->edges[i - 1] = tmp;
            }  // compare modification times between target and depencencies
            if (stat(start->targetList[i], &stDependency) != -1) {
                time_t depTime = stDependency.st_mtime;
                if (stat(start->targetList[0], &stTarget) != -1) {
                    time_t targetTime = stTarget.st_mtime;
                    if (depTime > targetTime)  // run if depencency is newer
                        start->shouldExecute = 1;
                } else {  // run if depencency does not exist
                    start->shouldExecute = 1;
                }
            } else {  // run if the target does not exist
                start->shouldExecute = 1;
            }
        }
    } else {  // targets with no dependencies always run
        start->shouldExecute = 1;
    }
    return 0;
}

void FreeCommands(CommandList *cmd) {
    if (cmd == NULL)
        return;
    if (cmd->next != NULL)
        FreeCommands(cmd->next);
    for (int i = 0; i < cmd->numCmds; i++)
        free(cmd->command[i]);
    free(cmd->command);
    free(cmd);
}
/*  @brief  This function is a helper function to recursively destroy
 *          the graph object.
 *  @param  node is the node being freed.
 */
void NodeDestroy(Node *node) {
    if (node == NULL)
        return;
    Node *tmp = node->next;
    if (tmp != NULL) {
        NodeDestroy(tmp);
    }
    for (int i = 0; i < node->numTargets; i++)
        free(node->targetList[i]);
    free(node->targetList);
    FreeCommands(node->CommandList);
    free(node->edges);
    free(node);
}

void GraphDestroy(Node *graph) {
    if (graph != NULL) {
        if (graph->next != NULL)
            NodeDestroy(graph->next);
        free(graph);
    }
}

/*  @brief Helper function to recursively do a bottom up traversal of the graph.
 *
 */
int GraphTraverseHelper(Node *node) {
    if (NULL == node)
        return 0;
    
    if (node->checked == 1) {
        return -1;
    }
    node->checked = 1;
    if (NULL != node->edges) {
        int error;
        for (int i = 0; i < node->numTargets - 1; i++) {
            if ((error = GraphTraverseHelper(node->edges[i])) != 0) {
                fprintf(stderr, "537Make: Circular %s <- ", 
                        node->targetList[0]);
                fprintf(stderr, "%s. Stop.\n", node->edges[i]->targetList[0]);
                exit(1);
            }
        }
    }
    node->checked = 0;
    return 0;
}

/*  @brief Helper function to recursively do a bottom up traversal of
 *  the graph, but will be used for executing.
 */
void GraphTraverseHelperExecute(Node *node, char *fileName, int *executed) {
    if (NULL == node)
        return;
    if (node->shouldExecute == 1) {
        if (NULL != node->edges) {
            for (int i = 0; i < node->numTargets - 1; i++)
                GraphTraverseHelperExecute(node->edges[i], fileName, executed);
        }
        if (NULL != node->CommandList) {
            CommandList *temp = node->CommandList;
            int error;
            while (temp != NULL) {
                if ((error = Execute(temp)) != 0) {
                    fprintf(stderr, "%s:%i: ", fileName, node->lineNumber);
                    fprintf(stderr, "recipe for target '");
                    fprintf(stderr, "%s' failed\n", node->targetList[0]);
                    fprintf(stderr, "537Make: *** [%s] ", node->targetList[0]);
                    fprintf(stderr, "Error\n");
                    exit(1);
                }
                temp = temp->next;
                *executed = *executed + 1;
            }
        }
    }
}


/*  @brief This function will check for a cycle within the graph
 *
 *  @param graph is a reference to the graph structure
 */
void CheckCycles(Node *graph) {
    Node *tmp;
    if (graph->next == NULL) {
        fprintf(stderr, "No targets defined.\n");
        exit(1);
    }
    tmp = graph->next;
    GraphTraverseHelper(tmp);
}

/*  @brief  This function implements a post order traversal of the graph
 *          in order to access each dependency of a target.
 *  @param  graph is a reference to the graph structure.
 *  @param  target is the name of the target we are going to build.
 */
void GraphTraverseExecute(Node *graph, char *target, char *fileName) {
    Node *tmp;
    if (graph->next == NULL) {
        fprintf(stderr, "No targets defined.\n");
        exit(1);
    }
    if ((tmp = GraphFindNode(graph, target)) == NULL) {
        tmp = graph->next;
        tmp->shouldExecute = 1;
    }
    if (tmp == NULL) {
        fprintf(stderr, "This target does not exist.\n");
        exit(1);
    }
    int executed = 0;
    GraphTraverseHelperExecute(tmp, fileName, &executed);
    if (executed == 0) {
        fprintf(stderr, "537Make: Nothing to be done for '");
        fprintf(stderr, "%s'.\n", tmp->targetList[0]);
    }
}
