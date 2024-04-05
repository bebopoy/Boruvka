#include <iostream>
#include <vector>
#include <algorithm>
#include "Assistance.h"
using namespace std;

struct Edge
{
    // 起、止序号以及权重
    int src, dest, weight;
};

struct Subset
{
    int parent, rank;
};

class Graph
{
    int V, E;
    vector<Edge> edges;

public:
    Graph(int v) : V(v), E(0) {}

    // 添加一条边到图中
    void addEdge(int src, int dest, int weight)
    {
        Edge edge = {src, dest, weight};
        edges.push_back(edge); // 加入vextor中
        E++;
    }

    // 查找当前元素所属的集合（用于并查集）
    int find(Subset subsets[], int i)
    {
        if (subsets[i].parent != i)
            subsets[i].parent = find(subsets, subsets[i].parent);
        return subsets[i].parent;
    }

    // 合并两个集合（用于并查集）
    void Union(Subset subsets[], int x, int y)
    {
        int xroot = find(subsets, x);
        int yroot = find(subsets, y);
        // 挂载策略
        if (subsets[xroot].rank < subsets[yroot].rank)
            subsets[xroot].parent = yroot;
        else if (subsets[xroot].rank > subsets[yroot].rank)
            subsets[yroot].parent = xroot;
        else
        {
            subsets[yroot].parent = xroot;
            subsets[xroot].rank++;
        }
    }

    // Boruvka算法的实现
    void boruvkaMST()
    {
        // 创建一个数组用于存储每个集合中的最小边
        Edge *cheapest = new Edge[V];

        // 创建一个并查集
        Subset *subsets = new Subset[V];
        int *parent = new int[V];

        // 初始化并查集和最小边数组
        for (int v = 0; v < V; ++v)
        {
            subsets[v].parent = v;
            subsets[v].rank = 0;
            cheapest[v].weight = DEFAULT_INFINITY;
            parent[v] = -1;
        }

        int numTrees = V;
        int MSTweight = 0;

        // 循环直到只剩下一棵树
        while (numTrees > 1)
        {
            // 找出每个集合中连接到其他集合的最小边
            for (int i = 0; i < E; i++)
            {
                int set1 = find(subsets, edges[i].src);
                int set2 = find(subsets, edges[i].dest);

                if (set1 == set2)
                    continue;

                // 更新最小边集
                if (cheapest[set1].weight > edges[i].weight)
                    cheapest[set1] = edges[i];

                if (cheapest[set2].weight > edges[i].weight)
                    cheapest[set2] = edges[i];
            }

            // 对于每个集合，将其与最小边所连接的集合合并
            for (int i = 0; i < V; i++)
            {
                if (cheapest[i].weight != DEFAULT_INFINITY)
                {
                    int set1 = find(subsets, cheapest[i].src);
                    int set2 = find(subsets, cheapest[i].dest);

                    if (set1 == set2)
                        continue;

                    // 总权重值
                    MSTweight += cheapest[i].weight;
                    Union(subsets, set1, set2);
                    parent[cheapest[i].src] = cheapest[i].dest;
                    numTrees--;
                }
            }
        }

        // 输出最小生成树的权重和边
        cout << "Weight of MST is " << MSTweight << endl;
        cout << "Edges of MST are:\n";
        for (int i = 0; i < V; ++i)
            cout << parent[i] << " - " << i << endl;

        cout << "并查集情况 i parent rank " << endl;
        for (int i = 0; i < V; i++)
        {
            cout << i << "\t" << subsets[i].parent << "\t" << subsets[i].rank << endl;
        }

        cout << "最短边连线" << endl;
        for (int i = 0; i < V; i++)
            cout << cheapest[i].src << " <-> " << cheapest[i].dest << endl;
    }
};
