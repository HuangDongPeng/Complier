#define HASHLENGTH 13
#include "Chart.h"
#include <iostream>	
using namespace std;

Content * tmpContent = new Content();


struct ConstTable
{
	class Content *content = nullptr;
	ConstTable* next = nullptr;
};
struct ConstTable* constTable;
struct ConstTable* baseConstTable;

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


//添加四元式
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
//显示四元式
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
//添加新标识符
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
//添加常量
void InsertConstNum(Content *content) {
	Content *tmp = FindConstContent(content->type, content->data);
	if (tmp != nullptr)
		return;

	constTable = (ConstTable*)malloc(sizeof(ConstTable));

	if (baseConstTable == nullptr)
	{
		baseConstTable = constTable;
	}


	constTable->content = content;
	content->addr = constTableAdress;
	constTableAdress += 1;
	constTable->next = (ConstTable*)malloc(sizeof(ConstTable));
	constTable = constTable->next;
	constTable->content = nullptr;

}
//查找常量表
Content* FindConstContent(Property _type, DataType data)
{
	struct ConstTable* tmp = baseConstTable;
	while (tmp != nullptr)
	{
		if (tmp->content->type != _type) {
			tmp = tmp->next;
			continue;
		}
		else
		{
			switch (_type)
			{
			case Z:
				if (tmp->content->data.intVal == data.intVal)
					return tmp->content;
				break;
			case F:
				if (tmp->content->data.floatVal == data.floatVal)
					return tmp->content;
				break;
			case C:
				if (tmp->content->data.charVal == data.charVal)
					return tmp->content;
				break;
			case CONST:
				break;
			default:
				break;
			}
			tmp = tmp->next;
			if (tmp->content == nullptr)
				break;
		}


	}
	return nullptr;
}
//删除标识符
void DeleteContent() {
	Content *target = s.top();
	s.pop();
	DeleteContent_HashMap(target);
	delete(target);
}
//初始化哈希表
void InitMap() {
	for (int i = 0; i < HASHLENGTH; i++) {
		hashMap[i] = nullptr;
	}
}
//标识符名转换为Key
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
//在哈希表插入key
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
	//如果哈希表位置已被占用，则设置冲突链
	if (hashMap[key] != nullptr)
	{
		content->link = hashMap[key];
	}

	hashMap[key] = content;
}
//从哈希表删除key
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
//显示元组内容
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
//String 转 char*
char* String2Char(string name) {
	char *result = new char;
	strcpy(result, name.data());
	return result;
}
//寻找标识符
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
//打印符号表
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
	cout << "———————————————————————————————" << endl;
}
//打印哈希表
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
	cout << "———————————————————————————————" << endl;

}

///////辅助函数
float GetValue(Content *content)
{
	switch (content->type)
	{
	case Z:
		return (content->data.intVal);
		break;
	case F:
		return (content->data.floatVal);
	default:
		break;
	}
	return 0;
}

char GetValueChar(Content *content) {
	return (content->data.charVal);
}

void SetValue(double dVar = 0, char cVar = ' ')
{
	Content* tmp;
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
	Content* tmpFind;
	tmp->type = _type;
	DataType data;

	switch (tmp->type)
	{
	case Z:
		data.intVal = dVar;
		tmpFind = FindConstContent(tmp->type, data);
		if (tmpFind != nullptr)
			return tmpFind;
		tmp->data.intVal = dVar;
		break;
	case F:
		data.floatVal = dVar;
		tmpFind = FindConstContent(tmp->type, data);
		if (tmpFind != nullptr)
			return tmpFind;
		tmp->data.floatVal = dVar;
		break;
	case C:
		data.charVal = cVar;
		tmpFind = FindConstContent(tmp->type, data);
		if (tmpFind != nullptr)
			return tmpFind;
		tmp->data.charVal = cVar;
		break;
	case CONST:
		cout << "CONST set value with return" << endl;

		tmp->value = dVar;
		InsertConstNum(tmp);
		break;
	default:
		break;
	}
	cout << "set value with return" << endl;

	return tmp;
}

void SetValueAfterDeclare(char* name, double dVar = 0, char cVar = ' ') {
	tmpContent = FindContent(name);
	if (tmpContent == nullptr)
		return;
	SetValue(dVar, cVar);
}

bool CompareData(Content* a,Content* b) {
	if (a->type == b->type) {
		switch (a->type)
		{
		case Z:
			if (a->data.intVal == b->data.intVal)
				return true;
			break;
		case C:
			if (a->data.charVal == b->data.charVal)
				return true;
			break;
		case F:
			if (a->data.floatVal == b->data.floatVal)
				return true;
			break;	
		default:
			break;
		}
	}
	else
	{
		return false;
	}
	return false;
}


Content::Content()
{
}

Content::~Content()
{


};

