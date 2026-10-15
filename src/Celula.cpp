#include <iostream>
#include <string>

#include "Celula.hpp"

// Construtor padrao: define o prox como nulltpr
template <typename T>
Celula<T>::Celula() : prox(nullptr) {}

// Construtor auxiliar: ja define o prox e o item
template <typename T>
Celula<T>::Celula(T item, Celula<T>* prox) : item(item), prox(prox) {}

// Getter para o item
template <typename T>
T Celula<T>::getItem() { return item; }

// Getter para o prox
template <typename T>
Celula<T>* Celula<T>::getProx() { return prox; }

// Setter para o item
template <typename T>
void Celula<T>::setItem(T item) { this->item = item; }

// Setter para o prox
template <typename T>
void Celula<T>::setProx(Celula<T>* prox) { this->prox = prox; }