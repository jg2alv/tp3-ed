#include <iostream>

#include "Celula.hpp"
#include "Lista.hpp"

#include "Celula.cpp"

// Getter de tamanho
template <typename T>
int Lista<T>::GetTamanho() { return tamanho; }

// Construtor padrao
template <typename T>
Lista<T>::Lista() {
    primeiro = new Celula<T>();
    ultimo = primeiro;
    tamanho = 0;
}

// Destrutor padrao
template <typename T>
Lista<T>::~Lista() {
    Limpa();
    delete primeiro;
}

// Posiciona o cursor da lista em uma posicao especifica
template <typename T>
Celula<T>* Lista<T>::Posiciona(int pos, bool antes) {
    Celula<T> *p = primeiro;

    for(int i=1; i<pos; i++){
        p = p->getProx();
    }

    if(!antes) p = p->getProx();

    return p;
}

// Interface publica do posiciona
template <typename T>
T Lista<T>::GetItem(int pos) {
    Celula<T> *p = Posiciona(pos+1);
    return p->getItem();
}

// Setter de um item da lista
template <typename T>
void Lista<T>::SetItem(T item, int pos) {
    Celula<T> *p = Posiciona(pos);
    return p->setItem(item);
}

// Insercao no inicio da lista
template <typename T>
void Lista<T>::InsereInicio(T item)
{
    Celula<T> *nova = new Celula<T>();
    nova->setItem(item);
    nova->setProx(primeiro->getProx());
    primeiro->setProx(nova);

    if(nova->getProx() == nullptr) ultimo = nova;
    tamanho++;
};

// Insercao no final da lista
template <typename T>
void Lista<T>::InsereFinal(T item)
{
    Celula<T> *nova = new Celula<T>();
    nova->setItem(item);
    ultimo->setProx(nova);
    ultimo = nova;
    tamanho++;
};

// Insercao em uma posicao especifica da lista
template <typename T>
void Lista<T>::InserePosicao(T item, int pos) {
    Celula<T>* p = Posiciona(pos, true);
    Celula<T>* nova = new Celula<T>();

    nova->setItem(item);
    nova->setProx(p->getProx());
    p->setProx(nova);

    if(nova->prox == nullptr) ultimo = nova;
    tamanho++;
};

// Remove do inicio da lista
template <typename T>
T Lista<T>::RemoveInicio() {;
    Celula<T> *p = primeiro->getProx();
    primeiro->setProx(p->getProx());

    if(primeiro->prox == nullptr) ultimo = primeiro;
    T aux = p->getItem();

    delete p;
    tamanho--;
    return aux;
}

// Remove do final da lista
template <typename T>
T Lista<T>::RemoveFinal() {
    Celula<T>* p = Posiciona(tamanho, true);
    T aux = ultimo->getItem();

    p->setProx(nullptr);

    delete ultimo;
    tamanho--;

    ultimo = p;
    return aux;
}

// Remove de uma posicao especifica na lista
template <typename T>
T Lista<T>::RemovePosicao(int pos) {
    Celula<T>* p = Posiciona(pos, true);
    Celula<T>* q = p->getProx();
    T aux = q->getItem();
    p->setProx(q->getProx());

    tamanho--;
    delete q;

    if(p->prox == nullptr) ultimo = p;

    return aux;
}

// Limpa a lista: percorre e destroi todo mundo
template <typename T>
void Lista<T>::Limpa() {
    Celula<T> *p = primeiro->getProx();
    while (p!=NULL) {
        primeiro->setProx(p->getProx());
        delete p;
        p = primeiro->getProx();
    }

    ultimo = primeiro;
    tamanho = 0;
};

// Esta vazia sse tamanho eh 0
template <typename T>
bool Lista<T>::Vazia() { return tamanho == 0; }

template <typename T>
void Lista<T>::Imprime() {
    Celula<T>* p = primeiro->getProx();
    while(p != nullptr) {
        std::cout << p->getItem() << " ";
        p = p->getProx();
    }
    std::cout << "\n";
}

// Retorna booleano indicando se aquele
// valor esta presente ou nao na lista
template <typename T>
bool Lista<T>::Contem(T item) {
    for(int i = 0; i < tamanho; i++) {
        if(item == this->GetItem(i)) return true;
    }

    return false;
}

// Overload do operador [] para melhorar legibilidade
template <typename T>
T Lista<T>::operator[](const int pos) { return GetItem(pos); }

// Funcao que realiza a juncao disjunta de duas listas
template <typename T>
void Lista<T>::Juntar(Lista<T>* l) {
    for(int i = 0; i < l->GetTamanho(); i++) {
        T item = l->GetItem(i);
        if(!this->Contem(item)) this->InsereFinal(item);
    }
}

// Funcao que salva os itens da lista encadeada em um vetor no heap
// e retorna o ponteiro para esse vetor
template <typename T>
T* Lista<T>::ToArray() {
    T* array = new T[tamanho];
    for(int i = 0; i < tamanho; i++) array[i] = this->GetItem(i);
    return array;
}