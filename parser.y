%{
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include "myCompiler.h"
#include "Chart.h"

extern struct QuadNum* quadNum;
extern struct QuadNum* baseQuad;


extern Content * tmpContent;

extern void InserContent(Content * content);
extern void InsertNewQuad(int op, int arg1, int arg2,int result);
extern void InsertConstNum(Content *content);
extern float GetValue(Content *content);
extern char GetValueChar(Content *content);
extern void SetValueAfterDeclare(char* name, double dVar = 0, char cVar = ' ');
extern Content* SetValueWithReturn(double , char , Property );
extern void SetValue(double , char );
extern bool CompareData(Content* a,Content* b);

float GetValue(Content *content);
char GetValueChar(Content *content);


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
	bool booler;

	union{
	class Content* content;
	double dVal;
	int iVal;
	char cVal;
	bool booller;
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
%type <data>boolexpr

%%

sentence:sentence expression NEWLINE
		|sentence declare NEWLINE
		|sentence ID ASSIGN expression NEWLINE
										{
										if($4.content->type==C){
										char value=GetValueChar($4.content);
										SetValueAfterDeclare($2,0,value);
										}
										else{
										float value=GetValue($4.content);
										SetValueAfterDeclare($2,value,' ');
										}
										 tmpContent=FindContent($2);
										 InsertNewQuad(OP_ASSIGN,$4.content->addr,USELESS_ARG,tmpContent->addr);
										 tmpContent=new Content();
										}
		|sentence IF '(' boolexpr ')' NEWLINE	{cout<<$4.booller<<endl;}
		|
		;				

boolexpr: expression EQ expression		{if(CompareData($1.content,$3.content))$$.booller=true;else{$$.booller=false;}}
		;

declare: dataType ID					{tmpContent->name=$2;
										 SetValue(0,' ');
										 InserContent(tmpContent);
										 InsertNewQuad(OP_DECLARE,-1,-1,tmpContent->addr);
										 tmpContent=new Content();
										 }
		|dataType ID ASSIGN expression  {tmpContent->name=$2;
										 if(tmpContent->type==C){
										 char value=GetValueChar($4.content);
										 SetValue(0,value);
										 }
										 else{
										 float value=GetValue($4.content);
										 SetValue(value,' ');
										 }
										 InserContent(tmpContent);
										 tmpContent=FindContent($2);
										 InsertNewQuad( OP_ASSIGN,$4.content->addr ,USELESS_ARG ,tmpContent->addr);
										 tmpContent=new Content();
										 }
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
										 $$.content=SetValueWithReturn(result,' ',CONST);
										 InsertNewQuad(OP_ADD, $1.content->addr,$3.content->addr,$$.content->addr);
										}
			| expression SUB expression	{
										 float tmpValue1=GetValue($1.content);
										 float tmpValue2=GetValue($3.content);
										 float result=tmpValue1-tmpValue2;
										 Property type=Z;
										 if($1.content->type==F||$3.content->type==F){
											type=F; 
										 }
										 $$.content=SetValueWithReturn(result,' ',CONST);
										 InsertNewQuad(OP_SUB, $1.content->addr,$3.content->addr,$$.content->addr);

										}
			|expression MUL expression	{
										 float tmpValue1=GetValue($1.content);
										 float tmpValue2=GetValue($3.content);
										 float result=tmpValue1*tmpValue2;
										 Property type=Z;
										 if($1.content->type==F||$3.content->type==F){
											type=F; 
										 }
										 $$.content=SetValueWithReturn(result,' ',CONST);
										 InsertNewQuad(OP_MUL, $1.content->addr,$3.content->addr,$$.content->addr);

										}
			|expression DIV expression	{
										 float tmpValue1=GetValue($1.content);
										 float tmpValue2=GetValue($3.content);
										 float result=tmpValue1/tmpValue2;
										 Property type=Z;
										 if($1.content->type==F||$3.content->type==F){
											type=F; 
										 }
										 $$.content=SetValueWithReturn(result,' ',CONST);
										 InsertNewQuad(OP_DIV, $1.content->addr,$3.content->addr,$$.content->addr);
										}
						
			| INTNUMBER					{$$.content=SetValueWithReturn($1,' ',Z);InsertConstNum($$.content);}
			| FLOATNUMBER				{$$.content=SetValueWithReturn($1,' ',F);InsertConstNum($$.content);}
			| CHARACTER					{$$.content=SetValueWithReturn(0,$1,C);InsertConstNum($$.content);}
			| ID						{$$.content=FindContent($1);if($$.content==nullptr)yyerror("undeclare symbol");}
			;


%%

