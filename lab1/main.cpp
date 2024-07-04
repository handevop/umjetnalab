#include <bits/stdc++.h>

using namespace std;

struct Grad{
    int id_roditelja;
    string naziv_roditelj;
    string naziv_grada;
    double tezina_grada;

    Grad() : id_roditelja(0), tezina_grada(0){};
    explicit Grad(string& naziv) : naziv_grada(naziv), id_roditelja(0), tezina_grada(0){};
    Grad(string& naziv, double tezina) : naziv_grada(naziv), id_roditelja(0), tezina_grada(tezina){};
};

struct Gradovi{
    string naziv_grada;
    vector<Grad> udaljenosti;
};

void read_args(int argc, char* argv[], string& algoritam, string& file_udaljenosti, string& file_heristika);
void read_udaljenosti(const string& file_udaljenosti, string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada);
void parse_line(string& line, int iterator_grad, string& naziv_mjesta, vector<string>& end, vector<Gradovi>& udaljenosti);
void print_udaljenosti(vector<Gradovi>& udaljenosti);
void bfs(string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada,
         bool& found_solution, int& states_visited, int& path_length, double& total_cost, vector<string>& path);

void read_args(int argc, char* argv[], string& algoritam, string& file_udaljenosti, string& file_heristika){
    algoritam = "";
    file_udaljenosti = "";
    file_heristika = "";
    for (int i = 1; i < argc; i += 2){
        if (string(argv[i]) == "--alg"){
            algoritam = string(argv[i + 1]);
        }
        else if (string(argv[i]) == "--ss"){
            file_udaljenosti = string(argv[i + 1]);
        }
        else if (string(argv[i]) == "--h"){
            file_heristika = string(argv[i + 1]);
        }
    }
}

void read_udaljenosti(const string& file_udaljenosti, string& poc, vector<string>& end,
                      vector<Gradovi>& udaljenosti, map<string, int>& it_grada){
    fstream file(file_udaljenosti);

    if (!file.is_open()){
        cerr << "Error opening the file!" << endl;
        return;
    }

    string line;
    string naziv_mjesta;
    int it = 0;

    while(getline(file, line)){
        if (line.empty()) continue;
        auto poz_char = line.find_last_not_of(' ');
        if (poz_char != string::npos) {
            if (line[poz_char] == '#'){
                continue;
            }
        }

        if (it < 2){
            parse_line(line, it, poc, end, udaljenosti);
        }
        else{
            parse_line(line, it, naziv_mjesta, end, udaljenosti);
            it_grada[naziv_mjesta] = it - 2;
        }

        it++;
    }
}

void parse_line(string& line, int iterator_grad, string& naziv_mjesta, vector<string>& end, vector<Gradovi>& udaljenosti){
    Gradovi gradovi;
    auto poz_dvotocka = line.find(':');
    if (poz_dvotocka != string::npos && iterator_grad >= 2){
        naziv_mjesta = line.substr(0, poz_dvotocka);
        gradovi.naziv_grada = naziv_mjesta;
        if (line.length() > poz_dvotocka + 1){
            line = line.substr(poz_dvotocka + 1);

            while(true){
                Grad grad;
                auto poz_zarez = line.find(',');
                if (poz_zarez != string::npos){
                    grad.naziv_grada = line.substr(0, poz_zarez);
                    line = line.substr(poz_zarez + 1);

                    auto poz_razmak = line.find(' ');
                    if (poz_razmak != string::npos){
                        grad.tezina_grada = stod(line.substr(0, poz_razmak));
                        gradovi.udaljenosti.push_back(grad);

                        if (line.length() > poz_razmak + 1){
                            line = line.substr(poz_razmak + 1);
                        }
                        else{
                            break;
                        }
                    }
                    else{
                        grad.tezina_grada = stod(line);
                        gradovi.udaljenosti.push_back(grad);
                        line = "";
                        break;
                    }
                }
            }
        }

        udaljenosti.push_back(gradovi);
    }
    else{
        if (iterator_grad == 0){
            naziv_mjesta = string(line);
            //cout<<endl;
        }
        else if (iterator_grad == 1){
            while(true){
                auto poz_razmak = line.find(' ');
                if (poz_razmak == string::npos){
                    end.push_back(line);
                    break;
                }
                end.push_back(line.substr(0, poz_razmak));

                if (line.length() > poz_razmak + 1){
                    line = line.substr(poz_razmak + 1);
                }
            }
        }
    }


}

void print_udaljenosti(vector<Gradovi>& udaljenosti){
    for (auto & i : udaljenosti){
        cout<<"Jedan zapis: "<<i.naziv_grada<<endl<<"Gradovi djeca: ";
        for (auto & j : i.udaljenosti){
            cout<<j.naziv_grada<<" "<<j.tezina_grada<<" ";
        }
        cout<<endl;
    }
}

void bfs(string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada,
         bool& found_solution, int& states_visited, int& path_length, double& total_cost, vector<string>& path){
    deque<Grad> otvorena_lista;
    vector<Grad> zatvorena_lista;

    Grad tmp_grad(poc);
    otvorena_lista.push_back(tmp_grad);

    while(!otvorena_lista.empty()){
        Grad trenutni_grad = otvorena_lista.front();
        for (const auto & i : end){
            if (i == trenutni_grad.naziv_grada){
                found_solution = true;
                states_visited = (int)zatvorena_lista.size() + 1;
                break;
            }
        }

        zatvorena_lista.push_back(otvorena_lista.front());
        otvorena_lista.pop_front();

        int it_trenutnog_grada = it_grada[trenutni_grad.naziv_grada];
        for (auto & i : udaljenosti[it_trenutnog_grada].udaljenosti){
            Grad tmp;
            tmp.naziv_grada = i.naziv_grada;
            tmp.tezina_grada = tmp.tezina_grada + i.tezina_grada;
            tmp.naziv_roditelj = trenutni_grad.naziv_grada;
            tmp.id_roditelja = (int)(zatvorena_lista.size()) - 1;

            bool bio = false;
            for (auto & j : zatvorena_lista){
                if(j.naziv_grada == tmp.naziv_grada){
                    bio = true;
                    break;
                }
            }

            for (auto & j : otvorena_lista){
                if(j.naziv_grada == tmp.naziv_grada){
                    bio = true;
                    break;
                }
            }

            if (!bio) otvorena_lista.push_back(tmp);
        }

        sort(otvorena_lista.begin(), otvorena_lista.end(), [](const Grad& g1, const Grad& g2){
            return g1.tezina_grada < g2.tezina_grada;
        });

    }
}

int main(int argc, char* argv[]) {
    string algoritam;
    string file_udaljenosti;
    string file_heuristika;
    vector<Gradovi> udaljenosti;
    string pocetni_grad;
    vector<string> ciljni_gradovi;
    map<string, int> it_grada;
    bool found_solution = false;
    int states_visited = 0;
    int path_length = 0;
    double total_cost = 0;
    vector<string> path;

    read_args(argc, argv, algoritam, file_udaljenosti, file_heuristika);
    read_udaljenosti(file_udaljenosti, pocetni_grad, ciljni_gradovi, udaljenosti, it_grada);
    //print_udaljenosti(udaljenosti);
    if (algoritam == "bfs") {
        bfs(pocetni_grad, ciljni_gradovi, udaljenosti, it_grada,
            found_solution, states_visited, path_length, total_cost, path);
        cout<<"# BFS\n";
    }

    cout<<"[FOUND_SOLUTION]: "<<(found_solution? "yes": "no")<<"\n";
    cout<<"[STATES_VISITED]: "<< states_visited<<"\n";


    return 0;
}
