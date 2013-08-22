#include "featureselection.h"

featureselection::featureselection()
{
}

featureselection::~featureselection()
{
}

double featureselection::similaridade (document& d1, document& d2) // calcula a similaridade
{
	double s1 = 0, s2 = 0, s3 = 0, s;
	for (unsigned i = 0; i < d1.tf_idf.size(); i++) {
		if (d1.tf_idf[i].pertence) {
			s1 += d1.tf_idf[i].valor * d2.tf_idf[i].valor;
			s2 += d1.tf_idf[i].valor * d1.tf_idf[i].valor;
			s3 += d2.tf_idf[i].valor * d2.tf_idf[i].valor;
		}
	}

	if ((s2 == 0.0) || (s3 == 0.0)) {
		return s1;
	}

	s = s1 / (sqrt(s2) * sqrt(s3));

	if (s == s) {
		if (s < 0.0001)
			return s;
		else
			return 0.0001;
	} else {
		return 0;
	}
}

featureselection::featureselection(vector<document>* valores)
{
	this->valores = valores;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		for (unsigned j = 0; j < (*this->valores)[i].tf_idf.size(); j++)
			(*this->valores)[i].tf_idf[j].pertence = true;
	}
}

vector<document> featureselection::getDocumentsPorCluster (const unsigned& cluster)
{
	vector<document> V;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if (this->isInCluster(cluster, (*this->valores)[i]))
			V.push_back((*this->valores)[i]);
	}
	return V;
}

bool featureselection::isInCluster(const unsigned& cluster, document& d)
{
	for (list<unsigned>::iterator It = d.inferidas.begin(); It != d.inferidas.end(); It++) {
		if (*It == cluster)
			return true;
	}
	return false;
}

double featureselection::getQualityTFV (const unsigned& cluster, const unsigned& term)
{
	double t1 = 0, t2 = 0, size = 0, t3;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if (this->isInCluster(cluster, (*this->valores)[i])) {
			t1 += pow((*this->valores)[i].tf[term], 2);
			t2 += (*this->valores)[i].tf[term];
			size++;
		}
	}
	t3 = (pow(t2,2))/size;
	return t1 - t3;
}

double featureselection::getQualityDF (const unsigned& cluster, const unsigned& term)
{
	double df = 0;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if (this->isInCluster(cluster, (*this->valores)[i])) {
			if ((*this->valores)[i].tf[term] > 0)
				df++;
		}
	}
	return df;
}

double featureselection::getQualityMeanTFIDF (const unsigned& cluster, const unsigned& term)
{
	double tfidf = 0, n = 0;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if (this->isInCluster(cluster, (*this->valores)[i])) {
			tfidf += (*this->valores)[i].tf_idf[term].valor;
			n++;
		}
	}
	return tfidf/n;
}

double featureselection::getQualityTS (const unsigned& cluster, const unsigned& term, const double& limiar)
{
	double dj = 0, dij = 0;
	for (unsigned i = 0; i < this->valores->size();i++) {
		for (unsigned j = 0; j < this->valores->size(); j++) {
			if (i < j) {
				if ((this->isInCluster(cluster, (*this->valores)[i])) &&
						(this->isInCluster(cluster, (*this->valores)[j]))) {
//					cout << i << " " << j << " " << this->similaridade(this->valores[i],this->valores[j]) << endl;
					if (this->similaridade((*this->valores)[i],(*this->valores)[j]) > limiar) {
						if ((*this->valores)[j].tf_idf[term].valor > 0) {
							dj++;
							if ((*this->valores)[i].tf_idf[term].valor > 0)
								dij++;
						}
					}
				}
			}
		}
	}

	cout << "No cluster " << cluster << " termo " << term << " tem qualidade " << dj << endl;

	return dij / dj;
}

void featureselection::calcularEntropia(const unsigned& cluster)
{
	double Sij, EnComTermo = 0;

	for (unsigned i = 0; i < this->valores->size();i++) {
		for (unsigned j = 0; j < this->valores->size(); j++) {
			if (i < j) {
				if ((this->isInCluster(cluster, (*this->valores)[i])) &&
						(this->isInCluster(cluster, (*this->valores)[j]))) {
					Sij = this->similaridade((*this->valores)[i],(*this->valores)[j]);

					double teste = Sij * log10(Sij);
					if (teste != teste) {
						teste = 0;
					}
					EnComTermo += (teste + ((1 - Sij)*(log10(1 - Sij))));
				}
			}
		}
	}

	this->Entropia = EnComTermo * (-1);
}

double featureselection::getQualityEn (const unsigned& cluster, const unsigned& term)
{
	double Sij, EnSemTermo = 0;

	for (unsigned i = 0; i < this->valores->size(); i++)
		(*this->valores)[i].tf_idf[term].pertence = false;

	for (unsigned i = 0; i < this->valores->size();i++) {
		for (unsigned j = 0; j < this->valores->size(); j++) {
			if (i < j) {
				if ((this->isInCluster(cluster, (*this->valores)[i])) &&
						(this->isInCluster(cluster, (*this->valores)[j]))) {
					Sij = this->similaridade((*this->valores)[i],(*this->valores)[j]);
					double teste = Sij * log10(Sij);
					if (teste != teste) {
						teste = 0;
					}
					EnSemTermo += (teste + ((1 - Sij)*(log10(1 - Sij))));
				}
			}
		}
	}

	for (unsigned i = 0; i < this->valores->size(); i++)
		(*this->valores)[i].tf_idf[term].pertence = true;

	EnSemTermo = EnSemTermo * (-1);

//	cout << "No cluster " << cluster << " termo " << term << " tem qualidade " << this->Entropia - EnSemTermo << endl;

	return this->Entropia - EnSemTermo;
}

void featureselection::gerarValoresComTermosSelecionadosAleatoriamente(const unsigned& n)
{
	list<unsigned> lu;
	calculos C;
	unsigned n_termos = (*this->valores)[0].tf_idf.size()-1;
	while (lu.size() < n) {
		lu.push_back(C.random (0, n_termos));
		lu.unique();
	}

	for (unsigned i = 0; i < this->valores->size(); i++)
		for (unsigned j = 0; j < (*this->valores)[i].tf_idf.size(); j++)
			(*this->valores)[i].tf_idf[j].pertence = false;

	for (list<unsigned>::iterator It = lu.begin(); It != lu.end(); It++) {
		for (unsigned i = 0; i < this->valores->size(); i++)
			(*this->valores)[i].tf_idf[*It].pertence = true;
	}
}

void featureselection::gerarValoresComTermosSelecionadosAleatoriamenteComPesos(const unsigned& n)
{
	list<unsigned> lu;
	vector<double> qualidades, soma_qualidades;
	qualidades.resize((*this->valores)[0].tf_idf.size());
	soma_qualidades.resize((*this->valores)[0].tf_idf.size());
	double soma = 0;
	for (unsigned i = 0; i < qualidades.size(); i++) {
		qualidades[i] = this->getQualityTFV(0,i);
		soma_qualidades[i] = soma;
		soma += qualidades[i];
//		cout << i << " " << qualidades[i] << " " << soma_qualidades[i] << endl;
	}

	double ale;
	unsigned t_selecionado = soma_qualidades.size() - 1;
	calculos C;
	while (lu.size() < n) {
		ale = C.random() * soma;
		for (unsigned i = 0; i < soma_qualidades.size(); i++) {
			if (soma_qualidades[i] > ale) {
				t_selecionado = i - 1;
				break;
			}
		}
		lu.push_back(t_selecionado);
		lu.unique();
	}

	for (unsigned i = 0; i < this->valores->size(); i++)
		for (unsigned j = 0; j < (*this->valores)[i].tf_idf.size(); j++)
			(*this->valores)[i].tf_idf[j].pertence = false;

	for (list<unsigned>::iterator It = lu.begin(); It != lu.end(); It++) {
//		cout << *It << " ";
		for (unsigned i = 0; i < this->valores->size(); i++)
			(*this->valores)[i].tf_idf[*It].pertence = true;
	}// cout << endl;
}

map<unsigned, vector<bool> > featureselection::gerarMapaDeCaracteristicasPorGrupo (const unsigned& termos, const unsigned& metodo)
{
	list<unsigned> lg;
	for (unsigned i = 0; i < this->valores->size(); i++)
		lg.push_back((*this->valores)[i].inferida);
	lg.sort();
	lg.unique();
	
	map<unsigned, vector<bool> > features;
	vector<bool> fea_selecionadas;
	fea_selecionadas.resize((*this->valores)[0].tf_idf.size());

	for (list<unsigned>::iterator It = lg.begin(); It != lg.end(); It++) {
		this->gerarValoresComTermosSelecionados(*It,termos,metodo);
		for (unsigned j = 0; j < (*this->valores)[0].tf_idf.size(); j++)
			fea_selecionadas[j] = (*this->valores)[0].tf_idf[j].pertence;
		features[*It] = fea_selecionadas;
	}
	return features;
}

map<unsigned, vector<bool> > featureselection::gerarTodosMapaDeCaracteristicasPorGrupo (const unsigned& termos, const unsigned& metodo,
		const unsigned& num_grupos)
{
	map<unsigned, vector<bool> > features;
	vector<bool> fea_selecionadas;
	fea_selecionadas.resize((*this->valores)[0].tf_idf.size());

	for (unsigned i = 0; i <= num_grupos; i++) {
		this->gerarValoresComTermosSelecionados(i,termos,metodo);
		for (unsigned j = 0; j < (*this->valores)[0].tf_idf.size(); j++)
			fea_selecionadas[j] = (*this->valores)[0].tf_idf[j].pertence;
		features[i] = fea_selecionadas;
	}
	return features;
}

list<unsigned> featureselection::getTermosSelecionadosParaCluster(const unsigned& cluster, int n, const int& tipo)
{
	map<unsigned, double> Quality;
	vector<unsigned> Termos;
	list<unsigned> Labels;
	Termos.resize((*this->valores)[0].tf_idf.size());

	for (unsigned i = 0; i < Termos.size(); i++) {
		Termos[i] = i;
	}

	switch (tipo) {
	case METODO_TFV :
		for (unsigned i = 0; i < Termos.size(); i++) {
			Quality[Termos[i]] = this->getQualityTFV(cluster, Termos[i]);
		}
		break;
	case METODO_DF :
		for (unsigned i = 0; i < Termos.size(); i++) {
			Quality[Termos[i]] = this->getQualityDF(cluster, Termos[i]);
		}
		break;
	case METODO_MEANTFIDF :
		for (unsigned i = 0; i < Termos.size(); i++) {
			Quality[Termos[i]] = this->getQualityMeanTFIDF(cluster, Termos[i]);
		}
		break;
	case METODO_ENTROPIA :
		this->calcularEntropia(cluster);
		for (unsigned i = 0; i < Termos.size(); i++) {
			Quality[Termos[i]] = this->getQualityEn(cluster, Termos[i]);
		}
		break;
	}

	unsigned aux;
	for (unsigned j = 0; j < Termos.size(); j++) {
		for (unsigned k = Termos.size()-1; k > j; k--) {
			if (Quality[Termos[k]] < Quality[Termos[k-1]]) {
				aux = Termos[k-1];
				Termos[k-1] = Termos[k];
				Termos[k] = aux;
			}
		}
	}

	stack<unsigned> pilha;

	this->Selecionados.clear();
	this->Selecionados.resize((*this->valores)[0].tf_idf.size());
	for (unsigned i = 0; i < this->Selecionados.size(); i++)
		this->Selecionados[i] = false;

	for (unsigned i = 0; i < Termos.size(); i++) {
		pilha.push(Termos[i]);
	}

	while (n > 0) {
		if (!(pilha.empty())) {
			this->Selecionados[pilha.top()] = true;
			Labels.push_back(pilha.top());
			pilha.pop();
		}
		n--;
	}
	return Labels;
}

void featureselection::gerarValoresComTermosSelecionados(const unsigned& cluster, const int& n,
		const int& tipo)
{
	this->getTermosSelecionadosParaCluster(cluster,n,tipo);

	for (unsigned i = 0; i < this->valores->size(); i++) {
		for (unsigned j = 0; j < (*this->valores)[i].tf_idf.size(); j++) {
			if (this->Selecionados[j] == false) {
				(*this->valores)[i].tf_idf[j].pertence = false;
			} else {
				(*this->valores)[i].tf_idf[j].pertence = true;
			}
		}
	}
}
