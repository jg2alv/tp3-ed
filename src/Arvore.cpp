#include <iostream>

#include "Arvore.hpp"
#include "Lista.hpp"
#include "Passagem.hpp"

#include "Lista.cpp"

// Construtor: apenas define a raiz como nula
Arvore::Arvore() {
    this->root = nullptr;
}

// Metodo auxiliar para retornar a altura de um no
int Arvore::height(Node* node) {
    if (node == nullptr) {
        return 0;
    } else {
        return node->height;
    }
}

// Calculo do fator de balanceamento de um no
int Arvore::getBalance(Node* node) {
    if (node == nullptr) {
        return 0;
    } else {
        return this->height(node->left) - this->height(node->right);
    }
}

// Rotacao para a direita
Node* Arvore::singleRightRotate(Node* node) {
    if (node == nullptr || node->left == nullptr) {
        return node;
    }

    Node* temp = node->left;
    node->left = temp->right;
    temp->right = node;

    node->height = std::max(this->height(node->left), this->height(node->right)) + 1;
    temp->height = std::max(this->height(temp->left), this->height(temp->right)) + 1;

    return temp;
}

// Rotacao para a esquerda
Node* Arvore::singleLeftRotate(Node* node) {
    if (node == nullptr || node->right == nullptr) {
        return node;
    }

    Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;

    node->height = std::max(this->height(node->left), this->height(node->right)) + 1;
    temp->height = std::max(this->height(temp->left), this->height(temp->right)) + 1;

    return temp;
}

// Rotacao esquerda esquerda
Node* Arvore::doubleLeftRotate(Node* node) {
    node->right = this->singleRightRotate(node->right);
    return this->singleLeftRotate(node);
}

// Rotacao direita direita
Node* Arvore::doubleRightRotate(Node* node) {
    node->left = this->singleLeftRotate(node->left);
    return this->singleRightRotate(node);
}

// Funcao para encontrar um no com uma chave (valor)
// Se nao encontrou, retorna nullptr
Node* Arvore::find(std::string value, Node* node) {
    if(node == nullptr) return nullptr;

    bool valueEhMenor = Passagem::func_lt(value, node->value),
         valueEhMaior = Passagem::func_gt(value, node->value);

    if (valueEhMenor) return this->find(value, node->left);
    else if (valueEhMaior) return this->find(value, node->right);
    else return node;
}

// Funcao de insercao na AVL
// Ja adiciona a passagem na lista de passagens do no
Node* Arvore::insere(std::string value, Node* node, Passagem* p) {
    if (node == nullptr) {
        node = new Node();
        node->value = value;
        node->height = 1;
        node->left = nullptr;
        node->right = nullptr;
        node->passagens.InsereFinal(p);
        return node;
    }

    bool valueEhMenor = Passagem::func_lt(value, node->value),
         valueEhMaior = Passagem::func_gt(value, node->value);

    if (valueEhMenor) node->left = this->insere(value, node->left, p);
    else if (valueEhMaior) node->right = this->insere(value, node->right, p);
    else return node;

    node->height = 1 + std::max(this->height(node->left), this->height(node->right));
    int balance = this->getBalance(node);

    if (balance > 1 && this->getBalance(node->left) >= 0) {
        return this->singleRightRotate(node);
    } else if (balance < -1 && this->getBalance(node->right) <= 0) {
        return this->singleLeftRotate(node);
    } else    if (balance > 1 && this->getBalance(node->left) < 0) {
        return this->doubleRightRotate(node);
    } else if (balance < -1 && this->getBalance(node->right) > 0) {
        return this->doubleLeftRotate(node);
    }

    return node;
}

// Funcao para encontrar um no minimo (no folha da subarvore da esquerda)
Node* Arvore::findMin(Node* node) {
    if (node == nullptr) {
        return nullptr;
    } else if (node->left == nullptr) {
        return node;
    } else {
        return this->findMin(node->left);
    }
}

// Funcao recursiva de remocao da AVL
Node* Arvore::remove(std::string value, Node* node) {
    if (!node) return node;

    bool valueEhMenor = Passagem::func_lt(value, node->value),
         valueEhMaior = Passagem::func_gt(value, node->value);

    if (valueEhMenor)
        node->left = this->remove(value, node->left);
    else if (valueEhMaior)
        node->right = this->remove(value, node->right);
    else {
        if (!node->left && !node->right) {
            delete node;
            node=nullptr;
        } else if (!node->left) {
            Node* temp = node;
            node = node->right;
            delete temp;
        } else if (!node->right) {
            Node* temp = node;
            node = node->left;
            delete temp;
        } else {
            Node* temp = this->findMin(node->right);
            node->value = temp->value;
            node->right = this->remove(temp->value, node->right);
        }
    }

    if (!node) return node;

    node->height = 1 + std::max(this->height(node->left), this->height(node->right));
    int balance = this->getBalance(node);

    if (balance > 1 && this->getBalance(node->left) >= 0) {
        return this->singleRightRotate(node);
    } else if (balance < -1 && this->getBalance(node->right) <= 0) {
        return this->singleLeftRotate(node);
    } else if (balance > 1 && this->getBalance(node->left) < 0) {
        return this->doubleRightRotate(node);
    } else if (balance < -1 && this->getBalance(node->right) > 0) {
        return this->doubleLeftRotate(node);
    }

    return node;
}

// Interface publica do metodo find
Node* Arvore::find(std::string value) {
    return this->find(value, this->root);
}

// Interface publica do metodo de insercao
void Arvore::insere(std::string value, Passagem* p) {
    this->root = this->insere(value, this->root, p);
}

// Interface publica do metodo de remocao
void Arvore::remove(std::string value) {
    this->root = this->remove(value, this->root);
}

// Funcao para preencher os dados nas arvores
// Pega o numero de passagens e os vetores de arvores e de passagens
// Insere, nas arvores adequadas, os nos
// Caso o no ja exista, atualiza a lista de passagens dele
// Uso de ponteiro de funcao para pegar o valor adequado
void Arvore::preencherArvores(int np, Arvore* arvores, Passagem* passagens) {
    std::string (Passagem::* getters[])() = {
        &Passagem::getOrg, &Passagem::getDst,
        &Passagem::getPrc, &Passagem::getSea,
        &Passagem::getDep, &Passagem::getArr,
        &Passagem::getSto, &Passagem::getDur
    };

    for(int i = 0; i < np; i++) {
        Passagem* p = &passagens[i];
        
        for(int j = 0; j < 8; j++) {
            std::string (Passagem::* func)() = getters[j];
            std::string value = (p->*func)();
            Node* node = arvores[j].find(value);

            if(node == nullptr) arvores[j].insere(value, p);
            else node->passagens.InsereFinal(p);
        }
    }
}