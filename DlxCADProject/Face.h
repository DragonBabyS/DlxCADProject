#pragma once
#include "Vertex.h"

class Face
{
public:
	static int num; //���ڶ�����б��
	int id;  //���id
	Face* prevf, * nextf; //��ָ�룬�ֱ�ָ��ǰһ����ͺ�һ����
	Solid* fsolid;  //����������
	Loop* floop;   //��Ļ�
	Face() :prevf(nullptr), nextf(nullptr), fsolid(nullptr), floop(nullptr) { id = num++; } //��id�洴��˳��˳��+1

};


