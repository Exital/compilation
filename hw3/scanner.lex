%{
#include "hw3_output.hpp"
#define YYSTYPE Node*
#include "tokenTypes.hpp"
#include "parser.tab.hpp"
%}

%option yylineno
%option noyywrap
%option nounput

whitespace	[ \t\n\r]

%%
{whitespace}							          ;
void 								                return VOID;
int 								                return INT;
byte                                return BYTE;
b                                   return B;
bool                                return BOOL;
const                               return CONST;
and                                 return AND;
or                                  return OR;
not                                 return NOT;
true                                return TRUE;
false                               return FALSE;
return                              return RETURN;
if                                  return IF;
else                                return ELSE;
while                               return WHILE;
break							                  return BREAK;
continue                            return CONTINUE;
;                                   return SC;
,                                   return COMMA;
\(                                  return LPAREN;
\)                                  return RPAREN;
"{"                                 return LBRACE;
"}"                                 return RBRACE;
=                                   return ASSIGN;
(>|<|<=|>=)                         return RELOP;
(==|!=)								              return EQUALITY;
(\+|\-)				                      return ADDSUB;
(\*|\/)								              return MULDIV;
[a-zA-Z][a-zA-Z0-9]*      	        {yylval = new Value(yytext); return ID;}
[0]|[1-9][0-9]*                     {yylval = new Value(yytext); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\"       return STRING;
\/\/[^\r\n]*(\r|\n|\r\n)?           ;
.									                  {output::errorLex(yylineno); exit(0);};


%%
