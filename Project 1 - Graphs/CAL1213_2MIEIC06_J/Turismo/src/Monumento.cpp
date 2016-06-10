/*
 * Monumento.cpp
 *
 *  Created on: 22 de Abr de 2013
 *      Author: ei11118
 */
#include "Monumento.h"

Monumento::Monumento(string n_nome)
{
	nome = n_nome;
}

void Monumento::setNome(string n_nome)
{
	nome = n_nome;
}

string Monumento::getNome()
{
	return nome;
}


