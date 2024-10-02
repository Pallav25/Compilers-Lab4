#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "poly.tab.h"
#include "polyutils.h"

#define POSITIVE 1
#define NEGATIVE -1

Node *root = NULL;

// Function to create a new node
Node* createNode(char type, Node *left, Node *middle,Node *right) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->type = type;
    node->left = left;
    node->middle = middle;
    node->right = right;
    node->inh=0;
    node->val=0;
    return node;
}



void printTree(Node *node, int level) {
    if (node == NULL) return;
    if(level==0) printf("+++ The annotated parse tree is +++\n");

    for (int i = 0; i < level; i++) printf("   ");

    if(node->type!='S') printf("==> %c ", node->type);
    else printf("   S ");

    if (node->type == 'S' || node->type == '+'|| node->type == '-'|| node->type == '^'|| node->type == 'X'|| node->type == 'x') {
        printf("[]\n");
    }
    else if (node->type == 'N') {
        printf("[val = %d]\n", node->val);
    }
    else if (node->type == 'M') {
        printf("[inh = %d , val = %d]\n",node->inh, node->val);
    }
    else if (node->type == 'P') {
        if(node->inh==POSITIVE)printf("[inh = +]\n");
        else printf("[inh = -]\n");
    }
    else if(node->type == 'T') {
        if(node->inh==POSITIVE)printf("[inh = +]\n");
        else printf("[inh = -]\n");
    }
    else {
        printf("[val = %d]\n", node->val);
    }
    printTree(node->left, level + 1);
    printTree(node->middle, level + 1);
    printTree(node->right, level + 1);
}

// function to set attributes to polynomial
void setatt(Node *node, int inh) {
    if (node == NULL) return;

    switch(node->type) {
        case 'P':
            node->inh = inh;
            setatt(node->left, inh);
            if(node->middle != NULL) {
                if(node->middle->type=='+')  setatt(node->right, POSITIVE);
                else setatt(node->right, NEGATIVE);
            }
            break;
        case 'T':
            node->inh=inh;
            if(node->left->type!='1') setatt(node->left,0);
            else node->left->val=1;
            setatt(node->middle,0);
            break;
        case 'S':
            if(node->left->type == 'P') {
                setatt(node->left, POSITIVE);
            } else {
                if(node->left->type == '+') {
                    setatt(node->middle, POSITIVE);
                } else {
                    setatt(node->middle, NEGATIVE);
                }
            }
            break;
        case 'N':
            if(node->middle!=NULL) {
                if(node->left->type == '1') {
                    setatt(node->middle, 1);
                } else {
                    setatt(node->middle, node->left->type - '0');
                }
                node->val = node->middle->val;
                node->left->val = node->left->type - '0';
            } else {
                node->val = node->left->type - '0';
                node->left->val = node->val;
            }
            break;
        case 'M':
            node->inh=inh;
            if(node->middle!=NULL) {
                if(node->left->type == '1') {
                    setatt(node->middle, 10*inh+1);
                } else if(node->left->type == '0') {
                    setatt(node->middle, 10*inh);
                } else {
                    setatt(node->middle, 10*inh+(node->left->type - '0'));
                }
                node->val = node->middle->val;
                node->left->val = node->left->type - '0';
            } else {
                node->val = inh*10+(node->left->type - '0');
                node->left->val = node->left->type - '0';
            }
            break;

        case 'X':
            if(node->right!=NULL){
                setatt(node->right, 0);
            }
            break;
        case 'x':
        case '+':
        case '-':
        case '^':
            break;
        default:
            if (node->type >= '0' && node->type <= '9') {
                node->val = node->type - '0';
            } else {
                node->val = 0; 
            }
            setatt(node->left, inh);
            setatt(node->middle, inh);
            setatt(node->right, inh);
            break;
    }
}


int evalpoly(Node *node,int valx) {
    if (node == NULL) return 0;

    switch(node->type) {
        case 'S':
            if(node->middle) return evalpoly(node->middle,valx);
            else return evalpoly(node->left,valx);
            break;
        case 'P':
            if(node->right) return evalpoly(node->left,valx)+evalpoly(node->right,valx);
            else return evalpoly(node->left,valx);
            break;
        case 'T':
            if(node->middle) {
                if(node->inh==POSITIVE) return evalpoly(node->left,valx)*evalpoly(node->middle,valx);
                else return evalpoly(node->left,valx)*evalpoly(node->middle,valx)*(-1);
            }
            else {
                if(node->inh==POSITIVE) return evalpoly(node->left,valx);
                else return (-1)*evalpoly(node->left,valx);
            }
            break;
        case 'X':
            if(node->right) //return valx^n
                return pow(valx,evalpoly(node->right,valx));
            else return valx; 
            break;
        case 'N':
            return node->val;
            break;
        default:
            if(node->type<='9'&&node->type>='0') return node->type-'0';
            return 0;
            break;
    }
    return 0;
}

int isconst(Node *node){
    if(node==NULL) return 1;
    if(node->type=='X'||node->type=='x') return 0;
    if(node->type=='+'||node->type=='-') return 1;
    if(node->type=='^') return 1;
    if(node->type=='N') return 1;
    if(node->type<='9'&&node->type>='0') return 1;
    return isconst(node->left)&&isconst(node->middle)&&isconst(node->right);
}

int powerofX(Node *node){
    if(node->type=='X'){
        if(node->right){
            return (node->right->val);
        }
        else return 1;
    }
    else {
        printf("Error: Not a polynomial\n");
        return -1;
    }
}

void printderivative(Node *node){
    if(node==NULL) return;
    switch(node->type){
        case 'S':
            if(node->middle) {
                printderivative(node->middle);
            }
            else printderivative(node->left);
            break;
        case 'P':
            printderivative(node->left);

            if(node->right) printderivative(node->right);
            break;
        case 'T':
            if(isconst(node)) return;
            if(node->inh==POSITIVE){
                printf("+");
            }
            else{
                printf("-");
            }
            if(node->middle){
                printf("%d",(node->left->val)*powerofX(node->middle));
                printderivative(node->middle);
            }
            else{
                printf("%d",powerofX(node->left));
                printderivative(node->left);
            }
            break;
        case 'X':
            if(node->right){
                if(node->right->val==2) printf("x"); 
                else printf("x^%d",node->right->val-1);
            }
            break;
        default:
            printf("Error: something wrong with expression\n");
            break;
            
    }
}

// Function to free the memory allocated for the parse tree
void freeTree(Node *node) {
    if (node == NULL) return;

    freeTree(node->left);
    freeTree(node->right);
    free(node);
}
void yyerror(const char *s) {
    
}
int main(){
    yyparse();
    setatt(root,0);
    printf("Subsequent actions are performed on the f(x) given in the input file sample.txt\n\n");
    printTree(root,0);
    int ans;
    printf("\nPolynomial value f(x) for x = -5 to 5 is :-\n\n");
    for (int i = -5; i < 6; i++)
    {
        ans=evalpoly(root,i);
        printf("f(x) for x = %d is : %d\n",i,ans);
    }
    printf("\nDerivative of the polynomial is :-\n\n");
    printderivative(root);
    printf("\n\n");

}