alpha [A-Za-z]
digit [0-9]

%%

[\t \n]
for             return FOR;
{digit}+    return NUM;
{alpha}({alpha}|{digit})* return ID;
"<="         return LE;
">="         return GE;
"=="         return EQ;
"!="          return NE;
"||"            return OR;
"&&"         return AND;
.                return yytext[0];

%%
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
