#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <list>
#include <climits>
#include <numeric>
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
double value2[NODE_NUM_MAX];
int np = 1;  // tot package number
vector<int> pkgs[PKG_NUM_MAX];
vector<vector<int>> rst;


void evaluate_nodes()
{
    double p = 1 - loss_prob;
    double p2 = p*(2-p);
    printf("p = %.2f, p2 = %.2f\n", p, p2);
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
        log_printf("h = %d\n", h);
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

    int c = 0;
    for (int x = 1; x <= n; ++x) {
        layer[x] = (acc_w[x] + PKG_SIZE - 1) / PKG_SIZE;
        if (layer[x] > 1)
            ++c;
        value[x] = pow(p, layer[x]);
        value2[x] = pow(p2, layer[x]);
        log_printf("acc_w[%d] = %d, value = %.1f, value2 = %.1f\n", x, acc_w[x], value[x], value2[x]);
    }
    printf("there are %d nodes whose layer > 1\n", c);
}

double lim1, lim2;
void assess_limit()
{
    evaluate_nodes();
    lim1 = accumulate(value+1, value+n+1, 0);
    lim2 = accumulate(value2+1, value2+n+1, 0);
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
    assess_limit();
    printf("lim1 = %.1f%%, lim2 = %.1f%%\n", lim1 / 864, lim2 / 864);
    output(answer_path);
    return 0;
}