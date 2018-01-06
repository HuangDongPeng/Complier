%{
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "myCompiler.h"
#include "Chart.h"


Content * tmpContent=new Content();

extern void InserContent(Content * content);
float GetValue(Content *content);
char GetValueChar(Content *content);


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

sentence:sentence expression NEWLINE
		|sentence declare NEWLINE
		|
		;

declare: dataType ID					{tmpContent->name=$2;SetValue(0,' ');InserContent(tmpContent);tmpContent=new Content();}
		|dataType ID ASSIGN expression  {tmpContent->name=$2;
										 if(tmpContent->type==C){
										 char value=GetValueChar($4.content);
										 SetValue(0,value);
										 }
										 else{
										 float value=GetValue($4.content);
										 SetValue(value,' ');
										 }
										 InserContent(tmpContent);tmpContent=new Content();}
		;

dataType:INT     {tmpContent->type=Z;}
		|CHAR    {tmpContent->type=C;}
		|FLOAT	 {tmpContent->type=F;}
		;

expression: expression ADD expression	{
										 float tmpValue1=GetValue($1.content);
										 float tmpValue2=GetValue($3.content);
										 float result=tmpValue1+tmpValue2;
										 Property type=Z;
										 if($1.content->type==F||$3.content->type==F){
											type=F; 
										 }
										 $$.content=SetValueWithReturn(result,' ',type);
										 cout<<"result is : "<<GetValue($$.content)<<endl;
										}
			| expression SUB expression	{
										 float tmpValue1=GetValue($1.content);
										 float tmpValue2=GetValue($3.content);
										 float result=tmpValue1-tmpValue2;
										 Property type=Z;
										 if($1.content->type==F||$3.content->type==F){
											type=F; 
										 }
										 $$.content=SetValueWithReturn(result,' ',type);
										 cout<<"result is : "<<GetValue($$.content)<<endl;
										}
			|expression MUL expression	{
										 float tmpValue1=GetValue($1.content);
										 float tmpValue2=GetValue($3.content);
										 float result=tmpValue1*tmpValue2;
										 Property type=Z;
										 if($1.content->type==F||$3.content->type==F){
											type=F; 
										 }
										 $$.content=SetValueWithReturn(result,' ',type);
										 cout<<"result is : "<<GetValue($$.content)<<endl;
										}
			|expression DIV expression	{
										 float tmpValue1=GetValue($1.content);
										 float tmpValue2=GetValue($3.content);
										 float result=tmpValue1/tmpValue2;
										 Property type=Z;
										 if($1.content->type==F||$3.content->type==F){
											type=F; 
										 }
										 $$.content=SetValueWithReturn(result,' ',type);
										 cout<<"result is : "<<GetValue($$.content)<<endl;
										}
						
			| INTNUMBER					{$$.content=SetValueWithReturn($1,' ',Z);}
			| FLOATNUMBER				{$$.content=SetValueWithReturn($1,' ',F);}
			| CHARACTER					{$$.content=SetValueWithReturn(0,$1,C);}
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

float GetValue(Content *content)
{
	switch (content->type)
	{
	case Z:
		return *(int*)content->pValue;
		break;
	case F:
		return *(float*)content->pValue;
	default:
		break;
	}
	return 0;
}

char GetValueChar(Content *content) {
	return *(char*)content->pValue;
}
