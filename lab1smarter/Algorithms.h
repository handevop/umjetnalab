#ifndef LAB1SMARTER_ALGORITHMS_H
#define LAB1SMARTER_ALGORITHMS_H

#include <bits/stdc++.h>
#include "Mapa.h"

using namespace std;

class Algorithms {
private:
    vector<pair<int,int>> open_list;
    vector<pair<int,int>> closed_list;
    vector<int> final_path;
    vector<int> parents;
    vector<int> weights;
    vector<int> bio;
    vector<pair<string, string>> solution_lines;
    double total_cost;
    int found_solution;

public:
    explicit Algorithms(int number_of_cities);
    void BFS(int start, const vector<int> &end, vector<vector<pair<int, double>>> connections);
    void UCS(int start, const vector<int> &end, vector<vector<pair<int, double>>> connections);
    void ASTAR(int start, const vector<int> &end, const vector<vector<pair<int, double>>> &connections, const vector<Grad>& cities);
    void CC(const string& file_heuristic ,const vector<vector<pair<int, double>>>& connections, const vector<Grad>& cities);
    void CO(const string& file_heuristic, const vector<int>& end, const vector<vector<pair<int, double>>>& connections, vector<Grad>& cities);

    void print_solution(const string& algorithm, const string& heuristic_file, const vector<Grad>& cities) const;
};


#endif //LAB1SMARTER_ALGORITHMS_H
