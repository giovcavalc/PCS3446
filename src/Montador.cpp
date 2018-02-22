/*
 * Montador.cpp
 *
 *  Created on: 21 de fev de 2018
 *      Author: giovcavalc
 */

#include "Montador.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>

namespace std {

Montador::Montador() {
	// TODO Auto-generated constructor stub

}

Montador::~Montador() {
	// TODO Auto-generated destructor stub
}

vector<pair<string, int>> Montador::carregaPrograma(string programa) {
    ifstream prog;
    int endereco;
    map<string, int> tabelaDeNomes;
    string operando;
    string label;
    string linha;
    string opcode;
    vector<string> campos;
    vector<pair<string, int>> instrucoes;

    //PRIMEIRA LEITURA
    this->setPrograma(programa);

    prog.open(this->programa, ios::in);

    if (prog.is_open()) {

        // Pega o endereço de início do programa
        getline(prog, linha);
        campos = this->split(linha);
        if (campos[0] == "@") {
            endereco = stoi(campos[1]);
            cout << "Identificando o inicio do programa: @ " << hex  << showbase << endereco << endl;
        }

        while (getline(prog, linha)) {
            char ch = linha[0];
            campos = split(linha);
            if ( (ch == ' ' || ch == '	') ) {
                opcode = campos[0];
                operando = campos[1];
                cout << "OPCODE: " << opcode << " - END: " << endereco << endl;
            } else {
                label = campos[0];
                opcode = campos[1];
                operando = campos[2];

                cout << "LABEL: " << label << " - OPCODE: " << opcode << " - END: " << operando << endl;

                pair<string, int> entry = make_pair(label, endereco);
                tabelaDeNomes.insert(entry);
            }
            endereco += 0x10;
        }
        mostraTabelaDeNomes(tabelaDeNomes);
    } else {
        cout << "Nao foi possivel abrir o arquivo." << endl;
    }

    prog.close();

    // SEGUNDA LEITURA

    prog.open(this->programa, ios::in);

    if (prog.is_open()) {
        // Pega o endereço de início do programa
        getline(prog, linha);
        campos = split(linha);
        if (campos[0] == "@") {
            endereco = stoi(campos[1]);
            cout << "Identificando o inicio do programa: @ " << hex  << showbase << endereco << endl;

            // Adiciona o endereço inicial do programa no vector de instruções
            stringstream buffer;
            buffer << campos[0] << hex << noshowbase << endereco;
            pair<string, int> instrucao = make_pair(buffer.str(), endereco);
            instrucoes.push_back(instrucao);
        } else {
        	cerr << "O formato do arquivo não é válido!" << endl;
        }

        while (getline(prog, linha)) {
            char ch = linha[0];
            stringstream buffer;
            campos = split(linha);
            string campo0 = campos[0];
            if (campos[0] == "#") {
            	cout << "Identificando o final do programa: # " << endereco << endl;

            	// Adiciona o endereço final do programa no vector de instruções
				buffer << campos[0] << hex << noshowbase << endereco;
				string end = buffer.str();
				pair<string, int> instrucao = make_pair(buffer.str(), endereco);
				instrucoes.push_back(instrucao);
            }
            else if ( (ch == ' ' || ch == '	') ) {
                opcode = campos[0];
                operando = campos[1];
                buffer << hex << noshowbase << mnemonicoToHex(opcode) << geraOperando(operando, tabelaDeNomes);
            } else {
                label = campos[0];
                opcode = campos[1];
                operando = campos[2];

                buffer << hex << noshowbase << mnemonicoToHex(opcode) << geraOperando(operando, tabelaDeNomes);
            }

            pair<string, int> instrucao = make_pair(buffer.str(), endereco);
            instrucoes.push_back(instrucao);

            endereco += 0x10;
        }
    }

    return instrucoes;
}

int Montador::geraOperando(string operando, map<string, int> tabelaDeNomes) {
    int operandoInteiro = stoi(operando);
    if (operandoInteiro == -1) {
        int operandoSimbolico = tabelaDeNomes[operando];
        return operandoSimbolico;
    } else {
        return operandoInteiro;
    }
}

int Montador::leInstrucao (string opcode, int operando) {

    int opcodeHex = mnemonicoToHex(opcode);
    return opcodeHex * 0x1000 + operando;
}

int Montador::mnemonicoToHex(string opcode) {

    int codigo;

    // jump unconditional
    if (opcode.compare("JP") == 0) {
        codigo = 0x0;
    } else
    // jump if zero
    if (opcode.compare("JZ") == 0) {
        codigo = 0x1;
    } else
    //jump if negative
    if (opcode.compare("JN") == 0) {
        codigo = 0x2;
    } else
    // load value
    if (opcode.compare("LV") == 0) {
        codigo = 0x3;
    } else
    // add
    if (opcode.compare("+") == 0) {
        codigo = 0x4;
    } else
    // subtract
    if (opcode.compare("-") == 0) {
        codigo = 0x5;
    } else
    // multiply
    if (opcode.compare("*") == 0) {
        codigo = 0x6;
    } else
    // divide
    if (opcode.compare("/") == 0) {
        codigo = 0x7;
    } else
    // load from memory
    if (opcode.compare("LD") == 0) {
        codigo = 0x8;
    } else
    // move to memory
    if (opcode.compare("MM") == 0) {
        codigo = 0x9;
    } else
    // subroutine call
    if (opcode.compare("SC") == 0) {
        codigo = 0xA;
    } else
    // return from subroutine
    if (opcode.compare("RS") == 0) {
        codigo = 0xB;
    } else
    // halt machine
    if (opcode.compare("HM") == 0) {
        codigo = 0xC;
    } else
    // get data
    if (opcode.compare("GD") == 0) {
        codigo = 0xD;
    } else
    // put data
    if (opcode.compare("PD") == 0) {
        codigo = 0xE;
    } else
    //operating system call
    if (opcode.compare("OS") == 0) {
        codigo = 0xF;
    } else
    // origin
    if (opcode.compare("@") == 0) {
        codigo = -1;
    } else
    // end
    if (opcode.compare("#") == 0) {
        codigo = -1;
    } else
    // constant
    if (opcode.compare("K") == 0) {
        codigo = 0;
    } else codigo = -1;

    return codigo;
}

void Montador::mostraTabelaDeNomes(map<string,int> tabelaDeNomes) {
    map<string, int>::iterator it;
    cout << endl;
    cout << "---- TABELA DE NOMES ----" << endl;
    cout << "|-----------|-----------|" << endl;
    for (it = tabelaDeNomes.begin(); it != tabelaDeNomes.end(); it++) {
        cout << "|";
        cout << setw(10) << right << it->first;
        cout << " | ";
        cout << setw(10) << left << it->second;
        cout << "|" << endl;
    }
    cout << "|-----------|-----------|" << endl;
    cout << endl;
}

vector<string> Montador::split(string sentence) {
    istringstream iss(sentence);
    vector<string> tokens{ istream_iterator<string>(iss), istream_iterator<string>{} };

    return tokens;
}

int Montador::stoi(string str) {
    // Verifica se o string é hexadecimal
    if (str[0] == '/')
        return stoi(str.substr(1, str.size()-1), 16);
    // Se não for converte para decimal
    return stoi(str, 10);
}

int Montador::stoi(string str, int base) {
    int n = 0;
    int pos = 1;

    // Leitura reversa do string
    for (int i = str.size()-1; i >= 0; i--) {
        char c = str[i];
        int dig;
        if ('0' <= c && c <= '9') {
            dig = c - '0';
        } else if ('a' <= c && c <= 'f') {
            dig = 10 + (c - 'a');
        } else if ('A' <= c && c <= 'F') {
            dig = 10 + (c - 'A');
        } else {
            // Indica que não se trata de um número
            return -1;
        }

        n += pos * dig;
        pos *= base;
    }

    return n;
}

string Montador::getPrograma() {
    return this->programa;
}

void Montador::setPrograma(string programa) {
    this->programa = programa;
}


} /* namespace std */
