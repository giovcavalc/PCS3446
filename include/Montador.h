#ifndef MONTADOR_H
#define MONTADOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <iomanip>

using namespace std;

class Montador
{
    public:
        string programa;

        Montador();
        virtual ~Montador();

        /**
         * L� o programa e devolve um vector de pares <instru��o, endere�o>
         */
        vector<pair<string, int>> carregaPrograma(string programa);

        /**
         * Exibe a tabela de Nomes para debug
         */
        void mostraTabelaDeNomes(map<string, int> tabelaDeNomes);

        /**
         * Converte o string para um int de base 10
         * Se o string come�ar com '/' converte para um int de base 16
         * @return inteiro de base 10 escrito no string
         */
        int stoi(string str);

        string getPrograma();

        void setPrograma(string programa);

    private:
        /**
         * Interpreta o operando como imediato ou s�mbolo
         * e devolve o inteiro correto.
         */
        int geraOperando(string operando, map<string, int> tabelaDeNomes);

        /**
         * Interpreta o opcode e gera uma instru��o hexadecimal completa
         */
        int leInstrucao(string opcode, int operando);

        /**
         * Interpreta o opcode para seu c�digo hexadecimal
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

#endif // MONTADOR_H
