/*
 * prototipacao_caracteristicas.h
 *
 *  Created on: 06/01/2010
 *      Author: Marcelo
 */

#ifndef PROTOTIPACAO_CARACTERISTICAS_H_
#define PROTOTIPACAO_CARACTERISTICAS_H_

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "nuvens.h"

struct feature {
	unsigned id;
	double densidade;
	vector<double> V;
	list<unsigned> representantes;
};

class prototipacao_caracteristicas {

	boost::numeric::ublas::matrix<double> Desimilaridades;
	vector<document> *In, *Out;
	vector<feature> features;
	vector<unsigned> mapeamento;

	unsigned getFeatureMaiorDensidade ();
	void apagarCaracteristica (const unsigned&);
	unsigned getMenorSimilaridade (const unsigned&, list<unsigned>&);
	feature getCentroide (const unsigned&, const unsigned&);
	void calcularDesimilaridades ();
	double desimilaridadeFeatures (const feature&, const feature&);

public:

	prototipacao_caracteristicas();
	prototipacao_caracteristicas(vector<document>*);
	virtual ~prototipacao_caracteristicas();

	void realizarPrototipacao(const double&, const unsigned&);
};

#endif /* PROTOTIPACAO_CARACTERISTICAS_H_ */
