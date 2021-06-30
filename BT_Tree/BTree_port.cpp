#include"BTree_head.h"



void newRoot(BTree &t, BTree p, KeyType x, BTree ap,Recode rec) {//生成新的结点t,只有一个关键字x，两个孩子指针依次为p,ap
	t = (BTree)malloc(sizeof(BTNode));
	t->keynum = 1; t->key[1] = x; t->ptr[0] = p; t->ptr[1] = ap; t->recptr[1] = rec;
	for (int i = 2; i <= m-1; i++)t->ptr[i] = NULL;
	if (p != NULL)p->parent = t;
	if (ap != NULL)ap->parent = t;
	t->parent = NULL;
}

void PrintBTree(BTree t) {//凹入表1显示B树t
	int j,depth;
	depth = NodeDepth(t);//求结点在树中的深度
	if (0 == depth)return;
	for (j = 1; j <= 4*m*(depth-1); j++)printf(" ");//空格数也可以满足一定的规律
	for (j = 1; j <= t->keynum-1; j++) printf("%lld,",t->key[j]);
	if (t->keynum > 0)printf("%lld\n", t->key[j]);
	else printf("\n");
	for (j = 0; j <= t->keynum; j++) {
		PrintBTree(t->ptr[j]);
	}
}
int NodeDepth(BTree t) {//求结点所在层数,0层表示NULL
	int i;
	if (NULL == t)return 0;
	for (i = 1; NULL != t->parent; i++) {
		t = t->parent;
	}
	return i;
}
void AddKey(BTree &q, KeyType x, BTree ap,Recode rec) {//在q结点后方加入关键字,子结点指针与记录的指针
	if (q->keynum == m-1)return;
	q->keynum++;
	q->key[q->keynum] = x;
	q->ptr[q->keynum] = ap;
	q->recptr[q->keynum] = rec;
	if (ap != NULL)ap->parent = q;
}


void Insert(BTree &q, int i, KeyType x, BTree ap,Recode rec) {
	//关键字x和新结点指针ap分别插入到q->key[i]和q->ptr[i]
	//若中间有空关键字的，插入失败，会产生结点的损坏
	//作为辅助函数
	int j, n = q->keynum;
	for (j = n; j >= i; j--) {//关键字依次后移
		q->ptr[j + 1] = q->ptr[j];
		q->key[j + 1] = q->key[j];
		q->recptr[j + 1] = q->recptr[j];
	}
	q->ptr[i] = ap; q->key[i] = x; q->recptr[i] = rec;
	if (ap != NULL)ap->parent = q;
	q->keynum++;
}
void split(BTree &q, int s, BTree &ap) {
	//B树的q结点有n个关键字，分裂为两个结点，前一半（s-1,A0，K1，A1，...，Ks-1，As-1）保留在原结点中,
	//另一半（As,Ks+1,...Kn,An）移入新结点(n-s,A0,K1,A1,..,Kn-s,An-s),ap指向这个新结点
	int i, j, n = q->keynum;
	ap = (BTNode*)malloc(sizeof(BTNode));//开辟新的结点
	ap->ptr[0] = q->ptr[s];//将结点q的As移入ap的A0
	for (i = s + 1, j = 1; i <= n; i++, j++) {//将结点q的(Ks+1,As+1,..,Km,Am)依次移入ap的(K1，A1,..,Km-s,Am-s)
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
		ap->recptr[j] = q->recptr[i];
	}
	ap->keynum = n - s;//n个关键字的结点，前s个保留在原结点，一个待插入的保存起来，剩余n-s个在新结点；
	ap->parent = q->parent;//生成的新结点是原结点在同一层的兄弟，双亲相同
	for (i = 0; i <= n - s; i++)//修改原来指向q的q的子结点的parent域指向新结点ap
		if (ap->ptr[i] != NULL)ap->ptr[i]->parent = ap;
	q->keynum = s - 1;//修改原结点的指针个数
}

void InsertBTree(BTree &t, KeyType k, Recode rec,BTree q, int i) {
	//在B树t中的q结点的key[i-1]和key[i]之间插入关键字k
	//若插入后结点的关键字个数等于B树的阶（这里设为m），则沿着双亲结点进行分裂。
	//B树可简写为(n(关键字个数),A0,K1,A1，..,Kn,An）
	KeyType x;
	Recode f;
	int s, finished = 0, needNewRoot = 0;
	BTree ap;
	if (NULL == q)newRoot(t, NULL, k, NULL,rec);
	else {
		x = k; f = rec; ap = NULL;//x与ap保存即将插入的关键字和孩子结点指针
		while (0 == needNewRoot && 0 == finished) {//如果有开辟新结点的标志或者插入成功的标志则跳出，否则继续
			Insert(q, i, x, ap,f);//和ap分别插入到q中的q->key[i]与q->ptr[i],
								//其余关键字与孩子结点指针后移。
			if (q->keynum < m)finished = 1;//不超过m-1合法，置结束标志
			else {//结点的关键字数超过了m-1，则需要分裂结点
				s = (m + 1) / 2;
				split(q, s, ap);//q结点有n个关键字，分裂为两个结点，前一半（s-1,A0，K1，A1，...，Ks-1，As-1）保留在原结点中,
								//另一半（As,Ks+1,...Kn,An）移入新结点(n-s,A0,K1,A1,..,Kn-s,An-s),ap指向这个新结点
				x = q->key[s];//待插入的关键字换成Ks
				f = q->recptr[s];
				if (q->parent != NULL) {  //有双亲可以试着插入新确定新的关键字x与新的孩子节点指针ap
					q = q->parent; i = SearchPosition(q, x);//在双亲结点中查找x的插入位置
				}
				else needNewRoot = 1;//无双亲供插入则设置一个需要开辟新结点的标志
			}
		}
		if (1 == needNewRoot)//需要开辟新结点
			newRoot(t, q, x, ap,f);//开辟一个新结点t，只有一个关键字x，第一个孩子结点指针指向q，第二个孩子结点指针指向ap
	}
}

int SearchPosition(BTree t, KeyType x) {//寻找x在t结点的插入位置i,其中key[i-1]<x<=key[i]
	int i=1;
	if (NULL == t)return 0;
	while (i <= t->keynum) {
		if (t->key[i] >= x)return i;
		i++;
	}
	return i;
}

void BuildBTree(BTree &t) {//输入B树的关键字和记录来构建B树
	KeyType x;
	Recode p;
	p = newRecode();
	result r;
	while (1){
		scanf_s("%lld", &x);
		newBook(p);
		if (x == 0)return;
		SearchInsert(t, x, r);
		InsertBTree(t,x,p,r.pt,r.i);
	}
	return;
}
void SearchInsert(BTree &t, KeyType x, result &r) {
	//根据大小关系寻找B树t中可以插入到最下层结点的结果
	//若树为空，则r.pt显示的插入结果是插入空结点
	int i;
	if (t == NULL) {
		r.pt = NULL;
		r.tag = 1;
		r.i = 1;
		return;
	}
	BTree  p = t, q = NULL;
	while (p != NULL) {
		i = SearchPosition(p, x);
		q = p; p = p->ptr[i - 1];
	}
	r.tag = 0; r.pt = q; r.i = i;
}


void SearchBTree(BTree &t,KeyType x,result &r) {
	//根据大小关系寻找B树t中的x的位置，找不到则返回插入位置
	int i = 0, found = 0;
	BTree  p = t, q = NULL;
	while (p != NULL && 0 == found) {
		i = SearchPosition(p, x);
		if (i <= p->keynum&&p->key[i] == x)found = 1;
		else {
			q = p; p = p->ptr[i-1];
		}
	}
	if (1 == found) {
		r.pt = p; r.tag = 1; r.i = i;
	}
	else {
		r.tag = 0; r.pt = q; r.i = i;
	}
}
void BorrowLeft(BTree &ap) {
	//结点ap向左兄弟借关键字（关键字顺时针循环）
	//相应的调整好B树
	if (ap == NULL||ap->parent==NULL)return;
	int i = ParentPosition(ap);//寻找ap的双亲指向它的指针ptr[i]的下标i
	if (ap->parent->ptr[i - 1] == NULL)return;
	int k = ap->parent->ptr[i - 1]->keynum;
	if (k <= minnum || ap->keynum == m)return;//不够借,或者自身已满
	Insert(ap, 1, ap->parent->key[i],NULL,ap->parent->recptr[i]);//父结点的关键字移下来
	ap->ptr[1] = ap->ptr[0];
	ap->ptr[0] = ap->parent->ptr[i - 1]->ptr[ap->parent->ptr[i - 1]->keynum];
	//左兄弟的最后一个指针给它
	if (NULL != ap->ptr[0])ap->ptr[0]->parent = ap;
	ap->parent->key[i] = ap->parent->ptr[i - 1]->key[ap->parent->ptr[i - 1]->keynum];//左兄弟的一个关键字移入父节点
	ap->parent->recptr[i] = ap->parent->ptr[i - 1]->recptr[ap->parent->ptr[i - 1]->keynum];
	ap->parent->ptr[i - 1]->keynum--;//左兄弟少一个
}
int ParentPosition(BTree ap) {
	//寻找ap的双亲指向它的指针ptr[i]的下标i
	if (NULL == ap)return -1;
	for (int i = 0; i <= ap->parent->keynum; i++) {
		if (ap->parent->ptr[i] == ap)return i;
	}
	return -1;
}

void BorrowRight(BTree &ap) {
	//结点ap向右兄弟借关键字（关键字顺时针循环）
	//相应的调整好B树
	if (ap == NULL || ap->parent == NULL)return;
	int i = ParentPosition(ap);//寻找ap的双亲指向它的指针ptr[i]的下标i
	if (ap->parent->ptr[i + 1] == NULL)return;
	int k = ap->parent->ptr[i + 1]->keynum;
	if (k <= minnum || ap->keynum == m - 1)return;//不够借,或者自身会溢出
	Insert(ap, ap->keynum + 1, ap->parent->key[i + 1], NULL,ap->parent->recptr[i + 1]);
	ap->parent->key[i + 1] = ap->parent->ptr[i + 1]->key[1];
	ap->parent->recptr[i + 1] = ap->parent->ptr[i + 1]->recptr[1];
	ap->ptr[ap->keynum] = ap->parent->ptr[i + 1]->ptr[0];
	ap->parent->ptr[i + 1]->ptr[0] = ap->parent->ptr[i + 1]->ptr[1];
	for (int j = 1; j <= ap->parent->ptr[i + 1]->keynum - 1; j++) {//youyi
		ap->parent->ptr[i + 1]->key[j] = ap->parent->ptr[i + 1]->key[j + 1];
		ap->parent->ptr[i + 1]->recptr[j] = ap->parent->ptr[i + 1]->recptr[j + 1];
		ap->parent->ptr[i + 1]->ptr[j] = ap->parent->ptr[i + 1]->ptr[j + 1];
	}
	ap->parent->ptr[i + 1]->keynum--;
	if (ap->ptr[ap->keynum] != NULL)ap->ptr[ap->keynum]->parent = ap;
}


void CombineLeft(BTree &ap) {//合并结点到它的左兄弟中
	if (ap == NULL)return;
	if (ap->parent == NULL)return;
	int i,j;
	BTree q;
	i = ParentPosition(ap);//寻找ap的双亲指向它的指针ptr[i]的下标i
	if (i == 0)return;
	if (ap->keynum + ap->parent->ptr[i - 1]->keynum + 1 >= m)return;
	int k = ap->parent->ptr[i - 1]->keynum;
	ap->parent->ptr[i - 1]->key[k + 1] = ap->parent->key[i];//把双亲的一个对应的关键字移入左兄弟
	ap->parent->ptr[i - 1]->recptr[k + 1] = ap->parent->recptr[i];
	ap->parent->ptr[i - 1]->ptr[k + 1] = ap->ptr[0];
	if(ap->ptr[0]!=NULL)ap->parent->ptr[i - 1]->ptr[k + 1]->parent = ap->parent->ptr[i - 1];
	k++;
	for (j = 1; j <= ap->keynum; j++) {//把结点的所有关键字和孩子结点都复制到左兄弟
		ap->parent->ptr[i - 1]->ptr[k + 1] = ap->ptr[j];
		if (ap->ptr[j] != NULL)ap->ptr[j]->parent = ap->parent->ptr[i - 1];
		ap->parent->ptr[i - 1]->key[k + 1] = ap->key[j];
		ap->parent->ptr[i - 1]->recptr[k + 1] = ap->recptr[j];
		k++;
	}
	ap->parent->ptr[i - 1]->keynum = k;
	for (j = i; j <= ap->parent->keynum; j++) {//双亲结点关键字左移
		ap->parent->key[j] = ap->parent->key[j + 1];
		ap->parent->recptr[j] = ap->parent->recptr[j + 1];
		ap->parent->ptr[j] = ap->parent->ptr[j + 1];
	}
	ap->parent->keynum--;
	q= ap->parent->ptr[i - 1];
	free(ap);
	ap = q;
}
void CombineRight(BTree &ap) {//合并结点到它的右兄弟中
	if (ap == NULL)return;
	if (ap->parent == NULL)return;
	int i;
	BTree q;
	i = ParentPosition(ap);//寻找ap的双亲指向它的指针ptr[i]的下标i
	if (i == ap->parent->keynum )return;
	q = ap->parent->ptr[i + 1];
	CombineLeft(q);
	ap = q;
}

void Successor(BTree &p,int &i) {//替换关键字为其最下层直接后继,返回该结点位置
	if (p == NULL)return;
	BTree q = p;
	if (p->ptr[i] != NULL)p = p->ptr[i];
	while (p->ptr[0] != NULL)p = p->ptr[0];
	q->key[i] = p->key[1];
	q->recptr[i] = p->recptr[1];
	i = 1;
}

void Remove(BTree &t, int i) {
	//从终端结点t中移除ki
	if (t == NULL)return;
	for (int j = i; j <= t->keynum - 1; j++) {
		t->recptr[j] = t->recptr[j + 1];
		t->key[j] = t->key[j + 1];
		t->ptr[j] = t->ptr[j + 1];
	}
	t->keynum--;
}

void Restore(BTree &t) {
	//调整t结点(仅考虑删除后的调整)
	if (t == NULL)return;
	int i,LN, RN, finished = 0;
	while (0 == finished && t->parent != NULL) {
		i=ParentPosition(t);
		if (i!=0)LN = t->parent->ptr[i - 1]->keynum;
		else LN = 0;
		if (i!=t->parent->keynum)RN = t->parent->ptr[i+1]->keynum;
		else RN = 0;
		if (t->keynum >= minnum)return;
		if (LN <= minnum&&RN <= minnum) {
			if (LN == 0)CombineRight(t);
			else CombineLeft(t);
			t = t->parent;
		}
		else if (LN > RN) {
			BorrowLeft(t);
			finished = 1;
		}
		else { 
			BorrowRight(t);
			finished = 1;
		}
	}
}
void DeleteKey(BTree &p, int i) {
	//B树p结点上删除i位置的关键字x
	//然后返回停止调整时的结点
	//删除后p会改变位置
	if (p->ptr[i - 1] != NULL) {//不是最下层非终端结点
		Successor(p, i);//替换关键字为其最下层直接后继
		DeleteKey(p, 1);//改为删除最下层非终端结点关键字
	}
	else {
		Remove(p, i);//从结点p中移除ki
		if (p->keynum < minnum)Restore(p);//B树调整
	}
}
void BTDelete(BTree &p,KeyType x) {//在B树p中删除x记录
	result r;
	SearchBTree(p, x, r);
	if (r.tag == 0)return;
	else DeleteKey(r.pt, r.i);

}
void BTInsert(BTree &p, KeyType x,Recode rec) {
	//在B树p中插入x关键字和记录指针rec
	//若树为空，则新建一棵树
	result r;
	SearchInsert(p, x, r);
	InsertBTree(p, x,rec,r.pt, r.i);
}

