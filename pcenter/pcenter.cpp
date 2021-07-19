
#include"pcenter.h"
void readtxt()
{
	scanf_s("%d%d", &point, &centernumber);
	map = new int* [point];
	neighbor_number = new int[point];
	for (int j = 0; j < point; j++)
	{
		int c = 0;
		scanf_s("%d", &c);
		map[j] = new int[c];
		neighbor_number[j] = c;
		for (int k = 0; k < c; k++)
		{
			scanf_s("%d", &map[j][k]);
		}
	}
}
void create_init_solution()
{
	selected = new bool[point];
	center = new int[point];
	best_selected = new bool[point];
	last_selected = new bool[point];
	all_selected=new int[centernumber];
	fix = new bool[point];
	fixcount = 0;
	uncovered = new bool[point];
	last_uncovered = new bool[point];
	best_uncovered = new bool[point];
	tabu = new int[point];
	W = new int[point];
	delta = new int[point];
	covered_by = new int[point];
	delta_copy = new int[point];
	for (int i = 0; i < point; i++)
	{
		selected[i] = false;
		uncovered[i] = true;
		fix[i] = false;
		center[i] = 0;
		covered_by[i] = -1;
	}
	//判断需要提前固定下来的节点
	for (int i = 0; i < point; i++)
	{
		if (neighbor_number[i] == 1)
		{
			all_selected[fixcount] = i;
			fixcount++;
			fix[i] = true;
			selected[i] = true;
			uncovered[i] = false;
			center[i] = 1;
		}
	}
	for (int i = 0; i < centernumber - fixcount; i++)
	
	{
		int lastselectcount = -1;
		int lastnumber = -1;
		int ccc = 1;
		for (int j = 0; j < point; j++)
		{

			if (!selected[j])//统计j附近未被覆盖的节点数 
			{
				int bbb = 0;//统计j附近未被覆盖的节点数 
				for (int k = 0; k < neighbor_number[j]; k++)
				{
					if (uncovered[map[j][k]]) bbb++;
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
		all_selected[i + fixcount] = lastnumber;
		for (int k = 0; k < neighbor_number[lastnumber]; k++)
		{
			uncovered[map[lastnumber][k]] = false;
			center[map[lastnumber][k]]++;
		}
	}
	for (int i = 0; i < point; i++)
	{
		if (center[i] == 1)
		{
			for (int j = 0; j < neighbor_number[i]; j++)
			{
				if (selected[map[i][j]])
				{
					covered_by[i] = map[i][j];
					break;
				}
			}
		}
	}
	memcpy(last_selected, selected, point * sizeof(bool));
	memcpy(best_selected, selected, point * sizeof(bool));
	memcpy(best_uncovered, uncovered, point * sizeof(bool));
	memcpy(last_uncovered, uncovered, point * sizeof(bool));
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
			for (int j = 0; j < neighbor_number[i]; j++)
			{
				int neighbor = map[i][j];

				if (center[neighbor] == 1) delta[i] = delta[i] + W[neighbor];
			}
		}
		else//未选中节点的delta值计算 ，计算未选中节点覆盖范围内目前没有被覆盖的节点 
		{
			for (int j = 0; j < neighbor_number[i]; j++)
			{
				if (uncovered[map[i][j]])
					delta[i] = delta[i] + W[map[i][j]];
			}
		}
	}
	f = 0;
	uncovered_num = 0;
	all_uncovered.clear();
	for (int i = 0; i < point; i++)
	{
		if (uncovered[i])
		{
			f = f + W[i];
			uncovered_num++;
			all_uncovered.push_back(i);
		}
	}
	best_uncovered_num = uncovered_num;
	last_uncovered_num = uncovered_num;
}
void realse_mem()
{
	for (int i = 0; i < point; i++)
		delete[] map[i];
	delete[] map;
	delete[] selected;
	delete[] best_selected;
	delete[] last_selected;
	delete[] fix;
	delete[] uncovered;
	delete[] last_uncovered;
	delete[] best_uncovered;
	delete[] tabu;
	delete[] W;
	delete[] delta;
	delete[] delta_copy;
	delete[] neighbor_number;
}
void  trytoopencenter(int addi)//试图将i节点加入center但是并没有真正加入 
{
	for (int i = 0; i < neighbor_number[addi]; i++)
	{
		int l = map[addi][i];
		if (center[l] == 1)
		{
			int mark = covered_by[l];
			delta[mark] = delta[mark] - W[l];
		}
	}
}
void find_best_SWAP()//返回要选择的交换对的下标 ,选择的要交换的不一定有上一次的好 
{
	int best_obj = -1;//无穷
	int uncoveredv;
	/*从uncovered节点里面随机选择一个节点uncoveredv*/
	int choose = rand() % all_uncovered.size();
	uncoveredv = all_uncovered[choose];
	/* 将目前的delta表格复制备份 */
	memcpy(delta_copy, delta, point * sizeof(int));
	int seq;
	bool marking = false;
	for (int i = 0; i < neighbor_number[uncoveredv]; i++)
	{
		int l = map[uncoveredv][i];
		if (tabu[l] < iter)
		{
			trytoopencenter(l);
			for (int s = 0; s < centernumber; s++)
			{
				int j = all_selected[s];
				if (!fix[j] && tabu[j] < iter)//j没有被禁忌 ，且是center节点 ,并且没有被固定
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
						if (rand() % seq == 0)
						{
							M.i = l;
							M.j = j;
						}
					}
				}
			}
			memcpy(delta, delta_copy, point * sizeof(int));
		}
	}
	if (!marking) {
		M.i = -1;
		M.j = -1;
	}
}
void makeSWAP(int addi, int delj)//加入addi节点，删除delj节点,更新交换之后新的节点的delta值。
{
	for(int i = 0; i < neighbor_number[addi]; i++)
	{
		int v = map[addi][i];
		int temp = 0;
		int mark;
		if (center[v] == 0)//以前是未覆盖现在变为覆盖
		{
			for (int j = 0; j < neighbor_number[v]; j++)
			{
				int x = map[v][j];
				if (x != addi) delta[x] = delta[x] - W[v];
			}
			uncovered[v] = false;
			for (int k = 0; k < all_uncovered.size(); k++)
			{
				if (all_uncovered[k] == v)
				{

					all_uncovered.erase(all_uncovered.begin() + k);
					break;
				}
			}
			covered_by[v] = addi;
		}
		else if (center[v] == 1)
		{
			for (int j = 0; j < neighbor_number[v]; j++)
			{
				int mark= map[v][j];
				if (selected[mark])
				{
					delta[mark] = delta[mark] - W[v];
				}
			}	
		}
	}
	selected[addi] = true;
	selected[delj] = false;
	for (int i = 0; i < centernumber; i++)
	{
		if (all_selected[i] == delj)
		{
			all_selected[i] = addi;
			break;
		}
	}
	for (int i = 0; i < neighbor_number[addi]; i++)
	{
		int mark = map[addi][i];
		center[mark]++;
	}
	for (int i = 0; i < neighbor_number[delj]; i++)
	{
		int mark = map[delj][i];
		center[mark]--;
		if (center[mark] < 0)
			printf("异常\n");
	}
	for (int i = 0; i < neighbor_number[delj]; i++)
	{
		int v = map[delj][i];
		int temp = 0;
		int mark;
		if(center[v]==0)
		{
			for (int j = 0; j < neighbor_number[v]; j++)
			{
				int x = map[v][j];
				if (x != delj) delta[x] = delta[x] + W[v];
			}
			uncovered[v] = true;
			all_uncovered.push_back(v);
		}
		else if (center[v] == 1)
		{
			for (int j = 0; j < neighbor_number[v]; j++)
			{
				int mark = map[v][j];
				if (selected[mark])
				{
					delta[mark] = delta[mark] + W[v];
					covered_by[v] = mark;
					break;
				}
			}
		}
	}
}
void check()
{
	bool mark;
	int num = 0;
	for (int i = 0; i < point; i++)
	{
		mark = false;
		for (int j = 0; j < neighbor_number[i]; j++)
		{
			int l = map[i][j];
			if (best_selected[l])
			{
				mark = true; break;
			}
		}
		if (!mark)num++;
	}
	printf("uncovered number:%d\n", num);
	for (int i = 0; i < point; i++)
		if (best_selected[i]) printf("%d ", i);
	printf("\n");
}

void search()
{
	iter = 1;
	while (f != 0)
	{
		memcpy(last_selected, selected, point * sizeof(bool));
		memcpy(last_uncovered, uncovered, point * sizeof(bool));
		last_uncovered_num = uncovered_num;
		find_best_SWAP();
		if (M.i == -1 && M.j == -1)
		{
			iter++;
			printf("no swap temporary\n");
			continue;
		}
		makeSWAP(M.i, M.j);
		uncovered_num = 0;
		for (int i = 0; i < point; i++)
		{
			if (uncovered[i])
				uncovered_num++;
		}
		if (best_uncovered_num > uncovered_num)//达到目前最优解 
		{
			memcpy(best_uncovered, uncovered, point * sizeof(bool));
			memcpy(best_selected, selected, point * sizeof(bool));
			best_uncovered_num = uncovered_num;
			printf("best_uncovered %d\n", best_uncovered_num);
		}
		else if (last_uncovered_num <= uncovered_num)
		{
			for (int i = 0; i < point; i++)
			{
				if (uncovered[i])
				{
					W[i]++;
					for (int j = 0; j < neighbor_number[i]; j++)
					{
						delta[map[i][j]]++;
					}
				}
			}
		}
		f = 0;
		for (int i = 0; i < point; i++)
		{
			if (uncovered[i])
				f = f + W[i];
		}
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
	end_t = clock();//时钟检测                                                       
	float usedtime = (float(end_t - start_t)) / CLOCKS_PER_SEC;
	printf("\n using time: %f\n", usedtime);
	realse_mem();
	fclose(stdin);//关闭文件
	fclose(stdout);//关闭文件
}
*/

int main()
{
	FILE* stream1;
	freopen_s(&stream1, "./data/pcb3038p50r298.04.txt", "r", stdin);
	//freopen_s(&stream1, "out.txt", "w", stdout);
	int tm = 1000;
	timelimit = 1000;
	srand(tm);
	readtxt();
	create_init_solution();
	search();
	check();
	end_t = clock();//时钟检测
	float usedtime = (float(end_t - start_t)) / CLOCKS_PER_SEC;
	printf("\n using time: %f,iter %d\n", usedtime,iter);
	fclose(stdin);//关闭文件 
	for (int i = 0; i < point; i++)
		delete[] map[i];
	delete[] map;
	delete[] selected;
	delete[] best_selected;
	delete[] last_selected;
	delete[] fix;
	delete[] uncovered;
	delete[] last_uncovered;
	delete[] best_uncovered;
	delete[] tabu;
	delete[] W;
	delete[] delta;
	delete[] delta_copy;
	delete[] neighbor_number;
	delete[] all_selected;
	delete[] covered_by;
	return 0;
}
