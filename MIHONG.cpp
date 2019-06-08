#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <iostream>
#define  TRUE 1
#define  FALSE   0
#define  OK  1
#define  ERROR   0
#define  INFEASIBLE   -1
#define  OVERFLOW   -2
#define  M 35
#define  N 35
using namespace std;
typedef  int  Status;

typedef struct
{
	int x,y;  //�Թ���λ�õ�����
}PosType;

typedef struct
{
	int m,n;
	char Array[M][N];  //�ö�ά�����ʾ�Թ�
}MazeType;

typedef struct
{
	int ord;     //ͨ������·���ϵġ���š�
	PosType seat; //ͨ�������Թ��еġ�����λ�á�
	int di;       //�Ӵ�ͨ���������¸�ͨ����ġ�����
}ElemType;        //ջ��Ԫ������

typedef struct NodeType
{
	ElemType data;
	struct NodeType *next;
}NodeType,*LinkType;    //ȷ��ջ�Ĵ洢�ṹ����ʽ��

typedef struct
{
	LinkType top;       //��ջ�Ķ��㶨��
	int size;
}Stack;

void   InitStack(Stack  &S)  //�½�һ����ջ
{
	S.top=NULL;
	S.size=0;
}

Status   MakeNode(LinkType &p,ElemType   e)  //����һ��ջ�Ľڵ�Ԫ��
{
	p=(NodeType *)malloc(sizeof(NodeType));
	if(!p)return   FALSE;
	p->data=e;
	p->next=NULL;
	return   TRUE;
}

Status   Push(Stack   &S,ElemType   e)   //��ջ����ջ����
{
	LinkType   p;
	if(MakeNode(p,e)){
		p->next=S.top;
		S.top=p;   //�޸�ջ��ָ�룬ʹ��ָ���²���Ľ��
		S.size++;    //��ջԪ�ؼ�1
		return   TRUE;
	}
	return   FALSE;
}

Status   StackEmpty(Stack   S)   //�ж�ջ�Ƿ�Ϊ��
{
	if(S.top==NULL)
		return   TRUE;
	return  FALSE;
}

Status   Pop(Stack   &S,ElemType   &e) //��ջ�ĳ�ջ����
{
	LinkType   p;
	if(StackEmpty(S))
		return   FALSE;
	else{
		p=S.top;
		S.top=S.top->next;  //��ջ��ָ��ָ��ǰջ����ָ����һ��Ԫ��
		e=p->data;
		S.size--;    //��ջԪ�ؼ�1
		free(p);
		return   TRUE;
	}
}

Status   Pass(MazeType   maze,PosType   curpos)
//����֪���Թ��͵�ǰ�������ߵ��Թ���λ���ж��Թ��Ƿ������
{
	if(maze.Array[curpos.x][curpos.y]==' ') //�����ǰ��ά����Ԫ��Ϊ�ո��������
		return   TRUE;
	return   FALSE;
}

Status   Same(PosType   curpos,PosType   end)
//�ɵ�ǰ�Թ�λ�������λ�ý��бȽ��ж��Ƿ��ߵ��Թ�����
{
	if(curpos.x==end.x && curpos.y==end.y)    //�жϵ�ǰλ�õ������Ƿ����������һ��
		return   TRUE;
	return   FALSE;
}

PosType   NextPos(PosType  curpos,int   di)
//�ɵ�ǰ�Թ�λ�úͷ����жϳ����߹������ߵ���һ���Թ�λ��
{
	switch(di){
        case 1:
            curpos.y++; //�������1
            break;
        case 2:
            curpos.x++; //�������1
            break;
        case 3:
            curpos.y--; //�������1
            break;
        case 4:
            curpos.x--; //�������1
            break;
	}
	return   curpos;
}

void   FootPrint(MazeType   &maze,PosType   curpos) //��ͨ��λ����-���
{
	maze.Array[curpos.x][curpos.y] = '-';
}

void  MarkPrint(MazeType &maze,PosType p)
{
	maze.Array[p.x][p.y]='x';   //�߹�����û����ͨ��λ���ñ��
}

void  PrintMaze(MazeType   ma)  //��ӡ������ɵ��Թ�
{
	int i,j;
	printf("\n");
	for(i=0;i<ma.m;i++)
	{
		printf("\t");
		for(j=0;j<ma.n;j++)
		{
			printf("%c ",ma.Array[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void   ChangMaze(MazeType  &maze,int  a[M][N],int row,int col) //��0,1����ת��Ϊ�Թ�
{
	int i,j;
	maze.m=row + 2;
	maze.n=col + 2;
	for(i = 0;i < row + 2; i++)
		for(j = 0;j < col + 2; j++){
			if(a[i][j]==0)
				maze.Array[i][j]=' ';   //�ո��ʾ·
			else
				maze.Array[i][j]='#';     //��#����ʾ�Թ����ǽ
		}

}
		
		
		


Status   MazePath(MazeType   &maze, PosType   in, PosType   out)
{//������֪���Թ�����ڣ�0,0���ͳ��ڣ�m,n������ǰ�������ж������ߵĻ���·��
	Stack   s;
	int   curstep=1;//̽����һ��
	int a;
	int b;
	int c;
	int i;
	Status   found=FALSE;
	ElemType   e;//����һ��elemtype���͵ı���������λ�򣬷��������
	PosType   curpos=in; //�趨��ǰλ��Ϊ���
	InitStack(s);
	do{
		if(Pass(maze,curpos)){   //��ǰλ�ÿ���ͨ��������δ���߹���ͨ����
			system("cls");   //����
			FootPrint(maze,curpos); //�����㼣

            e.ord=curstep;
            e.seat=curpos;
            e.di=1;//fxj(i)
			Push(s,e);  //����·��
			if(Same(curpos,out)){// �����յ�
				found=TRUE;
			}
			else{
				curpos=NextPos(curpos,1);//��һλ���ǵ�ǰλ�õ�C�����ڼ�
				curstep++;    //̽����һ��
			}
		}
		else   //��ǰλ�ò���ͨ��
			if(!StackEmpty(s)){
				Pop(s,e);
				while((e.di==4) && !StackEmpty(s))
				{
					system("cls");   //����
					MarkPrint(maze,e.seat);   //���²���ͨ���ı�ǣ����˻�һ��

					//PrintMaze(maze);
					Pop(s,e);
					curstep--;
				}
				if(e.di<4)
				{
					e.di++;
					Push(s,e);   //̽����һ����
					curpos=NextPos(e.seat,e.di);   //�趨��ǰλ���Ǹ��·����ϵ�����
				}
			}
	}while(!StackEmpty(s) && !found);  //�ж�ջ�Ƿ�Ϊ�գ��Ƿ񵽳���
	return   found;
}

Status fxj(int x)//�ɼ��̿����Թ����߷���
{
	switch( getch()){       // getch()�ӿ���̨�޻��Ե�ȡһ���ַ�
        case 13:    //�س���ֹ
            x = 5;
            break;
        case 27:    //Esc�˳�
            exit(0);
            break;
        case 68:  //��
            x = 1;
            break;
        case 83:  //��
            x = 2;
            break;
        case 65:  //��
            x = 3;
            break;
        case 87:  //��
            x = 4;
            break;
	}
	return x;
}

Status   ComputerPlay(MazeType   &maze, PosType   in, PosType   out)
{//������֪���Թ�����ڣ�0,0���ͳ��ڣ�m,n������ǰ�������ж������ߵĻ���·��
	Stack   s;
	int   curstep=1;//̽����һ��
	int a;
	int b;
	int c;
	int i;
	Status   found=FALSE;
	ElemType   e;//����һ��elemtype���͵ı���������λ�򣬷��������
	PosType   curpos=in; //�趨��ǰλ��Ϊ���
	InitStack(s);
	do{
		if(Pass(maze,curpos)){   //��ǰλ�ÿ���ͨ��������δ���߹���ͨ����
			system("cls");   //����
			printf("�������߹��̣�\n");
			FootPrint(maze,curpos); //�����㼣

			PrintMaze(maze);
            e.ord=curstep;
            e.seat=curpos;
            e.di=1;  //fxj(i)
			Push(s,e);  //����·��
			if(Same(curpos,out)){  // �����յ�
				found=TRUE;
			}
			else{
				curpos=NextPos(curpos,1);  //��һλ���ǵ�ǰλ�õ�C�����ڼ�
				curstep++;    //̽����һ��
			}
		}
		else   //��ǰλ�ò���ͨ��
			if(!StackEmpty(s)){
				Pop(s,e);
				while((e.di==4) && !StackEmpty(s))
				{
					system("cls");   //����
					printf("�������߹��̣�\n");
					MarkPrint(maze,e.seat);   //���²���ͨ���ı�ǣ����˻�һ��

					PrintMaze(maze);
					Pop(s,e);
					curstep--;
				}
				if(e.di<4)
				{
					e.di++;
					Push(s,e);   //̽����һ����
					curpos=NextPos(e.seat,e.di);   //�趨��ǰλ���Ǹ��·����ϵ�����
				}
			}
			//��ʱ��
            for(a=0;a<10000;a++)
                for(b=0;b<10000;b++);

	}while(!StackEmpty(s) && !found);  //�ж�ջ�Ƿ�Ϊ�գ��Ƿ񵽳���
	return   found;
}

Status player(MazeType   &maze,PosType   in,PosType   out)
{
	int c;
	int i;
	Status   found=FALSE;
	ElemType   e;//����һ��elemtype���͵ı���������λ�򣬷��������
	PosType   curpos=in; //�趨��ǰλ��Ϊ���

	do{
        if(maze.Array[curpos.x][curpos.y] != '#'){   //��ǰλ�������Թ���ǽ���򲻿���ͨ��

			system("cls");   //����
			printf("�Թ�����ͨ·,�������÷���������Թ��н�����\n");
			printf("�����������߹����а��س�����ֹ��Ϸ�����߰�ESC���˳���Ϸ\n");

			FootPrint(maze,curpos); //�����㼣
			PrintMaze(maze);

            e.seat=curpos;
			if(Same(curpos,out)){// �����յ�
				found=TRUE;
			}
			else{
			    rep:
                c = fxj(i);
                switch(c){
                    case 1: //��
                        curpos = NextPos(curpos,c);
                        break;
                    case 2: //��
                        curpos = NextPos(curpos,c);
                        break;
                    case 3: //��
                        curpos = NextPos(curpos,c);
                        break;
                    case 4: //��
                        curpos = NextPos(curpos,c);
                        break;
                    case 5:  //�˳�
                        goto xm;
                }
                if(maze.Array[curpos.x][curpos.y] == '-'){
                    MarkPrint(maze,e.seat);  //���²���ͨ���ı�ǣ����˻�һ��
                }
			}
		}
		else{
		    curpos = e.seat;
		    goto rep;
		}
	}while(!found);//�ж�ջ�Ƿ�Ϊ�գ��Ƿ񵽳���
	xm:return   found;
}

void Print(int maze[M][N],int m,int n)   //��ӡ0,1����������Թ�
{
	int i,j;
	printf("�������Թ����飺\n");
	for(i=0;i<m+2;i++)
	{
		printf("\t");
		for(j=0;j<n+2;j++)
		{
			printf("%d ",maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void shoudong_maze(int maze[M][N],int x,int y) //�ֶ������Թ�
{
	int i,j;
    int m=x;
	int n=y;//�Թ���,��
	printf("\n�������Թ��ĸ��и���:\n�ÿո����,0����·,1����ǽ�������벻�Ϸ���Ĭ��Ϊ1��\n");
	for(i=1;i<=m;i++)
		for(j=1;j<=n;j++)
		{
			scanf("%d",&maze[i][j]);
			if((maze[i][j]!=0)&&(maze[i][j]!=1)) maze[i][j]=1;
		}

		for(i=0;i<=m+1;i++) //��һȦΧǽ
		{
			maze[i][0]=1;
			maze[i][n+1]=1;
		}
		for(j=0;j<=n+1;j++)
		{
			maze[0][j]=1;
			maze[m+1][j]=1;
		}
}


void zidong_maze(int maze [M][N],int m,int n)   //�Զ���������Թ�
{
	srand( time( NULL ) );
	int i,j;
	printf("\n�Թ������С���\n\n");
	for(i=1;i<=m;i++)
		for(j=1;j<=n;j++)
			maze[i][j]=rand()%2;
        maze[1][1]=maze[m][n]=0;
		for(i=0;i<=m+1;i++){ //��һȦΧǽ
			maze[i][0]=1;
			maze[i][n+1]=1;
		}
		for(j=0;j<=n+1;j++){
			maze[0][j]=1;
			maze[m+1][j]=1;
		}
}

void input(int &m,int &n)
{
    mark:
    while(1){
        printf("�������Թ������� m=");
        fflush(stdin);   //����������뻺�棬�Ա㲻Ӱ���������Ķ���
        if(!scanf("%d",&m)) {
            printf("�����ַ�����������������!\n");
            continue;
        }
        else{
            break;}
    }

    while(1){
        printf("�������Թ������� n=");
        fflush(stdin);
        if(!scanf("%d",&n)){
            printf("�����ַ�����������������!\n");
            continue;
        }
        else{
            break;}
    }
    if((m<=0||m>25)||(n<=0||n>30)){
        printf("\n��Ǹ�������������������Ԥ�跶Χ(0-25,0-30),���������룺\n\n");
        goto mark;
    }
}

int main()
{
	int maze[M][N],m,n,i,kongzhi=0;
	MazeType L,R;    //�ö�ά�����ʾ�Թ�
    PosType   in,out;   //�Թ���λ�õ�����
	while(kongzhi!= -1 ){
		printf("\t    ******************��ӭ�����Թ���Ϸ****************\n");
		printf("\t    *              �ֶ������Թ�  �밴��1             *\n");
		printf("\t    *              �Զ������Թ�  �밴��2             *\n");
		printf("\t    *              ����          �밴��3             *\n");
		printf("\t    *              �˳�          �밴��4             *\n");
		printf("\t    **************************************************\n");
		printf("\n");
		printf("��ѡ����Ĳ�����");
		scanf("%d",&i);
		switch(i){
            case 1:
                input(m,n);
                shoudong_maze(maze,m,n);//�����Թ�
                Print(maze,m,n);//��ӡ���Թ�����
                ChangMaze(L,maze,m,n);
                in.x=1;   //ȷ���������
                in.y=1;
                out.x=m;  //ȷ����������
                out.y=n;
                R=L;
                printf("������ת�������Թ�");
                PrintMaze(L);

                printf("\n�����������!\n");
                getchar();
                while(getchar()!='\n');

                if(MazePath(L,in,out)){
                    int choice;
                    printf("���������ѡ��1�ɵ����ߣ�2���û��ߣ���");
                    scanf("%d",&choice);
                    if(choice==1){
                        ComputerPlay(R,in,out);    //�ɵ������Թ�
                    }
                    else{
                        if(player(R,in,out)){    //��������Թ�
                            printf("����ս�ɹ������߹���·��Ϊ��\n");
                            PrintMaze(R);
                        }
                        else
                            printf("��Ϸ������\n");
                    }
                }
                else
                    printf("���Թ�û��ͨ·��\n");
            printf("�����������!\n");
            getchar();
            while(getchar()!='\n');
            break;

            case 2:
                input(m,n);
                zd:
                zidong_maze(maze,m,n);
                ChangMaze(L,maze,m,n);
                in.x=1;   //ȷ���������
                in.y=1;
                out.x=m;  //ȷ����������
                out.y=n;
                R=L;
                if(!MazePath(L,in,out)){
                    goto zd;   //����ֱ����ͨ·���Թ�
			    }
                else {
                    int choice;
                    printf("���������ѡ��1�ɵ����ߣ�2���û��ߣ���");
                    scanf("%d",&choice);
                    if(choice==1){
                        ComputerPlay(R,in,out);
                    }
                    else{
                        //PrintMaze(R);
                        if(player(R,in,out))
                            printf("����ս�ɹ������߹���·��Ϊ��\n");
                        PrintMaze(R);
                    }
                }

                printf("�����������!\n");
                getchar();
                while(getchar()!='\n');
                    break;

            case 3:
                system("CLS");    //����
                break;
            case 4:
                kongzhi = -1;
                break;

            default:
                printf("\n�����������!���������룡\n");
                system("CLS");    //����
                break;
            }
	}
	return 0;
}
