#include"BTree_head.h"

int main() {
	system("color F0");
	system("title 图书管理系统");
	InitLibrary();
	int i;
	printf("——————————\n");
	printf("图书管理系统\n");
	printf("——————————\n");
	while (1) {
		printf("1:书本入库 2:借书 3:还书 4:清书 \n5:显示图书 6:查书 7:预约 8:时间链\n0:退出系统\n");
		printf("——————————\n");
		scanf_s("%d", &i);
		printf("——————\n");
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
