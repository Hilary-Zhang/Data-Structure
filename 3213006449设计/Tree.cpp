//����ͷ�ļ��Լ��궨��
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
#define MAX_TREE_SIZE 100//������� 
/***********************************************************************************/
//����˫�ױ�洢�ṹ
typedef char TElemType;//�����洢����ϢΪ�ַ���
typedef int Status;
typedef struct PTNode{ //���ṹ
	TElemType data;//Ԫ��ֵ 
	int parent; //˫��λ��,������parentΪ-1 
}PTNode;

typedef struct{  //���ṹ
	PTNode nodes[MAX_TREE_SIZE];//�ɳ�ʼ����������Ľ������ 
	int root,nodeNum;  //����λ�úͽ���� 
}PTree;
/***********************************************************************************/
//���ĸ������нṹ
typedef struct
{
	int num;//����λ��
	TElemType data;//�����Ϣ
}QElemType; //�������Ԫ������ 

//�������У������е���ʽ�洢�ṹ 
typedef struct LQNode
{
	QElemType data;
	struct LQNode *next;
}LQNode,*QueuePtr;//���ͽ��ָ������ 

typedef struct
{
	QueuePtr front,rear; //��ͷ����βָ��
}LinkQueue;//���������� 
/***********************************************************************************/
//���еĻ�������
// ����һ���ն���Q
Status InitQueue(LinkQueue &Q)
{ 
	Q.front=Q.rear=(LQNode*)malloc(sizeof(LQNode));
	if(!Q.front)
		exit(OVERFLOW);
	Q.front->next=NULL;
	return OK;
}
//���ٶ���Q
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
//��Q��Ϊ�ն��� 
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
//��QΪ�ն���,�򷵻�TRUE,���򷵻�FALSE 
Status QueueEmpty(LinkQueue Q)
{ 
	if(Q.front==Q.rear)
		return TRUE;
	else
		return FALSE;
}
//����Ԫ��eΪQ���µĶ�βԪ�� 
Status EnQueue(LinkQueue &Q,QElemType e)
{ 
	LQNode *p;
	p=(LQNode*)malloc(sizeof(LQNode));
	if(!p) //�洢����ʧ�� 
		return OVERFLOW;
	p->data=e;
	p->next=NULL;
	Q.rear->next=p;
	Q.rear=p;
	return OK;
}
//�����в���,ɾ��Q�Ķ�ͷԪ��,��e������ֵ,������OK,���򷵻�ERROR 
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
//���Ļ�������
//����������������T
Status InitTree(PTree &T)
{
	T.nodeNum=0;//���Ľ������Ϊ0
	return OK;
}

//��ʼ��������T���ڣ����������������T
void DestroyTree(PTree &T)
{
	T.nodeNum=0;//����˫�ױ�洢�ṹ���ö������������ͣ��޷�����
}

//��ʼ����������T���ڣ����������������T
Status CreateTree(PTree &T)
{
	LinkQueue Q;
	QElemType q,t;
	char temp[MAX_TREE_SIZE];
	int i=1,j,len;
	InitQueue(Q);
	printf("\t���������ĸ����(���������'#'�ַ��Ļ������Ϊ��):");
	T.nodes[0].data=getche();
	printf("\n");
	if(T.nodes[0].data!=NIL)//����㲻Ϊ'#'��������Ϊ��
	{
		T.nodes[0].parent=-1;
		q.data=T.nodes[0].data;
		q.num=0;
		EnQueue(Q,q);//�����
		while(i<MAX_TREE_SIZE&&!QueueEmpty(Q))
		{
			DeQueue(Q,q);
		//��������q.data�ĺ��ӽ�㣬��ʱ���浽temp[MAX_TREE_SIZE]��
		//���ﻹҪ������ô�������봴����
			printf("\t������%c�����к��ӽ��(���û�к��ӣ����س���������һ�����ĺ���):",q.data);
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
		if(i>=MAX_TREE_SIZE)//������Χ
			exit(OVERFLOW);
		T.nodeNum=i;
	}
	else 
	T.nodeNum=0;
	return OK;
}

//��ʼ��������T���ڣ����������������Ϊ����
Status ClearTree(PTree &T)
{
	T.nodeNum=0;
	return OK;
}

//��ʼ��������T���ڣ������������TΪ�������򷵻�TRUE�����򷵻�FALSE
Status TreeEmpty(PTree T)
{
	if(T.nodeNum==0) 
		return TRUE;
	else 
		return FALSE;
}

//��ʼ��������T���ڣ��������������T�����
Status TreeDepth(PTree T)
{
	int depthmax=0,depth,i,parent;
	//depthmax������������ȣ�depth���ڱ������ý�����ȣ�parent��ʾ˫�׵�λ��
	for(i=0;i<T.nodeNum;i++)
	{
		depth=1;
		parent=T.nodes[i].parent;
		while(parent!=-1)//��T.nodes[i]����˻ظ����,��һ������ȼ�1
		{
			parent=T.nodes[parent].parent;
			depth++;   
		}
		if(depthmax<depth) depthmax=depth;
	}
	return depthmax;
}

//��ʼ��������T���ڣ��������������T�ĸ�
TElemType Root(PTree T)
{
	if(T.nodeNum)
		return T.nodes[0].data;
	else {
	printf("\n\t��Ϊ�գ�û�и����!");
	return NIL;
	}
}

//��ʼ��������T���ڣ�position��T�е�ĳ������λ�ã�
//������������ص�position����ֵ,��position������ԭ�еĽ���������ش�����Ϣ
TElemType Value(PTree T,int position)
{
	if(position<=T.nodeNum) 
		return T.nodes[position-1].data;
	else
	{
		printf("\n\t�����ĵ�%d��㳬����ԭ�еĽ����������\n",position);
		return NIL;
	}
}

//��ʼ��������T���ڣ�position��T��ĳ������λ�ã�
//�������������position������ֵ��ֵΪvalue
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

//��ʼ��������T���ڣ�position��T��ĳ������λ��
//�������������position�������T�ķǸ���㣬�򷵻�����˫�ף�������ֵΪ��
TElemType Parent(PTree T,int position)
{
	if(position==1) 
	{		
		printf("\n\t�����ĵ�%d���Ϊ���ĸ���㣬�����û��˫�ף�\n",position);
		return NIL;
	}
	else if(position<=T.nodeNum) 
		return T.nodes[T.nodes[position-1].parent].data;
	else 
	{
		printf("\n\t�����ĵ�%d��㳬����ԭ�еĽ����������\n",position);
		return NIL;
	}
}

//��ʼ��������T���ڣ�value��T��ĳ������ֵ
//�����������value��T�ķ�Ҷ�ӽ�㣬�򷵻����������ӣ����򷵻ؿ�
TElemType LeftChild(PTree T,TElemType value)
{
	int i,j;
	for(i=0;i<T.nodeNum;i++)
		if(T.nodes[i].data==value)
		 break;//�ҵ�ֵΪvalue�Ľ��T.nodes[i]
	if(i>=T.nodeNum) {
		printf("\n\t���в�����ֵΪ%c�Ľ�㣡����",value);
		return NIL;
	}
	for(j=i+1;j<T.nodeNum;j++)
		if(T.nodes[j].parent==i) 
		return T.nodes[j].data;
	if(j>=T.nodeNum) { 
		printf("\n\tû���ҵ�%c�������ӽ�㣬˵��ֵΪ%c�Ľ��û�к��ӣ�",value,value);
		return NIL;
	}
}

//��ʼ��������T���ڣ�value��T��ĳ������ֵ
//�����������value�����ֵܣ��򷵻��������ֵܣ�������ֵΪ��
TElemType RightSibling(PTree T,TElemType value)
{
	int i;
	for(i=0;i<T.nodeNum;i++)//�ҳ�ֵΪvalue�Ľ��T.nodes[i]
		if(T.nodes[i].data==value)
			break;
	if(i>=T.nodeNum) {
		printf("\n\t���в�����ֵΪ%c�Ľ�㣡\n",value);
		return NULL;
	}
	if(T.nodes[i+1].parent==T.nodes[i].parent) //�ж��ǲ����ֵ�
		return T.nodes[i+1].data;
	printf("\n\tû���ҵ�%c�����ֵܽ�㣬˵��ֵΪ%c�Ľ��û�����ֵܣ�\n",value,value);
	return NULL;
}

//���Ӻ�����������ʾ������������
//��ʼ��������T���ڣ��������������T�����н����ʾ��
Status Print(PTree T)
{ 
	int i;
	if(T.nodeNum==0){
	printf("\n\t��Ϊ�գ������Ϊ0��");
	return OK;
	}
	printf("\n�����������Ľ�����: %d\n",T.nodeNum);
	printf("********************************************************************\n");
	printf(" ������\t���\t  ˫��λ��\t       ˫��\n");
	for(i=1;i<=T.nodeNum;i++)
	{ 
	printf("%5d",i);//������
	printf("   ----->  %c",Value(T,i)); //��� 
	printf("   ----->%5d",T.nodes[i-1].parent);//����˫�׵�λ��
	if(T.nodes[i-1].parent<0) printf("   ----->  �˽��Ϊ����㣬û��˫��");
	if(T.nodes[i-1].parent>=0) // ��˫�� 
		printf("   ----->\t%c",Value(T,T.nodes[i-1].parent+1)); // ˫�� 
	printf("\n");
	}
	printf("********************************************************************\n");
	return OK;
}

//��ʼ��������T���ڣ�pָ��T��ĳ����㣬1<=i<=p��ָ���Ķ�+1���ǿ���Tree1��T���ཻ
//�������������Tree1ΪT��pָ���ĵ�i������
Status InsertChild(PTree &T,TElemType p,int i,PTree Tree1)
{
	PTNode temp;//��ʱ���������ڽ����������ĸ��ӱ���
	int j,k,inposition,childnum;
	int flag;
	if(i<=0) {
	printf("\n\t����ǿ���Tree1��λ�ã���%d���λ���д���",i);
	return ERROR;}//i�������д�
	if(Tree1.nodeNum==0){
	printf("\n\t�������Tree1Ϊ�գ���Tδ�䣡");
	return ERROR;
	}
	if(T.nodeNum!=0)
	{
		for(j=0;j<T.nodeNum;j++)
			if(T.nodes[j].data==p)break;//�ҳ�ֵΪp�Ľ��T.nodes[j]
		if(j>=T.nodeNum) {
		printf("\n\t����û��ֵΪ%c�Ľ�㣬����",p);
		return ERROR;
		}
		//�ҳ������inposition
		if(i==1) //iΪ1ʱ�������Ϊp�ĵ�һ������
		{
			for(k=j+1;k<T.nodeNum;k++)//�ҳ�p�ĵ�һ������T.nodes[k]
				if(T.nodes[k].parent==j)break;
			if(k>=T.nodeNum)
			 	inposition=j+1;//���pû��������������Ϊj+1
			else 
				inposition=k;//�ҵ�p�ĵ�һ��������������Ϊk
		}
		else //����Ĳ���p�ĵ�һ��������������
		{
			for(k=j+1,childnum=0;k<T.nodeNum;k++)//������ҵ�p�ĵ�i��������λ�ã�����ǰ������Ϊ0
				if(T.nodes[k].parent==j){
					childnum++;//��������1
					if(childnum==i-1)//������Ϊi-1����õ�inpositionΪk+1
					break;
				}
				inposition=k+1;
		}
		if(inposition>=T.nodeNum){
			printf("\n\t����ǿ���Tree1��λ�ã���%d���λ���д���",i);
			return ERROR;
			}
		//���ǿ���Tree1����T�У�����Ҫ���Ƴ�Tree1.nodeNum������������������T�����������ú���
		if(inposition<T.nodeNum)
			for(k=T.nodeNum-1;k>=inposition;k--)
			{
				T.nodes[k+Tree1.nodeNum]=T.nodes[k];//������Ϣ����
				if(T.nodes[k].parent>=inposition)//����˫�״��ڲ���㣬˫����Ҫ��Tree1.nodeNum
					T.nodes[k+Tree1.nodeNum].parent+=Tree1.nodeNum;
			}
			//���ǿ���Tree1���뵽����㴦
			for(k=0;k<Tree1.nodeNum;k++){
				T.nodes[k+inposition].data=Tree1.nodes[k].data;
				T.nodes[k+inposition].parent=Tree1.nodes[k].parent+inposition;
			}
			T.nodes[inposition].parent=j;/*���뵽����㴦�Ľ���˫��Ӧ��Ϊj*/
			T.nodeNum+=Tree1.nodeNum;
			//������������Ľ�㰴�������򣬲���ð������ķ�ʽ
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
	printf("\n\t��Ϊ�գ�����");
	return ERROR;
	}
}

//��ʼ��������T���ڣ�pָ��T��ĳ����㣬1<=i<=pָ���Ķ�
//���������ɾ��T��p��ָ��ĵ�i������
Status DeleteChild(PTree &T,TElemType p,int i)
{
	int j,k,childnum=0,deposition;//childnumΪ��¼��������depositionΪ��һɾ�����
	int deleteflag[MAX_TREE_SIZE];//ɾ�����ı�־��Ϊ1����Ҫɾ���ģ�Ϊ0���ǲ���ɾ����
	LinkQueue Q;
	QElemType q1,q2;
	if(i<=0) {//i�������д�
	printf("\n\tɾ���ǿ���Tree1��λ�ã���%d���λ���д���(i>=1)��",i);
	return ERROR;
	}
	if(i>=T.nodeNum){
	printf("\n\tɾ���ǿ���Tree1��λ�ã�Ҫɾ����%d��㳬����T�ķ�Χ��",i);
	return ERROR;
	}
	for(j=0;j<MAX_TREE_SIZE;j++)//ɾ����־��Ϊ0
		deleteflag[j]=0;
	if(T.nodeNum!=0){
		for(j=0;j<T.nodeNum;j++)//�ҳ�ֵΪp�Ľ��T.nodes[j]
			if(T.nodes[j].data==p)break;
		if(j>=T.nodeNum){
		printf("\n\t����û��ֵΪ%c�Ľ�㣬����",p);
		return ERROR;
		}
		//�ҳ�ɾ����deposition
		for(k=j+1;k<T.nodeNum;k++){
			if(T.nodes[k].parent==j)
				childnum++;//��������1
			if(childnum==i)
			break;
		}
		deposition=k;
		if(deposition>=T.nodeNum){//ɾ���㳬����Χ
		printf("\n\t����ֵΪ%c�Ľ�㲻���ڵ�%d��������û��ɾ��������",p,i);
		return ERROR;
		}
		//ɾ���㲻������Χ
		InitQueue(Q);//��ʼ������
		q1.data=T.nodes[deposition].data;
		q1.num=deposition;
		EnQueue(Q,q1);//����һ��Ҫɾ���Ľ�������
		deleteflag[deposition]=1;//��ɾ����־��Ϊ1
		k=deposition;
		while(!QueueEmpty(Q)){
			DeQueue(Q,q2);//������
			for(k=q2.num+1;k<T.nodeNum;k++)//�������˫����ͳ����е�q2.num���
				if(q2.num==T.nodes[k].parent){
					q1.num=k;
					q1.data=T.nodes[k].data;
					EnQueue(Q,q1);//��������
					deleteflag[k]=1;//ɾ����־��Ϊ1
				}
		}
		for(k=deposition;k<T.nodeNum;k++)//���ݽ���ɾ����־��ɾ����㣬ɾ������ǰ�ƽ��λ��
			if(deleteflag[k]==1){   
				for(j=k;j<T.nodeNum;j++){//ǰ�ƽ��   
					deleteflag[j]=deleteflag[j+1];//ɾ����־ǰ��
					T.nodes[j]=T.nodes[j+1];//���ǰ��
					if(T.nodes[j-1].parent>k)//����˫�״���Ҫɾ���Ľ���λ�ã���1
						T.nodes[j-1].parent--;
				}
				k--;//k��1������ȥ�������ɾ����־
				T.nodeNum--;//�������1
			}
			return OK;
	}
	else
	{
		printf("\t��Ϊ�գ�����ɾ������������");
		return ERROR;
	}
}

//��ʼ��������T���ڣ�visit�ǶԽ�������Ӧ�ú�
//�������������δ������T��ÿһ�������ú���visit()������һ��
Status TraverseTree(PTree T,void(*visit)(TElemType))
{
	int i;
	printf("\n\t�����ղ�α�����˳��Ϊ��");
	for(i=0;i<T.nodeNum;i++)
		visit(T.nodes[i].data);
	return OK;
}
//���ʶԽ�������Ӧ�ú���
void visit(TElemType value)
{
	printf("%c",value);
}

/***********************************************************************************/
//���˵�����
char menu()
{
	system("cls");//����
	printf("\t+===============================================================+\n");
	printf("\t|**********             ���繤��1��������             **********|\n");
	printf("\t+===============================================================+\n");
	printf("\t|                            �˵���                             |\n"); 
	printf("\t+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n"); 
	printf("\t|                    A-----����һ�ÿ���                         |\n"); 
	printf("\t|                    B-----������                               |\n"); 
	printf("\t|                    C-----����Ϊ����                           |\n");
	printf("\t|                    D-----�ж����Ƿ�Ϊ��                       |\n"); 
	printf("\t|                    E-----�����������                         |\n"); 
	printf("\t|                    F-----�������ĸ��ڵ�                       |\n");
	printf("\t|                    G-----��������ĳ������ֵ                 |\n"); 
	printf("\t|                    H-----�����е�ĳ����㸳ֵ                 |\n");
	printf("\t|                    I-----�����е�ĳ������˫��               |\n");
	printf("\t|                    J-----�����е�ĳ������������           |\n");
	printf("\t|                    K-----�����е�ĳ���������ֵ�             |\n");
	printf("\t|                    L-----��ĳ������²���һ������             |\n"); 
	printf("\t|                    M-----ɾ��ĳ����������                   |\n");
	printf("\t|                    N-----��ʾ��                               |\n");
	printf("\t|                    O-----��α�����                           |\n");
	printf("\t|                    P-----�˳�                                 |\n"); 
	printf("\t+===============================================================+\n");
	printf("\t��ѡ��˵���(A---P):"); 
	return(getche());
}
/***********************************************************************************/
//������
int main() 
{
	int i;
	PTree T,Tree1;
	TElemType e,e1;
	InitTree(T);//��ʼ���� 
	CreateTree(T);//����һ���� 
	Print(T);//��ʾ ������������������ 
	printf("\n\t�������ɹ���");
	getchar();
	while(1)
	{
		system("cls");//���� 
		switch(menu())
		{
		case 'a':
		case 'A'://����һ���� 
			ClearTree(T);
			printf("\n\t���´���һ��������\n");
			CreateTree(T);
			getch();
			system("cls");
			Print(T);
			printf("\n\t�������ɹ���");
			getche();break;
		case 'b':
		case 'B'://������ 
			DestroyTree(T);
			printf("\n\t���ѱ�����!\n");
			getche();
			break;
		case 'c': 
		case 'C'://�������
			ClearTree(T);
			printf("\n\t���ѱ���Ϊ����!\n");
			getche();
			break;
		case 'd':
		case 'D'://�ж����Ƿ�Ϊ�� 
			if(TreeEmpty(T)) 
				printf("\n\t��Ϊ�գ�");
			else
				printf("\n\t����Ϊ�գ�");
			getche();
			break;
		case 'e':
		case 'E'://��������� 
			printf("\n\t�������Ϊ��%d",TreeDepth(T));
			getche();
			break;
		case 'f':
		case 'F'://�����ĸ���� 
			e=Root(T);
			if(e!=NIL)
				printf("\n\t���ĸ����Ϊ��%c",e);
			getche();
			break;
		case 'g':
		case 'G'://������ĳ������ֵ 
			printf("\n\t������Ҫ�õ��ڼ�������ֵ��");
			scanf("%d",&i);
			e=Value(T,i);
			if(e!=NIL)	printf("\n\t��%d������ֵΪ��%c",i,e);
			getche();
			break;
		case 'h':
		case 'H'://������ĳ����㸳ֵ 
			printf("\n\t������Ҫ��ֵ�Ľ����ǵڼ�����㣺");
			scanf("%d",&i);
			printf("\n\t������Ҫ��ֵ��ֵ��");
			e=getche();
			if(Assign(T,i,e)) 
				printf("\n\t��ֵ�ɹ���");
			else 
				printf("\n\t��ֵʧ�ܣ�");
			getche();
			break;
		case 'i':
		case 'I'://�����е�ĳ������˫��
			printf("\n\t������Ҫ�õ�˫�׵Ľ���ǵڼ�����㣺");
			scanf("%d",&i);
			e=Parent(T,i);
			if(e!=NIL)
				printf("\n\t��%d������˫��Ϊ��%c",i,e);
			getche();
			break;
		case 'j':
		case 'J'://�����е�ĳ������������
			printf("\n\t������Ҫ���������ӵĽ���ֵ��");
			e=getche();
			e1=LeftChild(T,e);
			if(e1!=NIL)printf("\n\tֵΪ%c�Ľ���������Ϊ��%c",e,e1);
			getche();
			break;
		case 'k':
		case 'K'://�����е�ĳ���������ֵ� 
			printf("\n\t������Ҫ�������ֵܵĽ���ֵ��");
			e=getche();
			e1=RightSibling(T,e);
			if(e1!=NIL)printf("\n\tֵΪ%c�����ֵ�Ϊ��%c",e,e1);
			getche();break;
		case 'l':
		case 'L'://��ĳ������±߲���һ������ 
			printf("\n\t����һ������������������\n");
			ClearTree(Tree1);
			InitTree(Tree1);
			CreateTree(Tree1);
			Print(Tree1);
			printf("\n\t���������ɹ���");
			printf("\n\t����Ҫ���뵽�ĸ�����£�(�������ֵ)");
			e=getche();
			printf("\n\t����Ҫ���뵽���%c���ǿ������£�",e);
			scanf("%d",&i);
			if(InsertChild(T,e,i,Tree1))
			{
				printf("\n\t���������ɹ���������������£�");
				Print(T);
			}
			getche();
			break;
		case 'm':
		case 'M'://ɾ��ĳ����������
			printf("\n\tɾ��һ��������ԭ���������£�");
			Print(T);
			printf("\n\t����Ҫɾ���ĸ�����������(�������ֵ)");
			e=getche();
			printf("\n\t����Ҫɾ�����%c�ĵڼ���������",e);
			scanf("%d",&i);
			if(DeleteChild(T,e,i))
			{
				printf("\n\tɾ���ɹ���ɾ����������£�");
				Print(T);
			}
			getche();
			break;
		case 'n':
		case 'N'://��ʾ��
			printf("\n\t��������ʾ�������:\n"); 
			Print(T);
			getche();
			break;
		case 'o':
		case 'O'://��α����� 
			TraverseTree(T,visit);
			getche();
			break;
		case 'p':
		case 'P'://�˳� 
			printf("\n\t�밴������˳�!\n");
			getche();
			exit(1);
		default :
			printf("\n\t����ѡ��������������룡");
			getche();
			break;
		}	
	}
}
/***********************************************************************************/


