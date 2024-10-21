%{

#include<stdio.h>

#include "y.tab.h"

extern int yylval;

%}



%%

[0-9]+ {

          yylval=atoi(yytext);

          return NUMBER;

       }

[\t] ;

[\n] return 0;

. return yytext[0];

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
