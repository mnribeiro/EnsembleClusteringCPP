#ifndef NUVENS_H_
#define NUVENS_H_

#include "../calculos/calculos.h"
#include "../calculos/tree.hh"
#include "featureselection.h"
#include <map>
#include <queue>
#include <stack>
//#include <omp.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#define METODO_TFV 1
#define METODO_DF 2
#define METODO_MEANTFIDF 3
#define METODO_ENTROPIA 4

class nuvens
{
	vector<document>* valores;
	vector<document> docs_saida;
	vector<int> mapeamento;
	vector<string> termos;
	vector<list<unsigned> > Vetor_Leaves;
	map<unsigned, vector<bool> > features;
	map<unsigned, string> labels;
	unsigned nclusters;
	tree<unsigned> Hierarquia;
	tree<unsigned>::pre_order_iterator IterTree;

	map<unsigned, document> Centroides;
	map<unsigned, double> Adequacoes;

	double adequacao, adeqC1, adeqC2; // adequações atuais
	document c1, c2; // centróides atuais

	void classificar (const unsigned&, const unsigned&, const unsigned&);
	void classificarN (const unsigned&, const unsigned&, const unsigned&, const unsigned&);
	void gerarMatrizProjetada (const unsigned&);
	double gerarAleatorio ();
	double getAdequacao();
	void zerarCaracteristicas ();

	string getLabelParaCluster(const unsigned& cluster, int n);

public:

	nuvens();
	nuvens(vector<document>*);
	nuvens(vector<document>*, vector<string>&);
	nuvens(vector<document>*, const unsigned&, vector<bool>&);
	nuvens(vector<document>*, const unsigned&);
	virtual ~nuvens();

	map<unsigned, vector<bool> > getFeatures ();

	vector<int> getMapeamento();
	void setPertinenciaFeatures (vector<bool>);
	double getCriterioParaFeatures(vector<bool> features, vector<int> map);

	double getAdequacaoPorCluster(const unsigned&);
	double getAdequacaoGeral();
	unsigned getMaiorCluster();
	vector<document>* getDocuments();

	static double similaridade (document&, document&);
	static double desimilaridade (document&, document&);
	double similaridadeGeral (document);
	double getThreshold (const unsigned&);
	document getCentroide (const int&);

	bool isInCluster(const unsigned& cluster, document& d);

	static vector<document> gerarDocuments();
	static vector<string> gerarTermos();
	double getQuality (const unsigned& cluster, const unsigned& term);
	vector<document> getDocumentsPorCluster (const unsigned& cluster);
	vector<unsigned> getTermosDoCluster(const unsigned& cluster);
	unsigned getTamanhoDoCluster (const unsigned& cluster);

	void BiSectionKMeans (const unsigned& nclusters, const unsigned& iter, const bool&,
			const bool&, const bool&, const bool&, const bool&, const unsigned&, const int&);

	string getLabelParaClusterC(const unsigned& cluster);

	unsigned getNumDocsPorClasse (const unsigned& classe);

	tree<unsigned> getHierarquia();
	vector<list<unsigned> > getLeaves ();

	unsigned getNumdeGrupos ();

	string imprimirLabels ();
	void imprimirHierarquia ();
};

#endif /*NUVENS_H_*/
