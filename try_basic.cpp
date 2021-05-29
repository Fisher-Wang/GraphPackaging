#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include "graph.h"
using namespace std;

const double loss_prob = 0.2;

vector<int> edges[NODE_NUM_MAX];
int weight[NODE_NUM_MAX];
int TW;
int n;  // tot node number
int in_deg[NODE_NUM_MAX];
double value[NODE_NUM_MAX];
int np; // tot package number
vector<int> seq;
vector<vector<int>> rst;

void evaluate_nodes()
{
    for (int x = 1; x <= n; ++x) {
        value[x] = 1. / weight[x];
    }
}

void make_package()
{
    static int cnt[NODE_NUM_MAX];
    memset(cnt, 0, sizeof(cnt));
    int tw = 0;  // current total weight
    struct Node {
        int index;
        double value;
        bool operator< (const Node& n) const {
            return value < n.value;
        }
    };
    priority_queue<Node> pq;  // 大顶堆
    pq.push({0,0});

    /* Stage 1: decide which node to be packaged */
    while (!pq.empty()) {
        int cur = pq.top().index;
        // printf("pq: cur = %d, weight = %d, v = %.2f\n", cur, weight[cur], pq.top().value * 1000);
        pq.pop();
        if ((tw + weight[cur]) * 2 > TW * 3)
            break;
        if (cur != 0) {
            tw += weight[cur];
            seq.push_back(cur);
        }
        
        /* add new nodes to pq */
        for (int nxt : edges[cur]) {
            double v = value[nxt];
            // double v = value[nxt] * pow(loss_prob, cnt[nxt]);
            // printf("nxt = %d, weight = %d, v = %.2f, in_deg = %d\n", nxt, weight[nxt], v * 1000, in_deg[nxt]);
            pq.push({nxt, v});
            ++cnt[nxt];
        }
    }

    /* Stage 2: grouping */
    int last_i = 0;
    int acc_weight = 0;
    for (int i = 0; i < (int)seq.size(); ++i)
    {
        int x = seq[i];
        if (weight[x] > PKG_SIZE) {
            fprintf(stderr, "[Warning] node %d has too large weight %d\n", x, weight[x]);
            continue;
        }
        if (acc_weight + weight[x] > PKG_SIZE) {
            rst.push_back(vector<int>(seq.begin() + last_i, seq.begin() + i));
            last_i = i;
            acc_weight = weight[x];
        }
        else 
            acc_weight += weight[x];
    }
    rst.push_back(vector<int>(seq.begin() + last_i, seq.end()));

    debug_printf("tw = %d\n", tw);
}

int main()
{
    if (LOG) {
        freopen("log.txt", "w", stdout);
    }
    char weight_path[] = "data/Out_SliceSize_Basketball_480_Slice16_Gop8_10.log";
    char edge_path[] = "data/Out_OutGraph_Basketball_480_Slice16_Gop8_10.log";
    char answer_path[] = "result.txt";
    read_graph(edge_path, weight_path); debug_printf("TW = %d\n", TW);
    evaluate_nodes();
    make_package();
    output(answer_path);
    return 0;
}