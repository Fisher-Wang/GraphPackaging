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
static void dfs(int cur)
{
    log_printf("calling dfs(%d), cur_weight_sum = %d, weight[%d] = %d\n", cur, cur_weight_sum, cur, weight[cur]);
    for (int nxt : edges[cur])
        if (!vis[nxt]) {
            vis[nxt] = 1;
            
            if (cur_weight_sum + weight[nxt] <= PKG_SIZE) { /* 当前包的剩余容量够装 */
                if (rcd_pkg[cur] == np || cur == 0) { /* 父子节点在同一包 */
                    cur_weight_sum += weight[nxt];
                    pkgs[np].push_back(nxt);
                }
                else { /* 如果父子节点不在同一包，令开一个包 */
                    cur_weight_sum = weight[nxt];
                    pkgs[++np].push_back(nxt);
                }
            }
            else { /* 当前包的剩余容量不够装，开一个新的包 */
                cur_weight_sum = weight[nxt];
                pkgs[++np].push_back(nxt);
            }
            /* 服务于备份操作 */
            tw += weight[nxt];
            if (tw <= TW/2)
                extra_np = max(extra_np, np);
            
            rcd_pkg[nxt] = np;

            dfs(nxt);
        }
}

void make_package()
{
    dfs(0);
    rst = vector<vector<int>>(pkgs+1, pkgs+np+1);
    rst.insert(rst.end(), pkgs+1, pkgs+extra_np+1);
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