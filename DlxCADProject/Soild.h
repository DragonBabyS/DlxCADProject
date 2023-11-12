#pragma once
#include "Vertex.h"

class Solid
{
public:
	Solid* prevs, * nexts; //实体链的前后实体
	Face* sface; //实体的第一个面，可从该面找到其他面
	Edge* sedge; //实体的第一个边，可从该边找到其他边
	Solid() :prevs(nullptr), nexts(nullptr), sface(nullptr), sedge(nullptr) { }
};