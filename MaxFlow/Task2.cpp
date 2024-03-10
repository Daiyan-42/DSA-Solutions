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

	int m,n,h,a,d;
	cin >> m >> n;

    vector<vector<int>> men(m, vector<int>(3, 0)); 
    vector<vector<int>> women(n, vector<int>(3, 0));  

    for (int i = 0; i < m; ++i) {
        cin >> h >> a >> d;
        men[i] = {h, a, d};  
    }

    for (int i = 0; i < n; ++i) {
        cin >> h >> a >> d;
        women[i] = {h, a, d}; 
    }

    vector<vector<int>> graph(m, vector<int>(n, 0));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            
            if ((abs(men[i][0] - women[j][0]) <= 10) && (abs(men[i][1] - women[j][1]) <= 5) && (men[i][2] == women[j][2])) graph[i][j] = 1;
        } 
    }
    
    cout << maxBPM(graph,m,n) << endl;

    return 0;
}
