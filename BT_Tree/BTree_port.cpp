#include"BTree_head.h"



void newRoot(BTree &t, BTree p, KeyType x, BTree ap,Recode rec) {//�����µĽ��t,ֻ��һ���ؼ���x����������ָ������Ϊp,ap
	t = (BTree)malloc(sizeof(BTNode));
	t->keynum = 1; t->key[1] = x; t->ptr[0] = p; t->ptr[1] = ap; t->recptr[1] = rec;
	for (int i = 2; i <= m-1; i++)t->ptr[i] = NULL;
	if (p != NULL)p->parent = t;
	if (ap != NULL)ap->parent = t;
	t->parent = NULL;
}

void PrintBTree(BTree t) {//�����1��ʾB��t
	int j,depth;
	depth = NodeDepth(t);//���������е����
	if (0 == depth)return;
	for (j = 1; j <= 4*m*(depth-1); j++)printf(" ");//�ո���Ҳ��������һ���Ĺ���
	for (j = 1; j <= t->keynum-1; j++) printf("%lld,",t->key[j]);
	if (t->keynum > 0)printf("%lld\n", t->key[j]);
	else printf("\n");
	for (j = 0; j <= t->keynum; j++) {
		PrintBTree(t->ptr[j]);
	}
}
int NodeDepth(BTree t) {//�������ڲ���,0���ʾNULL
	int i;
	if (NULL == t)return 0;
	for (i = 1; NULL != t->parent; i++) {
		t = t->parent;
	}
	return i;
}
void AddKey(BTree &q, KeyType x, BTree ap,Recode rec) {//��q���󷽼���ؼ���,�ӽ��ָ�����¼��ָ��
	if (q->keynum == m-1)return;
	q->keynum++;
	q->key[q->keynum] = x;
	q->ptr[q->keynum] = ap;
	q->recptr[q->keynum] = rec;
	if (ap != NULL)ap->parent = q;
}


void Insert(BTree &q, int i, KeyType x, BTree ap,Recode rec) {
	//�ؼ���x���½��ָ��ap�ֱ���뵽q->key[i]��q->ptr[i]
	//���м��пչؼ��ֵģ�����ʧ�ܣ������������
	//��Ϊ��������
	int j, n = q->keynum;
	for (j = n; j >= i; j--) {//�ؼ������κ���
		q->ptr[j + 1] = q->ptr[j];
		q->key[j + 1] = q->key[j];
		q->recptr[j + 1] = q->recptr[j];
	}
	q->ptr[i] = ap; q->key[i] = x; q->recptr[i] = rec;
	if (ap != NULL)ap->parent = q;
	q->keynum++;
}
void split(BTree &q, int s, BTree &ap) {
	//B����q�����n���ؼ��֣�����Ϊ������㣬ǰһ�루s-1,A0��K1��A1��...��Ks-1��As-1��������ԭ�����,
	//��һ�루As,Ks+1,...Kn,An�������½��(n-s,A0,K1,A1,..,Kn-s,An-s),apָ������½��
	int i, j, n = q->keynum;
	ap = (BTNode*)malloc(sizeof(BTNode));//�����µĽ��
	ap->ptr[0] = q->ptr[s];//�����q��As����ap��A0
	for (i = s + 1, j = 1; i <= n; i++, j++) {//�����q��(Ks+1,As+1,..,Km,Am)��������ap��(K1��A1,..,Km-s,Am-s)
		ap->key[j] = q->key[i];
		ap->ptr[j] = q->ptr[i];
		ap->recptr[j] = q->recptr[i];
	}
	ap->keynum = n - s;//n���ؼ��ֵĽ�㣬ǰs��������ԭ��㣬һ��������ı���������ʣ��n-s�����½�㣻
	ap->parent = q->parent;//���ɵ��½����ԭ�����ͬһ����ֵܣ�˫����ͬ
	for (i = 0; i <= n - s; i++)//�޸�ԭ��ָ��q��q���ӽ���parent��ָ���½��ap
		if (ap->ptr[i] != NULL)ap->ptr[i]->parent = ap;
	q->keynum = s - 1;//�޸�ԭ����ָ�����
}

void InsertBTree(BTree &t, KeyType k, Recode rec,BTree q, int i) {
	//��B��t�е�q����key[i-1]��key[i]֮�����ؼ���k
	//���������Ĺؼ��ָ�������B���Ľף�������Ϊm����������˫�׽����з��ѡ�
	//B���ɼ�дΪ(n(�ؼ��ָ���),A0,K1,A1��..,Kn,An��
	KeyType x;
	Recode f;
	int s, finished = 0, needNewRoot = 0;
	BTree ap;
	if (NULL == q)newRoot(t, NULL, k, NULL,rec);
	else {
		x = k; f = rec; ap = NULL;//x��ap���漴������Ĺؼ��ֺͺ��ӽ��ָ��
		while (0 == needNewRoot && 0 == finished) {//����п����½��ı�־���߲���ɹ��ı�־���������������
			Insert(q, i, x, ap,f);//��ap�ֱ���뵽q�е�q->key[i]��q->ptr[i],
								//����ؼ����뺢�ӽ��ָ����ơ�
			if (q->keynum < m)finished = 1;//������m-1�Ϸ����ý�����־
			else {//���Ĺؼ�����������m-1������Ҫ���ѽ��
				s = (m + 1) / 2;
				split(q, s, ap);//q�����n���ؼ��֣�����Ϊ������㣬ǰһ�루s-1,A0��K1��A1��...��Ks-1��As-1��������ԭ�����,
								//��һ�루As,Ks+1,...Kn,An�������½��(n-s,A0,K1,A1,..,Kn-s,An-s),apָ������½��
				x = q->key[s];//������Ĺؼ��ֻ���Ks
				f = q->recptr[s];
				if (q->parent != NULL) {  //��˫�׿������Ų�����ȷ���µĹؼ���x���µĺ��ӽڵ�ָ��ap
					q = q->parent; i = SearchPosition(q, x);//��˫�׽���в���x�Ĳ���λ��
				}
				else needNewRoot = 1;//��˫�׹�����������һ����Ҫ�����½��ı�־
			}
		}
		if (1 == needNewRoot)//��Ҫ�����½��
			newRoot(t, q, x, ap,f);//����һ���½��t��ֻ��һ���ؼ���x����һ�����ӽ��ָ��ָ��q���ڶ������ӽ��ָ��ָ��ap
	}
}

int SearchPosition(BTree t, KeyType x) {//Ѱ��x��t���Ĳ���λ��i,����key[i-1]<x<=key[i]
	int i=1;
	if (NULL == t)return 0;
	while (i <= t->keynum) {
		if (t->key[i] >= x)return i;
		i++;
	}
	return i;
}

void BuildBTree(BTree &t) {//����B���Ĺؼ��ֺͼ�¼������B��
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
	//���ݴ�С��ϵѰ��B��t�п��Բ��뵽���²���Ľ��
	//����Ϊ�գ���r.pt��ʾ�Ĳ������ǲ���ս��
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
	//���ݴ�С��ϵѰ��B��t�е�x��λ�ã��Ҳ����򷵻ز���λ��
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
	//���ap�����ֵܽ�ؼ��֣��ؼ���˳ʱ��ѭ����
	//��Ӧ�ĵ�����B��
	if (ap == NULL||ap->parent==NULL)return;
	int i = ParentPosition(ap);//Ѱ��ap��˫��ָ������ָ��ptr[i]���±�i
	if (ap->parent->ptr[i - 1] == NULL)return;
	int k = ap->parent->ptr[i - 1]->keynum;
	if (k <= minnum || ap->keynum == m)return;//������,������������
	Insert(ap, 1, ap->parent->key[i],NULL,ap->parent->recptr[i]);//�����Ĺؼ���������
	ap->ptr[1] = ap->ptr[0];
	ap->ptr[0] = ap->parent->ptr[i - 1]->ptr[ap->parent->ptr[i - 1]->keynum];
	//���ֵܵ����һ��ָ�����
	if (NULL != ap->ptr[0])ap->ptr[0]->parent = ap;
	ap->parent->key[i] = ap->parent->ptr[i - 1]->key[ap->parent->ptr[i - 1]->keynum];//���ֵܵ�һ���ؼ������븸�ڵ�
	ap->parent->recptr[i] = ap->parent->ptr[i - 1]->recptr[ap->parent->ptr[i - 1]->keynum];
	ap->parent->ptr[i - 1]->keynum--;//���ֵ���һ��
}
int ParentPosition(BTree ap) {
	//Ѱ��ap��˫��ָ������ָ��ptr[i]���±�i
	if (NULL == ap)return -1;
	for (int i = 0; i <= ap->parent->keynum; i++) {
		if (ap->parent->ptr[i] == ap)return i;
	}
	return -1;
}

void BorrowRight(BTree &ap) {
	//���ap�����ֵܽ�ؼ��֣��ؼ���˳ʱ��ѭ����
	//��Ӧ�ĵ�����B��
	if (ap == NULL || ap->parent == NULL)return;
	int i = ParentPosition(ap);//Ѱ��ap��˫��ָ������ָ��ptr[i]���±�i
	if (ap->parent->ptr[i + 1] == NULL)return;
	int k = ap->parent->ptr[i + 1]->keynum;
	if (k <= minnum || ap->keynum == m - 1)return;//������,������������
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


void CombineLeft(BTree &ap) {//�ϲ���㵽�������ֵ���
	if (ap == NULL)return;
	if (ap->parent == NULL)return;
	int i,j;
	BTree q;
	i = ParentPosition(ap);//Ѱ��ap��˫��ָ������ָ��ptr[i]���±�i
	if (i == 0)return;
	if (ap->keynum + ap->parent->ptr[i - 1]->keynum + 1 >= m)return;
	int k = ap->parent->ptr[i - 1]->keynum;
	ap->parent->ptr[i - 1]->key[k + 1] = ap->parent->key[i];//��˫�׵�һ����Ӧ�Ĺؼ����������ֵ�
	ap->parent->ptr[i - 1]->recptr[k + 1] = ap->parent->recptr[i];
	ap->parent->ptr[i - 1]->ptr[k + 1] = ap->ptr[0];
	if(ap->ptr[0]!=NULL)ap->parent->ptr[i - 1]->ptr[k + 1]->parent = ap->parent->ptr[i - 1];
	k++;
	for (j = 1; j <= ap->keynum; j++) {//�ѽ������йؼ��ֺͺ��ӽ�㶼���Ƶ����ֵ�
		ap->parent->ptr[i - 1]->ptr[k + 1] = ap->ptr[j];
		if (ap->ptr[j] != NULL)ap->ptr[j]->parent = ap->parent->ptr[i - 1];
		ap->parent->ptr[i - 1]->key[k + 1] = ap->key[j];
		ap->parent->ptr[i - 1]->recptr[k + 1] = ap->recptr[j];
		k++;
	}
	ap->parent->ptr[i - 1]->keynum = k;
	for (j = i; j <= ap->parent->keynum; j++) {//˫�׽��ؼ�������
		ap->parent->key[j] = ap->parent->key[j + 1];
		ap->parent->recptr[j] = ap->parent->recptr[j + 1];
		ap->parent->ptr[j] = ap->parent->ptr[j + 1];
	}
	ap->parent->keynum--;
	q= ap->parent->ptr[i - 1];
	free(ap);
	ap = q;
}
void CombineRight(BTree &ap) {//�ϲ���㵽�������ֵ���
	if (ap == NULL)return;
	if (ap->parent == NULL)return;
	int i;
	BTree q;
	i = ParentPosition(ap);//Ѱ��ap��˫��ָ������ָ��ptr[i]���±�i
	if (i == ap->parent->keynum )return;
	q = ap->parent->ptr[i + 1];
	CombineLeft(q);
	ap = q;
}

void Successor(BTree &p,int &i) {//�滻�ؼ���Ϊ�����²�ֱ�Ӻ��,���ظý��λ��
	if (p == NULL)return;
	BTree q = p;
	if (p->ptr[i] != NULL)p = p->ptr[i];
	while (p->ptr[0] != NULL)p = p->ptr[0];
	q->key[i] = p->key[1];
	q->recptr[i] = p->recptr[1];
	i = 1;
}

void Remove(BTree &t, int i) {
	//���ն˽��t���Ƴ�ki
	if (t == NULL)return;
	for (int j = i; j <= t->keynum - 1; j++) {
		t->recptr[j] = t->recptr[j + 1];
		t->key[j] = t->key[j + 1];
		t->ptr[j] = t->ptr[j + 1];
	}
	t->keynum--;
}

void Restore(BTree &t) {
	//����t���(������ɾ����ĵ���)
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
	//B��p�����ɾ��iλ�õĹؼ���x
	//Ȼ�󷵻�ֹͣ����ʱ�Ľ��
	//ɾ����p��ı�λ��
	if (p->ptr[i - 1] != NULL) {//�������²���ն˽��
		Successor(p, i);//�滻�ؼ���Ϊ�����²�ֱ�Ӻ��
		DeleteKey(p, 1);//��Ϊɾ�����²���ն˽��ؼ���
	}
	else {
		Remove(p, i);//�ӽ��p���Ƴ�ki
		if (p->keynum < minnum)Restore(p);//B������
	}
}
void BTDelete(BTree &p,KeyType x) {//��B��p��ɾ��x��¼
	result r;
	SearchBTree(p, x, r);
	if (r.tag == 0)return;
	else DeleteKey(r.pt, r.i);

}
void BTInsert(BTree &p, KeyType x,Recode rec) {
	//��B��p�в���x�ؼ��ֺͼ�¼ָ��rec
	//����Ϊ�գ����½�һ����
	result r;
	SearchInsert(p, x, r);
	InsertBTree(p, x,rec,r.pt, r.i);
}

