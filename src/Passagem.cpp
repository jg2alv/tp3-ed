#include <iostream>
#include <ctime>
#include <fstream>
#include <string>

#include "Passagem.hpp"

// Settando o trigrama para vazio
// Eh usado no metodo de ordenacao para saber em base em que deve-se ordenar
std::string Passagem::trigrama = "";

// Getters da passagem
std::string Passagem::getOrg() { return org; }
std::string Passagem::getDst() { return dst; }
std::string Passagem::getPrc() { return prc; }
std::string Passagem::getSea() { return sea; }
std::string Passagem::getDep() { return dep; }
std::string Passagem::getArr() { return arr; }
std::string Passagem::getSto() { return sto; }
std::string Passagem::getDur() { return dur; }

// Duas passagens sao iguais sse todos seus campos batem
bool Passagem::operator== (const Passagem& outra) {
    return this->org == outra.org &&
           this->dst == outra.dst &&
           this->prc == outra.prc &&
           this->sea == outra.sea &&
           this->dep == outra.dep &&
           this->arr == outra.arr &&
           this->sto == outra.sto &&
           this->dur == outra.dur;
}

// Uso do trigrama para determinar precedencia entre duas passagens
// Joga excecoes se o trigrama for invalido (ie, outros caracteres que nao p,d,s)
bool Passagem::operator< (const Passagem& outra) {
    bool eq;
    std::string Passagem::*campo;
    std::string trigrama = Passagem::trigrama;

    if(trigrama[0] == 'p') campo = &Passagem::prc;
    else if(trigrama[0] == 'd') campo = &Passagem::dur;
    else if(trigrama[0] == 's') campo = &Passagem::sto;
    else throw std::runtime_error("trigrama invalido");

    eq = Passagem::func_eq(this->*campo, outra.*campo);
    if(!eq) return Passagem::func_lt(this->*campo, outra.*campo);

    if(trigrama[1] == 'p') campo = &Passagem::prc;
    else if(trigrama[1] == 'd') campo = &Passagem::dur;
    else if(trigrama[1] == 's') campo = &Passagem::sto;
    else throw std::runtime_error("trigrama invalido");

    eq = Passagem::func_eq(this->*campo, outra.*campo);
    if(!eq) return Passagem::func_lt(this->*campo, outra.*campo);

    if(trigrama[2] == 'p') campo = &Passagem::prc;
    else if(trigrama[2] == 'd') campo = &Passagem::dur;
    else if(trigrama[2] == 's') campo = &Passagem::sto;
    else throw std::runtime_error("trigrama invalido");

    return Passagem::func_lt(this->*campo, outra.*campo);
}

// Overload do operador de <= com base no < e ==
bool Passagem::operator<= (const Passagem& outra) {
    return (*this < outra) || (*this == outra);
}

// Overload do operador de > com base na negacao do resultado de <=
bool Passagem::operator> (const Passagem& outra) {
    return !(*this <= outra);
}

// Overload do operador de >= com base na negacao do resultado de <
bool Passagem::operator>= (const Passagem& outra) {
    return !(*this < outra);
}

// Funcao que calcula a duracao de um voo
// Extrai a data e hora das informacoes de chegada e de saida
// Usa unix time para calcular a duracao em segundos
int Passagem::calcularDuracaoDoVoo(std::string dep, std::string arr) {
    int yeardep, mondep, daydep, hrdep, mindep, secdep,
        yeararr, monarr, dayarr, hrarr, minarr, secarr;

    sscanf(dep.c_str(), "%d-%d-%dT%d:%d:%d",
    &yeardep, &mondep, &daydep, &hrdep, &mindep, &secdep);

    sscanf(arr.c_str(), "%d-%d-%dT%d:%d:%d",
    &yeararr, &monarr, &dayarr, &hrarr, &minarr, &secarr);

    std::tm tmdep = { 0, mindep, hrdep, daydep, mondep - 1, yeardep - 1900 };
    std::tm tmarr = { 0, minarr, hrarr, dayarr, monarr - 1, yeararr - 1900 };
    std::time_t tsdep = std::mktime(&tmdep),
                tsarr = std::mktime(&tmarr);

    return tsarr - tsdep;
}

// Funcao que le as passagens do arquivo e salva no vetor de passagens
void Passagem::lerPassagens(int n, Passagem* passagens, std::ifstream& arquivo) {
    for(int i = 0; i < n; i++) {
        Passagem* p = &passagens[i];
        std::string org, dst, prc, sea, dep, arr, sto;

        arquivo >> org >> dst >> prc >> sea >> dep >> arr >> sto;

        p->org = org;
        p->dst = dst;
        p->prc = prc;
        p->sea = sea;
        p->dep = dep;
        p->arr = arr;
        p->sto = sto;
        p->dur = std::to_string(Passagem::calcularDuracaoDoVoo(dep, arr));
    }
}

// Membro eh numerico sse membro contem apenas digitos e ponto
bool Passagem::membroEhNumerico(std::string valor) {
    std::string::const_iterator it = valor.begin();
    auto isdigit = [](char c) {
        return c == '0' ||
               c == '1' ||
               c == '2' ||
               c == '3' ||
               c == '4' ||
               c == '5' ||
               c == '6' ||
               c == '7' ||
               c == '8' ||
               c == '9';
    };
    
    while (it != valor.end() && (isdigit(*it) || *it == '.')) ++it;
    return !valor.empty() && it == valor.end();
}

// Funcao de comparacao de igualdade
// Se ambos os campos forem strings, retorna a == b
// cc, retorna (double)a == (double)b (std::stod converte para double)
// Joga excecao se tentar comparar campo nao numerico (eg, origem) e numerico (eg, preco)
bool Passagem::func_eq(std::string a, std::string b)  {
    bool aEhNumero = Passagem::membroEhNumerico(a);
    bool bEhNumero = Passagem::membroEhNumerico(b);

    if(aEhNumero != bEhNumero) throw std::runtime_error("comparando membro numerico e nao numerico");

    if(aEhNumero && bEhNumero) return std::stod(a) == std::stod(b);
    else return a == b;
}

// Funcao de comparacao de menor ou igual
// Se ambos os campos forem strings, retorna a <= b
// cc, retorna (double)a <= (double)b (std::stod converte para double)
// Joga excecao se tentar comparar campo nao numerico (eg, origem) e numerico (eg, preco)
bool Passagem::func_leq(std::string a, std::string b) {
    bool aEhNumero = Passagem::membroEhNumerico(a);
    bool bEhNumero = Passagem::membroEhNumerico(b);

    if(aEhNumero != bEhNumero) throw std::runtime_error("comparando membro numerico e nao numerico");

    if(aEhNumero && bEhNumero) return std::stod(a) <= std::stod(b);
    else return a <= b;
}

// Funcao de comparacao de maior ou igual
// Se ambos os campos forem strings, retorna a >= b
// cc, retorna (double)a >= (double)b (std::stod converte para double)
// Joga excecao se tentar comparar campo nao numerico (eg, origem) e numerico (eg, preco)
bool Passagem::func_geq(std::string a, std::string b) {
    bool aEhNumero = Passagem::membroEhNumerico(a);
    bool bEhNumero = Passagem::membroEhNumerico(b);

    if(aEhNumero != bEhNumero) throw std::runtime_error("comparando membro numerico e nao numerico");

    if(aEhNumero && bEhNumero) return std::stod(a) >= std::stod(b);
    else return a >= b;
}

// Funcao de comparacao de menor
// Feita com base no menor ou igual e na negacao de igual
bool Passagem::func_lt(std::string a, std::string b) {
    return Passagem::func_leq(a, b) && !Passagem::func_eq(a, b);
}

// Funcao de comparacao de maior
// Feita com base no maior ou igual e na negacao de igual
bool Passagem::func_gt(std::string a, std::string b) {
    return Passagem::func_geq(a, b) && !Passagem::func_eq(a, b);
}

// Setter pro trigrama estatico
void Passagem::setTrigrama(std::string trigrama) { Passagem::trigrama = trigrama; }

// Implementacao do quicksort com mediana de 3
// Adaptado do TP1 de ordenacoes indiretas
// Recebe um vetor de ponteiros para passagem
// Faz a desreferencia de ponteiros na mediana e usa
// os operadores overloaded em passagem para a comparacao
void Passagem::ordena(Passagem** array, int Esq, int Dir) {
    auto median = [](Passagem* a, Passagem* b, Passagem* c) {
        if ((*a <= *b) && (*b <= *c)) return a;
        if ((*a <= *c) && (*c <= *b)) return c;
        if ((*b <= *a) && (*a <= *c)) return a;
        if ((*b <= *c) && (*c <= *a)) return c;
        if ((*c <= *a) && (*a <= *b)) return a;
        return b;
    };

    Passagem* x, *w;
    int i = Esq, j = Dir, mid = (i+j)/2;
    x = median(array[i], array[j], array[mid]);
    do
    {
        while (*x > *array[i]) i++;
        while (*x < *array[j]) j--;
        if (i <= j)
        {
            w = array[i];
            array[i] = array[j];
            array[j] = w;
            i++;
            j--;
        }
    } while (i <= j);

    if(Esq < j) Passagem::ordena(array, Esq, j);
    if(i < Dir) Passagem::ordena(array, i, Dir);
}