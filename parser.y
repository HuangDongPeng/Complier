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
Content* SetValueWithReturn(double,char ,Property );


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

	union{
	class Content* content;
	double dVal;
	int iVal;
	char cVal;
	}data;
}

%token <strVal>ID 
%token <intVal>INTNUMBER 
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


%left IF
%left ADD SUB
%left MUL DIV


%type <data>expression


%%

sentence: sentence expression NEWLINE
		|
		;

expression: expression ADD expression	{$$.content=SetValueWithReturn(0,' ',Z);}
			| INTNUMBER					{$$.content=SetValueWithReturn($1,' ',Z);cout<<"int NUBER VALUE: "<<*(int *)$$.content->pValue<<endl;}
			| FLOATNUMBER				{$$.content=SetValueWithReturn($1,' ',F);cout<<" float NUBER VALUE: "<<*(float *)$$.content->pValue<<endl;}
			| CHARACTER					{$$.content=SetValueWithReturn(0,$1,C);cout<<"CHAR VALUE: "<<*(char *)$$.content->pValue<<endl;}



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

Content* SetValueWithReturn(double dVar=0,char cVar=' ',Property _type=Z)
{
	Content* tmp = new Content();
	tmp->type=_type;

    float* pTmpValueF;
	int* pTmpValueI;
	char *pTmpChar;

	switch (tmp->type)
	{
	case Z:
		tmp->pValue = malloc(sizeof(int));
		pTmpValueI = (int*)tmp->pValue;
		*pTmpValueI = dVar;
		break;
	case F:
		tmp->pValue = malloc(sizeof(float));
		pTmpValueF = (float*)tmp->pValue;
		*pTmpValueF = dVar;
		break;
	case C:
		tmp->pValue = malloc(sizeof(char));
		pTmpChar = (char*)tmp->pValue;
		*pTmpChar = cVar;
		break;
	default:
		break;
	}
	return tmp;
}


void SetValueAfterDeclare(char* name,double dVar=0,char cVar=' '){
	tmpContent=FindContent(name);
	SetValue(dVar,cVar);
}