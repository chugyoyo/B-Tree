#include"BTree_head.h"
//双向链表

void Stock_In(Recode &Head, Recode p) {//p入库到储存库头,不建立索引
	if (p == NULL) {
		printf("没有要入库数据");
		return;
	}
	else if (Head == NULL) {
		Head = p;
		Head->next = NULL;
		Head->prior = NULL;
	}
	else if (Head == p)return;
	//同一个记录，则没有任何操作,用来实现第一个入库的记录
	else {
		p->next = Head;
		p->prior = NULL;
		Head->prior = p;
		Head = p;
	}

}

void Remove_Recode(Recode &p) {	//移除p记录,索引树不操作
	int s;//情况标志
	if (p == NULL)printf("没有可以移除的记录");
	else if (p->prior == NULL) {//头记录
		if (p->next != NULL)p->next->prior = NULL;
		free(p);
		p = NULL;
	}
	else if (p->next == NULL) {//尾记录
		p->prior->next = NULL;
		free(p);
		p = NULL;
	}
	else {//中间记录
		p->prior->next = p->next;
		p->next->prior = p->prior;
		free(p);
		p = NULL;
	}
}



Status Lend_Register(BTree &t,Borrower b){
	//借书者按书的索引，在t索引指向的记录p后的链表中，按时间顺序插入rec(借书者)
	//修改该书的相关内容
	result r;
	Borrower p = Borrower_Copy(b);
	SearchBTree(t, b->ISBN, r);
	if (r.tag == 0)return NOFOUND;
	else if (r.pt->recptr[r.i]->Now_Stock == 0)return ERROR;
	else {
		Borrower_In_Link(BORROWER, b);//入时间表
		Borrower_In(r.pt->recptr[r.i], p);//在记录后按时间顺序链接入借书人
		r.pt->recptr[r.i]->Now_Stock--;//书库存-1
		return SUCCESS;
	}
}

void Borrower_In_Link(Borrower &g, Borrower b) {//在链表p中按时间顺序插入b
	Borrower p = g;
	if (p == NULL) {//无借书人，则借书人插入在第一个
		b->next = NULL;
		b->prior = NULL;
		g = b;//记录的借书人指针指向b
		return;
	}
	for (; p != NULL; p = p->next) {//借书人插入在合适的位置
		if (p->Deadline > b->Deadline) {//b插入在p的前方
			b->prior = p->prior;
			b->next = p;//对b操作
			if (p->prior != NULL){
				b->prior->next = b;
			}
			else g = b;
			p->prior = b;//对b的前后两个操作
			return;
		}
		if (p->next == NULL) {
			b->next = NULL;
			b->prior = p;
			p->next = b;
			return;
		}////比任何人截止日期都晚，b插入q后
	}
}

void Borrower_In(Recode &rec, Borrower b) {//在rec记录后按时间顺序链接借书者
	if (b == NULL || rec == NULL)return;
	Borrower p;
	p= rec->Br;
	if (p == NULL) {//无借书人，则借书人插入在第一个
		b->next = NULL;
		b->prior = NULL;
		rec->Br = b;//记录的借书人指针指向b
		return;
	}
	for (; p != NULL; p = p->next) {//借书人插入在合适的位置
		if (p->Deadline > b->Deadline) {//b插入在p的前方
			b->prior = p->prior;
			b->next = p;//对b操作
			if (p->prior != NULL)b->prior->next = b;
			else rec->Br = b;
			p->prior = b;//对b的前后两个操作
			return;
		}
		if (p->next == NULL) {
			b->next = NULL;
			b->prior = p;
			p->next = b;
			return;
		}////比任何人截止日期都晚，b插入q后
	}
}



Status Print_Borrower(Recode rec){//输出这本书后的借书者(时间顺序)
	if (rec == NULL)return NOFOUND;
	else if (rec->Br == NULL)return NOFOUND;
	Borrower p = rec->Br;
	printf("\n");
	for (; p != NULL; p = p->next) {
		//cout << "书ISBN: ";
		//cout << p->ISBN << endl;
		cout << "借书人: ";
		cout << p->Name << endl;
		cout << "借书证号: ";
		cout << p->ID_Number << endl;
		cout << "还书截止日期: ";
		cout << p->Deadline << endl;
		if(p->next!=NULL)printf("\n");

	}
	return SUCCESS;
}

Borrower Random_Borrower(Recode rec) {//从记录库中随机生成借书人并返回指针
	Borrower b = new_Borrower();
	b->ID_Number = rand() % 100000000000000000;
	b->Deadline = rand()%1312423324;
	int i = rand() % 5, j = 0;
	strcpy_s(b->Name, "1");
	for (; rec != NULL; rec = rec->next) {
		if (j == i) {
			b->ISBN = rec->ISBN;
			break;
		}
		j++;
	}
	return b;
}

Borrower new_Borrower() {//新建一个借书人结点指针并返回
	Borrower b = NULL,q;
	if (NULL != (q = (Borrower)malloc(sizeof(BorrowerNode))))b = q;
	return b;
}

Recode Recode_Position(BTree t,long long index) {//根据索引值来返回待操作的书的位置
	if (t == NULL)return ERROR;
	result r;
	Recode rec;//临时变量
	SearchBTree(t, index, r);//搜索要操作的B树关键字以及所指的记录空间
	if (r.tag == 0) {
		return NULL;
	}
	else return r.pt->recptr[r.i];
}

Status Borrower_Out(BTree t,long long ID_Number, long long ISBN) {//根据书号和借书号来还书
	int i;
	Recode p = Recode_Position(t, ISBN);
	if (p == NULL) {
		if(Borrower_Out_Link(BORROWER,ID_Number,ISBN)==SUCCESS)return SUCCESS;
		else return ERROR;
	}
	for (Borrower b = p->Br; b != NULL; b = b->next) {
		if (b->ID_Number == ID_Number) {
			if (b->prior != NULL)b->prior->next = b->next;
			else p->Br = b->next;
			if (NULL != b->next)b->next->prior = b->prior;
			Borrower_Out_Link(BORROWER, b->ID_Number, b->ISBN);
			p->Now_Stock++;
			return SUCCESS;
		}
	}
	return NOFOUND;
}

Status Borrower_Out_Link(Borrower &head, long long ID_Number, long long ISBN_) {//借书者出时间链
	if (head == NULL)return ERROR;
	Borrower b = head;
	for (; b != NULL; b = b->next) {
		if (b->ID_Number == ID_Number && b->ISBN == ISBN_) {
			if (b->prior != NULL)b->prior->next = b->next;
			else head = b->next;
			if (NULL != b->next)b->next->prior = b->prior;
			return SUCCESS;
		}
	}
	return ERROR;
}

Oppointment new_Oppointment() {//新建一个借书人结点指针并返回
	Oppointment b = NULL, q;
	if (NULL != (q = (Oppointment)malloc(sizeof(OppointmentNode))))b = q;
	return b;
}

Status Oppointment_In_Link(Oppointment &t, Oppointment p) {//在预约时间链接入预约者p
	Oppointment g;
	if (p == NULL)return ERROR;
	if (t == NULL) {
		p->next = NULL;
		p->prior = NULL;
		t = p;
		return SUCCESS;
	}
	g = t;
	for (; g != NULL; g = g->next) {//借书人插入在合适的位置
		if (g->Oppointment_time > p->Oppointment_time) {//b插入在p的前方
			p->prior = g->prior;
			p->next = g;//对b操作
			if (g->prior != NULL) {
				p->prior->next = p;
			}
			else t = p;
			g->prior = p;//对b的前后两个操作
			return SUCCESS;
		}
		if (g->next == NULL) {
			p->next = NULL;
			p->prior = g;
			g->next = p;
			return SUCCESS;
		}////比任何人截止日期都晚，b插入q后
	}
}

Status Oppoint_Register(BTree t, Oppointment &head, Oppointment p) {
	//判断，之后将预约者按时间顺序登记到预约时间链
	if (p == NULL)return ERROR;
	result r;
	SearchBTree(t, p->ISBN, r);
	if (r.tag == 0)return NOFOUND;
	else if (r.pt->recptr[r.i]->Now_Stock == 0)return ERROR;
	else {
		Oppointment_In_Link(head, p);//在预约时间链接入预约者
		r.pt->recptr[r.i]->Now_Stock--;//书库存-1
		return SUCCESS;
	}
}

void Print_Oppointment_Link(Oppointment p) {//输出预约者
	for (; p != NULL; p = p->next) {
		cout << "ISBN: ";
		cout << p->ISBN << endl;
		cout << "预约者: ";
		cout << p->Name << endl;
		cout << "借书证号: ";
		cout << p->ID_Number << endl;
		cout << "预约日期: ";
		cout << p->Oppointment_time << endl;
		printf("——————————\n");
	}
}


void Print_Borrower_Link(Borrower p) {//输出借书者链
	for (; p != NULL; p = p->next) {
		cout << "ISBN: ";
		cout << p->ISBN << endl;
		cout << "借书者: ";
		cout << p->Name << endl;
		cout << "借书证号: ";
		cout << p->ID_Number << endl;
		cout << "还书截止日期: ";
		cout << p->Deadline << endl;
		printf("——————————\n");
	}
}


Status Oppointment_Out_Link(Oppointment &t,long long ID_Number, long long ISBN) {//预约者出链表
	for (Oppointment p = t; p != NULL; p = p->next) {
		if (p->ID_Number == ID_Number && p->ISBN == ISBN) {
			if (p->prior != NULL)p->prior->next = p->next;
			else t = p->next;
			if (NULL != p->next)p->next->prior = p->prior;
			return SUCCESS;
		}
	}
	return NOFOUND;//找不到预约者
}

Borrower Oppointment_To_Borrower(Oppointment p) {//由预约者变借书者
	Borrower b = new_Borrower();
	b->ISBN = p->ISBN;
	b->ID_Number = p->ID_Number;
	b->Deadline = p->Oppointment_time;
	strcpy_s(b->Name, p->Name);
	b->next = NULL;
	b->prior = NULL;
	return b;
}

Status Oppointment_Come(BTree &t,Oppointment op, Oppointment &head) {//预约者兑现预约
	result r;
	if (NOFOUND == Oppointment_Out_Link(head, op->ID_Number, op->ISBN))return NOFOUND;
	Borrower p = Oppointment_To_Borrower(op);
	SearchBTree(t, p->ISBN, r);
	r.pt->recptr[r.i]->Now_Stock++;
	if(NOFOUND==Lend_Register(t, p))return ERROR;
	return SUCCESS;
}

Borrower Borrower_Copy(Borrower b) {//复制b借书人，返回指针
	Borrower p = new_Borrower();
	p->ISBN = b->ISBN;
	p->Deadline = b->Deadline;
	p->ID_Number = b->ID_Number;
	strcpy_s(p->Name, b->Name);
	p->next = p->prior = NULL;
	return p;
}

//删除和还书以后借书链的变动