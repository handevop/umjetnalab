#ifndef LAB1SMARTER_MAPA_H
#define LAB1SMARTER_MAPA_H
#include <bits/stdc++.h>
#include "Grad.h"

class Mapa {
private:
    int start_city;
    std::vector<int> end_cities;
    std::vector<Grad> cities;
    std::vector< std::vector< std::pair<int, double>>> connections;
    std::map<std::string, int> city_id;

public:
    Mapa() : start_city(0){};

    [[maybe_unused]] explicit Mapa(std::vector<Grad> &city) : start_city(0), cities(city){};

    //static helper functions
    [[nodiscard]] auto find_city_by_name(std::string name) const;
    static void parse_line(const std::string &line, const std::string& delimiter, std::string& before, std::string& after);

    //print functions
    void print_cities() const;
    void print_connections() const;
    void print_start_city() const;
    void print_end_cities() const;

    //do
    void load_city_names(const std::string& file_name);
    void load_heuristic(const std::string& file_name);
    void load_connections(const std::string& file_name);

    //getters and setters
    [[nodiscard]] int get_start() const;
    std::vector<int> get_end();

    const std::vector<Grad> &getCities();
    std::vector<std::vector<std::pair<int, double>>> getConnections();
    int get_number_of_cities();
};


#endif
