#pragma once
#include "Vertex.h"


class HalfEdge
{
public:
	HalfEdge* prev, * next;  //��ߵ�ǰ�򣬺���ָ��
	Loop* wloop; //������ڵĻ�
	Edge* edg;   //��������ı�
	Vertex* startv;//start vertex of halfedge
	HalfEdge() :prev(nullptr), next(nullptr), wloop(nullptr), edg(nullptr), startv(nullptr) { }
};
