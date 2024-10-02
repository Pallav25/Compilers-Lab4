#ifndef POLYUTILS_H
#define POLYUTILS_H

typedef struct Node {
    char type;
    int val;
    int inh;
    struct Node *left;
    struct Node *middle;
    struct Node *right;
} Node;

// Function to create a new node
Node* createNode(char type, Node *left, Node *right, Node *middle);

// Function to set attributes for the parse tree nodes in inherited and synthesized manner
void setAttr(Node *node);

// Function to print the annotated parse tree
void printTree(Node *node, int level);

// Function to evaluate the polynomial
int evalpoly(Node *node, int x);

// Function to free the memory allocated for the parse tree
void freeTree(Node *node);

#endif // POLYUTILS_H