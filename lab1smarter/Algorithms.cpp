#include "Algorithms.h"
#include "Mapa.h"

Algorithms::Algorithms(int number_of_cities) {
    total_cost = 0;
    found_solution = 0;
    for (int i = 0; i < number_of_cities; i++){
        parents.push_back(-1);
        weights.push_back(0);
        bio.push_back(0);
    }
}

void Algorithms::BFS(int start, const vector<int>& end, vector<vector<pair<int, double>>> connections, const vector<Grad>& cities) {
    open_list.emplace_back(start, 0);
    int reached_end = -1;
    found_solution = 0;

    while(!open_list.empty()){
        int curr_poz = open_list.front().first;
        double curr_value = open_list.front().second;
        open_list.erase(open_list.begin());

        if (bio[curr_poz]) continue;
        bio[curr_poz] = 1;

        for (int i : end){
            if (curr_poz == i){
                reached_end = curr_poz;
                total_cost = curr_value;
                found_solution = 1;
                break;
            }
        }

        if (found_solution) break;

        for (auto & connection : connections[curr_poz]){
            if (bio[connection.first]) continue;
            auto poz_open = find_if(open_list.begin(), open_list.end(), [&connection](pair<int, double> p1){
                return p1.first == connection.first;
            });
            if (poz_open != open_list.end()) continue;
            open_list.emplace_back(connection.first, connection.second + curr_value);
            parents[connection.first] = curr_poz;
        }
    }

    if (found_solution){
        //finding path by tracing parents
        int curr_poz = reached_end;

        do{
            final_path.push_back(curr_poz);
            curr_poz = parents[curr_poz];
        }while(parents[curr_poz] != -1);

        final_path.push_back(start);
    }

}

void Algorithms::UCS(int start, const vector<int>& end, vector<vector<pair<int, double>>> connections, const vector<Grad>& cities) {
    open_list.emplace_back(start, 0);
    int reached_end = -1;
    found_solution = 0;

    while(!open_list.empty()){
        int curr_poz = open_list.front().first;
        double curr_value = open_list.front().second;
        open_list.erase(open_list.begin());

        if (bio[curr_poz]) continue;
        bio[curr_poz] = 1;

        for (int i : end){
            if (curr_poz == i){
                reached_end = curr_poz;
                total_cost = curr_value;
                found_solution = 1;
                break;
            }
        }

        if (found_solution) break;

        for (auto & connection : connections[curr_poz]){
            if (bio[connection.first]) continue;

            auto poz_to_insert = std::lower_bound(open_list.begin(), open_list.end(), connection.second + curr_value,
            [](const pair<int,double>& a, double b) {
                return a.second < b;
            });
            open_list.insert(poz_to_insert, make_pair(connection.first, connection.second + curr_value));
            parents[connection.first] = curr_poz;
        }
    }

    if (found_solution){
        //finding path by tracing parents
        int curr_poz = reached_end;

        do{
            final_path.push_back(curr_poz);
            curr_poz = parents[curr_poz];
        }while(parents[curr_poz] != -1);

        final_path.push_back(start);
    }
}

void Algorithms::ASTAR(int start, vector<int> &end, const vector<vector<pair<int, double>>>& connections, const vector<Grad>& cities) {
    open_list.emplace_back(start, 0);
    int reached_end = -1;
    found_solution = 0;

    while(!open_list.empty()){
        int curr_poz = open_list.front().first;
        double curr_value = open_list.front().second;
        open_list.erase(open_list.begin());

        if (bio[curr_poz]) continue;
        bio[curr_poz] = 1;

        for (int i : end){
            if (curr_poz == i){
                reached_end = curr_poz;
                total_cost = curr_value;
                found_solution = 1;
                break;
            }
        }

        if (found_solution) break;

        for (auto & connection : connections[curr_poz]){
            if (bio[connection.first]) continue;
            open_list.emplace_back(connection.first, connection.second + curr_value);
            parents[connection.first] = curr_poz;
        }

        sort(open_list.begin(), open_list.end(), [&cities](const pair<int, double>& g1, const pair<int, double>& g2){
            if (g1.second + cities[g1.first].getHeuristic() == g2.second + cities[g2.first].getHeuristic()){
                return cities[g1.first].getNaziv() < cities[g2.first].getNaziv();
            }
            return g1.second + cities[g1.first].getHeuristic() < g2.second + cities[g2.first].getHeuristic();
        });
    }

    if (found_solution){
        //finding path by tracing parents
        int curr_poz = reached_end;

        do{
            final_path.push_back(curr_poz);
            curr_poz = parents[curr_poz];
        }while(parents[curr_poz] != -1);

        final_path.push_back(start);
    }
}

void Algorithms::print_solution(const string& algorithm, const string& heuristic_file,const vector<Grad>& cities) const {
    printf("# %s %s\n", algorithm.c_str(), heuristic_file.c_str());
    printf("[FOUND_SOLUTION]: %s\n", (found_solution ? "yes": "no"));
    if (found_solution) {
        int states_visited = 0;
        for (int i : bio) if (i) states_visited++;

        printf("[STATES_VISITED]: %d\n", states_visited);
        printf("[PATH_LENGTH]: %d\n", (int)final_path.size());
        printf("[TOTAL_COST]: %.1f\n", total_cost);
        printf("[PATH]: ");

        for (int i = (int)final_path.size() - 1; i >= 0; i--){
            printf("%s",cities[final_path[i]].getNaziv().c_str());
            (i > 0) ? printf(" => ") : printf("\n");
        }
    }
}


