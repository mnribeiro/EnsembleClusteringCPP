#ifndef ESTABILIDADE_H_
#define ESTABILIDADE_H_

#include "nuvens.h"

struct cotovelo {
	unsigned vezes_indicado;
	double intra_distancia;
};

class estabilidade
{
	vector<document>* Original;
	vector<document>* Amostra;
	tree<unsigned> hier_ori, hier_amo;
	tree<unsigned> hier_final;
	unsigned num_grupos, granularidade;
	vector<list<unsigned> > LeavesOR, LeavesAM;
	list<unsigned> FolhasFinais;

	nuvens *agru_ori, *agru_amo;

	void amostragem(double porc);
	bool alreadyInAmostra(document& d);
	bool isInCluster(const unsigned&, document&);
	bool isInOneOfClusters(list<unsigned>&, document&);
	bool isSonOf(const unsigned&, const unsigned&);
	bool isFolhaFinal(const unsigned&);
	unsigned isOfCluster (list<unsigned>&, document&);
	unsigned getIndVetorPeloIDOri (const unsigned&);
	unsigned getIndVetorPeloIDAmo (const unsigned&);
	vector<list<unsigned> > getFolhasFilhosDe (const unsigned&);
	unsigned retornarIndiceEscolhido (vector<double>&);

public:

	estabilidade();
	estabilidade(vector<document>*, vector<string>&, double, double);
	virtual ~estabilidade();

	nuvens* getAgrupamento ();

	void inferirGrupos ();
	vector<document>* getAmostra ();
	vector<document>* getDocuments ();
	double getValorIndice (list<unsigned>&, list<unsigned>&);
	vector<unsigned> getIndicesDaInter (list<unsigned>&, list<unsigned>&);
	void imprimirHierarquia();
};

#endif /*ESTABILIDADE_H_*/
