%{
#include "y.tab.h"
%}

%%
for					return FOR;
[\(\)]					return PARANTHESIS;
[a-zA-Z0-9]*				return OPERAND;
"="|"<"|">"|">="|"<="|"=="|"++"|"--"	return OPERATOR;
\;					return SEMICOLON;
,					return COMMA;
\n					return NEWLINE;
.					;
%%


int yywrap()
{
	return 1;
}
/*
   lex for.l
yacc for.y
gcc y.tab.c -ll -ly -w
./a.out
for(i=0;i<n;i++)
i=i+1;
accepted
for(i=0;i<n;i++)
i=i+1
syntax error 
*/
