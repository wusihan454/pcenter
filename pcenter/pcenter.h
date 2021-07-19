#ifndef _PCENTER_H_
#define _PCENTER_H_
#include <iostream>
#include <vector>
#include <cstring>
#include <time.h>
#include <stdlib.h>
using namespace std;
struct SWAP
{
	int i;//add
	int j;//del
};
SWAP M;//寻找最好的交换对数时用来存放最好交换对数不止一个的情况，用于find_best_swap 
int point, centernumber;//point为图结点数，centernumber为中心数
int** map;
bool* fix;//初始值为false,一旦发现一个附近没有其他节点的孤立节点，那么将他固定下来。
int fixcount;
bool* selected;
bool* best_selected;//记录目前为止最好的一次结果 
bool* last_selected;//记录上一次的结果； 
bool* uncovered;
bool* last_uncovered;
bool* best_uncovered;
int* tabu;
int* W;//所有节点的权重值 
int* delta;//所有节点的delta值 
int* delta_copy;
int* covered_by;//记录每一个节点被谁覆盖着
int* neighbor_number;
int* center;//每一个节点周围中心节点的数量
int* all_selected;
int best_uncovered_num;
int last_uncovered_num;
int uncovered_num;
int f;//代价函数值 
int iter;//迭代次数 
clock_t start_t;
clock_t end_t;
int timelimit;
vector<int> all_uncovered;
char datapath[256], resultpath[256];
void readtxt();
void create_init_solution();
void  trytoopencenter(int addi);
void find_best_SWAP();
void makeSWAP(int addi, int delj);
void search();
void realse_mem();
#endif // !_PCENTER_H_
//内存需要释放