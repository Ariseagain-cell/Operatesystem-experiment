#include <stdio.h>
#include <stdlib.h>

#define Free 0         //空闲状态
#define Busy 1         //已用状态
#define OK 1           //完成
#define ERROR 0        //出错
#define MAX_length 640 //最大内存空间为640KB
typedef int Status;

typedef struct freearea //定义一个空闲区说明表结构
{
    int ID;       //分区号
    long size;    //分区大小
    long address; //分区地址
    int state;    //状态
} ElemType;
typedef struct DuLNode //定义一个向链表存储结构
{
    ElemType data;
    struct DuLNode *prior; //前趋指针
    struct DuLNode *next;  //后继指针
} DuLNode, *DuLinkList;

DuLinkList block_first; //头结点
DuLinkList block_last;  //尾结点

Status alloc(int);          //内存分配
Status free1(int);          //内存回收
Status First_fit(int, int); //首次适应算法
Status Best_fit(int, int);  //最佳适应算法
void show();                //查看分配
Status Initblock();         //开创空间表

Status Initblock() //开创带头结点的内存空间链表
{
    block_first = (DuLinkList)malloc(sizeof(DuLNode));
    block_last = (DuLinkList)malloc(sizeof(DuLNode));
    block_first->prior = NULL;
    block_first->next = block_last;
    block_last->prior = block_first;
    block_last->next = NULL;
    block_last->data.address = 0;
    block_last->data.size = MAX_length;
    block_last->data.ID = 0;
    block_last->data.state = Free;
    return OK;
}

//----------------------- 分 配 主 存 -------------------------
Status alloc(int ch)
{
    int ID, request;
    printf("请输入作业(分区号)：");
    scanf("%d", &ID);
    printf("请输入需要分配的主存大小(单位:KB)：");
    scanf("%d", &request);
    if (request < 0 || request == 0)
    {
        printf("分配大小不合适，请重试！\n");
        return ERROR;
    }

    if (ch == 2) //最佳适应算法
    {
        if (Best_fit(ID, request) == OK)
            printf("分配成功！\n");
        else
            printf("内存不足，分配失败！\n");
        return OK;
    }
    else //首次适应算法
    {
        if (First_fit(ID, request) == OK)
            printf("分配成功！\n");
        else
            printf("内存不足，分配失败！\n");
        return OK;
    }
}
//------------------ 首次适应算法 -----------------------
Status First_fit(int ID, int request) //传入作业名及申请量
{
    //为申请作业开辟新空间且初始化
    DuLinkList temp = (DuLinkList)malloc(sizeof(DuLNode));
    temp->data.ID = ID;
    temp->data.size = request;
    temp->data.state = Busy;

    DuLNode *p = block_first->next;
    while (p)
    {
        if (p->data.state == Free && p->data.size == request)
        { //有大小恰好合适的空闲块
            p->data.state = Busy;
            p->data.ID = ID;
            return OK;
            break;
        }
        if (p->data.state == Free && p->data.size > request)
        { //有空闲块能满足需求且有剩余"
            temp->prior = p->prior;
            temp->next = p;
            temp->data.address = p->data.address;
            p->prior->next = temp;
            p->prior = temp;
            p->data.address = temp->data.address + temp->data.size;
            p->data.size -= request;
            return OK;
            break;
        }
        p = p->next;
    }
    return ERROR;
}
//--------------------  最佳适应算法  ------------------------
Status Best_fit(int ID, int request)
{
    int ch; //记录最小剩余空间
    DuLinkList temp = (DuLinkList)malloc(sizeof(DuLNode));
    temp->data.ID = ID;
    temp->data.size = request;
    temp->data.state = Busy;
    DuLNode *p = block_first->next;
    DuLNode *q = NULL; //记录最佳插入位置
    while (p)          //初始化最小空间和最佳位置
    {
        if (p->data.state == Free &&
            (p->data.size > request || p->data.size == request))
        {
            q = p;
            ch = p->data.size - request;
            break;
        }
        p = p->next;
    }
    while (p)
    {
        if (p->data.state == Free && p->data.size == request)
        { //空闲块大小恰好合适
            p->data.ID = ID;
            p->data.state = Busy;
            return OK;
            break;
        }
        if (p->data.state == Free && p->data.size > request)
        {                                    //空闲块大于分配需求
            if (p->data.size - request < ch) //剩余空间比初值还小
            {
                ch = p->data.size - request; //更新剩余最小值
                q = p;                       //更新最佳位置指向
            }
        }
        p = p->next;
    }
    if (q == NULL)
        return ERROR; //没有找到空闲块
    else
    { //找到了最佳位置并实现分配
        temp->prior = q->prior;
        temp->next = q;
        temp->data.address = q->data.address;
        q->prior->next = temp;
        q->prior = temp;
        q->data.address += request;
        q->data.size = ch;
        return OK;
    }
}

//-----------------------   主 存 回 收   --------------------
Status free1(int ID)
{
    DuLNode *p = block_first;
    while (p)
    {
        if (p->data.ID == ID)
        {
            p->data.state = Free;
            p->data.ID = Free;
            if (p->prior->data.state == Free) //与前面的空闲块相连
            {
                p->prior->data.size += p->data.size;
                p->prior->next = p->next;
                p->next->prior = p->prior;
            }
            if (p->next->data.state == Free) //与后面的空闲块相连
            {
                p->next->data.size += p->data.size;
                p->next->prior = p->prior;
                p->prior->next = p->next;
                p->next->data.address = p->data.address;
            }
            break;
        }
        p = p->next;
    }
    return OK;
}

//---------------  显示主存分配情况 ------------------
void show()
{
    printf("+++++++++++++++++++++++++++++++++++++++\n");
    printf("++++        主 存 分 配 情 况        ++++\n");
    printf("+++++++++++++++++++++++++++++++++++++++\n");
    DuLNode *p = block_first->next;
    while (p)
    {
        printf("分 区 号：");
        if (p->data.ID == Free)
            printf("Free");
        else
            printf("%d   ", p->data.ID);
        printf(" 起始地址：%ld ", p->data.address);
        printf(" 分区大小：%ld KB", p->data.size);
        printf(" 状    态：");
        if (p->data.state == Free)
            printf("空  闲");
        else
            printf("已分配");
        printf("\n");
        p = p->next;
    }
}

//----------------------- 主  函  数---------------------------
main()
{
    int ch; //算法选择标记
    printf("请选择分配算法：\n");
    printf("1.首次适应算法\n2.最佳适应算法\n");
    printf("您选择的序号：");
    scanf("%d", &ch);
    Initblock(); //开创空间表
    int choice;  //操作选择标记
    while (1)
    {
        printf("********************************************\n");
        printf("**    1: 分配内存        2: 回收内存         **\n");
        printf("**    3: 查看分配        0: 退    出        **\n");
        printf("********************************************\n");
        printf("请输入您选择的操作序号 ：");
        scanf("%d", &choice);
        if (choice == 1)
            alloc(ch);        // 分配内存
        else if (choice == 2) // 内存回收
        {
            int ID;
            printf("请输入您要释放的分区号：");
            scanf("%d", &ID);
            free1(ID);
            printf("回收内存成功！\n");
        }
        else if (choice == 3)
            show(); //显示主存
        else if (choice == 0)
            break; //退出
        else       //输入操作有误
        {
            printf("输入有误，请重试！");
            continue;
        }
    }
}
