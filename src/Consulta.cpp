#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <fstream>

#include "Consulta.hpp"
#include "Passagem.hpp"
#include "Lista.hpp"

#include "Lista.cpp"

// Construtor padrao: define o vetor de clausulas como nulo
// Essa eh a forma de saber se uma consulta ainda nao foi parseada:
// quando seu vetor de clausulas eh nulo
Consulta::Consulta() {
    clausulas = nullptr;
}

// Destrutor apenas desaloca o vetor de clausulas
Consulta::~Consulta() {
    delete[] clausulas;
}

// Calcula a quantidade de clausulas de uma expressao
// conjuntiva com base no numero de &&
// n(clausulas) = 1 + n(&&)
int Consulta::getQuantidadeDeClausulas() {
    int len = exp.length(),
        acc = 1;

    if(len == 0) return 0;
    for(int i = 0; i < len; i++) {
        char curr = exp.at(i);

        if(curr == '&') {
            acc++;
            i++;
        }
    }

    return acc;
}

// Parser para as consultas
void Consulta::parse() {
    if(clausulas != nullptr) return;

    int len = exp.length(),
        idx = 0;
    std::string token = "";
    clausulas = new Clausula[n_clausulas];

    // Percorre a expressao em string de uma consulta
    // pula os parenteses, lida com os operadores
    // quando encontra fechamentode parenteses
    // faz a atribuicao correta (salva a info) e reseta o buffer (token)
    for(int i = 0; i < len; i++) {
        char curr = exp.at(i);
        Clausula* c = &clausulas[idx];

        if(curr == '(') continue;

        if(curr == '>' || curr == '<' || curr == '=') {
            c->membro = token;
            c->op = curr + std::string(1, '=');
            i++;
            token = "";
            continue;
        }

        if(curr == '&') {
            i++;
            continue;
        }

        if(curr == ')') {
            if(token.length() > 0) {
                c->valor = token;
                idx++;
                i++;
                token = "";
            }
            continue;
        }

        token += curr;
    }
}

// Valida uma consulta com base em uma passagem
// Como eh sempre expressoes conjuntivas, so eh true
// se der true em todas as clausulas
bool Consulta::avaliarPassagem(Passagem* passagem) {
    for(int i = 0; i < n_clausulas; i++) {
        Clausula c = clausulas[i];
        bool atende = c.avaliarClausula(passagem);
        if(!atende) return false;
    }

    return true;
}

// Para cada clausula da consulta, acha as passagens que satisfacam a clausula
// salva essas passagens numa lista
// Salva as passagens que satisfazem a consulta (ie, a todas as clausulas ao mesmo tempo)
// em outra lista, lista_de_satisfacao
// Retorna essa lista.
Lista<Passagem*> Consulta::acharPassagensQueSatisfacam(Arvore* arvores) {
    if(clausulas == nullptr) parse();
    Lista<Passagem*> lista_de_candidatas, lista_de_satisfacao;

    for(int i = 0; i < n_clausulas; i++) {
        int idx = -1;
        
        // Se o membro de uma clausula nao eh um dos 8 atributos de passagens aereas,
        // joga uma excecao
        if(clausulas[i].membro == "org") idx = 0;
        else if(clausulas[i].membro == "dst") idx = 1;
        else if(clausulas[i].membro == "prc") idx = 2;
        else if(clausulas[i].membro == "sea") idx = 3;
        else if(clausulas[i].membro == "dep") idx = 4;
        else if(clausulas[i].membro == "arr") idx = 5;
        else if(clausulas[i].membro == "sto") idx = 6;
        else if(clausulas[i].membro == "dur") idx = 7;
        else throw std::runtime_error("acharPassagensQueSatisfacam: nunca deveria chegar aqui");

        clausulas[i].acharPassagensQueSatisfacam(arvores[idx].root, &lista_de_candidatas);
    }

    int tamanho = lista_de_candidatas.GetTamanho();
    for(int i = 0; i < tamanho; i++) {
        Passagem* p = lista_de_candidatas[i];
        bool satisfaz = this->avaliarPassagem(p); 
        if(satisfaz) lista_de_satisfacao.InsereFinal(p);
    }
    
    return lista_de_satisfacao;
}

// Preenche o vetor de consultas com as consultas do arquivo de entrada
void Consulta::lerConsultas(int n, Consulta* consultas, std::ifstream& arquivo) {
    for(int i = 0; i < n; i++) {
        Consulta* c = &consultas[i];

        int n_resultados;
        std::string ordem_resultados, exp;

        arquivo >> n_resultados >> ordem_resultados >> exp;
        c->n_resultados = n_resultados;
        c->ordem_resultados = ordem_resultados;
        c->exp = exp;
        c->n_clausulas = c->getQuantidadeDeClausulas();
    }
}

// Getters para as consultas
int Consulta::getNResultados() { return n_resultados; }
std::string Consulta::getTrigrama() { return ordem_resultados; }
std::string Consulta::getExpressao() { return exp; }

// Avaliacao da clausula
// op eh ponteiro para funcao que pega duas strings e retorna booleano
// A funcao eh uma das funcoes auxiliares estaticas de Passagem
// A funcao compara strings (caso os membros sejam strings)
// ou converte-os para double antes de comparar com std::stod()
// Se o membro de uma clausula nao eh um dos 8 atributos de passagens aereas,
// joga uma excecao
bool Clausula::avaliarClausula(Passagem* passagem) {
    auto op = Clausula::converteOperador(this->op);
    std::string referencia;

    if(membro == "org") referencia = passagem->getOrg();
    else if(membro == "dst") referencia = passagem->getDst();
    else if(membro == "prc") referencia = passagem->getPrc();
    else if(membro == "sea") referencia = passagem->getSea();
    else if(membro == "dep") referencia = passagem->getDep();
    else if(membro == "arr") referencia = passagem->getArr();
    else if(membro == "sto") referencia = passagem->getSto();
    else if(membro == "dur") referencia = passagem->getDur();
    else throw std::runtime_error("avaliarClausula: nao deveria chegar aqui nunca");

    return op(referencia, valor);
}

// Funcao recursiva de busca em AVL para encontrar as passagens que satisfacam a uma clausula
// Percorre a arvore tomando vantagem da representacao de arvore binaria de pesquisa
// Apenas olha a subarvore da esquerda/ direita se necessario
// Isso garante a complexidade logaritmica da funcao
void Clausula::acharPassagensQueSatisfacam(Node* node, Lista<Passagem*>* passagens) {
    if(node == nullptr) return;

    bool node_leq = Passagem::func_leq(node->value, valor),
         node_geq = Passagem::func_geq(node->value, valor),
         node_lt = Passagem::func_lt(node->value, valor),
         node_gt = Passagem::func_gt(node->value, valor),
         node_eq = Passagem::func_eq(node->value, valor);

    if(op == "<=") {
        if(node_leq) passagens->Juntar(&node->passagens);
        acharPassagensQueSatisfacam(node->left, passagens);
        if(node_lt) acharPassagensQueSatisfacam(node->right, passagens);

        return;
    }

    if(op == ">=") {
        if(node_geq) passagens->Juntar(&node->passagens);
        if(node_gt) acharPassagensQueSatisfacam(node->left, passagens);
        acharPassagensQueSatisfacam(node->right, passagens);

        return;
    }

    if(op == "==") {
        if(node_gt) acharPassagensQueSatisfacam(node->left, passagens);
        else if(node_lt) acharPassagensQueSatisfacam(node->right, passagens);
        else passagens->Juntar(&node->passagens);

        return;
    }
}

// Conversao do operador: pega uma string representando a comparacao
// retorna o ponteiro para uma funcao de passagem que realiza essa comparacao
// Se o operador nao for ==, <= ou >=, joga uma excecao
bool (*Clausula::converteOperador(std::string op))(std::string a, std::string b) {
    if(op == "==") return Passagem::func_eq;
    else if(op == "<=") return Passagem::func_leq;
    else if(op == ">=") return Passagem::func_geq;
    else throw std::runtime_error("converteOperador: nao deveria chegar aqui nunca");
}