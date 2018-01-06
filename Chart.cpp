#define HASHLENGTH 13
#include "Chart.h"
#include <iostream>	
using namespace std;

struct QuadNum
{
	int op;
	void* arg1;
	void* arg2;
	void* result;
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

//�����½���ʶ��
void BuildNewContent(char *name,int type,double value) {
	Content *newContent = new Content(name, type, nullptr, value);
	InserContent(newContent);
}

void InsertNewQuad(int op, void* arg1, void*  arg2, void*  result)
{
	quadNum->op = op;
	quadNum->arg1 = arg1;
	quadNum->arg2 =arg2;
	quadNum->result = result;
	quadNum->next = (QuadNum*)malloc(sizeof(QuadNum));
	quadNum = quadNum->next;
	quadNum->next = nullptr;
	ShowQuadTable();
}
void ShowQuadTable()
{
	int index = 1;
	QuadNum* tmp = baseQuad;
	cout<<"index	" << "op	" << "arg1		" << "arg2		" << "result		" << endl;
	while (tmp->next != nullptr)
	{
		cout << index<<"	";
		cout << tmp->op << "	";
		cout << tmp->arg1 <<"	";
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
	ShowChart();
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
			cout << "Z	" << *(int*)tmp->pValue<< "	";
		}
		else if (tmp->type == F)
		{
			cout << "F	" << *(float*)tmp->pValue << "	";
		}
		else if (tmp->type == C)
		{
			cout << "C	" <<*(char*)tmp->pValue << "	";
		}
		cout << tmp->address << "	";
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
//����ѡ��
//void OperateChoose(int num) {
//	Content *result;
//	char* name = new char;
//
//
//	switch (num)
//	{
//	case 1:
//		break;
//	case 2:
//		cout << "�����ʶ������" << endl;
//		cin >> name;
//		result = FindContent(name);
//		if (result == nullptr)
//			cout << "û�������ʶ��" << endl;
//		else
//		{
//			ShowDetail(result);
//		}
//		break;
//	case 3:
//		DeleteContent();
//		break;
//	case 4:
//		ShowChart();
//		break;
//	case 5:
//		ShowHash();
//		break;
//	default:
//		break;
//	}
//}

Content::Content()
{
}

Content::~Content()
{


};

