/*
 * prototipacao_caracteristicas.cpp
 *
 *  Created on: 06/01/2010
 *      Author: Marcelo
 */

#include "prototipacao_caracteristicas.h"

prototipacao_caracteristicas::prototipacao_caracteristicas(vector<document>* In)
{
	this->In = In;
	this->features.resize((*this->In)[0].tf_idf.size());
	this->mapeamento.resize((*this->In)[0].tf_idf.size());
	for (unsigned i = 0; i < this->mapeamento.size(); i++)
		this->mapeamento[i] = i;
	this->calcularDesimilaridades();
}

prototipacao_caracteristicas::~prototipacao_caracteristicas()
{
}

unsigned prototipacao_caracteristicas::getFeatureMaiorDensidade ()
{
	double comp = 100;
	unsigned escolhido = 0;
	for (unsigned i = 0; i < this->In->size(); i++) {
		if (this->features[i].densidade < comp) {
			comp = this->features[i].densidade;
			escolhido = i;
		}
	}
	return escolhido;
}

void prototipacao_caracteristicas::calcularDesimilaridades ()
{
	for (unsigned j = 0; j < this->features.size(); j++)
		this->features[j].V.resize(this->In->size());
	for (unsigned i = 0; i < this->features.size(); i++)
		for (unsigned j = 0; j < this->features[i].V.size(); j++)
			this->features[i].V[j] = (*this->In)[j].tf_idf[i].valor;

	boost::numeric::ublas::matrix<double> M(this->features.size(), this->features.size());

    for (unsigned i = 0; i < M.size1 (); i++)
    	for (unsigned j = 0; j < M.size2 (); j++)
            M (i, j) = 0;

	for (unsigned i = 0; i < this->features.size() - 1; i++)
		for (unsigned j = i + 1; j < this->features.size(); j++)
			M (i, j) = this->desimilaridadeFeatures(this->features[i], this->features[j]);

	for (unsigned i = 0; i < M.size1() - 1; i++)
		for (unsigned j = i + 1; j < M.size2(); j++)
			M (j, i) = M (i, j);

	for (unsigned i = 0; i < M.size1 (); i++)
		M (i, i) = 1;

	this->Desimilaridades = M;
}

void prototipacao_caracteristicas::realizarPrototipacao(const double& red, const unsigned& k)
{
	unsigned new_size = unsigned((double (this->features.size()) / red));
	unsigned paraExcluir = this->features.size() - new_size;
	unsigned jahExcluidos = 0;
	for (unsigned i = 0; i < this->features.size(); i++)
		this->features[i] = this->getCentroide(i,k);

	while (jahExcluidos < paraExcluir) {
		feature aux = this->features[this->getFeatureMaiorDensidade()];
		for (list<unsigned>::iterator It = aux.representantes.begin(); It != aux.representantes.end(); It++) {
			this->apagarCaracteristica(*It);
			this->mapeamento[*It] = aux.id;
		}
		jahExcluidos += aux.representantes.size();
	}
}

feature prototipacao_caracteristicas::getCentroide (const unsigned& doc, const unsigned& k)
{
	feature saida;
	saida.densidade = 0;
	list<unsigned> escolhidos;
	unsigned aux = 0;
	for (unsigned i = 0; i < this->Desimilaridades.size2(); i++) {
		if (i == doc)
			continue;
		if (escolhidos.size() < k) {
			escolhidos.push_back(i);
		} else {
			aux = this->getMenorSimilaridade(doc, escolhidos);
			if (this->Desimilaridades(doc,i) < this->Desimilaridades(doc,aux)) {
				escolhidos.remove(aux);
				escolhidos.push_back(i);
			}
		}
	}

	saida.densidade = this->Desimilaridades(this->getMenorSimilaridade(doc, escolhidos), doc);
	saida.id = doc;
	saida.representantes = escolhidos;
	saida.representantes.sort();
	saida.representantes.unique();
    return saida;
}

unsigned prototipacao_caracteristicas::getMenorSimilaridade (const unsigned& doc, list<unsigned>& escolhidos)
{
	double comp = 0;
	unsigned maior = 0;
	for (list<unsigned>::iterator It = escolhidos.begin(); It != escolhidos.end(); It++) {
		if (this->Desimilaridades(doc,*It) > comp) {
			comp = this->Desimilaridades(doc,*It);
			maior = *It;
		}
	}
	return maior;
}

double prototipacao_caracteristicas::desimilaridadeFeatures (const feature& a, const feature& b)
{
	double varA = calculos::variancia(a.V);
	double varB = calculos::variancia(b.V);
	double correlacao = calculos::covariancia(a.V,b.V) / sqrt(varA * varB);
	return varA + varB - sqrt (pow(varA + varB, 2) - 4 * varA * varB * (1 - pow(correlacao,2)));
}

void prototipacao_caracteristicas::apagarCaracteristica (const unsigned& ID)
{
	for (unsigned i = 0; i < this->features.size(); i++)
		if (features[i].id == ID) {
			this->features.erase(this->features.begin() + i);
			break;
		}
}
