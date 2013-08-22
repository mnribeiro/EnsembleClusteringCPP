/*
 * prototipacao.h
 *
 *  Created on: 14/12/2009
 *      Author: Marcelo
 */

#ifndef PROTOTIPACAO_H_
#define PROTOTIPACAO_H_

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "nuvens.h"

class prototipacao {

	boost::numeric::ublas::matrix<double> Similaridades;
	vector<document> In, *Out;
	vector<unsigned> mapeamento;

	void apagarDocumento (const unsigned&);
	unsigned getMenorSimilaridade (const unsigned&, list<unsigned>&);
	document getCentroide (const unsigned&, const unsigned&);
	void calcularSimilaridades ();
	unsigned getDocMaiorDensidade ();

public:

	prototipacao();
	prototipacao(vector<document>*);
	virtual ~prototipacao();

	void realizarPrototipacao(const double&, const unsigned&);
};

#endif /* PROTOTIPACAO_H_ */
