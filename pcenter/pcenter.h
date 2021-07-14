#ifndef _PCENTER_H_
#define _PCENTER_H_
#include <iostream>
#include <vector>
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include <set>
using namespace std;
struct SWAP
{
	int i;//add
	int j;//del
};
SWAP M;//寻找最好的交换对数时用来存放最好交换对数不止一个的情况，用于find_best_swap 
vector<int> tabu;
int point, centernumber;//point为图结点数，centernumber为中心数
vector<vector<int> > map;
vector<bool> selected;
vector<bool> best_selected;//记录目前为止最好的一次结果 
vector<bool> last_selected;//记录上一次的结果； 
set<int> uncovered;
set<int> last_uncovered;
set<int> best_uncovered;
vector<int> W;//所有节点的权重值 
vector<int> delta;//所有节点的delta值 
int f;//代价函数值 
int iter;//迭代次数 
clock_t start_t;
clock_t end_t;
int timelimit;
char datapath[256], resultpath[256];
void readtxt();
void create_init_solution();
void  trytoopencenter(int addi);
void find_best_SWAP();
void makeSWAP(int addi, int delj);
void search();
#endif // !_PCENTER_H_