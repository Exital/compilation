%{
#include "tokens.hpp"
%}

%option yylineno
%option noyywrap

digit		[0-9]
letter		[a-zA-Z]
whitespace	[ \t\n\r]
ascii [^\\\"\n\r]
escape \\[\\nrt0\"]
hex_ascii \\x[0-7][0-9a-fA-F]
bad_escape \\[^\\nrt0\"]
bad_ascii \\x([^0-7]|[0-7][^0-9a-fA-F])

string ({ascii}|{escape}|{hex_ascii})

%%
{whitespace}	            ;
void                        return VOID;
int                         return INT;
byte                        return BYTE;
b                           return B;
bool                        return BOOL;
and                         return AND;
or                          return OR;
not                         return NOT;
true                        return TRUE;
false                       return FALSE;
return                      return RETURN;
if                          return IF;
else                        return ELSE;
while                       return WHILE;
break                       return BREAK;
continue                    return CONTINUE;
;                           return SC;
,                           return COMMA;
\(                          return LPAREN;
\)                          return RPAREN;
"{"                         return LBRACE;
"}"                         return RBRACE;
=                           return ASSIGN;
(==|!=|<|>|<=|>=)           return RELOP;
(\+|-|\*|\/)                return BINOP;
\/\/(.)*                    return COMMENT;
{letter}+({letter}|{digit})*		return ID;
(0|[1-9]{digit}*)		    return NUM;
\"{string}*\"         return STRING;
\"({string}|{bad_escape}|{bad_ascii})*\"         return UNDEFINED_ESCAPE;
\"[^\"]*                        return UNCLOSED_STRING;
.		                    return UNDEFINED_CHAR;
%%
