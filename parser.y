%{
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "myCompiler.h"

extern int yyerror(char *);
extern int yylex();
using namespace std;
%}

%token ID 
%token INT FLOAT CHAR
%token ADD SUB MUL DIV ASSIGN
%token OR AND NOT LESS LE GREAT GE EQ NQ 
%token IF ELSE WHILE 
%token NEWLINE

%%
declare	: declare type	ID NEWLINE		{cout<<"this is a declare"<<endl;}
		|						{cout<<"empty"<<endl;}
		;

type	:	CHAR
			|INT
			|FLOAT
			;

%%
