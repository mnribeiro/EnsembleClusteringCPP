/*
 * prototipacao.cpp
 *
 *  Created on: 14/12/2009
 *      Author: Marcelo
 */

#include "prototipacao.h"

prototipacao::prototipacao()
{
}

prototipacao::prototipacao(vector<document>* In)
{
	this->In = *In;
	this->calcularSimilaridades();
	this->mapeamento.resize(this->In.size());
	for (unsigned i = 0; i < this->In.size(); i++)
		this->mapeamento[i] = i;
}

prototipacao::~prototipacao()
{
}

unsigned prototipacao::getDocMaiorDensidade ()
{
	double comp = 0;
	unsigned escolhido = 0;
	for (unsigned i = 0; i < this->In.size(); i++) {
		if (this->In[i].densidade > comp) {
			comp = this->In[i].densidade;
			escolhido = i;
		}
	}
	return escolhido;
}

void prototipacao::calcularSimilaridades ()
{
	boost::numeric::ublas::matrix<double> M(In.size(), In.size());
	nuvens aux;


    for (unsigned i = 0; i < M.size1 (); i++)
    	for (unsigned j = 0; j < M.size2 (); j++)
            M (i, j) = 0;

	for (unsigned i = 0; i < In.size() - 1; i++)
		for (unsigned j = i + 1; j < In.size(); j++)
			M (i, j) = aux.similaridade(this->In[i], this->In[j]);

	for (unsigned i = 0; i < M.size1() - 1; i++)
		for (unsigned j = i + 1; j < M.size2(); j++)
			M (j, i) = M (i, j);

	for (unsigned i = 0; i < M.size1 (); i++)
		M (i, i) = 1;

	this->Similaridades = M;
}

void prototipacao::realizarPrototipacao(const double& red, const unsigned& k)
{
	unsigned new_size = unsigned((double (this->In.size()) / red));
	unsigned paraExcluir = this->In.size() - new_size;
	unsigned jahExcluidos = 0;
	for (unsigned i = 0; i < this->In.size(); i++)
		this->In[i] = this->getCentroide(i,k);

	while (jahExcluidos < paraExcluir) {
		document aux = this->In[this->getDocMaiorDensidade()];
		for (list<unsigned>::iterator It = aux.representantes.begin(); It != aux.representantes.end(); It++){
			this->apagarDocumento(*It);
			this->mapeamento[*It] = aux.id;
		}
		jahExcluidos += aux.representantes.size();
	}
}

document prototipacao::getCentroide (const unsigned& doc, const unsigned& k)
{
	document saida;
	saida.densidade = 0;
	list<unsigned> escolhidos;
	unsigned aux = 0;
	for (unsigned i = 0; i < this->Similaridades.size2(); i++) {
		if (i == doc)
			continue;
		if (escolhidos.size() < k) {
			escolhidos.push_back(i);
		} else {
			aux = this->getMenorSimilaridade(doc, escolhidos);
			if (this->Similaridades(doc,i) > this->Similaridades(doc,aux)) {
				escolhidos.remove(aux);
				escolhidos.push_back(i);
			}
		}
	}

	vector<termo> V;
	V.resize(this->In[0].tf_idf.size());
	for (unsigned i = 0; i < V.size(); i++) {
		V[i].valor = 0;
	}
	for (list<unsigned>::iterator It = escolhidos.begin(); It != escolhidos.end(); It++) {
    	for (unsigned j = 0; j < V.size(); j++) {
    		V[j].valor += this->In[*It].tf_idf[j].valor;
    		V[j].pertence = this->In[*It].tf_idf[j].pertence;
    	}
 	}

	for (unsigned i = 0; i < V.size(); i++)
		V[i].valor /= escolhidos.size();

	saida.tf_idf = V;
	saida.inferida = 0;
	saida.id = doc;

	nuvens nub;
	double soma = 0;
	for (list<unsigned>::iterator It = escolhidos.begin(); It != escolhidos.end(); It++)
		soma += nub.similaridade(this->In[*It], saida);

	saida.representantes = escolhidos;
	saida.representantes.sort();
	saida.representantes.unique();
	saida.densidade = soma / escolhidos.size();

    return saida;
}

unsigned prototipacao::getMenorSimilaridade (const unsigned& doc, list<unsigned>& escolhidos)
{
	double comp = 1;
	unsigned menor = 0;
	for (list<unsigned>::iterator It = escolhidos.begin(); It != escolhidos.end(); It++) {
		if (this->Similaridades(doc,*It) < comp) {
			comp = this->Similaridades(doc,*It);
			menor = *It;
		}
	}
	return menor;
}

void prototipacao::apagarDocumento (const unsigned& ID)
{
	unsigned pos;
	for (unsigned i = 0; i < this->In.size(); i++)
		if (this->In[i].id == ID)
			pos = i;
	this->In.erase(this->In.begin() + pos);
}
