/* Version: 2021.05.25 */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

int losspro = 0;

#define SEED 0
#define LOOSEPRO losspro
#define LOOSEN 4
#define MAXW 1572

struct edge
{
    int to, next;
} e[400005];

int to[200005], topo[200005];
unsigned int w[200005], totw[200005], tw, Tw;
int head[200005], cnt, n, np, belong[30000005], d[200005], D[200005];
bool b[200005], B[200005];

struct link
{
    int x, next;
} l[30000005];

int lhead[200005], lcnt;

void add(int u, int v)
{
    e[++cnt] = (edge){v, head[u]};
    head[u] = cnt;
    d[v]++, D[v]++;
}

void lins(int u, int v)
{
    l[++lcnt] = (link){v, lhead[u]};
    lhead[u] = lcnt;
}

unsigned int Rand() { return (rand() << 16) + rand(); }

bool gettopo()
{
    int r = 0, l = 1;
    for (int i = 0; i < n; i++)
        if (d[i] == 0)
            topo[++r] = i;
    while (r >= l)
    {
        int u = topo[l++];
        for (int i = head[u]; i; i = e[i].next)
        {
            int v = e[i].to;
            d[v]--;
            if (d[v] == 0)
                topo[++r] = v;
        }
    }
    return r < n;
}

int checkvalue()
{
    int ans = 0;
    for (int i = 0; i < n; i++)
        B[i] = 0, d[i] = D[i];
    for (int i = 0; i < np; i++)
        if (!b[i])
            for (int j = lhead[i]; j; j = l[j].next)
                B[l[j].x] = 1;
    for (int i = 1; i <= n; i++)
    {
        int u = topo[i];
        if (!B[u])
            continue;
        if (d[u])
            continue;
        ans++;
        for (int j = head[u]; j; j = e[j].next)
            d[e[j].to]--;
    }
    return ans;
}

void test1()
{
    for (int i = 0; i < np; i++)
        b[i] = 0;
    unsigned int size = 1LL * Tw * LOOSEPRO / 100;
    while (true)
    {
        unsigned int t = Rand() % Tw, p = belong[t];
        if (!b[p])
        {
            if (totw[p] < size)
                b[p] = 1, size -= totw[p];
            else
            {
                if (Rand() % totw[p] < size)
                    b[p] = 1;
                break;
            }
        }
    }
    printf("You get %d frames in mode 1(total frame numer=%d)\n", checkvalue(), n);
}

int getpr(int p, unsigned int size)
{
    int pr = p;
    while (true)
    {
        if (totw[pr] < size)
            size -= totw[pr];
        else
        {
            if (Rand() % totw[pr] >= size)
            {
                pr--;
                if (pr == -1)
                    pr = np - 1;
            }
            break;
        }
        pr++;
        if (pr == np)
            pr = 0;
    }
    return pr;
}

void test2()
{
    for (int i = 0; i < np; i++)
        b[i] = 0;
    int size = 1LL * Tw * LOOSEPRO / 100 / LOOSEN;
    for (int I = 0; I < LOOSEN;)
    {
        unsigned int t = Rand() % Tw;
        int p = belong[t], pr = getpr(p, size);
        if ((pr + 1) % np == p)
            continue;
        if (pr >= p)
        {
            bool flag = false;
            for (int j = p; j <= pr; j++)
                if (b[j])
                {
                    flag = true;
                    break;
                }
            if (flag)
                continue;
            for (int j = p; j <= pr; j++)
                b[j] = true;
            I++;
        }
        else
        {
            bool flag = false;
            for (int j = p; j < np; j++)
                if (b[j])
                {
                    flag = true;
                    break;
                }
            for (int j = 0; j <= pr; j++)
                if (b[j])
                {
                    flag = true;
                    break;
                }
            if (flag)
                continue;
            for (int j = p; j < np; j++)
                b[j] = true;
            for (int j = 0; j <= pr; j++)
                b[j] = true;
            I++;
        }
    }
    printf("You get %d frames in mode 2(total frame numer=%d)\n", checkvalue(), n);
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        puts("invaild input");
        for (int i = 0; i < argc; ++i)
            printf("argv[%d] = %s\n", i, argv[i]);
        return 0;
    }
    srand(time(0));
    srand(SEED);
    FILE *edges, *weights, *answer;
    if ((edges = fopen(argv[1], "r")) == NULL)
    {
        puts("invaild file 1");
        return 0;
    }
    if ((weights = fopen(argv[2], "r")) == NULL)
    {
        puts("invaild file 2");
        return 0;
    }
    if ((answer = fopen(argv[3], "r")) == NULL)
    {
        puts("invaild file 3");
        return 0;
    }
    losspro = atoi(argv[4]);
    int x, y;
    n = 0;
    while (fscanf(weights, "%d %d", &x, &y) != EOF)
    {
        if ((x == 0) && (y == 0))
            break;
        if (to[x])
            continue;
        w[n] = y;
        tw += w[n];
        to[x] = n++;
    }
    while (fscanf(edges, "%d %d", &x, &y) != EOF)
    {
        if ((x == 0) && (y == 0))
            break;
        add(to[x], to[y]);
    }
    if (gettopo())
    {
        puts("not dag");
        return 0;
    }
    fscanf(answer, "%d", &np);

    for (int i = 0; i < np; i++)
    {
        lhead[i] = 0;
        totw[i] = 0;
        int sn;
        fscanf(answer, "%d", &sn);
        for (int j = 0; j < sn; j++)
        {
            fscanf(answer, "%d", &x);
            totw[i] += w[to[x]];
            lins(i, to[x]);
        }
        if (totw[i] > MAXW)
        {
            printf("packet %d too big (count from 0)\n", i);
            return 0;
        }
        for (unsigned int j = 0; j < totw[i]; j++)
            belong[j + Tw] = i;
        Tw += totw[i];
        if (tw * 3 < Tw * 2)
        {
            printf("total packet weights too big\n");
            return 0;
        }
    }
    test1();
    test2();
    printf("Tw = %d\n", Tw);
}
