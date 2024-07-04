#include <bits/stdc++.h>

using namespace std;

struct Grad{
    int id_grada;
    string naziv_roditelj;
    string naziv_grada;
    int tezina_grada;

    Grad() : id_grada(0), tezina_grada(0){};
    explicit Grad(string& naziv) : naziv_grada(naziv), id_grada(0), tezina_grada(0){};
    Grad(string& naziv, int tezina) : naziv_grada(naziv), id_grada(0), tezina_grada(tezina){};
};

struct Gradovi{
    string naziv_grada;
    vector<Grad> udaljenosti;
};

void read_args(int argc, char* argv[], string& algoritam, string& file_udaljenosti, string& file_heristika);
void read_udaljenosti(const string& file_udaljenosti, string& poc, vector<string>& end, vector<Gradovi>& udaljenosti);
void parse_line(string& line, int iterator_grad, string& naziv_mjesto, vector<string>& end, vector<Gradovi>& udaljenosti);

template<typename T>
void ispis(T varijabla){
    cout<<"Ispis varijable: "<<varijabla<<endl;
}

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

void read_udaljenosti(const string& file_udaljenosti, string& poc, vector<string>& end, vector<Gradovi>& udaljenosti){
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
        }

        it++;
    }
}

void parse_line(string& line, int iterator_grad, string& naziv_mjesto, vector<string>& end, vector<Gradovi>& udaljenosti){
    naziv_mjesto = "";
    Gradovi gradovi;
    auto poz_dvotocka = line.find(':');
    if (poz_dvotocka != string::npos){
        naziv_mjesto = line.substr(0, poz_dvotocka);
        gradovi.naziv_grada = naziv_mjesto;
        if (line.length() > poz_dvotocka + 1){



        }

    }
    else{
        if (iterator_grad == 0){
            naziv_mjesto = line;
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

int main(int argc, char* argv[]) {
    string algoritam;
    string file_udaljenosti;
    string file_heuristika;
    vector<Gradovi> udaljenosti;
    string pocetni_grad;
    vector<string> ciljni_gradovi;

    read_args(argc, argv, algoritam, file_udaljenosti, file_heuristika);
    read_udaljenosti(file_udaljenosti, pocetni_grad, ciljni_gradovi, udaljenosti);


    return 0;
}
