#pragma once
#include "EularOperation.h"
#include <vector>

//ɨ�ӣ���һ��ƽ������(ֱ�߶Ρ�Բ������������Χ�ɵĶ�ά����)����һ���ɶ��Ĺ켣�ߣ�ɨ�������ά�ռ�㼯
//ƽ��ɨ��(translational sweeping)���켣��һ��ֱ�ߡ�

void sweep(Face* f, float direction[], std::vector<Vertex*>& new_v, Face* outter_face);