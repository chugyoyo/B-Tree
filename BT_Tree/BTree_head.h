
#pragma once

#define m 5
#define minnum 2
#define NOFOUND -1
#define SUCCESS 1
#define ERROR 0


#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<time.h> 


using namespace std;
typedef long long KeyType; 
typedef enum { EDU, SIC, REC, OTHER }BookKind;
typedef int Status;


typedef struct recode {
	//储存书信息的结点
	char Title[20];
	long long ISBN;
	char Autor[20];
	BookKind kind;
	int Total_Stock;
	int Now_Stock;
	struct BorrowerNode *Br;//在这种书的后方链接一串的信息
	struct OppointmentNode *Or;////在这种书的后方链接一串预约者的信息
	struct recode *next;
	struct recode *prior;
}*Recode, recode;

typedef struct BTNode {
	//B树可简写为(n(关键字个数),A0,K1,A1，..,Kn,An）
	int keynum;
	KeyType key[m + 1];//0号单元未用
	struct BTNode *ptr[m + 1];
	struct BTNode *parent;
	struct recode *recptr[m + 1];//记录指针向量，0号单元未用
}BTNode, *BTree;


typedef struct {
	BTree pt;//指向找到的结点
	int i;//1<=i<=m,在结点中的关键字位序
	int tag;//1:查找成功，0：查找失败
}result;


typedef struct BorrowerNode {
	//全部书籍按到期先后储存借阅者的信息链
	char Name[10];//姓名
	long long ID_Number;
	long long ISBN;
	int Deadline;//到期的截止时间，以便管理员的后续工作
	struct BorrowerNode *next,*prior;
}*Borrower, BorrowerNode;

typedef struct OppointmentNode {
	//全部书籍预约者的信息链
	char Name[10];//姓名
	long long ID_Number;
	long long ISBN;
	int Oppointment_time;//预约时间
	struct OppointmentNode *next,*prior;
}*Oppointment, OppointmentNode;


/////////////////////////////////////////////////////

extern Recode Head;
extern BTree ISBN;
extern Oppointment OPPO;//预约者链
extern Borrower BORROWER;//借书者链;

////////////////////////
void Business_1();
void Business_2();
void Business_3();
void Business_4();
void Business_5();
void Business_6();
void Business_7();
void Business_8();
void Business_9();



//////////////////////


void Books_In();//1
Recode newRecode();//生成新的记录空间
void newBook(Recode &p);//登记新书信息
void Stock_In(Recode &Head, Recode p);//p入库到储存库中头记录后
void Build_ISBN_Index(BTree &t,Recode p);//为p记录在B树t上建立ISBN码索引
Status Print_Book(Recode p);//输入数据库这种书的指针来显示该书信息
void Search_Book(BTree &t, KeyType x);///在t索引树上找x对应的记录
void Show_Database(BTree t);//按索引输出所有书目的所有信息
//在B树t的q结点i位置上插入Recode指针
Recode Random_Book();//随机生成一本书，全部数据为数字，测试用
void Test();//测试BTree_port函数
void BTInsert(BTree &p, KeyType x, Recode rec);
//建立索引
//在B树p中插入x关键字和记录指针rec
//若树为空，则新建一棵树
void Remove_Recode(Recode &p);	//移除p记录
void Delete_Book(BTree &t, KeyType x);//完全删除ISBN为x的书
void Borrower_In(Recode &t, Borrower b);//在rec记录后按时间顺序链接借书者
Status Lend_Register(BTree &t, Borrower b);
	//借书者按书的索引，在t索引指向的记录p后的链表中，按时间顺序插入(借书者)
	//修改该书的相关内容
Status Print_Borrower(Recode rec);//输出这本书后的借书者(时间顺序)
Borrower new_Borrower();//新建一个借书人结点指针并返回
Borrower Random_Borrower(Recode rec);//随机生成借书人并返回指针
Recode Recode_Position(BTree t, long long index);//根据索引值来返回待操作的书的位置的指针
Status Borrower_Out(BTree t, long long ID_Number, long long ISBN);//根据书号和借书号来还书
Oppointment new_Oppointment();//新建一个预约人结点指针并返回
Status Oppoint_Register(BTree t, Oppointment &head,Oppointment b);//判断，之后将预约者按时间顺序登记到预约时间链
Status Oppointment_In_Link(Oppointment &t, Oppointment b);//在预约时间链接入预约者
void Print_Oppointment_Link(Oppointment p);//按顺序输出预约者
void Print_Borrower_Link(Borrower p);//输出借书者链
Borrower Oppointment_To_Borrower(Oppointment p);//由预约者变借书者
Status Oppointment_Come(BTree &t, Oppointment op, Oppointment &head);//预约者兑现预约
void Borrower_In_Link(Borrower &q, Borrower b);//在链表p中按时间顺序插入b
Borrower Borrower_Copy(Borrower b);//复制b借书人，返回指针
Status Borrower_Out_Link(Borrower &head, long long ID_Number, long long ISBN_);//借书者出时间链
void InitLibrary();//给予图书馆初始数据
						  
///////////////////////////////////



int SearchPosition(BTree t, KeyType x);//寻找x在t结点的插入位置
void AddKey(BTree &q, KeyType x, BTree ap,Recode rec);//在q结点后方加入关键字x与ap指针
void newRoot(BTree &t, BTree p, KeyType x, BTree ap,Recode rec);
void InsertBTree(BTree &t, KeyType k,Recode rec, BTree q, int i);
//在B树t中的q结点的key[i-1]和key[i]之间插入关键字k
	//若插入后结点的关键字个数等于B树的阶（这里设为m），则沿着双亲结点进行分裂。
	//B树可简写为(n(关键字个数),A0,K1,A1，..,Kn,An）
int NodeDepth(BTree t);//求结点所在层数
void PrintBTree(BTree t);//凹入表1显示B树t
void Insert(BTree &q, int i, KeyType x, BTree ap,Recode rec);//关键字x和新结点指针ap分别插入到q->key[i]和q->ptr[i]
void split(BTree &q, int s, BTree &ap);
//B树的q结点有n个关键字，分裂为两个结点，前一半（s-1,A0，K1，A1，...，Ks-1，As-1）保留在原结点中,
	//另一半（As,Ks+1,...Kn,An）移入新结点(n-s,A0,K1,A1,..,Kn-s,An-s),ap指向这个新结点
void SearchBTree(BTree &t, KeyType x, result &r);
	//根据大小关系寻找B树t中的x的位置，找不到则返回插入位置
void BuildBTree(BTree &t);//输入B树的关键字来构建B树
void BorrowLeft(BTree &ap);
//结点ap向左兄弟借关键字（关键字顺时针循环）
void BorrowRight(BTree &ap);
	//结点ap向右兄弟借关键字（关键字顺时针循环）
	//相应的调整好B树
int ParentPosition(BTree ap);
	//寻找ap的双亲指向它的指针ptr[i]的下标i
void SearchInsert(BTree &t, KeyType x, result &r);
	//根据大小关系寻找B树t中可以插入到最下层结点的结果
void CombineLeft(BTree &ap);//合并结点到它的左兄弟中
void CombineRight(BTree &ap);//合并结点到它的右兄弟中
void Successor(BTree &p, int &i);
//p替换关键字为其最下层非终端结点中直接后继所在的结点
void Remove(BTree &t,int i);//从终端结点p中移除ki
void Restore(BTree &t);//调整t结点(仅考虑删除后的调整)
void DeleteKey(BTree &p, int i);//B树p结点上删除i位置的关键字x
void BTDelete(BTree &p, KeyType x);//在B树p中删除x
////////////

void Test_Insert();
void Test_Search();
void Test_Delete();
void Test_Datas();