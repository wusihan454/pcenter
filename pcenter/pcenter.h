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
SWAP M;//Ѱ����õĽ�������ʱ���������ý���������ֹһ�������������find_best_swap 
int point, centernumber;//pointΪͼ�������centernumberΪ������
int** map;
bool* fix;//��ʼֵΪfalse,һ������һ������û�������ڵ�Ĺ����ڵ㣬��ô�����̶�������
int fixcount;
bool* selected;
bool* best_selected;//��¼ĿǰΪֹ��õ�һ�ν�� 
bool* last_selected;//��¼��һ�εĽ���� 
bool* uncovered;
bool* last_uncovered;
bool* best_uncovered;
int* tabu;
int* W;//���нڵ��Ȩ��ֵ 
int* delta;//���нڵ��deltaֵ 
int* delta_copy;
int* covered_by;//��¼ÿһ���ڵ㱻˭������
int* neighbor_number;
int* center;//ÿһ���ڵ���Χ���Ľڵ������
int* all_selected;
int best_uncovered_num;
int last_uncovered_num;
int uncovered_num;
int f;//���ۺ���ֵ 
int iter;//�������� 
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
//�ڴ���Ҫ�ͷ�