#include "Sweep.h"

static int print_time = 0;
// ���ڵ���
extern void print_Loop(Face* newf);


void sweep(Face* f, float direction[], std::vector<Vertex*>& new_v, Face* outter_face) {
    // new_v Ϊ ɨ�ɲ����´����ĵ㣬�����Ա����ģ�͵�
    f = f->fsolid->sface;//�ҵ����������������

    //��ʵ���������е���������һ��vector
    std::vector<Face*> face_list;
    for (Face* tmp_f = f; f != nullptr; f = f->nextf) {
        face_list.push_back(f);
    }

    //�������������
    if (face_list.size() == 2) {
        face_list.clear();
        face_list.push_back(outter_face);//�ⲿ��
    }

    for (int i = 0; i < face_list.size(); i++) {
        //����ÿһ����
        Face* tmp_f = face_list[i];
        //���ĳһ����ֻ��һ�����������ڻ�������鿴��һ����
        if (tmp_f->floop->nextl != nullptr) {
            tmp_f = tmp_f->nextf;
            continue;
        }
        int v_beidx = new_v.size(); // begin index
        Loop* lp = tmp_f->floop; //��ǰ����׻�
        HalfEdge* he = lp->ledg;  //��ǰ�����װ��
        Vertex* firstv = he->startv; //��ǰ��ߵ���ʼ��

        Face* newf; //��������
        Edge* newe; //�����±�
        Vertex* newv; //�����µ�
        {
            //����λ���������µ�����
            float new_pos[3] = { he->startv->x + direction[0], he->startv->y + direction[1] ,he->startv->z + direction[2] };

            //�Ӱ�ߵ���ʼ�㵽�µ㹹���±�
            mev(&newe, &newv, lp, he->startv, new_pos);
        }
        new_v.push_back(newv); //��������µ�����´����ĵ㼯����
        he = he->next;//����ɨ����һ�����
        while (he->startv != firstv) { //�ظ�ɨ�赱ǰ�������а�ߣ�ֱ���ص���㣬�����µ㣬�����±�
            float new_pos[3] = { he->startv->x + direction[0], he->startv->y + direction[1] ,he->startv->z + direction[2] };
            mev(&newe, &newv, lp, he->startv, new_pos);
            new_v.push_back(newv);
            he = he->next;
        }
        // �����棬ʹ��֮ǰ������µ㣬�±߲��乹����
        for (int j = v_beidx; j < new_v.size() - 1; j++) {
            mef(&newe, &newf, new_v[j], new_v[j + 1], lp);
        }

        //���ʣ��һ��ʹ���յ��浥������
        mef(&newe, &newf, new_v[new_v.size() - 1], new_v[v_beidx], lp);
    }

    for (int i = 0; i < face_list.size(); i++) {
        Face* tmp_f = face_list[i];
        //���û�и���û���ڻ��Ҹ��治���ⲿ��
        if (tmp_f->floop->nextl == nullptr && tmp_f != outter_face) {
            kfmrh(outter_face, tmp_f);//������ɾ������ͨͨ��
        }
    }
    printf("Sweep a solid over \n");
}

