#pragma once
#include<vector>
class Solid;
class Face;
class Loop;
class Edge;
class HalfEdge;

class Vertex
{
public:
	float x, y, z;//点的坐标
	Vertex(float point[3]) :x(point[0]), y(point[1]), z(point[2]) { }  //构造函数，传入point数组做点的坐标初始化
};