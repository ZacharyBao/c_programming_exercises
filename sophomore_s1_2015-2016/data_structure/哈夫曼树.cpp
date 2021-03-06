#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#define NULL 0
using namespace std;
const int Maxvalue = 1000; //权值最大1000
const int Maxbit = 20;	   //编码位数不超过20，也就是哈夫曼树不高于20层
const int Maxnode = 100;   //哈夫曼树携带的最多节点数
struct store			   //存储字符以及其权值的结构体，为哈夫曼树做准备
{
	char ch;
	int weight;
	struct store *next;
};
struct haffNode //哈夫曼树节点数据构成
{
	int flag;
	int parent;
	int weight;
	int leftchild;
	int rightchild;
};
struct haffCode //哈夫曼叶节点的编码存储结构
{
	int start;
	int weight;
	int codebit[Maxbit]; //最大编码位数20位，也就是此哈夫曼树最高可以有20层
};
void transHafftree(haffNode node[], int n, store Weight[]) //构造哈夫曼树
{
	int i, j, m1, m2, x1, x2;
	;
	int child, parent;
	for (i = 0; i < 2 * n - 1; i++) //初始化所有结点
	{
		if (i < n)
			node[i].weight = Weight[i].weight;
		else
			node[i].weight = 0;
		node[i].parent = 0;
		node[i].flag = 0;
		node[i].leftchild = -1;
		node[i].rightchild = -1;
	}
	for (i = 0; i < n - 1; i++) //构建哈夫曼树
	{

		m1 = m2 = Maxvalue; //m1,m2,x1,x2需要放在循环里面，每次一个循环都要重新赋值
		x1 = x2 = 0;
		for (j = i; j < n + i; j++) //在所有所有已知权重的节点中找到两个最小的
		{
			if (node[j].weight < m1 && node[j].flag == 0)
			{
				m2 = m1;
				x2 = x1;
				m1 = node[j].weight;
				x1 = j;
			}
			else if (node[j].weight < m2 && node[j].flag == 0)
			{
				m2 = node[j].weight;
				x2 = j;
			}
		}
		node[x1].parent = n + i;
		node[x2].parent = n + i;
		node[x1].flag = 1;
		node[x2].flag = 1;
		node[n + i].leftchild = x1;
		node[n + i].rightchild = x2;
		node[n + i].weight = node[x1].weight + node[x2].weight;
	}
}
void transHaffcode(haffNode node[], int n, haffCode code[]) //求出每个字符的哈夫曼码
{
	haffCode CODE; //用来暂时存放某个叶节点的编码
	int i, j, child, parent;
	for (i = 0; i < n; i++)
	{
		child = i;
		parent = node[child].parent;
		CODE.start = 0;
		CODE.weight = node[i].weight;
		while (parent != 0)
		{
			if (node[parent].leftchild == child)
				CODE.codebit[CODE.start] = 0;
			else if (node[parent].rightchild == child)
				CODE.codebit[CODE.start] = 1;
			CODE.start++;
			child = parent;
			parent = node[child].parent;
		}
		code[i].weight = CODE.weight;
		code[i].start = CODE.start;
		for (j = 0; j < CODE.start; j++)
		{
			code[i].codebit[CODE.start - j - 1] = CODE.codebit[j]; //CODE.start-j-1在这里是有说法的，因为是start个parent，所以下标最大为S-1
																   /*Test cout << code[i].codebit[CODE.start-j-1];*/
		}
		/*Test for (j = 0; j < code[i].start; j++)
			cout << code[i].codebit[j];*/
	}
}
store *head;
store *returnTr(char str[]) //统计字符串中各个字符的个数
{
	int i, j = 0, k, flag; //j:链表节点的个数;flag:标记是否在节点的数据中找到相同的字符
	store *p1, *p2, *p3;
	head = p1 = p2 = p3 = (struct store *)malloc(sizeof(struct store));
	for (i = 0; i < strlen(str); i++)
	{
		if (i == 0) //i=0时，将字符串的首字符复制给链表头节点的ch
		{
			head->ch = str[0];
			head->weight = 0; //权值先初始化，这里既然出现了一次这个字符，权值为何是0？是由于下面i=0时权值还是会自加一次
			p1 = p2 = head;	  //head刚刚得到了数据赋值，将其数据给p1，p2
		}
		flag = 0;				 //由于字符串中每个字符都要进行链表的遍历比对，所以这个flag每次作用结束都要重新归零，方便下一个字符的标记
		p3 = head;				 //由于是链表遍历，每一次遍历结束后p3的数据都会发生变化，所以每次遍历开始重置p3数据
		for (k = 0; k <= j; k++) //j在这里是动态变化的，j+1是节点数量，遍历就是要访问每个节点
		{
			if (str[i] == p3->ch) //如果字符串中的字符和链表当中的ch有一致的话，该节点的权值自加
			{
				p3->weight++;
				flag = 1; //标记在链表节点的ch中找到一样的
				break;
			}
			p3 = p3->next;
		}
		if (flag == 0)
		{
			j++; //执行此语句说明这个字符是链表中没有的数据，所以需要将其加入
			p1 = (store *)malloc(sizeof(store));
			p1->ch = str[i];
			p1->weight = 1;
			p2->next = p1;
			p2 = p1;
		}
	}
	p1->next = NULL;
	p1 = head;
	return head;
}
void bottomCode(char str[], haffCode code[], int n, store Weight[]) //展示字符串被压缩后的补码
{
	int i, j, k;
	cout << "该字符串经过压缩后的补码如下" << endl;
	for (i = 0; i < strlen(str); i++)
	{
		for (j = 0; j < n; j++)
			if (str[i] == Weight[j].ch)
			{
				for (k = 0; k < code[j].start; k++)
					cout << code[j].codebit[k];
			}
	}
	cout << endl;
}
int main()
{
	store *p1, *p2;
	int i, k, j, count = 0;
	store *node, temp;
	char str[100];
	haffNode hNode[100]; //定义哈夫曼树节点
	haffCode hCode[100]; //定义哈夫曼树叶节点编码存储数组
	cout << "请输入字符串" << endl;
	cin >> str;
	p1 = p2 = returnTr(str);
	while (p1 != NULL)
	{
		count++; //计算出节点数量，为了数组的大小
		p1 = p1->next;
	}
	if (count > Maxnode)
	{
		cout << "节点数越界，请重新设定！" << endl;
		system("pause");
		exit(0);
	}
	node = (store *)malloc(sizeof(store) * count); //节省内存
	i = 0;
	while (p2 != NULL) //将链表的数据传给数组
	{
		node[i].weight = p2->weight;
		node[i].ch = p2->ch;
		i++;
		p2 = p2->next;
	}
	/*TEST for (i = 0; i < count; i++)
	{
	cout << node[i].ch << ":"<<node[i].weight<<endl;
	}*/
	for (i = 0; i < count - 1; i++) //将得到的数组权值从小到大冒泡排序
		for (j = 0; j < count - i - 1; j++)
		{
			if (node[j].weight > node[j + 1].weight)
			{
				temp = node[j];
				node[j] = node[j + 1];
				node[j + 1] = temp;
			}
		}
	/*以上是利用returnTr函数返回的链表头所携带的链表信息转化为数组形式*/
	transHafftree(hNode, count, node);	//构建哈夫曼树
	transHaffcode(hNode, count, hCode); //计算哈夫曼编码
	cout << "ch  wieght  code" << endl;
	for (i = 0; i < count; i++)
	{
		cout << " " << node[i].ch << "    " << node[i].weight << "     ";
		for (j = 0; j < hCode[i].start; j++)
			cout << hCode[i].codebit[j];
		cout << endl;
	}
	bottomCode(str, hCode, count, node);
	system("pause");
	return 0;
}

修改过后
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#define NULL 0
using namespace std;
const int Maxvalue = 1000; //权值最大1000
const int Maxbit = 20;	   //编码位数不超过20，也就是哈夫曼树不高于20层
const int Maxnode = 100;   //哈夫曼树携带的最多节点数
struct store			   //存储字符以及其权值的结构体，为哈夫曼树做准备
{
	char ch;
	int weight;
	struct store *next;
};
struct haffNode //哈夫曼树节点数据构成
{
	int flag;
	int parent;
	int weight;
	int leftchild;
	int rightchild;
};
struct haffCode //哈夫曼叶节点的编码存储结构
{
	int start;
	int weight;
	int codebit[Maxbit]; //最大编码位数20位，也就是此哈夫曼树最高可以有20层
};
void transHafftree(haffNode node[], int n, store Weight[]) //构造哈夫曼树
{
	int i, j, m1, m2, x1, x2;
	;
	int child, parent;
	for (i = 0; i < 2 * n - 1; i++) //初始化所有结点,n个叶节点会早就2*n-1个节点的哈夫曼树
	{
		if (i < n)
			node[i].weight = Weight[i].weight;
		else
			node[i].weight = 0;
		node[i].parent = 0;
		node[i].flag = 0;
		node[i].leftchild = -1;
		node[i].rightchild = -1;
	}
	for (i = 0; i < n - 1; i++) //构建哈夫曼树
	{

		m1 = m2 = Maxvalue; //m1,m2,x1,x2需要放在循环里面，每次一个循环都要重新赋值
		x1 = x2 = 0;
		for (j = i; j < n + i; j++) //在所有所有已知权重的节点中找到两个最小的，由于进行过冒泡排序，每次从i开始也行
		{
			if (node[j].weight < m1 && node[j].flag == 0)
			{
				m2 = m1;
				x2 = x1;
				m1 = node[j].weight;
				x1 = j;
			}
			else if (node[j].weight < m2 && node[j].flag == 0)
			{
				m2 = node[j].weight;
				x2 = j;
			}
		}
		node[x1].parent = n + i;
		node[x2].parent = n + i;
		node[x1].flag = 1;
		node[x2].flag = 1;
		node[n + i].leftchild = x1;
		node[n + i].rightchild = x2;
		node[n + i].weight = node[x1].weight + node[x2].weight;
	}
}
void transHaffcode(haffNode node[], int n, haffCode code[]) //求出每个字符的哈夫曼码
{
	haffCode CODE; //用来暂时存放某个叶节点的编码
	int i, j, child, parent;
	for (i = 0; i < n; i++) //叶节点就是哈弗曼数组的前n个
	{
		child = i;
		parent = node[child].parent;
		CODE.start = 0;
		CODE.weight = node[i].weight;
		while (parent != 0)
		{
			if (node[parent].leftchild == child)
				CODE.codebit[CODE.start] = 0;
			else if (node[parent].rightchild == child)
				CODE.codebit[CODE.start] = 1;
			CODE.start++;
			child = parent;
			parent = node[child].parent;
		}
		code[i].weight = CODE.weight;
		code[i].start = CODE.start;
		for (j = 0; j < CODE.start; j++)
		{
			code[i].codebit[CODE.start - j - 1] = CODE.codebit[j]; //CODE.start-j-1在这里是有说法的，因为是start个parent，所以下标最大为S-1
																   /*Test cout << code[i].codebit[CODE.start-j-1];*/
		}														   //将原始的code调换左右的顺序
																   /*Test for (j = 0; j < code[i].start; j++)
		cout << code[i].codebit[j];*/
	}
}
store *head;
store *returnTr(char str[]) //统计字符串中各个字符的个数
{
	int i, j = 0, k, flag; //j:链表节点的个数;flag:标记是否在节点的数据中找到相同的字符
	store *p1, *p2, *p3;
	head = p1 = p2 = p3 = (struct store *)malloc(sizeof(struct store));
	for (i = 0; i < strlen(str); i++)
	{
		if (i == 0) //i=0时，将字符串的首字符复制给链表头节点的ch
		{
			head->ch = str[0];
			head->weight = 0; //权值先初始化，这里既然出现了一次这个字符，权值为何是0？是由于下面i=0时权值还是会自加一次
			p1 = p2 = head;	  //head刚刚得到了数据赋值，将其数据给p1，p2
		}
		flag = 0;				 //由于字符串中每个字符都要进行链表的遍历比对，所以这个flag每次作用结束都要重新归零，方便下一个字符的标记
		p3 = head;				 //由于是链表遍历，每一次遍历结束后p3的数据都会发生变化，所以每次遍历开始重置p3数据
		for (k = 0; k <= j; k++) //j在这里是动态变化的，j+1是节点数量，遍历就是要访问每个节点
		{
			if (str[i] == p3->ch) //如果字符串中的字符和链表当中的ch有一致的话，该节点的权值自加
			{
				p3->weight++;
				flag = 1; //标记在链表节点的ch中找到一样的
				break;
			}
			p3 = p3->next;
		}
		if (flag == 0)
		{
			j++; //执行此语句说明这个字符是链表中没有的数据，所以需要将其加入
			p1 = (store *)malloc(sizeof(store));
			p1->ch = str[i];
			p1->weight = 1;
			p2->next = p1;
			p2 = p1;
		}
	}
	p1->next = NULL;
	p1 = head;
	return head;
}
void bottomCode(char str[], haffCode code[], int n, store Weight[]) //展示字符串被压缩后的补码
{
	int i, j, k;
	cout << "该字符串经过压缩后的补码如下" << endl;
	for (i = 0; i < strlen(str); i++)
	{
		for (j = 0; j < n; j++)
			if (str[i] == Weight[j].ch)
			{
				for (k = 0; k < code[j].start; k++)
					cout << code[j].codebit[k];
			}
	}
	cout << endl;
}
int main()
{
	store *p1, *p2;
	int i, k, j, count = 0;
	store *node, temp;
	char str[100];
	haffNode hNode[100]; //定义哈夫曼树节点
	haffCode hCode[100]; //定义哈夫曼树叶节点编码存储数组
	cout << "请输入字符串" << endl;
	cin >> str;
	p1 = p2 = returnTr(str);
	while (p1 != NULL)
	{
		count++; //计算出节点数量，为了数组的大小
		p1 = p1->next;
	}
	if (count > Maxnode)
	{
		cout << "节点数越界，请重新设定！" << endl;
		system("pause");
		exit(0);
	}
	node = (store *)malloc(sizeof(store) * count); //节省内存
	i = 0;
	while (p2 != NULL) //将链表的数据传给数组
	{
		node[i].weight = p2->weight;
		node[i].ch = p2->ch;
		i++;
		p2 = p2->next;
	}
	/*TEST for (i = 0; i < count; i++)
	{
	cout << node[i].ch << ":"<<node[i].weight<<endl;
	}*/
	for (i = 0; i < count - 1; i++) //将得到的数组权值从小到大冒泡排序
		for (j = 0; j < count - i - 1; j++)
		{
			if (node[j].weight > node[j + 1].weight)
			{
				temp = node[j];
				node[j] = node[j + 1];
				node[j + 1] = temp;
			}
		}
	/*以上是利用returnTr函数返回的链表头所携带的链表信息转化为数组形式*/
	transHafftree(hNode, count, node);	//构建哈夫曼树
	transHaffcode(hNode, count, hCode); //计算哈夫曼编码
	cout << "ch  weight  code" << endl;
	for (i = 0; i < count; i++)
	{
		cout << " " << node[i].ch << "    " << node[i].weight << "     ";
		for (j = 0; j < hCode[i].start; j++)
			cout << hCode[i].codebit[j];
		cout << endl;
	}
	bottomCode(str, hCode, count, node);
	system("pause");
	return 0;
}
