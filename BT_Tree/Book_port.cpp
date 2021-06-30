#include"BTree_head.h"



Recode Head;
BTree ISBN;
Oppointment OPPO;//预约者链
Borrower BORROWER;//借书者链


Recode newRecode() {//生成新的记录空间
	Recode p = NULL, q;
	if (NULL != (q = (Recode)malloc(sizeof(recode))))p = q;
	return p;
}

void newBook(Recode &p) {//登记新书信息
	cout << "输入书名，书号，书作者，书的类型，书的总库存" << endl;
	cout << "输入书名" << endl;
	cin >> p->Title;
	cout << "输入书号" << endl;
	cin >> p->ISBN;
	cout << "输入书作者" << endl;
	cin >> p->Autor;
	//	cout << "输入书的类型" << endl;
	//	cin >> kind;
	//	Head->kind = kind;
	cout << "输入书的总库存量" << endl;
	cin >> p->Total_Stock;
	p->Now_Stock = p->Total_Stock;
}


Status Print_Book(Recode p) {
	//输入数据库这种书的指针来显示该书信息
	if (p == NULL) {
		return ERROR;
	}
	if (p->Autor == NULL)return ERROR;
	cout << "ISBN码:  ";
	cout << p->ISBN << endl;
	cout << "书名:  ";
	cout << p->Title << endl;
	cout << "作者名:  ";
	cout << p->Autor << endl;
	//	cout << "类型:  ";
	//cout << p->kind << endl;
	cout << "总库存:  ";
	cout << p->Total_Stock << endl;
	cout << "剩余书目:  ";
	cout << p->Now_Stock << endl;
	Print_Borrower(p);
	printf("————————\n");
	return SUCCESS;
}

void Books_In() {
	int i;
	Recode p;
	printf("############按0退出主界面,1输入#################\n");
	while (scanf_s("%d", &i)) {
		switch (i) {
		case 0:
			return;
		case 1:
			p = newRecode();
			newBook(p);
			Stock_In(Head, p);
			Build_ISBN_Index(ISBN, p);
			printf("\n#############按1继续输入，按0退出############\n\n"); 
		}
	}
}

void Build_ISBN_Index(BTree &t,Recode p) {
	//为p记录在B树t上建立ISBN码索引
	result r;
	if (t == NULL) {//第一个索引，开辟新树
		newRoot(t, NULL, p->ISBN, NULL,p);
		t->recptr[1] = p;
	}
	else {//不是第一个索引，则插入
		SearchInsert(t, p->ISBN, r);
		InsertBTree(t, p->ISBN,p ,r.pt, r.i);
	}
}


void Search_Book(BTree &t,KeyType x) {//在t索引树上找x对应的记录
	result r;
	SearchBTree(t, x, r);
	if (r.tag == 0) {
		printf("##没找到该书##\n");
		printf("——————————\n");
	}
	else Print_Book(r.pt->recptr[r.i]);
}

void Show_Database(BTree t) {//按索引输出所有书目的所有信息
	int i;
	if (t == NULL) {
		printf("##没有书##\n");
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
void Test() {//测试索引，存储方式，删除书及索引，插入书及索引
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
		rec->ISBN = x;//以上初始化书库
		BTInsert(a, x, rec);//记录指针插入B树
		Stock_In(Head, rec);//记录入数据库
	}
	PrintBTree(a);
	Show_Database(a);
	for (i = 1; i <=25; i++) {
		b = Random_Borrower(Head);
		Lend_Register(a, b);
	}
	Show_Database(a);
	while (1) {
		printf("请输入要还书的人的ID及书本的ISBN？");
		scanf_s("%lld %lld", &x, &y);
		if (Borrower_Out(a, x, y) != NOFOUND)printf("还书成功");
		else {
			printf("找不到书");
			break;
		}
		Show_Database(a);
	}
	printf("你要删除哪本？");
	scanf_s("%lld", &x);
	Delete_Book(a, x);//完全删除ISBN为x的书
	PrintBTree(a);
	Show_Database(a);
	/*
	Recode p;
	for ( p = Head; p != NULL; p = p->next)Print_Book(p);
	for(;p!=NULL;p=p->prior)Print_Book(p);
	Show_Database(a);
	printf("你要搜哪本？");
	scanf_s("%lld", &x);
	Search_Book(a,x);
	*/
}

void Delete_Book(BTree &t,KeyType x) {//完全删除索引为x的书
	if (t == NULL)return;
	result r;
	Recode rec;//临时变量
	SearchBTree(t, x, r);//搜索要操作的B树关键字以及所指的记录空间
	if (r.tag == 0) {
		printf("##没有这本书，清书失败##\n");
		printf("—————————\n");
		return;
	}
	rec = r.pt->recptr[r.i];//保留记录指针位置
	if (rec == Head)Head = rec->next;
	DeleteKey(r.pt, r.i);//删除后r.pt移位
	Remove_Recode(rec);//删除记录空间
	printf("##清书成功##\n");
	printf("—————————\n");

}

void InitLibrary() {//给予图书馆初始数据
	Recode rec = newRecode();
	strcpy_s(rec->Title, "数据结构");
	strcpy_s(rec->Autor,"吴伟民");
	rec->ISBN = 9787040479461;
	rec->Total_Stock = rec->Now_Stock = 5;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);//入B树
	rec = newRecode();
	strcpy_s(rec->Title, "数字逻辑与EDA设计");
	strcpy_s(rec->Autor, "张海笑");
	rec->ISBN = 9787115459343;
	rec->Total_Stock = rec->Now_Stock = 10;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "计算机组成原理");
	strcpy_s(rec->Autor, "蒋本珊");
	rec->ISBN = 9787302328438;
	rec->Total_Stock = rec->Now_Stock = 12;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "离散数学及其运用");
	strcpy_s(rec->Autor, "傅彦");
	rec->ISBN = 9787040371482;
	rec->Total_Stock = rec->Now_Stock = 6;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "简明大学物理学");
	strcpy_s(rec->Autor, "范仰才");
	rec->ISBN = 9787040494341;
	rec->Total_Stock = rec->Now_Stock = 20;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "捉猫故事集");
	strcpy_s(rec->Autor, "马塞尔·艾梅");
	rec->ISBN = 9787559637598;
	rec->Total_Stock = rec->Now_Stock = 5;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "黑色天鹅");
	strcpy_s(rec->Autor, "鲇川哲也");
	rec->ISBN = 9787513337816;
	rec->Total_Stock = rec->Now_Stock = 2;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "我们盗走星座的理由");
	strcpy_s(rec->Autor, "北山猛邦");
	rec->ISBN = 9787513337700;
	rec->Total_Stock = rec->Now_Stock = 4;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "乌暗暝");
	strcpy_s(rec->Autor, "黄锦树");
	rec->ISBN = 9787532174003;
	rec->Total_Stock = rec->Now_Stock = 6;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);
	rec = newRecode();
	strcpy_s(rec->Title, "米，面，鱼");
	strcpy_s(rec->Autor, "马特·古尔丁");
	rec->ISBN = 9787559817501;
	rec->Total_Stock = rec->Now_Stock = 2;
	rec->Br = NULL;
	Stock_In(Head, rec);//入库
	BTInsert(ISBN, rec->ISBN, rec);
}


void Business_1() {//入库
	int i;
	result r;
	char Title[20];
	long long ISBN_;
	char Autor[20];
	int Quantity;
	Recode p;
	while (1) {
		p = newRecode();
		printf("请依次输入书的信息\n");
		printf("———————\n");
		printf("书名: "); cin>>Title;
		printf("作者: "); cin>>Autor;
		printf("数量: "); scanf_s("%d", &Quantity);
		printf("ISBN: ");  scanf_s("%lld", &ISBN_);
		printf("———————\n");
		SearchBTree(ISBN, ISBN_, r);
		if (r.tag == 1) {//库中已有
			printf("##成功增加书本数量##\n");
			printf("———————\n");
			r.pt->recptr[r.i]->Total_Stock += Quantity;
			r.pt->recptr[r.i]->Now_Stock += Quantity;

		}
		else {//库中没有
			strcpy_s(p->Autor, Autor);
			strcpy_s(p->Title, Title);
			p->ISBN = ISBN_;
			p->Total_Stock = Quantity;
			p->Br = NULL;
			p->Or = NULL;
			p->Now_Stock = Quantity;
			Stock_In(Head, p);//入库
			printf("##添加新书成功##\n");
			printf("———————\n");
			BTInsert(ISBN, p->ISBN, p);//入B树
		}
		printf("1.继续输入 0.回到主界面\n");
		printf("———————\n");
		scanf_s("%d", &i);
		printf("———————\n");
		switch (i){
		case 1: break;
		default: return;
		}
	}
}


void Business_2() {//借书
	int i;
	Borrower p;
	Oppointment q;
	while (1) {
		printf("1:无预约 2:有预约 0:回到主界面\n");
		printf("——————————\n");
		scanf_s("%d", &i);
		printf("——————————\n");
		switch (i){
		case 1:
			while (1) {
				p = new_Borrower();//初始化借书人
				printf("借书者姓名: ");
				cin >> p->Name;
				printf("证件ID: ");
				scanf_s("%lld", &p->ID_Number);
				printf("要借书籍ISBN: ");
				scanf_s("%lld", &p->ISBN);
				printf("借书归还期限: ");
				scanf_s("%d", &p->Deadline);
				printf("——————————\n");
				i = Lend_Register(ISBN, p);
				if (i == NOFOUND)printf("##书本已不在##\n");
				else if (i == SUCCESS)printf("##借书成功##\n");
				else if (i == ERROR)printf("##书本余量不足##\n");
				printf("——————————\n");
				printf("1:继续登记借书 0:回到主界面\n");
				printf("——————————\n");
				scanf_s("%d", &i);
				printf("——————————\n");
				switch (i) {
				case 1: break;
				default: return;
				}
			}
		case 2:
			while (1) {
				q = new_Oppointment();//初始化预约借书的人
				printf("预约者姓名: ");
				cin >> q->Name;
				printf("预约者ID: ");
				scanf_s("%lld", &q->ID_Number);
				printf("预约书籍ISBN: ");
				scanf_s("%lld", &q->ISBN);
				printf("截止日期: ");
				scanf_s("%d", &q->Oppointment_time);
				printf("——————————\n");
				i = Oppointment_Come(ISBN, q, OPPO);
				if (i == NOFOUND)printf("##找不到预约记录##\n");
				else if (i == ERROR)printf("##该书已经不存在##\n##已帮您删除预约记录##\n");
				else if (i == SUCCESS)printf("##借书成功##\n");
				printf("——————————\n");
				printf("1:下一个预约者 0:回到主界面\n");
				printf("——————————\n");
				scanf_s("%d", &i);
				printf("——————————\n");
				switch (i) {
				case 1: break;
				default: return;
				}
			}
		default:return;
		}
	}
}


void Business_3() {//还书
	long long ISBN_;
	long long ID;
	int i;
	int tag;
	while (1) {
		printf("书籍ISBN: ");
		scanf_s("%lld", &ISBN_);
		printf("还书人ID: ");
		scanf_s("%lld", &ID);
		printf("——————————\n");
		tag = Borrower_Out(ISBN, ID, ISBN_);
		if (tag == SUCCESS)printf("##还书成功##\n");
		else if (tag == ERROR)printf("##找不到书##\n");
		else if (tag == NOFOUND)printf("##没有该借书记录##\n");
		printf("——————————\n");
		printf("1:继续还书 0:回到主界面\n");
		printf("——————————\n");
		scanf_s("%d", &i);
		printf("——————————\n");
		switch (i) {
		case 1: break;
		default: return;
		}
	}

}
void Business_4() {//清除书籍
	long long ISBN_;
	int i;
	while (1) {
		printf("清除书籍ISBN: ");
		scanf_s("%lld", &ISBN_);
		printf("——————————\n");
		Delete_Book(ISBN, ISBN_);//完全删除索引ISBN的书
		printf("1:继续删除 0:回到主界面\n");
		printf("——————————\n");
		scanf_s("%d", &i);
		printf("————————\n");
		switch (i){
		case 1: break;
		default: return; 
		}
	}

}
void Business_5() {//显示
	int i;
	while (1) {
		printf("1:输出B树 2:ISBN顺序书目 \n3:输出储存链表 0:回到主界面\n");
		printf("——————————\n");
		scanf_s("%d", &i);
		printf("——————————\n");
		switch (i) {
		case 1:
			printf("B树结构:\n");
			printf("————————\n");
			PrintBTree(ISBN);
			printf("————————\n");
			break;
		case 2:
			printf("按ISBN顺序书目:\n");
			printf("————————\n");
			Show_Database(ISBN);
			break;
		case 3:
			printf("输出链表\n");
			printf("————————\n");
			for (Recode p = Head; p != NULL; p = p->next) {
				if (ERROR == Print_Book(p)) {
					printf("##没有书籍##\n");
					printf("————————\n");
				}
			}
			break;
		default:return;
		}
	}
}
void Business_6() {//查询书籍
	long long ISBN_;
	int i;
	while (1) {
		printf("查询书籍ISBN: ");
		scanf_s("%lld", &ISBN_);
		printf("——————————\n");
		Search_Book(ISBN, ISBN_);//在树ISBN上查询书籍.....可能需要加入借者功能
		printf("1:继续查询 0:回到主页面\n");
		printf("——————————\n");
		scanf_s("%d", &i);
		printf("——————————\n");
		switch (i){
		case 1: break;
		default: return;
		}
	}
}

void Business_7() {//预约借书
	int i;
	Oppointment p;
	while (1) {
		p = new_Oppointment();//初始化借书人
		printf("预约者姓名: ");
		cin >> p->Name;
		printf("预约者ID: ");
		scanf_s("%lld", &p->ID_Number);
		printf("预约书籍ISBN: ");
		scanf_s("%lld", &p->ISBN);
		printf("预约时间: ");
		scanf_s("%d", &p->Oppointment_time);
		printf("——————————\n");
		i = Oppoint_Register(ISBN,OPPO, p);
		if (i == NOFOUND)printf("##找不到这本书##\n");
		else if (i == SUCCESS)printf("##预约成功##\n");
		else if (i == ERROR)printf("##书本余量不足##\n");
		printf("——————————\n");
		printf("1:继续登记预约 0:回到主界面\n");
		printf("——————————\n");
		scanf_s("%d", &i);
		printf("——————————\n");
		switch (i) {
		case 1: break;
		default: return;
		}
	}
}

void Business_8() {//输出时间链
	int i;
	Oppointment p;
	while (1) {
		printf("1:预约信息 2:借书信息 0:回到主界面\n");
		printf("——————————\n");
		scanf_s("%d", &i);
		printf("——————————\n");
		switch (i) {
		case 1: Print_Oppointment_Link(OPPO); break;
		case 2: Print_Borrower_Link(BORROWER); break;
		default: return;
		}
	}
}

void Business_9() {
	
}