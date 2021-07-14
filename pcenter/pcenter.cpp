
#include"pcenter.h"
void readtxt()
{
	scanf_s("%d%d", &point, &centernumber);
	map.resize(point);
	for (int j = 0; j < point; j++)
	{
		int c = 0;
		scanf_s("%d", &c);
		int nextnode;
		for (int k = 0; k < c; k++)
		{
			scanf_s("%d", &nextnode);
			map[j].push_back(nextnode);
		}
	}
}
void create_init_solution()
{
	selected.clear();
	selected.resize(point);
	for(auto l : selected) l = false;
	for (int i = 0; i < point; i++) uncovered.insert(i);
	for (int i = 0; i < centernumber; i++)
	{
		int lastselectcount = -1;
		int lastnumber = -1;
		for (int j = 0; j < point; j++)
		{
			if (!selected[j])//统计j附近未被覆盖的节点数 
			{
				int bbb = 0;//统计j附近未被覆盖的节点数 
				int ccc = 1;
				for (auto l:map[j])
				{
					if (uncovered.find(l)!=uncovered.end()) bbb++;
				}
				if (bbb > lastselectcount)
				{
					ccc = 1;
					lastselectcount = bbb;
					lastnumber = j;
				}
				else if (bbb == lastselectcount)//等概率随机选择一个； 
				{
					ccc++;
					if (rand() % ccc == 0)
					{
						lastnumber = j;
					}
				}
			}
		}
		selected[lastnumber] = true;
		for (auto l:map[lastnumber])
		{
			auto iter = uncovered.find(l);
			if (iter != uncovered.end())
				uncovered.erase(iter);
		}
	}
	last_selected.clear();
	best_selected.clear();
	last_selected.assign(selected.begin(), selected.end());
	best_selected.assign(selected.begin(),selected.end());
	best_uncovered.clear();
	for (auto l : uncovered)
	{
		last_uncovered.insert(l);
		best_uncovered.insert(l);
	}
	tabu.clear();
	W.clear();
	delta.clear();
	tabu.resize(point);
	W.resize(point);
	delta.resize(point);
	for (int i = 0; i < point; i++)
	{
		W[i] = 1;//初始时每个节点的w值为1 
		delta[i] = 0;
		tabu[i] = 0;//初始时禁忌初始化
	}
	/*计算初始时每个节点的delta值*/
	for (int i = 0; i < point; i++)
	{
		if (selected[i])//对于已经选中的节点的delta值，统计只能被该节点覆盖的节点的w之和
		{
			for (auto l:map[i])//判断map[i][j]这个节点是不是只能被i节点覆盖 
			{
				int temp=0;
				for (auto x : map[l])
				{
					if (selected[x]) temp++;
				}
				if(temp==1) delta[i] = delta[i] + W[l];
			}
		}
		else//未选中节点的delta值计算 ，计算未选中节点覆盖范围内目前没有被覆盖的节点 
		{
			for (auto l : map[i])
			{
				if(uncovered.find(l)!=uncovered.end())
					delta[i] = delta[i] + W[l];
			}
		}
	}
	f = 0;
	for (auto l:uncovered)
	{
		f = f + W[l];
	}
}

void  trytoopencenter(int addi)//试图将i节点加入center但是并没有真正加入 
{
	for (auto l : map[addi])
	{
		int temp = 0;
		int mark = 0;
		for (auto x : map[l])
		{
			if (selected[x])
			{
				temp++;
				mark = x;
			}
		}
		if(temp==1) delta[mark] = delta[mark] - W[l];
	}
}
void find_best_SWAP()//返回要选择的交换对的下标 ,选择的要交换的不一定有上一次的好 
{
	int best_obj = -1;//无穷
	int uncoveredv;
	/*从uncovered节点里面随机选择一个节点uncoveredv*/
	int random = rand() % uncovered.size();
	set<int>::const_iterator it(uncovered.begin());
	advance(it, random);
	uncoveredv = *it;
	/* 将目前的delta表格复制备份 */
	vector<int> delta_copy;
	delta_copy.clear();
	delta_copy.resize(point);
	for (int i = 0; i < point; i++)
	{
		delta_copy[i] = delta[i];
	}
	int seq;
	bool marking=false;
	for (auto l : map[uncoveredv])
	{
		//如果没有被覆盖节点周围只有他自己，并且上一次刚刚是被del了也应该选择该节点
		if (tabu[l] < iter)
		{//没有被禁忌 
			trytoopencenter(l);
			for (int j = 0; j < point; j++)
			{
				if (selected[j] && tabu[j] < iter)//j没有被禁忌 ，且是center节点 
				{
					marking = true;

					if ((best_obj > f - delta[l] + delta[j]) || best_obj == -1)//达到局部最优 
					{
						seq = 1;
						best_obj = f - delta[l] + delta[j];
						M.i = l;
						M.j = j;
					}
					else if (best_obj == f - delta[l] + delta[j])
					{
						seq++;
						if(rand()%seq==0)
						{
							M.i = l;
							M.j = j;
						}
					}
				}
			}
			/*将delta还原*/
			for (int i = 0; i < point; i++)//trytoopencenter(i)一次表格就会修改一次，就要复原一次 
			{
				delta[i] = delta_copy[i];
			}
		}
	}
	if (!marking) {
		M.i = -1;
		M.j = -1;
	}
}
void makeSWAP(int addi, int delj)//加入addi节点，删除delj节点
{
	for (auto v: map[addi])
	{
		int temp = 0;
		int mark;
		for (auto l : map[v])
		{
			if (selected[l])
			{
				temp++;
				mark = l;
			}
		}
		if (temp == 0)
		{
			for (auto x: map[v])
			{
				if (x != addi) delta[x] = delta[x] - W[v];
			}

		}
		if (temp == 1) 
		{
			delta[mark] = delta[mark] - W[v];
		}
	}
	selected[addi] = true;
	selected[delj] = false;
	for (auto v : map[delj])
	{
		int temp = 0;
		int mark;
		for (auto l : map[v])
		{
			if (selected[l])
			{
				temp++;
				mark = l;
			}
		}
		if (temp == 0)
		{
			for (auto x : map[v])
			{
				if (x != delj) delta[x] = delta[x] + W[v];
			}

		}
		if (temp == 1)
		{
			delta[mark] = delta[mark] +W[v];
		}
	}
}
void check()
{
	bool mark = false;
	int uncoverednum = 0;
	for (int i = 0; i < point; i++)
	{
		for (auto l : map[i])
		{
			if (selected[l])
			{
				mark = true; break;
			}
		}
		if (!mark) uncoverednum++;
	}
	printf("uncovered number:%d\n", uncoverednum);
	for (int i = 0; i < point; i++)
		if (selected[i]) printf("%d ", i);
	printf("\n");
}

void search()
{
	iter = 1;
	while (f != 0)
	{
		last_selected.clear();
		last_selected.assign(selected.begin(), selected.end());
		last_uncovered.clear();
		for (auto l : uncovered)
		{
			last_uncovered.insert(l);
		}
		find_best_SWAP();
		if (M.i == -1 && M.j == -1)
		{
			iter++;
			continue;
		}
		makeSWAP(M.i, M.j);
		for (auto l : map[M.i])
		{
			auto iter = uncovered.find(l);
			if (iter != uncovered.end()) uncovered.erase(iter);
		}
		for (auto l : map[M.j])
		{
			int count = 0;
			for (auto x : map[l])
			{
				if (selected[x]) count++;
			}
			if (count == 0) uncovered.insert(l);
		}
		if (best_uncovered.size() > uncovered.size())//达到目前最优解 
		{
			best_uncovered.clear();
			for (auto l : uncovered)
			{
				best_uncovered.insert(l);
			}
			best_selected.clear();
			best_selected.assign(selected.begin(), selected.end());
		}
		else if (last_uncovered.size()<=uncovered.size()) 
		{
			for (auto l : uncovered)
			{
				W[l]++;
				for (auto x : map[l])
				{
					delta[x]++;
				}
			}
		}
		f = 0;
		for (auto l : uncovered)
			f = f + W[l];
		tabu[M.i] = iter + 1;
		tabu[M.j] = iter + 1;
		end_t = clock();//时钟检测
		if (end_t > start_t + timelimit * CLOCKS_PER_SEC)
		{

			break;
		}
		iter++;
	}
}
/*
int main(int argc, char* argv[])
{

	clock_t start_t = clock();
#pragma warning(disable:4996)
	strcpy(datapath, argv[1]);
	strcpy(resultpath, argv[2]);
	timelimit = atoi(argv[3]);
	unsigned seed = atoi(argv[4]);
	srand(seed);
	clock_t end_t;
	FILE *stream1;
	FILE *stream2;
	freopen_s(&stream1, datapath, "r", stdin);
	freopen_s(&stream2, resultpath, "w", stdout);
	readtxt();
	create_init_solution();
	search();
	check();
	fclose(stdin);//关闭文件 
	fclose(stdout);//关闭文件
}
*/

int main()
{
	FILE *stream1;
	freopen_s(&stream1, "./data/u1817p10r457.91.txt", "r", stdin);
	//freopen_s(&stream1, "out.txt", "w", stdout);
	int tm = 12345;
	timelimit = 1000;
	srand(tm);
	readtxt();
	create_init_solution();
	search();
	check();
	fclose(stdin);//关闭文件 
	return 0;
}
