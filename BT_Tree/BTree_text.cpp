#include"BTree_head.h"

int main() {
	system("color F0");
	system("title ͼ�����ϵͳ");
	InitLibrary();
	int i;
	printf("��������������������\n");
	printf("ͼ�����ϵͳ\n");
	printf("��������������������\n");
	while (1) {
		printf("1:�鱾��� 2:���� 3:���� 4:���� \n5:��ʾͼ�� 6:���� 7:ԤԼ 8:ʱ����\n0:�˳�ϵͳ\n");
		printf("��������������������\n");
		scanf_s("%d", &i);
		printf("������������\n");
		switch (i) {
		case 0: return 0;
		case 1:	Business_1(); break;
		case 2:	Business_2(); break;
		case 3:	Business_3(); break;
		case 4:	Business_4(); break;
		case 5:	Business_5(); break;
		case 6:	Business_6(); break;
		case 7:	Business_7(); break;
		case 8:	Business_8(); break;
		case 9:	Business_9(); break;
		default: break;
		}
	}

	return 0;
}
