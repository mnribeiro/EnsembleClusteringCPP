/*
 * ensemble.cpp
 *
 *  Created on: 16/11/2009
 *      Author: Marcelo
 */

#include "ensemble.h"

ensemble::ensemble() {
}

ensemble::ensemble(list<vector<int> >* p)
{
	this->Particoes = p;
	this->num_docs = this->Particoes->begin()->size();
	this->Ensemble.resize(this->num_docs);
	if (this->Particoes->size() > 0)
		this->calcularConsenso();
}

ensemble::ensemble(vector<vector<int> > p)
{
	this->Particoes = new (list<vector<int> >)();
	this->putParticoes(p);
	this->num_docs = this->Particoes->begin()->size();
	this->Ensemble.resize(this->num_docs);
	if (this->Particoes->size() > 0)
		this->calcularConsenso();
}

ensemble::~ensemble() {
}

void ensemble::addParticao (vector<int> v)
{
	this->Particoes->push_back(v);
}

vector<int> ensemble::getEnsemble ()
{
	return this->Ensemble;
}

double ensemble::nIJ(vector<int> A, vector<int> B, int a, int b)
{
	double count = 0;
	for (unsigned i = 0; i < A.size(); i++)
		if ((A[i] == a) && (B[i] == b))
			count++;
	return count;
}

double ensemble::nI(vector<int> A, int a)
{
	double count = 0;
	for (unsigned i = 0; i < A.size(); i++)
		if ((A[i] == a))
			count++;
	return count;
}

double ensemble::randAjustado(vector<int> A, vector<int> B)
{
	list<unsigned> cA, cB;
	for (unsigned i = 0; i < A.size(); i++)
		cA.push_back(A[i]);
	for (unsigned i = 0; i < B.size(); i++)
		cB.push_back(B[i]);
	cA.sort();
	cA.unique();
	cB.sort();
	cB.unique();

	double t1 = 0, t2 = 0, t3 = 0, aux = 0, arAUX = 0;

	for (list<unsigned>::iterator It = cA.begin(); It != cA.end(); It++) {
		aux = this->nI(A, *It);
		t1 += (aux * (aux - 1)) / 2;
	}
	for (list<unsigned>::iterator It = cB.begin(); It != cB.end(); It++) {
		aux = this->nI(B, *It);
		t2 += (aux * (aux - 1)) / 2;
	}
	t3 = (2 * t1 * t2) / (A.size() * (A.size() - 1));

	for (list<unsigned>::iterator ItA = cA.begin(); ItA != cA.end(); ItA++) {
		for (list<unsigned>::iterator ItB = cB.begin(); ItB != cB.end(); ItB++) {
			aux = this->nIJ (A, B, *ItA, *ItB);
			arAUX += (aux * (aux - 1)) / 2;
		}
	}
	return (arAUX - t3) / (0.5 * (t1 + t2) - t3);
}

double ensemble::rand(vector<int> A, vector<int> B)
{
	double a = 0, b = 0;
//#pragma omp parallel for
	for (unsigned i = 0; i < A.size(); i++) {
		for (unsigned j = 0; j < i; j++) {
			if ((A[i] == A[j]) && (B[i] == B[j]))
				a++;
			if ((A[i] != A[j]) && (B[i] != B[j]))
				b++;
		}
	}
	double s = double(A.size());
	s = (s * (s - 1.0)) / 2;
	return (a + b) / s;
}

double ensemble::diversidadeDP ()
{
	double soma = 0;
	list<vector<int> >::iterator inicio = this->Particoes->begin();
	list<vector<int> >::iterator fim = this->Particoes->end();
	inicio++;
	fim--;
	for (list<vector<int> >::iterator It1 = this->Particoes->begin(); It1 != fim; It1++) {
		for (list<vector<int> >::iterator It2 = inicio; It2 != this->Particoes->end(); It2++) {
			if (It1 == It2)
				continue;
			soma += (1 - this->randAjustado(*It1,*It2));
			cout << "DP " << soma << endl;
		}
	}
	double L = this->Particoes->size();
	return (2 / (L*(L-1))) * soma;
}

double ensemble::diversidadeDNP ()
{
	double soma = 0;
	for (list<vector<int> >::iterator It = this->Particoes->begin(); It != this->Particoes->end(); It++) {
		soma += (1-this->randAjustado(*It,this->Ensemble));
		cout << "DNP " << soma << endl;
	}
	return soma / this->Particoes->size();
}

void ensemble::gerarParticoesSelecaoLocal (vector<document>* docs, const unsigned& num_termos,
		const unsigned& normais, bool local)
{
/*
	METODO_TFV METODO_DF METODO_MEANTFIDF METODO_ENTROPIA
	cortes | ITER | label | localizada | global | localizada-auto | termos selecionados | Método
*/
	this->Particoes = new (list<vector<int> >)();
	for (unsigned i = 0; i < normais; i++) {
		nuvens* n = new nuvens(docs);
		n->BiSectionKMeans(39,5,false,false,false,false,false,num_termos,METODO_TFV);
		this->Particoes->push_back(n->getMapeamento());
		avaliacao* A = new avaliacao(docs);
		cout << i << " : " << A->getPrecisao() << endl;
		delete n;
	}

//	cout << "Diversidade neste nível " << this->diversidadeDP() << endl;

	if (local) {

		nuvens* n = new nuvens(docs);
		n->BiSectionKMeans(39,5,false,false,false,false,false,0,METODO_TFV);
		featureselection* selecao = new featureselection(docs);
		map<unsigned, vector<bool> > Features = selecao->gerarMapaDeCaracteristicasPorGrupo(num_termos,METODO_TFV);
//		map<unsigned, vector<bool> > Features = n->getFeatures();
		avaliacao* A = new avaliacao(docs);
		cout << "Usando seleção local " << A->getPrecisao() << endl;
		delete A;
		delete n;
		delete selecao;

		for (map<unsigned, vector<bool> >::iterator Itmap = Features.begin(); Itmap != Features.end(); Itmap++) {
			nuvens* n = new nuvens(docs);
			n->setPertinenciaFeatures(Features[Itmap->first]);
			n->BiSectionKMeans(39,5,false,false,false,false,false,0,METODO_TFV);
			this->Particoes->push_back(n->getMapeamento());
			avaliacao* A = new avaliacao(docs);
			cout << Itmap->first << " " << A->getPrecisao() << endl;
			//		cout << A.similaridadeGeral() << endl;
			delete n;
			delete A;
		}
	}
	this->num_docs = this->Particoes->begin()->size();
	this->Ensemble.resize(this->num_docs);
	if (this->Particoes->size() > 0)
		this->calcularConsenso();
}

void ensemble::gerarParticoesSelecaoLocalComAmostragem (vector<document>* docs, const unsigned& num_termos,
		const unsigned& normais, bool local)
{
/*
	METODO_TFV METODO_DF METODO_MEANTFIDF METODO_ENTROPIA
	cortes | ITER | label | localizada | global | localizada-auto | termos selecionados | Método
*/
	this->Particoes = new (list<vector<int> >)();
	for (unsigned i = 0; i < normais; i++) {
		nuvens* n = new nuvens(docs);
		n->BiSectionKMeans(39,5,false,false,false,false,false,num_termos,METODO_TFV);
		this->Particoes->push_back(n->getMapeamento());
		avaliacao* A = new avaliacao(docs);
		cout << i << " : " << A->getPrecisao() << endl;
		delete n;
	}

//	cout << "Diversidade neste nível " << this->diversidadeDP() << endl;

	if (local) {

		nuvens* n = new nuvens(docs);
		n->BiSectionKMeans(39,5,false,false,false,true,false,0,METODO_TFV);
		featureselection* selecao = new featureselection(docs);
//		map<unsigned, vector<bool> > Features = selecao->gerarTodosMapaDeCaracteristicasPorGrupo(num_termos,METODO_TFV,n->getNumdeGrupos());
		map<unsigned, vector<bool> > Features = n->getFeatures();
		avaliacao* A = new avaliacao(docs);
		cout << "Usando seleção local " << A->getPrecisao() << endl;
		delete A;
		delete n;
		delete selecao;

		cout << "passei aqui" << endl;
/*
		for (map<unsigned, vector<bool> >::iterator Itmap = Features.begin(); Itmap != Features.end(); Itmap++) {
			cout << Itmap->first << " ";
			for (unsigned i = 0; i < Features[Itmap->first].size(); i++)
				if (Features[Itmap->first][i] == true)
					cout << i << " ";
			cout << endl;
		}
*/
		for (map<unsigned, vector<bool> >::iterator Itmap = Features.begin(); Itmap != Features.end(); Itmap++) {
			cout << Itmap->first << endl;
			nuvens* n = new nuvens(docs, Itmap->first, Features[Itmap->first]);
			this->Particoes->push_back(n->getMapeamento());
//			avaliacao* A = new avaliacao(docs);
//			cout << Itmap->first << " " << A->getPrecisao() << endl;
			//		cout << A.similaridadeGeral() << endl;
			delete n;
//			delete A;
		}
	}
	this->num_docs = this->Particoes->begin()->size();
	this->Ensemble.resize(this->num_docs);
	if (this->Particoes->size() > 0)
		this->calcularConsenso();
}

void ensemble::gerarParticoesSelecaoAleatoria (vector<document>* docs, const unsigned& normais, const unsigned& num_termos)
{
/*
	METODO_TFV METODO_DF METODO_MEANTFIDF METODO_ENTROPIA
	cortes | ITER | label | localizada | global | localizada-auto | termos selecionados | Método
*/
	this->Particoes = new (list<vector<int> >)();
	for (unsigned i = 0; i < normais; i++) {
		nuvens* n = new nuvens(docs);
		n->BiSectionKMeans(39,5,false,false,false,false,true,num_termos,METODO_TFV);
		this->Particoes->push_back(n->getMapeamento());
		avaliacao* A = new avaliacao(docs);
		cout << i << " : " << A->getPrecisao() << endl;
		delete n;
	}

//	cout << "Diversidade neste nível " << this->diversidadeDP() << endl;

	this->num_docs = this->Particoes->begin()->size();
	this->Ensemble.resize(this->num_docs);
	if (this->Particoes->size() > 0)
		this->calcularConsenso();
}

void ensemble::putParticoes (vector<vector<int> > V) {
	for (unsigned i = 0; i < V.size(); i++)
		this->addParticao(V[i]);
}


void ensemble::calcularConsenso ()
{
	unsigned num_particoes = this->Particoes->size();
	boost::numeric::ublas::matrix<double> M(this->num_docs, this->num_docs);
	boost::numeric::ublas::matrix<double> C(this->num_docs, this->num_docs);

//#pragma omp parallel for private (j)
    for (unsigned i = 0; i < M.size1 (); i++) {
    	for (unsigned j = 0; j < M.size2 (); j++){
            M (i, j) = 0;
            C (i, j) = 0;
    	}
    }

//#pragma omp parallel for private(j,It)
	for (unsigned i = 0; i < this->num_docs - 1; i++)
		for (unsigned j = i + 1; j < this->num_docs; j++)
			for (list<vector<int> >::iterator It = this->Particoes->begin(); It != this->Particoes->end(); It++) {
				if (((*It)[i] != -1) && ((*It)[j] != -1) && ((*It)[i] == (*It)[j])) {
					M (i, j)++;
				} else {
					if (((*It)[i] == -1) || ((*It)[j] == -1))
						C (i, j)++;
				}
			}
/*
	for (list<vector<int> >::iterator It = this->Particoes->begin(); It != this->Particoes->end(); It++) {
		for (unsigned i = 0; i < this->num_docs - 1; i++)
			cout << (*It)[i] << " ";
		cout << endl;
	}
*/
	//	M /= num_particoes;

//#pragma omp parallel for private(j)
    for (unsigned i = 0; i < M.size1 (); i++)
    	for (unsigned j = 0; j < M.size2 (); j++)
            M (i, j) = M (i, j) / (num_particoes - C(i,j));
//#pragma omp parallel for private(j)
	for (unsigned i = 0; i < this->num_docs - 1; i++)
		for (unsigned j = i + 1; j < this->num_docs; j++)
			M (j, i) = M (i, j);
//#pragma omp parallel for
    for (unsigned i = 0; i < M.size1 (); i++)
    	M (i, i) = 1;

    this->MConsenso = M;
}

void ensemble::gerarParticaoConsensual(const unsigned& K, const unsigned& tipo_link)
{
	this->flag = tipo_link;
	vector<grupo> Lista_de_grupos;
	unsigned num_aglutinacoes = (this->num_docs * 2) - K; // range de grupos [num_docs, num_docs + num_aglutinacoes]

	grupo aux;

	// Inicialização de cada ponto em um grupo.
//#pragma omp parallel for
	for (unsigned i = 0; i < this->num_docs; i++) {
		aux.eh_no = true;
		aux.pontos.push_back(i);
		Lista_de_grupos.push_back(aux);
		aux.pontos.clear();
	}

	double comp, count, s;
	unsigned e1 = 0, e2 = 0; // pontos escolhidos em cada iteração
	// Até que se formem K grupos de alto nível, escolher...
	for (unsigned i = this->num_docs; i < (this->num_docs + num_aglutinacoes); i++) {
		comp = 0;
		for (unsigned j = 0; j < Lista_de_grupos.size() - 1; j++) {
			for (unsigned k = j + 1; k < Lista_de_grupos.size(); k++) {
				if (Lista_de_grupos[j].eh_no && Lista_de_grupos[k].eh_no) {
					s = this->similaridade(Lista_de_grupos[j], Lista_de_grupos[k]);
					if (s >= comp) {
						e1 = j; e2 = k;
						comp = s;
					}
				}
			}
		}
		// ... unir e criar novos grupos
		aux.eh_no = true;

//		cout << "Unindo " << e1 << "[" << Lista_de_grupos[e1].pontos.size() << "] e " << e2 <<
//				"[" << Lista_de_grupos[e2].pontos.size() << "] em " << i;

		Lista_de_grupos[e1].eh_no = false;
		Lista_de_grupos[e2].eh_no = false;
		for (unsigned j = 0; j < Lista_de_grupos[e1].pontos.size(); j++)
			aux.pontos.push_back(Lista_de_grupos[e1].pontos[j]);
		for (unsigned j = 0; j < Lista_de_grupos[e2].pontos.size(); j++)
			aux.pontos.push_back(Lista_de_grupos[e2].pontos[j]);
		Lista_de_grupos.push_back(aux);
		aux.pontos.clear();

		count = 0;
		for (unsigned j = 0; j < Lista_de_grupos.size(); j++)
			if (Lista_de_grupos[j].eh_no)
				count++;
//		cout << " " << count << endl;
		if (count == K)
			break;

	}

	for (unsigned i = 0; i < Lista_de_grupos.size(); i++) {
		if (Lista_de_grupos[i].eh_no) {
			for (unsigned j = 0; j < Lista_de_grupos[i].pontos.size(); j++)
				this->Ensemble[Lista_de_grupos[i].pontos[j]] = i;
		}
	}
}

double ensemble::similaridade(grupo& A, grupo& B) {
	double comp, s = 0;
	if ((A.pontos.size() == 1) &&(B.pontos.size() == 1))
		return this->MConsenso(A.pontos[0], B.pontos[0]);
	if (this->flag == SINGLE_LINK) {
		comp = 0;
		for (unsigned i = 0; i < A.pontos.size(); i++) {
			for (unsigned j = 0;j < B.pontos.size(); j++) {
				if (this->MConsenso(A.pontos[i],B.pontos[j]) > comp) {
					s = this->MConsenso(A.pontos[i],B.pontos[j]);
					comp = s;
				}
			}
		}
		return s;
	}
	if (this->flag == COMPLETE_LINK) {
		comp = 2;
		for (unsigned i = 0; i < A.pontos.size(); i++) {
			for (unsigned j = 0;j < B.pontos.size(); j++) {
				if (this->MConsenso(A.pontos[i],B.pontos[j]) < comp) {
					s = this->MConsenso(A.pontos[i],B.pontos[j]);
					comp = s;
				}
			}
		}
		return s;
	}
	if (this->flag == AVERAGE_LINK) {
		double count = 0, s = 0;
		for (unsigned i = 0; i < A.pontos.size(); i++) {
			for (unsigned j = 0;j < B.pontos.size(); j++) {
				s += this->MConsenso(A.pontos[i],B.pontos[j]);
				count++;
			}
		}
		return s / count;
	}
	return 0;
}
