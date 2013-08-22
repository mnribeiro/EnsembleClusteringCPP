// BSK-Ensembles.cpp : Defines the entry point for the console application.
//

#include "../clustering/nuvens.h"
#include "../clustering/avaliacao.h"
#include "../representacao/representacao.h"
#include "../clustering/estabilidade.h"
#include "../clustering/ensemble.h"
#include "../clustering/wrapper.h"
#include "../ag/ag.h"
#include <boost/tr1/random.hpp>
//#include <ctime>

using namespace std::tr1;

boost::variate_generator<boost::mt19937, uniform_real<> > r(boost::mt19937(24), uniform_real<>(0,1));

int main(int argc, char* argv[]) {
/*
	representacao r;
	r.criarArquivo(40);
*/

	cout << "Carregando documentos...." << endl;
	vector<document> semmexer = nuvens::gerarDocuments();
	cout << semmexer.size() << " documentos carregados com " << semmexer[0].tf_idf.size() << " termos." << endl;

	for (unsigned i = 0; i < semmexer.size(); i++) {
		semmexer[i].inferidas.push_back(0);
		semmexer[i].inferida = 0;
	}

	double mut = 0.05;
	unsigned pais = 2;
	for (unsigned i = 0; i < 30; i++) {
		ag* A = new ag(100, &semmexer);
		cout << "Experimento "<< i+1 << endl;
		cout << "Taxa de mutação de " << mut << " e num de pais " << pais << endl;
		A->runNgeracoes(1, mut, pais);
		A->gerarEXPERIMENTO(1);
		delete A;
	}

/*
METODO_TFV METODO_DF METODO_MEANTFIDF METODO_ENTROPIA
cortes | ITER | label | localizada | global | localizada-auto | termos selecionados | Método
*/
/*
	vector<document>* docs = new (vector<document>)(semmexer);
	avaliacao A(docs);
	A.inserirInferidasTreeSOM("./map.cod.0.28862.list");
	A.calcularRepresentantes();
	cout << A.getPrecisao() << endl;
	cout << A.similaridadeGeral();
	delete docs;
*/
//	list<vector<unsigned> >* particoes = new (list<vector<unsigned> >)();
//	vector<string> V = nuvens::gerarTermos();
//	vector<document>* docs = new (vector<document>)(semmexer);
/*	wrapper w (docs, V);
	w.realizarWrapper(600, 40);
	w.imprimirCaracteristicasSelecionadas();
*/ /*
	ofstream execucao("./execucao.txt", ios::out);

//	double soma;
//	soma = 0;
	vector<unsigned> aux;
	for (unsigned i = 0; i < 5; i++) {
		vector<document>* docs = new (vector<document>)(semmexer);
		nuvens* n = new nuvens(docs, V);
		n->BiSectionKMeans(39,5,false,false,true,false,200,METODO_TFV);
		aux = n->getMapeamento();
		execucao << "Iteração " << i <<  endl;
		for (unsigned j = 0; j < aux.size(); j++)
			execucao << "doc " << j << " grupo " << aux[j] << endl;
		execucao << endl;
		particoes->push_back(aux);
		avaliacao A(docs);
//		soma += A.getPrecisao();
		cout << A.getPrecisao() << endl;
//		cout << A.similaridadeGeral() << endl;
		delete n;
		delete docs;
	}
*/ /*
	vector<document>* docs = new (vector<document>)(semmexer);

	unsigned t = 1311;

	while (t > 0) {
		nuvens* n = new nuvens(docs);
		n->BiSectionKMeans(39,5,false,false,false,false,true,10,METODO_TFV);
		avaliacao* A = new avaliacao(docs);
		cout << t << " : " << A->getPrecisao() << endl;
		t -= 50;
		delete A;
		delete n;
	}
*/
	/*
	double media = 0;
	time_t sec = time(NULL);
	for (unsigned i = 0; i < 1; i++) {
		cout << "Ensemble " << i+1 << endl;
		ensemble* E = new ensemble();
		E->gerarParticoesSelecaoLocalComAmostragem(docs,200,80,false);
//		E->gerarParticoesSelecaoAleatoria(docs, 200, 10);
		E->gerarParticaoConsensual(40, AVERAGE_LINK);
		vector<int> aux = E->getEnsemble();
//		cout << "Diversidade final DP e DNP: " << E->diversidadeDP() << " e " <<
//				E->diversidadeDNP() << endl;
		cout << "E o ensemble! " << endl;
		time_t sec2 = time(NULL);
		cout << "Tantos segundos" << sec2 - sec << endl;
		for (unsigned i = 0; i < aux.size(); i++) {
			(*docs)[i].inferida = aux[i];
		}
		avaliacao A(docs);
		cout << A.getPrecisao() << endl;
		media += A.getPrecisao();
		delete E;
	}
	cout << "Resultado final " << media/5 << endl;
	delete docs;

	execucao << "	" << soma/30 << endl;
	cout << endl;
	execucao.close();
*/
/*
	estabilidade Est(docs,V,0.8,100);
	Est.inferirGrupos();
	Est.imprimirHierarquia();
	cout << docs->size();
	for (unsigned i = 0; i < docs->size(); i++) {
		execucao << "Doc " << (*docs)[i].id << " de classe " << (*docs)[i].classe << " ficou nos clusters ";
		for (list<unsigned>::iterator It = (*docs)[i].inferidas.begin(); It != (*docs)[i].inferidas.end(); It++)
			execucao << *It << " ";
		execucao << endl;
	}

	execucao << "Labels:" << endl;
	nuvens* n = Est.getAgrupamento();
	execucao << n->imprimirLabels();
	execucao << endl;

	L = n1.getDocuments();
	L1 =  n2.getDocuments();
	for (unsigned i = 0; i < L.size(); i++) {
		execucao << "Doc " << L[i].id << " de classe " << L[i].classe << " ficou nos clusters ";
		for (list<unsigned>::iterator It = L[i].inferidas.begin(); It != L[i].inferidas.end(); It++)
			execucao << *It << " ";
		execucao << endl;
	}

	execucao << endl;

	if (label) {
		execucao << "Labels:" << endl;
		execucao << n1.imprimirLabels();

		execucao << endl;
	}

	execucao << "Similaridade geral: " << n1.getAdequacaoGeral() << endl;

	avaliacao A(L);

	execucao << "Precisão: " << A.getPrecisao() << endl;

	for (unsigned i = 0; i < L1.size(); i++) {
		execucao << "Doc " << L1[i].id << " de classe " << L1[i].classe << " ficou nos clusters ";
		for (list<unsigned>::iterator It = L1[i].inferidas.begin(); It != L1[i].inferidas.end(); It++)
			execucao << *It << " ";
		execucao << endl;
	}

	execucao << endl;

	if (label) {
		execucao << "Labels:" << endl;
		execucao << n1.imprimirLabels();

		execucao << endl;
	}

	execucao << "Similaridade geral: " << n2.getAdequacaoGeral() << endl;

	avaliacao B(L1);

	execucao << "Precisão: " << B.getPrecisao() << endl;
*/
	return 0;
}
