/*
 * Simulador.cpp
 *
 *  Created on: 23 de fev de 2018
 *      Author: giovcavalc
 */

#include "Simulador.h"

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

#include "Montador.h"

#define DEBUG false
#define DEBUGECLIPSE false

namespace std {

Simulador::Simulador() {
	this->enderecamentoIndireto = false;
	this->halt = false;
	this->step = true;

	this->ac = 0x0;
	this->pc = 0x0;
	this->memory = new Memoria();
	this->montador = new Montador();
}

Simulador::~Simulador() {
	delete this->montador;
}

// Métodos auxiliares
void Simulador::carregaPrograma(string programa) {
	this->resetMemoria();
	this->resetRegistradores();

	vector<pair<string, int>> instrucoes = montador->carregaPrograma(
			"resources/" + programa);

	for (pair<string, int> instrucao : instrucoes) {
		if (DEBUG) {
			cout << instrucao.first << " : " << instrucao.second << endl;
		}

		if (*(instrucao.first.begin()) == '@') {
			continue;
		} else if (*(instrucao.first.begin()) == '#') {
			// Indica o endereço em que a execução do programa começa
			string instrEndInicial = instrucao.first;
			int enderecoInicial = montador->stoi(
					instrEndInicial.substr(1, instrEndInicial.size() - 1), 16);

			this->setPc(enderecoInicial);
			break;
		} else {
			this->setMemoryWord(montador->stoi(instrucao.first, 16),
					instrucao.second);
		}

	}

}

void Simulador::executaInstrucao(int instrucao) {

	int operacao, endereco;

	operacao = instrucao / 0x1000;
	endereco = instrucao % 0x1000;

	if (DEBUG) {
		cout << "operacao: " << operacao << endl << "endereco: " << endereco
				<< endl;
	}

	switch (operacao) {
	// jump unconditional
	case 0x0: // OK
		if (!this->getEnderecamentoIndireto()) {
			this->setPc(endereco);
		} else {
			this->setPc(this->getMemoryWord(endereco));
			this->setEnderecamentoIndireto(false);
		}
		break;
		// jump if zero
	case 0x1: // OK
		if (this->getAc() == 0x0) {
			if (!this->getEnderecamentoIndireto()) {
				this->setPc(endereco); // complemento de 2 - primeiro bit e' 1
			} else {
				this->setPc(this->getMemoryWord(endereco));
				this->setEnderecamentoIndireto(false);
			}
		} else
			this->incrementaPc();
		break;
		//jump if negative
	case 0x2: // OK
		//0111 1111 1111 1111 - ultimo positivo
		//7    F    F    F
		if (this->getAc() > 0x7FFF) {
			if (!this->getEnderecamentoIndireto()) {
				this->setPc(endereco); // complemento de 2 - primeiro bit e' 1
			} else {
				this->setPc(this->getMemoryWord(endereco));
				this->setEnderecamentoIndireto(false);
			}
		} else
			this->incrementaPc();
		break;
		// load value
	case 0x3: // OK
		if (!this->getEnderecamentoIndireto()) {
			this->setAc(endereco);
		} else {
			this->setAc(this->getMemoryWord(endereco));
			this->setEnderecamentoIndireto(false);
		}
		this->incrementaPc();
		break;
		// add
	case 0x4:
		if (!this->getEnderecamentoIndireto()) {
			this->setAc(this->getAc() + this->getMemoryWord(endereco));
		} else {
			this->setAc(
					this->getAc()
							+ this->getMemoryWord(
									this->getMemoryWord(endereco)));
			this->setEnderecamentoIndireto(false);
		}
		this->incrementaPc();
		break;
		// subtract
	case 0x5:
		if (!this->getEnderecamentoIndireto()) {
			this->setAc(this->getAc() - this->getMemoryWord(endereco));
		} else {
			this->setAc(
					this->getAc()
							- this->getMemoryWord(
									this->getMemoryWord(endereco)));
			this->setEnderecamentoIndireto(false);
		}
		this->incrementaPc();
		break;
		// multiply
	case 0x6:
		if (!this->getEnderecamentoIndireto()) {
			this->setAc(this->getAc() * this->getMemoryWord(endereco));
		} else {
			this->setAc(
					this->getAc()
							* this->getMemoryWord(
									this->getMemoryWord(endereco)));
			this->setEnderecamentoIndireto(false);
		}
		this->incrementaPc();
		break;
		// divide
	case 0x7:
		if (!this->getEnderecamentoIndireto()) {
			this->setAc(this->getAc() / this->getMemoryWord(endereco));
		} else {
			this->setAc(
					this->getAc()
							/ this->getMemoryWord(
									this->getMemoryWord(endereco)));
			this->setEnderecamentoIndireto(false);
		}
		this->incrementaPc();
		break;
		// load from memory
	case 0x8:
		if (!this->getEnderecamentoIndireto()) {
			this->setAc(this->getMemoryWord(endereco));
		} else {
			this->setAc(this->getMemoryWord(this->getMemoryWord(endereco)));
			this->setEnderecamentoIndireto(false);
		}
		this->incrementaPc();
		break;
		// move to memory
	case 0x9:
		if (!this->getEnderecamentoIndireto()) {
			//memoria[endereco] = ac;
			this->setMemoryWord(this->getAc(), endereco);
		} else {
			this->setMemoryWord(this->getAc(), this->getMemoryWord(endereco));
			this->setEnderecamentoIndireto(false);
		}
		this->incrementaPc();
		break;
		// subroutine call
	case 0xA:
		if (!this->getEnderecamentoIndireto()) {
			//memoria[endereco] = pc + 0x10;
			this->setMemoryWord(this->getPc() + 0x10, endereco);
			this->setPc(endereco + 0x10);
		} else {
			// memoria[ac] = pc + 0x10;
			this->setMemoryWord(this->getPc() + 0x10, this->getAc());
			this->setPc(this->getAc() + 0x10);
			this->setEnderecamentoIndireto(false);
		}
		break;
		// indirect addressing
	case 0xB:
		/**
		 * Instruções disponíveis: JP, JZ, JN, LV, +, -, *, /, LM, MM, SC
		 */
		this->setEnderecamentoIndireto(true);
		this->incrementaPc();
		break;
		// halt machine
	case 0xC:
		this->setPc(endereco);
		this->setHalt(true);
		break;
		// get data
	case 0xD:
		int input;
		cout << "Acumulador = ";
//        scanf("%hd", &ac);
		cin >> setbase(16) >> input;
		this->setAc(input);
		this->incrementaPc();
		break;
		// put data
	case 0xE:
		cout << endl << "********************" << endl;
//        printf("ACUMULADOR = %hd", ac);
		cout << "ACUMULADOR = " << hex << noshowbase << this->getAc();
		cout << endl << "********************" << endl;
		this->incrementaPc();
		break;
		//operating system call
	case 0xF:

		break;

	default:
		break;
	}
}

void Simulador::executaPrograma() {
	this->setHalt(false);
	while (!this->getHalt()) {
		int instrucao = this->getMemoryWord(this->getPc());

		if (this->step) {
			this->mostraDetalhes();
			cout << "Pressione ENTER para continuar" << endl;
			if (!DEBUGECLIPSE) {
				getchar();
			}
		}

		this->executaInstrucao(instrucao);

		if (DEBUG) {
			cout << "HALT = " << this->getHalt() << endl;
		}
	}
	this->mostraDetalhes();
}

void Simulador::exibeMemoria() {
	// Exibe toda a memória em words
	exibeMemoriaWords(0x0, 0x10);
}

void Simulador::exibeMemoriaWords(int endereco, int linhas) {
	cout
			<< "-----------------------------------------------------------------------------------"
			<< endl;
	cout
			<< "    00   10   20   30   40   50   60   70   80   90   a0   b0   c0   d0   e0   f0  "
			<< endl;
	for (int l = 0; l < linhas; l++) {
//        printf("%01x - ", ( endereco/0x100+l ));
		cout << hex << noshowbase << setfill('0') << setw(1)
				<< (endereco / 0x100 + l) << " - ";
		for (int i = 0; i < 0x10; i++) {
			cout << hex << noshowbase << setfill('0') << setw(4)
					<< this->getMemoryWord(endereco + 0x100 * l + 0x10 * i)
					<< " ";
		}
		cout << endl;
	}
	cout
			<< "-----------------------------------------------------------------------------------"
			<< endl;
}

void Simulador::incrementaPc() {
	this->pc += 0x10;
}

void Simulador::mostraDetalhes() {
	this->mostraRegistradores();
	this->exibeMemoria();
}

void Simulador::mostraRegistradores() {
	cout << "-----------------------" << endl;
	cout << "   Acumulador   = " << hex << noshowbase << setfill('0') << setw(4)
			<< right << this->getAc();
	cout << endl;
//    printf("Program Counter = %04h" PRIx16, pc);
	cout << "Program Counter = " << hex << noshowbase << setfill('0') << setw(4)
			<< right << this->getPc();
	cout << endl;
	cout << "-----------------------" << endl;
}

void Simulador::resetMemoria() {
	this->memory->reset();
}

void Simulador::resetRegistradores() {
	this->ac = 0x0;
	this->pc = 0x0;
}

// Getter e Setters
int Simulador::getAc() {
	return this->ac;
}

void Simulador::setAc(int ac) {
	this->ac = ac;
}

int Simulador::getPc() {
	return this->pc;
}

void Simulador::setPc(int pc) {
	this->pc = pc;
}

bool Simulador::getEnderecamentoIndireto() {
	return this->enderecamentoIndireto;
}

void Simulador::setEnderecamentoIndireto(bool enderecamentoIndireto) {
	this->enderecamentoIndireto = enderecamentoIndireto;
}

bool Simulador::getHalt() {
	return this->halt;
}

void Simulador::setHalt(bool halt) {
	this->halt = halt;
}

bool Simulador::getStep() {
	return this->step;
}

string Simulador::getStepString() {
	if (this->step) {
		return "ATIVADO";
	} else {
		return "DESATIVADO";
	}
}

void Simulador::setStep(bool step) {
	this->step = step;
}

void Simulador::toggleStep() {
	this->step = !this->step;
}

int Simulador::getMemoryWord(int endereco) {
	return memory->getWord(endereco);
}

void Simulador::setMemoryWord(int dado, int endereco) {
	this->memory->setWord(dado, endereco);
}

} /* namespace std */
