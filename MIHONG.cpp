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
	int x,y;  //迷宫中位置的坐标
}PosType;

typedef struct
{
	int m,n;
	char Array[M][N];  //用二维数组表示迷宫
}MazeType;

typedef struct
{
	int ord;     //通道块在路径上的“序号”
	PosType seat; //通道块在迷宫中的“坐标位置”
	int di;       //从此通道块走向下个通道块的“方向”
}ElemType;        //栈的元素类型

typedef struct NodeType
{
	ElemType data;
	struct NodeType *next;
}NodeType,*LinkType;    //确定栈的存储结构（链式）

typedef struct
{
	LinkType top;       //链栈的顶点定义
	int size;
}Stack;

void   InitStack(Stack  &S)  //新建一个空栈
{
	S.top=NULL;
	S.size=0;
}

Status   MakeNode(LinkType &p,ElemType   e)  //建立一个栈的节点元素
{
	p=(NodeType *)malloc(sizeof(NodeType));
	if(!p)return   FALSE;
	p->data=e;
	p->next=NULL;
	return   TRUE;
}

Status   Push(Stack   &S,ElemType   e)   //链栈的入栈操作
{
	LinkType   p;
	if(MakeNode(p,e)){
		p->next=S.top;
		S.top=p;   //修改栈顶指针，使其指向新插入的结点
		S.size++;    //堆栈元素加1
		return   TRUE;
	}
	return   FALSE;
}

Status   StackEmpty(Stack   S)   //判断栈是否为空
{
	if(S.top==NULL)
		return   TRUE;
	return  FALSE;
}

Status   Pop(Stack   &S,ElemType   &e) //链栈的出栈操作
{
	LinkType   p;
	if(StackEmpty(S))
		return   FALSE;
	else{
		p=S.top;
		S.top=S.top->next;  //将栈顶指针指向当前栈顶所指的下一个元素
		e=p->data;
		S.size--;    //堆栈元素减1
		free(p);
		return   TRUE;
	}
}

Status   Pass(MazeType   maze,PosType   curpos)
//由已知的迷宫和当前正在行走的迷宫的位置判断迷宫是否可以走
{
	if(maze.Array[curpos.x][curpos.y]==' ') //如果当前二维数组元素为空格则可以走
		return   TRUE;
	return   FALSE;
}

Status   Same(PosType   curpos,PosType   end)
//由当前迷宫位置与出口位置进行比较判断是否走到迷宫出口
{
	if(curpos.x==end.x && curpos.y==end.y)    //判断当前位置的坐标是否与出口坐标一致
		return   TRUE;
	return   FALSE;
}

PosType   NextPos(PosType  curpos,int   di)
//由当前迷宫位置和方向判断出行走过程中走的下一个迷宫位置
{
	switch(di){
        case 1:
            curpos.y++; //列坐标加1
            break;
        case 2:
            curpos.x++; //横坐标加1
            break;
        case 3:
            curpos.y--; //列坐标减1
            break;
        case 4:
            curpos.x--; //横坐标减1
            break;
	}
	return   curpos;
}

void   FootPrint(MazeType   &maze,PosType   curpos) //走通的位置用-标记
{
	maze.Array[curpos.x][curpos.y] = '-';
}

void  MarkPrint(MazeType &maze,PosType p)
{
	maze.Array[p.x][p.y]='x';   //走过但是没有走通的位置用标记
}

void  PrintMaze(MazeType   ma)  //打印输出生成的迷宫
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

void   ChangMaze(MazeType  &maze,int  a[M][N],int row,int col) //将0,1矩阵转化为迷宫
{
	int i,j;
	maze.m=row + 2;
	maze.n=col + 2;
	for(i = 0;i < row + 2; i++)
		for(j = 0;j < col + 2; j++){
			if(a[i][j]==0)
				maze.Array[i][j]=' ';   //空格表示路
			else
				maze.Array[i][j]='#';     //“#”表示迷宫里的墙
		}

}
		
		
		


Status   MazePath(MazeType   &maze, PosType   in, PosType   out)
{//根据已知的迷宫、入口（0,0）和出口（m,n）进行前进与否的判断与行走的基本路径
	Stack   s;
	int   curstep=1;//探索第一步
	int a;
	int b;
	int c;
	int i;
	Status   found=FALSE;
	ElemType   e;//定义一个elemtype类型的变量，包含位序，方向和坐标
	PosType   curpos=in; //设定当前位置为入口
	InitStack(s);
	do{
		if(Pass(maze,curpos)){   //当前位置可以通过，即是未曾走过的通道块
			system("cls");   //清屏
			FootPrint(maze,curpos); //留下足迹

            e.ord=curstep;
            e.seat=curpos;
            e.di=1;//fxj(i)
			Push(s,e);  //加入路径
			if(Same(curpos,out)){// 到达终点
				found=TRUE;
			}
			else{
				curpos=NextPos(curpos,1);//下一位置是当前位置的C方向邻家
				curstep++;    //探索下一步
			}
		}
		else   //当前位置不能通过
			if(!StackEmpty(s)){
				Pop(s,e);
				while((e.di==4) && !StackEmpty(s))
				{
					system("cls");   //清屏
					MarkPrint(maze,e.seat);   //留下不能通过的标记，并退回一步

					//PrintMaze(maze);
					Pop(s,e);
					curstep--;
				}
				if(e.di<4)
				{
					e.di++;
					Push(s,e);   //探索下一方向
					curpos=NextPos(e.seat,e.di);   //设定当前位置是该新方向上的相邻
				}
			}
	}while(!StackEmpty(s) && !found);  //判断栈是否为空，是否到出口
	return   found;
}

Status fxj(int x)//由键盘控制迷宫行走方向
{
	switch( getch()){       // getch()从控制台无回显地取一个字符
        case 13:    //回车终止
            x = 5;
            break;
        case 27:    //Esc退出
            exit(0);
            break;
        case 68:  //右
            x = 1;
            break;
        case 83:  //下
            x = 2;
            break;
        case 65:  //左
            x = 3;
            break;
        case 87:  //上
            x = 4;
            break;
	}
	return x;
}

Status   ComputerPlay(MazeType   &maze, PosType   in, PosType   out)
{//根据已知的迷宫、入口（0,0）和出口（m,n）进行前进与否的判断与行走的基本路径
	Stack   s;
	int   curstep=1;//探索第一步
	int a;
	int b;
	int c;
	int i;
	Status   found=FALSE;
	ElemType   e;//定义一个elemtype类型的变量，包含位序，方向和坐标
	PosType   curpos=in; //设定当前位置为入口
	InitStack(s);
	do{
		if(Pass(maze,curpos)){   //当前位置可以通过，即是未曾走过的通道块
			system("cls");   //清屏
			printf("电脑行走过程：\n");
			FootPrint(maze,curpos); //留下足迹

			PrintMaze(maze);
            e.ord=curstep;
            e.seat=curpos;
            e.di=1;  //fxj(i)
			Push(s,e);  //加入路径
			if(Same(curpos,out)){  // 到达终点
				found=TRUE;
			}
			else{
				curpos=NextPos(curpos,1);  //下一位置是当前位置的C方向邻家
				curstep++;    //探索下一步
			}
		}
		else   //当前位置不能通过
			if(!StackEmpty(s)){
				Pop(s,e);
				while((e.di==4) && !StackEmpty(s))
				{
					system("cls");   //清屏
					printf("电脑行走过程：\n");
					MarkPrint(maze,e.seat);   //留下不能通过的标记，并退回一步

					PrintMaze(maze);
					Pop(s,e);
					curstep--;
				}
				if(e.di<4)
				{
					e.di++;
					Push(s,e);   //探索下一方向
					curpos=NextPos(e.seat,e.di);   //设定当前位置是该新方向上的相邻
				}
			}
			//延时用
            for(a=0;a<10000;a++)
                for(b=0;b<10000;b++);

	}while(!StackEmpty(s) && !found);  //判断栈是否为空，是否到出口
	return   found;
}

Status player(MazeType   &maze,PosType   in,PosType   out)
{
	int c;
	int i;
	Status   found=FALSE;
	ElemType   e;//定义一个elemtype类型的变量，包含位序，方向和坐标
	PosType   curpos=in; //设定当前位置为入口

	do{
        if(maze.Array[curpos.x][curpos.y] != '#'){   //当前位置若是迷宫的墙，则不可以通过

			system("cls");   //清屏
			printf("迷宫存在通路,请您利用方向键控制迷宫行进方向\n");
			printf("您可以在行走过程中按回车键终止游戏，或者按ESC键退出游戏\n");

			FootPrint(maze,curpos); //留下足迹
			PrintMaze(maze);

            e.seat=curpos;
			if(Same(curpos,out)){// 到达终点
				found=TRUE;
			}
			else{
			    rep:
                c = fxj(i);
                switch(c){
                    case 1: //右
                        curpos = NextPos(curpos,c);
                        break;
                    case 2: //下
                        curpos = NextPos(curpos,c);
                        break;
                    case 3: //左
                        curpos = NextPos(curpos,c);
                        break;
                    case 4: //上
                        curpos = NextPos(curpos,c);
                        break;
                    case 5:  //退出
                        goto xm;
                }
                if(maze.Array[curpos.x][curpos.y] == '-'){
                    MarkPrint(maze,e.seat);  //留下不能通过的标记，并退回一步
                }
			}
		}
		else{
		    curpos = e.seat;
		    goto rep;
		}
	}while(!found);//判断栈是否为空，是否到出口
	xm:return   found;
}

void Print(int maze[M][N],int m,int n)   //打印0,1矩阵代表的迷宫
{
	int i,j;
	printf("建立的迷宫数组：\n");
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

void shoudong_maze(int maze[M][N],int x,int y) //手动输入迷宫
{
	int i,j;
    int m=x;
	int n=y;//迷宫行,列
	printf("\n请输入迷宫的各行各列:\n用空格隔开,0代表路,1代表墙。若输入不合法，默认为1。\n");
	for(i=1;i<=m;i++)
		for(j=1;j<=n;j++)
		{
			scanf("%d",&maze[i][j]);
			if((maze[i][j]!=0)&&(maze[i][j]!=1)) maze[i][j]=1;
		}

		for(i=0;i<=m+1;i++) //加一圈围墙
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


void zidong_maze(int maze [M][N],int m,int n)   //自动生成随机迷宫
{
	srand( time( NULL ) );
	int i,j;
	printf("\n迷宫生成中……\n\n");
	for(i=1;i<=m;i++)
		for(j=1;j<=n;j++)
			maze[i][j]=rand()%2;
        maze[1][1]=maze[m][n]=0;
		for(i=0;i<=m+1;i++){ //加一圈围墙
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
        printf("请输入迷宫的行数 m=");
        fflush(stdin);   //用来清空输入缓存，以便不影响后面输入的东西
        if(!scanf("%d",&m)) {
            printf("输入字符出错！请重新输入!\n");
            continue;
        }
        else{
            break;}
    }

    while(1){
        printf("请输入迷宫的列数 n=");
        fflush(stdin);
        if(!scanf("%d",&n)){
            printf("输入字符出错！请重新输入!\n");
            continue;
        }
        else{
            break;}
    }
    if((m<=0||m>25)||(n<=0||n>30)){
        printf("\n抱歉，你输入的行列数超出预设范围(0-25,0-30),请重新输入：\n\n");
        goto mark;
    }
}

int main()
{
	int maze[M][N],m,n,i,kongzhi=0;
	MazeType L,R;    //用二维数组表示迷宫
    PosType   in,out;   //迷宫中位置的坐标
	while(kongzhi!= -1 ){
		printf("\t    ******************欢迎进入迷宫游戏****************\n");
		printf("\t    *              手动生成迷宫  请按：1             *\n");
		printf("\t    *              自动生成迷宫  请按：2             *\n");
		printf("\t    *              清屏          请按：3             *\n");
		printf("\t    *              退出          请按：4             *\n");
		printf("\t    **************************************************\n");
		printf("\n");
		printf("请选择你的操作：");
		scanf("%d",&i);
		switch(i){
            case 1:
                input(m,n);
                shoudong_maze(maze,m,n);//建立迷宫
                Print(maze,m,n);//打印出迷宫数组
                ChangMaze(L,maze,m,n);
                in.x=1;   //确定入口坐标
                in.y=1;
                out.x=m;  //确定出口坐标
                out.y=n;
                R=L;
                printf("由数组转化出的迷宫");
                PrintMaze(L);

                printf("\n按任意键继续!\n");
                getchar();
                while(getchar()!='\n');

                if(MazePath(L,in,out)){
                    int choice;
                    printf("请输入你的选择（1由电脑走，2由用户走）：");
                    scanf("%d",&choice);
                    if(choice==1){
                        ComputerPlay(R,in,out);    //由电脑走迷宫
                    }
                    else{
                        if(player(R,in,out)){    //由玩家走迷宫
                            printf("您挑战成功，所走过的路径为：\n");
                            PrintMaze(R);
                        }
                        else
                            printf("游戏结束！\n");
                    }
                }
                else
                    printf("此迷宫没有通路！\n");
            printf("按任意键继续!\n");
            getchar();
            while(getchar()!='\n');
            break;

            case 2:
                input(m,n);
                zd:
                zidong_maze(maze,m,n);
                ChangMaze(L,maze,m,n);
                in.x=1;   //确定入口坐标
                in.y=1;
                out.x=m;  //确定出口坐标
                out.y=n;
                R=L;
                if(!MazePath(L,in,out)){
                    goto zd;   //生成直至有通路的迷宫
			    }
                else {
                    int choice;
                    printf("请输入你的选择（1由电脑走，2由用户走）：");
                    scanf("%d",&choice);
                    if(choice==1){
                        ComputerPlay(R,in,out);
                    }
                    else{
                        //PrintMaze(R);
                        if(player(R,in,out))
                            printf("您挑战成功，所走过的路径为：\n");
                        PrintMaze(R);
                    }
                }

                printf("按任意键继续!\n");
                getchar();
                while(getchar()!='\n');
                    break;

            case 3:
                system("CLS");    //清屏
                break;
            case 4:
                kongzhi = -1;
                break;

            default:
                printf("\n你的输入有误!请重新输入！\n");
                system("CLS");    //清屏
                break;
            }
	}
	return 0;
}

