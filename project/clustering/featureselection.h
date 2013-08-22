#ifndef FEATURESELECTION_H_
#define FEATURESELECTION_H_

#include "../calculos/calculos.h"
#include "nuvens.h"
#include <map>
#include <queue>
#include <stack>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

class featureselection
{
	vector<document>* valores;
	vector<bool> Selecionados;

	double Entropia;

	double getQualityTFV (const unsigned&, const unsigned&);
	double getQualityDF (const unsigned&, const unsigned&);
	double getQualityMeanTFIDF (const unsigned&, const unsigned&);
	double getQualityTS (const unsigned&, const unsigned&, const double&);
	double getQualityEn (const unsigned&, const unsigned&);
	bool isInCluster(const unsigned&, document&);
	vector<document> getDocumentsPorCluster (const unsigned&);

	double similaridade (document&, document&);

	void calcularEntropia(const unsigned&);

public:

	featureselection();
	featureselection(vector<document>*);
	virtual ~featureselection();

	list<unsigned> getTermosSelecionadosParaCluster(const unsigned&, int, const int&);
	void gerarValoresComTermosSelecionados(const unsigned&, const int&, const int&);
	void gerarValoresComTermosSelecionadosAleatoriamente(const unsigned&);
	void gerarValoresComTermosSelecionadosAleatoriamenteComPesos(const unsigned& n);
	map<unsigned, vector<bool> > gerarTodosMapaDeCaracteristicasPorGrupo (const unsigned&, const unsigned&, const unsigned&);
	map<unsigned, vector<bool> > gerarMapaDeCaracteristicasPorGrupo (const unsigned&, const unsigned&);

	void printSimilaridades ();
};

#endif /*FEATURESELECTION_H_*/
