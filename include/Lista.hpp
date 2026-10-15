#ifndef LISTA_HPP
#define LISTA_HPP

#include <iostream>

#include "Celula.hpp"

template <typename T>
class Lista {
    public:
        Lista();
        ~Lista();

        int GetTamanho();
        T GetItem(int pos);
        void SetItem(T item, int pos);
        void InsereInicio(T item);
        void InsereFinal(T item);
        void InserePosicao(T item, int pos);
        T RemoveInicio();
        T RemoveFinal();
        T RemovePosicao(int pos);
        void Limpa();
        bool Vazia();
        void Imprime();
        bool Contem(T item);
        void Juntar(Lista<T>* l);
        T* ToArray();

        T operator[] (const int pos);

    private:
        Celula<T>* primeiro;
        Celula<T>* ultimo;
        Celula<T>* Posiciona(int pos, bool antes=false);
        int tamanho;
};

#endif