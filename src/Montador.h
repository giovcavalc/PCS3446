/*
 * Montador.h
 *
 *  Created on: 21 de fev de 2018
 *      Author: giovcavalc
 */

#ifndef MONTADOR_H_
#define MONTADOR_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace std {

class Montador {
public:
    string programa;

    Montador();
    virtual ~Montador();

    /**
     * Lê o programa e devolve um vector de pares <instrução, endereço>
     */
    vector<pair<string, int>> carregaPrograma(string programa);

    /**
     * Exibe a tabela de Nomes para debug
     */
    void mostraTabelaDeNomes(map<string, int> tabelaDeNomes);

    /**
     * Converte o string para um int de base 10
     * Se o string começar com '/' converte para um int de base 16
     * @return inteiro de base 10 escrito no string
     */
    int stoi(string str);

    string getPrograma();

    void setPrograma(string programa);

private:
    /**
     * Interpreta o operando como imediato ou símbolo
     * e devolve o inteiro correto.
     */
    int geraOperando(string operando, map<string, int> tabelaDeNomes);

    /**
     * Interpreta o opcode e gera uma instrução hexadecimal completa
     */
    int leInstrucao(string opcode, int operando);

    /**
     * Interpreta o opcode para seu código hexadecimal
     */
    int mnemonicoToHex(string opcode);

    /**
     * Divide a linha recebida em um array de strings
     * @return vector<string> com as palavras
     */
    vector<string> split(string sentence);

    /**
     * Converte o string para um int com a base dada
     * @return inteiro de base 10 escrito no string
     */
    int stoi(string str, int base);
};

} /* namespace std */

#endif /* MONTADOR_H_ */
