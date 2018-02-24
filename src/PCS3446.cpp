//==============================================================================
// Name        : PCS3446.cpp
// Author      : Giovanni Cavalcante
// Version     :
// Copyright   : Your copyright notice
// Description : Exercício programa referente à disciplina Sistemas Operacionais
//==============================================================================

#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "Montador.h"
#include "Simulador.h"

#define DEBUG false

using namespace std;

#define MAX 8 // Tamanho máximo de uma instrucao. Na verdade o tamanho nao vai passar de 7 se houver um unico espaco. *(/NULL)

Simulador *simulador = new Simulador();

int main() {
    //char instrucao[MAX];
    //int instrucaoHex;

	vector<int> opcoes = {3, 4, 0};
	vector<int>::iterator it = opcoes.begin();

    int opcao = -1;

    while (opcao) {
        cout << "1. Carregar programa N Quadrado na memoria" << endl;
        cout << "2. Carregar programa Soma de Quadrados na memoria" << endl;
        cout << "3. Carregar programa de teste do Endereçamento Indireto" << endl;
        cout << "4. Executar o programa carregado" << endl;
        cout << "5. Ativa/Desativar modo step-by-step (" << simulador->getStepString() << ")" << endl;
        cout << "0. Finalizar simulacao" << endl;

        cin >> opcao;
        if (DEBUG) {
			opcao = *it;
			it++;
        }

        switch (opcao) {
        case 1:
            simulador->carregaPrograma("prog1.txt");
            cout << "Programa 1 carregado com sucesso" << endl;
            break;
        case 2:
            simulador->carregaPrograma("prog2.txt");
            cout << "Programa 2 carregado com sucesso" << endl;
            cout << "O programa ira pedir dois numeros para calcular a soma dos seus quadrados no inicio da execucao" << endl;
            break;

        case 3:
        	simulador->carregaPrograma("prog3.txt");
        	cout << "Programa 3 carregado com sucesso" << endl;
        	break;
        case 4:
            simulador->executaPrograma();
            cout << "Programa executado com sucesso" << endl;
            break;
        case 5:
        	simulador->toggleStep();
            break;
        default:
            delete simulador;
        	break;
        }
        cout << endl << endl;
    }

    return 0;
}
