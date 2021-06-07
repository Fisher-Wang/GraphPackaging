#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <list>
#include <climits>
#include <numeric>
#include <bitset>
#include "graph.h"
using namespace std;

const double INF_DOUBLE = 1e20;

double loss_prob = 0.2;

vector<int> edges[NODE_NUM_MAX];
int weight[NODE_NUM_MAX];
int TW;
int n;  // tot node number
int in_deg[NODE_NUM_MAX];
int out_deg[NODE_NUM_MAX];
int np = 1;  // tot package number
vector<int> pkgs[PKG_NUM_MAX];
vector<vector<int>> rst;

int depth[NODE_NUM_MAX];
void dfs(int cur)
{
    static bool vis[NODE_NUM_MAX];
    for (int nxt : edges[cur]) {
        if (vis[nxt])
            continue;
        vis[nxt] = 1;
        depth[nxt] = depth[cur] + 1;
        dfs(nxt);
    }
}

int succ[NODE_NUM_MAX];
void findSuccessor(int u, int verbose = 0){
    static bool vzt[NODE_NUM_MAX];
    static bitset<100000> successor[100000];
	if(vzt[u]) return;
	vzt[u] = 1;
	for(int i=0; i<edges[u].size(); i++)
	{
		int v = edges[u][i];
		successor[u][v] = 1;
		findSuccessor(v);
		successor[u] |= successor[v];
	}
    succ[u] = successor[u].count();
	
	/* debug */
	if(verbose)
		printf("Node %d has %d Successors\n", u, successor[u].count());
}


int main(int argc, char* argv[])
{
    if (LOG) {
        freopen("log.txt", "w", stdout);
    }
    char *edge_path, *weight_path, *answer_path;
    if (argc == 5) {
        edge_path = argv[1];
        weight_path = argv[2];
        answer_path = argv[3];
        loss_prob = atoi(argv[4]) / 100.;
    }
    else {
        printf("using default setting...\n");
        edge_path = (char *)"data/Out_OutGraph_Basketball_480_Slice16_Gop8_10.log";
        weight_path = (char *)"data/Out_SliceSize_Basketball_480_Slice16_Gop8_10.log";
        answer_path = (char *)"result.txt";
        loss_prob = 0.2;
    }
    read_graph(edge_path, weight_path); debug_printf("TW = %d\n", TW);
    dfs(0);
    findSuccessor(0);
    for (int u = 1; u <= n; ++u)
        log_printf("id = %5d, weight =%4d, in_deg =%2d, out_deg =%2u, succ =%5d, depth=%3d\n", u, weight[u], in_deg[u], edges[u].size(), succ[u], depth[u]);
    
    output(answer_path);
    return 0;
}