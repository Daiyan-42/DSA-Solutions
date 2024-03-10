#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <climits>

using namespace std;

struct Edge {
    int u,v,w;
};


bool relax(vector<int>& dist, Edge edge, vector<int>& parent) {
    if (dist[edge.u] != INT_MAX && dist[edge.u] + edge.w < dist[edge.v]) {
        dist[edge.v] = dist[edge.u] + edge.w;
        parent[edge.v] = edge.u;
        return true;
    } else { return false; }
}

void bellmanFord_testNewEdge(vector<Edge>& graph, int V, int A, int B, int L, int H, int src, int dest) {
    vector<int> dist(V+1, INT_MAX);
    vector<int> parent(V+1,-1);
    graph.push_back({A,B,L});
    dist[src] = 0;

    for (int i = 1; i < V; ++i) {
        for (const auto& edge : graph) {
            relax(dist, edge, parent);
        }
    }

    bool negCycle = false;
    for (const auto& edge : graph) {
        if (relax(dist, edge, parent)) { negCycle = true; }
    }

    if (!negCycle)
    {
        cout << L << " " << dist[dest] << endl;
    } else {
        int v = B, minWeight = 0;
        while(true) {
            for(auto edge: graph) {
                if (edge.u == parent[v] && edge.v == v)
                    minWeight += edge.w; 
            }
            if (parent[v] == B) break;
            v = parent[v];
        }
        
        int W = L - minWeight;
        if (W >= H || W <= L) { cout << "impossible" << endl; }
        else {
            vector<int> anotherdist(V+1, INT_MAX);
            graph.pop_back(); 
            graph.push_back({A,B,W});
            anotherdist[src] = 0;
            for (int i = 1; i < V; ++i) {
                for (const auto& edge :graph) {
                    if (anotherdist[edge.u] != INT_MAX && anotherdist[edge.u] + edge.w < anotherdist[edge.v])
                                anotherdist[edge.v] = anotherdist[edge.u] + edge.w;
                    } 
                }

                cout << W << " " << anotherdist[dest] << endl;
        }
    }

}

int main() {
    int N,M,u,v,w;
    cin >> N >> M;

    vector<Edge> graph(M);
    for (int i = 0; i < M; ++i) {
            cin >> u >> v >> w;
            graph.push_back({u,v,w});
    }

    int A,B,L,H,s,d;
    cin >> A >> B >> L >> H >> s >> d;

    bellmanFord_testNewEdge(graph, N, A, B, L, H, s, d);

    return 0;
}
