#include"BTree_head.h"
//˫������

void Stock_In(Recode &Head, Recode p) {//p��⵽�����ͷ,����������
	if (p == NULL) {
		printf("û��Ҫ�������");
		return;
	}
	else if (Head == NULL) {
		Head = p;
		Head->next = NULL;
		Head->prior = NULL;
	}
	else if (Head == p)return;
	//ͬһ����¼����û���κβ���,����ʵ�ֵ�һ�����ļ�¼
	else {
		p->next = Head;
		p->prior = NULL;
		Head->prior = p;
		Head = p;
	}

}

void Remove_Recode(Recode &p) {	//�Ƴ�p��¼,������������
	int s;//�����־
	if (p == NULL)printf("û�п����Ƴ��ļ�¼");
	else if (p->prior == NULL) {//ͷ��¼
		if (p->next != NULL)p->next->prior = NULL;
		free(p);
		p = NULL;
	}
	else if (p->next == NULL) {//β��¼
		p->prior->next = NULL;
		free(p);
		p = NULL;
	}
	else {//�м��¼
		p->prior->next = p->next;
		p->next->prior = p->prior;
		free(p);
		p = NULL;
	}
}



Status Lend_Register(BTree &t,Borrower b){
	//�����߰������������t����ָ��ļ�¼p��������У���ʱ��˳�����rec(������)
	//�޸ĸ�����������
	result r;
	Borrower p = Borrower_Copy(b);
	SearchBTree(t, b->ISBN, r);
	if (r.tag == 0)return NOFOUND;
	else if (r.pt->recptr[r.i]->Now_Stock == 0)return ERROR;
	else {
		Borrower_In_Link(BORROWER, b);//��ʱ���
		Borrower_In(r.pt->recptr[r.i], p);//�ڼ�¼��ʱ��˳�������������
		r.pt->recptr[r.i]->Now_Stock--;//����-1
		return SUCCESS;
	}
}

void Borrower_In_Link(Borrower &g, Borrower b) {//������p�а�ʱ��˳�����b
	Borrower p = g;
	if (p == NULL) {//�޽����ˣ�������˲����ڵ�һ��
		b->next = NULL;
		b->prior = NULL;
		g = b;//��¼�Ľ�����ָ��ָ��b
		return;
	}
	for (; p != NULL; p = p->next) {//�����˲����ں��ʵ�λ��
		if (p->Deadline > b->Deadline) {//b������p��ǰ��
			b->prior = p->prior;
			b->next = p;//��b����
			if (p->prior != NULL){
				b->prior->next = b;
			}
			else g = b;
			p->prior = b;//��b��ǰ����������
			return;
		}
		if (p->next == NULL) {
			b->next = NULL;
			b->prior = p;
			p->next = b;
			return;
		}////���κ��˽�ֹ���ڶ���b����q��
	}
}

void Borrower_In(Recode &rec, Borrower b) {//��rec��¼��ʱ��˳�����ӽ�����
	if (b == NULL || rec == NULL)return;
	Borrower p;
	p= rec->Br;
	if (p == NULL) {//�޽����ˣ�������˲����ڵ�һ��
		b->next = NULL;
		b->prior = NULL;
		rec->Br = b;//��¼�Ľ�����ָ��ָ��b
		return;
	}
	for (; p != NULL; p = p->next) {//�����˲����ں��ʵ�λ��
		if (p->Deadline > b->Deadline) {//b������p��ǰ��
			b->prior = p->prior;
			b->next = p;//��b����
			if (p->prior != NULL)b->prior->next = b;
			else rec->Br = b;
			p->prior = b;//��b��ǰ����������
			return;
		}
		if (p->next == NULL) {
			b->next = NULL;
			b->prior = p;
			p->next = b;
			return;
		}////���κ��˽�ֹ���ڶ���b����q��
	}
}



Status Print_Borrower(Recode rec){//����Ȿ���Ľ�����(ʱ��˳��)
	if (rec == NULL)return NOFOUND;
	else if (rec->Br == NULL)return NOFOUND;
	Borrower p = rec->Br;
	printf("\n");
	for (; p != NULL; p = p->next) {
		//cout << "��ISBN: ";
		//cout << p->ISBN << endl;
		cout << "������: ";
		cout << p->Name << endl;
		cout << "����֤��: ";
		cout << p->ID_Number << endl;
		cout << "�����ֹ����: ";
		cout << p->Deadline << endl;
		if(p->next!=NULL)printf("\n");

	}
	return SUCCESS;
}

Borrower Random_Borrower(Recode rec) {//�Ӽ�¼����������ɽ����˲�����ָ��
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

Borrower new_Borrower() {//�½�һ�������˽��ָ�벢����
	Borrower b = NULL,q;
	if (NULL != (q = (Borrower)malloc(sizeof(BorrowerNode))))b = q;
	return b;
}

Recode Recode_Position(BTree t,long long index) {//��������ֵ�����ش����������λ��
	if (t == NULL)return ERROR;
	result r;
	Recode rec;//��ʱ����
	SearchBTree(t, index, r);//����Ҫ������B���ؼ����Լ���ָ�ļ�¼�ռ�
	if (r.tag == 0) {
		return NULL;
	}
	else return r.pt->recptr[r.i];
}

Status Borrower_Out(BTree t,long long ID_Number, long long ISBN) {//������źͽ����������
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

Status Borrower_Out_Link(Borrower &head, long long ID_Number, long long ISBN_) {//�����߳�ʱ����
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

Oppointment new_Oppointment() {//�½�һ�������˽��ָ�벢����
	Oppointment b = NULL, q;
	if (NULL != (q = (Oppointment)malloc(sizeof(OppointmentNode))))b = q;
	return b;
}

Status Oppointment_In_Link(Oppointment &t, Oppointment p) {//��ԤԼʱ��������ԤԼ��p
	Oppointment g;
	if (p == NULL)return ERROR;
	if (t == NULL) {
		p->next = NULL;
		p->prior = NULL;
		t = p;
		return SUCCESS;
	}
	g = t;
	for (; g != NULL; g = g->next) {//�����˲����ں��ʵ�λ��
		if (g->Oppointment_time > p->Oppointment_time) {//b������p��ǰ��
			p->prior = g->prior;
			p->next = g;//��b����
			if (g->prior != NULL) {
				p->prior->next = p;
			}
			else t = p;
			g->prior = p;//��b��ǰ����������
			return SUCCESS;
		}
		if (g->next == NULL) {
			p->next = NULL;
			p->prior = g;
			g->next = p;
			return SUCCESS;
		}////���κ��˽�ֹ���ڶ���b����q��
	}
}

Status Oppoint_Register(BTree t, Oppointment &head, Oppointment p) {
	//�жϣ�֮��ԤԼ�߰�ʱ��˳��Ǽǵ�ԤԼʱ����
	if (p == NULL)return ERROR;
	result r;
	SearchBTree(t, p->ISBN, r);
	if (r.tag == 0)return NOFOUND;
	else if (r.pt->recptr[r.i]->Now_Stock == 0)return ERROR;
	else {
		Oppointment_In_Link(head, p);//��ԤԼʱ��������ԤԼ��
		r.pt->recptr[r.i]->Now_Stock--;//����-1
		return SUCCESS;
	}
}

void Print_Oppointment_Link(Oppointment p) {//���ԤԼ��
	for (; p != NULL; p = p->next) {
		cout << "ISBN: ";
		cout << p->ISBN << endl;
		cout << "ԤԼ��: ";
		cout << p->Name << endl;
		cout << "����֤��: ";
		cout << p->ID_Number << endl;
		cout << "ԤԼ����: ";
		cout << p->Oppointment_time << endl;
		printf("��������������������\n");
	}
}


void Print_Borrower_Link(Borrower p) {//�����������
	for (; p != NULL; p = p->next) {
		cout << "ISBN: ";
		cout << p->ISBN << endl;
		cout << "������: ";
		cout << p->Name << endl;
		cout << "����֤��: ";
		cout << p->ID_Number << endl;
		cout << "�����ֹ����: ";
		cout << p->Deadline << endl;
		printf("��������������������\n");
	}
}


Status Oppointment_Out_Link(Oppointment &t,long long ID_Number, long long ISBN) {//ԤԼ�߳�����
	for (Oppointment p = t; p != NULL; p = p->next) {
		if (p->ID_Number == ID_Number && p->ISBN == ISBN) {
			if (p->prior != NULL)p->prior->next = p->next;
			else t = p->next;
			if (NULL != p->next)p->next->prior = p->prior;
			return SUCCESS;
		}
	}
	return NOFOUND;//�Ҳ���ԤԼ��
}

Borrower Oppointment_To_Borrower(Oppointment p) {//��ԤԼ�߱������
	Borrower b = new_Borrower();
	b->ISBN = p->ISBN;
	b->ID_Number = p->ID_Number;
	b->Deadline = p->Oppointment_time;
	strcpy_s(b->Name, p->Name);
	b->next = NULL;
	b->prior = NULL;
	return b;
}

Status Oppointment_Come(BTree &t,Oppointment op, Oppointment &head) {//ԤԼ�߶���ԤԼ
	result r;
	if (NOFOUND == Oppointment_Out_Link(head, op->ID_Number, op->ISBN))return NOFOUND;
	Borrower p = Oppointment_To_Borrower(op);
	SearchBTree(t, p->ISBN, r);
	r.pt->recptr[r.i]->Now_Stock++;
	if(NOFOUND==Lend_Register(t, p))return ERROR;
	return SUCCESS;
}

Borrower Borrower_Copy(Borrower b) {//����b�����ˣ�����ָ��
	Borrower p = new_Borrower();
	p->ISBN = b->ISBN;
	p->Deadline = b->Deadline;
	p->ID_Number = b->ID_Number;
	strcpy_s(p->Name, b->Name);
	p->next = p->prior = NULL;
	return p;
}

//ɾ���ͻ����Ժ�������ı䶯