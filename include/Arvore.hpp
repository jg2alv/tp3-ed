#ifndef ARVORE_HPP
#define ARVORE_HPP

#include "Lista.hpp"
#include "Passagem.hpp"

struct Node {
    std::string value;
    Node* left;
    Node* right;
    int height;
    Lista<Passagem*> passagens;
};

class Arvore {
public:
    Arvore();
    void insere(std::string value, Passagem* p);
    void remove(std::string value);
    Node* find(std::string value);

    static void preencherArvores(int np, Arvore* arvores, Passagem* passagens);
    
    Node* root;

private:
    Node* insere(std::string value, Node* node, Passagem* p);
    Node* remove(std::string value, Node* node);
    Node* find(std::string value, Node* node);
    Node* findMin(Node* node);
    Node* singleRightRotate(Node* node);
    Node* singleLeftRotate(Node* node);
    Node* doubleLeftRotate(Node* node);
    Node* doubleRightRotate(Node* node);
    int height(Node* node);
    int getBalance(Node* node);
};

#endif