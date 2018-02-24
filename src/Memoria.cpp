/*
 * Memoria.cpp
 *
 *  Created on: 24 de fev de 2018
 *      Author: giovcavalc
 */

#include "Memoria.h"

#include <cmath>

#include "MVNException.h"

namespace std {

Memoria::Memoria() {
	this->reset();
}

Memoria::~Memoria() {
	// TODO Auto-generated destructor stub
}

void Memoria::reset() {
	for (int i = 0; i < this->MEMSIZE; i++) {
		this->memory[i] = 0;
	}
}

int Memoria::getByte(int address) {
	const int BYTE_SIZE = 8;

	if (address < 0) {
		throw new MVNException("Memoria::getByte - N�o s�o suportados valores negativos para a mem�ria");
	} else if (address % BYTE_SIZE != 0) {
		throw new MVNException("Memoria::getByte - O endere�o n�o indica o in�cio de um byte");
	}

	int byte = 0;
	for (int i = 0; i < BYTE_SIZE; i++) {
		byte += pow(2,i) * this->memory[address + i];
	}
	return byte;

}

void Memoria::setByte(int byte, int address) {
	const int BYTE_SIZE = 8;

	if (byte < 0 || address < 0) {
		throw new MVNException("Memoria::setByte - N�o s�o suportados valores negativos para a mem�ria");
	} else if (address % BYTE_SIZE != 0) {
		throw new MVNException("Memoria::setByte - O endere�o n�o indica o in�cio de um byte");
	}

	int byteTemp = byte;
	for (int i = 0; i < BYTE_SIZE; i++) {
		memory[address + i] = byteTemp % 2;
		byteTemp = byteTemp / 2;
	}

	if (byteTemp > 0) {
		throw new MVNException("Memoria::setByte - O inteiro fornecido para um byte � maior do que o suportado");
	}

}

int Memoria::getWord(int address) {
	const int WORD_SIZE = 16;
	if (address < 0) {
		throw new MVNException("Memoria::getWord - N�o s�o suportados valores negativos para a mem�ria");
	} else if (address % WORD_SIZE != 0) {
		throw new MVNException("Memoria::getWord - O endere�o n�o indica o in�cio de um byte");
	}

	int word = 0;
	for (int i = 0; i < WORD_SIZE; i++) {
		word += pow(2,i) * this->memory[address + i];
	}
	return word;
}

void Memoria::setWord(int word, int address) {
	const int WORD_SIZE = 16;

	if (word < 0 || address < 0) {
		throw new MVNException("Memoria::setWord - N�o s�o suportados valores negativos para a mem�ria");
	} else if (address % 16 != 0) {
		throw new MVNException("Memoria::setWord - O endere�o n�o indica o in�cio de uma palavra");
	}

	int wordTemp = word;
	for (int i = 0; i < WORD_SIZE; i++) {
		memory[address + i] = wordTemp % 2;
		wordTemp = wordTemp / 2;
	}

	if (wordTemp > 0) {
		throw new MVNException("Memoria::setWord - O inteiro fornecido para uma palavra � maior do que o suportado");
	}

}

} /* namespace std */
