#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <stack>
#include "graph.h"
using namespace std;

double loss_prob = 0.2;

vector<int> edges[NODE_NUM_MAX];
int weight[NODE_NUM_MAX];
int TW;
int n;  // tot node number
int in_deg[NODE_NUM_MAX];
double value[NODE_NUM_MAX];
int np = 0; // tot package number
vector<int> seq;
vector<vector<int> > rst;
static bool vis[NODE_NUM_MAX];
vector<int> solution;
vector<double> pkg_val;
vector<int> pkg_w;
class Node {
    public:
        int index;
        double value;
        Node(int a,double b):index(a), value(b){}
        Node(){}
        bool operator< (const Node& n) const {
            return value < n.value;
        }
};
static double preset(int cur,int depth)
{
    static bool vis[NODE_NUM_MAX];
    if(edges[cur].empty())return 1;
    int s = edges[cur].size();
    double temp_val = 0;
    for(int i = 0;i < s;i++)
    {
        if(!vis[edges[cur][i]]){
            if(loss_prob > 0.3)temp_val += preset(edges[cur][i], depth+1)*pow(1-loss_prob, depth);         //1、丢包率小的情况下可以做改进。
                                                                                        //2、看前置的装的包的个数
            else temp_val += preset(edges[cur][i], depth+1) * (1-loss_prob);
            vis[edges[cur][i]] = true;
        }
        else temp_val += value[edges[cur][i]];
        
    }
    
    if(cur)value[cur] += temp_val;
    return temp_val;
}
static int pack(int cur)
{
    
    int tw = 0;
    int tot_val = 0;
    stack<int> nodes;
    nodes.push(cur);
    int now = cur;
    solution.clear();
    
    while(!nodes.empty() && tw < PKG_SIZE)
    {
        if(vis[now]){
            now = nodes.top();nodes.pop();
            continue;
        }
        vis[now] = true;
        vector<int> neighbor = edges[now];
        int s = neighbor.size();
        for(int i = 0;i < s;i++){
            nodes.push(neighbor[i]);
        }
        if(tw + weight[now] > PKG_SIZE)break;
        if(now != 0 ){
            tot_val++;
            tw += weight[now];
            solution.push_back(now);
        }
        
        now = nodes.top();nodes.pop();
    }
    if(!tw)return 0;
    pkg_val.push_back(tot_val);
    pkg_w.push_back(tw);
    return tw;
}
void evaluate_nodes()
{
    preset(0, 0);
    for (int x = 1; x <= n; ++x) {
        value[x] += 10000. / weight[x];
        //if(!vis[x])debug_printf("%d\n", x);
    }
    
}

void make_package()
{
    static int cnt[NODE_NUM_MAX];
    memset(cnt, 0, sizeof(cnt));
    memset(vis, false, sizeof(vis));
    int tw = 0;  // current total weight
    
    priority_queue<Node> pq;  // 大顶堆
    pq.push(Node(0,0));
    int num = 0;
    /* Stage 1: decide which node to be packaged */
    while (!pq.empty()) {
        int cur = pq.top().index;
        // printf("pq: cur = %d, weight = %d, v = %.2f\n", cur, weight[cur], pq.top().value * 1000);
        pq.pop();
        if ((tw + weight[cur]) * 2 > TW * 3)                        //按价值降序考虑可以满足重量要求的结点
            break;
        
        else{
            int p = pack(cur);
            if((tw + p) * 2 > TW * 3)break;
            if(!p)continue;
            rst.push_back(solution);
            tw += p;
            num++;
            
            if(!cur){
                for(int nxt:edges[0]){
                    if(vis[nxt])continue;
                    double v = value[nxt];
                    pq.push(Node(nxt, v));
                    ++cnt[nxt];
                }
            }
            for(int node:solution){
                for(int nxt:edges[node]){
                    if(vis[nxt])continue;
                    double v = value[nxt];
                    pq.push(Node(nxt, v));
                    ++cnt[nxt];
                }
            }
        }
    }
    /* Stage 2: grouping */
    //copy the packets
    debug_printf("stage 1:tw = %d, np = %d\n",tw, num);
    np = pkg_val.size();
    for(int i = 0;i < np;i++){
        double imax = 0;
        int index = 0;
        for(int j = 0;j < np;j++){
            if(imax < pkg_val[j]){
                imax = pkg_val[j];
                index = j;
            }
            
        }
        if((tw + pkg_w[index]) * 2 > TW * 3)break;
        rst.push_back(rst[index]);
        tw += pkg_w[index];
        pkg_val[index] *= loss_prob;
        
    }
    debug_printf("tw = %d, np = %d\n", tw, num);

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