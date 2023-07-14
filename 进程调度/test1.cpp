#include <conio.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define getpch(type) (type *)malloc(sizeof(type))
#define N 50

/*定义进程控制块PCB */
struct pcb
{
    char name[10];    //进程名
    char state;       //进程状态：W就绪、R运行、F完成
    int super;        //表示优先级
    int needtime;     //需要运行的时间
    int runedtime;    //已经运行的时间
    struct pcb *link; //链接下一个进程控制块
} *ready = NULL, *p;
typedef struct pcb PCB;

/* 对进程进行短进程优先排列*/
void SPF()
{
    PCB *first, *second, *q;
    int insert = 0;
    q = p;

    /*进程最短者,插入队首*/
    if ((ready == NULL) || ((q->needtime) < (ready->needtime)))
    {
        q->link = ready;
        ready = q;
    }
    /*进程比较运行所需时间,插入适当的位置中*/
    else
    {
        first = ready;
        second = first->link;
        while (second != NULL)
        {
            /*若插入进程比当前进程运行所需时间小,插入到当前进程前面*/
            if ((q->needtime) < (second->needtime))
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

/* 建立进程控制块函数*/
void input()
{
    int i, num;

    printf("\n 请输入总共的进程数：");
    scanf("%d", &num);
    for (i = 0; i < num; i++)
    {
        printf("\n 进程号No.%d:\n", i + 1);
        p = getpch(PCB);

        printf("\n 输入进程名:");
        scanf("%s", p->name);

        printf("\n 输入进程运行时间:");
        scanf("%d", &p->needtime);

        p->super = rand() % 10 + 1;
        printf("\n 随机生成进程优先数:%d", p->super);

        printf("\n\n");

        p->runedtime = 0;
        p->state = 'W';
        p->link = NULL;

        SPF();
    }
}

/*返回链表长度*/
int length()
{
    int l = 0;
    PCB *pr = ready;
    while (pr != NULL)
    {
        l++;
        pr = pr->link;
    }
    return (l);
}

/*显示当前所有进程*/
void display(PCB *pr)
{
    printf("进程名：%s  ", pr->name);
    printf("|进程状态：%c ", pr->state);
    printf("|优先级：%d  ", pr->super);
    printf("\n");
    printf("|运行所需时间：%d  ", pr->needtime);
    printf("|已运行时间：%d", pr->runedtime);
    printf("\n\n");
}

/*查看目前运行的进程*/
void check()
{
    PCB *pr;

    printf("\n-----------当前正在运行的进程是:%s-----------\n", p->name); /*显示当前运行进程*/
    display(p);

    pr = ready;
    printf("\n-----------当前就绪队列状态-----------\n"); /*显示就绪队列状态*/
    while (pr != NULL)
    {
        display(pr);
        pr = pr->link;
    }

    printf("\n——————————————————————————————\n");
    printf("\n");
}

/* 建立进程撤消函数(进程运行结束,撤消进程)*/
void destroy()
{
    printf("\n 进程 [%s] 已完成.\n\n", p->name);
    p->state = 'F';
    free(p);
}

/* 运行函数，运行时间等于所需时间时撤销进程，否则就继续运行*/
void running()
{
    (p->runedtime)++;
    if (p->runedtime == p->needtime)
    {
        destroy(); /* 调用destroy函数*/
    }
    else
    {
        //		(p->super)--;
        p->state = 'W';
        SPF(); /*调用最短进程优先函数*/
    }
}

/*主函数*/
int main()
{
    int len;
    int count = 0;
    char key;
    int flag = 0; //控制运行的形式是单步运行还是完整运行
    input();
    len = length();
    getchar(); //吸收换行符
    while ((len != 0) && (ready != NULL))
    {
        count++;
        printf("\n——————————————————————————————\n");
        printf("\n执行数：%d \n", count);

        p = ready;
        ready = p->link;
        p->link = NULL;
        p->state = 'R';

        check();
        running();

        if (flag == 0)
        {
            printf("\n 按p键自动执行，按回车逐步执行：");
            if (getchar() == 'p')
                flag = 1;
        }
    }
    printf("\n\n进程已经完成，最终的执行次数：%d次\n", count);
    printf("\n按任意键退出！");
    getchar();
    getchar(); //防止运行窗口自动关闭
    return 0;
}
