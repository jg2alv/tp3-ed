#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "Passagem.hpp"
#include "Consulta.hpp"
#include "Arvore.hpp"

#include "Lista.cpp"

// Inicio da funcao main
// Recebe o caminho para o arquivo por parametro
int main(int argc, char** argv) {
    // Abertura do arquivo
    std::ifstream arquivo;
    arquivo.open(argv[1]);
    // Estrategia de robustez: para a execucao se o arquivo estiver corrompido
    if(!arquivo.good()) {
        std::cout << "erro na abertura do arquivo" << std::endl;
        arquivo.close();
        return 1;
    }

    int np;
    arquivo >> np;
    Passagem* passagens = new Passagem[np];
    
    // Le as passagens aereas do arquivo
    try {
        Passagem::lerPassagens(np, passagens, arquivo);
    } catch(std::exception& exc) {
        std::cout << exc.what() << std::endl;
        delete[] passagens;
        arquivo.close();
        return 1;
    }

    int nc;
    arquivo >> nc;
    Consulta* consultas = new Consulta[nc];
    
    // Le as consultas do arquivo
    try {
        Consulta::lerConsultas(nc, consultas, arquivo);
    } catch(std::exception& exc) {
        std::cout << exc.what() << std::endl;
        delete[] passagens;
        delete[] consultas;
        arquivo.close();
        return 1;
    }

    int na = 8;
    Arvore* arvores = new Arvore[na];
    
    // Preenche as arvores com base nas passagens aereas
    try {
        Arvore::preencherArvores(np, arvores, passagens);
    } catch(std::exception& exc) {
        std::cout << exc.what() << std::endl;
        delete[] passagens;
        delete[] consultas;
        delete[] arvores;
        arquivo.close();
        return 1;    
    }

    try {
        // Para cada consulta
        for(int i = 0; i < nc; i++) {
            // Encontra as passagens que a satisfazem
            Consulta* c = &consultas[i];
            Lista<Passagem*> lista = c->acharPassagensQueSatisfacam(arvores);
            Passagem** passagens = lista.ToArray();
            int n = lista.GetTamanho(),
                m = c->getNResultados();
    
            // Ordena as passagens que a satisfazem com base no trigrama
            Passagem::setTrigrama(c->getTrigrama());
            Passagem::ordena(passagens, 0, n - 1);
    
            // Imprime so as m primeiras passagens
            std::cout << c->getNResultados() << " " << c->getTrigrama() << " " << c->getExpressao() << std::endl;
            for(int i = 0; i < m; i++) {
                Passagem* p = passagens[i];
                std::cout << p->getOrg() << " "
                          << p->getDst() << " "
                          << p->getPrc() << " "
                          << p->getSea() << " "
                          << p->getDep() << " "
                          << p->getArr() << " "
                          << p->getSto() << std::endl;
            }
    
            // Desalocando a memoria extra
            delete[] passagens;
        }
    } catch(std::exception& exc) {
        std::cout << exc.what() << std::endl;
        delete[] passagens;
        delete[] consultas;
        delete[] arvores;
        arquivo.close();
        return 1;
    }

    // Desalocando a memoria alocada ao longo
    // da execucao do programa
    arquivo.close();
    delete[] arvores;
    delete[] passagens;
    delete[] consultas;
}