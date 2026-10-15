#ifndef CELULA_HPP
#define CELULA_HPP

#include <string>

template <typename T>
class Celula {
public:
    Celula();
    Celula(T item, Celula<T>* prox);
    T getItem();
    Celula<T>* getProx();
    void setItem(T item);
    void setProx(Celula<T>* prox);

private:
    T item;
    Celula<T> *prox;
};

#endif