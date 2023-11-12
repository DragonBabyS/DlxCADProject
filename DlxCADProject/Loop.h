#pragma once
#include "Vertex.h"

class Loop
{
public:
	Loop* prevl, * nextl; //loop的前后loop指针
	Face* lface;  //loop所在的面
	HalfEdge* ledg;  //loop的起始半边
	bool inner; //是否是内环
	Loop() :prevl(nullptr), nextl(nullptr), lface(nullptr), ledg(nullptr), inner(false) { };
};
