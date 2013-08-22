#include "estabilidade.h"

estabilidade::estabilidade()
{
}

estabilidade::~estabilidade()
{
	delete agru_amo;
	delete agru_ori;
}

estabilidade::estabilidade(vector<document>* L, vector<string>& V, double porc, double prop)
{
	this->Original = L;
	this->Amostra = new vector<document>();
	this->amostragem(porc);
	this->granularidade = unsigned(prop);
	unsigned i = unsigned(double(this->Original->size())/prop) - 1;
	this->agru_ori = new nuvens(this->Original, V);
	this->agru_amo = new nuvens(this->Amostra, V);
	this->agru_ori->BiSectionKMeans(i,5,true,false,false,true,false,0,METODO_TFV);
	this->agru_amo->BiSectionKMeans(i,5,false,false,false,true,false, 0,METODO_TFV);
	this->Original = this->agru_ori->getDocuments();
	this->Amostra = this->agru_amo->getDocuments();
	this->hier_ori = this->agru_ori->getHierarquia();
	this->hier_amo = this->agru_amo->getHierarquia();
	this->LeavesOR = this->agru_ori->getLeaves();
	this->FolhasFinais = this->LeavesOR[this->LeavesOR.size()-1];
	this->LeavesAM = this->agru_amo->getLeaves();
	this->num_grupos = 2*i;
}

vector<document>* estabilidade::getDocuments ()
{
	return this->Original;
}

nuvens* estabilidade::getAgrupamento ()
{
	return this->agru_ori;
}

bool estabilidade::isInCluster(const unsigned& cluster, document& d)
{
	for (list<unsigned>::iterator It = d.inferidas.begin(); It != d.inferidas.end(); It++) {
		if (*It == cluster)
			return true;
	}
	return false;
}

bool estabilidade::isInOneOfClusters(list<unsigned>& l, document& d)
{
	for (list<unsigned>::iterator It = l.begin(); It != l.end(); It++) {
		if (this->isInCluster(*It, d))
			return true;
	}
	return false;
}

unsigned estabilidade::isOfCluster (list<unsigned>& l, document& d)
{
	for (list<unsigned>::iterator It = l.begin(); It != l.end(); It++) {
		if (this->isInCluster(*It, d))
			return *It;
	}
	return 0;
}

void estabilidade::amostragem(double porc)
{
	unsigned i;
	unsigned size = this->Original->size();
	calculos C;
	while (true) {
		i = C.random(0,size);
		if (!(this->alreadyInAmostra((*this->Original)[i]))) {
			if (!(double(this->Amostra->size()) > porc * double(size))) {
				this->Amostra->push_back((*this->Original)[i]);
			} else {
				break;
			}
		} else {
			continue;
		}
	}
}

bool estabilidade::alreadyInAmostra(document& d)
{
	for (unsigned i = 0; i < this->Amostra->size(); i++) {
		if (d.id == (*this->Amostra)[i].id)
			return true;
	}
	return false;
}

bool estabilidade::isSonOf(const unsigned& filho, const unsigned& pai)
{
	tree<unsigned>::iterator It = find(this->hier_ori.begin(), this->hier_ori.end(), filho);
	if (filho == pai)
		return true;
	if ((filho == 0)  && (pai != 0))
		return false;
	while (*It != 0) {
		It = this->hier_ori.parent(It);
		if (*It == pai)
			return true;
	}
	return false;
}

vector<list<unsigned> > estabilidade::getFolhasFilhosDe (const unsigned& pai)
{
	unsigned tam = 0;
	list<unsigned> aux;
	vector<list<unsigned> > folhas;
	for (unsigned i = 0; i < this->LeavesOR.size(); i++) {
		for (list<unsigned>::iterator It = this->LeavesOR[i].begin(); It != this->LeavesOR[i].end(); It++) {
			if (this->isSonOf(*It,pai))
				aux.push_back(*It);
		}
		if (aux.size() > tam) {
			tam++;
			folhas.push_back(aux);
		}
		aux.clear();
	}
	return folhas;
}

vector<unsigned> estabilidade::getIndicesDaInter (list<unsigned>& lo, list<unsigned>& la)
{
	unsigned ind;
	vector<unsigned> vetor;
	for (unsigned i = 0; i < this->Original->size(); i++) {
		if (this->isInOneOfClusters(lo, (*this->Original)[i])) {
			ind = this->getIndVetorPeloIDAmo((*this->Original)[i].id);
			if (ind > this->Amostra->size()) {
				continue;
			}
			if (this->isInOneOfClusters(la, (*this->Amostra)[ind]))
				vetor.push_back(i);
		}
	}
	return vetor;
}

unsigned estabilidade::getIndVetorPeloIDOri (const unsigned& id)
{
	for (unsigned i = 0; i < this->Original->size(); i++) {
		if ((*this->Original)[i].id == id)
			return i;
	}
	return this->Original->size()+1;
}

unsigned estabilidade::getIndVetorPeloIDAmo (const unsigned& id)
{
	for (unsigned i = 0; i < this->Amostra->size(); i++) {
		if ((*this->Amostra)[i].id == id)
			return i;
	}
	return this->Amostra->size()+1;
}

double estabilidade::getValorIndice (list<unsigned>& lo, list<unsigned>& la)
{
	double a = 0, b = 0, c = 0, d = 0;
	for (unsigned i = 0; i < this->Original->size(); i++) {
		if (this->isInOneOfClusters(lo, (*this->Original)[i]))
			(*this->Original)[i].inferida = this->isOfCluster(lo, (*this->Original)[i]);
	}
	for (unsigned i = 0; i < this->Amostra->size(); i++) {
		if (this->isInOneOfClusters(la, (*this->Amostra)[i]))
			(*this->Amostra)[i].inferida = this->isOfCluster(la, (*this->Amostra)[i]);
	}
	vector<unsigned> Inter = this->getIndicesDaInter(lo, la);
	for (unsigned i = 0; i < Inter.size(); i++) {
		for (unsigned j = 0; j < Inter.size(); j++) {
			if (i < j) {
				if ((*this->Original)[Inter[i]].inferida == (*this->Original)[Inter[j]].inferida
						&& (*this->Amostra)[this->getIndVetorPeloIDAmo((*this->Original)[Inter[i]].id)].inferida ==
						(*this->Amostra)[this->getIndVetorPeloIDAmo((*this->Original)[Inter[j]].id)].inferida) {
					a++;
					continue;
				}
				if ((*this->Original)[Inter[i]].inferida != (*this->Original)[Inter[j]].inferida
						&& (*this->Amostra)[this->getIndVetorPeloIDAmo((*this->Original)[Inter[i]].id)].inferida !=
						(*this->Amostra)[this->getIndVetorPeloIDAmo((*this->Original)[Inter[j]].id)].inferida) {
					d++;
					continue;
				}
				if ((*this->Original)[Inter[i]].inferida == (*this->Original)[Inter[j]].inferida
						&& (*this->Amostra)[this->getIndVetorPeloIDAmo((*this->Original)[Inter[i]].id)].inferida !=
						(*this->Amostra)[this->getIndVetorPeloIDAmo((*this->Original)[Inter[j]].id)].inferida) {
					b++;
					continue;
				}
				if ((*this->Original)[Inter[i]].inferida != (*this->Original)[Inter[j]].inferida
						&& (*this->Amostra)[this->getIndVetorPeloIDAmo((*this->Original)[Inter[i]].id)].inferida ==
						(*this->Amostra)[this->getIndVetorPeloIDAmo((*this->Original)[Inter[j]].id)].inferida) {
					c++;
					continue;
				}
			}
		}
	}
	double rand = a / (a + b + c);

	for (list<unsigned>::iterator It = lo.begin(); It != lo.end(); It++) {
		cout << *It << " ";
	}
	cout << rand << endl;
	return rand;
}

bool estabilidade::isFolhaFinal(const unsigned& f)
{
	for (list<unsigned>::iterator It = this->FolhasFinais.begin(); It != this->FolhasFinais.end(); It++) {
		if (*It == f)
			return true;
	}
	return false;
}

unsigned estabilidade::retornarIndiceEscolhido (vector<double>& v)
{
	map<unsigned, cotovelo> pontos;

	// Inicia
	cout << "Intra-distancia" << endl;
	for (unsigned i = 1; i < v.size(); i++) {
		if (i == 1) {
			pontos[i].intra_distancia = abs(v[1] - v[2]) + abs(v[1] - v[3]);
			cout << i << " " << pontos[i].intra_distancia << " ";
		} else {
			if (i == (v.size()-1)) {
				pontos[i].intra_distancia = abs(v[v.size()-1] - v[v.size()-2]) + abs(v[v.size()-1] - v[v.size()-3]);
				cout << i << " " << pontos[i].intra_distancia << " ";
			} else {
				pontos[i].intra_distancia = abs(v[i] - v[i-1]) + abs(v[i] - v[i+1]);
				cout << i << " " << pontos[i].intra_distancia << " ";
			}
		}
		pontos[i].vezes_indicado = 0;
	}
	cout << endl;
	//
	for (unsigned i = 2; i < v.size()-1; i++) {
		double dist1, dist2, dist3;
		dist1 = abs(v[i-1] - v[i]) + abs(v[i-1] - v[i+1]);
		dist2 = abs(v[i-1] - v[i]) + abs(v[i] - v[i+1]);
		dist3 = abs(v[i] - v[i+1]) + abs(v[i-1] - v[i+1]);
		// i - 1
		if ((v[i-1] > v[i]) && (v[i-1] > v[i+1])) {
			if ((dist1 > dist2) && (dist1 > dist3))
				pontos[i-1].vezes_indicado++;
		}
		// i
		if ((v[i] > v[i-1]) && (v[i] > v[i+1])) {
			if ((dist2 > dist1) && (dist2 > dist3))
				pontos[i].vezes_indicado++;
		}
		if ((v[i+1] > v[i]) && (v[i+1] > v[i-1])) {
			if ((dist3 > dist1) && (dist3 > dist2))
				pontos[i+1].vezes_indicado++;
		}

	}
	unsigned escolhido = 1;
	unsigned comp_inds = 0;
	double maior_intra = 0;
	for (unsigned i = 1; i < v.size(); i++) {
		cout << "Para " << i << " " << pontos[i].vezes_indicado << endl;
		if (pontos[i].vezes_indicado > comp_inds) {
			escolhido = i;
			comp_inds = pontos[i].vezes_indicado;
			maior_intra = pontos[i].intra_distancia;
		} else {
			if (pontos[i].vezes_indicado == comp_inds) {
				if (pontos[i].intra_distancia > maior_intra) {
					escolhido = i;
					maior_intra = pontos[i].intra_distancia;
				}
			}
		}
	}
	return escolhido;
}

void estabilidade::inferirGrupos ()
{
	stack<unsigned> GruposADividir;
	unsigned ind_escolhido = this->LeavesOR.size()+1, pai;
	vector<double> v;
	v.resize(this->LeavesOR.size());
	for (unsigned i = 1; i < this->LeavesOR.size(); i++)
		v[i] = this->getValorIndice(this->LeavesOR[i], this->LeavesAM[i]);
	ind_escolhido = this->retornarIndiceEscolhido(v);
	cout << "Escolhido " << ind_escolhido << endl;
	v.clear();
	tree<unsigned>::iterator Iterator = this->hier_final.set_head(0);
	for (list<unsigned>::iterator It = this->LeavesOR[ind_escolhido].begin(); It != this->LeavesOR[ind_escolhido].end(); It++) {
		if (!(this->isFolhaFinal(*It)))
			GruposADividir.push(*It);
		this->hier_final.append_child(Iterator,*It);
		cout << *It << " ";;
	}
	cout << endl;
	vector<list<unsigned> > V;
	while (!(GruposADividir.empty())) {
		pai = GruposADividir.top();
		cout << pai<< endl;
		GruposADividir.pop();
		V = this->getFolhasFilhosDe(pai);
		v.resize(V.size());
		for (unsigned i = 1; i < V.size(); i++)
			v[i] = this->getValorIndice(V[i], this->LeavesAM[i]);
		ind_escolhido = this->retornarIndiceEscolhido(v);
		cout << "Escolhido " << ind_escolhido << endl;
		v.clear();
		Iterator = find(this->hier_final.begin(), this->hier_final.end(),pai);
		for (list<unsigned>::iterator It = V[ind_escolhido].begin(); It != V[ind_escolhido].end(); It++) {
			if (!(this->isFolhaFinal(*It)))
				GruposADividir.push(*It);
			this->hier_final.append_child(Iterator,*It);
			cout << *It << " ";
		}
		cout << endl;
	}
}

void estabilidade::imprimirHierarquia ()
{
	for (tree<unsigned>::pre_order_iterator It = this->hier_final.begin();It != this->hier_final.end();It++) {
		cout << *It;
		if (!(*It == 0)) {
			cout << "[" << *(this->hier_final.parent(It)) << "] ";
		} else cout << " ";
	}
	cout  << endl;
}

vector<document>* estabilidade::getAmostra ()
{
	return this->Amostra;
}
