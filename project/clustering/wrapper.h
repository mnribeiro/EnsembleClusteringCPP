/*
 * wrapper.h
 *
 *  Created on: 24/11/2009
 *      Author: Marcelo
 */

#ifndef WRAPPER_H_
#define WRAPPER_H_

#include "nuvens.h"
#include "avaliacao.h"
#include <vector>

class wrapper {

	vector<document>* docs;
	vector<string> termos;
	list<vector<unsigned> >* Particoes;
	vector<bool> selecionadas;
	vector<double> qualidades;
	list<unsigned> caract_recente_add, caract_recente_rem;
	unsigned num_total_features;

	unsigned selecionarMenosRelevanteDentroDoConjunto ();
	unsigned selecionarMaisRelevanteForaDoConjunto ();
	double valorCriterioComCaracteristicas (vector<bool>&, vector<int>&);
	double getQualidadeTermo (const unsigned&);
	unsigned numCaracteristicasSelecionadas ();
	bool comparaSubsetFeatures(vector<bool>&, vector<bool>&, const unsigned&);
	vector<int> getParticaoGeradaPorCaracteristicas (vector<bool>&, const unsigned&);
	bool ehCaract_Recente_Add (const unsigned&);
	bool ehCaract_Recente_Rem (const unsigned&);

public:

	wrapper();
	wrapper(vector<document>* L, vector<string>& V);
	virtual ~wrapper();

	void realizarWrapper (const unsigned&, const unsigned&);
	void imprimirCaracteristicasSelecionadas ();

};

#endif /* WRAPPER_H_ */
