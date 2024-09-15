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

    bool operator<(const Grad& rhs) const
    {
        return naziv_grada < rhs.naziv_grada;
    }

    bool operator==(const Grad& rhs) const
    {
        return naziv_grada == rhs.naziv_grada;
    }
};

struct Gradovi{
    string naziv_grada;
    vector<Grad> udaljenosti;
};



void read_args(int argc, char* argv[], bool& algoritmi, string& algoritam, string& file_udaljenosti, string& file_heristika, bool& check_optimistic, bool& check_consistent);
void read_udaljenosti(const string& file_udaljenosti, string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada);
void read_heuristic(const string& file_heuristika, map<string, double>& heuristika);
void parse_heuristic(string& line, map<string, double>& heuristika);
void parse_line(string& line, int iterator_grad, string& naziv_mjesta, vector<string>& end, vector<Gradovi>& udaljenosti);
void print_udaljenosti(vector<Gradovi>& udaljenosti);
void bfs(string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada,
         bool& found_solution, int& states_visited, int& path_length, double& total_cost, deque<string>& path);
void ucs(string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada,
         bool& found_solution, int& states_visited, int& path_length, double& total_cost, deque<string>& path);
void astar(string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada, map<string, double> &heuristika,
           bool& found_solution, int& states_visited, int& path_length, double& total_cost, deque<string>& path);
void print_solution(bool& found_solution, int& states_visited, int& path_length, double& total_cost, deque<string>& path);
void dijkstra(string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada, map<string, int>& prave_udaljenosti);



void read_args(int argc, char* argv[], string& algoritam, string& file_udaljenosti, string& file_heristika, bool& check_optimistic, bool& check_consistent){
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
        else if (string(argv[i]) == "--check-optimistic"){
            check_optimistic = true;
        }
        else if (string(argv[i]) == "--check-consistent"){
            check_consistent = true;
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
        auto poz_char = line.find_first_not_of(' ');
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

void read_heuristic(const string& file_heuristika, map<string, double>& heuristika){
    fstream file(file_heuristika);

    if (!file.is_open()){
        cerr << "Error opening the file!" << endl;
        return;
    }

    string line;
    int it = 0;

    while(getline(file, line)){
        if (line.empty()) continue;
        auto poz_char = line.find_first_not_of(' ');
        if (poz_char != string::npos) {
            if (line[poz_char] == '#'){
                continue;
            }
        }

        parse_heuristic(line, heuristika);

        it++;
    }
}

void parse_heuristic(string& line, map<string, double>& heuristika){
    auto poz_dvotocka = line.find(':');

    if (poz_dvotocka == string::npos) return;

    string grad = line.substr(0, poz_dvotocka);
    double vrijednost = stod(line.substr(poz_dvotocka + 2));

    heuristika[grad] = vrijednost;
}

void parse_line(string& line, int iterator_grad, string& naziv_mjesta, vector<string>& end, vector<Gradovi>& udaljenosti){
    Gradovi gradovi;
    auto poz_dvotocka = line.find(':');
    if (poz_dvotocka != string::npos && iterator_grad >= 2){
        naziv_mjesta = line.substr(0, poz_dvotocka);
        gradovi.naziv_grada = naziv_mjesta;
        if (line.length() > poz_dvotocka + 2){
            line = line.substr(poz_dvotocka + 2);

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
        sort(gradovi.udaljenosti.begin(), gradovi.udaljenosti.end(), [](const Grad& g1, const Grad& g2){
           return g1.naziv_grada < g2.naziv_grada;
        });
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
         bool& found_solution, int& states_visited, int& path_length, double& total_cost, deque<string>& path){
    deque<Grad> otvorena_lista;
    deque<Grad> zatvorena_lista;

    Grad tmp_grad(poc);
    tmp_grad.id_roditelja = -1; // korijenu postavljamo da je roditelj -1
    otvorena_lista.push_back(tmp_grad);

    while(!otvorena_lista.empty()){
        Grad trenutni_grad = otvorena_lista.front();

        //cout<<trenutni_grad.naziv_grada<<"\n";
        bool bio_f = false;

        for (auto & j : zatvorena_lista){
            if(j.naziv_grada == trenutni_grad.naziv_grada){
                bio_f = true;
                otvorena_lista.pop_front();
                break;
            }
        }

        if (bio_f) continue;

        for (auto & i : end){
            if ((string)(i) == (string)(trenutni_grad.naziv_grada)){
                found_solution = true;
                states_visited = (int)zatvorena_lista.size() + 1;
                total_cost = trenutni_grad.tezina_grada;

                Grad tr_put = trenutni_grad;
                while(tr_put.id_roditelja != -1){
                    path.push_front(tr_put.naziv_grada);
                    int sljedeci = tr_put.id_roditelja;
                    tr_put = zatvorena_lista[sljedeci];
                }
                path.push_front(tr_put.naziv_grada);
                path_length = (int)path.size();

                break;
            }
        }

        if (found_solution) break;

        zatvorena_lista.push_back(otvorena_lista.front());
        otvorena_lista.pop_front();

        int it_trenutnog_grada = it_grada[trenutni_grad.naziv_grada];
        for (auto & i : udaljenosti[it_trenutnog_grada].udaljenosti){
            Grad tmp;
            tmp.naziv_grada = i.naziv_grada;
            tmp.tezina_grada = trenutni_grad.tezina_grada + i.tezina_grada;
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

    }
}

void ucs(string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada,
         bool& found_solution, int& states_visited, int& path_length, double& total_cost, deque<string>& path){
    deque<Grad> otvorena_lista;
    deque<Grad> zatvorena_lista;

    Grad tmp_grad(poc);
    tmp_grad.id_roditelja = -1; // korijenu postavljamo da je roditelj -1
    otvorena_lista.push_back(tmp_grad);

    while(!otvorena_lista.empty()){
        Grad trenutni_grad = otvorena_lista.front();

        bool bio = false;

        for (auto & j : zatvorena_lista){
            if(j.naziv_grada == trenutni_grad.naziv_grada){
                bio = true;
//                if (trenutni_grad.tezina_grada < j.tezina_grada){
//                    j = trenutni_grad;
//                    //cout<<"Izmjena gradova jer je manja udaljenost\n";
//                }
            }
        }

        if (bio){
            otvorena_lista.pop_front();
            continue;
        }

        //<<"Trenutni grad: "<<trenutni_grad.naziv_grada<<" "<<trenutni_grad.tezina_grada<<" roditelj: "<<trenutni_grad.naziv_roditelj<<"\n";
        for (auto & i : end){
            if ((string)(i) == (string)(trenutni_grad.naziv_grada)){
                found_solution = true;
                states_visited = (int)zatvorena_lista.size() + 1;
                total_cost = trenutni_grad.tezina_grada;

                Grad tr_put = trenutni_grad;
                while(tr_put.id_roditelja != -1){
                    path.push_front(tr_put.naziv_grada);
                    int sljedeci = tr_put.id_roditelja;
                    tr_put = zatvorena_lista[sljedeci];
                }
                path.push_front(tr_put.naziv_grada);
                path_length = (int)path.size();

                break;
            }
        }

        if (found_solution) break;

        zatvorena_lista.push_back(otvorena_lista.front());
        otvorena_lista.pop_front();

        int it_trenutnog_grada = it_grada[trenutni_grad.naziv_grada];
        for (auto & i : udaljenosti[it_trenutnog_grada].udaljenosti){
            Grad tmp;
            tmp.naziv_grada = i.naziv_grada;
            tmp.tezina_grada = trenutni_grad.tezina_grada + i.tezina_grada;
            tmp.naziv_roditelj = trenutni_grad.naziv_grada;
            tmp.id_roditelja = (int)(zatvorena_lista.size()) - 1;

            otvorena_lista.push_back(tmp);
        }

        sort(otvorena_lista.begin(), otvorena_lista.end(), [](const Grad& g1, const Grad& g2){
           return (g1.tezina_grada == g2.tezina_grada ? g1.naziv_grada < g2.naziv_grada: g1.tezina_grada < g2.tezina_grada);
        });

    }
}

void astar(string& poc, vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada, map<string, double> &heuristika,
         bool& found_solution, int& states_visited, int& path_length, double& total_cost, deque<string>& path){
    deque<Grad> otvorena_lista;
    deque<Grad> zatvorena_lista;

    Grad tmp_grad(poc);
    tmp_grad.id_roditelja = -1; // korijenu postavljamo da je roditelj -1
    otvorena_lista.push_back(tmp_grad);

    while(!otvorena_lista.empty()){
        Grad trenutni_grad = otvorena_lista.front();

        //<<"Trenutni grad: "<<trenutni_grad.naziv_grada<<" "<<trenutni_grad.tezina_grada<<" roditelj: "<<trenutni_grad.naziv_roditelj<<"\n";
        for (auto & i : end){
            if ((string)(i) == (string)(trenutni_grad.naziv_grada)){
                found_solution = true;
                states_visited = (int)zatvorena_lista.size() + 1;
                total_cost = trenutni_grad.tezina_grada;

                Grad tr_put = trenutni_grad;
                while(tr_put.id_roditelja != -1){
                    path.push_front(tr_put.naziv_grada);
                    int sljedeci = tr_put.id_roditelja;
                    tr_put = zatvorena_lista[sljedeci];
                }
                path.push_front(tr_put.naziv_grada);
                path_length = (int)path.size();

                break;
            }
        }

        if (found_solution) break;

        zatvorena_lista.push_back(otvorena_lista.front());
        otvorena_lista.pop_front();

        int it_trenutnog_grada = it_grada[trenutni_grad.naziv_grada];
        for (auto & i : udaljenosti[it_trenutnog_grada].udaljenosti){
            Grad tmp;
            tmp.naziv_grada = i.naziv_grada;
            tmp.tezina_grada = trenutni_grad.tezina_grada + i.tezina_grada;
            tmp.naziv_roditelj = trenutni_grad.naziv_grada;
            tmp.id_roditelja = (int)(zatvorena_lista.size()) - 1;

            auto it_zatvorena = std::find(zatvorena_lista.begin(), zatvorena_lista.end(), tmp);
            auto it_otvorena = std::find(otvorena_lista.begin(), otvorena_lista.end(), tmp);

            if (it_zatvorena != zatvorena_lista.end()){
                if (it_zatvorena->tezina_grada < tmp.tezina_grada ){
                    continue;
                }
                else{
                    zatvorena_lista.erase(it_zatvorena);
                }
            }

            if (it_otvorena != otvorena_lista.end()){
                if (it_otvorena->tezina_grada < tmp.tezina_grada){
                    continue;
                }
                else{
                    otvorena_lista.erase(it_otvorena);
                }
            }

            otvorena_lista.push_back(tmp);
        }

        sort(otvorena_lista.begin(), otvorena_lista.end(), [&heuristika](const Grad& g1, const Grad& g2){
            if (g1.tezina_grada + heuristika[g1.naziv_grada] == g2.tezina_grada + heuristika[g2.naziv_grada]){
                return g1.naziv_grada < g2.naziv_grada;
            }
            return g1.tezina_grada + heuristika[g1.naziv_grada] < g2.tezina_grada + heuristika[g2.naziv_grada];
        });

    }
}

void print_solution(bool& found_solution, int& states_visited, int& path_length, double& total_cost, deque<string>& path){
    cout<<"[FOUND_SOLUTION]: "<<(found_solution? "yes": "no")<<"\n";
    cout<<"[STATES_VISITED]: "<< states_visited<<"\n";
    cout<<"[PATH_LENGTH]: "<<path_length<<"\n";
    printf("[TOTAL_COST]: %.1f\n", total_cost);
    cout<<"[PATH]: ";
    for (int i = 0; i < path.size(); i++){
        cout<<path[i];
        if (i != path.size() - 1) cout<<" => ";
    }
}

void dijkstra(vector<string>& end, vector<Gradovi>& udaljenosti, map<string, int>& it_grada, map<string, double>& prave_udaljenosti){

    for (int t = 0; t < udaljenosti.size(); t++) {
        deque<Grad> otvorena_lista;
        deque<Grad> zatvorena_lista;

        string poc = udaljenosti[t].naziv_grada;
        bool nemoze = false;
        for (auto &j: zatvorena_lista) {
            if (j.naziv_grada == poc) {
                nemoze = true;
                break;
            }
        }

        if (nemoze) continue;

        Grad tmp_grad(poc);
        tmp_grad.id_roditelja = -1; // korijenu postavljamo da je roditelj -1
        otvorena_lista.push_back(tmp_grad);

        while (!otvorena_lista.empty()) {
            Grad trenutni_grad = otvorena_lista.front();

            bool bio_f = false;

            for (auto &j: zatvorena_lista) {
                if (j.naziv_grada == trenutni_grad.naziv_grada) {
                    bio_f = true;
                    otvorena_lista.pop_front();
                    break;
                }
            }

            if (bio_f) continue;
            bool found_solution = false;

            for (auto & i : end){
                if ((string)(i) == (string)(trenutni_grad.naziv_grada)){
                    prave_udaljenosti[poc] = trenutni_grad.tezina_grada;
                    found_solution = true;
                    break;
                }
            }

            if (found_solution) break;

            zatvorena_lista.push_back(otvorena_lista.front());
            otvorena_lista.pop_front();

            int it_trenutnog_grada = it_grada[trenutni_grad.naziv_grada];
            for (auto &i: udaljenosti[it_trenutnog_grada].udaljenosti) {
                Grad tmp;
                tmp.naziv_grada = i.naziv_grada;
                tmp.tezina_grada = trenutni_grad.tezina_grada + i.tezina_grada;
                tmp.naziv_roditelj = trenutni_grad.naziv_grada;
                tmp.id_roditelja = (int) (zatvorena_lista.size()) - 1;

                otvorena_lista.push_back(tmp);
            }

            sort(otvorena_lista.begin(), otvorena_lista.end(), [](const Grad& g1, const Grad& g2){
                return g1.tezina_grada < g2.tezina_grada;
            });
        }
    }

}

void check_if_optimistic(){

}

void check_if_consistent(){

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
    deque<string> path;
    map<string, double> heuristika;
    bool check_optimistic = false;
    string file_optimistic;
    bool check_consistent = false;
    string file_consistent;
    map<string, double> ukupne_udaljenosti;

    read_args(argc, argv, algoritam, file_udaljenosti, file_heuristika, check_optimistic, check_consistent);
    read_udaljenosti(file_udaljenosti, pocetni_grad, ciljni_gradovi, udaljenosti, it_grada);
    //print_udaljenosti(udaljenosti);
    if (algoritam == "bfs") {
        bfs(pocetni_grad, ciljni_gradovi, udaljenosti, it_grada,
            found_solution, states_visited, path_length, total_cost, path);
        cout<<"# BFS\n";
        print_solution(found_solution, states_visited, path_length, total_cost, path);
    }
    else if (algoritam == "ucs"){
        ucs(pocetni_grad, ciljni_gradovi, udaljenosti, it_grada,
            found_solution, states_visited, path_length, total_cost, path);
        cout<<"# UCS\n";
        print_solution(found_solution, states_visited, path_length, total_cost, path);
    }
    else if (algoritam == "astar"){
        read_heuristic(file_heuristika, heuristika);

        astar(pocetni_grad, ciljni_gradovi, udaljenosti, it_grada, heuristika,
              found_solution, states_visited, path_length, total_cost, path);

        if (check_optimistic || check_consistent) dijkstra(ciljni_gradovi, udaljenosti, it_grada, ukupne_udaljenosti);

        if (check_optimistic){
            check_if_optimistic();
            bool postojalo = false;
            cout<<"# HEURISTIC-OPTIMISTIC "<<file_heuristika<<"\n";
            for (auto & i : udaljenosti){
                bool opt = false;
                if (heuristika[i.naziv_grada] <= ukupne_udaljenosti[i.naziv_grada]){
                    opt = true;
                    postojalo = true;
                }
                printf("[CONDITION]: [%s] h(%s) <= h(s) + c: %.1f <= %.1f\n",(opt ? "OK" : "ERR") , i.naziv_grada.c_str(), heuristika[i.naziv_grada], ukupne_udaljenosti[i.naziv_grada]);
            }

            cout<<"[CONCLUSION]: Heuristic "<<(postojalo ? "is not": "is")<<" optimistic.";
        }

        if (check_consistent){
            check_if_consistent();
            bool postojalo = false;
            cout<<"# HEURISTIC-CONSISTENT "<<file_heuristika<<"\n";
            for (auto & i : udaljenosti){
                bool opt = false;

                for (auto & j : i.udaljenosti) {
                    opt = false;
                    if (heuristika[i.naziv_grada] <= heuristika[j.naziv_grada] + j.tezina_grada) {
                        opt = true;
                        postojalo = true;
                    }
                    printf("[CONDITION]: [%s] h(%s) <= h(%s) + c: %.1f <= %.1f + %.1f\n",
                           (opt ? "OK" : "ERR"), i.naziv_grada.c_str(), j.naziv_grada.c_str(),
                           heuristika[i.naziv_grada], heuristika[j.naziv_grada], j.tezina_grada);
                }
            }

            cout<<"[CONCLUSION]: Heuristic "<<(postojalo ? "is not": "is")<<" consistent.";
        }

        if (!check_optimistic && !check_consistent) {
            cout << "# A-STAR " << file_heuristika << "\n";
            print_solution(found_solution, states_visited, path_length, total_cost, path);
        }
    }





    return 0;
}
