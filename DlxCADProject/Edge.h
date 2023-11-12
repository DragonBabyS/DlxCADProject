#pragma once
#pragma once
#include "Vertex.h"

class Edge
{
public:
	HalfEdge* he1, * he2;  //一条边的两个半边
	Edge() :he1(nullptr), he2(nullptr) { }    //构造函数初始化为null
};