#include "Sweep.h"

static int print_time = 0;
// 便于调试
extern void print_Loop(Face* newf);


void sweep(Face* f, float direction[], std::vector<Vertex*>& new_v, Face* outter_face) {
    // new_v 为 扫成操作新创建的点，传回以便输出模型点
    f = f->fsolid->sface;//找到面所在体的面链表

    //将实体面链表中的所有面存进一个vector
    std::vector<Face*> face_list;
    for (Face* tmp_f = f; f != nullptr; f = f->nextf) {
        face_list.push_back(f);
    }

    //如果仅有两个面
    if (face_list.size() == 2) {
        face_list.clear();
        face_list.push_back(outter_face);//外部面
    }

    for (int i = 0; i < face_list.size(); i++) {
        //遍历每一个面
        Face* tmp_f = face_list[i];
        //如果某一个面只有一个环（即无内环），则查看下一个面
        if (tmp_f->floop->nextl != nullptr) {
            tmp_f = tmp_f->nextf;
            continue;
        }
        int v_beidx = new_v.size(); // begin index
        Loop* lp = tmp_f->floop; //当前面的首环
        HalfEdge* he = lp->ledg;  //当前环的首半边
        Vertex* firstv = he->startv; //当前半边的起始点

        Face* newf; //构造新面
        Edge* newe; //构造新边
        Vertex* newv; //构造新点
        {
            //根据位移量构造新点坐标
            float new_pos[3] = { he->startv->x + direction[0], he->startv->y + direction[1] ,he->startv->z + direction[2] };

            //从半边的起始点到新点构造新边
            mev(&newe, &newv, lp, he->startv, new_pos);
        }
        new_v.push_back(newv); //将构造的新点加入新创建的点集合中
        he = he->next;//继续扫描下一个半边
        while (he->startv != firstv) { //重复扫描当前环的所有半边，直至回到起点，构造新点，构造新边
            float new_pos[3] = { he->startv->x + direction[0], he->startv->y + direction[1] ,he->startv->z + direction[2] };
            mev(&newe, &newv, lp, he->startv, new_pos);
            new_v.push_back(newv);
            he = he->next;
        }
        // 连接面，使用之前构造的新点，新边补充构造面
        for (int j = v_beidx; j < new_v.size() - 1; j++) {
            mef(&newe, &newf, new_v[j], new_v[j + 1], lp);
        }

        //最后剩下一个使体封闭的面单独构造
        mef(&newe, &newf, new_v[new_v.size() - 1], new_v[v_beidx], lp);
    }

    for (int i = 0; i < face_list.size(); i++) {
        Face* tmp_f = face_list[i];
        //如果没有该面没有内环且该面不是外部面
        if (tmp_f->floop->nextl == nullptr && tmp_f != outter_face) {
            kfmrh(outter_face, tmp_f);//将此面删除，打通通孔
        }
    }
    printf("Sweep a solid over \n");
}

