#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdint.h>
#include <inttypes.h>

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
int leInstrucao (char instrucao[MAX]); // OK
int mnemonicoToHex (char mnemonico[MAX/2]); // OK
void executaInstrucao (int instrucao); // OK
int leDadoDaMemoria (int endereco); // OK
void insereDadoNaMemoria(int dado, int endereco); // OK
void exibeMemoriaBits (int endereco, int linhas); // OK
void exibeMemoriaWords (int endereco, int linhas); // OK
void exibeMemoria (char c); // OK
void executaPrograma (int step); // OK

void carregaPrograma1(); // OK
void carregaPrograma2(); // OK

int main() {

    char instrucao[MAX];
    int instrucaoHex;

    int opcao = -1;
    char STEP[11];
    strcpy(STEP, "DESATIVADO");

    // Inicializa memoria
    resetRegistradores();
    resetMemoria();

    while (opcao) {
        printf("1. Carregar programa N Quadrado na memoria\n");
        printf("2. Carregar programa Soma de Quadrados na memoria\n");
        printf("3. Executar o programa carregado\n");
        printf("4. Ativa/Desativar modo step-by-step (%s)\n", STEP);
        printf("0. Finalizar simulacao\n");

        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            carregaPrograma1();
            printf("Programa carregado com sucesso\n");
            printf("O programa ira pedir um numero para calcular seu quadrado no inicio da execucao\n");
            break;
        case 2:
            carregaPrograma2();
            printf("Programa carregado com sucesso\n");
            printf("O programa ira pedir dois numeros para calcular a soma dos seus quadrados no inicio da execucao\n");
            break;
        case 3:
            if (strcmp(STEP, "ATIVADO") == 0)   executaPrograma(1);
            else if(strcmp(STEP, "DESATIVADO") == 0)   executaPrograma(0);
            printf("Programa executado com sucesso\n");
            break;
        case 4:
            if (strcmp(STEP, "ATIVADO") == 0)   strcpy(STEP, "DESATIVADO");
            else if (strcmp(STEP, "DESATIVADO") == 0)   strcpy(STEP, "ATIVADO");
            break;
        default:
            break;
        }
        printf("\n\n");
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
    printf("-----------------------\n");
    printf("   Acumulador   = %04h" PRIx16, ac);
    printf("\n");
    printf("Program Counter = %04h" PRIx16, pc);
    printf("\n");
    printf("-----------------------\n");
}

int leInstrucao (char instrucao[MAX]) {

    int operacao, endereco;
    int start = -1, end = -1;
    int len = strlen(instrucao);
    char strOperacao[MAX/2], strEndereco[MAX/2];

    // Procura o primeiro espaco na instrucao para isolar o codigo da operacao
    for (int i = 0; i < MAX; i++) {
        if (instrucao[i] == ' ') {
            end = i;
            break;
        }
    }

    // Procura quando acabam o(s) espacos para isolar o endereco
    for (int i = end; i < MAX; i++) {
        if (instrucao[i] != ' ') {
            start = i;
            break;
        }
    }

    // Copia o codigo da operacao para o ponteiro da operacao
    for (int i = 0; i < end; i++) {
        strOperacao[i] = instrucao[i];
    }
    strOperacao[end] = '\0'; // completa o string com o char de fim
    operacao = mnemonicoToHex(strOperacao);

    // Copia o endereco da operacao para o ponteiro endereco
    for (int i = start; i < len; i++) {
        strEndereco[i-start] = instrucao[i];
    }
    strEndereco[len-start] = '\0'; // completa o string com o char de fim
    endereco = (int)strtol(strEndereco, NULL, 16);

    return 0x1000*operacao + endereco;
}

int mnemonicoToHex(char mnemonico[MAX/2]) {

    int codigo;

    // jump unconditional
    if (strcmp(mnemonico, "JP") == 0) {
        codigo = 0x0;
    } else
    // jump if zero
    if (strcmp(mnemonico, "JZ") == 0) {
        codigo = 0x1;
    } else
    //jump if negative
    if (strcmp(mnemonico, "JN") == 0) {
        codigo = 0x2;
    } else
    // load value
    if (strcmp(mnemonico, "LV") == 0) {
        codigo = 0x3;
    } else
    // add
    if (strcmp(mnemonico, "+") == 0) {
        codigo = 0x4;
    } else
    // subtract
    if (strcmp(mnemonico, "-") == 0) {
        codigo = 0x5;
    } else
    // multiply
    if (strcmp(mnemonico, "*") == 0) {
        codigo = 0x6;
    } else
    // divide
    if (strcmp(mnemonico, "/") == 0) {
        codigo = 0x7;
    } else
    // load from memory
    if (strcmp(mnemonico, "LD") == 0) {
        codigo = 0x8;
    } else
    // move to memory
    if (strcmp(mnemonico, "MM") == 0) {
        codigo = 0x9;
    } else
    // subroutine call
    if (strcmp(mnemonico, "SC") == 0) {
        codigo = 0xA;
    } else
    // return from subroutine
    if (strcmp(mnemonico, "RS") == 0) {
        codigo = 0xB;
    } else
    // halt machine
    if (strcmp(mnemonico, "HM") == 0) {
        codigo = 0xC;
    } else
    // get data
    if (strcmp(mnemonico, "GD") == 0) {
        codigo = 0xD;
    } else
    // put data
    if (strcmp(mnemonico, "PD") == 0) {
        codigo = 0xE;
    } else
    //operating system call
    if (strcmp(mnemonico, "OS") == 0) {
        codigo = 0xF;
    } else
    // origin
    if (strcmp(mnemonico, "@") == 0) {
        codigo = -1;
    } else
    // end
    if (strcmp(mnemonico, "#") == 0) {
        codigo = -1;
    } else
    // constant
    if (strcmp(mnemonico, "K") == 0) {
        codigo = -1;
    } else codigo = -1;

    return codigo;
}

void executaInstrucao(int instrucao) {

    int operacao, endereco;

    operacao = instrucao / 0x1000;
    endereco = instrucao % 0x1000;

    if (DEBUG) {
        printf("operacao: %x\nendereco: %x\n", operacao, endereco);
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
        printf("Acumulador = ");
        scanf("%hd", &ac);
        pc += 0x10;
        break;
    // put data
    case 0xE:
        printf("\n********************\n");
        printf("ACUMULADOR = %hd", ac);
        printf("\n********************\n");
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

void exibeMemoriaBits(int endereco, int linhas) {
    printf("-------------------------------------\n");
    printf("     0 1 2 3 4 5 6 7 8 9 a b c d e f\n");
    for (int l = 0; l < linhas; l++) {
        printf("%02x - ", ( endereco/0x10+l ));
        for (int i = 0; i < 0x10; i++) {
            printf("%d ", memoria[endereco + 0x10*l + i]);
        }
        printf("\n");
    }
    printf("-------------------------------------\n");
}

void exibeMemoriaWords(int endereco, int linhas) {
    printf("-----------------------------------------------------------------------------------\n");
    printf("    00   10   20   30   40   50   60   70   80   90   a0   b0   c0   d0   e0   f0  \n");
    for (int l = 0; l < linhas; l++) {
        printf("%01x - ", ( endereco/0x100+l ));
        for (int i = 0; i < 0x10; i++) {
            printf("%04hx ", leDadoDaMemoria(endereco + 0x100*l + 0x10*i));
        }
        printf("\n");
    }
    printf("-----------------------------------------------------------------------------------\n");
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
            printf("Pressione ENTER para continuar\n");
            getchar();
        }

        executaInstrucao(instrucao);

        if (DEBUG) {
            printf("HALT = %d\n", HALT);
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

    // Pede a entrada do programa
    insereDadoNaMemoria(0x0200, 0x000);
    insereDadoNaMemoria(0xD000, 0x200);
    insereDadoNaMemoria(0x9150, 0x210);
    insereDadoNaMemoria(0x0010, 0x220);

    // Algoritmo do programa
    insereDadoNaMemoria(0x8120, 0x010);
    insereDadoNaMemoria(0x9170, 0x020);
    insereDadoNaMemoria(0x9140, 0x030);
    insereDadoNaMemoria(0x9160, 0x040);
    insereDadoNaMemoria(0x8170, 0x050);
    insereDadoNaMemoria(0x5150, 0x060);
    insereDadoNaMemoria(0x1110, 0x070);
    insereDadoNaMemoria(0x8170, 0x080);
    insereDadoNaMemoria(0x4120, 0x090);
    insereDadoNaMemoria(0x9170, 0x0A0);
    insereDadoNaMemoria(0x8140, 0x0B0);
    insereDadoNaMemoria(0x4130, 0x0C0);
    insereDadoNaMemoria(0x9140, 0x0D0);
    insereDadoNaMemoria(0x4160, 0x0E0);
    insereDadoNaMemoria(0x9160, 0x0F0);
    insereDadoNaMemoria(0x0050, 0x100);
    insereDadoNaMemoria(0xC110, 0x110);

    // Constantes usadas pelo programa
    insereDadoNaMemoria(0x0001, 0x120); // UM
    insereDadoNaMemoria(0x0002, 0x130); // DOIS

    // Ajusta o contador de programa para o início do programa carregado
    pc = 0x0;
}

void carregaPrograma2() {
    resetMemoria();
    resetRegistradores();

    // Programa Principal
    insereDadoNaMemoria(leInstrucao("GD 000"), 0x000);
    insereDadoNaMemoria(leInstrucao("MM 0E0"), 0x010);
    insereDadoNaMemoria(leInstrucao("GD 000"), 0x020);
    insereDadoNaMemoria(leInstrucao("MM 0F0"), 0x030);
    insereDadoNaMemoria(leInstrucao("MM FD0"), 0x040);
    insereDadoNaMemoria(leInstrucao("SC E70"), 0x050);
    insereDadoNaMemoria(leInstrucao("MM 100"), 0x060);
    insereDadoNaMemoria(leInstrucao("LD 0E0"), 0x070);
    insereDadoNaMemoria(leInstrucao("MM FD0"), 0x080);
    insereDadoNaMemoria(leInstrucao("SC E70"), 0x090);
    insereDadoNaMemoria(leInstrucao("+  100"), 0x0A0);
    insereDadoNaMemoria(leInstrucao("MM 100"), 0x0B0);
    insereDadoNaMemoria(leInstrucao("PD 000"), 0x0C0);
    insereDadoNaMemoria(leInstrucao("HM 200"), 0x0D0);
    // 0x0E0 = A
    // 0x0F0 = B
    // 0x100 = B2 -> B2 + A2

    // Subrotina N Quadrado - começa em 0xE70 com o endereço de retorno
    // 0xE70 = endereço de retorno
    insereDadoNaMemoria(leInstrucao("LD FA0"), 0xE80);
    insereDadoNaMemoria(leInstrucao("MM FF0"), 0xE90);
    insereDadoNaMemoria(leInstrucao("MM FE0"), 0xEA0);
    insereDadoNaMemoria(leInstrucao("MM FC0"), 0xEB0);
    insereDadoNaMemoria(leInstrucao("LD FF0"), 0xEC0);
    insereDadoNaMemoria(leInstrucao("-  FD0"), 0xED0);
    insereDadoNaMemoria(leInstrucao("JZ F80"), 0xEE0);
    insereDadoNaMemoria(leInstrucao("LD FF0"), 0xEF0);
    insereDadoNaMemoria(leInstrucao("+  FA0"), 0xF00);
    insereDadoNaMemoria(leInstrucao("MM FF0"), 0xF10);
    insereDadoNaMemoria(leInstrucao("LD FC0"), 0xF20);
    insereDadoNaMemoria(leInstrucao("+  FB0"), 0xF30);
    insereDadoNaMemoria(leInstrucao("MM FC0"), 0xF40);
    insereDadoNaMemoria(leInstrucao("+  FE0"), 0xF50);
    insereDadoNaMemoria(leInstrucao("MM FE0"), 0xF60);
    insereDadoNaMemoria(leInstrucao("JP EC0"), 0xF70);
    insereDadoNaMemoria(leInstrucao("LD FE0"), 0xF80);
    insereDadoNaMemoria(leInstrucao("RS E70"), 0xF90);
    insereDadoNaMemoria(0x001, 0xFA0); // UM       = 0xFA0
    insereDadoNaMemoria(0x002, 0xFB0); // DOIS     = 0xFB0
    // IMPAR    = 0xFC0
    // N        = 0xFD0
    // N2       = 0xFE0
    // CONT     = 0xFF0

    pc = 0x0;
}
