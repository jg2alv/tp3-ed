#ifndef VOO_HPP
#define VOO_HPP

#include <iostream>
#include <string>

class Passagem {
public:
    std::string getOrg();
    std::string getDst();
    std::string getPrc();
    std::string getSea();
    std::string getDep();
    std::string getArr();
    std::string getSto();
    std::string getDur();

    bool operator== (const Passagem& outra);
    bool operator< (const Passagem& outra);
    bool operator<= (const Passagem& outra);
    bool operator> (const Passagem& outra);
    bool operator>= (const Passagem& outra);

    static void lerPassagens(int n, Passagem* passagens, std::ifstream& arquivo);
    static int calcularDuracaoDoVoo(std::string dep, std::string arr);
    static bool membroEhNumerico(std::string valor);
    static bool func_eq(std::string a, std::string b);
    static bool func_leq(std::string a, std::string b);
    static bool func_geq(std::string a, std::string b);
    static bool func_lt(std::string a, std::string b);
    static bool func_gt(std::string a, std::string b);
    static void ordena(Passagem** array, int Esq, int Dir);
    static void setTrigrama(std::string trigrama);

private:
    std::string org;
    std::string dst;
    std::string prc;
    std::string sea;
    std::string dep;
    std::string arr;
    std::string sto;
    std::string dur;
    static std::string trigrama;
};

#endif