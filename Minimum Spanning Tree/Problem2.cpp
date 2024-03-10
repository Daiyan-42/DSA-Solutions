#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <algorithm>

using namespace std; 

class DSU { 
	int* parent; 
	int* rank; 

public: 
	DSU(int n) 
	{ 
		parent = new int[n]; 
		rank = new int[n]; 

		for (int i = 0; i < n; i++) { 
			parent[i] = -1; 
			rank[i] = 1; 
		} 
	} 
 
	int find(int i) 
	{ 
		if (parent[i] == -1) 
			return i; 

		return parent[i] = find(parent[i]); 
	} 

	void unite(int x, int y) 
	{ 
		int s1 = find(x); 
		int s2 = find(y); 

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
}; 

class Graph { 
	vector<vector<int>> edgelist; 
	int V;
	int mstWeight; 

public: 
	Graph(int V) { this->V = V; } 

	void addEdge(int x, int y, int w) 
	{ 
		edgelist.push_back({ w, x, y }); 
	} 

	vector<vector<int>> kruskalsMST() 
	{ 
		vector<vector<int>> MSTedges;
		sort(edgelist.begin(), edgelist.end());

		DSU s(V);

		int totalWeight = 0; 
		for (auto edge : edgelist) { 
			int w = edge[0]; 
			int x = edge[1]; 
			int y = edge[2]; 

			if (s.find(x) != s.find(y)) { 
				s.unite(x, y); 
				totalWeight += w;
				MSTedges.push_back({ w, x, y }); 
				 
			} 
		}
		mstWeight = totalWeight;
		return MSTedges; 
	}

	void printMST(vector<vector<int>> edges) {
		cout << "[";
		for (auto edge: edges) {
			cout << " [ " << edge[1] <<","<< edge[2] <<","<< edge[0] << " ]" << ",";
		}
		cout << "]" << endl;
	}

	vector<vector<vector<int>>> AllMSTs() {
		vector<vector<int>> MSTedges = kruskalsMST();
		set<vector<int>> mstSet(MSTedges.begin(), MSTedges.end());
		vector<vector<int>> sortededges = edgelist;
		sort(sortededges.begin(), sortededges.end());
		set<vector<int>> sortedSet(sortededges.begin(), sortededges.end());
		set<vector<int>> difference;
		set_difference(sortedSet.begin(), sortedSet.end(), mstSet.begin(), mstSet.end(), inserter(difference, difference.begin()));

    	vector<vector<int>> unusedEdges(difference.begin(), difference.end());

    	int num,numberOfMSTs = 1;
    	for (const auto& path: MSTedges) {
    		num = 1;
    		for (const auto& edge: unusedEdges) {
    			if (path[0] == edge[0]) num += 1;
    		}
    		numberOfMSTs *= num;
    	}
    	
    	vector<vector<vector<int>>> MSTs;
    	MSTs.push_back(MSTedges);

    	for (const auto& path : MSTedges) {
        for (const auto& edge : unusedEdges) {
            if (path[0] == edge[0]) {
          
                vector<vector<int>> newMST = MSTedges;
                auto it = find(newMST.begin(), newMST.end(), path);
                if (it != newMST.end()) {
                    *it = edge;
                    if (isMST(newMST)) {
                    MSTs.push_back(newMST); }
                	}
            	}
        	}
    	}

    	return MSTs;
    }

    bool isMST(vector<vector<int>> mst) {
  
    	DSU d(V);
    	vector<vector<int>> tempMST;
    	int totalW = 0;
    	for (auto edge : mst) { 
			int w = edge[0]; 
			int x = edge[1]; 
			int y = edge[2]; 

			if (d.find(x) != d.find(y)) { 
				d.unite(x, y); 
				totalW += w;
				tempMST.push_back({ w, x, y }); 
				 
			} 
		}

		return totalW == mstWeight;
    }



    void printEdgeAppearance() {
    	vector<vector<vector<int>>> MSTs = AllMSTs();
    	vector<vector<int>> myedges = edgelist;
    	
    	int numofmst = MSTs.size();

    	for (auto edge: myedges) {
    		int count = 0;
    		bool atleastone = false;
    		for (auto mst: MSTs) {
    			for (auto path: mst) {
    				if (path[0] == edge[0] && path[1] == edge[1] && path[2] == edge[2]) {
    					atleastone = true;
    					count += 1;
    				}
    			}
    		}
    		if (!atleastone) cout << "none" << endl;
    		else if (count < numofmst) cout << "at least one" << endl;
    		else cout << "any" << endl;
    	} 
    }
};


int main() 
{ 	
	int n,m,u,v,w;
	cin >> n >> m;
	Graph g(n);

	for (int i = 0; i < m; ++i)
	 {
	 	cin >> u >> v >> w;
	 	g.addEdge(u,v,w);
	 } 

	 g.printEdgeAppearance();

	return 0; 
}
