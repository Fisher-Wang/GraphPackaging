#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>

using namespace std;

const int NODE_NUM_MAX = 200005;
const int PKG_NUM_MAX = 200005;
const int PKG_SIZE = 1572;
const double loss_prob = 0.2;

vector<int> edges[NODE_NUM_MAX];
int weight[NODE_NUM_MAX];
int TW;
int n;  // tot node number
int in_deg[NODE_NUM_MAX];
double value[NODE_NUM_MAX];
int np; // tot package number
int pkg[PKG_NUM_MAX];
vector<vector<int>> rst;

int read_graph(char* edge_path, char* weight_path)
{
    memset(weight, 255, sizeof(weight));
    TW = 0;

    FILE *f_edge, *f_weight;
    if ((f_edge = fopen(edge_path, "r")) == NULL)
    {
        fprintf(stderr, "Failed to open edge file: %s\n", edge_path);
        return -1;
    }
    if ((f_weight = fopen(weight_path, "r")) == NULL)
    {
        fprintf(stderr, "Failed to open weight file: %s\n", weight_path);
        return -1;
    }
    int x, y;
    while (fscanf(f_weight, "%d %d", &x, &y)) {
        if (x == 0 && y == 0)
            break;
        if (weight[x] == -1) {
            weight[x] = y;
            TW += weight[x];
        }
    }
    while (fscanf(f_edge, "%d %d", &x, &y)) {
        if (x == 0 && y == 0)
            break;
        edges[x].push_back(y);
        n = max(n, x);
    }
    fclose(f_edge);
    fclose(f_weight);
    // 去重边
    for (int x = 1; x <= n; ++x) {
        sort(edges[x].begin(), edges[x].end());
        unique(edges[x].begin(), edges[x].end());
    }
    // 计算每个节点的入度
    for (int x = 1; x <= n; ++x)
        for (int y : edges[x])
            in_deg[y] += 1;
    // 0号节点连向所有入度为0的节点
    for (int x = 1; x <= n; ++x)
        if (in_deg[x] == 0)
            edges[0].push_back(x);
    return 0;
}

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
            return value > n.value;
        }
    };
    priority_queue<Node> pq;
    pq.push({0,0});
    vector<int> seq;

    /* Stage 1: decide which node to be packaged */
    while (!pq.empty()) {
        int cur = pq.top().index; pq.pop();
        if ((tw + weight[cur]) * 2 > TW * 3)
            break;
        if (cur != 0) {
            tw += weight[cur];
            seq.push_back(cur);
        }
        
        /* add new nodes to pq */
        for (int nxt : edges[cur]) {
            double v = value[nxt] * pow(loss_prob, cnt[nxt]);
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
}

int output(char* answer_path)
{
    FILE* f_answer;
    if ((f_answer = fopen(answer_path, "w+")) == NULL) {
        fprintf(stderr, "[Error] Failed to open edge file: %s\n", answer_path);
        return -1;
    }
    fprintf(f_answer, "%u\n", rst.size());
    for (vector<int>& grp : rst) {
        fprintf(f_answer, "%u", grp.size());
        for (int idx : grp)
            fprintf(f_answer, " %d", idx);
        fprintf(f_answer, "\n");
    }
    fclose(f_answer);
    return 0;
}

int main()
{
    char weight_path[] = "data/Out_SliceSize_Basketball_480_Slice16_Gop8_10.log";
    char edge_path[] = "data/Out_OutGraph_Basketball_480_Slice16_Gop8_10.log";
    char answer_path[] = "result.txt";
    read_graph(edge_path, weight_path);
    evaluate_nodes();
    make_package();
    output(answer_path);
    return 0;
}