#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <list>
#include <limits>
#include "graph.h"
using namespace std;

class Node2 {
    public:
        int index;
        double value;
        Node2(int a,double b):index(a), value(b){}
        Node2(){}
        bool operator< (const Node2& n) const {
            return value < n.value;
        }
};
double loss_prob = 0.2;

vector<int> edges[NODE_NUM_MAX];
int weight[NODE_NUM_MAX];
int TW;
int n;  // tot node number
int in_deg[NODE_NUM_MAX];
vector<int> in_node[NODE_NUM_MAX];
double value[NODE_NUM_MAX];
int np = 1;  // tot package number
vector<int> pkgs[PKG_NUM_MAX];
int rcd_pkg[NODE_NUM_MAX];
priority_queue<Node2> nodes; 
vector<vector<int>> rst;

void evaluate_nodes()
{
    /*static int acc_w[NODE_NUM_MAX];
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
    }*/

    
    for (int x = 1; x <= n; ++x) {
        //layer[x] = (acc_w[x] + PKG_SIZE - 1) / PKG_SIZE;
        //value[x] = 10000. / weight[x] * pow(1 - loss_prob, layer[x]);
        value[x] = in_deg[x] + edges[x].size();
        nodes.push(Node2(x, value[x]));
    }
}

static bool vis[NODE_NUM_MAX];
static int tw = 0;
static int cur_weight_sum = 0;
static int extra_np = 0;
static void dfs(int cur)
{
    log_printf("calling dfs(%d), cur_weight_sum = %d\n", cur, cur_weight_sum);
    for (int nxt : edges[cur]){
        if(cur && find(in_node[nxt].begin(), in_node[nxt].end(), cur) == in_node[nxt].end()){
            in_node[nxt].push_back(cur);
        }
        if (!vis[nxt]) {
            vis[nxt] = 1;
            
            if (cur_weight_sum + weight[nxt] <= PKG_SIZE) {
                log_printf("case1, cur_weight_sum = %d, weight[%d] = %d\n", cur_weight_sum, nxt, weight[nxt]);
                cur_weight_sum += weight[nxt];
                pkgs[np].push_back(nxt);
            }
            else {
                cur_weight_sum = weight[nxt];
                pkgs[++np].push_back(nxt);
            }
            tw += weight[nxt];

            dfs(nxt);
        }
    }
        
}

void make_package()
{
    dfs(0);
    rst = vector<vector<int>>(pkgs+1, pkgs+np+1);
    vector<int> cur_pkg;
    int acc_weight = 0;
    while(!nodes.empty()){
        int now = nodes.top().index;nodes.pop();
        if(acc_weight + weight[now] > PKG_SIZE){
            rst.push_back(cur_pkg);
                tw += acc_weight;
                cur_pkg.clear();
                acc_weight = 0;
                continue;
        }
        acc_weight += weight[now];
        cur_pkg.push_back(now);
        for(int i:in_node[now]){
            if((tw + acc_weight) * 2 > TW * 3)break;
            if(acc_weight + weight[i] > PKG_SIZE){
                
                rst.push_back(cur_pkg);
                tw += acc_weight;
                cur_pkg.clear();
                acc_weight = 0;
                break;
            }
            else{
                cur_pkg.push_back(i);
                acc_weight += weight[i];
            }
        }
        if((tw + acc_weight) * 2 > TW * 3)break;
        
    }
    
    debug_printf("tw = %d\n", tw);
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