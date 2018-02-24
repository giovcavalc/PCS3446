/*
 * Memoria.h
 *
 *  Created on: 24 de fev de 2018
 *      Author: giovcavalc
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

namespace std {

class Memoria {
public:
	Memoria();
	virtual ~Memoria();

	void reset();

	/**
	 * Retorna um inteiro hexadecimal que representa a
	 * palavra contida na memória a partir do endereço dado
	 * @param address
	 */
	int getWord(int address);
	/**
	 * Salva a palavra correspondente ao valor inteiro
	 * dado na memória a partir do endereço dado
	 * @param word
	 * @param address
	 */
	void setWord(int word, int address);

private:
	static const int MEMSIZE = 0x1000;
	int memory[MEMSIZE];

	/**
	 * Retorna um inteiro hexadecimal que representa o
	 * byte contido na memória a partir do endereço dado
	 * @param address
	 */
	int getByte(int address);
	/**
	 * Salva o byte correspondente ao valor inteiro
	 * dado na memória a partir do endereço dado
	 * @param byte
	 * @param address
	 */
	void setByte(int byte, int address);

};

} /* namespace std */

#endif /* MEMORIA_H_ */
