#include <conio.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define getpch(type) (type *)malloc(sizeof(type))

/*定义作业控制块JCB */
struct jcb
{
    char name[10];         //进程名
    char state;            //进程状态：W就绪、R运行、F完成
    int arrivetime;        //到达时间
    int needtime;          //需要运行的时间
    int runedtime;         //已经运行的时间
    int waittime;          //已经等待的时间
    int starttime;         //开始运行时间
    int finishtime;        //进程完成时间
    int turnovertime;      //周转时间
    double averweighttime; //带权周转时间
    double rate;           //响应比
    struct jcb *link;      //链接下一个进程控制块
} *ready = NULL, *p;
typedef struct jcb JCB;

double Rate(JCB *p)
{
    p->rate = (p->waittime + p->needtime) * 1.0 / p->needtime;
    return p->rate;
}

/* 对进程进行先来先服务排列*/
void FCFS()
{
    JCB *first, *second, *q;
    int insert = 0;
    q = p;

    if (q->state == 'W')
    {
        /*到达时间较早者,插入队首*/
        if ((ready == NULL) || q->arrivetime < ready->arrivetime)
        {
            q->link = ready;
            ready = q;
        }
        /*进程到达时间,插入适当的位置中*/
        else
        {
            first = ready;
            second = first->link;
            while (second != NULL)
            {
                /*若到达时间比当前进程到达时间小,插入到当前进程前面*/
                if (q->arrivetime < ready->arrivetime)
                {
                    q->link = second;
                    first->link = q;
                    second = NULL;
                    insert = 1;
                }
                /* 插入进程到达时间最长,则插入到队尾*/
                else
                {
                    first = first->link;
                    second = second->link;
                }
            }
            if (insert == 0)
                first->link = q;
        }
    }
}

void Exchange(JCB *p1, JCB *p2)
{
    int temp;
    char tempc;
    char t[10];
    double tempd;
    JCB *tempp;

    strcpy(t, p1->name);
    strcpy(p1->name, p2->name);
    strcpy(p2->name, t);

    tempc = p1->state;
    p1->state = p2->state;
    p2->state = tempc;

    temp = p1->arrivetime;
    p1->arrivetime = p2->arrivetime;
    p2->arrivetime = temp;

    temp = p1->needtime;
    p1->needtime = p2->needtime;
    p2->needtime = temp;

    temp = p1->runedtime;
    p1->runedtime = p2->runedtime;
    p2->runedtime = temp;

    temp = p1->waittime;
    p1->waittime = p2->waittime;
    p2->waittime = temp;

    temp = p1->starttime;
    p1->starttime = p2->starttime;
    p2->starttime = temp;

    temp = p1->finishtime;
    p1->finishtime = p2->finishtime;
    p2->finishtime = temp;

    temp = p1->turnovertime;
    p1->turnovertime = p2->turnovertime;
    p2->turnovertime = temp;

    tempd = p1->averweighttime;
    p1->averweighttime = p2->averweighttime;
    p2->averweighttime = tempd;

    tempd = p1->rate;
    p1->rate = p2->rate;
    p2->rate = tempd;

    tempp = p1->link;
    p1->link = p2->link;
    p2->link = tempp;
}

/* 对就绪队列进行响应比高者优先重新排列*/
void sortList()
{
    JCB *cur = ready;
    JCB *tail = ready;
    JCB *p = ready;

    //    DataType node = 0;
    int length = 0;
    int flag = 0;

    //尾结点,tail指向空
    while (tail != NULL)
    {
        tail = tail->link;
    }
    //外循环，循环链表长度-1次
    while (p->link != tail)
    {
        flag = 0;
        cur = ready;
        while (cur->link != tail)
        {
            if (cur->rate > cur->link->rate)
            {
                //前后交换
                Exchange(cur, cur->link);
                flag = 1;
            }
            cur = cur->link;
        }
        //尾指针向前移动
        tail = cur;
        //若为1，则证明链表已经有序
        if (flag == 0)
        {
        }
    }
}

/* 对进程进行响应比高者优先排列*/
void HRN()
{
    JCB *first, *second, *q;
    int insert = 0;
    q = p;
    if (q->state == 'W')
    {
        /*响应比高者,插入队首*/
        if ((ready == NULL) || (Rate(q) > Rate(ready)))
        {
            q->link = ready;
            ready = q;
        }
        /*进程比较响应比,插入适当的位置中*/
        else
        {
            first = ready;
            second = first->link;
            while (second != NULL)
            {
                /*若插入进程比当前进程运行所需时间小,插入到当前进程前面*/
                if ((Rate(q) > Rate(ready)))
                {
                    q->link = second;
                    first->link = q;
                    second = NULL;
                    insert = 1;
                }
                /* 插入进程运行所需时间最长,则插入到队尾*/
                else
                {
                    first = first->link;
                    second = second->link;
                }
            }

            if (insert == 0)
                first->link = q;
        }
    }
}

/* 建立进程控制块函数*/
void input1()
{
    int i, num;

    printf("\n 请输入总共的进程数：");
    scanf("%d", &num);
    for (i = 0; i < num; i++)
    {
        printf("\n 进程号No.%d:\n", i + 1);
        p = getpch(JCB);

        printf("\n 输入进程名:");
        scanf("%s", p->name);

        printf("\n 输入进程需要运行时间:");
        scanf("%d", &p->needtime);

        printf("\n 输入进程到达时间:");
        scanf("%d", &p->arrivetime);

        printf("\n\n");

        // 初始化余下的变量
        p->runedtime = 0;
        p->waittime = 0;
        p->state = 'W';
        p->link = NULL;

        FCFS();
    }
}

/* 建立进程控制块函数*/
void input2()
{
    int i, num;

    printf("\n 请输入总共的进程数：");
    scanf("%d", &num);
    for (i = 0; i < num; i++)
    {
        printf("\n 进程号No.%d:\n", i + 1);
        p = getpch(JCB);

        printf("\n 输入进程名:");
        scanf("%s", p->name);

        printf("\n 输入进程运行时间:");
        scanf("%d", &p->needtime);

        printf("\n 输入进程到达时间:");
        scanf("%d", &p->arrivetime);

        printf("\n\n");

        // 初始化余下的变量
        p->runedtime = 0;
        p->waittime = 0;
        p->state = 'W';
        p->link = NULL;

        HRN();
    }
}

/*返回链表长度*/
int length()
{
    int l = 0;
    JCB *pr = ready;
    while (pr != NULL)
    {
        l++;
        pr = pr->link;
    }
    return (l);
}

/*显示FCFS当前所有进程*/
void display(JCB *pr)
{
    printf("进程名：%s  ", pr->name);
    printf("|进程状态：%c ", pr->state);
    printf("|已经等待时间：%d  ", pr->waittime);

    printf("\n");
    printf("|运行所需时间：%d  ", pr->needtime);
    printf("|已运行时间：%d", pr->runedtime);
    printf("|到达时间：%d", pr->arrivetime);

    printf("\n\n");
}

/*显示HRN当前所有进程*/
void display2(JCB *pr)
{
    printf("进程名：%s  ", pr->name);
    printf("|响应比：%f  ", pr->rate);
    printf("|进程状态：%c ", pr->state);
    printf("|已经等待时间：%d  ", pr->waittime);

    printf("\n");
    printf("|运行所需时间：%d  ", pr->needtime);
    printf("|已运行时间：%d", pr->runedtime);
    printf("|到达时间：%d", pr->arrivetime);

    printf("\n\n");
}

double aver_turnovertime(JCB *q, int num)
{
    JCB *pr;
    pr = q;
    int sum = 0;
    while (pr != NULL)
    {
        sum += pr->finishtime - pr->arrivetime;
        pr = pr->link;
    }
    return sum * 1.0 / num;
}

double aver_weighttime(JCB *q, int num)
{
    JCB *pr;
    pr = q;
    double sum = 0.0;
    while (pr != NULL)
    {
        sum += (pr->finishtime - pr->arrivetime) * 1.0 / pr->runedtime;
        pr = pr->link;
    }
    return sum * 1.0 / num;
}

void displayAll(JCB *pr, int count, double aver_turnovertime, double averweighttime)
{

    printf("进程名：%s  \n", pr->name);
    printf("到达时间：%d", pr->arrivetime);
    printf("|进程状态：%c ", pr->state);
    printf("|已经等待时间：%d  ", pr->waittime);
    printf("|开始运行时间：%d  ", pr->starttime);
    printf("|完成时间：%d", pr->finishtime);
    printf("\n");
    printf("|周转时间：%d", pr->finishtime - pr->arrivetime);
    printf("|平均周转时间：%f\n", aver_turnovertime);
    printf("|带权周转时间：%f", (pr->finishtime - pr->arrivetime) * 1.0 / pr->runedtime);
    printf("|平均带权周转时间：%f", averweighttime);

    printf("\n\n");
}

/*查看目前运行的进程*/
void check(int mode)
{
    JCB *pr;
    if (p->state == 'R')
    {
        printf("\n-----------当前正在运行的进程是:%s-----------\n", p->name); /*显示当前运行进程*/
        if (mode == 1)
        {
            display(p);
        }
        else
        {
            display2(p);
        }
    }

    if (ready != NULL)
    {
        pr = ready;
        printf("\n-----------当前就绪队列状态-----------\n"); /*显示就绪队列状态*/
        if (mode == 1)
        {
            while (pr != NULL)
            {
                display(pr);
                pr = pr->link;
            }
        }
        else
        {
            while (pr != NULL)
            {
                display2(pr);
                pr = pr->link;
            }
        }
    }

    printf("\n——————————————————————————————\n");
    printf("\n");
}

/* 建立进程撤消函数(进程运行结束,撤消进程) */
void destroy()
{
    printf("\n 进程 [%s] 已完成.\n\n", p->name);
    p->state = 'F';
}

/* 运行函数，运行时间等于所需时间时撤销进程，否则就继续运行 */
int running(int mode, int count)
{
    if (p->arrivetime != count)
        (p->runedtime)++;
    if (p->runedtime == p->needtime)
    {
        destroy(); /* 调用destroy函数*/
        p->finishtime = count;
        return 1;
    }
    else
    {

        if (mode == 1)
        {
            FCFS();
            return 0;
        }
        if (mode == 2)
        {
            HRN();
            sortList();
            JCB *pr = ready;
            while (pr != NULL)
            {
                display2(pr);
                pr = pr->link;
            }
            return 0;
        }
    }
}

void wait(int count)
{
    JCB *pr;
    pr = ready;
    int wait;

    while (pr != NULL)
    {
        wait = count - pr->arrivetime;
        if (wait < 0)
        {
            pr->waittime = 0;
        }
        else
        {
            pr->waittime = wait;
        }
        pr = pr->link;
    }
}

/*主函数*/
int main()
{
    int len;
    int count = 0;
    char key;
    int flag = 0; //控制运行的形式是单步运行还是完整运行
    int flag1 = 0;
    int flag2 = 1;
    int flag3 = 1;
    int mode = 0;
    double aver1 = 0;
    double aver2 = 0;
    int num = 0;

    do
    {
        printf("\n——————————————————————————————\n");
        printf("                      请选用您想要的算法：\n");
        printf("             1.先来先服务（FCFS） 2.高响应比优先（HRN）\n");
        scanf("%d", &mode);

        if (mode == 1)
        {
            input1();
            flag1 = 0;
        }
        else if (mode == 2)
        {
            input2();
            flag1 = 0;
        }
        else
        {
            printf("请输入正确的序号！");
            flag1 = 1;
        }
    } while (flag1 == 1);

    JCB *pr = ready;
    JCB *pr1 = ready;
    JCB *pr2 = ready;
    JCB *pr0 = ready;
    if (mode == 1)
    {
        printf("最初的作业表：");
        while (pr != NULL)
        {
            display(pr);
            pr = pr->link;
        }
    }
    else
    {
        while (pr != NULL)
        {
            display2(pr);
            pr = pr->link;
        }
    }

    len = length();
    getchar(); //吸收换行符
    while (len != 0)
    {
        pr2 = pr0;
        count++;
        printf("\n——————————————————————————————\n");
        printf("\n执行数：%d \n", count);
        if (flag2 == 1)
        {
            p = ready;
        }

        if (p->arrivetime <= count)
        {
            //		p->link = NULL;

            ready = p->link;
            p->state = 'R';
            if (p->runedtime == 0 && flag2 == 1)
            {
                p->starttime = count;
            }
            flag2 = running(mode, count);

            wait(count);
        }
        while (pr2 != NULL)
        {
            Rate(pr2);

            pr2 = pr2->link;
        }

        check(mode);

        if (flag == 0)
        {
            printf("\n 按p键自动执行，按回车逐步执行：");
            if (getchar() == 'p')
                flag = 1;
        }
        if (p->link == NULL && p->state == 'F')
        {
            break;
        }
    }

    aver1 = aver_turnovertime(pr1, len);
    aver2 = aver_weighttime(pr1, len);
    printf("\n\n进程已经完成，最终的执行次数：%d次\n\n", count);
    while (pr1 != NULL)
    {
        displayAll(pr1, len, aver1, aver2);
        pr1 = pr1->link;
    }

    printf("\n按任意键退出！");
    getchar();
    getchar(); //防止运行窗口自动关闭
    return 0;
}
