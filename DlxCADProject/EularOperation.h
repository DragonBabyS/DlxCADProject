#pragma once

#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Loop.h"
#include "Soild.h"
#include "Vertex.h"

void mvfs(Vertex** v, Face** f, Solid** s, float point[3]); //生成含有一个点的面，并且构成一个新的体

void mev(Edge** ed, Vertex** v2, Loop* lp, Vertex* v1, float point[3]);//生成一个新的点ve2，连接该点到已有点v1，构造一条新的边

void mef(Edge** edg, Face** f2, Vertex* v1, Vertex* v2, Loop* lp1);//连接同一面上两个点v1，v2，生成一条新边edg，并产生一个新面f2

void kemr(Loop** newlp, Vertex* v1, Vertex* v2, Loop* lp);//删除一条边e，生成该边某一邻面上的新的内环newlp

void kfmrh(Face* f1, Face* f2);//删除与面f1相接触的一个面f2，生成面f1上的一个内环，并形成体上的一个通孔
