/*
 * ensemble.h
 *
 *  Created on: 16/11/2009
 *      Author: Marcelo
 */

#ifndef ENSEMBLE_H_
#define ENSEMBLE_H_

#include "nuvens.h"
#include "avaliacao.h"
#include <vector>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#define SINGLE_LINK 1
#define COMPLETE_LINK 2
#define AVERAGE_LINK 3

struct grupo {
	bool eh_no;
	vector<unsigned> pontos;
};

class ensemble {

	unsigned num_docs;
	double adequacao;
	unsigned flag;

	list<vector<int> >* Particoes;
	boost::numeric::ublas::matrix<double> MConsenso;
	vector<int> Ensemble;

	double similaridade(grupo&, grupo&);
	void calcularConsenso ();

public:

	ensemble();
	ensemble(list<vector<int> >*);
	ensemble(vector<vector<int> > p);
	virtual ~ensemble();

	double nIJ(vector<int>, vector<int>, int, int);
	double nI(vector<int>, int);
	double randAjustado(vector<int>, vector<int>);
	double rand(vector<int>, vector<int>);

	double diversidadeDNP ();
	double diversidadeDP ();

	void addParticao (vector<int>);
	void putParticoes (vector<vector<int> > V);
	void gerarParticaoConsensual(const unsigned&, const unsigned&);
	void gerarParticoesSelecaoLocal (vector<document>*, const unsigned&, const unsigned&, bool);
	void gerarParticoesSelecaoAleatoria (vector<document>*, const unsigned&, const unsigned&);
	void gerarParticoesSelecaoLocalComAmostragem (vector<document>*, const unsigned&,
			const unsigned&, bool);

	vector<int> getEnsemble ();

	boost::numeric::ublas::matrix<double> getConsenso ();
};

#endif /* ENSEMBLE_H_ */
