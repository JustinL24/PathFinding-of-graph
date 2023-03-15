#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "ArgumentManager.h"
using namespace std;

int NewIndex(vector<pair<int, float>> edges, int num1, float num2);
void DFS(vector<int>& cu, int start, int end, vector<vector<int>>& edges, vector<vector<int>>& whole,unordered_set<int> &visited);
vector<vector<pair<int, float>>> figure;

int main(int argc, char *argv[]) {
	ArgumentManager am(argc, argv);
	string infilename = am.get("input");
	string pathfilename = am.get("path");
	string outfilename = am.get("output");
	ifstream input(infilename);
	ifstream path(pathfilename);
	ofstream output(outfilename);

	string line;

	int v = 0;
	while (getline(input, line)) {
		if (line == "") {
			continue;
		}
		stringstream stream(line);
		int a, b;
		stream >> a >> b;
		v = max({v, a, b});
	}
	
	input.clear();
    input.seekg(0);
	
	figure = vector<vector<pair<int, float>>>  (v + 1);
	vector<vector<int>> edges (v + 1);
	
	while (getline(input, line)) {
		if (line == "") {
			continue;
		}
		stringstream stream(line);
		int n1, n2;
		float n3;
		stream >> n1 >> n2 >> n3;
		int index = NewIndex(figure[n1], n2, n3);
		if (index != -1) {
			figure[n1][index] = {n2,n3};
		} else {
			figure[n1].push_back({n2,n3});
		}
		edges[n1].push_back(n2);
		
	}
	
	int start, end;
	path >> start >> end;

	vector<int> pway2;
	vector<vector<int>> pway1;
	unordered_set<int> visited;
	DFS(pway2, start, end, edges, pway1, visited);
		
	float longest = -1;
	float shortest = numeric_limits<float>::max();
	for (vector<int> p : pway1) {
		float length = 0;
		for (int i = 0; i < p.size() - 1; i++) {
			int temp = p[i];
			for (int j = 0; j < figure[temp].size(); j++) {
				if (figure[temp][j].first == p[i+1]) {
					length+=figure[temp][j].second;
				}
			}
		}
		longest = max(longest, length);
		shortest = min(shortest, length);
	}
	
	if (shortest == numeric_limits<float>::max()) {
		output << "Infinite" << endl;
	} else {
		output << shortest << endl;
	}
	
	if (longest == -1) {
		output << "Infinite" << endl;
	} else {
		output << longest << endl;
	}
	output << pway1.size();

}

int NewIndex(vector<pair<int, float>> edges, int num1, float num2) {
	int index = -1;
	for (int i = 0; i < edges.size(); i++) {
		if (edges[i].first == num1) {
			return i;
		}
	}
	return index;
}

void DFS(vector<int>& cu, int start, int end, vector<vector<int>>& edges, vector<vector<int>>& whole, unordered_set<int> &visited) {
	if (start >= edges.size() || end >= edges.size()) return;
	visited.insert(start);
	cu.push_back(start);
	
	if (start == end) {
		whole.push_back(cu);
	} else {
		for (int vertex : edges[start]) {
			if (!visited.count(vertex)) {
				DFS(cu, vertex, end, edges, whole, visited);
			}
		}
	}
	visited.erase(start);
	cu.pop_back();
}