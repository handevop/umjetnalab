#include "Mapa.h"
#include <bits/stdc++.h>

using namespace std;

//do functions
auto Mapa::find_city_by_name(string name) const{
    return std::find_if(cities.begin(), cities.end(), [&name](const Grad& grad){
        return grad.getNaziv() == name;
    });
}
void Mapa::load_city_names(const string& file_name) {
    fstream file(file_name);

    if (!file.is_open()){
        cerr << "Error opening the file!" << endl;
        return;
    }

    string line;
    int it = 0;

    while(getline(file, line)){
        if (line.empty()) continue;
        auto first_char_poz = line.find_first_not_of(' ');
        if (first_char_poz != string::npos) {
            if (line[first_char_poz] == '#'){
                continue;
            }
        }

        if (it > 1){
            string city_name, empty;
            parse_line(line, ":", city_name, empty);
            cities.emplace_back(it - 2, city_name);
            city_id[city_name] = it - 2;
        }

        it++;
    }

    file.close();
}
void Mapa::parse_line(const string& line, const string& delimiter, string& before, string& after){
    auto poz_colon = line.find(delimiter);
    if (poz_colon != string::npos){
        before = line.substr(0, poz_colon);
        after = line.substr(poz_colon + delimiter.size());
        return;
    }
    before = line;
    after = "";
}
void Mapa::load_heuristic(const string& file_name) {
    fstream file(file_name);

    if (!file.is_open()){
        cerr << "Error opening the file!" << endl;
        return;
    }

    string line;
    int it = 0;

    while(getline(file, line)){
        if (line.empty()) continue;
        auto first_char_poz = line.find_first_not_of(' ');
        if (first_char_poz != string::npos) {
            if (line[first_char_poz] == '#'){
                continue;
            }
        }

        string city_name, heuristic_value;
        parse_line(line, ": ", city_name, heuristic_value);
        auto poz_city = find_city_by_name(city_name);
        int poz_city_int = (int)(poz_city - cities.begin());

        cities[poz_city_int].set_heuristic_value(stoi(heuristic_value));
        it++;
    }

    file.close();
}
void Mapa::load_connections(const string& file_name) {
    ifstream file(file_name, std::ios::binary | std::ios::ate);
    streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    vector<char> buffer(size);
    if (file.read(buffer.data(), size)){
        int faze = 0; //fazes 0 - start_city, 1 - end_cities, 2 - city_names 3 - skip because comments
        int last_faze = 0;
        string curr_name;
        curr_name = "";
        for (char i : buffer){
            if (curr_name.length() == 0 && i == '#'){
                last_faze = faze;
                faze = 3;
                continue;
            }
            if (i == '\n') {
                if (faze == 0){
                    auto poz_start = find_city_by_name(curr_name);
                    start_city = (int)(poz_start - cities.begin());
                    faze = 1;
                    curr_name = "";
                    continue;
                }
                else if (faze == 1){
                    string line_end_cities = curr_name;

                    while(!line_end_cities.empty()){
                        string end_city;
                        parse_line(line_end_cities, " ", end_city, line_end_cities);
                        auto poz_end = find_city_by_name(end_city);
                        end_cities.push_back((int)(poz_end - cities.begin()));
                    }

                    curr_name = "";
                    faze = 2;
                    continue;
                }
                else if (faze == 2){
                    string city_name, neighbours;
                    parse_line(curr_name, ": ", city_name, neighbours);
                    vector<pair<int, double>> load_neighbours;

                    while (!neighbours.empty()) {
                        string city, value;
                        parse_line(neighbours, ",", city, neighbours);
                        parse_line(neighbours, " ", value, neighbours);

//                        auto poz_city = find_city_by_name(city);
//                        int poz_city_int = (int) (poz_city - cities.begin());

                        int poz_city_int = city_id[city];
                        double value_double = stod(value);

                        load_neighbours.emplace_back(poz_city_int, value_double);
                    }

                    sort(load_neighbours.begin(), load_neighbours.end(),
                         [this](pair<int, double> g1, pair<int, double> g2) {
                             return cities[g1.first].getNaziv() < cities[g2.first].getNaziv();
                         });

                    connections.push_back(load_neighbours);

                    curr_name = "";
                    continue;
                }
                else{ // if # that means line is comment, so it does not go in program
                    faze = last_faze;
                    curr_name = "";
                    continue;
                }
            }
            curr_name += i;
        }
    }

}

//print functions
[[maybe_unused]] void Mapa::print_cities() const{
    for (const auto& grad: cities){
        cout<<grad<<endl;
    }
}

[[maybe_unused]] void Mapa::print_connections() const {
    for (const auto & connection : connections){
        for (int j = 0; j < connection.size(); j++){
            cout<<"pair: ("<<connection[j].first<<", "<<connection[j].second<<")";
            if (j == connection.size() - 1) cout<<"\n";
            else cout<<", ";
        }
    }
}

//getters and setters
const std::vector<Grad> &Mapa::getCities() {
    return cities;
}

std::vector<Grad> &Mapa::getCitiesInstance() {
    return cities;
}

vector<vector<pair<int, double>>> Mapa::getConnections() {
    return connections;
}
int Mapa::get_start() const {
    return start_city;
}
std::vector<int> Mapa::get_end() {
    return end_cities;
}
int Mapa::get_number_of_cities() {
    return (int)cities.size();
}

[[maybe_unused]] void Mapa::print_start_city() const {
    cout<<"Start city id: "<<start_city<<", City description: "<<cities[start_city]<<"\n";
}

[[maybe_unused]] void Mapa::print_end_cities() const {
    for (int end_city : end_cities){
        cout<<"End city id: "<<end_city<<", City description: "<<cities[end_city]<<"\n";
    }
}

