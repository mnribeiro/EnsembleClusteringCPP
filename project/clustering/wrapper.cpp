/*
 * wrapper.cpp
 *
 *  Created on: 24/11/2009
 *      Author: Marcelo
 */

#include "wrapper.h"

wrapper::wrapper() {
	// TODO Auto-generated constructor stub
}

wrapper::~wrapper() {
	// TODO Auto-generated destructor stub
}

wrapper::wrapper(vector<document>* L, vector<string>& V)
{
	this->docs = L;
	this->termos = V;
	this->num_total_features = V.size();
	this->selecionadas.resize(this->num_total_features);
	this->qualidades.resize(this->num_total_features);
	for (unsigned i = 0; i < this->qualidades.size(); i++)
		this->qualidades[i] = this->getQualidadeTermo(i);
}

double wrapper::getQualidadeTermo (const unsigned& term)
{
	double t1 = 0, t2 = 0, size = 0, t3;
	for (unsigned i = 0; i < this->docs->size(); i++) {
		t1 += pow((*this->docs)[i].tf[term], 2);
		t2 += (*this->docs)[i].tf[term];
		size++;
	}
	t3 = (pow(t2,2))/size;
	return t1 - t3;
}

void wrapper::realizarWrapper (const unsigned& num_termos, const unsigned& K)
{
	vector<bool> aux;
	unsigned a = 0, b = 0, c = 0;
	for (unsigned i = 0; i < this->selecionadas.size(); i++)
		this->selecionadas[i] = true;

	for (unsigned i = 0; i < this->docs->size(); i++)
		for (unsigned j = 0; j < (*this->docs)[i].tf_idf.size(); j++)
			(*this->docs)[i].tf_idf[j].pertence = true;

	a = this->selecionarMenosRelevanteDentroDoConjunto();
	this->selecionadas[a] = false;

	while (this->numCaracteristicasSelecionadas() > num_termos) {

		this->caract_recente_rem.clear();
		b = this->selecionarMenosRelevanteDentroDoConjunto();
		this->caract_recente_rem.push_back(b);

		this->selecionadas[b] = false;
		this->caract_recente_add.clear();

		while (true) {
			c = this->selecionarMaisRelevanteForaDoConjunto();
			aux = this->selecionadas;
			aux[c] = true;
			if (this->comparaSubsetFeatures(aux, this->selecionadas, K)) {
				cout << "Entrou " << c << endl;
				this->selecionadas = aux;
				this->caract_recente_add.push_back(c);
				continue;
			} else {
				for (unsigned i = 0; i < this->selecionadas.size(); i++)
					if (!this->selecionadas[i]) {
						cout << i << " ";
					}
				cout << endl;
				cout << "Não entrou " << c << endl;
				break;
			}
		}
	}
}

void wrapper::imprimirCaracteristicasSelecionadas ()
{
	for (unsigned i = 0; i < this->selecionadas.size(); i++)
		if (this->selecionadas[i])
			cout << i << " ";
	cout << endl;
}

bool wrapper::ehCaract_Recente_Add (const unsigned& comp)
{
	for (list<unsigned>::iterator It = this->caract_recente_add.begin(); It != this->caract_recente_add.end(); It++)
		if (*It == comp)
			return true;
	return false;
}

bool wrapper::ehCaract_Recente_Rem (const unsigned& comp)
{
	for (list<unsigned>::iterator It = this->caract_recente_rem.begin(); It != this->caract_recente_rem.end(); It++)
		if (*It == comp)
			return true;
	return false;
}

bool wrapper::comparaSubsetFeatures(vector<bool>& s1, vector<bool>& s2, const unsigned& K)
{
	vector<int> p1, p2;
	double c11 = 0, c12 = 0, c21 = 0, c22 = 0;
	p1 = this->getParticaoGeradaPorCaracteristicas(s1, K);
	p2 = this->getParticaoGeradaPorCaracteristicas(s2, K);
	c11 = this->valorCriterioComCaracteristicas(s1,p1);
	c12 = this->valorCriterioComCaracteristicas(s1,p2);
	c21 = this->valorCriterioComCaracteristicas(s2,p1);
	c22 = this->valorCriterioComCaracteristicas(s2,p2);

	cout << c11 << " " << c12 << " " << c21 << " " << c22 << endl;

	if ((c11 * c12) > (c21 * c22)) {
		return true;
	} else {
		return false;
	}
}

vector<int> wrapper::getParticaoGeradaPorCaracteristicas (vector<bool>& features, const unsigned& K)
{
	vector<int> p;
	for (unsigned i = 0; i < this->docs->size(); i++)
		for (unsigned j = 0; j < (*this->docs)[i].tf_idf.size(); j++)
			(*this->docs)[i].tf_idf[j].pertence = features[j];
	nuvens* n = new nuvens(this->docs);
	n->BiSectionKMeans(K-1,1,false,false,false,false,false,0,0);
	p = n->getMapeamento();
//	n->imprimirHierarquia();
	delete n;
	return p;
}

unsigned wrapper::selecionarMenosRelevanteDentroDoConjunto ()
{
	double aux = 9999999;
	unsigned selecionado = 0;
	unsigned count = 0;
	for (unsigned i = 0; i < this->qualidades.size(); i++)
		if ((this->selecionadas[i]) && !(this->ehCaract_Recente_Add(i))) {
			count++;
			if (this->qualidades[i] < aux) {
				aux = this->qualidades[i];
				selecionado = i;
			}
		}
	cout << "Saiu " << selecionado << endl;
	return selecionado;
}

unsigned wrapper::selecionarMaisRelevanteForaDoConjunto ()
{
	double aux = 0;
	unsigned selecionado = 0;
	unsigned count = 0;
	for (unsigned i = 0; i < this->qualidades.size(); i++)
		if (!(this->selecionadas[i]) && !(this->ehCaract_Recente_Rem(i))) {
			count++;
			if (this->qualidades[i] > aux) {
				aux = this->qualidades[i];
				selecionado = i;
			}
		}
	return selecionado;
}

double wrapper::valorCriterioComCaracteristicas (vector<bool>& features, vector<int>& particao)
{
	for (unsigned i = 0; i < this->docs->size(); i++)
		for (unsigned j = 0; j < (*this->docs)[i].tf_idf.size(); j++)
			(*this->docs)[i].tf_idf[j].pertence = features[j];

	for (unsigned i = 0; i < particao.size(); i++)
		(*this->docs)[i].inferida = particao[i];

	avaliacao A(this->docs);

	return A.similaridadeGeral(); // vai ser o critério interno
}

unsigned wrapper::numCaracteristicasSelecionadas ()
{
	unsigned count = 0;
	for (unsigned i = 0; i < this->selecionadas.size(); i++)
		if (this->selecionadas[i] == true)
			count++;
	return count;
}
