//公用头文件以及宏定义
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define OVERFLOW 0
#define NIL '#'
#define MAX_TREE_SIZE 100//最大结点数 
/***********************************************************************************/
//树的双亲表存储结构
typedef char TElemType;//树结点存储的信息为字符型
typedef int Status;
typedef struct PTNode{ //结点结构
	TElemType data;//元素值 
	int parent; //双亲位置,根结点的parent为-1 
}PTNode;

typedef struct{  //树结构
	PTNode nodes[MAX_TREE_SIZE];//由初始换操作分配的结点数组 
	int root,nodeNum;  //根的位置和结点数 
}PTree;
/***********************************************************************************/
//树的辅助队列结构
typedef struct
{
	int num;//结点的位置
	TElemType data;//结点信息
}QElemType; //定义队列元素类型 

//单链队列－－队列的链式存储结构 
typedef struct LQNode
{
	QElemType data;
	struct LQNode *next;
}LQNode,*QueuePtr;//结点和结点指针类型 

typedef struct
{
	QueuePtr front,rear; //队头、队尾指针
}LinkQueue;//链队列类型 
/***********************************************************************************/
//队列的基本操作
// 构造一个空队列Q
Status InitQueue(LinkQueue &Q)
{ 
	Q.front=Q.rear=(LQNode*)malloc(sizeof(LQNode));
	if(!Q.front)
		exit(OVERFLOW);
	Q.front->next=NULL;
	return OK;
}
//销毁队列Q
Status DestroyQueue(LinkQueue &Q)
{ 
	while(Q.front)
	{
		Q.rear=Q.front->next;
		free(Q.front);
		Q.front=Q.rear;
	}
	return OK;
}
//将Q清为空队列 
Status ClearQueue(LinkQueue &Q)
{ 
	QueuePtr p,q;
	Q.rear=Q.front;
	p=Q.front->next;
	Q.front->next=NULL;
	while(p)
	{
		q=p;
		p=p->next;
		free(q);
	}
	return OK;
}
//若Q为空队列,则返回TRUE,否则返回FALSE 
Status QueueEmpty(LinkQueue Q)
{ 
	if(Q.front==Q.rear)
		return TRUE;
	else
		return FALSE;
}
//插入元素e为Q的新的队尾元素 
Status EnQueue(LinkQueue &Q,QElemType e)
{ 
	LQNode *p;
	p=(LQNode*)malloc(sizeof(LQNode));
	if(!p) //存储分配失败 
		return OVERFLOW;
	p->data=e;
	p->next=NULL;
	Q.rear->next=p;
	Q.rear=p;
	return OK;
}
//若队列不空,删除Q的队头元素,用e返回其值,并返回OK,否则返回ERROR 
Status DeQueue(LinkQueue &Q,QElemType &e)
{ 
	QueuePtr p;
	if(Q.front==Q.rear)
		return ERROR;
	p=Q.front->next;
	e=p->data;
	Q.front->next=p->next;
	if(Q.rear==p)
		Q.rear=Q.front;
	free(p);
	return OK;
}

/***********************************************************************************/
//树的基本操作
//操作结果：构造空树T
Status InitTree(PTree &T)
{
	T.nodeNum=0;//树的结点数置为0
	return OK;
}

//初始条件：树T存在；操作结果：销毁树T
void DestroyTree(PTree &T)
{
	T.nodeNum=0;//树的双亲表存储结构是用定长的数据类型，无法销毁
}

//初始条件：空树T存在；操作结果：构造树T
Status CreateTree(PTree &T)
{
	LinkQueue Q;
	QElemType q,t;
	char temp[MAX_TREE_SIZE];
	int i=1,j,len;
	InitQueue(Q);
	printf("\t请输入树的根结点(如果输入是'#'字符的话，则根为空):");
	T.nodes[0].data=getche();
	printf("\n");
	if(T.nodes[0].data!=NIL)//根结点不为'#'符，即不为空
	{
		T.nodes[0].parent=-1;
		q.data=T.nodes[0].data;
		q.num=0;
		EnQueue(Q,q);//根入队
		while(i<MAX_TREE_SIZE&&!QueueEmpty(Q))
		{
			DeQueue(Q,q);
		//输入所有q.data的孩子结点，临时保存到temp[MAX_TREE_SIZE]中
		//这里还要考虑怎么结束输入创建树
			printf("\t请输入%c的所有孩子结点(如果没有孩子，按回车键输入下一个结点的孩子):",q.data);
			gets(temp);
			len=strlen(temp);
			for(j=0;j<len;j++,i++)
			{
				T.nodes[i].parent=q.num;
				T.nodes[i].data=temp[j];
				t.data=temp[j];
				t.num=i;
				EnQueue(Q,t);
			}
		}
		if(i>=MAX_TREE_SIZE)//超出范围
			exit(OVERFLOW);
		T.nodeNum=i;
	}
	else 
	T.nodeNum=0;
	return OK;
}

//初始条件：树T存在；操作结果：将树清为空树
Status ClearTree(PTree &T)
{
	T.nodeNum=0;
	return OK;
}

//初始条件：树T存在；操作结果：若T为空树，则返回TRUE，否则返回FALSE
Status TreeEmpty(PTree T)
{
	if(T.nodeNum==0) 
		return TRUE;
	else 
		return FALSE;
}

//初始条件：树T存在；操作结果：返回T的深度
Status TreeDepth(PTree T)
{
	int depthmax=0,depth,i,parent;
	//depthmax存放树的最大深度，depth存在遍历到该结点的深度，parent表示双亲的位置
	for(i=0;i<T.nodeNum;i++)
	{
		depth=1;
		parent=T.nodes[i].parent;
		while(parent!=-1)//从T.nodes[i]结点退回根结点,退一步，深度加1
		{
			parent=T.nodes[parent].parent;
			depth++;   
		}
		if(depthmax<depth) depthmax=depth;
	}
	return depthmax;
}

//初始条件：树T存在；操作结果：返回T的根
TElemType Root(PTree T)
{
	if(T.nodeNum)
		return T.nodes[0].data;
	else {
	printf("\n\t树为空，没有根结点!");
	return NIL;
	}
}

//初始条件：树T存在，position是T中的某个结点的位置；
//操作结果：返回第position结点的值,若position大于树原有的结点数，返回错误信息
TElemType Value(PTree T,int position)
{
	if(position<=T.nodeNum) 
		return T.nodes[position-1].data;
	else
	{
		printf("\n\t给出的第%d结点超出树原有的结点数，错误！\n",position);
		return NIL;
	}
}

//初始条件：树T存在，position是T中某个结点的位置；
//操作结果：将第position个结点的值赋值为value
Status Assign(PTree &T,int position,TElemType value)
{
	if(position<=T.nodeNum)
	{
		T.nodes[position-1].data=value;
		return OK;
	}
	else 
		return ERROR;
}

//初始条件：树T存在，position是T中某个结点的位置
//操作结果：若第position个结点是T的非根结点，则返回它的双亲，否则函数值为空
TElemType Parent(PTree T,int position)
{
	if(position==1) 
	{		
		printf("\n\t给出的第%d结点为树的根结点，根结点没有双亲！\n",position);
		return NIL;
	}
	else if(position<=T.nodeNum) 
		return T.nodes[T.nodes[position-1].parent].data;
	else 
	{
		printf("\n\t给出的第%d结点超出树原有的结点数，错误！\n",position);
		return NIL;
	}
}

//初始条件：树T存在，value是T中某个结点的值
//操作结果：若value是T的非叶子结点，则返回它的最左孩子，否则返回空
TElemType LeftChild(PTree T,TElemType value)
{
	int i,j;
	for(i=0;i<T.nodeNum;i++)
		if(T.nodes[i].data==value)
		 break;//找到值为value的结点T.nodes[i]
	if(i>=T.nodeNum) {
		printf("\n\t树中不存在值为%c的结点！错误！",value);
		return NIL;
	}
	for(j=i+1;j<T.nodeNum;j++)
		if(T.nodes[j].parent==i) 
		return T.nodes[j].data;
	if(j>=T.nodeNum) { 
		printf("\n\t没有找到%c的最左孩子结点，说明值为%c的结点没有孩子！",value,value);
		return NIL;
	}
}

//初始条件：树T存在，value是T中某个结点的值
//操作结果：若value有右兄弟，则返回它的右兄弟，否则函数值为空
TElemType RightSibling(PTree T,TElemType value)
{
	int i;
	for(i=0;i<T.nodeNum;i++)//找出值为value的结点T.nodes[i]
		if(T.nodes[i].data==value)
			break;
	if(i>=T.nodeNum) {
		printf("\n\t树中不存在值为%c的结点！\n",value);
		return NULL;
	}
	if(T.nodes[i+1].parent==T.nodes[i].parent) //判断是不是兄弟
		return T.nodes[i+1].data;
	printf("\n\t没有找到%c的右兄弟结点，说明值为%c的结点没有右兄弟！\n",value,value);
	return NULL;
}

//附加函数：用于显示树的所有内容
//初始条件：树T存在；操作结果：将树T的所有结点显示出
Status Print(PTree T)
{ 
	int i;
	if(T.nodeNum==0){
	printf("\n\t树为空！结点数为0！");
	return OK;
	}
	printf("\n所建立的树的结点个数: %d\n",T.nodeNum);
	printf("********************************************************************\n");
	printf(" 结点序号\t结点\t  双亲位置\t       双亲\n");
	for(i=1;i<=T.nodeNum;i++)
	{ 
	printf("%5d",i);//结点序号
	printf("   ----->  %c",Value(T,i)); //结点 
	printf("   ----->%5d",T.nodes[i-1].parent);//结点的双亲的位置
	if(T.nodes[i-1].parent<0) printf("   ----->  此结点为根结点，没有双亲");
	if(T.nodes[i-1].parent>=0) // 有双亲 
		printf("   ----->\t%c",Value(T,T.nodes[i-1].parent+1)); // 双亲 
	printf("\n");
	}
	printf("********************************************************************\n");
	return OK;
}

//初始条件：树T存在，p指向T中某个结点，1<=i<=p所指结点的度+1，非空树Tree1与T不相交
//操作结果：插入Tree1为T中p指结点的第i棵子树
Status InsertChild(PTree &T,TElemType p,int i,PTree Tree1)
{
	PTNode temp;//临时变量，用于交换两个结点的附加变量
	int j,k,inposition,childnum;
	int flag;
	if(i<=0) {
	printf("\n\t插入非空树Tree1的位置，第%d结点位置有错误！",i);
	return ERROR;}//i的数据有错
	if(Tree1.nodeNum==0){
	printf("\n\t插入的树Tree1为空，树T未变！");
	return ERROR;
	}
	if(T.nodeNum!=0)
	{
		for(j=0;j<T.nodeNum;j++)
			if(T.nodes[j].data==p)break;//找出值为p的结点T.nodes[j]
		if(j>=T.nodeNum) {
		printf("\n\t树中没有值为%c的结点，错误！",p);
		return ERROR;
		}
		//找出插入点inposition
		if(i==1) //i为1时，插入点为p的第一棵子树
		{
			for(k=j+1;k<T.nodeNum;k++)//找出p的第一棵子树T.nodes[k]
				if(T.nodes[k].parent==j)break;
			if(k>=T.nodeNum)
			 	inposition=j+1;//如果p没有子树，则插入点为j+1
			else 
				inposition=k;//找到p的第一棵子树，则插入点为k
		}
		else //插入的不是p的第一棵子树，求插入点
		{
			for(k=j+1,childnum=0;k<T.nodeNum;k++)//往后查找到p的第i棵子树的位置，查找前孩子数为0
				if(T.nodes[k].parent==j){
					childnum++;//孩子数加1
					if(childnum==i-1)//孩子数为i-1，则得到inposition为k+1
					break;
				}
				inposition=k+1;
		}
		if(inposition>=T.nodeNum){
			printf("\n\t插入非空树Tree1的位置，第%d结点位置有错误！",i);
			return ERROR;
			}
		//将非空树Tree1插入T中，首先要后移出Tree1.nodeNum个结点数，当插入点在T的最后结点后，则不用后移
		if(inposition<T.nodeNum)
			for(k=T.nodeNum-1;k>=inposition;k--)
			{
				T.nodes[k+Tree1.nodeNum]=T.nodes[k];//结点的信息后移
				if(T.nodes[k].parent>=inposition)//结点的双亲大于插入点，双亲域要加Tree1.nodeNum
					T.nodes[k+Tree1.nodeNum].parent+=Tree1.nodeNum;
			}
			//将非空树Tree1插入到插入点处
			for(k=0;k<Tree1.nodeNum;k++){
				T.nodes[k+inposition].data=Tree1.nodes[k].data;
				T.nodes[k+inposition].parent=Tree1.nodes[k].parent+inposition;
			}
			T.nodes[inposition].parent=j;/*插入到插入点处的结点的双亲应该为j*/
			T.nodeNum+=Tree1.nodeNum;
			//将插入子树后的结点按层序排序，采用冒泡排序的方式
			flag=1;
			while(flag){	
				flag=0;
				for(k=inposition;k<T.nodeNum-1;k++)
					if(T.nodes[k].parent>T.nodes[k+1].parent){
						temp=T.nodes[k];
						T.nodes[k]=T.nodes[k+1];
						T.nodes[k+1]=temp;
						flag=1;
					}
			}
			return OK;
	}
	else {
	printf("\n\t树为空，错误！");
	return ERROR;
	}
}

//初始条件：树T存在，p指向T中某个结点，1<=i<=p指结点的度
//操作结果：删除T中p所指向的第i棵子树
Status DeleteChild(PTree &T,TElemType p,int i)
{
	int j,k,childnum=0,deposition;//childnum为记录孩子数，deposition为第一删除结点
	int deleteflag[MAX_TREE_SIZE];//删除结点的标志，为1即是要删除的，为0即是不用删除的
	LinkQueue Q;
	QElemType q1,q2;
	if(i<=0) {//i的数据有错
	printf("\n\t删除非空树Tree1的位置，第%d结点位置有错误(i>=1)！",i);
	return ERROR;
	}
	if(i>=T.nodeNum){
	printf("\n\t删除非空树Tree1的位置，要删除的%d结点超出树T的范围！",i);
	return ERROR;
	}
	for(j=0;j<MAX_TREE_SIZE;j++)//删除标志置为0
		deleteflag[j]=0;
	if(T.nodeNum!=0){
		for(j=0;j<T.nodeNum;j++)//找出值为p的结点T.nodes[j]
			if(T.nodes[j].data==p)break;
		if(j>=T.nodeNum){
		printf("\n\t树中没有值为%c的结点，错误！",p);
		return ERROR;
		}
		//找出删除点deposition
		for(k=j+1;k<T.nodeNum;k++){
			if(T.nodes[k].parent==j)
				childnum++;//孩子数加1
			if(childnum==i)
			break;
		}
		deposition=k;
		if(deposition>=T.nodeNum){//删除点超出范围
		printf("\n\t树中值为%c的结点不存在第%d棵子树，没法删除，错误！",p,i);
		return ERROR;
		}
		//删除点不超出范围
		InitQueue(Q);//初始化队列
		q1.data=T.nodes[deposition].data;
		q1.num=deposition;
		EnQueue(Q,q1);//将第一个要删除的结点入队列
		deleteflag[deposition]=1;//将删除标志置为1
		k=deposition;
		while(!QueueEmpty(Q)){
			DeQueue(Q,q2);//出队列
			for(k=q2.num+1;k<T.nodeNum;k++)//如果结点的双亲域和出队列的q2.num相等
				if(q2.num==T.nodes[k].parent){
					q1.num=k;
					q1.data=T.nodes[k].data;
					EnQueue(Q,q1);//结点入队列
					deleteflag[k]=1;//删除标志置为1
				}
		}
		for(k=deposition;k<T.nodeNum;k++)//根据结点的删除标志来删除结点，删除即是前移结点位置
			if(deleteflag[k]==1){   
				for(j=k;j<T.nodeNum;j++){//前移结点   
					deleteflag[j]=deleteflag[j+1];//删除标志前移
					T.nodes[j]=T.nodes[j+1];//结点前移
					if(T.nodes[j-1].parent>k)//结点的双亲大于要删除的结点的位置，减1
						T.nodes[j-1].parent--;
				}
				k--;//k减1，倒回去继续检查删除标志
				T.nodeNum--;//结点数减1
			}
			return OK;
	}
	else
	{
		printf("\t树为空，不能删除子树！错误");
		return ERROR;
	}
}

//初始条件：树T存在，visit是对结点操作的应用函
//操作结果：按层次次序对树T的每一个结点调用函数visit()且至多一次
Status TraverseTree(PTree T,void(*visit)(TElemType))
{
	int i;
	printf("\n\t树按照层次遍历的顺序为：");
	for(i=0;i<T.nodeNum;i++)
		visit(T.nodes[i].data);
	return OK;
}
//访问对结点操作的应用函数
void visit(TElemType value)
{
	printf("%c",value);
}

/***********************************************************************************/
//主菜单函数
char menu()
{
	system("cls");//清屏
	printf("\t+===============================================================+\n");
	printf("\t|**********             网络工程1班张丽蓉             **********|\n");
	printf("\t+===============================================================+\n");
	printf("\t|                            菜单栏                             |\n"); 
	printf("\t+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n"); 
	printf("\t|                    A-----创建一棵空树                         |\n"); 
	printf("\t|                    B-----销毁树                               |\n"); 
	printf("\t|                    C-----将清为空树                           |\n");
	printf("\t|                    D-----判断树是否为空                       |\n"); 
	printf("\t|                    E-----返回树的深度                         |\n"); 
	printf("\t|                    F-----返回树的根节点                       |\n");
	printf("\t|                    G-----返回树中某个结点的值                 |\n"); 
	printf("\t|                    H-----将树中的某个结点赋值                 |\n");
	printf("\t|                    I-----求树中的某个结点的双亲               |\n");
	printf("\t|                    J-----求树中的某个结点的最左孩子           |\n");
	printf("\t|                    K-----求树中的某个结点的右兄弟             |\n");
	printf("\t|                    L-----在某个结点下插入一棵子树             |\n"); 
	printf("\t|                    M-----删除某个结点的子树                   |\n");
	printf("\t|                    N-----显示树                               |\n");
	printf("\t|                    O-----层次遍历树                           |\n");
	printf("\t|                    P-----退出                                 |\n"); 
	printf("\t+===============================================================+\n");
	printf("\t请选择菜单号(A---P):"); 
	return(getche());
}
/***********************************************************************************/
//主函数
int main() 
{
	int i;
	PTree T,Tree1;
	TElemType e,e1;
	InitTree(T);//初始化树 
	CreateTree(T);//创建一棵树 
	Print(T);//显示 创建的树的所有内容 
	printf("\n\t树创建成功！");
	getchar();
	while(1)
	{
		system("cls");//清屏 
		switch(menu())
		{
		case 'a':
		case 'A'://创建一棵树 
			ClearTree(T);
			printf("\n\t重新创建一棵新树：\n");
			CreateTree(T);
			getch();
			system("cls");
			Print(T);
			printf("\n\t树创建成功！");
			getche();break;
		case 'b':
		case 'B'://销毁树 
			DestroyTree(T);
			printf("\n\t树已被销毁!\n");
			getche();
			break;
		case 'c': 
		case 'C'://将树清空
			ClearTree(T);
			printf("\n\t树已被清为空树!\n");
			getche();
			break;
		case 'd':
		case 'D'://判断树是否为空 
			if(TreeEmpty(T)) 
				printf("\n\t树为空！");
			else
				printf("\n\t树不为空！");
			getche();
			break;
		case 'e':
		case 'E'://求树的深度 
			printf("\n\t树的深度为：%d",TreeDepth(T));
			getche();
			break;
		case 'f':
		case 'F'://求树的根结点 
			e=Root(T);
			if(e!=NIL)
				printf("\n\t树的根结点为：%c",e);
			getche();
			break;
		case 'g':
		case 'G'://求树中某个结点的值 
			printf("\n\t输入你要得到第几个结点的值：");
			scanf("%d",&i);
			e=Value(T,i);
			if(e!=NIL)	printf("\n\t第%d个结点的值为：%c",i,e);
			getche();
			break;
		case 'h':
		case 'H'://将树中某个结点赋值 
			printf("\n\t输入你要赋值的结点的是第几个结点：");
			scanf("%d",&i);
			printf("\n\t输入你要赋值的值：");
			e=getche();
			if(Assign(T,i,e)) 
				printf("\n\t赋值成功！");
			else 
				printf("\n\t赋值失败！");
			getche();
			break;
		case 'i':
		case 'I'://求树中的某个结点的双亲
			printf("\n\t输入你要得到双亲的结点是第几个结点：");
			scanf("%d",&i);
			e=Parent(T,i);
			if(e!=NIL)
				printf("\n\t第%d个结点的双亲为：%c",i,e);
			getche();
			break;
		case 'j':
		case 'J'://求树中的某个结点的最左孩子
			printf("\n\t输入你要求其最左孩子的结点的值：");
			e=getche();
			e1=LeftChild(T,e);
			if(e1!=NIL)printf("\n\t值为%c的结点的最左孩子为：%c",e,e1);
			getche();
			break;
		case 'k':
		case 'K'://求树中的某个结点的右兄弟 
			printf("\n\t输入你要求其右兄弟的结点的值：");
			e=getche();
			e1=RightSibling(T,e);
			if(e1!=NIL)printf("\n\t值为%c的右兄弟为：%c",e,e1);
			getche();break;
		case 'l':
		case 'L'://在某个结点下边插入一棵子树 
			printf("\n\t插入一棵子树，创建子树：\n");
			ClearTree(Tree1);
			InitTree(Tree1);
			CreateTree(Tree1);
			Print(Tree1);
			printf("\n\t子树创建成功！");
			printf("\n\t输入要插入到哪个结点下：(输入结点的值)");
			e=getche();
			printf("\n\t输入要插入到结点%c的那棵子树下：",e);
			scanf("%d",&i);
			if(InsertChild(T,e,i,Tree1))
			{
				printf("\n\t插入子树成功！其插入后的树如下：");
				Print(T);
			}
			getche();
			break;
		case 'm':
		case 'M'://删除某个结点的子树
			printf("\n\t删除一棵子树，原来的树如下：");
			Print(T);
			printf("\n\t输入要删除哪个结点的子树：(输入结点的值)");
			e=getche();
			printf("\n\t输入要删除结点%c的第几棵子树：",e);
			scanf("%d",&i);
			if(DeleteChild(T,e,i))
			{
				printf("\n\t删除成功！删除后的树如下：");
				Print(T);
			}
			getche();
			break;
		case 'n':
		case 'N'://显示树
			printf("\n\t该树的显示情况如下:\n"); 
			Print(T);
			getche();
			break;
		case 'o':
		case 'O'://层次遍历树 
			TraverseTree(T,visit);
			getche();
			break;
		case 'p':
		case 'P'://退出 
			printf("\n\t请按任意键退出!\n");
			getche();
			exit(1);
		default :
			printf("\n\t输入选择错误！请重新输入！");
			getche();
			break;
		}	
	}
}
/***********************************************************************************/


