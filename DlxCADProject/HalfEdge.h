#pragma once
#include "Vertex.h"


class HalfEdge
{
public:
	HalfEdge* prev, * next;  //半边的前向，后向指针
	Loop* wloop; //半边所在的环
	Edge* edg;   //半边所属的边
	Vertex* startv;//start vertex of halfedge
	HalfEdge() :prev(nullptr), next(nullptr), wloop(nullptr), edg(nullptr), startv(nullptr) { }
};
