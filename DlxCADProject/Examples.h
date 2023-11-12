#pragma once
#include "EularOperation.h"
#include "Sweep.h"
#include <GL/glut.h>
#include <map>
using namespace std;

map<float, map<float, map<float, int>>> m;//将坐标映射为id
vector<double*> mem_point;//gl使用的点信息,会分配内存

void print_Solid(Solid* s);
void print_Loop(Face* newf);

#define DRAW_EXAMPLE 2
Solid* Example1();
Solid* Example2();


// 每当你添加新点，请使用该ADD_VERTEX去构建映射和用于渲染的点
#define ADD_VERTEX(point)\
{\
	m[point[0]][point[1]][point[2]] = mem_point.size();\
	double *_tmp_p = new double[6]{ point[0],point[1],point[2],\
	color[c_s % color_num][0],color[c_s % color_num][1], color[(c_s++) % color_num][2] };\
	mem_point.push_back(_tmp_p); \
	int _id = mem_point.size()-1;\
	printf("point:%2d\t  pos: %4.1f, %4.1f, %4.1f, \tcolor: %2.1f, %2.1f, %2.1f \n",_id,point[0],point[1],point[2], mem_point[_id][3], mem_point[_id][4], mem_point[_id][5] );\
}\

//颜色信息
const int color_num = 9;
int c_s = 0;
GLdouble color[color_num][3] = { {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},{0.0, 0.0, 1.0},
								 {1.0, 1.0, 0.0},{1.0, 0.0, 1.0},{0.0, 1.0, 1.0},
								 {0.5, 0.5, 1.0},{0.5, 1.0, 0.5},{1.0, 0.5, 0.5} };

extern int print_time = 0;

Solid* Example1() {

	float point[12][3] = {
		{-2,1.5,0	},	    { -2,-1.5,0  },		{ 3,-1.5,0   },		{ 3,1.5,0   },
		{ -1,0.5,0  },    { -1,-0.5,0},		{ 0,-0.5,0   },	    { 0,0.5,0 },
		{2,0.5,0    },      {1,0.5,0     },     {1,-0.5,0    },     {2,-0.5,0   }

	};
	for (int i = 0; i < 12; i++) {
		ADD_VERTEX(point[i])
	}
	Face* f[4];
	Edge* e;
	Vertex* v[12];
	Solid* s = nullptr;
	// 构造一个方形
	mvfs(&v[0], &f[0], &s, point[0]);
	mev(&e, &v[1], (s->sface->floop), v[0], point[1]);
	mev(&e, &v[2], (s->sface->floop), v[1], point[2]);
	mev(&e, &v[3], (s->sface->floop), v[2], point[3]);
	mef(&e, &f[1], v[3], v[0], (f[0]->floop));

	//构造第一个环
	mev(&e, &v[4], f[0]->floop, v[0], point[4]);
	mev(&e, &v[7], f[0]->floop, v[4], point[7]);
	mev(&e, &v[6], f[0]->floop, v[7], point[6]);
	mev(&e, &v[5], f[0]->floop, v[6], point[5]);
	mef(&e, &f[2], v[5], v[4], f[0]->floop);

	//构造第二个环
	mev(&e, &v[8], f[0]->floop, v[3], point[8]);
	mev(&e, &v[9], f[0]->floop, v[8], point[9]);
	mev(&e, &v[10], f[0]->floop, v[9], point[10]);
	mev(&e, &v[11], f[0]->floop, v[10], point[11]);
	mef(&e, &f[3], v[11], v[8], f[0]->floop);

	Loop* lp1_tmp, * lp2_tmp;

	kemr(&lp1_tmp, v[4], v[0], f[0]->floop);
	kemr(&lp2_tmp, v[8], v[3], f[0]->floop);

	vector<Vertex*> new_v;
	float direction[3] = { 0,0,1 };
	sweep(s->sface, direction, new_v, f[1]);
	for (int i = 0; i < new_v.size(); i++) {
		float tmp_point[3] = { new_v[i]->x,new_v[i]->y, new_v[i]->z };
		ADD_VERTEX(tmp_point)
	}

	print_Solid(s);

	return s;
}



Solid* Example2() {
	Solid* s = nullptr;
	Face* f = nullptr, * tmp_f = nullptr;
	Edge* e = nullptr;
	Loop* lp = nullptr;
	Vertex* v[100];
	int v_idx;

	//字母D 前5为外环，后5为内环
	float point_d[10][3] = {
		{-2,1.5,-2.0},		{-4,1.5,-2.0},		{-4,-1.5,-2.0},		{-3.4,-1.5,-2.0},		{-2,-0.9,-2.0},
		{-2.3,1.2,-2.0},		{-3.7,1.2,-2.0},		{-3.7,-1.2,-2.0} ,	{-3.4,-1.2,-2.0},		{-2.3,-0.6,-2.0},
	};
	v_idx = mem_point.size();
	for (int i = 0; i < 10; i++) {
		ADD_VERTEX(point_d[i])
	}
	mvfs(&v[v_idx + 0], &f, &s, point_d[0]);
	for (int i = 1; i < 5; i++) {
		mev(&e, &v[v_idx + i], s->sface->floop, v[v_idx + i - 1], point_d[i]);
	}
	mef(&e, &tmp_f, v[v_idx + 4], v[v_idx + 0], f->floop);


	// 构造d的内环
	mev(&e, &v[v_idx + 5], s->sface->floop, v[v_idx + 0], point_d[5]);
	mev(&e, &v[v_idx + 9], s->sface->floop, v[v_idx + 5], point_d[9]);
	mev(&e, &v[v_idx + 8], s->sface->floop, v[v_idx + 9], point_d[8]);
	mev(&e, &v[v_idx + 7], s->sface->floop, v[v_idx + 8], point_d[7]);
	mev(&e, &v[v_idx + 6], s->sface->floop, v[v_idx + 7], point_d[6]);
	mef(&e, &tmp_f, v[v_idx + 6], v[v_idx + 5], f->floop);
	kemr(&lp, v[v_idx + 5], v[v_idx + 0], f->floop);


	//字母L 只有外环
	float point_l[6][3] = {
	{ -1,1.5,-2  },		{ -0.6,1.5,-2 },		{-0.6,-1.2,-2},	{ 0.7,-1.2,-2  },
	{ 0.7,-1.5,-2 },		{ -1,-1.5,-2}
	};
	v_idx = mem_point.size();
	for (int i = 0; i < 6; i++) {
		ADD_VERTEX(point_l[i])
	}

	mvfs(&v[v_idx + 0], &f, &s, point_l[0]);
	for (int i = 1; i < 6; i++) {
		mev(&e, &v[v_idx + i], s->sface->floop, v[v_idx + i - 1], point_l[i]);
	}
	mef(&e, &f, v[v_idx + 5], v[v_idx + 0], f->floop);


	//字母X 只有外环
	float point_x[12][3] = {
		{2,-1.2,-2},{2.3,-1.5,-2},{3,-0.2,-2},{3.7,-1.2,-2},
		{4,-1.5,-2},{3.1,0,-2},{4,1.2,-2},{3.7,1.5,-2},
		{3,0.2,-2},{2.3,1.5,-2},{2,1.2,-2},{2.9,0,-2}
	};
	v_idx = mem_point.size();
	for (int i = 0; i < 12; i++) {
		ADD_VERTEX(point_x[i])
	}

	mvfs(&v[v_idx + 0], &f, &s, point_x[0]);
	for (int i = 1; i < 12; i++) {
		mev(&e, &v[v_idx + i], s->sface->floop, v[v_idx + i - 1], point_x[i]);
	}
	mef(&e, &f, v[v_idx + 11], v[v_idx + 0], f->floop);


	// 外框
	float point_f[8][3] = {
	{5.5,3.6,-2.0},	{-5.5,3.6,-2.0},		{-5.5,-2.5,-2.0},		{5.5,-2.5,-2.0},
	{5,2,-2.0},	{-5,2,-2.0},	{-5,-2,-2.0} ,	{5,-2,-2.0}
	};
	v_idx = mem_point.size();
	for (int i = 0; i < 8; i++) {
		ADD_VERTEX(point_f[i])
	}
	mvfs(&v[v_idx + 0], &f, &s, point_f[0]);
	for (int i = 1; i < 4; i++) {
		mev(&e, &v[v_idx + i], s->sface->floop, v[v_idx + i - 1], point_f[i]);
	}
	mef(&e, &tmp_f, v[v_idx + 3], v[v_idx + 0], f->floop);
	// 构造外框及字母内环

	mev(&e, &v[v_idx + 4], s->sface->floop, v[v_idx + 0], point_f[4]);
	mev(&e, &v[v_idx + 7], s->sface->floop, v[v_idx + 4], point_f[7]);
	mev(&e, &v[v_idx + 6], s->sface->floop, v[v_idx + 7], point_f[6]);
	mev(&e, &v[v_idx + 5], s->sface->floop, v[v_idx + 6], point_f[5]);
	mef(&e, &tmp_f, v[v_idx + 5], v[v_idx + 4], f->floop);
	kemr(&lp, v[v_idx + 4], v[v_idx + 0], f->floop);
	point_f[4][0] = -4.05, point_f[4][1] = 3.3;
	point_f[5][0] = -5.05, point_f[5][1] = 3.3;
	point_f[6][0] = -5.05, point_f[6][1] = 2.3;
	point_f[7][0] = -4.05, point_f[7][1] = 2.3;
	for (int i = 0; i < 8; i++) {
		for (int j = 4; j < 8; j++) {
			ADD_VERTEX(point_f[j])
		}
		mev(&e, &v[v_idx + 4], s->sface->floop, v[v_idx + 0], point_f[4]);
		mev(&e, &v[v_idx + 7], s->sface->floop, v[v_idx + 4], point_f[7]);
		mev(&e, &v[v_idx + 6], s->sface->floop, v[v_idx + 7], point_f[6]);
		mev(&e, &v[v_idx + 5], s->sface->floop, v[v_idx + 6], point_f[5]);
		mef(&e, &tmp_f, v[v_idx + 5], v[v_idx + 4], f->floop);
		kemr(&lp, v[v_idx + 4], v[v_idx + 0], f->floop);
		for (int j = 4; j < 8; j++) {
			point_f[j][0] += 1.3;
		}
	}


	vector<Vertex*> new_v;
	float direction[3] = { 0.5,0.5,-1 };
	//前移到第一个face
	for (s; s->prevs != nullptr; s = s->prevs);

	printf("-----------------------follows the faces wait for sweep--------------------");
	for (Solid* tmp_s = s; tmp_s != nullptr; tmp_s = tmp_s->nexts) {
		print_Solid(s);
	}

	for (s; s->nexts != nullptr; s = s->nexts) {
		sweep(s->sface, direction, new_v, s->sface->nextf);
	}
	sweep(s->sface, direction, new_v, s->sface->nextf);

	for (int i = 0; i < new_v.size(); i++) {
		float tmp_point[3] = { new_v[i]->x,new_v[i]->y, new_v[i]->z };
		ADD_VERTEX(tmp_point)
	}

	for (s; s->prevs != nullptr; s = s->prevs);
	return s;
}




void print_Solid(Solid* s) {
	if (s == nullptr) {
		printf("no solid!");
		return;
	}
	printf("\n-----------------------------print time : %d-----------------------------------\n", print_time++);
	for (Face* f = s->sface; f != nullptr; f = f->nextf) {
		int lp_count = 0;
		if (f->floop == nullptr)
			printf("face id :%d has no loop\n", f->id);
		for (Loop* lp = f->floop; lp != nullptr; lp = lp->nextl) {
			printf("face id :%d ,the %d lp: ", f->id, lp_count);
			HalfEdge* edg = lp->ledg;
			Vertex* start = edg->startv;
			//printf("{%2.1f,%2.1f,%2.1f}, ", edg->startv->x,edg->startv->y,edg->startv->z);
			printf("%d - ", m[edg->startv->x][edg->startv->y][edg->startv->z]);
			edg = edg->next;
			while (edg->startv != start) {
				printf("%d - ", m[edg->startv->x][edg->startv->y][edg->startv->z]);
				edg = edg->next;
			}
			printf("%d", m[edg->startv->x][edg->startv->y][edg->startv->z]);

			if (lp->inner) printf("   inner loop!\n");
			else printf("\n");
			lp_count++;
		}
		printf("\n");
	}
}


void print_Loop(Face* newf) {

	printf("print time : %d ,face_id %d , the lp: ", print_time++, newf->id);
	Loop* _lp = newf->floop;
	HalfEdge* edg = _lp->ledg;
	Vertex* start = edg->startv;
	printf("{%2.0f,%2.0f,%2.0f}, ", edg->startv->x, edg->startv->y, edg->startv->z);
	edg = edg->next;
	while (edg->startv != start) {
		printf("{%2.0f,%2.0f,%2.0f}, ", edg->startv->x, edg->startv->y, edg->startv->z);
		edg = edg->next;
	}
	printf("{%2.0f,%2.0f,%2.0f}\t", edg->startv->x, edg->startv->y, edg->startv->z);

	if (_lp->inner) printf("   inner loop!\n");
	else printf("\n");
}
