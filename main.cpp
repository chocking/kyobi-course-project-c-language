#include <math.h>
#include "function.h"

void gameProcedure(int pattern)//游戏过程
{
    char keyDown;
    int target[2];
    int lock[2];
    //int icon[12][12];//只用下标1-10，其余置0
    int assist[12][12];//辅助数组，用来记录可消除的图块
    int x,y;
    int account;
    int flag,isTool;
    int rounds, round;
    int sumScore;

    srand( (unsigned)time( NULL ) );//取系统运行时间作为随机数的种子

    if(pattern==1)
    {
        //seconds = 60;
        rounds = 1;
    }
    else if(pattern==2)
    {
        rounds = 3;
    }
    for(x=0; x<=11; x++)
    {
        if(x==0||x==11)
        {
            for(y=0; y<=11; y++)
                icon[x][y]=0;
        }
        else
        {
            icon[x][0]=0;
            icon[x][11]=0;
        }
    }//矩阵四周置0


    sumScore = 0;
    for(round = 1 ; round <= rounds ; round ++)//根据选择的模式设置回合数
    {
        seconds = 75-15*round;//设置倒数秒数
        score = 0;
        numberOfTool_1=0,numberOfTool_2=0,numberOfTool_3=0;
        for(y=1; y<=10; y++)
        {
            for(x=1; x<=10; x++)
            {
                icon[x][y]=(int)((rand()%6)+1);

            }
        }//生成随机数组矩阵

        //icon[7][10]=11;//道具测试
        //icon[8][10]=11;//道具测试
        //icon[9][10]=11;//道具测试
        //icon[10][10]=26;//道具测试
        setColor(0x00);
        system("cls");

        target[0] = 1;
        target[1] = 1;
        lock[0] = -1;
        lock[1] = -1;
        printFrame(pattern , round);//打印游戏框架
        printInformation();//打印游戏信息（分数）
        printInterface(1,10,1,10,1);//打印数组
        control(0,target);//初始化移动光标

        flag=0;
        isTool=0;
        initAssist(assist);//初始化辅助数组
        judgeRemove(assist);//判断有无连续可消除图块
        //while(!kbhit()||(keyDown=getch())!=27)
        while(!kbhit()||(keyDown=getch()))
        {
            if(keyDown==27)//暂停
            {
                switch(pause(target, lock))
                {
                case 2:
                    gameProcedure(pattern);
                    return ;
                case 3:
                    return ;
                default:
                    break;
                }



            }

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
            if(seconds<0)
            {
                break ;//////**************
            }

            if(flag==0)
            {
                while((account = numberOfRemove(assist))!=0)
                {

                    flag=1;
                    remove(assist);//消除图块
                    calculateScore(account);//计算分数
                    if(pattern == 2)
                    {
                        if((round==1&&score>=roundScore_1)||(round==2&&score>=roundScore_2)||(round==3&&score>=roundScore_3))
                        {
                            setColor(0x82);
                            cursorMove(66+3*round,20);
                            printf("■");


                        }
                    }
                    printInformation();//打印游戏信息

                    fillBlank();//填补空缺位置
                    if(isTool==0)//非道具消除多个图块，随机位置出现道具
                    {
                        creatTool(account);//生成道具
                    }


                    initAssist(assist);//初始化辅助数组
                    judgeRemove(assist);//判断有无连续可消除图块
                    isTool=0;
                }
            }
            while(!judgeFeasibility())//判断游戏可玩性
            {
                rebuiltIcon(icon);//重新建立图块阵列(全部赋-1)
                fillBlank();
            }
            if(keyDown==-32)
            {
                switch(keyDown=getch())
                {
                case 75:
                    control(1,target);
                    break;//左
                case 72:
                    control(2,target);
                    break;//上
                case 77:
                    control(3,target);
                    break;//右
                case 80:
                    control(4,target);
                    break;//下

                default:
                    break;
                }


            }
            else if(keyDown==32)//按下空格键锁定图块
            {
                if(icon[target[0]][target[1]]/10)//选择的是道具
                {
                    if(lock[0]!=-1)//清除锁定标记
                    {
                        printInterface(lock[0],lock[0],lock[1],lock[1],1);
                        lock[0]=-1;
                    }
                    printInterface(target[0],target[0],target[1],target[1],2);
                    initAssist(assist);
                    toolRemove(assist,target[0],target[1]);
                    isTool=1;
                    flag=0;
                }
                else //选择的不是道具
                {

                    if(lock[0]==-1)//此前并未锁定图块
                    {
                        printInterface(target[0],target[0],target[1],target[1],2);
                        lock[0]=target[0];
                        lock[1]=target[1];

                    }
                    else//之前已锁定图块
                    {
                        if((lock[0]==target[0]&&abs(lock[1]-target[1])==1)||//相邻
                                (lock[1]==target[1]&&abs(lock[0]-target[0])==1))
                        {
                            // printInterface(target[0],target[0],target[1],target[1],1);
                            move(lock,target);
                            initAssist(assist);
                            if(judgeRemove(assist)==0)//移动后没有连续图块
                            {
                                Sleep(300);
                                move(lock,target);

                            }
                            else
                            {
                                isTool=0;
                                flag=0;
                            }
                            lock[0]=-1;

                        }
                        else if(lock[0]!=target[0]||lock[1]!=target[1])//相离
                        {

                            printInterface(target[0],target[0],target[1],target[1],2);
                            printInterface(lock[0],lock[0],lock[1],lock[1],1);
                            lock[0]=target[0];
                            lock[1]=target[1];
                        }
                        else //if(lock[0]==target[0]&&lock[1]==target[1])//同一位置
                        {
                            printInterface(lock[0],lock[0],lock[1],lock[1],1);
                            lock[0]=-1;
                        }


                    }
                }
            }


            keyDown=0;
        }//

        if(pattern == 2)
        {
            //分数达不到要求
            if((round==1&&score < roundScore_1) || (round==2&&score < roundScore_2) || (round==3&&score < roundScore_3))
            {
                if(gameOver()==1)//重新开始
                {
                    gameProcedure(pattern);
                    return ;
                }
                else
                    return ;
            }
            //通关 进入下一关
            else
                congratulation(round);
        }
        sumScore += score;

    }
     Sleep(1000);
    if(judgeHonour(pattern,sumScore)==0)
    {
        if(gameOver()==1)//重新开始
        {
            gameProcedure(pattern);
            return ;
        }
       else
            return ;
    }
    else
    {
        rank();
    }
}

int menu()//菜单功能
{
    int choice,tem;
    int i,j;
    char keyDown;
    char groupname[30]="designed by ___小菜鸡";
    char choices[5][9]= {"自由模式","闯关模式","光荣榜","帮助","退出"};
    int over =0 ;//退出
    while(over !=1)
    {

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
        printf("┳━┓                 ┏      ┓                     ┳   ");
        Sleep(100);
        cursorMove(10,2);
        printf("┃  ┃                 ┃ ╲╱ ┃                     ┃   ");
        Sleep(100);
        cursorMove(10,3);
        printf("┣━┛┏━┓ ┳ ┳┓   ┃  ┃  ┃ ┏━┓━╋━ ┏━┓ ┣━┓  ");
        Sleep(100);
        cursorMove(10,4);
        setColor(0x8F);
        printf("┃    ┃  ┃ ┃ ┃     ┃  ┻  ┃ ┃  ┃  ┃   ┃     ┃  ┃      ");
        Sleep(100);
        cursorMove(10,5);
        printf("┻    ┗━╋ ┻ ┻     ┻      ┻ ┗━╋  ┗━ ┗━┛ ┻  ┻      ");

        setColor(0x8E);
        cursorMove(23,0);
        printf("●");
        Sleep(100);

        cursorMove(23,0);
        printf("  ");
        cursorMove(23,1);
        printf("●");
        Sleep(80);

        cursorMove(23,1);
        printf("  ");
        cursorMove(23,2);
        printf("●");
        Sleep(50);

        cursorMove(23,2);
        printf("  ");
        cursorMove(23,1);
        printf("●");
        Sleep(250);

        cursorMove(23,1);
        printf("  ");
        cursorMove(23,2);
        printf("●");
        //Sleep(50);

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


        setColor(0x70);
        cursorMove(30,10);
        puts(choices[0]);//printf("自由模式");//60秒内尽可能拿高分

        setColor(0x7F);
        cursorMove(25,12);
        puts(choices[1]);//printf("闯关模式");

        cursorMove(27,14);
        puts(choices[2]);//printf("  光荣榜");//各模式历史前五名最高分

        cursorMove(29,16);
        puts(choices[3]);//printf("    帮助");

        cursorMove(29,18);
        puts(choices[4]);//printf("    退出");


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
                        choice = 5;

                }
                if(keyDown==80)//键盘下
                {
                    tem = choice;//记录上一次位置
                    choice += 1;
                    if(choice == 6)
                        choice = 1;

                }
                if(keyDown==72||keyDown==80)
                {
                    setColor(0x7F);
                    cursorMove(25,8+2*tem);
                    printf("%9s      ",choices[tem-1]);

                    setColor(0x70);
                    cursorMove(25,8+2*choice);
                    printf("%13s",choices[choice-1]);
                }


            }
            else if(keyDown==13)
            {
                switch(choice)
                {
                case 1:
                case 2:
                    j=0;
                    break;
                case 3:
                    j=2;
                    break;
                case 4:
                case 5:
                    j=4;
                    break;
                default:
                    break;

                }
                cursorMove(0,8+2*choice);
                setColor(0x70);
                for(i=1; i<=72+j; i++)
                {
                    if(i==31+j)
                    {
                        setColor(0x7C);
                        puts(choices[choice-1]);

                        setColor(0x70);
                        cursorMove(38,8+2*choice);
                    }
                    printf("_");
                    Sleep(5);


                }
                switch(choice)
                {
                case 1:
                    gameProcedure(1);
                    break;
                case 2:
                    gameProcedure(2);
                    break;
                case 3:
                    rank();
                    break;
                case 4:
                    help();
                    break;
                case 5:
                    over = 1;
                    break;//gameover();
                default:
                    break;
                }

                break;

            }
            keyDown = 0;
        }

    }
    return 0;

}



int main()
{
    HideCursor();//隐藏光标
    SetConsoleTitle("————————————— Pair Match ————————————— Designed By 小菜鸡");//设置窗口标题

    HANDLE con=GetStdHandle(STD_OUTPUT_HANDLE);// 获取标准输出设备句柄
    SMALL_RECT winPon= {0,0,79,26}; //窗口大小：宽度从0-79，高度从0-26
    COORD buf= {80,27}; //屏幕缓冲区大小：宽度80，高度27
    SetConsoleWindowInfo(con,1,&winPon);//设置窗口大小
    SetConsoleScreenBufferSize(con,buf);//设置屏幕缓冲区大小
    menu();
    //gameProcedure();
    return 0;
}
