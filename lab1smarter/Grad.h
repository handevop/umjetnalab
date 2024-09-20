#ifndef LAB1SMARTER_GRAD_H
#define LAB1SMARTER_GRAD_H
#include <utility>
#include <vector>
#include <string>
#include <ostream>


class Grad{
private:
    int iterator;
    std::string naziv;
    int iterator_roditelj;
    long int heuristic;
    double real_price;
    bool bio;
public:
    Grad() : iterator(-1), naziv("-1"), iterator_roditelj(-1), heuristic(-1), real_price(-1), bio(false){};
    Grad(int it, std::string naz) : iterator(it), naziv(std::move(naz)),
         iterator_roditelj(-1), heuristic(-1), real_price(-1), bio(false){};

    //getters and setters
    void set_heuristic_value(int load_heuristic);

    [[nodiscard]] const std::string &getNaziv() const;
    [[nodiscard]] long double getHeuristic() const;

    [[nodiscard]] double getRealPrice() const;

    void setRealPrice(double realPrice);

    friend std::ostream &operator<<(std::ostream &os, const Grad &grad);
};


#endif //LAB1SMARTER_GRAD_H
