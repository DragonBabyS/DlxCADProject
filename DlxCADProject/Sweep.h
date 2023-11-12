#pragma once
#include "EularOperation.h"
#include <vector>

//扫掠：有一个平面区域(直线段、圆弧、自由曲线围成的二维曲线)，延一条可定的轨迹线，扫描产生三维空间点集
//平移扫成(translational sweeping)：轨迹是一条直线。

void sweep(Face* f, float direction[], std::vector<Vertex*>& new_v, Face* outter_face);