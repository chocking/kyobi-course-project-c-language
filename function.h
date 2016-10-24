#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define xStart 3
#define yStart 2
#define maxTool_1 3
#define maxTool_2 2
#define maxTool_3 1
#define roundScore_1 13000//
#define roundScore_2 10000//
#define roundScore_3 8000//
#define roundTime_1  60
#define roundTime_2  45
#define roundTime_3  30

int icon[12][12];//定义全局矩阵
int numberOfTool_1,numberOfTool_2,numberOfTool_3;//定义全局各道具数目
int score;
int temSecond,currentSecond,seconds;

typedef struct people//光荣榜结构体
{
    char name[7];//过关者
    int score;//过关分数
    struct people *next;
} honour,*honourList;

void cursorMove(int x,int y)
{

    COORD coord;
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}//光标移动

void HideCursor()//隐藏光标
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0}; //赋初始值，第二个参数为0时不可见
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

}

int* getCursor()
{
    int *cursor;
    cursor=(int*)malloc(2*sizeof(int));
    CONSOLE_SCREEN_BUFFER_INFO    bInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&bInfo);
    cursor[0]=bInfo.dwCursorPosition.X;
    cursor[1]=bInfo.dwCursorPosition.Y;
    return cursor;
}//返回光标坐标


int  timeSecond()
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p=gmtime(&timep);

    return (p->tm_sec);
}//返回当前时间的秒数

void setColor(int color)
{

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);

}//设置背景及前景色

int printInterface(int xMin,int xMax,int yMin,int yMax,int situation)
//打印游戏界面（图块阵列）
//situation 1:黑色背景 光标自动寻位
//situation 2:白色背景 光标自动寻位
//situation 3:黑色背景 光标不自动寻位
{
    int x,y;

    for(y=yMin; y<=yMax; y++)
    {

        for(x=xMin; x<=xMax; x++)
        {
            if(situation==1||situation==2)
            {
                cursorMove(xStart-2+x*4,yStart-1+2*y);    //数组坐标为x，y的控制台坐标
            }


            switch(icon[x][y])
            {
            case 1:
                if(situation==2)
                    setColor(0xF8);
                else setColor(0x08);
                printf("▲");
                break;
            case 2:
                if(situation==2)
                    setColor(0xF9);
                else setColor(0x09);
                printf("▼");
                break;
            case 3:
                if(situation==2)
                    setColor(0xFA);
                else setColor(0x0A);
                printf("●");
                break;
            case 4:
                if(situation==2)
                    setColor(0xFB);
                else setColor(0x0B);
                printf("◆");
                break;
            case 5:
                if(situation==2)
                    setColor(0xFD);
                else setColor(0x0D);
                printf("■");
                break;
            case 6:
                if(situation==2)
                    setColor(0xFC);
                else  setColor(0x0C);
                printf("★");
                break;
            case 11://道具（1）
                if(situation==2)
                    setColor(0xFE);
                else
                    setColor(0x0E);
                printf("¤");
                break;
            case 12://道具（2）
                if(situation==2)
                    setColor(0xFE);
                else
                    setColor(0x0E);
                printf("╬");
                break;
                /* case 13://道具（3）
                     if(situation==2)
                         setColor(0xFE);
                     else
                         setColor(0x0E);
                     printf("☆");
                     break;
                    */
            case 21://道具（3）-1
                if(situation==2)
                    setColor(0xFE);
                else
                    setColor(0x0E);
                printf("▲");
                break;
            case 22://道具（3）-2
                if(situation==2)
                    setColor(0xFE);
                else
                    setColor(0x0E);
                printf("▼");
                break;
            case 23://道具（3）-3
                if(situation==2)
                    setColor(0xFE);
                else
                    setColor(0x0E);
                printf("●");
                break;
            case 24://道具（3）-4
                if(situation==2)
                    setColor(0xFE);
                else
                    setColor(0x0E);
                printf("◆");
                break;
            case 25://道具（3）-5
                if(situation==2)
                    setColor(0xFE);
                else
                    setColor(0x0E);
                printf("■");
                break;
            case 26://道具（3）-6
                if(situation==2)
                    setColor(0xFE);
                else
                    setColor(0x0E);
                printf("★");
                break;
            default:
                break;
            }


            //printf("%d ",icon[x][y]);
        }
        //printf("\n\n");

    }



    return 0;
}

void rebuiltIcon(int icon[12][12])//矩阵重新随机赋值
{
    int x,y;
    for(y=1; y<=10; y++)
    {
        for(x=1; x<=10; x++)
        {
            icon[x][y] = -1;

        }
    }//生成随机数组矩阵

}

int judgeFeasibility()//判断游戏的可行性（没有可消除图块的情况下）
{
    int x,y;
    int tem;
    for(y=1; y<=10; y++)
    {
        for(x=1; x<=10; x++)
        {
            tem=icon[x][y] ;

            if(tem>10)//道具
            {
                return 1;
            }

            if(icon[x-1][y]==tem&&icon[x+1][y]!=0)//左图块相同且右图块不为0
            {
                if(icon[x+1][y-1]==tem||icon[x+1][y+1]==tem||icon[x+2][y]==tem)
                    return 1;
            }
            if(icon[x][y-1]==tem&&icon[x][y+1]!=0)//上图块相同且下图块不为0
            {
                if(icon[x-1][y+1]==tem||icon[x+1][y+1]==tem||icon[x][y+2]==tem)
                    return 1;
            }
            if(icon[x+1][y]==tem&&icon[x-1][y]!=0)//右图块相同且左图块不为0
            {
                if(icon[x-1][y-1]==tem||icon[x-1][y+1]==tem||icon[x-2][y]==tem)
                    return 1;
            }
            if(icon[x][y+1]==tem&&icon[x][y-1]!=0)//下图块相同且上图块不为0
            {
                if(icon[x-1][y-1]==tem||icon[x+1][y-1]==tem||icon[x][y-2]==tem)
                    return 1;
            }

            if(icon[x-1][y-1]==tem)//左上图块相同
            {
                if(icon[x+1][y-1]==tem||icon[x-1][y+1]==tem)
                    return 1;
            }
            if(icon[x+1][y+1]==tem)//右下图块相同
            {
                if(icon[x-1][y+1]==tem||icon[x+1][y-1]==tem)
                    return 1;
            }
        }
    }


    return 0;
}

int judgeRemove(int assist[12][12])//判断并标记连续可消除的图块
{

    // int assist[12][12];
    int x,y,i;
    int flag=0;

    /* for(y=1; y<=10; y++)
     {
         for(x=1; x<=10; x++)
         {
             assist[x][y]=0;
         }
     }//辅助数组初始化
     */
    for(y=1; y<=10; y++)
    {
        for(x=1; x<=10; x++)
        {

            if(assist[x][y]==0 && icon[x][y]<10)//不属于某一连续快并且不是道具
            {
                if(icon[x+1][y]==icon[x][y]&&icon[x+2][y]==icon[x][y])
                {
                    //向右比较
                    assist[x][y]=1;
                    flag=1;
                    i=x+1;
                    while(icon[i][y]==icon[x][y])
                    {
                        assist[i++][y]=1;
                    }
                }

                if(icon[x][y+1]==icon[x][y]&&icon[x][y+2]==icon[x][y])
                {
                    //向下比较
                    assist[x][y]=1;
                    flag=1;
                    i=y+1;
                    while(icon[x][i]==icon[x][y])
                    {
                        assist[x][i++]=1;
                    }
                }
            }

            else  if(assist[x][y]==1)//属于某一连续快
            {
                if(assist[x+1][y]==0&&icon[x+1][y]==icon[x][y]&&icon[x+2][y]==icon[x][y])
                {
                    //向右比较
                    i=x+1;
                    while(icon[i][y]==icon[x][y])
                    {
                        assist[i++][y]=1;
                    }
                }
                if(assist[x][y+1]==0&&icon[x][y+1]==icon[x][y]&&icon[x][y+2]==icon[x][y])
                {
                    //向下比较
                    i=y+1;
                    while(icon[x][i]==icon[x][y])
                    {
                        assist[x][i++]=1;
                    }
                }
            }

        }
    }
    /* for(y=0; y<12; y++)
     {
         for(x=0; x<12; x++)
         {
             if(assist[x][y]==1)
                 account++;
         }
     }
     */

    return flag;

}

void toolRemove(int assist[12][12],int x,int y)//标记使用道具后需消除的图块
{
    int i,j;
    int color;
    /* for(j=1; j<=10; j++)
     {
         for(i=1; i<=10; i++)
         {

             assist[i][j]=0;
         }
     }//辅助数组初始化
     */
    switch(icon[x][y])
    {
    case 11://道具（1），消除以道具为中心的九个图块
        icon[x][y]=-1;
        for(j=y-1; j<=y+1; j++)
        {
            for(i=x-1; i<=x+1; i++)
            {
                /* if(icon[i][j]>10&&(i!=x||j!=y))
                 {
                     toolRemove(assist,i,j);
                 }
                 */
                assist[i][j]=1;
            }
        }
        numberOfTool_1--;
        break;
    case 12://道具（2），消除道具所在的行和列
        icon[x][y]=-1;
        for(i=1; i<=10; i++)//消除行
        {
            /*if(icon[i][y]>10&&i!=x)
               {
                   toolRemove(assist,i,y);
               }
               */
            assist[i][y]=1;

        }
        for(j=1; j<=10; j++)//消除列
        {
            /*if(icon[x][j]>10&&j!=y)
               {
                   toolRemove(assist,x,j);
               }
               */
            assist[x][j]=1;
        }
        numberOfTool_2--;
        break;
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26://道具（3），消除同一种颜色的图块
        //randomColor=rand()%6+1;

        color=icon[x][y]%10;
        icon[x][y]=-1;
        for(j=1; j<=10; j++)
        {
            for(i=1; i<=10; i++)
            {
                if(icon[i][j]==color)
                {
                    assist[i][j]=1;
                }

            }
        }
        assist[x][y]=1;
        numberOfTool_3--;
        break;
    default:
        break;
    }
}

int  numberOfRemove(int assist[12][12])
{
    int x,y;
    int account=0;
    for(y=1; y<=10; y++)
    {
        for(x=1; x<=10; x++)
        {
            if(assist[x][y]==1)
                account++;
        }
    }
    return account ;
}

void initAssist(int assist[12][12])
{
    int x,y;
    for(y=1; y<=10; y++)
    {
        for(x=1; x<=10; x++)
        {
            assist[x][y]=0;
        }
    }//辅助数组初始化
}


void control(int direction,int target[])//键盘控制光标上下左右移动
{


    switch(direction)
    {
    case 0://初始target（选框）位置
        setColor(15);
        cursorMove(xStart,yStart);
        printf("┏  ┓");
        cursorMove(xStart,yStart+2);
        printf("┗  ┛");
        break;
    case 1:
        if(target[0]>1)
        {
            setColor(0);
            cursorMove(target[0]*4+xStart-4,target[1]*2+yStart-2);
            printf("      ");
            cursorMove(target[0]*4+xStart-4,target[1]*2+yStart);
            printf("      ");

            setColor(15);
            cursorMove(target[0]*4-8+xStart,target[1]*2+yStart-2);
            printf("┏  ┓");
            cursorMove(target[0]*4-8+xStart,target[1]*2+yStart);
            printf("┗  ┛");
            target[0]--;

        }
        break;//左
    case 2:
        if(target[1]>1)
        {
            setColor(0);
            cursorMove(target[0]*4+xStart-4,target[1]*2+yStart-2);
            printf("      ");
            cursorMove(target[0]*4+xStart-4,target[1]*2+yStart);
            printf("      ");
            setColor(15);
            cursorMove(target[0]*4+xStart-4,target[1]*2-4+yStart);
            printf("┏  ┓");
            cursorMove(target[0]*4+xStart-4,target[1]*2-2+yStart);
            printf("┗  ┛");
            target[1]--;
        }
        break;//上
    case 3:
        if(target[0]<10)
        {
            setColor(0);
            cursorMove(target[0]*4+xStart-4,target[1]*2+yStart-2);
            printf("      ");
            cursorMove(target[0]*4+xStart-4,target[1]*2+yStart);
            printf("      ");
            setColor(15);
            cursorMove(target[0]*4+xStart,target[1]*2+yStart-2);
            printf("┏  ┓");
            cursorMove(target[0]*4+xStart,target[1]*2+yStart);
            printf("┗  ┛");
            target[0]++;
        }

        break;//右
    case 4:
        if(target[1]<10)
        {
            setColor(0);
            cursorMove(target[0]*4-4+xStart,target[1]*2+yStart-2);
            printf("      ");
            cursorMove(target[0]*4-4+xStart,target[1]*2+yStart);
            printf("      ");
            setColor(15);
            cursorMove(target[0]*4+xStart-4,target[1]*2+yStart);
            printf("┏  ┓");
            cursorMove(target[0]*4+xStart-4,target[1]*2+2+yStart);
            printf("┗  ┛");
            target[1]++;
        }
        break;//下
    case 5:
        setColor(15);
        cursorMove(target[0]*4+xStart-4,target[1]*2+yStart-2);
        printf("┏  ┓");
        cursorMove(target[0]*4+xStart-4,target[1]*2+yStart);
        printf("┗  ┛");
        break;//当前位置重新打印
    default:
        break;
    }
}//玩家控制光标

void remove(int assist[12][12])
{
    int x,y;
    //char ch;
    setColor(14);
    for(y=1; y<=10; y++)
    {
        for(x=1; x<=10; x++)
        {
            if(assist[x][y]==1)
            {
                cursorMove(xStart-2+x*4,yStart-1+2*y);
                printf("¤");
            }
        }
    }
    Sleep(200);
    for(y=1; y<=10; y++)
    {
        for(x=1; x<=10; x++)
        {
            if(assist[x][y]==1)
            {
                cursorMove(xStart-2+x*4,yStart-1+2*y);
                printf("* ");
            }
        }
    }
    Sleep(200);
    for(y=1; y<=10; y++)
    {
        for(x=1; x<=10; x++)
        {
            if(assist[x][y]==1)
            {

                cursorMove(xStart-2+x*4,yStart-1+2*y);
                printf("  ");
                icon[x][y]=-1;//icon数组已消除的位置置-1
            }
        }
    }

//ch=getch();



}//消除连续图块

void move(int lock[2],int target[2])//交换图块位置
{
    int high,low;//优先移动权
    int tem;

    if(lock[0]==target[0])//x坐标相同，上下交换
    {
        high=lock[1]>target[1]?lock[1]:target[1];//y坐标大
        low=lock[1]<target[1]?lock[1]:target[1];//y坐标小

        // printInterface(lock[0],lock[0],low,low,1);//y小的置黑色背景

        cursorMove(xStart-2+lock[0]*4,yStart-1+2*high);
        setColor(0);
        printf("  ");//y大的消失

        cursorMove(xStart-2+lock[0]*4,yStart+2*high-2);
        printInterface(lock[0],lock[0],high,high,3);//y大的上移出现
        Sleep(50);

        cursorMove(xStart-2+lock[0]*4,yStart+2*high-3);
        printInterface(lock[0],lock[0],high,high,3);//y大的上移

        cursorMove(xStart-2+lock[0]*4,yStart+2*high-2);
        printInterface(lock[0],lock[0],low,low,3);//y小的下移
        Sleep(50);

        cursorMove(xStart-2+lock[0]*4,yStart+2*high-2);//y小的消失
        setColor(0);
        printf("  ");

        cursorMove(xStart-2+lock[0]*4,yStart-1+2*high);
        printInterface(lock[0],lock[0],low,low,3);//y小的下移出现

        tem=icon[lock[0]][high];
        icon[lock[0]][high]=icon[lock[0]][low];
        icon[lock[0]][low]=tem;
    }
    else//y坐标相同，左右交换
    {
        high=lock[0]>target[0]?lock[0]:target[0];//x坐标大
        low=lock[0]<target[0]?lock[0]:target[0];//x坐标小

        // printInterface(low,low,lock[1],lock[1],1);//x小的置黑色背景

        cursorMove(xStart-2+high*4,yStart-1+2*lock[1]);
        setColor(0);
        printf("  ");//x大的消失

        cursorMove(xStart+high*4-4,yStart-1+2*lock[1]);
        printInterface(high,high,lock[1],lock[1],3);//x大的左移出现
        Sleep(50);

        cursorMove(xStart+high*4-6,yStart-1+2*lock[1]);
        printInterface(high,high,lock[1],lock[1],3);//x大的左移

        cursorMove(xStart+high*4-4,yStart-1+2*lock[1]);
        printInterface(low,low,lock[1],lock[1],3);//x小的右移
        Sleep(50);

        cursorMove(xStart+high*4-4,yStart-1+2*lock[1]);//x小的消失
        setColor(0);
        printf("  ");

        cursorMove(xStart-2+high*4,yStart-1+2*lock[1]);
        printInterface(low,low,lock[1],lock[1],3);//x小的右移出现


        tem=icon[high][lock[1]];
        icon[high][lock[1]]=icon[low][lock[1]];
        icon[low][lock[1]]=tem;
    }

}//交换两个图块的位置

void fillBlank()
{
    int record[12];
    //char ch;
    int x,y;
    int flag=0;
    for(x=1; x<=10; x++)
    {
        record[x]=0;
    }

    for(x=1; x<=10; x++)
    {
        //用record数组记录每一列从上往下数遇到的第一个空缺
        for(y=1; y<=10; y++)
        {
            if(icon[x][y]==-1)
            {
                record[x]=y;
                flag=1;
                break;
            }
        }
    }
    while(flag==1)
    {
        currentSecond = timeSecond();//秒数跳数
        if(temSecond != currentSecond && seconds>=0)
        {
            cursorMove(72,10);
            if(seconds>10)
                setColor(0x8E);
            else
                setColor(0x8C);
            printf("%3d",seconds--);
            temSecond = currentSecond;
        }
        //ch=getch();
        Sleep(150);
        for(x=1; x<=10; x++)
        {
            //有空缺的每一列下移一格
            if(record[x]!=0)
            {
                for(y=record[x]; y>1; y--)
                {
                    icon[x][y]=icon[x][y-1];

                }
                //srand( (unsigned)time( NULL ) );
                icon[x][1]=(int)(rand()%6+1);
                printInterface(x,x,1,record[x],1);
                record[x]=0;
            }
        }
        flag=0;
        for(x=1; x<=10; x++)
        {
            //用record数组记录每一列从上往下数遇到的第一个空缺
            for(y=1; y<=10; y++)
            {
                if(icon[x][y]==-1)
                {
                    record[x]=y;
                    flag=1;
                    break;
                }
            }
        }

    }



}//填补消除图块的空缺

void creatTool(int number)//一次性消除多个图块，随机位置出现道具
{
    int x,y;
    x=rand()%10+1;
    y=rand()%10+1;
    while(icon[x][y]>10)//随机位置已有道具
    {
        x=rand()%10+1;
        y=rand()%10+1;
    }


    if(number>=10&&numberOfTool_3<maxTool_3)//同时消除10或以上个图块且数量小于上限
    {

        // icon[x][y]=13;
        icon[x][y] = 20+rand()%6+1;///////////////////
        setColor(0xEE);
        cursorMove(xStart-2+x*4,yStart-1+2*y);
        printf("  ");
        Sleep(300);
        printInterface(x,x,y,y,1);
        numberOfTool_3++;

    }
    else  if(number>=8&&numberOfTool_2<maxTool_2)//同时消除8-9个图块且数量小于上限
    {

        icon[x][y]=12;
        setColor(0xEE);
        cursorMove(xStart-2+x*4,yStart-1+2*y);
        printf("  ");
        Sleep(300);
        printInterface(x,x,y,y,1);
        numberOfTool_2++;

    }
    else  if(number>=4&&numberOfTool_1<maxTool_1)//同时消除4-7个图块且数量小于上限
    {

        icon[x][y]=11;
        setColor(0xEE);
        cursorMove(xStart-2+x*4,yStart-1+2*y);
        printf("  ");
        Sleep(300);
        printInterface(x,x,y,y,1);
        numberOfTool_1++;

    }
}

void calculateScore(int account)
{
    if(account<=5)
    {
        score += account*100;

    }
    else if(account<=9)
    {
        score += account*120 - 100;//account*100+(account-5)*20

    }
    else  if(account>=10)
    {
        score += account*150 - 370;//account*100+4*20+(account-9)*50

    }
}

void printInformation()
{
    setColor(0xE0);
    cursorMove(60,7);
    printf("%7d",score);
}//打印分数等游戏信息


honourList getHonourList(int pattern)//读取光荣榜文件 pattern=1:自由模式 2:闯关模式
{
    FILE *fp;
    int i;
    char mode[25];
    honourList head,q,p;
    head = (honourList)malloc(sizeof(honour));
    head->next=NULL;

    if(pattern == 1)
    {
        strcpy(mode,"freehonourlist.txt");
    }
    else
    {
        strcpy(mode,"roundshonourlist.txt");
    }

    if((fp=fopen(mode,"r")) == NULL)
    {

        fp=fopen(mode,"w");
        p=head;
        for(i=1; i<=5; i++)
        {
            q=(honourList)malloc(sizeof(honour));
            q->next = NULL;
            strcpy(q->name,"XXX");
            q->score = 0;
            p->next = q;
            p=q;
        }
        //fwrite(,10*sizeof(honour),10,fp);
        p=head->next;
        while(p)
        {
            fprintf(fp,"%s %d\n",p->name,p->score);
            p = p->next;

        }
        fclose(fp);
        //return head;
    }
    else
    {
        //fread(hon,10*sizeof(honour),10,fp);
        p=head;
        for(i=1; i<=5; i++)
        {
            q=(honourList)malloc(sizeof(honour));
            q->next = NULL;
            fscanf(fp,"%s %d",q->name,&q->score);
            p->next = q;
            p = q;

        }

    }

    return head;
}

void rank()
{
    int i,j;
    honourList  head,p;
    char keyDown;
    char groupname[30]="Designed by ___小菜鸡";

    setColor(0x70);
    system("cls");
    setColor(0x80);
    for(i=1; i<=7; i++)
    {
        for(j=1; j<=80; j++)
        {
            printf(" ");
        }

    }

    cursorMove(10,1);
    printf("  ▉▉▉▉          ▉          ▉▋  ▉      ▉  ▉▋");
    Sleep(100);
    cursorMove(10,2);
    printf("  ▉    ▉        ▉  ▉        ▉▋  ▉      ▉ ▋");
    Sleep(100);
    cursorMove(10,3);
    printf("  ▉▉▉        ▉▉▉▉▉      ▉ ▋ ▉      ▉▋");
    Sleep(100);
    cursorMove(10,4);
    printf("  ▉  ▋        ▉      ▉      ▉  ▋▉      ▉ ▋");
    Sleep(100);
    cursorMove(10,5);
    printf("  ▉  ▉▋      ▉      ▉      ▉    ▉      ▉  ▉▋");
    Sleep(100);
    setColor(0x8F);
    cursorMove(20,0);
    printf("★∵");
    Sleep(100);

    cursorMove(0,0);
    printf("  ");
    cursorMove(20,1);
    printf("★∵");
    Sleep(80);

    cursorMove(20,1);
    printf("  ");
    cursorMove(20,2);
    printf("★∵");
    Sleep(50);

    cursorMove(20,2);
    printf("  ");
    cursorMove(20,1);
    printf("★∵");
    Sleep(250);

    cursorMove(20,2);
    printf("★∵");
    cursorMove(20,1);
    printf("  ");

    setColor(0x8E);
    cursorMove(0,60);
    printf("*☆");
    Sleep(100);

    cursorMove(62,5);
    printf("☆");
    Sleep(250);
    cursorMove(62,5);
    printf("★");
    Sleep(200);

    setColor(0x70);
    cursorMove(55,25);
    i=0;
    while(groupname[i]!='\0')
    {
        if(i==12)
        {
            cursorMove(65,26);
        }
        putchar(groupname[i++]);
        Sleep(30);
    }


    setColor(0x7E);
    cursorMove(18,10);
    printf("【自由模式】                    【闯关模式】");


    setColor(0x70);
    head = getHonourList(1);
    p=head->next;
    i=12;
    j=1;
    while(p)
    {
        cursorMove(17,i);
        printf("%d. %-6s  %-6d",j++,p->name,p->score);
        p=p->next;
        i+=2;
    }

    head = getHonourList(2);
    p=head->next;
    i=12;
    j=1;
    while(p)
    {
        cursorMove(49,i);
        printf("%d. %-6s  %-6d",j++,p->name,p->score);
        p=p->next;
        i+=2;
    }

    cursorMove(0,26);
    printf("[Esc]———返回");
    while((keyDown=getch())!=27)
    {

    }





}

void printFrame(int pattern , int round)
{
    int i;
    honourList head;
    setColor(0x40);
    cursorMove(1,1);
    printf("╬═════════╬╬╬═════════╬");
    for(i=1; i<=21; i++)
    {
        cursorMove(1,i+1);
        printf("║");
    }
    for(i=1; i<=21; i++)
    {
        cursorMove(45,i+1);
        printf("║");
    }
    cursorMove(1,23);
    printf("╬═════════╬╬╬═════════╬");


    setColor(0xE0);
    cursorMove(50,1);
    printf(" ┏━                 ┏━  ");
    cursorMove(50,2);
    printf(" ┃                   ┃    ");
    cursorMove(50,3);
    printf(" ┗-┓ ┏┓ ┏┓ ┳┓ ┣━  ");
    cursorMove(50,4);
    printf("    ┃ ┃   ┃┃ ┃   ┃    ");
    cursorMove(50,5);
    printf("  ━┛ ┗┛ ┗┛ ┻   ┗━  ");

    setColor(0x0E);

    cursorMove(53,6);
    printf("┃");
    cursorMove(53,7);
    printf("┗━");
    cursorMove(73,6);
    printf("┃");
    cursorMove(71,7);
    printf("━┛");



    setColor(0x80);
    cursorMove(49,9);
    printf("  ╓───────────╖  ");
    cursorMove(49,10);
    printf("╠╣                      ╠╣");
    cursorMove(49,11);
    printf("  ╙───────────╜  ");


    setColor(0x8E);
    cursorMove(53,10);
    printf("The Rest Of Time :");

    setColor(0x0E);
    cursorMove(0,24);

    for(i=1; i<=47; i++)
    {
        printf("_");
    }

    cursorMove(0,25);

    for(i=1; i<=80; i++)
    {
        printf("_");

    }

    cursorMove(57,7);
    setColor(0xE0);
    printf("              ");

    setColor(0x80);
    for(i=1; i<=12; i++)
    {
        cursorMove(49,12+i);
        printf("║                          ║");
    }
    setColor(0x81);
    cursorMove(51,13);
    printf("┏━━━━━━━┓");
    cursorMove(51,14);
    printf("┃     Mode :   ┃");
    cursorMove(51,15);
    printf("┗━━━━━━━┛");
    cursorMove(51,16);
    printf("┏━━━━━━━┓");
    cursorMove(51,17);
    printf("┃  High Record:┃");
    cursorMove(51,18);
    printf("┗━━━━━━━┛");
    cursorMove(51,19);
    printf("┏━━━━━━━┓");
    cursorMove(51,20);
    printf("┃Current Round:┃");
    cursorMove(51,21);
    printf("┗━━━━━━━┛");
    cursorMove(51,22);
    printf("┏━━━━━━━┓");
    cursorMove(51,23);
    printf("┃Total Rounds :┃");
    cursorMove(51,24);
    printf("┗━━━━━━━┛");

    setColor(0x80);//模式
    cursorMove(71,14);
    if(pattern == 1)
        printf("自由");
    else
        printf("闯关");

    cursorMove(69,17);
    head = getHonourList(pattern);
    printf("%6d",head->next->score);
    //printf("%7d",getHighRecord(pattern));


    setColor(0x82);
    cursorMove(69,23);//总关数
    if(pattern == 1)
    {
        printf(" ━━━ ");
    }
    else
    {
        printf("■ ■ ■");
    }

    //setColor(0x8C);
    cursorMove(69,20);//当前关数
    if(pattern == 1)
    {
        printf(" ━━━ ");
    }
    else
    {
        switch(round)
        {
        case 1:
            printf("□");
            break;
        case 2:
            printf("■ □");
            break;
        case 3:
            printf("■ ■ □");
            break;
        default:
            break;
        }
    }
}

int pause(int target[2], int lock[2])
{
    int i;
    char keyDown;
    int choice,tem;
    char choices[3][9]= {"继续游戏","重新开始"," 退  出 "};
    setColor(0x70);
    cursorMove(xStart+14,yStart+5);
    printf("╆═════╅");

    for(i=1; i<=7; i++)
    {
        cursorMove(xStart+14,yStart+5+i);
        printf("║          ║");
    }

    cursorMove(xStart+14,yStart+13);
    printf("╄═════╃");

    cursorMove(xStart+17,yStart+7);
    //printf("继续游戏");
    puts(choices[0]);

    setColor(0x7F);
    cursorMove(xStart+17,yStart+9);
    //printf("重新开始");
    puts(choices[1]);
    cursorMove(xStart+17,yStart+11);
    //printf(" 退  出 ");
    puts(choices[2]);

    choice = 1;

    while((keyDown=getch()))
    {
        //cursor = getCursor();
        if(keyDown==-32)
        {

            keyDown=getch();
            if(keyDown==72)//键盘上
            {
                tem = choice;//记录上一次位置
                choice -= 1;
                if(choice == 0)
                    choice = 3;

            }
            if(keyDown==80)//键盘下
            {
                tem = choice;//记录上一次位置
                choice += 1;
                if(choice == 4)
                    choice = 1;

            }
            if(keyDown==72||keyDown==80)
            {
                setColor(0x7F);
                cursorMove(xStart+17,yStart+5+2*tem);
                puts(choices[tem-1]);

                setColor(0x70);
                cursorMove(xStart+17,yStart+5+2*choice);
                puts(choices[choice-1]);
            }


        }
        else if(keyDown==13)
        {

            if(choice !=3)
            {
                setColor(0x00);
                for(i=1; i<=9; i++)
                {
                    cursorMove(xStart+14,yStart+4+i);
                    printf("              ");

                }
                if(choice == 1)
                {
                    control(5,target);
                    printInterface(4,7,3,7,1);
                    if(lock[0] >0 )
                    {
                        printInterface(lock[0],lock[0],lock[1],lock[1],2);
                    }
                }
            }

            return choice;
        }
        keyDown = 0;
    }
}

void help()
{
    char keyDown;
    setColor(0x70);
    system("cls");
    cursorMove(34,1);
    printf("【规则说明】\n\n");
    Sleep(30);
    printf("━━游戏有6种不同的图块（");
    setColor(0x78);
    printf("▲");
    setColor(0x79);
    printf("▼");
    setColor(0x7A);
    printf("●");
    setColor(0x7B);
    printf("◆");
    setColor(0x7D);
    printf("■");
    setColor(0x7C);
    printf("★");
    setColor(0x70);
    printf("）,当有3个或以上相同的图块连在一起（竖直或");
    Sleep(30);
    printf("水平）时，则会消失变成分数，一次性消除图块越多，加成越多，还能出道具。");
    printf("（Ps：道具可以一次性消除许多图块噢(*^__^*) ）\n");
    Sleep(30);
    printf("━━本游戏有自由模式和闯关模式：自由模式需要在60s内尽量得高分；闯关模式由3");
    Sleep(30);
    printf("个关卡组成(第一关：60s内得13000分；第二关：45s内得10000分；第三关：30s内得8000分；)，通过3个关卡后的总分才为");
    Sleep(30);
    printf("有效。\n\n");

    cursorMove(34,9);
    printf("【操作说明】\n\n");
    Sleep(30);
    printf("                        ↑      \n");
    Sleep(30);
    printf("━━方向键移动光标 :  ←  →        锁定图块/使用道具 ：  Space（空格键）    \n");
    Sleep(30);
    printf("                        ↓    \n");
    Sleep(30);
    printf("━━选择 ：Enter（回车）            暂停/返回 ： Esc");
    Sleep(30);

    cursorMove(34,17);
    printf("【道具说明】\n\n");
    Sleep(30);
    printf("━━");
    setColor(0x7E);
    printf("¤");
    setColor(0x70);
    printf(" : 消除以道具为中心的9个图块(同时最多存在3个该道具)\n\n━━");
    Sleep(30);
    setColor(0x7E);
    printf("╬");
    setColor(0x70);
    printf(" : 消除以道具所在行、列的所有图块(同时最多存在2个该道具)\n\n━━");
    Sleep(30);
    setColor(0x7E);
    printf("▲▼●◆■★");
    setColor(0x70);
    printf(" : 消除所有与道具形状相同的图块(同时最多存在1个该道具)\n\n\n");
    Sleep(30);
    printf("[Esc]———返回");
    while((keyDown=getch())!=27)//按Esc返回主菜单
    {

    }
}

int gameOver()
{
    int choice;
    char keyDown;
    /*GAME*/
    setColor(0x70);
    system("cls");

    setColor(0x00);
    cursorMove(17,5);
    printf("          ");
    cursorMove(33,5);
    printf("  ");
    cursorMove(43,5);
    printf("  ");
    cursorMove(47,5);
    printf("  ");
    cursorMove(53,5);
    printf("          ");

    cursorMove(17,6);
    printf("  ");
    cursorMove(31,6);
    printf("  ");
    cursorMove(35,6);
    printf("  ");
    cursorMove(41,6);
    printf("  ");
    cursorMove(45,6);
    printf("  ");
    cursorMove(49,6);
    printf("  ");
    cursorMove(53,6);
    printf("  ");

    cursorMove(17,7);
    printf("  ");
    cursorMove(23,7);
    printf("    ");
    cursorMove(29,7);
    printf("  ");
    cursorMove(37,7);
    printf("  ");
    cursorMove(41,7);
    printf("  ");
    cursorMove(45,7);
    printf("  ");
    cursorMove(49,7);
    printf("  ");
    cursorMove(53,7);
    printf("        ");

    cursorMove(17,8);
    printf("  ");
    cursorMove(25,8);
    printf("  ");
    cursorMove(29,8);
    printf("          ");
    cursorMove(41,8);
    printf("  ");
    cursorMove(45,8);
    printf("  ");
    cursorMove(49,8);
    printf("  ");
    cursorMove(53,8);
    printf("  ");

    cursorMove(17,9);
    printf("          ");
    cursorMove(29,9);
    printf("  ");
    cursorMove(37,9);
    printf("  ");
    cursorMove(41,9);
    printf("  ");
    cursorMove(45,9);
    printf("  ");
    cursorMove(49,9);
    printf("  ");
    cursorMove(53,9);
    printf("          ");

    /*OVER*/
    cursorMove(17,11);
    printf("          ");
    cursorMove(29,11);
    printf("  ");
    cursorMove(37,11);
    printf("  ");
    cursorMove(41,11);
    printf("          ");
    cursorMove(53,11);
    printf("          ");


    cursorMove(17,12);
    printf("  ");
    cursorMove(25,12);
    printf("  ");
    cursorMove(29,12);
    printf("  ");
    cursorMove(37,12);
    printf("  ");
    cursorMove(41,12);
    printf("  ");
    cursorMove(53,12);
    printf("  ");
    cursorMove(61,12);
    printf("  ");

    cursorMove(17,13);
    printf("  ");
    cursorMove(25,13);
    printf("  ");
    cursorMove(29,13);
    printf("  ");
    cursorMove(37,13);
    printf("  ");
    cursorMove(41,13);
    printf("        ");
    cursorMove(53,13);
    printf("          ");

    cursorMove(17,14);
    printf("  ");
    cursorMove(25,14);
    printf("  ");
    cursorMove(31,14);
    printf("  ");
    cursorMove(35,14);
    printf("  ");
    cursorMove(41,14);
    printf("  ");
    cursorMove(53,14);
    printf("  ");
    cursorMove(57,14);
    printf("  ");

    cursorMove(17,15);
    printf("          ");
    cursorMove(33,15);
    printf("  ");
    cursorMove(41,15);
    printf("          ");
    cursorMove(53,15);
    printf("  ");
    cursorMove(59,15);
    printf("    ");

    /*选项*/
    setColor(0x70);
    cursorMove(28,19);
    printf("重新开始");
    setColor(0x7F);
    cursorMove(49,19);
    printf("退出");


    choice = 1;
    while((keyDown = getch()))
    {
        if(keyDown==-32)
        {

            keyDown=getch();
            if(keyDown==75 || keyDown==77)//键盘左或右
            {
                if(choice == 1)
                {
                    setColor(0x7F);
                    cursorMove(28,19);
                    printf("重新开始");
                    setColor(0x70);
                    cursorMove(48,19);
                    printf("退出");
                    choice =2;

                }
                else
                {
                    setColor(0x70);
                    cursorMove(28,19);
                    printf("重新开始");
                    setColor(0x7F);
                    cursorMove(48,19);
                    printf("退出");
                    choice =1;
                }
            }
        }
        else if(keyDown == 13)
        {
            return choice;
        }
        keyDown = 0;
    }


}

void congratulation(int round)
{
    int i;
    setColor(0x70);
    cursorMove(xStart+14,yStart+5);
    printf("╆═════╅");

    for(i=1; i<=7; i++)
    {
        cursorMove(xStart+14,yStart+5+i);
        printf("║          ║");
    }

    cursorMove(xStart+14,yStart+13);
    printf("╄═════╃");

    cursorMove(xStart+17,yStart+7);
    printf("Round %d",round);
    Sleep(1000);
    cursorMove(xStart+17,yStart+9);
    printf("PASS！！");
    cursorMove(xStart+17,yStart+11);
    printf("%d 分",score);

    Sleep(2000);
    if(round<3)
    {
        cursorMove(xStart+17,yStart+7);
        printf("Round %d",round+1);
        cursorMove(xStart+16,yStart+9);
        printf("          ");
        cursorMove(xStart+16,yStart+11);
        printf("          ");
        Sleep(1000);
        cursorMove(xStart+16,yStart+10);
        printf("   GO !!  ");
        Sleep(1000);

        setColor(0x00);
        for(i=1; i<=9; i++)
        {
            cursorMove(xStart+14,yStart+4+i);
            printf("              ");
        }
        printInterface(4,7,3,7,1);
    }
    return ;

}

int  judgeHonour(int pattern, int sumscore)
{
    FILE *fp;
    int i;
    char name[100];
    honourList head,p,q,t;
    head = getHonourList(pattern);
    p=head->next;
    while(p->next)
    {
        p=p->next;
    }
    if(sumscore >= p->score && sumscore>0)
    {
        setColor(0x70);
        system("cls");
        cursorMove(30,8);
        printf("Your score : %d",sumscore);
        cursorMove(10,12);
        printf("Please input your name(less than 6 letters): ");
        scanf("%s",name);
        while(strlen(name)>6)
        {
            system("cls");
            cursorMove(30,8);
            printf("Your score : %d",sumscore);
            cursorMove(10,12);
            printf("Please input your name(less than 6 letters): ");
            scanf("%s",name);
        }
        t=head;
        p=head->next;
        while(p->score > sumscore)
        {
            t=p;
            p=p->next;
        }
        q = (honourList)malloc(sizeof(honour));
        q->score=sumscore;
        strcpy(q->name,name);
        q->next=p;
        t->next=q;

        if(pattern==1)
        {
            fp=fopen("freehonourlist.txt","w");
        }
        else
        {
            fp=fopen("roundshonourlist.txt","w");
        }
            p=head->next;
            for(i=1;i<=5;i++)
            {
                fprintf(fp,"%s %d\n",p->name,p->score);
                p=p->next;
            }

        fclose(fp);
        return 1;
    }
    return 0;

}
