#ifndef AVALIACAO_H_
#define AVALIACAO_H_

#include "nuvens.h"

class avaliacao
{
	vector<document>* valores;
	map<unsigned, unsigned> RepresentantesGC;

	unsigned majoritario (list<unsigned>&);

	double alpha (const unsigned& c); // documentos corretamente afetados para classe c
	double beta (const unsigned& c); // documentos incorretamente afetados para classe c
	double gamma (const unsigned& c); // documentos incorretamente não-afetados para classe c

	double alphaTotal ();
	double betaTotal ();
	double gammaTotal ();

public:

	void calcularRepresentantes();
	avaliacao();
	avaliacao(vector<document>*);
	virtual ~avaliacao();

	double similaridadeGeralPorGrupo (document&);
	double similaridadeGeral ();
	void inserirInferidasTreeSOM(const char*);
	document getCentroide (const int&);
	void imprimirRepresentantes();
	double getPrecisao();
	double getCobertura();
};

#endif /*AVALIACAO_H_*/
