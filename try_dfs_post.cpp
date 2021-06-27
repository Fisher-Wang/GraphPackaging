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
int np = 0;  // tot package number
vector<int> pkgs[PKG_NUM_MAX];
vector<vector<int>> rst;

void evaluate_nodes()
{
    static int acc_w[NODE_NUM_MAX];
    static int layer[NODE_NUM_MAX];
    memset(acc_w, 255, sizeof(acc_w));
    struct Node {
        int index;
    };
    vector<Node> *q = new vector<Node>;
    vector<Node> *q2 = new vector<Node>;

    acc_w[0] = 0;
    q->push_back({0});
    for (int h = 0; !q->empty(); ++h) {
        for (Node& node : *q) {
            int cur = node.index;
            for (int nxt : edges[cur]) {
                if (weight[nxt] == -1) {
                    acc_w[nxt] = weight[nxt] + acc_w[cur];
                    q->push_back({nxt});
                }
                else if (weight[nxt] + acc_w[cur] < acc_w[nxt]) {
                    acc_w[nxt] = weight[nxt] + acc_w[cur];
                }
            }
        }
        q->clear();
        swap(q, q2);
    }

    for (int x = 1; x <= n; ++x) {
        layer[x] = (acc_w[x] + PKG_SIZE - 1) / PKG_SIZE;
        value[x] = 10000. / weight[x] * pow(1 - loss_prob, layer[x]);
    }
}

static bool vis[NODE_NUM_MAX];
static void dfs(int cur)
{
    log_printf("calling dfs(%d)\n", cur);
    for (int nxt : edges[cur])
        if (!vis[nxt]) {
            vis[nxt] = 1;
            dfs(nxt);
            seq.push_back(nxt);
        }
}


static bool cmp(int u, int v) {
    return value[u] > value[v];
}
void make_package()
{
    /* Stage 0: Sort by value */
    for (int x = 1; x <= n; ++x)
        sort(edges[x].begin(), edges[x].end(), cmp);
    /* Stage 1: DFS */
    memset(vis, 0, sizeof(vis));
    dfs(0);
    // reverse(seq.begin(), seq.end());
    debug_printf("stage 1 complete\n");

    /* Stage 2: grouping */
    int last_i = 0;
    int acc_weight = 0;
    auto bak_seq = vector<int>(seq.begin(), seq.end());
    seq.insert(seq.end(), bak_seq.begin(), bak_seq.end());
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
            if (acc_weight + weight[x] > PKG_SIZE) {
                pkgs[++np] = vector<int>(seq.begin() + last_i, seq.begin() + i);
                if (np == 1) {
                    log_printf("pkg %d weight = %d\n", np, acc_weight);
                    for (int x : pkgs[np]) {
                        log_printf("%d ", x);
                    }
                    printf("\n");
                }
                last_i = i;
                acc_weight = weight[x];
            }
            else 
                acc_weight += weight[x];
        }
        ASSERT(acc_weight <= PKG_SIZE);
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
    }

    read_graph(edge_path, weight_path); debug_printf("TW = %d\n", TW);
    evaluate_nodes();
    make_package();
    output(answer_path);
    return 0;
}