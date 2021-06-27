#include <ctime>
#include <vector>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
#define maxn 200005
#define SEED 0
// char graphFile[]="Out_OutGraph_Gop32_3.log";
// char sizeFile[]="Out_SliceSize_Gop32_3.log";
// char answerFile[]="random3.txt" ;
//char answerFile[]="output.txt";
vector<int> e[maxn];
vector<int> re[maxn];
int w[maxn];
vector<int> bag[maxn];
bool ok[maxn];
vector<int> baggy[maxn]; //bag
vector<int> bagweight;
vector<int> bagsize;
bool already[maxn];
bool bagReach[maxn];
int bagnum = 1;
unsigned long long IWeight = 0, totalnodeWeight = 0, totalbagWeight = 0;
int main(int argc, char *argv[])
{
	char *edge_path, *weight_path, *answer_path;
	if (argc == 5)
	{
		edge_path = argv[1];
		weight_path = argv[2];
		answer_path = argv[3];
	}
	else
	{
		edge_path = (char *)"data/Out_OutGraph_Basketball_480_Slice16_Gop8_10.log";
		weight_path = (char *)"data/Out_SliceSize_Basketball_480_Slice16_Gop8_10.log";
		answer_path = (char *)"result.txt";
	}
	FILE *f1 = fopen(edge_path, "r");
	FILE *f2 = fopen(weight_path, "r");
	int u, v;
	int m = 0, n = 0;
	memset(w, -1, sizeof(w));
	while (~fscanf(f1, "%d%d", &u, &v) && u && v)
	{
		m++;
		assert(u < v);
		e[u].push_back(v);
		re[v].push_back(u);
		//	n=max(n,u);
		//	n=max(n,v);
		assert(n < maxn);
	}
	fclose(f1);
	int ind, weight;
	while (~fscanf(f2, "%d %d", &ind, &weight) && ind && weight)
	{
		w[ind] = weight;
		already[ind] = false;
		totalnodeWeight += weight;
		n = max(n, ind);
	}
	fclose(f2);
	for (int i = 1; i <= n; i++)
		assert(w[i] >= 0);
	//随机装包
	bagweight.push_back(0);
	//srand(time(0));
	srand(SEED);
	for (int i = 0; i < 1000000; i++)
	{
		int tt = rand() % (n + 1);
		if (tt == 0 || already[tt])
			continue;
		bool flag = false;
		for (int j = 0; j < bagnum; j++)
		{
			if (bagweight[j] + w[tt] <= 1572)
			{
				flag = true;
				bagweight[j] += w[tt];
				baggy[j].push_back(tt);
				break;
			}
		}
		//如果全都装不下，新建一个包
		if (flag == false)
		{
			bagweight.push_back(w[tt]);
			baggy[bagnum].push_back(tt);
			bagnum++;
		}
		totalbagWeight += w[tt];
		already[tt] = true;
	}
	cout << "all is well" << endl;
	//检查保证所有节点装包
	for (int i = 1; i <= n; i++)
	{
		if (already[i] == false)
		{
			bool flag = false;
			for (int j = 0; j < bagnum; j++)
			{
				if (bagweight[j] + w[i] <= 1572)
				{
					flag = true;
					bagweight[j] += w[i];
					baggy[j].push_back(i);
					break;
				}
			}
			if (flag == false)
			{
				bagweight.push_back(w[i]);
				baggy[bagnum].push_back(i);
				bagnum++;
			}
			already[i] = true;
			totalbagWeight += w[i];
		}
	}
	if (totalbagWeight != totalnodeWeight)
	{
		printf("error\n");
	}
	//将所有I层节点装5次，在保证不超标的情况下
	int bagfull = 0;
	for (int k = 0; k < 5; k++)
	{
		for (int i = 1; i <= 900; i++)
		{
			bool flag = false;
			for (int j = 0; j < bagnum; j++)
			{
				if (bagweight[j] + w[i] <= 1572 && totalbagWeight + w[i] <= totalnodeWeight + totalnodeWeight / 2)
				{
					flag = true;
					bagweight[j] += w[i];
					baggy[j].push_back(i);
					totalbagWeight += w[i];
					break;
				}
			}
			if (flag == false && totalbagWeight + w[i] <= totalnodeWeight + totalnodeWeight / 2)
			{
				bagweight.push_back(w[i]);
				baggy[bagnum].push_back(i);
				bagnum++;
				totalbagWeight += w[i];
			}
			else if (totalbagWeight + w[i] > totalnodeWeight + totalnodeWeight / 2)
				bagfull = 1;
		}
		if (bagfull)
			break;
	}
	//接下来随机装到1.5倍
	while (totalbagWeight <= totalnodeWeight + totalnodeWeight / 2)
	{
		int rr = rand() % (n + 1);
		if (rr == 0)
			continue;
		bool flag = false;
		for (int j = 0; j < bagnum; j++)
		{
			if (bagweight[j] + w[rr] <= 1572 && totalbagWeight + w[rr] <= totalnodeWeight + totalnodeWeight / 2)
			{
				flag = true;
				bagweight[j] += w[rr];
				baggy[j].push_back(rr);
				totalbagWeight += w[rr];
				break;
			}
		}
		if (flag == false && totalbagWeight + w[rr] <= totalnodeWeight + totalnodeWeight / 2)
		{
			bagweight.push_back(w[rr]);
			baggy[bagnum].push_back(rr);
			bagnum++;
			totalbagWeight += w[rr];
		}
		else if (totalbagWeight + w[rr] > totalnodeWeight + totalnodeWeight / 2)
		{
			bagfull = 1;
		}
		if (bagfull)
			break;
	}

	//写入文件
	int order[maxn];
	for (int i = 0; i < bagnum; i++)
		order[i] = i;
	//random_shuffle(order,order+bagnum);
	FILE *ans = fopen(answer_path, "w");
	fprintf(ans, "%d\n", bagnum);
	for (int t = 0; t < bagnum; t++)
	{
		int i = order[t];
		fprintf(ans, "%d", baggy[i].size());
		for (int j = 0; j < baggy[i].size(); j++)
		{
			fprintf(ans, " %d", baggy[i][j]);
		}
		fprintf(ans, "\n");
	}
	fclose(ans);
}
