#include <iostream>
#include <vector>
#define INF 0x3f3f3f3f

using namespace std;

void floydWarshall(vector<vector<int>>& graph, int V, int thres) {
    
    vector<vector<int>> dist(graph);
    
    
    for (int k = 0; k < V; ++k) {
        
        for (int i = 0; i < V; ++i) {
            
            for (int j = 0; j < V; ++j) {
                
                if (i==j) { dist[i][j] = 0; continue; }
                if ((dist[i][k] != INF && dist[k][j] != INF) && ((dist[i][k] + dist[k][j]) < dist[i][j])) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    vector<int> reachableCities(V, 0);
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if(dist[i][j] <= thres && dist[i][j] > 0) { reachableCities[i] += 1; }
        }
    }

    int min = INF;
    for(int j=0;j<V;j++) 
        if(reachableCities[j] < min) min = reachableCities[j];

    for(int j=0;j<V;j++) 
        if(reachableCities[j] == min) cout << j+1 << " ";  
}

int main() {
    int V, E; 
    cin >> V >> E;

    vector<vector<int>> graph(V, vector<int>(V, INF));
    int src, dest, weight;
    for (int i = 0; i < E; ++i) {
        
        cin >> src >> dest >> weight;
        graph[src-1][dest-1] = weight;
        graph[dest-1][src-1] = weight;
    }

    int thres;
    cin >> thres;

    floydWarshall(graph, V, thres);

    return 0;
}
