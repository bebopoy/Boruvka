#include <iostream>
#include <vector>
#include <algorithm>
#include "Assistance.h"
using namespace std;

struct Edge
{
    // ��ֹ����Լ�Ȩ��
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

    // ���һ���ߵ�ͼ��
    void addEdge(int src, int dest, int weight)
    {
        Edge edge = {src, dest, weight};
        edges.push_back(edge); // ����vextor��
        E++;
    }

    // ���ҵ�ǰԪ�������ļ��ϣ����ڲ��鼯��
    int find(Subset subsets[], int i)
    {
        if (subsets[i].parent != i)
            subsets[i].parent = find(subsets, subsets[i].parent);
        return subsets[i].parent;
    }

    // �ϲ��������ϣ����ڲ��鼯��
    void Union(Subset subsets[], int x, int y)
    {
        int xroot = find(subsets, x);
        int yroot = find(subsets, y);
        // ���ز���
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

    // Boruvka�㷨��ʵ��
    void boruvkaMST()
    {
        // ����һ���������ڴ洢ÿ�������е���С��
        Edge *cheapest = new Edge[V];

        // ����һ�����鼯
        Subset *subsets = new Subset[V];
        int *parent = new int[V];

        // ��ʼ�����鼯����С������
        for (int v = 0; v < V; ++v)
        {
            subsets[v].parent = v;
            subsets[v].rank = 0;
            cheapest[v].weight = DEFAULT_INFINITY;
            parent[v] = -1;
        }

        int numTrees = V;
        int MSTweight = 0;

        // ѭ��ֱ��ֻʣ��һ����
        while (numTrees > 1)
        {
            // �ҳ�ÿ�����������ӵ��������ϵ���С��
            for (int i = 0; i < E; i++)
            {
                int set1 = find(subsets, edges[i].src);
                int set2 = find(subsets, edges[i].dest);

                if (set1 == set2)
                    continue;

                // ������С�߼�
                if (cheapest[set1].weight > edges[i].weight)
                    cheapest[set1] = edges[i];

                if (cheapest[set2].weight > edges[i].weight)
                    cheapest[set2] = edges[i];
            }

            // ����ÿ�����ϣ���������С�������ӵļ��Ϻϲ�
            for (int i = 0; i < V; i++)
            {
                if (cheapest[i].weight != DEFAULT_INFINITY)
                {
                    int set1 = find(subsets, cheapest[i].src);
                    int set2 = find(subsets, cheapest[i].dest);

                    if (set1 == set2)
                        continue;

                    // ��Ȩ��ֵ
                    MSTweight += cheapest[i].weight;
                    Union(subsets, set1, set2);
                    parent[cheapest[i].src] = cheapest[i].dest;
                    numTrees--;
                }
            }
        }

        // �����С��������Ȩ�غͱ�
        cout << "Weight of MST is " << MSTweight << endl;
        cout << "Edges of MST are:\n";
        for (int i = 0; i < V; ++i)
            cout << parent[i] << " - " << i << endl;

        cout << "���鼯��� i parent rank " << endl;
        for (int i = 0; i < V; i++)
        {
            cout << i << "\t" << subsets[i].parent << "\t" << subsets[i].rank << endl;
        }

        cout << "��̱�����" << endl;
        for (int i = 0; i < V; i++)
            cout << cheapest[i].src << " <-> " << cheapest[i].dest << endl;
    }
};
