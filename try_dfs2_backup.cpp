#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <numeric>
#include "graph.h"
using namespace std;

double loss_prob = 0.2;

vector<int> edges[NODE_NUM_MAX];
int weight[NODE_NUM_MAX];
int TW;
int n;  // tot node number
int in_deg[NODE_NUM_MAX];
double value[NODE_NUM_MAX];
int np = 1;  // tot package number
vector<int> pkgs[PKG_NUM_MAX];
int rcd_pkg[NODE_NUM_MAX];
vector<vector<int>> rst;

void evaluate_nodes()
{
}

static bool vis[NODE_NUM_MAX];
static int tw = 0;
static int cur_weight_sum = 0;
static int extra_np = 0;
static int depth[NODE_NUM_MAX];
static int depth_max = 0;
static int pkg_of_node[NODE_NUM_MAX];
static void dfs(int cur)
{
    log_printf("calling dfs(%d), cur_weight_sum = %d\n", cur, cur_weight_sum);
    for (int nxt : edges[cur])
        if (!vis[nxt]) {
            vis[nxt] = 1;

            depth[nxt] = max(depth[nxt], depth[cur] + 1);
            depth_max = max(depth_max, depth[nxt]);
            
            if (cur_weight_sum + weight[nxt] <= PKG_SIZE) {
                log_printf("case1, cur_weight_sum = %d, weight[%d] = %d\n", cur_weight_sum, nxt, weight[nxt]);
                cur_weight_sum += weight[nxt];
                pkgs[np].push_back(nxt);
            }
            else {
                cur_weight_sum = weight[nxt];
                pkgs[++np].push_back(nxt);
            }
            pkg_of_node[nxt] = np;
            tw += weight[nxt];
            if (tw <= TW/2)
                extra_np = max(extra_np, np);

            dfs(nxt);
        }
}

void backup()
{
    const int MAX_DEPTH = 150;
    ASSERT(depth_max < MAX_DEPTH);
    vector<int> nodes_of_depth[MAX_DEPTH];
    for (int x = 1; x <= n; ++x)
        nodes_of_depth[depth[x]].push_back(x);
    int tw_extra = 0;
    static bool is_backed_up[PKG_NUM_MAX];
    for (int d = 1; d <= depth_max; ++d) {
        for (int x : nodes_of_depth[d]) {
            int p = pkg_of_node[x];
            if (is_backed_up[p])
                continue;

            int weight_sum = 0;
            for (int i = 0; i < (int)pkgs[p].size(); ++i) {
                weight_sum += weight[i];
            }
            if ((tw_extra + weight_sum + tw) * 2 > TW * 3)
                goto END;

            tw_extra += weight_sum;
            is_backed_up[p] = true;
            rst.push_back(pkgs[p]);
        }
    }
    END:;
}

void make_package()
{
    dfs(0);
    rst = vector<vector<int>>(pkgs+1, pkgs+np+1);
    backup();
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
    printf("np = %d, rst.size() = %u\n", np, rst.size());
    output(answer_path);
    return 0;
}