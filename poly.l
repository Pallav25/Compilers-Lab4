%{
#include <string.h>
#include "poly.tab.h"
#define YY_NO_INPUT
#define YY_NO_UNPUT
int yyerror(const char *format, ...);
int errorsnumber = 0;
%}

%%
"+"     { return PLUS; }
"-"     { return MINUS; }
"x"     { return VARIABLE; }
"^"    { return EXPONENT; }
[0]     {return ZERO; }
[1]     {return ONE; }
[2-9]  {yylval.value = yytext[0]; return DIGIT; }
[ \t\n]   { /* Ignore whitespace */ }
.       {errorsnumber++; }
%%

