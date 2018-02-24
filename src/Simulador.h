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
	int getMemoryBit(int bitAddress);
	string getStepString();
	void setAc(int ac);
	void setEnderecamentoIndireto(bool enderecamentoIndireto);
	void setHalt(bool halt);
	void setPc(int pc);
	void setMemoryBit(int bit, int bitAddress);
	void setStep(bool step);
	void toggleStep();

	// Métodos públicos
	void carregaPrograma(string programa);
	void executaPrograma();

private:
	/**
	 * Tamanho da memoria
	 */
	const static int MEMSIZE = 0x1000;

	// Atributos
	bool enderecamentoIndireto;
	bool halt;
	bool step;
	int memory[MEMSIZE];
	int16_t ac; // accumulator
	int16_t pc; // program counter
	Montador *montador;

	// Métodos auxiliares
	int getMemoryWord (int endereco);
	void executaInstrucao (int instrucao);
	void exibeMemoria (char c);
	void exibeMemoriaBits (int endereco, int linhas);
	void exibeMemoriaWords (int endereco, int linhas);
	void incrementaPc();
	void setMemoryWord(int dado, int endereco);
	void mostraDetalhes();
	void mostraRegistradores();
	void resetMemoria();
	void resetRegistradores();
};

} /* namespace std */

#endif /* SIMULADOR_H_ */
