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
    
    if(edges[cur].empty())return 1;
    int s = edges[cur].size();
    double temp_val = 0;
    for(int i = 0;i < s;i++)
    {
        if(!vis[edges[cur][i]]){
            if(loss_prob > 0.3)temp_val += preset(edges[cur][i], depth+1)*pow(1-loss_prob, depth);
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
    //debug_printf("%d\n", tw);
    return tw;
}
void evaluate_nodes()
{
    preset(0, 0);
    for (int x = 1; x <= n; ++x) {
        value[x] += 1000. / weight[x];
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
            //seq.push_back(cur);
        
        
            /* add new nodes to pq */
            /*for (int nxt : edges[cur]) {                        //尚未加入pack链当中的点的其他分支。
                if(vis[nxt])continue;
                double v = value[nxt];
                // double v = value[nxt] * pow(loss_prob, cnt[nxt]);
                // printf("nxt = %d, weight = %d, v = %.2f, in_deg = %d\n", nxt, weight[nxt], v * 1000, in_deg[nxt]);
                pq.push(Node(nxt, v));
                ++cnt[nxt];                         
            }*/
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
    /*
     Stage 2: grouping 
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
    */
    //copy the packets
    debug_printf("stage 1:tw = %d\n",tw);
    
    
    int s = rst.size();
    for(int i = 0;i < s;i++){
        for(int j = 0;j < s-i-1;j++){
            if(pkg_w[j] < pkg_w[j+1]){
                swap(rst[j], rst[j+1]);
                int k = pkg_w[j];
                pkg_w[j] = pkg_w[j+1];
                pkg_w[j+1] = k;
                double v = pkg_val[j];
                pkg_val[j] = pkg_val[j+1];
                pkg_val[j+1] = v;
            }
        }
    }
    
    int acc_weight = 0;
    double acc_val = 0;
    int last_i = s-1;
    for(int i = s-1;i > 0;i--){
        if(acc_weight + pkg_w.back() <= PKG_SIZE){
            acc_weight += pkg_w.back();
            pkg_w.pop_back();
            acc_val += pkg_val.back();
            pkg_val.pop_back();
        }
        else if(last_i == i + 1)break;
        else{
            vector<int> new_ans;
            for(int j = last_i;j > i;j--){
                new_ans.insert(new_ans.end(), rst.back().begin(), rst.back().end());
                rst.pop_back();
            }
            pkg_w.insert(pkg_w.begin(), acc_weight);
            pkg_val.insert(pkg_val.begin(), acc_val);

            rst.insert(rst.begin(), new_ans);
            
            break;
            acc_weight = 0;
            acc_val = 0;
            last_i = i;
            i++;
        }
    }
    np = pkg_val.size();
    debug_printf("stage 2 :tw = %d, np = %d\n", tw, np);
    /*for(int i = 0;i < np;i++){
        //debug_printf("packet %d: weight = %d",i, pkg_w[i]);
        int k = rst[i].size();
        for(int j = 0;j < k;j++){
            debug_printf("%d ", rst[i][j]);
        }
        debug_printf("\n");
    }*/
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
        //debug_printf("%d\n", pkg_w[index]);
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