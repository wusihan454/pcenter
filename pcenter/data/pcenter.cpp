#include <iostream>
#include <vector>
#include <cstring>
#include <time.h>
#include <stdlib.h>
using namespace std; 
struct SWAP
{
	int i;
	int j;
};
vector<SWAP> M;//寻找最好的交换对数时用来存放最好交换对数不止一个的情况，用于find_best_swap 
vector<int> tabu;
int point,centernumber;//point为图结点数，centernumber为中心数
vector<vector<int> > map;

vector<bool> selected;
vector<bool> best_selected;//记录目前为止最好的一次结果 
vector<bool> last_selected;//记录上一次的结果； 
set<int> uncovered;
set<int> last_uncovered;
vector<int> W;//所有节点的权重值 
vector<int> delta;//所有节点的delta值 
vector<int> delta_copy;
int f;//代价函数值 
int iter;//迭代次数 
int best_uncovered_num;
/*
第一行给出两个由空格分隔的整数 N 和 P, 分别表示节点数和中心数 
(从集合覆盖的角度来看, N 既是集合数又是元素数,P为可挑选出的集合数).
接下来每两行一组, 连续 N 组给出每个集合的覆盖范围.
每组中第一行为该集合能覆盖的元素数量 C, 第二行为空格分隔的 C 个数字, 分别表示该集合能覆盖的元素的编号.
*/ 
void printtest()
{
	//检查初始贪心解构造是否正确
	for(int i=0;i<point;i++)
	{
			cout<<selected[i]<<" "<<map[i].size()<<endl; 
	}
}


void inital()
{
	
        selected.resize(point); 
        uncovered.resize(point); 
        last_selected.resize(point); 
        best_selected.resize(point); 
        tabu.resize(point);
		//cout<<"*****"<<V[mark_start].count<<"   "<<bb<<endl;
//之前的策略是有问题的需要变为每次选择,初始化贪心选择 
        for(int i=0;i<centernumber;i++)
		{
			int lastselectcount=-1;
			int lastnumber=-1;
			for(int j=0;j<point;j++)
			{
				if(!selected[j])//统计j附近未被覆盖的节点数 
				{
					int bbb=0;//统计j附近未被覆盖的节点数 
					int ccc=1;
					for(int k=0;k<map[j].size();k++)
					{
						if(uncovered[map[j][k]]) bbb++;
					}
					if(bbb>lastselectcount)
					{
						ccc=1;
						lastselectcount=bbb;
						lastnumber=j;
					}
					else if(bbb==lastselectcount)//等概率随机选择一个； 
					{
						ccc++;
						if(rand()%ccc==0)
						{
							lastselectcount=bbb;
							lastnumber=j;
						}
					}	
				}
			}
			selected[lastnumber]=true;
			cout<<lastnumber<<" "<<map[lastnumber].size()<<" count:"<<lastselectcount<<endl;
			for(int j=0;j<map[lastnumber].size();j++)
			{
				uncovered[map[lastnumber][j]]=false;
			}
		}	
/*初始时所有节点权重为1:*/ 
		W.resize(point);
		delta.resize(point); 
		delta_copy.resize(point); 
		last_uncovered.resize(point);
		for(int i=0;i<point;i++)
		{
			W[i]=1;//初始时每个节点的w值为1 
			delta[i]=0; 
			tabu[i]=0;//初始时禁忌初始化
		} 

/*计算初始时每个节点的delta值*/ 		
		for(int i=0;i<point;i++) 
		{
			if(selected[i])//对于已经选中的节点的delta值，统计只能被该节点覆盖的节点的w之和
			{
				for(int j=0;j<map[i].size();j++)//判断map[i][j]这个节点是不是只能被i节点覆盖 
				{
					if(!selected[map[i][j]]) //如果这个节点是center,直接跳过 
					{
						int u=map[i][j];
						int addcount=0;
						for(int k=0;k<map[u].size();k++)
						{
							if(selected[map[u][k]])
							addcount++;
					 	} 
					 	if(addcount==1) delta[i]=delta[i]+W[u];
					}
				}
				bool booli=true;//判断i本身是不是只有自己为自己服务 
				for(int j=0;j<map[i].size();j++)//j=0是他自己 
				{
				   if(selected[map[i][j]]&&map[i][j]!=i) 
				   {
				   		booli=false;
				   		break;
				   }	    
				}
				if(booli) delta[i]=delta[i]+W[i];	
			} 
			else//未选中节点的delta值计算 ，计算未选中节点覆盖范围内目前没有被覆盖的节点 
			{
			
				for(int j=0;j<map[i].size();j++)
				{
					int u=map[i][j];	
					if(uncovered[u])  
					delta[i]=delta[i]+W[u];
				}
				//去掉下面，因为未选中节点自身本身在adjacent里面存储了； 
			/*	if(uncovered[i])//未选中节点自身也可能是未被覆盖节点 
				{
					delta[i]=delta[i]+W[i];
				}*/
			}	
		}

	f=0; 
	for(int i=0;i<point;i++)
	{
		if(uncovered[i])
		f=f+W[i]; 
	} 
/*给最好的解以及上一次的解赋初值*/
	for(int i=0;i<point;i++)
	{
		best_selected[i]=selected[i];
		last_selected[i]=selected[i];
	}	 
}

void  trytoopencenter(int addi)//试图将i节点加入center但是并，没有真正加入 
{
	
	for(int j=0;j<map[addi].size();j++)//对于addi范围内的所有节点
	{
		int addcount=0;
		int mark=-1;//记住需要修改delta值的节点的标号 
		int u=map[addi][j];//addi范围内节点记为u 
		for(int i=0;i<map[u].size();i++)//对一个节点覆盖范围内的节点 
		{
			if(selected[map[u][i]]) 
			{
				addcount++;
				mark=map[u][i];
			}
		}
		if(addcount==1)
		{
//有可能这个节点自己是服务节点，没加入addi之前这个节点自己周围没有服务节点  ，还需要问下，不一定！！！！！！！！！！！！！！！！ 
					delta[mark]=delta[mark]-W[u];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		}	
	} 	
} 


int find_best_SWAP()//返回要选择的交换对的下标 ,选择的要交换的不一定有上一次的好 
{
	M.resize(0);
	int best_obj=-1;//无穷
	int count=0;
	int uncoveredv=-1;
	int mark=-1;
	/*从uncovered节点里面随机选择一个节点uncoveredv*/
	for(int i=0;i<point;i++)
	{
		if(uncovered[i])
		{
			count++;
			if(count==1) mark=i;
			if(rand()%count==0&&count!=1)
			{
				uncoveredv=i;
				break;
			}
		}
	 } 
	 if(uncoveredv==-1)//抽中第一个节点 
	 {
	 	uncoveredv=mark;
	 }
	 /* 将目前的delta表格复制备份 */
	for(int i=0;i<point;i++)
	{
		delta_copy[i]=delta[i];
	}
	int u;
	for(int i=0;i<map[uncoveredv].size();i++)
	{
		u=map[uncoveredv][i];//未覆盖节点周围节点，一定是非中心节点 
			//cout<<"u"<<u<<" ";
		//如果没有被覆盖节点周围只有他自己，并且上一次刚刚是被del了也应该选择该节点
		if(tabu[u]<iter) 
		{//没有被禁忌 
			trytoopencenter(u);
			//cout<<"u"<<u<<" ";
			for(int j=0;j<point;j++)
			{
				if(selected[j]&&tabu[j]<iter)//j没有被禁忌 ，且是center节点 
				{
				    if((best_obj>f-delta[u]+delta[j])||best_obj==-1)//达到局部最优 
				    {
				    	best_obj=f-delta[u]+delta[j];
				    	M.resize(0);
				    	SWAP node;
				    	node.i =u;
				    	node.j=j;
				    	M.push_back(node); 
					}
					else if(best_obj==f-delta[u]+delta[j])
				    {
				    	SWAP node;
				    	node.i =u;
				    	node.j=j;
				    	M.push_back(node); 
					}
				}
			} 
				/*将delta还原*/
			for(int i=0;i<point;i++)//trytoopencenter(i)一次表格就会修改一次，就要复原一次 
			{
				delta[i]=delta_copy[i];
			}
		}		
	}
   	int selresult=-1;
	if(M.size()>0)    selresult=rand()%M.size();//选择的交换对的下标
	    //cout<<M.size() <<" select:"<<selresult<<endl; 
	return selresult;
}

void makeSWAP(int addi,int delj)//加入addi节点，删除delj节点
{
	for(int j=0;j<map[addi].size();j++)//对于addi范围内的所有节点 
	{
		int addcount=0;
		int mark=-1;//记住需要修改delta值的节点的标号 
		int u=map[addi][j];//addi范围内节点记为u 
		for(int i=0;i<map[u].size();i++)//对一个节点覆盖范围内的节点 
		{
			if(selected[map[u][i]]) 
			{
				addcount++;
				mark=map[u][i];
			}
		}
		if(addcount==1)
		{
//有可能这个节点自己是服务节点，没加入addi之前这个节点自己周围没有服务节点  ，还需要问下，不一定！！！！！！！！！！！！！！！！ 
			delta[mark]=delta[mark]-W[u];
		}	
/*addi范围内的一个节点周围没有服务节点，自己也不是服务节点 */ 
/*  该节点的覆盖范围内除了该节点的节点的delta值原本为附近所有的未被覆盖节点的权值之和  */
		else if(addcount==0)
		{
			for(int k=0;k<map[u].size();k++) 
			{
				if(map[u][k]!=addi)
				{
					delta[map[u][k]]=delta[map[u][k]]-W[u];
				}
			}
		}		
	} 
//更新
	selected[addi]=true;
	selected[delj]=false;
	for(int j=0;j<map[delj].size();j++)//对于delj范围内的所有节点
	{
		int addcount=0;
		int mark=-1;//记住需要修改delta值的节点的标号 
		int u=map[delj][j];//addi范围内节点记为u 
		for(int i=0;i<map[u].size();i++)//对一个节点覆盖范围内的节点 
		{
			if(selected[map[u][i]]) 
			{
				addcount++;
				mark=map[u][i];
			}
		}
		if(addcount==1)
		{
//有可能这个节点自己是服务节点，没加入addi之前这个节点自己周围没有服务节点  ，还需要问下，不一定！！！！！！！！！！！！！！！！ 
			delta[mark]=delta[mark]+W[u];
		}	
/*addi范围内的一个节点周围没有服务节点，自己也不是服务节点 */ 
/*  该节点的覆盖范围内除了该节点的节点的delta值原本为附近所有的未被覆盖节点的权值之和  */
		if(addcount==0)
		{
			for(int k=0;k<map[u].size();k++) 
			{
				if(map[u][k]!=delj)
				{
					delta[map[u][k]]=delta[map[u][k]]+W[u];
				}
			}
		}		
	} 
} 
void check()
{
	bool mark=true;
	int uncoverednum=0;
	for(int i=0;i<point;i++)
	{
		if(!selected[i])
		{
			int add=0;
			for(int j=0;j<map[i].size();j++)
			{
				if(selected[map[i][j]]) add++;
			}
			if(add==0) 
			{
				mark=false;
				uncoverednum++;
				
			}
		}
	}
	if(mark) printf("all covered!\n");
	else printf("uncovered number:%d\n",uncoverednum);
}

int main()
{
	freopen("pmed33.n700p70.txt","r",stdin);
    //freopen("o.txt","w",stdout);
	scanf("%d%d",&point,&centernumber);
	map.resize(point);
	for(int j=0;j<point;j++)
	{
		int c=0;
		scanf("%d",&c);
		int nextnode;
		for(int k=0;k<c;k++)
		{
			scanf("%d",&nextnode);
			map[j].push_back(nextnode); 	
		}
	}
    inital();
    //到这里没啥问题 
   // printtest();
   iter=1;
   best_uncovered_num=-1;
   while(f!=0)
   {
   	    for(int i=0;i<point;i++)
   	    {
   	    	last_uncovered[i]=uncovered[i];
		}
   	    int value=find_best_SWAP();
   	    if(value==-1)  
   	    {
   	    	iter++;//可以选择的都被禁忌了 
   	    	continue;
		}
   	    makeSWAP(M[value].i ,M[value].j);
   	    int adddi=M[value].i;
   	    int dellj=M[value].j;
   	    uncovered[adddi]=false;
   	    for(int i=0;i<map[adddi].size();i++)
   	    {
   	    	uncovered[map[adddi][i]]=false;
		}
		for(int i=0;i<map[dellj].size();i++)
		{
			int aaaa=map[dellj][i];
			bool aa=true;
			if(!selected[aaaa])
			{
				for(int j=0;j<map[aaaa].size();j++)
				{
					if(selected[map[aaaa][j]]) {
						aa=false;
						break;
					}
				}
				if(aa)  uncovered[aaaa]=true;
			}
		} 
		//cout<<"makeswap "<<adddi<<" "<<dellj<<endl;
    //分别计算这一次的uncovered和上一次的uncovered
    int last_num=0;
    int now_num=0;
    for(int i=0;i<point;i++)
    {
    	if(uncovered[i]) now_num++;
    	if(last_uncovered[i]) last_num++;
	}
	if(best_uncovered_num==-1||best_uncovered_num>now_num)//达到目前最优解 
	best_uncovered_num=now_num; 
	else if(last_num<now_num) //破除局部最优 
	{
		for(int i=0;i<point;i++)
		{
			if(uncovered[i]) //未被覆盖节点权重全部加一，未覆盖节点范围内的所有节点，delta值全部加一 
			{
				W[i]++;
				for(int k=0;k<map[i].size();k++)//
				{
					delta[map[i][k]]++; 
				} 
			} 
		}
	}
	f=0;
	for(int i=0;i<point;i++)//计算f 
	{
		if(uncovered[i])
		{
			f=f+W[i]; 
		}
	} 
	tabu[M[value].i]=iter+2;
	tabu[M[value].j]=iter+2;
	//printf("now_num %d f: %d\n",now_num,f);
   	iter++;
   }
   check();
   for(int i=0;i<point;i++)
   {
   	if(selected[i]) cout<<i<<" ";
   }
  // cout<<endl<<"iter:"<<iter; 
	return 0;
}

/*
int main(int argc,char* argv[])
{

	clock_t start_t=clock();
	char datapath[500],resultpath[500];
	strcpy(datapath,argv[1]);
	strcpy(resultpath,argv[2]);
	int timelimit=atoi(argv[3]);
	unsigned seed=atoi(argv[4]);
	srand(seed);
	clock_t end_t;
	freopen(datapath,"r",stdin);
	freopen(resultpath,"w",stdout);
	scanf("%d%d",&point,&centernumber);
	map.resize(point);
	for(int j=0;j<point;j++)
	{
		int c=0;
		scanf("%d",&c);
		int nextnode;
		for(int k=0;k<c;k++)
		{
			scanf("%d",&nextnode);
			map[j].push_back(nextnode); 	
		}
	
	}
    inital();
    //到这里没啥问题 
   // printtest();
   iter=1;
   best_uncovered_num=-1;
   while(f!=0)
   {
   	    for(int i=0;i<point;i++)
   	    {
   	    	last_uncovered[i]=uncovered[i];
		}
   	    int value=find_best_SWAP();
   	    if(value==-1)  
   	    {
   	    	iter++;//可以选择的都被禁忌了 
   	    	continue;
		}
   	    makeSWAP(M[value].i ,M[value].j);
   	    int adddi=M[value].i;
   	    int dellj=M[value].j;
   	    uncovered[adddi]=false;
   	    for(int i=0;i<map[adddi].size();i++)
   	    {
   	    	uncovered[map[adddi][i]]=false;
		}
		for(int i=0;i<map[dellj].size();i++)
		{
			int aaaa=map[dellj][i];
			bool aa=true;
			if(!selected[aaaa])
			{
				for(int j=0;j<map[aaaa].size();j++)
				{
					if(selected[map[aaaa][j]]) {
						aa=false;
						break;
					}
				}
				if(aa)  uncovered[aaaa]=true;
			}
		} 
		//cout<<"makeswap "<<adddi<<" "<<dellj<<endl;
    //分别计算这一次的uncovered和上一次的uncovered
    int last_num=0;
    int now_num=0;
    for(int i=0;i<point;i++)
    {
    	if(uncovered[i]) now_num++;
    	if(last_uncovered[i]) last_num++;
	}
	if(best_uncovered_num==-1||best_uncovered_num>now_num)//达到目前最优解 
	best_uncovered_num=now_num; 
	else if(last_num<now_num) //破除局部最优 
	{
		for(int i=0;i<point;i++)
		{
			if(uncovered[i]) //未被覆盖节点权重全部加一，未覆盖节点范围内的所有节点，delta值全部加一 
			{
				W[i]++;
				for(int k=0;k<map[i].size();k++)//
				{
					delta[map[i][k]]++; 
				} 
			} 
		}
	}
	f=0;
//	cout<<"uncover: " ;
	for(int i=0;i<point;i++)//计算f 
	{
		if(uncovered[i])
		{
			//	cout<<i<<" "; 
			f=f+W[i]; 
		}
	} 
	tabu[M[value].i]=iter+2;
	tabu[M[value].j]=iter+2;
   	iter++;
   	//printf("now_num %d f: %d\n",now_num,f);
   	end_t=clock();//时钟检测
	   	if(end_t>start_t+timelimit*CLOCKS_PER_SEC)
		{
		
			break; 
		}
		 
   }
   check();
   for(int i=0;i<point;i++)
   {
   	if(selected[i]) cout<<i<<" "; 
   }

  // cout<<endl<<"iter:"<<iter; 
	return 0;
}*/
