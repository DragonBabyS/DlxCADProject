#pragma once
#pragma once
#include "Vertex.h"

class Edge
{
public:
	HalfEdge* he1, * he2;  //һ���ߵ��������
	Edge() :he1(nullptr), he2(nullptr) { }    //���캯����ʼ��Ϊnull
};