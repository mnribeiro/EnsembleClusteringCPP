/*
 * ag.h
 *
 *  Created on: 17/10/2011
 *      Author: Marcelo Nunes
 */

#ifndef AG_H_
#define AG_H_

#include <vector>
#include <list>
#include "../calculos/calculos.h"
#include "../clustering/nuvens.h"
#include "../clustering/avaliacao.h"
#include "../clustering/ensemble.h"

struct Individuo {
	vector<bool> V;
	bool aptoCrossover;
};

class ag {

	vector<Individuo>* P;
	vector<Individuo>* newP;
	vector<double> Fitness;
	vector<vector<int> > Particoes;
	vector<vector<int> >* TParticoes;
	vector<int> Ensemble;
	vector<double> p_dos_ensembles;
	list<unsigned> Ordenados;
	list<unsigned> jahSelecionados;

	vector<document>* DOCS;

	void geracaoInicial (const unsigned& num_ind, const unsigned& tam_vetor);
	void geracaoInicialPro (const unsigned& num_ind, const unsigned& tam_vetor);
	Individuo getRandomIndividuo (const unsigned& tam, const double& prob);
	void gerarFitness ();
	void gerarFitnessPro ();
	void gerarFitnessPro2 (const double& lambda);
	void gerarParticoes ();
	void gerarEnsemble();
	unsigned melhorIndividuo (const unsigned& i, const unsigned& j); // FITNESS com CRITERIO
	bool jahOrdenado (const unsigned& t);
	unsigned getMelhorIndividuoTodas();
	bool jahSelecionado (const unsigned& t);

	void crossover (Individuo&, Individuo&);
	void crossoverAleatorioDosMelhores();
	Individuo mutacao (Individuo&, const double& tax_mut);

public:

	ag();
	virtual ~ag();
	ag(const unsigned& num_ind, vector<document>* docs);

	vector<Individuo>* getGeracaoAtual();
	void nextGeracao(const double& tax_mut, const unsigned& pais);
	void runNgeracoes (const unsigned& n, const double& tax_mut, const unsigned& pais);
	Individuo getMelhorIndividuo();
	void ordenarIndividuos ();
	void printIndividuos ();
	vector<vector<int> > getParticoes();
	vector<vector<int> >* getTodasParticoes();
	vector<int> getEnsemble();
	unsigned getQuantidadeFeatures (Individuo&);
	void gerarEXPERIMENTO (const double& lambda);

};

#endif /* AG_H_ */
