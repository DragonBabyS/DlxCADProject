#pragma once
#include "Vertex.h"

class Face
{
public:
	static int num; //用于对面进行编号
	int id;  //面的id
	Face* prevf, * nextf; //面指针，分别指向前一个面和后一个面
	Solid* fsolid;  //面所属的体
	Loop* floop;   //面的环
	Face() :prevf(nullptr), nextf(nullptr), fsolid(nullptr), floop(nullptr) { id = num++; } //面id随创建顺序顺次+1

};


