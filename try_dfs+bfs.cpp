#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include "graph.h"
using namespace std;

double loss_prob = 0.2;

vector<int> edges[NODE_NUM_MAX];
int weight[NODE_NUM_MAX];
int TW;
int n;  // tot node number
int in_deg[NODE_NUM_MAX];
double value[NODE_NUM_MAX];
vector<int> seq;
vector<int> seq2;
int np;  // tot package number
vector<int> pkgs[PKG_NUM_MAX];
vector<vector<int>> rst;

void evaluate_nodes()
{
}

static bool vis[NODE_NUM_MAX];
static void bfs(int cur)
{
    log_printf("calling bfs(%d)\n", cur);
    memset(vis, false, sizeof(vis));
    queue<int> nodes;
    nodes.push(cur);
    int now;
    while(!nodes.empty()){
        now = nodes.front();nodes.pop();
        if(vis[now])continue;
        vis[now] = true;
        for(int nxt:edges[now]){
            nodes.push(nxt);
        }
        seq2.push_back(now);
    }
}
static void dfs(int cur){
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
    /* Stage 1: BFS */
    memset(vis, 0, sizeof(vis));
    dfs(0);
    bfs(0);
    debug_printf("stage 1 complete\n");

    /* Stage 2: grouping */
    int last_i = 0;
    int acc_weight = 0;
    seq.insert(seq.end(), seq2.begin(), seq2.end());
    int tw = 0;
    int i = 0;
    for (i = 0; i < (int)seq.size(); ++i)
    {
        int x = seq[i];
        if (weight[x] > PKG_SIZE) {
            fprintf(stderr, "[Warning] node %d has too large weight %d\n", x, weight[x]);
            continue;
        }
        if ((tw + weight[x]) * 2 > TW * 3)
            break;
        else {
            tw += weight[x];
            if (acc_weight + weight[x] > PKG_SIZE || x == 0) {
                pkgs[++np] = vector<int>(seq.begin() + last_i, seq.begin() + i);
                last_i = (x==0)?(i+1):i;
                acc_weight = weight[x];
            }
            else 
                acc_weight += weight[x];
        }
    }
    pkgs[++np] = vector<int>(seq.begin() + last_i, seq.begin() + i);
    rst = vector<vector<int>>(pkgs+1, pkgs+np+1);
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
        edge_path = (char *)"data/Out_OutGraph_Basketball_480_Slice16_Gop8_10.log";
        weight_path = (char *)"data/Out_SliceSize_Basketball_480_Slice16_Gop8_10.log";
        answer_path = (char *)"result.txt";
        loss_prob = 0.2;
    }

    read_graph(edge_path, weight_path); debug_printf("TW = %d\n", TW);
    evaluate_nodes();
    make_package();
    output(answer_path);
    return 0;
}