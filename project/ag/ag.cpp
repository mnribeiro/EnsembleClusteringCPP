/*
 * ag.cpp
 *
 *  Created on: 17/10/2011
 *      Author: Marcelo Nunes
 */

#include "ag.h"

ag::ag() {
	// TODO Auto-generated constructor stub
}

ag::ag(const unsigned& num_ind, vector<document>* docs) {
//	this->geracaoInicial(num_ind, (*docs)[0].tf_idf.size());
	this->geracaoInicialPro(num_ind, (*docs)[0].tf_idf.size());
	this->DOCS = docs;
	this->Particoes.resize(num_ind);
}

void ag::geracaoInicial (const unsigned& num_ind, const unsigned& tam_vetor) {
	this->P = new vector<Individuo> ();
	for (unsigned i = 0; i < num_ind; i++)
		this->P->push_back(this->getRandomIndividuo(tam_vetor, 0.05));
}

void ag::geracaoInicialPro (const unsigned& num_ind, const unsigned& tam_vetor) {
	this->P = new vector<Individuo> ();
	unsigned criados = 0, aux = 10;
	while (criados < num_ind) {
		aux = aux % 100;
		if (aux == 0) {
			aux += 10;
			continue;
		}
		this->P->push_back(this->getRandomIndividuo(tam_vetor, double(aux)/100.0));
		aux += 10;
		criados++;
	}
}

Individuo ag::getRandomIndividuo (const unsigned& tam, const double& prob) {
	vector<bool> V;
	V.resize(tam);
	calculos C;

	for (unsigned i = 0; i < V.size(); i++) {
		if (C.random() < prob) {
			V[i] = true;
		} else {
			V[i] = false;
		}
	}
	Individuo q;
	q.V = V;
	return q;
}

unsigned ag::melhorIndividuo (const unsigned& i, const unsigned& j) // USO do CRITERIO
{
	nuvens* n = new nuvens(this->DOCS);
	double cI = 0, cJ = 0;
	cI = n->getCriterioParaFeatures((*this->P)[i].V, this->Particoes[i]) *
			n->getCriterioParaFeatures((*this->P)[j].V, this->Particoes[i]);

	cJ = n->getCriterioParaFeatures((*this->P)[i].V, this->Particoes[j]) *
			n->getCriterioParaFeatures((*this->P)[j].V, this->Particoes[j]);

	delete n;
	//	cout << cI << " " << cJ << endl;
	if (cI > cJ) {
		return i;
	} else {
		return j;
	}
}

void ag::gerarParticoes () {
	cout << "EXTERNO, INTERNO e N de FEATURES" << endl;
	for (unsigned i = 0; i < this->P->size();i++) {     // GERAR TODAS AS PARTICOES com SUBCONJUNTOS DE FEATURES
//		nuvens* n = new nuvens(this->DOCS, 0, (*this->P)[i].V);
		vector<document>* p = new vector<document>(*this->DOCS);
		nuvens* n = new nuvens(this->DOCS, 0, (*this->P)[i].V);
//		nuvens* n = new nuvens(p, 10);      // RANDOM PROJECTIONS
		this->Particoes[i] = n->getMapeamento();
		vector<int> aux = n->getMapeamento();

		for (unsigned j = 0; j < aux.size(); j++) {
			(*this->DOCS)[j].inferida = aux[j];
		}
		avaliacao* A = new avaliacao(this->DOCS);
		cout << i << " " << A->getPrecisao() << " "
				<< n->getCriterioParaFeatures((*this->P)[i].V, this->Particoes[i]) << " " <<
				this->getQuantidadeFeatures((*this->P)[i]) << endl;
		delete n;
		delete A;
		delete p;
	}
}

unsigned ag::getQuantidadeFeatures (Individuo& I) {
	unsigned cont = 0;
	for (unsigned i = 0; i < I.V.size(); i++)
		if (I.V[i] == true)
			cont++;
	return cont;
}

vector<int> ag::getEnsemble () {
	return this->Ensemble;
}

void ag::gerarEnsemble() {
//	ensemble* E = new ensemble(this->Particoes);
	ensemble* E = new ensemble(*this->TParticoes);
	E->gerarParticaoConsensual(40, AVERAGE_LINK);

	this->Ensemble = E->getEnsemble();
	for (unsigned j = 0; j < this->Ensemble.size(); j++)
		(*this->DOCS)[j].inferida = this->Ensemble[j];

	avaliacao* A = new avaliacao(this->DOCS);
	this->p_dos_ensembles.push_back(A->getPrecisao());
	cout << "PRECISAO DO ENSEMBLE " << A->getPrecisao() << endl;
	delete E;
	delete A;
}

void ag::gerarFitness () {
	unsigned num_ind = this->P->size();
	this->Fitness.resize(num_ind); // gerar votacao zerada
	for (unsigned i = 0; i < this->Fitness.size(); i++)
		this->Fitness[i] = 0;
	for (unsigned i = 0; i < num_ind; i++)
	    for (unsigned j = 0; j < i; j++)
	    	this->Fitness[this->melhorIndividuo(i,j)]++; // votacao para calcular o valor de fitness
}

void ag::gerarFitnessPro () {
	unsigned num_ind = this->P->size();
	ensemble aux;
	this->Fitness.resize(num_ind); // gerar votacao zerada
	for (unsigned i = 0; i < this->Fitness.size(); i++)
		this->Fitness[i] = 0;
	for (unsigned i = 0; i < num_ind; i++)
    	this->Fitness[i] = aux.rand(Ensemble,this->Particoes[i]); // calcular o valor de fitness
}

void ag::gerarFitnessPro2 (const double& lambda) {
	vector<double> D, Q;
	D.resize(this->P->size());
	Q.resize(this->P->size());
	ensemble aux;
	this->Fitness.resize(this->P->size()); // gerar votacao zerada
	for (unsigned i = 0; i < this->Fitness.size(); i++)
		this->Fitness[i] = 0;
	double max_d = 0, aux_d;
	for (unsigned i = 0; i < D.size(); i++)  {
		aux_d = 0;
		for (unsigned j = 0; j < D.size(); j++)
			if (j != i)
				aux_d += aux.rand(this->Particoes[i],this->Particoes[j]);
		aux_d = 1.0 - (aux_d / (double(D.size()) - 1.0));
		if (aux_d > max_d)
			max_d = aux_d;
		D[i] = aux_d;
	}
	double max_q = 0;
	for (unsigned i = 0; i < Q.size(); i++)  {
		Q[i] = aux.rand(this->Ensemble,this->Particoes[i]);
		if (Q[i] > max_q)
			max_q = Q[i];
	}
	for (unsigned i = 0; i < this->P->size(); i++)
		this->Fitness[i] = (1 - lambda) * (Q[i] / max_q) + lambda * (D[i] / max_d);
}

void ag::gerarEXPERIMENTO (const double& lambda) {
	vector<double> D, Q;
	D.resize(this->TParticoes->size());
	Q.resize(this->TParticoes->size());
	ensemble aux;
	this->Fitness.resize(this->TParticoes->size());
	for (unsigned i = 0; i < this->Fitness.size(); i++)
		this->Fitness[i] = 0;
	double max_d = 0, aux_d;
	for (unsigned i = 0; i < D.size(); i++)  {
		aux_d = 0;
		for (unsigned j = 0; j < D.size(); j++)
			if (j != i)
				aux_d += aux.rand((*this->TParticoes)[i],(*this->TParticoes)[j]);
		aux_d = 1.0 - (aux_d / (double(D.size()) - 1.0));
		if (aux_d > max_d)
			max_d = aux_d;
		D[i] = aux_d;
	}
	double max_q = 0;
	for (unsigned i = 0; i < Q.size(); i++)  {
		Q[i] = aux.rand(this->Ensemble,(*this->TParticoes)[i]);
		if (Q[i] > max_q)
			max_q = Q[i];
	}
	for (unsigned i = 0; i < this->TParticoes->size(); i++)
		this->Fitness[i] = (1 - lambda) * (Q[i] / max_q) + lambda * (D[i] / max_d);

	//imprimir qualidade, diversidade e fitness

	cout << "Qualidade, diversidade e fitness por particao" << endl;
	for (unsigned i = 0; i < this->TParticoes->size(); i++)
		cout << i << " " << Q[i] << " " << D[i] << " " << this->Fitness[i] << endl;
	cout << endl;
/*
	//imprimir ensembles a cada passo
	cout << "Criterio externo do ensemble por geracao cumulativo" << endl;
	for (unsigned i = 0; i < this->p_dos_ensembles.size(); i++)
		cout << i << " " << this->p_dos_ensembles[i] << endl;
	cout << endl;
*/
	vector<vector<int> >* p_local;
	vector<int> En;

	cout << "Criterio externo aleatoriamente" << endl;
	calculos C;
	unsigned au;
	for (unsigned i = 10; i <= 100; i+=10) {
		p_local = new vector<vector<int> >();
		while (p_local->size() < i) {
			au = C.random(0,this->TParticoes->size());
			if (!this->jahSelecionado(au)) {
				p_local->push_back((*this->TParticoes)[au]);
				this->jahSelecionados.push_back(au);
			}
		}
		this->jahSelecionados.clear();
		ensemble* E = new ensemble(*p_local);
		E->gerarParticaoConsensual(40, AVERAGE_LINK);
		En = E->getEnsemble();
		for (unsigned j = 0; j < En.size(); j++)
			(*this->DOCS)[j].inferida = En[j];

		avaliacao* A = new avaliacao(this->DOCS);
		cout << i << " " << A->getPrecisao() << endl;
		delete E;
		delete A;
		delete p_local;
	}
	cout << endl;
/*
	cout << "Criterio externo do ensemble por geracao" << endl;
	for (unsigned i = 0; i < this->TParticoes->size(); i++) {
		p_local.push_back((*this->TParticoes)[i]);
		if ((i + 1) % 10 == 0) {
			ensemble* E = new ensemble(p_local);
			E->gerarParticaoConsensual(40, AVERAGE_LINK);
			En = E->getEnsemble();
			for (unsigned j = 0; j < En.size(); j++)
				(*this->DOCS)[j].inferida = En[j];

			avaliacao* A = new avaliacao(this->DOCS);
			cout << i << " " << A->getPrecisao() << endl;
			delete E;
			delete A;
			p_local.clear();
		}
	}
	cout << endl;
*/
    // passos para ordenar todas as particoes
	vector<vector<int> >* POrde = new vector<vector<int> >();
	POrde->resize(this->TParticoes->size());
	this->Ordenados.clear();
	for (unsigned i = 0; i < POrde->size(); i++)
		(*POrde)[i] = (*this->TParticoes)[this->getMelhorIndividuoTodas()];

	vector<vector<int> > p_local2;

	cout << "Ensembles com porcentagem dos melhores" << endl;
	for (unsigned i = 0; i < POrde->size(); i++) {
		p_local2.push_back((*POrde)[i]);
		if ((i + 1) % 10 == 0) {
			ensemble* E = new ensemble(p_local2);
			E->gerarParticaoConsensual(40, AVERAGE_LINK);
			En = E->getEnsemble();
			for (unsigned j = 0; j < En.size(); j++)
				(*this->DOCS)[j].inferida = En[j];

			avaliacao* A = new avaliacao(this->DOCS);
			cout << i + 1 << " " << A->getPrecisao() << endl;
			delete E;
			delete A;
		}
	}
	delete POrde;
//	delete this->TParticoes;
	cout << endl;
}

void ag::runNgeracoes (const unsigned& n, const double& tax_mut, const unsigned& pais) {
	this->TParticoes = new vector<vector<int> >();
	for (unsigned i = 0; i < n; i++) {
		cout << "Geração " << i+1 << endl << endl;
		this->gerarParticoes();
		for (unsigned j = 0; j < this->Particoes.size(); j++)
			this->TParticoes->push_back(this->Particoes[j]);
//		this->gerarFitness();
		this->gerarEnsemble();
//		this->gerarFitnessPro();
//		this->gerarFitnessPro2(0.5); // DESCOMENTAR

//		this->printIndividuos();

//		this->nextGeracao(tax_mut, pais); // DESCOMENTAR
	}
//	delete this->P;
//	delete this->newP;
}

vector<Individuo>* ag::getGeracaoAtual() {
	return this->P;
}

vector<vector<int> > ag::getParticoes() {
	return this->Particoes;
}

vector<vector<int> >* ag::getTodasParticoes() {
	return this->TParticoes;
}

void ag::nextGeracao(const double& tax_mut, const unsigned& pais) {
	this->newP = new vector<Individuo>();

	for (unsigned i = 0; i < this->Fitness.size(); i++)
		cout << i << " " << this->Fitness[i] << endl;
	cout << endl;

	unsigned popu = this->P->size();

	for (unsigned i = 0; i < this->P->size(); i++) // tag de crossover
		(*this->P)[i].aptoCrossover = true;

	this->ordenarIndividuos();

	for(unsigned i = 0; i < pais; i++)  // selecao dos pais e tb sobreviventes
		this->newP->push_back((*this->P)[i]);

	while (this->newP->size() < popu) // recombinação
		this->crossoverAleatorioDosMelhores();

	for (unsigned i = 0; i < this->newP->size(); i++) // realiza mutações
		(*this->newP)[i] = this->mutacao((*this->newP)[i], tax_mut);
	while (this->newP->size() > popu)
		this->newP->erase(this->newP->end());
	delete this->P;
	this->P = this->newP;
}

Individuo ag::getMelhorIndividuo() {
	double teste = 0;
	unsigned melhor = 0;
	for (unsigned i = 0; i < this->Fitness.size(); i++) {
		if (this->Fitness[i] > teste && !(this->jahOrdenado(i))) {
			melhor = i;
			teste = this->Fitness[i];
		}
	}
	this->Ordenados.push_back(melhor);
	return (*this->P)[melhor];
}

bool ag::jahOrdenado (const unsigned& t) {
	for (list<unsigned>::iterator It = this->Ordenados.begin(); It != this->Ordenados.end(); It++)
		if (*It == t)
			return true;
	return false;
}

bool ag::jahSelecionado (const unsigned& t) {
	for (list<unsigned>::iterator It = this->jahSelecionados.begin(); It != this->jahSelecionados.end(); It++)
		if (*It == t)
			return true;
	return false;
}

unsigned ag::getMelhorIndividuoTodas() {
	double teste = 0;
	unsigned melhor = 0;
	for (unsigned i = 0; i < this->Fitness.size(); i++) {
		if (this->Fitness[i] > teste && !(this->jahOrdenado(i))) {
			melhor = i;
			teste = this->Fitness[i];
		}
	}
	this->Ordenados.push_back(melhor);
	return melhor;
}

void ag::ordenarIndividuos() {
	vector<Individuo>* aux = new vector<Individuo>(this->P->size());
	this->Ordenados.clear();
	for (unsigned i = 0; i < aux->size(); i++)
		(*aux)[i] = this->getMelhorIndividuo();
	delete this->P;
	this->P = aux;
}

void ag::printIndividuos () {
	for (unsigned i = 0; i < this->P->size(); i++) {
		for (unsigned j = 0; j < (*this->P)[i].V.size(); j++)
			cout << (*this->P)[i].V[j] << " ";
		cout << endl;
	}
}

void ag::crossoverAleatorioDosMelhores() {
	unsigned a, b;
	calculos C;
	a = C.random(0,this->P->size());
	b = a;
	while (b == a)
		b = C.random(0,this->P->size());
//	cout << "Crossover de " << a << " e " << b;
	this->crossover((*this->P)[a],(*this->P)[b]);
}

void ag::crossover (Individuo& A, Individuo& B) {
	Individuo newA, newB;
	vector<bool> newVA, newVB;
	newVA.resize(A.V.size());
	newVB.resize(B.V.size());
	calculos C;
	unsigned corte = C.random(1, A.V.size());
//	cout << " no ponto " << corte << endl;
	for (unsigned i = 0; i < A.V.size(); i++) {
		if (i <= corte) {
			newVA[i] = A.V[i];
			newVB[i] = B.V[i];
		} else {
			newVB[i] = A.V[i];
			newVA[i] = B.V[i];
		}
	}
	newA.V = newVA;
	newB.V = newVB;
	this->newP->push_back(newA);
	this->newP->push_back(newB);
}

Individuo ag::mutacao (Individuo& A, const double& tax_mut) {
	calculos C;
	for (unsigned i = 0; i < A.V.size(); i++) {
		if (C.random() < tax_mut) {
//			cout << "Mutação do gene " << i << endl;
			if (A.V[i] == true) {
				A.V[i] = false;
			} else {
				A.V[i] = true;
			}
		}
	}
	return A;
}

ag::~ag() {
	// TODO Auto-generated destructor stub
//	delete this->newP;
//	delete this->P;
}
