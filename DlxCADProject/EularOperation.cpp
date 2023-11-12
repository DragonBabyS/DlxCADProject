#include "EularOperation.h"
#include <vector>


extern void print_Loop(Face* newf);


//生成含有一个点的面，并且构成一个新的体
void  mvfs(Vertex** v, Face** f, Solid** s, float point[3])
{
	Solid* new_s = new Solid;
	*v = new Vertex(point);
	*f = new Face;
	Loop* lp = new Loop;
	new_s->sface = *f;

	(*f)->fsolid = new_s;
	(*f)->floop = lp;

	lp->lface = *f;

	if (*s == nullptr) {
		*s = new_s;
	}
	else {
		for (*s; (*s)->nexts != nullptr; (*s) = (*s)->nexts);
		(*s)->nexts = new_s;
		new_s->prevs = (*s);
		(*s) = new_s;//返回新s
	}
}


//生成一个新的点ve2，连接该点到已有点v1，构造一条新的边
void mev(Edge** edg, Vertex** v2, Loop* lp, Vertex* v1, float point[3])
{
	HalfEdge* he1 = new HalfEdge;
	HalfEdge* he2 = new HalfEdge;
	*edg = new Edge;
	*v2 = new Vertex(point);

	(*edg)->he1 = he1;
	(*edg)->he2 = he2;

	he1->startv = v1;
	he1->wloop = lp;
	he1->next = he2;
	he1->edg = *edg;

	he2->startv = *v2;
	he2->wloop = lp;
	he2->prev = he1;
	he2->edg = *edg;

	if (lp->ledg == nullptr) {
		he1->prev = he2;
		he2->next = he1;
		lp->ledg = he1;
	}
	else {
		HalfEdge* he;
		for (he = lp->ledg; he->next->startv != v1; he = he->next);

		he2->next = he->next;
		he->next->prev = he2;

		he->next = he1;
		he1->prev = he;
	}

}


//连接同一面上两个点v1，v2，生成一条新边edg，并产生一个新面f2
void mef(Edge** edg, Face** f2, Vertex* v1, Vertex* v2, Loop* lp1)
{

	HalfEdge* he1 = new HalfEdge;
	HalfEdge* he2 = new HalfEdge;
	Loop* lp2 = new Loop;
	*f2 = new Face;
	*edg = new Edge;

	// 1.构造边的信息
	(*edg)->he1 = he1;
	(*edg)->he2 = he2;
	he1->edg = he2->edg = *edg;

	// 2.构造半边
	he1->startv = v1;
	he2->startv = v2;

	// 找以v1 ,v2为起点的半边并修改指针,这种半边可能有多条,应该找出最短的一条路径
	// 以下代码能处理部分半边有多条情况
	{
		int v1_he = -1, v2_he = -1;
		HalfEdge* he_tmp[10];//保存至多十个
		int he_num = 0; //v1 ,v2为起点的半边的数量

		// 找到所有以v1,v2为起点的边
		if (lp1->ledg->startv == v1 || lp1->ledg->startv == v2) {
			he_tmp[he_num++] = lp1->ledg;
		}
		for (HalfEdge* tmphe = lp1->ledg->next; tmphe != lp1->ledg; tmphe = tmphe->next) {
			if (tmphe->startv == v1 || tmphe->startv == v2) {
				he_tmp[he_num++] = tmphe;
			}
		}

		//遍历所有以v1, v2为起点的边，找到其路径最短的的半边编号
		for (int i = 0; i <= he_num; i++) {
			if (he_tmp[i % he_num]->startv == v1 && he_tmp[(i + 1) % he_num]->startv == v2) {
				v1_he = i;
				v2_he = i + 1;
				break;
			}
		}
		he1->next = he_tmp[v2_he % he_num];
		he1->prev = he_tmp[v1_he % he_num]->prev;
		he2->next = he_tmp[v1_he % he_num];
		he2->prev = he_tmp[v2_he % he_num]->prev;

		he1->next->prev = he1;
		he2->next->prev = he2;
		he1->prev->next = he1;
		he2->prev->next = he2;
	}

	// 3.构造两个环
	he1->wloop = lp1;
	he2->wloop = lp2;

	//he2所在环其他半边的所属环均在原来的lp1环上，将其所在环修改为lp2
	HalfEdge* tmp;
	for (tmp = he2->next; tmp->startv != v2; tmp = tmp->next)
		tmp->wloop = lp2;

	//重新赋值两个环的起始半边
	lp1->ledg = he1;
	lp2->ledg = he2;
	//lp2->inner = true;//指定lp2为内环

	lp2->lface = *f2;  //新环在新生成的面上
	(*f2)->floop = lp2;  //赋值新面上的环
	(*f2)->fsolid = lp1->lface->fsolid; //新面所属的体是原来点所在面的所在体

	// 4.在体内补充面
	Face* f_end;
	//将新生成的面加入所在体的面链表
	for (f_end = lp1->lface->fsolid->sface; f_end->nextf != nullptr; f_end = f_end->nextf);
	f_end->nextf = *f2;
	(*f2)->prevf = f_end;
}


//删除一条边e，生成该边某一邻面上的新的内环newlp
void kemr(Loop** newlp, Vertex* v1, Vertex* v2, Loop* lp)
{
	*newlp = new Loop;//新的内环


	HalfEdge* he1, * he2;
	//遍历原来的环，分别找到以v1,v2为起点，且不在以v1,v2为边的两端点的半边，也即：分别找到删除指定边后内外环以v1,v2为起点的
	for (he1 = lp->ledg; he1->startv != v1 || he1->next->startv == v2; he1 = he1->next);
	for (he2 = lp->ledg; he2->startv != v2 || he2->next->startv == v1; he2 = he2->next);

	HalfEdge* tmp1 = he1->prev, * tmp2 = he2->prev;


	he1->prev->next = he2->next;
	he2->next->prev = he1->prev;

	he1->next->prev = he2->prev;
	he2->prev->next = he1->next;

	lp->ledg = tmp1;    //旧环的起始半边
	(*newlp)->ledg = tmp2;  //新内环的起始半边

	tmp2->wloop = *newlp;
	//对新内环的所属半边的所在环赋值为新内环
	for (HalfEdge* hf = tmp2->next; hf != tmp2; hf = hf->next)
		hf->wloop = *newlp;

	(*newlp)->inner = true;//newlp为新的内环


	//将新内环加入到原环所在面的环链表中
	if (lp->lface->floop == nullptr)
		lp->lface->floop = *newlp;
	else {
		(*newlp)->nextl = lp->lface->floop->nextl;
		lp->lface->floop->nextl = *newlp;
	}

	//释放待删边及其所属两个半边的空间
	delete he1->edg;
	delete he1;
	delete he2;

}


//删除与面f1相接触的一个面f2，生成面f1上的一个内环，并形成体上的一个通孔
void kfmrh(Face* f1, Face* f2)
{
	// 将f2从体内删除
	Face* tmpf = f1->fsolid->sface;
	if (tmpf != f2) {
		f2->prevf->nextf = f2->nextf;
		f2->nextf->prevf = f2->prevf;
	}
	else {
		f1->fsolid->sface = f2->nextf;
		f2->nextf->prevf = nullptr;
	}

	// 修改f2中loop指向的面
	Loop* f2lp_start = f2->floop, * f2lp_end;
	for (Loop* tmp_f2lp = f2->floop; tmp_f2lp != nullptr; tmp_f2lp = tmp_f2lp->nextl) {
		tmp_f2lp->lface = f1;
		tmp_f2lp->inner = true;
	}

	// 将f2的环插入f1
	Loop* f1lp_end;
	for (f1lp_end = f1->floop; f1lp_end->nextl != nullptr; f1lp_end = f1lp_end->nextl);
	f1lp_end->nextl = f2->floop;
	f2->floop->prevl = f1lp_end;

	delete f2;
}