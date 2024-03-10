#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

bool DFS(vector<vector<int>> &graph, int u, vector<bool>& visited, int partner[], int m, int n) {
    
    for (int v=0;v<n;v++) {

        if (graph[u][v] && !visited[v])
        {
            visited[v] = true; 
 
            if (partner[v] < 0 || DFS(graph, partner[v],visited,partner,m,n))
            {
                partner[v] = u;
                return true;
            }
        }
    }
    return false;
}
 

int maxBPM(vector<vector<int>> &graph, int m, int n) {

    int partner[n];
    memset(partner, -1, sizeof(partner));
    int maxbpm = 0;

    
    for (int h = 0; h < m ; h++) {

        vector<bool> visited(n, false);

        if(DFS(graph,h,visited,partner,m,n)) {
            maxbpm++;
        }
    }
    return maxbpm;
}


int main() {

	int m,n,u,v;
	cin >> n >> m;

    vector<vector<int>> graph(m, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        vector<int> applicants;
        
        graph[u][v] = 1;
    }
    
    cout << maxBPM(graph,m,n) << endl;

    return 0;
}
