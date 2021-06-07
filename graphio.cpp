#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cassert>
#include "graph.h"
using namespace std;

extern int weight[NODE_NUM_MAX];
extern int TW;
extern vector<int> edges[NODE_NUM_MAX];
extern int n;  // tot node number
extern int in_deg[NODE_NUM_MAX];
extern vector<vector<int>> rst;

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
        assert(weight[x] != -1 && weight[y] != -1);    
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
    assert(edges[0].empty());
    for (int x = 1; x <= n; ++x)
        if (in_deg[x] == 0)
            edges[0].push_back(x);
    
    return 0;
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