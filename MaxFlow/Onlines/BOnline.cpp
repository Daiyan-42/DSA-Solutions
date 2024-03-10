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

	int N,M,S;
	cin >> N >> M;
    
    vector<int> workers(N); 
    vector<int> jobs(M);  

    for (int i = 0; i < N; ++i) {
        cin >> S;
        workers[i] = S;   
    }

    for (int i = 0; i < M; ++i) {
        cin >> S;
        jobs[i] = S; 
    }

    vector<vector<int>> graph(N, vector<int>(M, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            
            if (workers[i] >= jobs[j]) graph[i][j] = 1;
        } 
    }
    
    cout << maxBPM(graph,N,M) << endl;

    return 0;
}
