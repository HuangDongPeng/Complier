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

	union{
	double dVal;
	int iVal;
	char cVal;
	}data;
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


%left IF
%left ADD SUB
%left MUL DIV


%type <data>expression


%%

sentence: declare
		|IF '(' expression ')'
		;

declare	:  declare type	ID 	NEWLINE							 {tmpContent->name=$3;SetValue(0,' ');InserContent(tmpContent);tmpContent=new Content();}
		|  declare ID ASSIGN expression	NEWLINE				 {SetValueAfterDeclare($2,$4.dVal,$4.cVal);tmpContent=new Content();}
		|  declare type ID ASSIGN expression NEWLINE         { tmpContent->name=$3;
															  SetValue($5.dVal,$5.cVal);
															  InserContent(tmpContent);
															  tmpContent=new Content();}
		| declare expression NEWLINE						
		|
		;



expression  :expression ADD expression	{$$.dVal=$1.dVal+$3.dVal;}
			|expression SUB expression	{$$.dVal=$1.dVal-$3.dVal;}
			|expression MUL expression	{$$.dVal=$1.dVal*$3.dVal;}
			|expression DIV expression	{$$.dVal=$1.dVal/$3.dVal;}
			|NUMBER						{$$.dVal=$1;}
			|CHARACTER					{$$.cVal=$1;}
			|ID							{
											Content* newContent=FindContent($1);
											if(newContent==nullptr){cout<<"unDeclare"<<endl;}
											else
											{
												if(newContent->type==C)
												    {
													$$.cVal=*(char*)newContent->pValue;
													cout<<"ID value: "<<$$.cVal;
													}
												else if(newContent->type==F){
													$$.dVal=*(float*)newContent->pValue;
													cout<<"ID value: "<<$$.dVal;
													}
												else if(newContent->type==Z){
													$$.dVal=*(int*)newContent->pValue;
													cout<<"ID value: "<<$$.dVal;

													}
											}
										}
			;


type	:	 CHAR    {tmpContent->type=C;}
			|INT    {tmpContent->type=Z;}
			|FLOAT  {tmpContent->type=F;}  
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