#pragma once

#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Loop.h"
#include "Soild.h"
#include "Vertex.h"

void mvfs(Vertex** v, Face** f, Solid** s, float point[3]); //���ɺ���һ������棬���ҹ���һ���µ���

void mev(Edge** ed, Vertex** v2, Loop* lp, Vertex* v1, float point[3]);//����һ���µĵ�ve2�����Ӹõ㵽���е�v1������һ���µı�

void mef(Edge** edg, Face** f2, Vertex* v1, Vertex* v2, Loop* lp1);//����ͬһ����������v1��v2������һ���±�edg��������һ������f2

void kemr(Loop** newlp, Vertex* v1, Vertex* v2, Loop* lp);//ɾ��һ����e�����ɸñ�ĳһ�����ϵ��µ��ڻ�newlp

void kfmrh(Face* f1, Face* f2);//ɾ������f1��Ӵ���һ����f2��������f1�ϵ�һ���ڻ������γ����ϵ�һ��ͨ��
