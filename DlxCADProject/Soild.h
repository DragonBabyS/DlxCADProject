#pragma once
#include "Vertex.h"

class Solid
{
public:
	Solid* prevs, * nexts; //ʵ������ǰ��ʵ��
	Face* sface; //ʵ��ĵ�һ���棬�ɴӸ����ҵ�������
	Edge* sedge; //ʵ��ĵ�һ���ߣ��ɴӸñ��ҵ�������
	Solid() :prevs(nullptr), nexts(nullptr), sface(nullptr), sedge(nullptr) { }
};