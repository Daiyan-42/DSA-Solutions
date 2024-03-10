#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

bool dfs(vector<vector<int>> &rGraph, int s, int t, int parent[], int V) {
    
    bool visited[V];
    memset(visited, 0, sizeof(visited));

    int stack[V];
    int top = -1;
    stack[++top] = s;
    visited[s] = true;
    parent[s] = -1;

    while (top >= 0) {
        int u = stack[top--];

        for (int v = 0; v < V; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                stack[++top] = v;
                parent[v] = u;
                visited[v] = true;

                if (v == t)
                    return true;
            }
        }
    }

    return false;
}


void fordFulkerson(vector<vector<int>> graph, int s, int t, int V) {
    int u, v;

    vector<vector<int>> rGraph(graph);
    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
             rGraph[u][v] = graph[u][v];

    int parent[V];
    vector<int> maxInflow(V, 0);
    int max_flow = 0;

    while (dfs(rGraph, s, t, parent, V)) {
        
        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
            maxInflow[v] += path_flow;
        }

        max_flow += path_flow;
    }

    int MaxNode, MaxInflow = INT_MIN;
    for (int i=0;i<V;++i) {
    	if (MaxInflow < maxInflow[i] && i != t && i != s) { MaxInflow = maxInflow[i];
    	MaxNode = i; }
    }
    cout << MaxNode+1 << " " << MaxInflow << endl;
}


int main() {

	int n,m,u,v,w,s,t;
	cin >> n >> m;

    vector<vector<int>> graph(n, vector<int>(n, 0));
 
    for (int i=0;i<m;++i) {
    	cin >> u >> v >> w;
    	graph[u-1][v-1] = w;
    }
    	
    cin >> s >> t;
    fordFulkerson(graph, s-1, t-1, n);

    return 0;
}
