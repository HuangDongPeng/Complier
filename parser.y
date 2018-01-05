%{
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "myCompiler.h"
#include "Chart.h"


Content * tmpContent=new Content();

extern void InserContent(Content * content);
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


declare	:  declare type	ID 	NEWLINE			    {tmpContent->name=$3;InserContent(tmpContent);tmpContent=new Content();}
		|  declare ID ASSIGN expression			{cout<<"赋值语句"<<endl;}
		|  declare type ID ASSIGN var	        {cout<<"赋值初始化"<<endl;}
		|
		;



expression  :expression ADD expression	{$$=$1+$3;cout<<"result is : "<<$$<<endl;}
			|expression SUB expression	{$$=$1-$3;cout<<"result is : "<<$$<<endl;}
			|expression MUL expression	{$$=$1*$3;cout<<"result is : "<<$$<<endl;}
			|expression DIV expression	{$$=$1/$3;cout<<"result is : "<<$$<<endl;}
			|NUMBER
			;


type	:	CHAR    {tmpContent->type=C;}
			|INT    {tmpContent->type=Z;}
			|FLOAT  {tmpContent->type=F;}  
			;

var		:   CHARACTER
		|	NUMBER
		;
%%

void SetValue(double dVar=0,char cVar='')
{

    float* pTmpValueF;
	int* pTmpValueI;
	char *pTmpChar;

	switch (tmpContent->type)
	{
	case Z:
		tmpContent->pValue = malloc(sizeof(int));
		pTmpValueI = (int*)pValue;
		*pTmpValueI = _value;
		break;
	case F:
		tmpContent->pValue = malloc(sizeof(float));
		pTmpValueF = (float*)pValue;
		*pTmpValueF = _value;
		break;
	case C:
		tmpContent->pValue = malloc(sizeof(char));
		pTmpChar = (char*)pValue;
		*pTmpChar = _value;
		break;
	default:
		break;
	}
}