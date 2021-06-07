#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <list>
#include <climits>
#include "graph.h"
using namespace std;

const double INF_DOUBLE = 1e20;

double loss_prob = 0.2;

vector<int> edges[NODE_NUM_MAX];
int weight[NODE_NUM_MAX];
int TW;
int n;  // tot node number
int in_deg[NODE_NUM_MAX];
double value[NODE_NUM_MAX];
int np = 1;  // tot package number
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
                if (acc_w[nxt] == -1) {
                    acc_w[nxt] = weight[nxt] + acc_w[cur];
                    q2->push_back({nxt});
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

static int cnt[NODE_NUM_MAX];
struct Node {
    int index;
    int count;
    double value;
    bool operator< (const Node& n) const {
        return value < n.value;
    }
};
static priority_queue<Node> pq; // 大顶堆

static int tw2 = 0;
void dfs_packaging_helper(int cur, int c)
{
    static int cur_weight_sum = 0;
    static bool vis[NODE_NUM_MAX] = {0};
    if (cur == 0)
        memset(vis, 0, sizeof(vis));

    for (int nxt : edges[cur]) {
        if (!vis[nxt] && c <= cnt[nxt]) {
            vis[nxt] = true;

            if (cur_weight_sum + weight[nxt] <= PKG_SIZE) {
                /* 当前包的剩余容量够装 */
                cur_weight_sum += weight[nxt];
                pkgs[np].push_back(nxt);
            }
            else {
                /* 当前包的剩余容量不够装，开一个新的包 */
                cur_weight_sum = weight[nxt];
                pkgs[++np].push_back(nxt);
            }
            tw2 += weight[nxt];

            dfs_packaging_helper(nxt, c);
        }
    }
}

static int tw = 0;
void make_packages()
{
    /* Stage 1: decide which node(and how many times) to be packaged */
    pq.push({0,1,INF_DOUBLE});

    int flag = 1;

    static int cnt2[NODE_NUM_MAX];

    for (;;) {
        Node node = pq.top(); pq.pop();
        int cur = node.index;

        if ((tw + weight[cur]) * 2 > TW * 3)
            break;
        tw += weight[cur];

        ASSERT(cnt[cur] == node.count - 1);
        cnt[cur] = node.count;
        for (int nxt : edges[cur]) {
            if (cnt2[nxt] < cnt[cur]) {
                ASSERT(cnt2[nxt] == cnt[cur]-1);
                pq.push({nxt, cnt[cur], value[nxt] * pow(loss_prob, cnt[cur]-1)});
                ++cnt2[nxt];
            }
        }
        
        /* 只要有非零的k层次节点被扩展，那么k+1层次的节点就要做好准备 */
        if (cur != 0 && cnt[cur] == flag) {
            flag += 1;
            pq.push({0,cnt[cur]+1,INF_DOUBLE});
        }
    }
    /* Stage 2: grouping */
    for (int c = 1; c < flag; ++c) {
        dfs_packaging_helper(0, c);
        ++np; // 要求两个层次的节点不能混装在一个包中
    }

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
    make_packages();
    printf("np = %d, rst.size() = %u\n", np, rst.size());
    printf("tw = %d, tw2 = %d\n", tw, tw2);
    output(answer_path);
    return 0;
}