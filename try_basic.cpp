#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

const int NODE_NUMBER_MAX = 200005;

vector<int> edges[NODE_NUMBER_MAX];
int weight[NODE_NUMBER_MAX];
int tot_weight;
int n;  // tot node number
int in_deg[NODE_NUMBER_MAX];

int read_graph(char* edge_path, char* weight_path)
{
    memset(weight, 255, sizeof(weight));
    tot_weight = 0;

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
            tot_weight += weight[x];
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

int main()
{

}