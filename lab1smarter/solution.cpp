#include <iostream>
#include "Mapa.h"
#include "Algorithms.h"

using namespace std;

void load_args(int argc, char* argv[], string& algorithm, string& file, string& file_heuristic, bool& check_optimistic, bool& check_consistent){
    algorithm = "";
    file = "";
    file_heuristic = "";
    check_optimistic = false;
    check_consistent = false;
    for (int i = 1; i < argc; i += 2){
        if (string(argv[i]) == "--alg"){
            algorithm = string(argv[i + 1]);
        }
        else if (string(argv[i]) == "--ss"){
            file = string(argv[i + 1]);
        }
        else if (string(argv[i]) == "--h"){
            file_heuristic = string(argv[i + 1]);
        }
        else if (string(argv[i]) == "--check-optimistic"){
            check_optimistic = true;
        }
        else if (string(argv[i]) == "--check-consistent"){
            check_consistent = true;
        }
    }
}

int main(int argc, char* argv[]) {
    string algorithm, file, file_heuristic;
    bool check_optimistic, check_consistent;
    load_args(argc, argv, algorithm, file, file_heuristic, check_optimistic, check_consistent);

    Mapa mapa;

    if (algorithm == "bfs"){
        mapa.load_city_names(file);
        mapa.load_connections(file);

        Algorithms bfs = Algorithms(mapa.get_number_of_cities());
        bfs.BFS(mapa.get_start(), mapa.get_end(), mapa.getConnections());
        bfs.print_solution("BFS", "", mapa.getCities());
    }
    else if (algorithm == "ucs"){
        mapa.load_city_names(file);
        mapa.load_connections(file);

        Algorithms ucs = Algorithms(mapa.get_number_of_cities());
        ucs.UCS(mapa.get_start(), mapa.get_end(), mapa.getConnections());
        ucs.print_solution("UCS", "", mapa.getCities());
    }
    else if (algorithm == "astar"){
        mapa.load_city_names(file);
        mapa.load_connections(file);
        mapa.load_heuristic(file_heuristic); // calling with astar only

        Algorithms astar = Algorithms(mapa.get_number_of_cities());
        astar.ASTAR(mapa.get_start(), mapa.get_end(), mapa.getConnections(), mapa.getCities());
        astar.print_solution("ASTAR", file_heuristic, mapa.getCities());
    }
    else if (check_consistent){
        mapa.load_city_names(file);
        mapa.load_connections(file);
        mapa.load_heuristic(file_heuristic); // calling with astar only

        Algorithms cc = Algorithms(mapa.get_number_of_cities());
        cc.CC(file_heuristic, mapa.getConnections(), mapa.getCities());
    }
    else if (check_optimistic){
        mapa.load_city_names(file);
        mapa.load_connections(file);
        mapa.load_heuristic(file_heuristic); // calling with astar only

        Algorithms co = Algorithms(mapa.get_number_of_cities());
        co.CO(file_heuristic, mapa.get_end(), mapa.getConnections(), mapa.getCitiesInstance());
    }
}


