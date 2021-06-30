#include"BTree_head.h"



Recode Head;
BTree ISBN;
Oppointment OPPO;//ԤԼ����
Borrower BORROWER;//��������


Recode newRecode() {//�����µļ�¼�ռ�
	Recode p = NULL, q;
	if (NULL != (q = (Recode)malloc(sizeof(recode))))p = q;
	return p;
}

void newBook(Recode &p) {//�Ǽ�������Ϣ
	cout << "������������ţ������ߣ�������ͣ�����ܿ��" << endl;
	cout << "��������" << endl;
	cin >> p->Title;
	cout << "�������" << endl;
	cin >> p->ISBN;
	cout << "����������" << endl;
	cin >> p->Autor;
	//	cout << "�����������" << endl;
	//	cin >> kind;
	//	Head->kind = kind;
	cout << "��������ܿ����" << endl;
	cin >> p->Total_Stock;
	p->Now_Stock = p->Total_Stock;
}


Status Print_Book(Recode p) {
	//�������ݿ��������ָ������ʾ������Ϣ
	if (p == NULL) {
		return ERROR;
	}
	if (p->Autor == NULL)return ERROR;
	cout << "ISBN��:  ";
	cout << p->ISBN << endl;
	cout << "����:  ";
	cout << p->Title << endl;
	cout << "������:  ";
	cout << p->Autor << endl;
	//	cout << "����:  ";
	//cout << p->kind << endl;
	cout << "�ܿ��:  ";
	cout << p->Total_Stock << endl;
	cout << "ʣ����Ŀ:  ";
	cout << p->Now_Stock << endl;
	Print_Borrower(p);
	printf("����������������\n");
	return SUCCESS;
}

void Books_In() {
	int i;
	Recode p;
	printf("############��0�˳�������,1����#################\n");
	while (scanf_s("%d", &i)) {
		switch (i) {
		case 0:
			return;
		case 1:
			p = newRecode();
			newBook(p);
			Stock_In(Head, p);
			Build_ISBN_Index(ISBN, p);
			printf("\n#############��1�������룬��0�˳�############\n\n"); 
		}
	}
}

void Build_ISBN_Index(BTree &t,Recode p) {
	//Ϊp��¼��B��t�Ͻ���ISBN������
	result r;
	if (t == NULL) {//��һ����������������
		newRoot(t, NULL, p->ISBN, NULL,p);
		t->recptr[1] = p;
	}
	else {//���ǵ�һ�������������
		SearchInsert(t, p->ISBN, r);
		InsertBTree(t, p->ISBN,p ,r.pt, r.i);
	}
}


void Search_Book(BTree &t,KeyType x) {//��t����������x��Ӧ�ļ�¼
	result r;
	SearchBTree(t, x, r);
	if (r.tag == 0) {
		printf("##û�ҵ�����##\n");
		printf("��������������������\n");
	}
	else Print_Book(r.pt->recptr[r.i]);
}

void Show_Database(BTree t) {//���������������Ŀ��������Ϣ
	int i;
	if (t == NULL) {
		printf("##û����##\n");
		return;
	}
	for (i = 0; i <= t->keynum; i++) {
		if (i != 0)Print_Book(t->recptr[i]);
		if(t->ptr[i]!=NULL)Show_Database(t->ptr[i]);
	}
}


Recode Random_Book() {
	int x[10];
	Recode p = newRecode();
	for (int i = 0; i <= 9; i++) {
		x[i] = rand() % 5;
	}
	strcpy_s(p->Autor, "1");
	p->ISBN = x[1];
	p->Now_Stock = x[2];
	strcpy_s(p->Title, "1");
	p->Total_Stock = x[4];
	p->Br = NULL;
	p->next = NULL;
	p->Or = NULL;
	return p;
}
void Test() {//�����������洢��ʽ��ɾ���鼰�����������鼰����
	BTree a = NULL;
	result r;
	int i;
	KeyType x,y;
	Borrower b;
	Recode rec = newRecode();
	srand((unsigned)time(NULL));
	for (i = 1; i <= 25; i++) {
		x = i;
		rec = Random_Book();
		rec->ISBN = x;//���ϳ�ʼ�����
		BTInsert(a, x, rec);//��¼ָ�����B��
		Stock_In(Head, rec);//��¼�����ݿ�
	}
	PrintBTree(a);
	Show_Database(a);
	for (i = 1; i <=25; i++) {
		b = Random_Borrower(Head);
		Lend_Register(a, b);
	}
	Show_Database(a);
	while (1) {
		printf("������Ҫ������˵�ID���鱾��ISBN��");
		scanf_s("%lld %lld", &x, &y);
		if (Borrower_Out(a, x, y) != NOFOUND)printf("����ɹ�");
		else {
			printf("�Ҳ�����");
			break;
		}
		Show_Database(a);
	}
	printf("��Ҫɾ���ı���");
	scanf_s("%lld", &x);
	Delete_Book(a, x);//��ȫɾ��ISBNΪx����
	PrintBTree(a);
	Show_Database(a);
	/*
	Recode p;
	for ( p = Head; p != NULL; p = p->next)Print_Book(p);
	for(;p!=NULL;p=p->prior)Print_Book(p);
	Show_Database(a);
	printf("��Ҫ���ı���");
	scanf_s("%lld", &x);
	Search_Book(a,x);
	*/
}

void Delete_Book(BTree &t,KeyType x) {//��ȫɾ������Ϊx����
	if (t == NULL)return;
	result r;
	Recode rec;//��ʱ����
	SearchBTree(t, x, r);//����Ҫ������B���ؼ����Լ���ָ�ļ�¼�ռ�
	if (r.tag == 0) {
		printf("##û���Ȿ�飬����ʧ��##\n");
		printf("������������������\n");
		return;
	}
	rec = r.pt->recptr[r.i];//������¼ָ��λ��
	if (rec == Head)Head = rec->next;
	DeleteKey(r.pt, r.i);//ɾ����r.pt��λ
	Remove_Recode(rec);//ɾ����¼�ռ�
	printf("##����ɹ�##\n");
	printf("������������������\n");

}

void InitLibrary() {//����ͼ��ݳ�ʼ����
	Recode rec = newRecode();
	strcpy_s(rec->Title, "���ݽṹ");
	strcpy_s(rec->Autor,"��ΰ��");
	rec->ISBN = 9787040479461;
	rec->Total_Stock = rec->Now_Stock = 5;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);//��B��
	rec = newRecode();
	strcpy_s(rec->Title, "�����߼���EDA���");
	strcpy_s(rec->Autor, "�ź�Ц");
	rec->ISBN = 9787115459343;
	rec->Total_Stock = rec->Now_Stock = 10;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "��������ԭ��");
	strcpy_s(rec->Autor, "����ɺ");
	rec->ISBN = 9787302328438;
	rec->Total_Stock = rec->Now_Stock = 12;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "��ɢ��ѧ��������");
	strcpy_s(rec->Autor, "����");
	rec->ISBN = 9787040371482;
	rec->Total_Stock = rec->Now_Stock = 6;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "������ѧ����ѧ");
	strcpy_s(rec->Autor, "������");
	rec->ISBN = 9787040494341;
	rec->Total_Stock = rec->Now_Stock = 20;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "׽è���¼�");
	strcpy_s(rec->Autor, "����������÷");
	rec->ISBN = 9787559637598;
	rec->Total_Stock = rec->Now_Stock = 5;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "��ɫ���");
	strcpy_s(rec->Autor, "�Ӵ���Ҳ");
	rec->ISBN = 9787513337816;
	rec->Total_Stock = rec->Now_Stock = 2;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "���ǵ�������������");
	strcpy_s(rec->Autor, "��ɽ�Ͱ�");
	rec->ISBN = 9787513337700;
	rec->Total_Stock = rec->Now_Stock = 4;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "�ڰ���");
	strcpy_s(rec->Autor, "�ƽ���");
	rec->ISBN = 9787532174003;
	rec->Total_Stock = rec->Now_Stock = 6;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "�ף��棬��");
	strcpy_s(rec->Autor, "���ء��Ŷ���");
	rec->ISBN = 9787559817501;
	rec->Total_Stock = rec->Now_Stock = 2;
	rec->Br = NULL;
	Stock_In(Head, rec);//���
	BTInsert(ISBN, rec->ISBN, rec);
}


void Business_1() {//���
	int i;
	result r;
	char Title[20];
	long long ISBN_;
	char Autor[20];
	int Quantity;
	Recode p;
	while (1) {
		p = newRecode();
		printf("���������������Ϣ\n");
		printf("��������������\n");
		printf("����: "); cin>>Title;
		printf("����: "); cin>>Autor;
		printf("����: "); scanf_s("%d", &Quantity);
		printf("ISBN: ");  scanf_s("%lld", &ISBN_);
		printf("��������������\n");
		SearchBTree(ISBN, ISBN_, r);
		if (r.tag == 1) {//��������
			printf("##�ɹ������鱾����##\n");
			printf("��������������\n");
			r.pt->recptr[r.i]->Total_Stock += Quantity;
			r.pt->recptr[r.i]->Now_Stock += Quantity;

		}
		else {//����û��
			strcpy_s(p->Autor, Autor);
			strcpy_s(p->Title, Title);
			p->ISBN = ISBN_;
			p->Total_Stock = Quantity;
			p->Br = NULL;
			p->Or = NULL;
			p->Now_Stock = Quantity;
			Stock_In(Head, p);//���
			printf("##�������ɹ�##\n");
			printf("��������������\n");
			BTInsert(ISBN, p->ISBN, p);//��B��
		}
		printf("1.�������� 0.�ص�������\n");
		printf("��������������\n");
		scanf_s("%d", &i);
		printf("��������������\n");
		switch (i){
		case 1: break;
		default: return;
		}
	}
}


void Business_2() {//����
	int i;
	Borrower p;
	Oppointment q;
	while (1) {
		printf("1:��ԤԼ 2:��ԤԼ 0:�ص�������\n");
		printf("��������������������\n");
		scanf_s("%d", &i);
		printf("��������������������\n");
		switch (i){
		case 1:
			while (1) {
				p = new_Borrower();//��ʼ��������
				printf("����������: ");
				cin >> p->Name;
				printf("֤��ID: ");
				scanf_s("%lld", &p->ID_Number);
				printf("Ҫ���鼮ISBN: ");
				scanf_s("%lld", &p->ISBN);
				printf("����黹����: ");
				scanf_s("%d", &p->Deadline);
				printf("��������������������\n");
				i = Lend_Register(ISBN, p);
				if (i == NOFOUND)printf("##�鱾�Ѳ���##\n");
				else if (i == SUCCESS)printf("##����ɹ�##\n");
				else if (i == ERROR)printf("##�鱾��������##\n");
				printf("��������������������\n");
				printf("1:�����Ǽǽ��� 0:�ص�������\n");
				printf("��������������������\n");
				scanf_s("%d", &i);
				printf("��������������������\n");
				switch (i) {
				case 1: break;
				default: return;
				}
			}
		case 2:
			while (1) {
				q = new_Oppointment();//��ʼ��ԤԼ�������
				printf("ԤԼ������: ");
				cin >> q->Name;
				printf("ԤԼ��ID: ");
				scanf_s("%lld", &q->ID_Number);
				printf("ԤԼ�鼮ISBN: ");
				scanf_s("%lld", &q->ISBN);
				printf("��ֹ����: ");
				scanf_s("%d", &q->Oppointment_time);
				printf("��������������������\n");
				i = Oppointment_Come(ISBN, q, OPPO);
				if (i == NOFOUND)printf("##�Ҳ���ԤԼ��¼##\n");
				else if (i == ERROR)printf("##�����Ѿ�������##\n##�Ѱ���ɾ��ԤԼ��¼##\n");
				else if (i == SUCCESS)printf("##����ɹ�##\n");
				printf("��������������������\n");
				printf("1:��һ��ԤԼ�� 0:�ص�������\n");
				printf("��������������������\n");
				scanf_s("%d", &i);
				printf("��������������������\n");
				switch (i) {
				case 1: break;
				default: return;
				}
			}
		default:return;
		}
	}
}


void Business_3() {//����
	long long ISBN_;
	long long ID;
	int i;
	int tag;
	while (1) {
		printf("�鼮ISBN: ");
		scanf_s("%lld", &ISBN_);
		printf("������ID: ");
		scanf_s("%lld", &ID);
		printf("��������������������\n");
		tag = Borrower_Out(ISBN, ID, ISBN_);
		if (tag == SUCCESS)printf("##����ɹ�##\n");
		else if (tag == ERROR)printf("##�Ҳ�����##\n");
		else if (tag == NOFOUND)printf("##û�иý����¼##\n");
		printf("��������������������\n");
		printf("1:�������� 0:�ص�������\n");
		printf("��������������������\n");
		scanf_s("%d", &i);
		printf("��������������������\n");
		switch (i) {
		case 1: break;
		default: return;
		}
	}

}
void Business_4() {//����鼮
	long long ISBN_;
	int i;
	while (1) {
		printf("����鼮ISBN: ");
		scanf_s("%lld", &ISBN_);
		printf("��������������������\n");
		Delete_Book(ISBN, ISBN_);//��ȫɾ������ISBN����
		printf("1:����ɾ�� 0:�ص�������\n");
		printf("��������������������\n");
		scanf_s("%d", &i);
		printf("����������������\n");
		switch (i){
		case 1: break;
		default: return; 
		}
	}

}
void Business_5() {//��ʾ
	int i;
	while (1) {
		printf("1:���B�� 2:ISBN˳����Ŀ \n3:����������� 0:�ص�������\n");
		printf("��������������������\n");
		scanf_s("%d", &i);
		printf("��������������������\n");
		switch (i) {
		case 1:
			printf("B���ṹ:\n");
			printf("����������������\n");
			PrintBTree(ISBN);
			printf("����������������\n");
			break;
		case 2:
			printf("��ISBN˳����Ŀ:\n");
			printf("����������������\n");
			Show_Database(ISBN);
			break;
		case 3:
			printf("�������\n");
			printf("����������������\n");
			for (Recode p = Head; p != NULL; p = p->next) {
				if (ERROR == Print_Book(p)) {
					printf("##û���鼮##\n");
					printf("����������������\n");
				}
			}
			break;
		default:return;
		}
	}
}
void Business_6() {//��ѯ�鼮
	long long ISBN_;
	int i;
	while (1) {
		printf("��ѯ�鼮ISBN: ");
		scanf_s("%lld", &ISBN_);
		printf("��������������������\n");
		Search_Book(ISBN, ISBN_);//����ISBN�ϲ�ѯ�鼮.....������Ҫ������߹���
		printf("1:������ѯ 0:�ص���ҳ��\n");
		printf("��������������������\n");
		scanf_s("%d", &i);
		printf("��������������������\n");
		switch (i){
		case 1: break;
		default: return;
		}
	}
}

void Business_7() {//ԤԼ����
	int i;
	Oppointment p;
	while (1) {
		p = new_Oppointment();//��ʼ��������
		printf("ԤԼ������: ");
		cin >> p->Name;
		printf("ԤԼ��ID: ");
		scanf_s("%lld", &p->ID_Number);
		printf("ԤԼ�鼮ISBN: ");
		scanf_s("%lld", &p->ISBN);
		printf("ԤԼʱ��: ");
		scanf_s("%d", &p->Oppointment_time);
		printf("��������������������\n");
		i = Oppoint_Register(ISBN,OPPO, p);
		if (i == NOFOUND)printf("##�Ҳ����Ȿ��##\n");
		else if (i == SUCCESS)printf("##ԤԼ�ɹ�##\n");
		else if (i == ERROR)printf("##�鱾��������##\n");
		printf("��������������������\n");
		printf("1:�����Ǽ�ԤԼ 0:�ص�������\n");
		printf("��������������������\n");
		scanf_s("%d", &i);
		printf("��������������������\n");
		switch (i) {
		case 1: break;
		default: return;
		}
	}
}

void Business_8() {//���ʱ����
	int i;
	Oppointment p;
	while (1) {
		printf("1:ԤԼ��Ϣ 2:������Ϣ 0:�ص�������\n");
		printf("��������������������\n");
		scanf_s("%d", &i);
		printf("��������������������\n");
		switch (i) {
		case 1: Print_Oppointment_Link(OPPO); break;
		case 2: Print_Borrower_Link(BORROWER); break;
		default: return;
		}
	}
}

void Business_9() {
	
}