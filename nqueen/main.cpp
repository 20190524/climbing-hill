
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<iostream>
using namespace std;

#define NUM_QUEENS 128  //皇后的个数，可以的就是有点久
#define MAX_NODES 1000	//队列大小，存放最好的邻居
#define MAX_SIDE_WAYS 100 //最多连续水平移动的次数
#define DEBUG_MODE 0   //为0，不输出调试信息，否则输出调试信息
#define MAX_TRY 100  //最大random restart 次数

//队列操作
#define put_node(st) _queue[_tl++]=st  //放入新的节点
#define get_node() _queue[_hd++];   //获取队列头部节点
#define reset_queue() _hd=_tl=0  //初始化队列
#define queue_not_empty (_hd < _tl)  //判断队列是否为空
#define reset_queue_head() _hd=0  //将队列头重置为0

//state&operator =(state _s,state _t){
//    _s.value=_t.value;
//    _s.pos=new int[NUM_QUEENS];
//    for(int i=0;i<NUM_QUEENS;i++){
//        _s.pos[i]=_t.pos[i];
//    }
//}


//状态
typedef struct state{
    int value; //该状态的价值，value越大，价值越高
    int pos[NUM_QUEENS]; //每一列皇后的位置，用一个数组储存
//    cout<<"creatastate"<<endl;
//    printf("creat a state\n");
}state;
void generateInit(state *);
    //随机产生一个初始状态, 将状态存入输入参数指针所指向的空间
state findBestNeighbor(state);
    //找到输入状态的最好邻居
int evaluate(state);
    //评估一个状态的价值
void printSolution(state );
    //打印输出一个状态的信息
int isConflict(state );
    //判断一个状态是否是要找的结果
int try_once();
    //产生一个初始状态，并找到局部最优解;若找到了一个目标结果
    //（皇后相互不冲突）返回1，否则返回0
int isEqual(state s1, state s2);
    //判断两个状态是否相同

//注意，全局变量的命名，均以_开始
state _queue[MAX_NODES];//队列，存放一个状态的邻居
int _hd=0, _tl = 0;//初始化队列头尾

int main(){
    int count = 0;//已经尝试的次数
    srand((unsigned int)(time(NULL)));//设置随机序列器的种子(注意程序其他地方不要再调用srand)

    while (count++<MAX_TRY){
        if (DEBUG_MODE){
            printf("*********************************\n");
            printf("begin the %d try.\n", count);
        }
        if (try_once()){
            break;//找到了目标
        }
    }

    if (DEBUG_MODE){
        printf("*********************");
    }

    return 0;
}


/*
基于一个初始状态，进行尝试
*/
int try_once(){

//    cout<<"odinary'svalue"<<odinary.value<<endl;
//    put_node(odinary);//放入一个节点
//    cout<<"numbernormal"<<_tl<<endl;
//    while (queue_not_empty) {
//        cout<<"NOTEMPTY"<<endl;
//        cout<<"formal head"<<_hd<<endl;
//        state head=get_node();
//        _hd++;
//        cout<<"now head"<<_hd<<endl;
//        cout<<"head'svalue"<<head.value<<endl;
//        if(isConflict(head))cout<<"conflict!!!"<<endl;

//        put_node(findBestNeighbor(odinary));
    state odinary;
    generateInit(&odinary);//生成一个随机状态，获得相互攻击的皇后数量
//        state newstate=findBestNeighbor(odinary);//
        while (isConflict(odinary)<0) {//检测是否为冲突条件
            odinary=findBestNeighbor(odinary);//找到最好邻居
            if(isConflict(odinary)==0){//不冲突则输出
                printSolution(odinary);
                return 1;
//                continue;
            }
        }
        return 0;
//        cout<<"newstatevalue"<<newstate.value<<endl;
//        put_node(newstate);
//        cout<<"numbertl"<<_tl<<endl;
//    }

}

//产生一个随机状态
void generateInit(state * p_st){
    int i,p;

    for (i=0;i<NUM_QUEENS;i++){
        p = rand() % NUM_QUEENS;
        p_st->pos[i] = p;
    }

    p_st->value = evaluate(*p_st);
}



/*
找到当前状态的最好的邻居
邻居的定义可以是固定7个列，更换一个列的皇后的位置，这样共有8*7=56个邻居
注意，若有多个邻居的价值均等于最大值，则随机从中选择一个
*/
state findBestNeighbor(state current){
//    cout<<"findbest!!!!!"<<endl;
    state nbr,temp;
    int i,j;
    int maxvalue=-10000;
    temp=current;
    for(i=0;i<NUM_QUEENS;i++)
    {
        for(j=0;j<current.pos[i];j++)
        {
            temp.pos[i]=j;//为什么只限制是小于的呢
            if((temp.value=evaluate(temp))>maxvalue)
            {
                maxvalue=temp.value;
                reset_queue();
                put_node(temp);
            }
            if((temp.value=evaluate(temp))==maxvalue)
            {
                put_node(temp);//大于和等于都设置为队列中的一员
            }
        }
        for(j=current.pos[i]+1;j<NUM_QUEENS;j++)//避开当前位置
        {
            temp.pos[i]=j;
            if((temp.value=evaluate(temp))>maxvalue)
            {
                maxvalue=temp.value;
                reset_queue();
                put_node(temp);
            }
            if((temp.value=evaluate(temp))==maxvalue)
            {
                put_node(temp);
            }
        }
        temp=current;
    }
    return _queue[rand()%(_tl)];//返回随机最好邻居
}


//判断两个状态是否相同,相同返回1，不同返回0
int isEqual(state s1, state s2){
    int i,num=0;
    for(i=0;i<NUM_QUEENS;i++)
    {
        if(s1.pos[i]==s2.pos[i])
            num++;
    }
    if(num==NUM_QUEENS)
        return 1;
    return 0;
}

//评估一个状态的价值
//可以将价值定义为 -1 * 可以相互攻击的皇后对数
//提示，可以用三个数组，分别结算每行的皇后个数，
//每条主对角线和副对角线上的皇后个数，然后计算可以相互攻击的皇后对数
//比如，若第一行有k个皇后，则该行上可相互攻击的皇后对数为 k*(k-1)/2
int evaluate(state   st){
    int i,j;
    int value=0;
    for(i=0;i<NUM_QUEENS-1;i++)//少一个皇后
    {
        for(j=i+1;j<NUM_QUEENS;j++)//这个皇后之后的皇后和它是否会冲突
        {
            if(st.pos[i]==st.pos[j])
                value++;
            if((st.pos[i]-st.pos[j])==(i-j))
                value++;
            if((st.pos[i]-st.pos[j])==(j-i))
                value++;
        }
    }
    st.value=(-1*value);
//	printf("%d\n",st.value);
    return (st.value);
}

//打印一个状态的信息
void printSolution(state st){
    int i;
    for (i=0;i<NUM_QUEENS;i++){
        printf("%d ", st.pos[i]);
    }
    printf("\n");
    printf("value is %d\n", st.value);
//    cout<<endl;
}

//判断一个状态中是否存在某对皇后，它们可以相互攻击
//注意，若isConflict返回0，则说明该状态为目标状态
int isConflict(state st){
//    printf("isconfilct\n");
    st.value = evaluate(st);

    return st.value;
}


