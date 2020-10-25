#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include<malloc.h>
#include<cstdlib>
struct Queue {
    int* const  elems;	   	//elems申请内存用于存放队列的元素
    const  int  max;	  	//elems申请的最大元素个数max
    int   head, tail;//队列头head和尾tail，队空head=tail;初始head=tail=0
};

void initQueue(Queue* const p, int m)//初始化p指队列：最多申请m个元素
{
    int** pr=(int**)&p->elems;
   *pr= new int[m];
   int* temp = (int*)&p->max;
   *temp = m;
   p->head = 0;
   p->tail = 0;
}

void initQueue(Queue* const p, const Queue& s) //用s深拷贝初始化p指队列
{//注意函数默认已经构造了一个queue的实体由p指向
    int* temp1=(int *)&p->max;
    int** temp2;//两个用来修改const变量的指针
    temp2 = (int**)&p->elems;
    *temp1 = s.max;
    *temp2 = new int[s.max];
    p->head = s.head;
    p->tail = s.tail;
    for (int i = 0; i < p->max; i++)
        p->elems[i] = s.elems[i];
}

void initQueue(Queue* const p, Queue&& s) //用s移动初始化p指队列
{
    int* temp1;
    int** temp2;//两个用来修改const变量的指针
    temp1 = (int*)&p->max;
    temp2 = (int**)&p->elems;
    p->head = s.head;
    p->tail = s.tail;
    *temp1 = s.max;
    *temp2 = s.elems;//窃取内存
    temp2 = (int**)&s.elems;
    *temp2 = 0;//让成为安全可解析状态
    temp1 = (int*)&s.max;
    *temp1 = s.tail = s.head = 0;
}

//先暂时默认tail指向的是队尾元素的后一位
int  number(const Queue* const p)//返回p指队列的实际元素个数
{
    return (p->tail - p->head + p->max) / p->max;
}


int  size(const Queue* const p)			//返回p指队列申请的最大元素个数max
{
    return p->max;
}


Queue* const enter(Queue* const p, int e) //将e入队列尾部，并返回p
{
    if ((p->tail + 1) % p->max == p->head) return p;//队列满了不能插入直接返回
    p->elems[p->tail] = e;
    p->tail = (p->tail + 1) % p->max;
    return p;
}


Queue* const leave(Queue* const p, int& e)//从队首出元素到e，并返回p
{
    if (p->tail == p->head) return p;//空队列不能出元素
    e = p->elems[p->head];
    p->head = (p->head + 1) % p->max;
}


Queue* const assign(Queue* const p, const Queue& q) //深拷贝赋s给队列并返回p
{
    if (p->elems) delete p->elems;//如果p内存不为空则应该先释放
    int* temp1 = (int*)&p->max;
    int** temp2 = (int**)&p->elems;
    *temp1 = q.max;
    *temp2 = new int[q.max];
    p->head = q.head;
    p->tail = q.tail;
    for (int i = 0; i < q.max; i++)
        p->elems[i] = q.elems[i];
    return p;
}


Queue* const assign(Queue* const p, Queue&& q) //移动赋s给队列并返回p
{
    if (p->elems) delete p->elems;
    int* temp1 = (int*)&p->max;
    int** temp2 = (int**)&p->elems;
    *temp1 = q.max;
    *temp2 = q.elems;//使用内存
    p->head = q.head;
    p->tail = q.tail;
    temp2 = (int**)&q.elems;
    temp1 = (int*)&q.max;
    *temp2 = 0;//成为安全可析构的状态
    *temp1 = 0;
    q.head = q.tail = 0;
    return p;
}


char* print(const Queue* const p, char* s)//打印p指队列至s并返回s
{
    int t = 0,i=p->head;
    while (i != p->tail)
    {
        s[t++] = p->elems[i]+'0';
        i = (i + 1) % p->max;
        s[t++] = ',';
    }
    s[t] = 0;
    return s;
}


void destroyQueue(Queue* const p)	 //销毁p指向的队列
{
    delete p->elems;//先释放元素的内存
    delete p;
}
extern const char* TestQueue(int& s);

int main()
{
    int t = 0;
    const char* s = TestQueue(t);
    printf("%s", s);
    getchar();
   /* Queue* p = (Queue*)malloc(sizeof(Queue));
    initQueue(p, 10);
    Queue tp{ new int[20],20,4,4 };
    //initQueue(p, Queue{ new int[10],10,1,3 });
    assign(&tp, *p);*/
    return 0;
}