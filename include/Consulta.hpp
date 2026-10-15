#ifndef CONSULTA_HPP
#define CONSULTA_HPP

#include <string>

#include "Passagem.hpp"
#include "Arvore.hpp"
#include "Lista.hpp"

class Clausula {
public:
    bool avaliarClausula(Passagem* passagem);

    static bool (*converteOperador(std::string op))(std::string a, std::string b);

private:
    void acharPassagensQueSatisfacam(Node* node, Lista<Passagem*>* passagens);
    std::string membro;
    std::string op;
    std::string valor;

    friend class Consulta;
};

class Consulta {
public:
    Consulta();
    ~Consulta();
    int getQuantidadeDeClausulas();
    bool avaliarPassagem(Passagem* passagem);
    Lista<Passagem*> acharPassagensQueSatisfacam(Arvore* arvores);
    int getNResultados();
    std::string getTrigrama();
    std::string getExpressao();

    static void lerConsultas(int n, Consulta* consultas, std::ifstream& arquivo);

private:
    void parse();
    int n_resultados;
    int n_clausulas;
    std::string ordem_resultados;
    std::string exp;
    Clausula* clausulas;
};

#endif