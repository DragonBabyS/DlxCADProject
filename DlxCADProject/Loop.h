#pragma once
#include "Vertex.h"

class Loop
{
public:
	Loop* prevl, * nextl; //loop��ǰ��loopָ��
	Face* lface;  //loop���ڵ���
	HalfEdge* ledg;  //loop����ʼ���
	bool inner; //�Ƿ����ڻ�
	Loop() :prevl(nullptr), nextl(nullptr), lface(nullptr), ledg(nullptr), inner(false) { };
};
