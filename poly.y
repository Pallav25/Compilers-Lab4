%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *format, ...);
%}

%code requires { 
    #include "polyutils.h"
    extern Node *root;
    extern int errorsnumber;
}   

%union {
    char value;
    Node *nodeptr;
}

%token PLUS MINUS VARIABLE ONE ZERO EXPONENT INVALID
%token<value> DIGIT

%type<nodeptr> S P T X N M NS
%start NS
%%

NS: S { root = $1; }    
 ;

S: P {$$ = createNode('S', $1, NULL, NULL);}
 | PLUS P {$$ = createNode('S', createNode('+', NULL, NULL, NULL), $2, NULL);}
 | MINUS P { $$ = createNode('S', createNode('-', NULL, NULL, NULL), $2, NULL);}
 ;

P: T { 
        $$ = createNode('P', $1, NULL, NULL);
        
    }
 | T PLUS P { 
        $$ = createNode('P', $1, createNode('+', NULL, NULL, NULL), $3);
        
    }
 | T MINUS P { 
        $$ = createNode('P', $1, createNode('-', NULL, NULL, NULL), $3);
    }
 | error PLUS P { yyerrok; $$ = $3; }
 | error MINUS P { yyerrok; $$ = $3; }
 ;

T: ONE { 
        Node* ptr = createNode('T', createNode('1', NULL, NULL, NULL), NULL, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | N { 
        Node* ptr = createNode('T', $1, NULL, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | X { 
        Node* ptr = createNode('T', $1, NULL, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | N X { 
        Node* ptr = createNode('T', $1, $2, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 ;

X: VARIABLE { 
        Node* ptr = createNode('X', createNode('x', NULL, NULL, NULL), NULL, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | VARIABLE EXPONENT N { 
        Node* ptr = createNode('X', createNode('x', NULL, NULL, NULL), createNode('^', NULL, NULL, NULL), $3);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 ;

N: DIGIT { 
        Node* ptr = createNode('N', createNode($1, NULL, NULL, NULL), NULL, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | DIGIT M { 
        Node* ptr = createNode('N', createNode($1, NULL, NULL, NULL), $2, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | ONE M { 
        Node* ptr = createNode('N', createNode('1', NULL, NULL, NULL), $2, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 ;

M: ZERO { 
        Node* ptr = createNode('M', createNode('0', NULL, NULL, NULL), NULL, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | ONE { 
        Node* ptr = createNode('M', createNode('1', NULL, NULL, NULL), NULL, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | DIGIT { 
        Node* ptr = createNode('M', createNode($1, NULL, NULL, NULL), NULL, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | ZERO M { 
        Node* ptr = createNode('M', createNode('0', NULL, NULL, NULL), $2, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | ONE M { 
        Node* ptr = createNode('M', createNode('1', NULL, NULL, NULL), $2, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 | DIGIT M { 
        Node* ptr = createNode('M', createNode($1, NULL, NULL, NULL), $2, NULL);
        if(ptr == NULL) {
            YYERROR;
        } else {
            $$ = ptr;
        }
    }
 ;

%%

int yywrap(void) {
    if(yynerrs!=0){
        printf("\nTotal invalid characters found= %d\n", errorsnumber);
        printf("\nTotal syntax errors found= %d\n\n", yynerrs);
        printf("NOTE :- Printing the parse tree and doing other operations while ignoring the errors(output can be wrong)\n\n");
    }

    return 1;
}