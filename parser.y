%{
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "myCompiler.h"
#include "Chart.h"


Content * tmpContent=new Content();

extern void InserContent(Content * content);
void SetValue(double ,char );
void SetValueAfterDeclare(char* ,double ,char);

extern int yyerror(char *);
extern int yylex();

using namespace std;
%}

%union
{
	int intVal;
	double doubleVal;
	char strVal[20];
	char charVal;
}

%token <strVal>ID 
%token <doubleVal>NUMBER 
%token <doubleVal>FLOATNUMBER
%token <charVal>CHARACTER
%token INT FLOAT CHAR
%token ADD SUB MUL DIV ASSIGN
%token OR AND NOT LESS LE GREAT GE EQ NQ 
%token IF 
%token ELSE
%token WHILE
%token '(' ')'
%token NEWLINE

%left ADD SUB
%left MUL DIV


%type <doubleVal>expression

%%


declare	:  declare type	ID 	NEWLINE						{tmpContent->name=$3;SetValue(0,' ');InserContent(tmpContent);tmpContent=new Content();}
		|  declare ID ASSIGN expression	NEWLINE			{SetValueAfterDeclare($2,$4,' ');tmpContent=new Content();}
		|  declare type ID ASSIGN value NEWLINE         { tmpContent->name=$3;
														  SetValue(yylval.doubleVal,yylval.charVal);
														  InserContent(tmpContent);
														  tmpContent=new Content();}
		|
		;



expression  :expression ADD expression	{$$=$1+$3;cout<<"result is : "<<$$<<endl;}
			|expression SUB expression	{$$=$1-$3;cout<<"result is : "<<$$<<endl;}
			|expression MUL expression	{$$=$1*$3;cout<<"result is : "<<$$<<endl;}
			|expression DIV expression	{$$=$1/$3;cout<<"result is : "<<$$<<endl;}
			|NUMBER
			;


type	:	 CHAR    {tmpContent->type=C;}
			|INT    {tmpContent->type=Z;}
			|FLOAT  {tmpContent->type=F;}  
			;

value		:   CHARACTER
		    |	NUMBER
		    ;
%%

void SetValue(double dVar=0,char cVar=' ')
{

    float* pTmpValueF;
	int* pTmpValueI;
	char *pTmpChar;

	switch (tmpContent->type)
	{
	case Z:
		tmpContent->pValue = malloc(sizeof(int));
		pTmpValueI = (int*)tmpContent->pValue;
		*pTmpValueI = dVar;
		break;
	case F:
		tmpContent->pValue = malloc(sizeof(float));
		pTmpValueF = (float*)tmpContent->pValue;
		*pTmpValueF = dVar;
		break;
	case C:
		tmpContent->pValue = malloc(sizeof(char));
		pTmpChar = (char*)tmpContent->pValue;
		*pTmpChar = cVar;
		break;
	default:
		break;
	}
}

void SetValueAfterDeclare(char* name,double dVar=0,char cVar=' '){
	tmpContent=FindContent(name);
	SetValue(dVar,cVar);
}