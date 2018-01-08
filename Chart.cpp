#define HASHLENGTH 13
#include "Chart.h"
#include <iostream>	
using namespace std;

Content * tmpContent = new Content();

struct QuadNum
{
	int op;
	int arg1;
	int arg2;
	int result;
	QuadNum* next = nullptr;
};
QuadNum* quadNum = (QuadNum*)malloc(sizeof(QuadNum));
QuadNum* baseQuad = quadNum;


stack<Content*> s;
hash_map<int, Content*> hashMap;
hash_map<int, Content*> blockMap;
hash_map<int, Content*> levelMap;
int hashMapSize = HASHLENGTH;
static int index = 0;


//�����Ԫʽ
void InsertNewQuad(int op, int arg1, int  arg2, int result)
{
	quadNum->op = op;
	quadNum->arg1 = arg1;
	quadNum->arg2 = arg2;
	quadNum->result = result;
	quadNum->next = (QuadNum*)malloc(sizeof(QuadNum));
	quadNum = quadNum->next;
	quadNum->next = nullptr;
	ShowQuadTable();
}
//��ʾ��Ԫʽ
void ShowQuadTable()
{
	int index = 1;
	QuadNum* tmp = baseQuad;
	cout << "index	" << "op	" << "arg1	" << "arg2	" << "result		" << endl;
	while (tmp->next != nullptr)
	{
		cout << index << "	";
		cout << tmp->op << "	";
		if (tmp->arg1 == USELESS_ARG)
			cout << "	";
		else
			cout << tmp->arg1 << "	";
		if (tmp->arg2 == USELESS_ARG)
			cout << "	";
		else
			cout << tmp->arg2 << "	";
		cout << tmp->result << "	" << endl;
		tmp = tmp->next;
		index++;
	}
}
//����±�ʶ��
void InserContent(Content * content) {
	s.push(content);
	InsertContent_HashMap(content);
	switch (content->type)
	{
	case Z:
		content->addr = symTableAddress;
		symTableAddress += 4;
		break;
	case F:
		content->addr = symTableAddress;
		symTableAddress += 8;
		break;
	case C:
		content->addr = symTableAddress;
		symTableAddress += 1;
		break;
	default:
		break;
	}
	ShowChart();
}
//��ӳ���
void InsertConstNum(Content *content) {
	content->addr = constTableAdress;
	constTableAdress += 1;
}
//ɾ����ʶ��
void DeleteContent() {
	Content *target = s.top();
	s.pop();
	DeleteContent_HashMap(target);
	delete(target);
}
//��ʼ����ϣ��
void InitMap() {
	for (int i = 0; i < HASHLENGTH; i++) {
		hashMap[i] = nullptr;
	}
}
//��ʶ����ת��ΪKey
int StringToKey(string name) {
	char *nameTmp = new char;
	strcpy(nameTmp, name.data());
	int tmp = 0;

	while (*nameTmp != '\0')
	{
		tmp += *nameTmp;
		nameTmp++;
	}

	return (tmp%HASHLENGTH);
}
//�ڹ�ϣ�����key
void InsertContent_HashMap(Content *content) {
	int key = 0;
	if (content->name == "") {
		cout << "error" << endl;
		return;
	}
	else
	{
		key = StringToKey(content->name);
	}
	//�����ϣ��λ���ѱ�ռ�ã������ó�ͻ��
	if (hashMap[key] != nullptr)
	{
		content->link = hashMap[key];
	}

	hashMap[key] = content;
}
//�ӹ�ϣ��ɾ��key
void DeleteContent_HashMap(Content *target) {
	int key = -1;
	for (int i = 0; i < HASHLENGTH; i++) {
		if (hashMap[i] == target)
		{
			key = i;
			break;
		}
	}

	if (key == -1)
		return;

	hashMap[key] = nullptr;
}
//��ʾԪ������
void ShowDetail(Content * content) {
	char *name = new char;
	strcpy(name, (content->name).data());
	cout << "name: " << name << endl;
	cout << "type: " << content->type << endl;
	if (content->link == nullptr)
		cout << "null" << endl;
	else
		cout << "Link: " << content->link << endl;
	cout << "key: " << StringToKey(content->name) << endl;
}
//String ת char*
char* String2Char(string name) {
	char *result = new char;
	strcpy(result, name.data());
	return result;
}
//Ѱ�ұ�ʶ��
bool NameCompare(char *a, string name) {
	char *b = new char;
	strcpy(b, name.data());
	while (*a != '\0'&&*b != '\0')
	{
		if (*a != *b)
			return false;
		a++;
		b++;
	}
	if (*a == '\0'&&*b != '\0')
		return false;
	if (*a != '\0'&&*b == '\0')
		return false;

	return true;
}

Content * FindContent(string name) {
	return hashMap[StringToKey(name)];
}
Content * FindContent(char* name) {
	int tmp = 0;
	char* origin = name;
	while (*name != '\0')
	{
		tmp += *name;
		name++;
	}
	if (hashMap[tmp%HASHLENGTH] == nullptr)
		return nullptr;
	else
	{
		Content *tmpP = hashMap[tmp%HASHLENGTH];
		while (!NameCompare(origin, tmpP->name)) {
			tmpP = tmpP->link;
		}
		return tmpP;

	}
}
//��ӡ���ű�
void ShowChart() {
	stack<Content*> sCopy;
	stack<Content*> sOut;
	Content *tmp;
	sCopy = s;

	while (!sCopy.empty())
	{
		tmp = sCopy.top();
		sCopy.pop();
		sOut.push(tmp);
	}
	int index = 1;
	cout << "index	" << "name	" << "type	" << "value	" << "adress		" << "link	" << endl;
	while (!sOut.empty())
	{
		tmp = sOut.top();
		sOut.pop();
		cout << index << "	";
		cout << String2Char(tmp->name) << "	";

		if (tmp->type == Z) {
			cout << "Z	" << tmp->data.intVal << "	";
		}
		else if (tmp->type == F)
		{
			cout << "F	" << tmp->data.floatVal << "	";
		}
		else if (tmp->type == C)
		{
			cout << "C	" << tmp->data.charVal << "	";
		}
		cout << tmp->addr << "	";
		if (tmp->link == nullptr)
			cout << "null	" << endl;
		else
			cout << tmp->link << "	" << endl;

		index++;
	}
	cout << "��������������������������������������������������������������" << endl;
}
//��ӡ��ϣ��
void ShowHash() {
	int index = 1;
	for (int i = 0; i < HASHLENGTH; i++) {
		cout << index << "	";
		if (hashMap[i] == nullptr)
			cout << "null" << endl;
		else
			cout << hashMap[i] << endl;
		index++;
	}
	cout << "��������������������������������������������������������������" << endl;

}

///////��������
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

void SetValue(double dVar = 0, char cVar = ' ')
{

	float* pTmpValueF;
	int* pTmpValueI;
	char *pTmpChar;

	switch (tmpContent->type)
	{
	case Z:
	    tmpContent->data.intVal = dVar;
		break;
	case F:
		tmpContent->data.floatVal = dVar;
		break;
	case C:
		tmpContent->data.charVal = cVar;
		break;
	default:
		break;
	}
}

Content* SetValueWithReturn(double dVar = 0, char cVar = ' ', Property _type = Z)
{
	Content* tmp = new Content();
	tmp->type = _type;

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
	case CONST:
		tmp->value = dVar;
		InsertConstNum(tmp);
		break;
	default:
		break;
	}
	return tmp;
}

void SetValueAfterDeclare(char* name, double dVar = 0, char cVar = ' ') {
	tmpContent = FindContent(name);
	if (tmpContent == nullptr)
		return;
	SetValue(dVar, cVar);
}


Content::Content()
{
}

Content::~Content()
{


};

