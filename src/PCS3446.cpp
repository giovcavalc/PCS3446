//==============================================================================
// Name        : PCS3446.cpp
// Author      : Giovanni Cavalcante
// Version     :
// Copyright   : Your copyright notice
// Description : Exercício programa referente à disciplina Sistemas Operacionais
//==============================================================================

#include <math.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "Montador.h"

using namespace std;

#define MAX 8 // Tamanho máximo de uma instrucao. Na verdade o tamanho nao vai passar de 7 se houver um unico espaco. *(/NULL)
#define SIZEMEM 0xFFF // Tamanho da memoria

#define DEBUG 0 // para mostrar os prints utilizados para acompanhar o desenvolvimento do programa

// Essa variável indica o estado da máquina (ativa ou inativa)
int HALT = 0;

// CRIACAO DA MEMORIA
int memoria[SIZEMEM];

// DEFINICAO DOS REGISTRADORES
// Cria os registradores pelo seu tipo
int16_t ac; // accumulator
int16_t pc; // program counter

// CABECALHOS DAS FUNCOES
void resetRegistradores(); // OK
void resetMemoria(); // OK
void mostraRegistradores(); // OK
void executaInstrucao (int instrucao); // OK
int leDadoDaMemoria (int endereco); // OK
void insereDadoNaMemoria(int dado, int endereco); // OK
void exibeMemoriaBits (int endereco, int linhas); // OK
void exibeMemoriaWords (int endereco, int linhas); // OK
void exibeMemoria (char c); // OK
void executaPrograma (int step); // OK

void carregaPrograma1(); // OK
void carregaPrograma2(); // OK

Montador *montador = new Montador();

int main() {

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

    //char instrucao[MAX];
    //int instrucaoHex;

	vector<int> opcoes = {1, 3, 0};
	vector<int>::iterator it = opcoes.begin();

    int opcao = -1;
    char STEP[11];
    strcpy(STEP, "DESATIVADO");

    // Inicializa memoria
    resetRegistradores();
    resetMemoria();

    while (opcao) {
        cout << "1. Carregar programa N Quadrado na memoria" << endl;
        cout << "2. Carregar programa Soma de Quadrados na memoria" << endl;
        cout << "3. Executar o programa carregado" << endl;
        cout << "4. Ativa/Desativar modo step-by-step (" << STEP << ")" << endl;
        cout << "0. Finalizar simulacao" << endl;

        cin >> opcao;
        opcao = *it;
        it++;

        switch (opcao) {
        case 1:
            carregaPrograma1();
            cout << "Programa carregado com sucesso" << endl;
            cout << "O programa ira pedir um numero para calcular seu quadrado no inicio da execucao" << endl;
            break;
        case 2:
            carregaPrograma2();
            cout << "Programa carregado com sucesso" << endl;
            cout << "O programa ira pedir dois numeros para calcular a soma dos seus quadrados no inicio da execucao" << endl;
            break;
        case 3:
            if (strcmp(STEP, "ATIVADO") == 0)   executaPrograma(1);
            else if(strcmp(STEP, "DESATIVADO") == 0)   executaPrograma(0);
            cout << "Programa executado com sucesso" << endl;
            break;
        case 4:
            if (strcmp(STEP, "ATIVADO") == 0)   strcpy(STEP, "DESATIVADO");
            else if (strcmp(STEP, "DESATIVADO") == 0)   strcpy(STEP, "ATIVADO");
            break;
        default:
            delete montador;
            break;
        }
        cout << endl << endl;
    }

    return 0;
}

// FUNÇÕES RESPONSÁVEIS PELO FUNCIONAMENTO DA MÁQUINA DE VON NEUMANN
void resetRegistradores() {
    ac = 0x0;
    pc = 0x0;
}

void resetMemoria() {
    for (int i = 0; i < SIZEMEM; i++)
        memoria[i] = 0;
}

void mostraRegistradores() {
    cout << "-----------------------" << endl;
    cout << "   Acumulador   = " << hex << noshowbase << setfill('0') << setw(4) << ac;
//    printf("   Acumulador   = %04h" PRIx16, ac);
    cout << endl;
//    printf("Program Counter = %04h" PRIx16, pc);
    cout << "Program Counter = " << hex << noshowbase << setfill('0') << setw(4) << pc;
    cout << endl;
    cout << "-----------------------" << endl;
}

void executaInstrucao(int instrucao) {

    int operacao, endereco;

    operacao = instrucao / 0x1000;
    endereco = instrucao % 0x1000;

    if (DEBUG) {
        cout << "operacao: " << operacao << endl << "endereco: " << endereco << endl;
    }

    switch (operacao) {
    // jump unconditional
    case 0x0: // OK
        pc = endereco;
        break;
    // jump if zero
    case 0x1: // OK
        if (ac == 0x0) pc = endereco; // complemento de 2 - primeiro bit e' 1
        else pc += 0x10; // pula para a prooxima instrucao
        break;
    //jump if negative
    case 0x2: // OK
        //0111 1111 1111 1111 - ultimo positivo
        //7    F    F    F
        if (ac > 0x7FFF) pc = endereco; // complemento de 2 - primeiro bit e' 1
        else pc += 0x10; // pula para a prooxima instrucao
        break;
    // load value
    case 0x3: // OK (somente em hexa - buga pra -1/fff)
        // 1000 0000 0000
        // 8    0    0
        if (endereco < 0x800) ac = endereco;
        else ac = 0xF000 + endereco;
        pc += 0x10;
        break;
    // add
    case 0x4:
        ac = ac + leDadoDaMemoria(endereco);
        pc += 0x10;
        break;
    // subtract
    case 0x5:
        ac = ac - leDadoDaMemoria(endereco);
        pc += 0x10;
        break;
    // multiply
    case 0x6:
        ac = ac * leDadoDaMemoria(endereco);
        pc += 0x10;
        break;
    // divide
    case 0x7:
        ac = ac / leDadoDaMemoria(endereco);
        break;
    // load from memory
    case 0x8:
        ac = leDadoDaMemoria(endereco);
        pc += 0x10;
        break;
    // move to memory
    case 0x9:
        //memoria[endereco] = ac;
        insereDadoNaMemoria(ac, endereco);
        pc += 0x10;
        break;
    // subroutine call
    case 0xA:
        //memoria[endereco] = pc + 0x10;
        insereDadoNaMemoria(pc+0x10, endereco);
        pc = endereco + 0x10;
        break;
    // return from subroutine
    case 0xB:
        pc = leDadoDaMemoria(endereco);
        break;
    // halt machine
    case 0xC:
        pc = endereco;
        HALT = 1;
        break;
    // get data
    case 0xD:
        cout << "Acumulador = ";
//        scanf("%hd", &ac);
        cin >> setbase(16) >> ac;
        pc += 0x10;
        break;
    // put data
    case 0xE:
        cout << endl << "********************" << endl;
//        printf("ACUMULADOR = %hd", ac);
        cout << "ACUMULADOR = " << hex << noshowbase << ac;
        cout << endl << "********************" << endl;
        pc += 0x10;
        break;
    //operating system call
    case 0xF:

        break;

    default: break;
    }
}

int leDadoDaMemoria(int endereco) {
    // Supõe-se que o operando está escrito do bit menos significativo para
    // o mais significativo no sentido crescente dos endereços de memória
    int operando = 0;
    for (int i = 0; i < 16; i++) {
        operando += (int) pow(2, i) * memoria[endereco+i];
    }
    return operando;
}

void insereDadoNaMemoria (int dado, int endereco) {
    // Falta verificar se o endereço é múltiplo de 16 !!!!

	if (dado >= 0 && endereco >= 0) {
		// Muda a base do dado de decimal para binário, colocando o resultado
		// num vetor em little endian, assim como a memória
		int dadoBinario[16];

		for (int i = 0; i < 16; i++) {
			dadoBinario[i] = dado % 2;
			dado = dado / 2;
		}

		// Agora copia-se o vetor num espaço de 16 bits da memória começando em "endereco"
		for (int i = 0; i < 0x10; i++) memoria[endereco+i] = dadoBinario[i];
	}
}

void exibeMemoriaBits(int endereco, int linhas) {
    cout << "-------------------------------------" << endl;
    cout << "     0 1 2 3 4 5 6 7 8 9 a b c d e f " << endl;
    for (int l = 0; l < linhas; l++) {
//        printf("%02x - ", ( endereco/0x10+l ));
    	cout << hex << noshowbase << setfill('0') << setw(2) << ( endereco/0x10+l ) << " - ";
        for (int i = 0; i < 0x10; i++) {
//            printf("%d ", memoria[endereco + 0x10*l + i]);
        	cout << memoria[endereco + 0x10*l + i] << " " << endl;
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl;
}

void exibeMemoriaWords(int endereco, int linhas) {
    cout << "-----------------------------------------------------------------------------------" << endl;
    cout << "    00   10   20   30   40   50   60   70   80   90   a0   b0   c0   d0   e0   f0  " << endl;
    for (int l = 0; l < linhas; l++) {
//        printf("%01x - ", ( endereco/0x100+l ));
    	cout << hex << noshowbase << setfill('0') << setw(1) << ( endereco/0x100+l ) << " - ";
        for (int i = 0; i < 0x10; i++) {
            cout << hex << noshowbase << setfill('0') << setw(4) << leDadoDaMemoria(endereco + 0x100*l + 0x10*i) << " ";
        }
        cout << endl;
    }
    cout << "-----------------------------------------------------------------------------------" << endl;
}

void exibeMemoria(char c) {
    // Exibe toda a memória em bits ou em words
    switch (c) {
    case 'b':
        exibeMemoriaBits(0x0, 0x100);
        break;
    case 'w':
        exibeMemoriaWords(0x0, 0x10);
        break;
    default: break;
    }
}

void executaPrograma(int step) {
    HALT = 0;
    while (!HALT) {
        int instrucao = leDadoDaMemoria(pc);

        if (instrucao == pc)    break;

        if (step) {
            mostraRegistradores();
            exibeMemoria('w');
            cout << "Pressione ENTER para continuar" << endl;
            getchar();
        }

        executaInstrucao(instrucao);

        if (DEBUG) {
            cout << "HALT = " <<  HALT << endl;
        }
    }
    mostraRegistradores();
    exibeMemoria('w');
}

void carregaPrograma1() {
    resetMemoria();
    resetRegistradores();

    /*
            @   /010    010
    INIC:               010
            LD  UM      010 8120
            MM  CONT    020 9170
            MM  IMPAR   030 9140
            MM  N2      040 9160
    LOOP:               050
            LD  CONT    050 8170
            -   N       060 5150
            JZ  FORA    070 1110
            LD  CONT    080 8170
            +   UM      090 4120
            MM  CONT    0A0 9170
            LD  IMPAR   0B0 8140
            +   DOIS    0C0 4130
            MM  IMPAR   0D0 9140
            +   N2      0E0 4160
            MM  N2      0F0 9160
            JP  LOOP    100 0050
    FORA:               110
            HM  FORA    110 C110
    UM      K   01      120 01
    DOIS    K   02      130 02
    IMPAR   K   0       140 00
    N       K   4       150 04
    N2      K   0       160 00
    CONT    K   0       170 00
            #   INIC    010
    */

    vector<pair<string, int>> instrucoes = montador->carregaPrograma("resources/prog1.txt");

    for (pair<string, int> instrucao : instrucoes) {
        cout << instrucao.first << " : " << instrucao.second << endl;

        if (instrucao.first.substr(1, 1) == "@") {
        	// Início do programa
        	string instrEndInicial = instrucao.first;
        	int enderecoInicial = montador->stoi( instrEndInicial.substr(1, instrEndInicial.size()-1), 16 );

        	pc = enderecoInicial;
        } else if (instrucao.first.substr(1, 1) == "#") {
        	// Final do programa
        	break;
        } else {
        	insereDadoNaMemoria(montador->stoi(instrucao.first, 16), instrucao.second);
        }

    }

    // Ajusta o contador de programa para o início do programa carregado
    pc = 0x10;
}

void carregaPrograma2() {
    resetMemoria();
    resetRegistradores();

    vector<pair<string, int>> instrucoes = montador->carregaPrograma("resources/prog2.txt");

    for (pair<string, int> instrucao : instrucoes) {
        cout << instrucao.first << " : " << instrucao.second << endl;

        insereDadoNaMemoria(montador->stoi(instrucao.first), instrucao.second);
    }

    pc = 0x0;
}
