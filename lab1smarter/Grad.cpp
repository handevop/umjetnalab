#include "Grad.h"

void Grad::set_heuristic_value(int load_heuristic) {
    heuristic = load_heuristic;
}

const std::string &Grad::getNaziv() const{
    return naziv;
}

long double Grad::getHeuristic() const{
    return heuristic;
}

std::ostream &operator<<(std::ostream &os, const Grad &grad) {
    os << "iterator: " << grad.iterator << " naziv: " << grad.naziv << " iterator_roditelj: " << grad.iterator_roditelj
       << " heuristic: " << grad.heuristic << " real_price: " << grad.real_price << " bio: " << grad.bio;
    return os;
}

double Grad::getRealPrice() const {
    return real_price;
}

void Grad::setRealPrice(const double realPrice) {
    real_price = realPrice;
}
