%{
#include "parser.tab.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "myCompiler.h"
using namespace std;

void printStr(char *);


%}


delim		[ \t]
ws			{delim}+
nextline	[\n\r]
digit		[0-9]
letter		[a-zA-Z]

id			{letter}({letter}|{digit})*

exp			{[Ee][]-+?[0-9]+}

interger	{digit}+
float		({digit}*\.{digit}*+|{digit}+\.){exp}?
char		"'"."'"
number     {digit}+(\.{digit}+)?

comment_interline	(\/\*([^*]|\*+[^/*])*\*+\/)
comment_inline		"//".*\n
%%

{ws}			{}

{comment_interline}			{}
{comment_inline}			{}



"int"		{return (INT);}
"char"		{return (CHAR);}
"float"		{return (FLOAT);}

{char}		{yylval.singleChar=yytext[0];return CHARACTER;}
{number}	{yylval.doubleVal= atof(yytext);return NUMBER;}
{id}		{strcpy(yylval.strVal,yytext);return (ID);}

"+"		{return ADD;}
"-"		{return	(SUB);}
"*"		{return (MUL);}
"/"		{return	(DIV);}
"="		{return	(ASSIGN);}
"||"	{return	(OR);}
"&&"	{return	(AND);}
"!"		{return	(NOT);}
"("		{return	('(');}
")"		{return	(')');}
"{"		{return	('{');}
"}"		{return	('}');}
"\n"     {return NEWLINE;}

"<"		{return	(LESS);}
"<="	{return	(LE);}
">"		{return	(GREAT);}
">="	{return	(GE);}
"=="	{return	(EQ);}
"!="	{return	(NQ);}

"if"		{return (IF);}
"else"		{return (ELSE);}
"while"		{return (WHILE);}




%%

int yywrap()
{
 return 1;
}

int yyerror(char* msg)
{
 cout<<"yyerror : "<<msg<<endl;
 return 0;
}

void printStr(char *a){
cout<<a<<endl;
}