#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<windows.h>
 
#define DIS 22
#define BLAN 9
struct link
{
    int data;
    struct link *next;
};
struct link *head=NULL;
typedef struct bird
{
    COORD pos;
    int score;
}BIRD;
 
struct link *appenddata(struct link*head,int data);
//创建链表记录每一次的成绩
void onplay(struct link *head);
//将链表的数据全部展示出来
int findmax(struct link *head);
//找到链表中的最大值
int findthelast(struct link *head);
// 找到最后一次游戏的成绩
void mydelete(struct link *head);
//游戏结束，释放链表所有数据
void checkwall(COORD wall[]);
//显示墙体
void prtbird(BIRD *bird);
 //显示小鸟
int checkwin(COORD *wall, BIRD *bird);
//检测小鸟是否碰墙或者超出边界
void begin(BIRD *bird);
//显示上下边界和分数
BOOL SetConsoleColor(unsigned int wAttributes);
//设置颜色
void gotoxy(int x,int y);
//定位光标
BOOL SetConsoleColor(unsigned int wAttributes);
// 设置颜色
void Hidecursor();
//隐藏光标，避免闪屏现象，提高游戏体验
int main()
{
 
    int i;
    char ch;
    char c;
    int high=0;
    int last=0;
    printf("游戏规则：\n"
           "你需要用空格操控小鸟飞跃一个又一个柱子\n"
           "无论是飞出边界还是撞上柱子都会失败\n"
           "你活得越久分数就越高\n"
           "开始我们的 游戏吧！ \n"
           "                              \n"
           "                              \n"
           "                              \n"
           "do you want to play it?(enter y/n)\n"
           );
    scanf(" %c",&c);
    while(c=='y')
    {
    BIRD bird={{22,10},0};//小鸟的初始位置
    COORD wall[3]={{40,10},{60,6},{80,8}};//柱子的初始位置和高度
    while(checkwin(wall,&bird))
    {
        begin(&bird);//清除屏幕并显示上下边界和分数
        checkwall(wall);//显示柱子樯
        prtbird(&bird);//显示小鸟
        Sleep(200);
        if(kbhit())// 检测键盘输入
        {
            ch=getch();
            if(ch==' ')
            {
                bird.pos.Y=bird.pos.Y-1;
            }
        }
        else
        {
            bird.pos.Y+=1;
        }
        for(i=0;i<3;++i)
        {
            wall[i].X--;
        }
    }
    printf("不要灰心，更高的山峰在等待着攀登的那个人！\n");
    printf("\n");
    printf("do you want to play it again?(enter y/n)\n");
    scanf(" %c",&c);
    }
    onplay(head);
    high=findmax(head);
    last=findthelast(head);
    printf("the highest score is :%d\n",high);
    printf("\n");
    printf("you 差 the highest %d score\n",high-last);
    printf("\n");
    if(last>=high)
        printf("                           you overcome the highest!!!\n");
    free(head);
    return 0;
}
void checkwall(COORD wall[])
{
    int i;
    Hidecursor();
    srand(time(NULL));
    COORD temp={wall[2].X+DIS,rand()%13+5};//随机产生一个新的柱子
    if(wall[0].X<10)//超出预设边界
    {
        wall[0]=wall[1];
        wall[1]=wall[2];
        wall[2]=temp;
    }
    for(i=0;i<3;++i)// 每次显示3个柱子樯
    {
        //显示上半部分柱子樯
        temp.X=wall[i].X+1;//向右缩进一格显示图案
        SetConsoleColor(0x0c);//设置黑色背景，亮红色前景
        for(temp.Y=2;temp.Y<wall[i].Y;temp.Y++)
        {
            gotoxy(temp.X,temp.Y);
            printf("*******");
        }
        temp.X--;              //向左移动一格显示图案
        gotoxy(temp.X,temp.Y);
        printf("********");     //显示下半部分柱子樯
        temp.Y+=BLAN;
        gotoxy(temp.X,temp.Y);
        printf("********");
        temp.X++;               //向右缩进一格显示图案
        temp.Y++;               //在下一行显示下面的图案
        for(;(temp.Y)<26;temp.Y++)//一直显示到第25行
        {
             gotoxy(temp.X,temp.Y);
             printf("******");
        }
    }
}
void prtbird(BIRD *bird)
{
    SetConsoleColor(0x0E); //设置黑色背景，亮黄色前景
    gotoxy(bird->pos.X,bird->pos.Y);
    printf("0->");        //显示小鸟
}
int checkwin(COORD *wall,BIRD *bird)
{
    if(bird->pos.X>=wall->X) // 小鸟的横坐标进入了柱子坐标范围
    {
        if(bird->pos.Y<=wall->Y||bird->pos.Y>=wall->Y+BLAN)
        {
            head=appenddata(head,bird->score);
            system("cls");
            return 0;       //小鸟的纵坐标碰到上下柱子，返回0
        }
    }
    if(bird->pos.Y<1||bird->pos.Y>26)
    {
        head=appenddata(head,bird->score);
        system("cls");
        return 0;           //小鸟的位置超出上下边界
    }
    (bird->score)++;         //分数加一
    return 1;
}
void begin(BIRD *bird)
{
    system("cls");
    gotoxy(0,26);
    printf("=============================="
           "============================"
           "================================");
    gotoxy(0,1);
    printf("=============================="
           "============================"
           "================================");
    SetConsoleColor(0x0E);
    printf("\n% 4d",bird->score);
}
void gotoxy(int x,int y)
{
    COORD pos={x,y};
    HANDLE hOutput =GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput,pos);
}
//函数功能：设置颜色
//一共有16种文字颜色，16种背景颜色，组合有256种，传入的参数值应该小于256
//字节的低4位控制前景色，高四位的控制背景色，高亮+红+绿+蓝
BOOL SetConsoleColor(unsigned int wAttributes)
{
    HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput==INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    return SetConsoleTextAttribute(hOutput,wAttributes);
}
//隐藏函数光标
void Hidecursor()
{
    HANDLE handle =GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CURSORINFO;
    GetConsoleCursorInfo(handle,&CURSORINFO);
    CURSORINFO.bVisible=0;
    SetConsoleCursorInfo(handle,&CURSORINFO);
}
struct link *appenddata(struct link *head,int data)
{
    struct link*p=NULL;
    struct link *pr=head;
    p=(struct link*)malloc(sizeof(struct link));
    if(p==NULL)
        exit(0);
    else
    {
        if(head==NULL)
        {
            p->next=NULL;
            p->data=data;
            head=p;
            return head;
        }
        else
        {
            while(pr->next!=NULL)
            {
                pr=pr->next;
            }
            pr->next=p;
            p->data=data;
            p->next=NULL;
            return head;
        }
    }
    return head;
};
int findmax(struct link *head)
{
    struct link *p=head;
    int mymax=0;
    if(head==NULL)
    {
        printf("you have no score!\n");
        printf("\n");
        mymax=0;
    }
    else
    {
        while(p!=NULL)
        {
            if((p->data)>=mymax)
            {
                mymax=p->data;
            }
            p=p->next;
        }
 
    }
    return mymax;
}
int findthelast(struct link *head)
{
    int last;
    struct link *p=head;
    if(head==NULL)
    {
        printf("there is no last score!\n");
        printf("\n");
        last=0;
    }
    else
    {
        while(p!=NULL)
        {
            last=p->data;
            p=p->next;
        }
    }
    return last;
}
void onplay(struct link *head)
{
    struct link *p=head;
    int i=1;
    while(p!=NULL)
    {
        printf("your 第%d次 score is :%d\n",i,p->data);
        printf("\n");
        p=p->next;
        i++;
    }
}
void mydelete(struct link *head)
{
    struct link*p=head;
    struct link *pr=head;
    while(p!=NULL)
    {
        pr=p;
        p=p->next;
        free(pr);
        printf("your data has been freed!\n");
    }
}