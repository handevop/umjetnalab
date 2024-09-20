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

void Algorithms::BFS(int start, const vector<int> &end, vector<vector<pair<int, double>>> connections) {
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

void Algorithms::UCS(int start, const vector<int> &end, vector<vector<pair<int, double>>> connections) {
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

void Algorithms::ASTAR(int start, const vector<int>& end, const vector<vector<pair<int, double>>>& connections, const vector<Grad>& cities) {
    open_list.emplace_back(start, 0);
    int reached_end = -1;
    found_solution = 0;

    while(!open_list.empty()){
        int curr_poz = open_list.front().first;
        double curr_value = open_list.front().second;
        closed_list.emplace_back(open_list.front());
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

            // first find element and if not in open list or closed list then find position to place it
            // else swap it

            auto found_in_open = std::find_if(open_list.begin(), open_list.end(),
            [&connection](pair<int, double> a) {
                return a.first == connection.first;
            });

            auto found_in_closed = std::find_if(closed_list.begin(), closed_list.end(),
            [&connection](pair<int, double> a) {
                return a.first == connection.first;
            });

            if (found_in_closed != closed_list.end()){
                if (closed_list[(int)(found_in_closed - closed_list.begin())].second > connection.second + curr_value) {
                    closed_list.erase(found_in_closed);
                    closed_list.insert(found_in_closed, make_pair(connection.first, connection.second + curr_value));
                }
            }
            else if (found_in_open != open_list.end()){
                if (open_list[(int)(found_in_open - open_list.begin())].second > connection.second + curr_value) {
                    open_list.erase(found_in_open);
                    open_list.insert(found_in_open, make_pair(connection.first, connection.second + curr_value));
                }
            }
            else{
                auto poz_to_insert = std::lower_bound(open_list.begin(), open_list.end(), connection,
                [&cities, curr_value](const pair<int,double>& a, const pair<int,double>& b) {
                    if (a.second + cities[a.first].getHeuristic() == b.second + curr_value + cities[a.first].getHeuristic()){
                        return cities[a.first].getNaziv() < cities[b.first].getNaziv();
                    }

                    return a.second + cities[a.first].getHeuristic() < b.second + curr_value + cities[b.first].getHeuristic();
                });
                open_list.insert(poz_to_insert, make_pair(connection.first, connection.second + curr_value));
            }

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

void Algorithms::CC(const string& file_heuristic ,const vector<vector<pair<int, double>>>& connections, const vector<Grad>& cities){
     //first string represents city names to sort by them, second one represents print line
    string line;
    line = "";
    bool is_consistent = true;
    cout<<"# HEURISTIC-CONSISTENT "<< file_heuristic<<"\n";
    for (int i = 0; i < connections.size(); i++){
        for (int j = 0; j < connections[i].size(); j++){
            line += "[CONDITION]: ";
            if (cities[i].getHeuristic() <= cities[connections[i][j].first].getHeuristic() + connections[i][j].second){
                line += "[OK] ";
            }
            else {
                line += "[ERR] ";
                is_consistent = false;
            }

            line += "h(" +  cities[i].getNaziv() + ") <= h(" + cities[connections[i][j].first].getNaziv() + ") + c: ";

            stringstream ss1, ss2, ss3;
            ss1 << std::fixed << std::setprecision(1) << cities[i].getHeuristic();
            ss2 << std::fixed << std::setprecision(1) << cities[connections[i][j].first].getHeuristic();
            ss3 << std::fixed << std::setprecision(1) << connections[i][j].second;

            line += ss1.str() + " <= " + ss2.str() + " + " + ss3.str() + "\n";

            solution_lines.emplace_back(cities[i].getNaziv() + cities[connections[i][j].first].getNaziv(), line);
            line = "";
        }
    }

    sort(solution_lines.begin(), solution_lines.end());

    for (auto & solution_line : solution_lines){
        cout<<solution_line.second;
    }

    if (is_consistent){
        cout<<"[CONCLUSION]: Heuristic is consistent.";
    }
    else{
        cout<<"[CONCLUSION]: Heuristic is not consistent.";
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

void Algorithms::CO(const string& file_heuristic, const vector<int> &end, const vector<vector<pair<int, double>>>& connections,
                    vector<Grad> &cities) {

    vector<vector<pair<int, double>>> inverted_connections(connections.size());

    for (int i = 0; i < connections.size(); i++){
        for (int j = 0; j < connections[i].size(); j++){
            inverted_connections[connections[i][j].first].emplace_back(i,connections[i][j].second);
        }
    }

    for (int i : end){
        cities[i].setRealPrice(0);
        for (auto & j : inverted_connections[i]){
            open_list.emplace_back(j);
        }
    }

    sort(open_list.begin(), open_list.end(), [](pair<int, double> g1, pair<int, double> g2){
        return g1.second < g2.second;
    });

    while (!open_list.empty()) {
        int curr_poz = open_list.front().first;
        double curr_value = open_list.front().second;
        open_list.erase(open_list.begin());

        for (int i: end) {
            if (curr_poz == i) {
                cities[curr_poz].setRealPrice(0);
                found_solution = 1;
                break;
            }
        }

        if (found_solution){
            found_solution = 0;
            continue;
        }

        if (cities[curr_poz].getRealPrice() > curr_value || cities[curr_poz].getRealPrice() == -1){
            cities[curr_poz].setRealPrice(curr_value);
        }

        if (bio[curr_poz]) {
            continue;
        }
        bio[curr_poz] = 1;


        for (auto &connection: inverted_connections[curr_poz]) {
            if (bio[connection.first]) continue;
            open_list.emplace_back(connection.first, connection.second + curr_value);
        }

        sort(open_list.begin(), open_list.end(), [](pair<int, double> g1, pair<int, double> g2){
            return g1.second < g2.second;
        });
    }


    string line;
    line = "";
    bool is_optimistic = true;
    cout<<"# HEURISTIC-OPTIMISTIC "<< file_heuristic<<"\n";

    for (const Grad & city : cities){
        line += "[CONDITION]: ";
        if (city.getHeuristic() <= city.getRealPrice()){
            line += "[OK] ";
        }
        else {
            line += "[ERR] ";
            is_optimistic = false;
        }

        line += "h(" + city.getNaziv() + ") <= h*: ";

        stringstream ss1, ss2, ss3;
        ss1 << std::fixed << std::setprecision(1) << city.getHeuristic();
        ss2 << std::fixed << std::setprecision(1) << city.getRealPrice();

        line += ss1.str() + " <= " + ss2.str() + "\n";

        solution_lines.emplace_back(city.getNaziv(), line);
        line = "";
    }


    sort(solution_lines.begin(), solution_lines.end());

    for (auto & solution_line : solution_lines){
        cout<<solution_line.second;
    }

    if (is_optimistic){
        cout<<"[CONCLUSION]: Heuristic is optimistic.";
    }
    else{
        cout<<"[CONCLUSION]: Heuristic is not optimistic.";
    }
}

