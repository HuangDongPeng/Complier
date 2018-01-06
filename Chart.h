#pragma once
#pragma once
#ifndef CHART_H
#define CHART_H
#include <stack>
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#define HASHLENGTH 13
#include <hash_map>
#include <unordered_map>

using namespace std;

enum Property
{
	Z,
	F,
	C
};

union DataType
{
	int intVal;
	char charVal;
	float floatVal;
};
/////////////////////////////ËÄÔªÊý////////////



void ShowQuadTable();

class Content
{
public:
	string name;
	Property type = Z;
	Content* link = nullptr;

	void * address = (void*)this;
	void * pValue = nullptr;
	float value=0;

	Content();
	Content(char* _name, int _type, Content* _link , float _value);
	~Content();


private:

};

inline Content::Content(char * _name, int _type, Content * _link, float _value)
{
	float* pTmpValueF;
	int* pTmpValueI;
	char *pTmpChar;

	name = _name;
	switch (_type)
	{
	case 1:
		type = Z;
		pValue = malloc(sizeof(int));
		pTmpValueI = (int*)pValue;
		*pTmpValueI = _value;
		break;
	case 2:
		type = F;
		pValue = malloc(sizeof(float));
		pTmpValueF = (float*)pValue;
		*pTmpValueF = _value;
		break;
	case 3:
		type = C;
		pValue = malloc(sizeof(char));
		pTmpChar = (char*)pValue;
		*pTmpChar = _value;
		break;
	default:
		break;
	}
	link = _link;
}


void InserContent(Content * content);

void DeleteContent();

void InitMap();

void InsertContent_HashMap(Content *content);

void DeleteContent_HashMap(Content *target);

void ShowDetail(Content * content);

void BuildNewContent(char *name, int type, double value);

void ShowChart();

void ShowHash();

void chartSet();

void ChartInsert(char * name, int type);


Content * FindContent(string name);




#endif // !CHAR_H
