#pragma once
#pragma once
#ifndef CHART_H
#define CHART_H
#include <stack>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#define HASHLENGTH 13
#define SYMTABLE_BEGIN_ADRESS 2000
#define CONST_BEGIN_ADDRESS 5000
#define USELESS_ARG -1

#include <hash_map>
#include <unordered_map>

using namespace std;

enum Property
{
	Z,
	F,
	C,
	CONST
};

union DataType
{
	int intVal;
	char charVal;
	float floatVal;
};
/////////////////////////////ËÄÔªÊý////////////

static int symTableAddress= SYMTABLE_BEGIN_ADRESS;
static int constTableAdress = CONST_BEGIN_ADDRESS;


void ShowQuadTable();

class Content
{
public:
	string name;
	Property type = Z;
	Content* link = nullptr;

	int addr;
	int width=4;
	DataType data;
	void * pValue = nullptr;
	float value=0;

	Content();
	~Content();
private:

};


void InsertConstNum(Content *content);

void InserContent(Content * content);

void DeleteContent();

void InitMap();

void InsertContent_HashMap(Content *content);

void DeleteContent_HashMap(Content *target);

void ShowDetail(Content * content);


void ShowChart();

void ShowHash();

void chartSet();

void ChartInsert(char * name, int type);


Content * FindContent(string name);


#endif // !CHAR_H
