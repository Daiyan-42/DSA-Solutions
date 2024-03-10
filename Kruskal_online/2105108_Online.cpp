#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

int distance(int x1, int x2) {
	int dif = x1-x2;
	if(dif < 0) return -dif;
	else return dif;
}

struct house {
public:
	int v;
	pair<int,int> cord;
	house(int _v, int x, int y) { v = _v; cord.first = x; cord.second = y; }
};

struct edge {
public:
	int h1;
    int h2;
    int w;
    edge(int u, int v, int weight) : h1(u), h2(v), w(weight) {}
};

bool compareEdges(const edge &a, const edge &b) {
    return a.w < b.w;
}

int find(int i, vector<int>& parent){
    while(true){
        if (parent[i] < 0)
            break;
        i = parent[i];
    }
    return i;
}

void unite(int x, int y, vector<int>& parent, vector<int>& rank) { 
        int s1 = find(x, parent); 
        int s2 = find(y, parent); 
  
        if (s1 != s2) { 
            if (rank[s1] < rank[s2]) { 
                parent[s1] = s2; 
            } 
            else if (rank[s1] > rank[s2]) { 
                parent[s2] = s1; 
            } 
            else { 
                parent[s2] = s1; 
                rank[s1] += 1; 
            } 
        } 
    }

int kruskalMST(vector<edge>& graph, int V) {

	sort(graph.begin(), graph.end(), compareEdges);

	vector<int> parent(V, -1);
	vector<int> rank(V, 1);

	int totalWeight = 0;

	for (auto Edge : graph) {

		if (find(Edge.h1, parent) != find(Edge.h2, parent)) { 
                unite(Edge.h1, Edge.h2, parent, rank); 
                totalWeight += Edge.w;
            }
	}

	cout << totalWeight << endl;
	return totalWeight;
}

int main() {
	
	int n,u,v;
	cin >> n;
	vector<house> houses;
	for(int i = 0;i<n;i++) {
		cin >> u >> v;
		houses.push_back(house(i,u,v));
	}

	vector<edge> pipelines;
	for (int i=0; i<n; i++) {
		for (int j=i; j < n; j++)
		{	
			if(i==j) continue;

			int w = distance(houses[i].cord.first, houses[j].cord.first) + distance(houses[i].cord.second, houses[j].cord.second);
			pipelines.push_back(edge(houses[i].v, houses[j].v, w));
		}
	}

	kruskalMST(pipelines, n);

	return 0;
}