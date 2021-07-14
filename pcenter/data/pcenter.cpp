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
vector<SWAP> M;//Ѱ����õĽ�������ʱ���������ý���������ֹһ�������������find_best_swap 
vector<int> tabu;
int point,centernumber;//pointΪͼ�������centernumberΪ������
vector<vector<int> > map;

vector<bool> selected;
vector<bool> best_selected;//��¼ĿǰΪֹ��õ�һ�ν�� 
vector<bool> last_selected;//��¼��һ�εĽ���� 
set<int> uncovered;
set<int> last_uncovered;
vector<int> W;//���нڵ��Ȩ��ֵ 
vector<int> delta;//���нڵ��deltaֵ 
vector<int> delta_copy;
int f;//���ۺ���ֵ 
int iter;//�������� 
int best_uncovered_num;
/*
��һ�и��������ɿո�ָ������� N �� P, �ֱ��ʾ�ڵ����������� 
(�Ӽ��ϸ��ǵĽǶ�����, N ���Ǽ���������Ԫ����,PΪ����ѡ���ļ�����).
������ÿ����һ��, ���� N �����ÿ�����ϵĸ��Ƿ�Χ.
ÿ���е�һ��Ϊ�ü����ܸ��ǵ�Ԫ������ C, �ڶ���Ϊ�ո�ָ��� C ������, �ֱ��ʾ�ü����ܸ��ǵ�Ԫ�صı��.
*/ 
void printtest()
{
	//����ʼ̰�Ľ⹹���Ƿ���ȷ
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
//֮ǰ�Ĳ��������������Ҫ��Ϊÿ��ѡ��,��ʼ��̰��ѡ�� 
        for(int i=0;i<centernumber;i++)
		{
			int lastselectcount=-1;
			int lastnumber=-1;
			for(int j=0;j<point;j++)
			{
				if(!selected[j])//ͳ��j����δ�����ǵĽڵ��� 
				{
					int bbb=0;//ͳ��j����δ�����ǵĽڵ��� 
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
					else if(bbb==lastselectcount)//�ȸ������ѡ��һ���� 
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
/*��ʼʱ���нڵ�Ȩ��Ϊ1:*/ 
		W.resize(point);
		delta.resize(point); 
		delta_copy.resize(point); 
		last_uncovered.resize(point);
		for(int i=0;i<point;i++)
		{
			W[i]=1;//��ʼʱÿ���ڵ��wֵΪ1 
			delta[i]=0; 
			tabu[i]=0;//��ʼʱ���ɳ�ʼ��
		} 

/*�����ʼʱÿ���ڵ��deltaֵ*/ 		
		for(int i=0;i<point;i++) 
		{
			if(selected[i])//�����Ѿ�ѡ�еĽڵ��deltaֵ��ͳ��ֻ�ܱ��ýڵ㸲�ǵĽڵ��w֮��
			{
				for(int j=0;j<map[i].size();j++)//�ж�map[i][j]����ڵ��ǲ���ֻ�ܱ�i�ڵ㸲�� 
				{
					if(!selected[map[i][j]]) //�������ڵ���center,ֱ������ 
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
				bool booli=true;//�ж�i�����ǲ���ֻ���Լ�Ϊ�Լ����� 
				for(int j=0;j<map[i].size();j++)//j=0�����Լ� 
				{
				   if(selected[map[i][j]]&&map[i][j]!=i) 
				   {
				   		booli=false;
				   		break;
				   }	    
				}
				if(booli) delta[i]=delta[i]+W[i];	
			} 
			else//δѡ�нڵ��deltaֵ���� ������δѡ�нڵ㸲�Ƿ�Χ��Ŀǰû�б����ǵĽڵ� 
			{
			
				for(int j=0;j<map[i].size();j++)
				{
					int u=map[i][j];	
					if(uncovered[u])  
					delta[i]=delta[i]+W[u];
				}
				//ȥ�����棬��Ϊδѡ�нڵ���������adjacent����洢�ˣ� 
			/*	if(uncovered[i])//δѡ�нڵ�����Ҳ������δ�����ǽڵ� 
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
/*����õĽ��Լ���һ�εĽ⸳��ֵ*/
	for(int i=0;i<point;i++)
	{
		best_selected[i]=selected[i];
		last_selected[i]=selected[i];
	}	 
}

void  trytoopencenter(int addi)//��ͼ��i�ڵ����center���ǲ���û���������� 
{
	
	for(int j=0;j<map[addi].size();j++)//����addi��Χ�ڵ����нڵ�
	{
		int addcount=0;
		int mark=-1;//��ס��Ҫ�޸�deltaֵ�Ľڵ�ı�� 
		int u=map[addi][j];//addi��Χ�ڽڵ��Ϊu 
		for(int i=0;i<map[u].size();i++)//��һ���ڵ㸲�Ƿ�Χ�ڵĽڵ� 
		{
			if(selected[map[u][i]]) 
			{
				addcount++;
				mark=map[u][i];
			}
		}
		if(addcount==1)
		{
//�п�������ڵ��Լ��Ƿ���ڵ㣬û����addi֮ǰ����ڵ��Լ���Χû�з���ڵ�  ������Ҫ���£���һ���������������������������������� 
					delta[mark]=delta[mark]-W[u];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		}	
	} 	
} 


int find_best_SWAP()//����Ҫѡ��Ľ����Ե��±� ,ѡ���Ҫ�����Ĳ�һ������һ�εĺ� 
{
	M.resize(0);
	int best_obj=-1;//����
	int count=0;
	int uncoveredv=-1;
	int mark=-1;
	/*��uncovered�ڵ��������ѡ��һ���ڵ�uncoveredv*/
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
	 if(uncoveredv==-1)//���е�һ���ڵ� 
	 {
	 	uncoveredv=mark;
	 }
	 /* ��Ŀǰ��delta����Ʊ��� */
	for(int i=0;i<point;i++)
	{
		delta_copy[i]=delta[i];
	}
	int u;
	for(int i=0;i<map[uncoveredv].size();i++)
	{
		u=map[uncoveredv][i];//δ���ǽڵ���Χ�ڵ㣬һ���Ƿ����Ľڵ� 
			//cout<<"u"<<u<<" ";
		//���û�б����ǽڵ���Χֻ�����Լ���������һ�θո��Ǳ�del��ҲӦ��ѡ��ýڵ�
		if(tabu[u]<iter) 
		{//û�б����� 
			trytoopencenter(u);
			//cout<<"u"<<u<<" ";
			for(int j=0;j<point;j++)
			{
				if(selected[j]&&tabu[j]<iter)//jû�б����� ������center�ڵ� 
				{
				    if((best_obj>f-delta[u]+delta[j])||best_obj==-1)//�ﵽ�ֲ����� 
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
				/*��delta��ԭ*/
			for(int i=0;i<point;i++)//trytoopencenter(i)һ�α��ͻ��޸�һ�Σ���Ҫ��ԭһ�� 
			{
				delta[i]=delta_copy[i];
			}
		}		
	}
   	int selresult=-1;
	if(M.size()>0)    selresult=rand()%M.size();//ѡ��Ľ����Ե��±�
	    //cout<<M.size() <<" select:"<<selresult<<endl; 
	return selresult;
}

void makeSWAP(int addi,int delj)//����addi�ڵ㣬ɾ��delj�ڵ�
{
	for(int j=0;j<map[addi].size();j++)//����addi��Χ�ڵ����нڵ� 
	{
		int addcount=0;
		int mark=-1;//��ס��Ҫ�޸�deltaֵ�Ľڵ�ı�� 
		int u=map[addi][j];//addi��Χ�ڽڵ��Ϊu 
		for(int i=0;i<map[u].size();i++)//��һ���ڵ㸲�Ƿ�Χ�ڵĽڵ� 
		{
			if(selected[map[u][i]]) 
			{
				addcount++;
				mark=map[u][i];
			}
		}
		if(addcount==1)
		{
//�п�������ڵ��Լ��Ƿ���ڵ㣬û����addi֮ǰ����ڵ��Լ���Χû�з���ڵ�  ������Ҫ���£���һ���������������������������������� 
			delta[mark]=delta[mark]-W[u];
		}	
/*addi��Χ�ڵ�һ���ڵ���Χû�з���ڵ㣬�Լ�Ҳ���Ƿ���ڵ� */ 
/*  �ýڵ�ĸ��Ƿ�Χ�ڳ��˸ýڵ�Ľڵ��deltaֵԭ��Ϊ�������е�δ�����ǽڵ��Ȩֵ֮��  */
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
//����
	selected[addi]=true;
	selected[delj]=false;
	for(int j=0;j<map[delj].size();j++)//����delj��Χ�ڵ����нڵ�
	{
		int addcount=0;
		int mark=-1;//��ס��Ҫ�޸�deltaֵ�Ľڵ�ı�� 
		int u=map[delj][j];//addi��Χ�ڽڵ��Ϊu 
		for(int i=0;i<map[u].size();i++)//��һ���ڵ㸲�Ƿ�Χ�ڵĽڵ� 
		{
			if(selected[map[u][i]]) 
			{
				addcount++;
				mark=map[u][i];
			}
		}
		if(addcount==1)
		{
//�п�������ڵ��Լ��Ƿ���ڵ㣬û����addi֮ǰ����ڵ��Լ���Χû�з���ڵ�  ������Ҫ���£���һ���������������������������������� 
			delta[mark]=delta[mark]+W[u];
		}	
/*addi��Χ�ڵ�һ���ڵ���Χû�з���ڵ㣬�Լ�Ҳ���Ƿ���ڵ� */ 
/*  �ýڵ�ĸ��Ƿ�Χ�ڳ��˸ýڵ�Ľڵ��deltaֵԭ��Ϊ�������е�δ�����ǽڵ��Ȩֵ֮��  */
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
    //������ûɶ���� 
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
   	    	iter++;//����ѡ��Ķ��������� 
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
    //�ֱ������һ�ε�uncovered����һ�ε�uncovered
    int last_num=0;
    int now_num=0;
    for(int i=0;i<point;i++)
    {
    	if(uncovered[i]) now_num++;
    	if(last_uncovered[i]) last_num++;
	}
	if(best_uncovered_num==-1||best_uncovered_num>now_num)//�ﵽĿǰ���Ž� 
	best_uncovered_num=now_num; 
	else if(last_num<now_num) //�Ƴ��ֲ����� 
	{
		for(int i=0;i<point;i++)
		{
			if(uncovered[i]) //δ�����ǽڵ�Ȩ��ȫ����һ��δ���ǽڵ㷶Χ�ڵ����нڵ㣬deltaֵȫ����һ 
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
	for(int i=0;i<point;i++)//����f 
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
    //������ûɶ���� 
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
   	    	iter++;//����ѡ��Ķ��������� 
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
    //�ֱ������һ�ε�uncovered����һ�ε�uncovered
    int last_num=0;
    int now_num=0;
    for(int i=0;i<point;i++)
    {
    	if(uncovered[i]) now_num++;
    	if(last_uncovered[i]) last_num++;
	}
	if(best_uncovered_num==-1||best_uncovered_num>now_num)//�ﵽĿǰ���Ž� 
	best_uncovered_num=now_num; 
	else if(last_num<now_num) //�Ƴ��ֲ����� 
	{
		for(int i=0;i<point;i++)
		{
			if(uncovered[i]) //δ�����ǽڵ�Ȩ��ȫ����һ��δ���ǽڵ㷶Χ�ڵ����нڵ㣬deltaֵȫ����һ 
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
	for(int i=0;i<point;i++)//����f 
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
   	end_t=clock();//ʱ�Ӽ��
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
