#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include<bitset>
#include "graph.h"
using namespace std;

const double loss_prob = 0.2;

vector<int> edges[NODE_NUM_MAX];
int weight[NODE_NUM_MAX];
int TW;
int n;  // tot node number
int in_deg[NODE_NUM_MAX];
vector<int> seq;
int np;  // tot package number
vector<int> pkgs[PKG_NUM_MAX];
vector<vector<int>> rst;
bool vzt[NODE_NUM_MAX];
bitset<100000> successor[100000];

struct PACK{
	int sizSucc;
	int index;
	bool operator < (const PACK &rhs) const {
		return sizSucc > rhs.sizSucc;
	}
}Pinfo[PKG_NUM_MAX];

void findSuccessor(int u, int verbose = 0){
	if(vzt[u]) return;
	vzt[u] = 1;
	for(int i=0; i<edges[u].size(); i++)
	{
		int v = edges[u][i];
		successor[u][v] = 1;
		findSuccessor(v);
		successor[u] |= successor[v];
	}
	
	/* debug */
	if(verbose)
		printf("Node %d has %d Successors\n", u, successor[u].count());
}

void find_all_successors()
{
    for(int i=1;i<=n;++i)
        findSuccessor(i,0);
}



void evaluate_nodes()
{
}




static bool vis[NODE_NUM_MAX];
static void dfs(int cur)
{
    log_printf("calling dfs(%d)\n", cur);
    for (int nxt : edges[cur])
        if (!vis[nxt]) {
            vis[nxt] = 1;
            seq.push_back(nxt);
            dfs(nxt);
        }
}

void make_package()
{
    /* Stage 1: DFS */
    memset(vis, 0, sizeof(vis));
    dfs(0);
    debug_printf("stage 1 complete\n");

    /* Stage 2: grouping */
    int last_i = 0;
    int acc_weight = 0;
    auto bak_seq = vector<int>(seq.begin(), seq.end());
    seq.insert(seq.end(), bak_seq.begin(), bak_seq.end());
    int tw = 0;
    int i = 0;
    int flag = 0;
    for (i = 0; i < (int)seq.size(); ++i)
    {
        printf("i = %d\n", i);
        int x = seq[i];
        if (weight[x] > PKG_SIZE) {
            fprintf(stderr, "[Warning] node %d has too large weight %d\n", x, weight[x]);
            continue;
        }
        if ((tw + weight[x]) > TW )// all nodes been packed 
            break;
        else {
            tw += weight[x];
            if (acc_weight + weight[x] > PKG_SIZE) {
                pkgs[++np] = vector<int>(seq.begin() + last_i, seq.begin() + i);
                last_i = i;
                acc_weight = weight[x];
            }
            else 
                acc_weight += weight[x];
        }
    }
    pkgs[++np] = vector<int>(seq.begin() + last_i, seq.begin() + i);
    puts("copying best packages...");
	int cntPackage = 0;
	for(int i = 1; i <= np; i++) if(pkgs[i].size())
	{
		static bitset<100000> b;
		b = 0;
		for(int j = 0; j < pkgs[i].size(); j++) b |= successor[pkgs[i][j]];
		Pinfo[++cntPackage] = (PACK){ (int)b.count(), i };
	}
	sort(Pinfo+1, Pinfo+cntPackage+1);
	
	int extra = 1;
    int next_weight = 0;
    int psize = pkgs[extra].size();
    for(int i=0;i<psize;++i)
        next_weight += weight[pkgs[Pinfo[extra].index][i]];
    while((tw + next_weight)*2 <= TW *3)
	{
		tw += next_weight;
		extra++;
        next_weight = 0;
        psize = pkgs[extra].size();
        for(int i=0;i<psize;++i)
            next_weight += weight[pkgs[Pinfo[extra].index][i]];
	}
    for(int i=1;i<extra;++i)
        pkgs[++np] = vector<int>(pkgs[Pinfo[i].index].begin(),pkgs[Pinfo[i].index].end());
    rst = vector<vector<int>>(pkgs+1, pkgs+np+1);
}

int main(int argc, char* argv[])
{
    if (LOG) {
        freopen("log.txt", "w", stdout);
    }
    char *edge_path, *weight_path, *answer_path;
    if (argc == 4) {
        edge_path = argv[1];
        weight_path = argv[2];
        answer_path = argv[3];
    }
    else {
        edge_path = (char *)"data/Out_OutGraph_Basketball_480_Slice16_Gop8_10.log";
        weight_path = (char *)"data/Out_SliceSize_Basketball_480_Slice16_Gop8_10.log";
        answer_path = (char *)"result.txt";
    }

    read_graph(edge_path, weight_path); debug_printf("TW = %d\n", TW);
    find_all_successors();
    evaluate_nodes();
    make_package();
    output(answer_path);
    return 0;
}