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
SWAP M;//Ѱ����õĽ�������ʱ���������ý���������ֹһ�������������find_best_swap 
vector<int> tabu;
int point, centernumber;//pointΪͼ�������centernumberΪ������
vector<vector<int> > map;
vector<bool> selected;
vector<bool> best_selected;//��¼ĿǰΪֹ��õ�һ�ν�� 
vector<bool> last_selected;//��¼��һ�εĽ���� 
set<int> uncovered;
set<int> last_uncovered;
set<int> best_uncovered;
vector<int> W;//���нڵ��Ȩ��ֵ 
vector<int> delta;//���нڵ��deltaֵ 
int f;//���ۺ���ֵ 
int iter;//�������� 
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