/*
 * Simulador.h
 *
 *  Created on: 23 de fev de 2018
 *      Author: giovcavalc
 */

#ifndef SIMULADOR_H_
#define SIMULADOR_H_

#include <cstdint>
#include <string>

#include "Memoria.h"

namespace std {
class Montador;
} /* namespace std */

namespace std {

class Simulador {
public:
	Simulador();
	virtual ~Simulador();

	// Getter e Setters
	bool getEnderecamentoIndireto();
	bool getHalt();
	bool getStep();
	int getAc();
	int getPc();
	int getMemoryWord(int endereco);
	string getStepString();
	void setAc(int ac);
	void setEnderecamentoIndireto(bool enderecamentoIndireto);
	void setHalt(bool halt);
	void setPc(int pc);
	void setMemoryWord(int dado, int endereco);
	void setStep(bool step);
	void toggleStep();

	// Métodos públicos
	void carregaPrograma(string programa);
	void executaPrograma();

private:

	// Atributos
	bool enderecamentoIndireto;
	bool halt;
	bool step;
	int16_t ac; // accumulator
	int16_t pc; // program counter
	Memoria *memory;
	Montador *montador;

	// Métodos auxiliares
	void executaInstrucao(int instrucao);
	void exibeMemoria();
	void exibeMemoriaWords(int endereco, int linhas);
	void incrementaPc();
	void mostraDetalhes();
	void mostraRegistradores();
	void resetMemoria();
	void resetRegistradores();
};

} /* namespace std */

#endif /* SIMULADOR_H_ */
