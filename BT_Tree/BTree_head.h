
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
	//��������Ϣ�Ľ��
	char Title[20];
	long long ISBN;
	char Autor[20];
	BookKind kind;
	int Total_Stock;
	int Now_Stock;
	struct BorrowerNode *Br;//��������ĺ�����һ������Ϣ
	struct OppointmentNode *Or;////��������ĺ�����һ��ԤԼ�ߵ���Ϣ
	struct recode *next;
	struct recode *prior;
}*Recode, recode;

typedef struct BTNode {
	//B���ɼ�дΪ(n(�ؼ��ָ���),A0,K1,A1��..,Kn,An��
	int keynum;
	KeyType key[m + 1];//0�ŵ�Ԫδ��
	struct BTNode *ptr[m + 1];
	struct BTNode *parent;
	struct recode *recptr[m + 1];//��¼ָ��������0�ŵ�Ԫδ��
}BTNode, *BTree;


typedef struct {
	BTree pt;//ָ���ҵ��Ľ��
	int i;//1<=i<=m,�ڽ���еĹؼ���λ��
	int tag;//1:���ҳɹ���0������ʧ��
}result;


typedef struct BorrowerNode {
	//ȫ���鼮�������Ⱥ󴢴�����ߵ���Ϣ��
	char Name[10];//����
	long long ID_Number;
	long long ISBN;
	int Deadline;//���ڵĽ�ֹʱ�䣬�Ա����Ա�ĺ�������
	struct BorrowerNode *next,*prior;
}*Borrower, BorrowerNode;

typedef struct OppointmentNode {
	//ȫ���鼮ԤԼ�ߵ���Ϣ��
	char Name[10];//����
	long long ID_Number;
	long long ISBN;
	int Oppointment_time;//ԤԼʱ��
	struct OppointmentNode *next,*prior;
}*Oppointment, OppointmentNode;


/////////////////////////////////////////////////////

extern Recode Head;
extern BTree ISBN;
extern Oppointment OPPO;//ԤԼ����
extern Borrower BORROWER;//��������;

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
Recode newRecode();//�����µļ�¼�ռ�
void newBook(Recode &p);//�Ǽ�������Ϣ
void Stock_In(Recode &Head, Recode p);//p��⵽�������ͷ��¼��
void Build_ISBN_Index(BTree &t,Recode p);//Ϊp��¼��B��t�Ͻ���ISBN������
Status Print_Book(Recode p);//�������ݿ��������ָ������ʾ������Ϣ
void Search_Book(BTree &t, KeyType x);///��t����������x��Ӧ�ļ�¼
void Show_Database(BTree t);//���������������Ŀ��������Ϣ
//��B��t��q���iλ���ϲ���Recodeָ��
Recode Random_Book();//�������һ���飬ȫ������Ϊ���֣�������
void Test();//����BTree_port����
void BTInsert(BTree &p, KeyType x, Recode rec);
//��������
//��B��p�в���x�ؼ��ֺͼ�¼ָ��rec
//����Ϊ�գ����½�һ����
void Remove_Recode(Recode &p);	//�Ƴ�p��¼
void Delete_Book(BTree &t, KeyType x);//��ȫɾ��ISBNΪx����
void Borrower_In(Recode &t, Borrower b);//��rec��¼��ʱ��˳�����ӽ�����
Status Lend_Register(BTree &t, Borrower b);
	//�����߰������������t����ָ��ļ�¼p��������У���ʱ��˳�����(������)
	//�޸ĸ�����������
Status Print_Borrower(Recode rec);//����Ȿ���Ľ�����(ʱ��˳��)
Borrower new_Borrower();//�½�һ�������˽��ָ�벢����
Borrower Random_Borrower(Recode rec);//������ɽ����˲�����ָ��
Recode Recode_Position(BTree t, long long index);//��������ֵ�����ش����������λ�õ�ָ��
Status Borrower_Out(BTree t, long long ID_Number, long long ISBN);//������źͽ����������
Oppointment new_Oppointment();//�½�һ��ԤԼ�˽��ָ�벢����
Status Oppoint_Register(BTree t, Oppointment &head,Oppointment b);//�жϣ�֮��ԤԼ�߰�ʱ��˳��Ǽǵ�ԤԼʱ����
Status Oppointment_In_Link(Oppointment &t, Oppointment b);//��ԤԼʱ��������ԤԼ��
void Print_Oppointment_Link(Oppointment p);//��˳�����ԤԼ��
void Print_Borrower_Link(Borrower p);//�����������
Borrower Oppointment_To_Borrower(Oppointment p);//��ԤԼ�߱������
Status Oppointment_Come(BTree &t, Oppointment op, Oppointment &head);//ԤԼ�߶���ԤԼ
void Borrower_In_Link(Borrower &q, Borrower b);//������p�а�ʱ��˳�����b
Borrower Borrower_Copy(Borrower b);//����b�����ˣ�����ָ��
Status Borrower_Out_Link(Borrower &head, long long ID_Number, long long ISBN_);//�����߳�ʱ����
void InitLibrary();//����ͼ��ݳ�ʼ����
						  
///////////////////////////////////



int SearchPosition(BTree t, KeyType x);//Ѱ��x��t���Ĳ���λ��
void AddKey(BTree &q, KeyType x, BTree ap,Recode rec);//��q���󷽼���ؼ���x��apָ��
void newRoot(BTree &t, BTree p, KeyType x, BTree ap,Recode rec);
void InsertBTree(BTree &t, KeyType k,Recode rec, BTree q, int i);
//��B��t�е�q����key[i-1]��key[i]֮�����ؼ���k
	//���������Ĺؼ��ָ�������B���Ľף�������Ϊm����������˫�׽����з��ѡ�
	//B���ɼ�дΪ(n(�ؼ��ָ���),A0,K1,A1��..,Kn,An��
int NodeDepth(BTree t);//�������ڲ���
void PrintBTree(BTree t);//�����1��ʾB��t
void Insert(BTree &q, int i, KeyType x, BTree ap,Recode rec);//�ؼ���x���½��ָ��ap�ֱ���뵽q->key[i]��q->ptr[i]
void split(BTree &q, int s, BTree &ap);
//B����q�����n���ؼ��֣�����Ϊ������㣬ǰһ�루s-1,A0��K1��A1��...��Ks-1��As-1��������ԭ�����,
	//��һ�루As,Ks+1,...Kn,An�������½��(n-s,A0,K1,A1,..,Kn-s,An-s),apָ������½��
void SearchBTree(BTree &t, KeyType x, result &r);
	//���ݴ�С��ϵѰ��B��t�е�x��λ�ã��Ҳ����򷵻ز���λ��
void BuildBTree(BTree &t);//����B���Ĺؼ���������B��
void BorrowLeft(BTree &ap);
//���ap�����ֵܽ�ؼ��֣��ؼ���˳ʱ��ѭ����
void BorrowRight(BTree &ap);
	//���ap�����ֵܽ�ؼ��֣��ؼ���˳ʱ��ѭ����
	//��Ӧ�ĵ�����B��
int ParentPosition(BTree ap);
	//Ѱ��ap��˫��ָ������ָ��ptr[i]���±�i
void SearchInsert(BTree &t, KeyType x, result &r);
	//���ݴ�С��ϵѰ��B��t�п��Բ��뵽���²���Ľ��
void CombineLeft(BTree &ap);//�ϲ���㵽�������ֵ���
void CombineRight(BTree &ap);//�ϲ���㵽�������ֵ���
void Successor(BTree &p, int &i);
//p�滻�ؼ���Ϊ�����²���ն˽����ֱ�Ӻ�����ڵĽ��
void Remove(BTree &t,int i);//���ն˽��p���Ƴ�ki
void Restore(BTree &t);//����t���(������ɾ����ĵ���)
void DeleteKey(BTree &p, int i);//B��p�����ɾ��iλ�õĹؼ���x
void BTDelete(BTree &p, KeyType x);//��B��p��ɾ��x
////////////

void Test_Insert();
void Test_Search();
void Test_Delete();
void Test_Datas();