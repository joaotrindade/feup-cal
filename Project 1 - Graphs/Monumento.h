/*
 * Monumento.h
 *
 *  Created on: 22 de Abr de 2013
 *      Author: ei11118
 */

#ifndef MONUMENTO_H_
#define MONUMENTO_H_

#include <string>
using namespace std;

/** Classe Representativa de cada monumento */
class Monumento{
	/** Nome do monumento */
	string nome;
public:
	Monumento(string n_nome);
	/** Altera o nome de um monumento */
	void setNome(string n_nome);
	/** Devolve o nome de um monumento */
	string getNome();
};



#endif /* MONUMENTO_H_ */
