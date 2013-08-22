#include "nuvens.h"

nuvens::nuvens()
{
}

nuvens::~nuvens()
{
}

nuvens::nuvens(vector<document>* L, vector<string>& V) // legado
{
	this->valores = L;
	this->termos = V;

	for (unsigned i = 0; i < this->valores->size(); i++) {
		(*this->valores)[i].inferidas.push_back(0);
	}

	this->zerarCaracteristicas();

	this->IterTree = this->Hierarquia.insert(this->Hierarquia.begin(), 0);
}

nuvens::nuvens(vector<document>* L, const unsigned& grupo, vector<bool>& features)
{
	vector<document>* aux = new vector<document>();
	this->mapeamento.resize(L->size());
	this->docs_saida = *L;

	for (unsigned i = 0; i < this->docs_saida.size(); i++) {
		if (this->isInCluster(grupo, this->docs_saida[i])) {
			aux->push_back(this->docs_saida[i]);
			this->mapeamento[i] = aux->size() - 1;
		} else { // -1 para desconsiderar todos que não forem de GRUPO
			this->mapeamento[i] = -1;
			this->docs_saida[i].inferida = -1;
		}
	}
	this->valores = aux;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		(*this->valores)[i].inferidas.clear();
		(*this->valores)[i].inferidas.push_back(0);
		(*this->valores)[i].inferida = 0;
	}
	this->setPertinenciaFeatures(features);
	this->IterTree = this->Hierarquia.insert(this->Hierarquia.begin(), 0);

	unsigned n_grupos = unsigned(sqrt(double(this->valores->size())) - 1);

	this->BiSectionKMeans (n_grupos, 5, false, false, false, false, false, 0, METODO_TFV);

	for (unsigned i = 0; i < this->mapeamento.size(); i++)
		if (this->mapeamento[i] != -1)
			this->docs_saida[i] = (*this->valores)[this->mapeamento[i]];

	delete aux;
	this->valores = &(this->docs_saida);
}

nuvens::nuvens(vector<document>* L)
{
	this->valores = L;

	for (unsigned i = 0; i < this->valores->size(); i++) {
		(*this->valores)[i].inferidas.clear();
		(*this->valores)[i].inferidas.push_back(0);
		(*this->valores)[i].inferida = 0;
	}

	this->zerarCaracteristicas();

	this->IterTree = this->Hierarquia.insert(this->Hierarquia.begin(), 0);
}

nuvens::nuvens(vector<document>* L, const unsigned& d) // RANDOM PROJECTION
{
	this->valores = L;

	for (unsigned i = 0; i < this->valores->size(); i++) {
		(*this->valores)[i].inferidas.clear();
		(*this->valores)[i].inferidas.push_back(0);
		(*this->valores)[i].inferida = 0;
	}

	this->gerarMatrizProjetada(d);

	this->zerarCaracteristicas();

	this->IterTree = this->Hierarquia.insert(this->Hierarquia.begin(), 0);

	unsigned n_grupos = unsigned(sqrt(double(this->valores->size())) - 1);

	this->BiSectionKMeans (n_grupos, 5, false, false, false, false, false, 0, METODO_TFV);
}

void nuvens::zerarCaracteristicas ()
{
	for (unsigned i = 0; i < this->valores->size(); i++)
		for (unsigned j = 0; j < (*this->valores)[i].tf_idf.size(); j++)
			(*this->valores)[i].tf_idf[j].pertence = true;
}

void nuvens::gerarMatrizProjetada (const unsigned& d)
{
	using namespace boost::numeric::ublas;

	matrix<double> D (this->valores->size(), (*this->valores)[0].tf_idf.size());
	matrix<double> P ((*this->valores)[0].tf_idf.size(), d);
	matrix<double> DR (this->valores->size(), d);

	// crio a matriz D;

	for (unsigned i = 0; i < this->valores->size(); i++)
		for (unsigned j = 0; j < (*this->valores)[i].tf_idf.size(); j++)
			D (i,j) = (*this->valores)[i].tf_idf[j].valor;


	for (unsigned i = 0; i < P.size1 (); i++)
		for (unsigned j = 0; j < P.size2 (); j++)
			P (i, j) = (1.0 / sqrt (d)) * this->gerarAleatorio();

	double s = 0;

	for (unsigned i = 0; i < DR.size1 (); i++)
		for (unsigned j = 0; j < DR.size2 (); j++) {
			s = 0;
			for (unsigned l = 0; l < D.size2(); l++)
				s += (D(i,l) * P(l,j));
			DR (i, j) = s;
		}
/*
	cout << D << endl;
	cout << P << endl;
	cout << DR << endl;
*/
	for (unsigned i = 0; i < this->valores->size(); i++)
		(*this->valores)[i].tf_idf.resize(d);

	for (unsigned i = 0; i < this->valores->size(); i++)
		for (unsigned j = 0; j < (*this->valores)[i].tf_idf.size(); j++)
			(*this->valores)[i].tf_idf[j].valor = DR(i,j);
}

double nuvens::gerarAleatorio ()
{
	calculos C;
	if (C.random() < 0.5) {
		return -1.0;
	} else {
		return 1.0;
	}
}

vector<document>* nuvens::getDocuments()
{
	return this->valores;
}

unsigned nuvens::getTamanhoDoCluster (const unsigned& cluster)
{
	unsigned count = 0;
	for (unsigned i = 0; i < this->valores->size(); i++)
		for (list<unsigned>::iterator It = (*this->valores)[i].inferidas.begin(); It != (*this->valores)[i].inferidas.end(); It++)
			if (*It == cluster)
				count++;
	return count;
}

void nuvens::setPertinenciaFeatures (vector<bool> V)
{
	for (unsigned i = 0; i < this->valores->size(); i++)
		for (unsigned j = 0; j < (*this->valores)[i].tf_idf.size(); j++)
			(*this->valores)[i].tf_idf[j].pertence = V[j];
}

map<unsigned, vector<bool> > nuvens::getFeatures ()
{
	return this->features;
}

unsigned nuvens::getNumdeGrupos () {
	return this->nclusters;
}

void nuvens::BiSectionKMeans (const unsigned& nclusters, const unsigned& iter, const bool& label,
		const bool& localizada, const bool& global, const bool& localizadaauto, const bool& aleatoria, 
		const unsigned& termos, const int& tipo)
{
	queue<unsigned> fila;
	list<unsigned> leaves;
	vector<bool> fea_selecionadas;
	fea_selecionadas.resize((*this->valores)[0].tf_idf.size());

	unsigned ta = (nclusters * 2) + 1;
	for (unsigned i = 1; i <= ta; i++) {
		fila.push(i);
	}

	unsigned c1, c2;
	unsigned C = 0;
	leaves.push_back(0);

	double rate = double((*this->valores)[0].tf_idf.size()) / double(this->valores->size());

	if (global) {
		featureselection* selecao = new featureselection(this->valores);
		selecao->gerarValoresComTermosSelecionados(0,termos,tipo);
		delete selecao;
	}

	if (aleatoria) {
		featureselection* selecao = new featureselection(this->valores);
//		selecao->gerarValoresComTermosSelecionadosAleatoriamente(termos);
		selecao->gerarValoresComTermosSelecionadosAleatoriamenteComPesos(termos);
		delete selecao;
	}

	for (unsigned j = 0; j < nclusters; j++) {
		Vetor_Leaves.push_back(leaves);
		leaves.remove(C);
		c1 = fila.front();
		fila.pop();
		c2 = fila.front();

		this->Hierarquia.append_child(this->IterTree, c1);
		this->Hierarquia.append_child(this->IterTree, c2);
		leaves.push_back(c1);
		leaves.push_back(c2);
		this->nclusters = c2;
		fila.pop();
		this->classificarN(C,c1,c2,iter);

//		cout << C << " " << c1 << " " << c2 << endl;
		
		C = this->getMaiorCluster();

		this->IterTree = find(this->Hierarquia.begin(), this->Hierarquia.end(), C);

		if (localizada) {
			featureselection* selecao = new featureselection(this->valores);
			selecao->gerarValoresComTermosSelecionados(C,termos,tipo);

			for (unsigned j = 0; j < (*this->valores)[0].tf_idf.size(); j++){
				fea_selecionadas[j] = (*this->valores)[0].tf_idf[j].pertence;
			}

			this->features[C] = fea_selecionadas;
			delete selecao;
		}
		if (localizadaauto) {
			featureselection* selecao = new featureselection(this->valores);
			unsigned t = unsigned(rate * double(this->getTamanhoDoCluster(C)));
//			cout << t << endl;
			selecao->gerarValoresComTermosSelecionados(C,t,tipo);

			for (unsigned j = 0; j < (*this->valores)[0].tf_idf.size(); j++)
				fea_selecionadas[j] = (*this->valores)[0].tf_idf[j].pertence;

			this->features[C] = fea_selecionadas;
			delete selecao;
		}
	}

	if (label) {
		ostringstream out;
		for (unsigned i = 0; i <= this->nclusters; i++) {
			featureselection* selecao = new featureselection(this->valores);
			list<unsigned> termos = selecao->getTermosSelecionadosParaCluster(i,5,tipo);
			delete selecao;
			for (list<unsigned>::iterator It = termos.begin(); It != termos.end(); It++) {
				out << this->termos[*It] << ",";
			}
			this->labels[i] = out.str();
			termos.clear();
			out.str("");
		}
	}
}

void nuvens::classificarN (const unsigned& cluster, const unsigned& classe1, const unsigned& classe2,
		const unsigned& n)
{
//	cout << cluster << " " << classe1 << " " << classe2 << " " << n << endl;
	vector<document> laux = *(this->valores);
	vector<document> melhor;
	double adeq = 0;
	for (unsigned i = 0; i < n; i++) {
		this->classificar(cluster, classe1, classe2);
		if (this->getAdequacao() >= adeq) {
			adeq = this->getAdequacao();
			melhor = *(this->valores);

			this->Centroides[classe1] = this->c1;
			this->Centroides[classe2] = this->c2;
			this->Adequacoes[classe1] = this->adeqC1;
			this->Adequacoes[classe2] = this->adeqC2;
		}
		*(this->valores) = laux;
	}
	*(this->valores) = melhor;
}

void nuvens::classificar (const unsigned& cluster, const unsigned& classe1, const unsigned& classe2)
{
	// escolha dos representantes da etapa de inicialização

	for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((*this->valores)[i].inferida == cluster) {
			(*this->valores)[i].inferida_antes = cluster;
		}
	}

	vector<unsigned> indices;
	unsigned ind1, ind2;

	for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((*this->valores)[i].inferida_antes == cluster) {
			indices.push_back(i);
		}
	}

	calculos C;

	ind1 = C.random(0,indices.size()-1);
	ind2 = C.random(0,indices.size()-1);

	while (ind2 == ind1) {
		ind2 = C.random(0,indices.size()-1);
	}

	unsigned r1 = indices[ind1];
	unsigned r2 = indices[ind2];

	(*this->valores)[r1].inferida = classe1;
	(*this->valores)[r2].inferida = classe2;

	this->c1 = (*this->valores)[r1];
	this->c2 = (*this->valores)[r2];

	double s1, s2;

	// etapa de inicialização
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((*this->valores)[i].inferida_antes == cluster) {
//			s1 = this->Similaridades(i,r1);
//			s2 = this->Similaridades(i,r2);
			s1 = this->similaridade((*this->valores)[i],this->c1);
			s2 = this->similaridade((*this->valores)[i],this->c2);
			if (s1 > s2) {
				(*this->valores)[i].inferida = classe1;
				continue;
			} else {
				(*this->valores)[i].inferida = classe2;
				continue;
			}
		}
	}

    unsigned mudou;
//    unsigned sairfora = 0; // suavizar

    // etapas de representação e classificação
    while (true) {

    	// escolha dos novos representantes de cada classe

    	this->c1 = this->getCentroide(classe1);
    	this->c2 = this->getCentroide(classe2);

		mudou = 0;

		// classificação, até que ninguém mude
		for (unsigned i = 0; i < this->valores->size(); i++) {
			if ((*this->valores)[i].inferida_antes == cluster) {
				s1 = this->similaridade((*this->valores)[i],this->c1);
				s2 = this->similaridade((*this->valores)[i],this->c2);

				if (s1 > s2) {
					if ((*this->valores)[i].inferida == classe2)
						mudou++;
					(*this->valores)[i].inferida = classe1;
					continue;
				} else {
					if ((*this->valores)[i].inferida == classe1)
						mudou++;
					(*this->valores)[i].inferida = classe2;
					continue;
				}
			}
		}
/*		sairfora++; // suavizar
		if (sairfora > 1)// suavizar
			break;// suavizar

 */   	if (mudou != 0) {
    		continue;
    	} else {
    		break;
    	}
    }

	for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((*this->valores)[i].inferida_antes == cluster) {
			(*this->valores)[i].inferidas.push_back((*this->valores)[i].inferida);
		}
	}

	s1 = this->similaridadeGeral(this->c1);
	s2 = this->similaridadeGeral(this->c2);

	this->adeqC1 = s1;
	this->adeqC2 = s2;

	this->adequacao = s1 + s2;
}

double nuvens::getAdequacao()
{
	return this->adequacao;
}

double nuvens::getAdequacaoPorCluster(const unsigned& cluster)
{
	return this->Adequacoes[cluster];
}

vector<int> nuvens::getMapeamento()
{
	vector<int> M;
	M.resize(this->valores->size());
	for (unsigned i = 0; i < this->valores->size(); i++)
		M[i] = (*this->valores)[i].inferida;
	return M;
}

double nuvens::getAdequacaoGeral()
{
	list<unsigned> L;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		L.push_back((*this->valores)[i].inferida);
	}
	L.sort();
	L.unique();

	double a = 0;
	for (list<unsigned>::iterator It = L.begin(); It != L.end(); It++) {
		a += this->getAdequacaoPorCluster(*It);
	}

	return a/double(L.size());
}

unsigned nuvens::getMaiorCluster()
{
	map <unsigned, list<unsigned> > M;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		M[(*this->valores)[i].inferida].push_back(i);
	}
	unsigned aux = 0;
	unsigned cluster = 0;
	for (map<unsigned, list<unsigned> >::iterator It = M.begin(); It != M.end(); It++) {
		if (M[It->first].size() > aux) {
			cluster = It->first;
			aux = M[It->first].size();
		}
	}
	return cluster;
}

vector<document> nuvens::gerarDocuments ()
{
	ifstream fd("./tf-idf.txt", ios::in);
	ifstream fd_tf("./tf.txt", ios::in);
	typedef tokenizer<char_separator<char> > tokenizer;
	char_separator<char> sep_vig(",");
	char_separator<char> sep_ptvig(";");

	document doc_aux;
	termo termo_aux;
	vector<document> docs;
	string str, str2;

	while (!(fd.eof()) && !(fd_tf.eof())) {
		getline(fd, str);
		getline(fd_tf, str2);
		if (!fd.eof()) {
			tokenizer tokens(str, sep_vig);
			tokenizer tokens_tf(str2, sep_vig);
			tokenizer::iterator tok_iter = tokens.begin();
			tokenizer::iterator tok_iter_tf = tokens_tf.begin();
			tokenizer tokens2(*tok_iter, sep_ptvig);
			tokenizer tokens_tf2(*tok_iter_tf, sep_ptvig);
			for (tokenizer::iterator tok_iter2 = tokens2.begin(); tok_iter2 != tokens2.end(); tok_iter2++) {
				termo_aux.valor = calculos::stod(string(*tok_iter2));
				termo_aux.pertence = true;
				doc_aux.tf_idf.push_back(termo_aux);
			}
			for (tokenizer::iterator tok_iter_tf2 = tokens_tf2.begin(); tok_iter_tf2 != tokens_tf2.end(); tok_iter_tf2++) {
				doc_aux.tf.push_back(calculos::stod(string(*tok_iter_tf2)));
			}
			tok_iter++;
			doc_aux.id = unsigned(calculos::stod(string(*tok_iter)));
			tok_iter++;
			doc_aux.classe = unsigned(calculos::stod(string(*tok_iter)));
			doc_aux.inferida = 0;
			docs.push_back(doc_aux);
			doc_aux.tf_idf.clear();
			doc_aux.tf.clear();
		}
	}
	fd.close();
	fd_tf.close();
	return docs;
}

double nuvens::similaridadeGeral (document centroide) // calcula a soma das distâncias de um valor a todos os outros
{
	double b = 0;
	double c = 0;
    for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((*this->valores)[i].inferida == centroide.inferida) {
			b += this->similaridade(centroide, (*this->valores)[i]);
			c++;
		}
    }
    if (c == 0)
    	return 0;
    return b/c;
}

double nuvens::getCriterioParaFeatures(vector<bool> features, vector<int> map)
{
	this->setPertinenciaFeatures(features);

	for (unsigned i = 0; i < this->valores->size(); i++)
		(*this->valores)[i].inferida = map[i];

	list<unsigned> L;
	for (unsigned i = 0; i < this->valores->size(); i++)
		L.push_back((*this->valores)[i].inferida);
	L.sort();
	L.unique();

	double a = 0;
	for (list<unsigned>::iterator It = L.begin(); It != L.end(); It++) {
		a += this->similaridadeGeral(this->getCentroide(*It));
	}
	return a/double(L.size());
}

double nuvens::getThreshold (const unsigned& m)
{
	double dist_interna = 0, dist_externa = 0;
	for (unsigned i = 0; i < this->valores->size(); i++)
		for (unsigned j = 0; j < this->valores->size(); j++)
			if ((i < j) && ((*this->valores)[i].inferida == (*this->valores)[j].inferida))
				dist_interna += this->desimilaridade((*this->valores)[i], (*this->valores)[j]);
	for (unsigned i = 0; i < this->valores->size(); i++)
		for (unsigned j = 0; j < this->valores->size(); j++)
			if ((i < j) && ((*this->valores)[i].inferida != (*this->valores)[j].inferida))
				dist_externa += this->desimilaridade((*this->valores)[i], (*this->valores)[j]);
	return (m * (dist_interna / dist_externa));
}

document nuvens::getCentroide (const int& classe)
{
	vector<termo> V;
	V.resize((*this->valores)[0].tf_idf.size());
	for (unsigned i = 0; i < V.size(); i++) {
		V[i].valor = 0;
	}
	double c = 0;
//#pragma omp parallel for
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((*this->valores)[i].inferida == classe) {
			for (unsigned j = 0; j < V.size(); j++) {
				V[j].valor += (*this->valores)[i].tf_idf[j].valor;
				V[j].pertence = (*this->valores)[i].tf_idf[j].pertence;
			}
			c++;
		}
	}
	if (c != 0)
		for (unsigned i = 0; i < V.size(); i++)
			V[i].valor /= c;
	document saida;
	saida.tf_idf = V;
	saida.inferida = classe;

	return saida;
}

double nuvens::similaridade (document& d1, document& d2) // calcula a similaridade
{
	double s1 = 0, s2 = 0, s3 = 0;
//#pragma omp parallel for reduction(+:s1,s2,s3)
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

	return s1 / (sqrt(s2) * sqrt(s3));
}

double nuvens::desimilaridade (document& d1, document& d2) // calcula a similaridade
{
	double s = 0;
	for (unsigned i = 0; i < d1.tf_idf.size(); i++) {
		if (d1.tf_idf[i].pertence)
			s += pow(d1.tf_idf[i].valor - d2.tf_idf[i].valor,2);
	}
	return sqrt(s);
}

vector<string> nuvens::gerarTermos()
{
	vector<string> V;
	string str;
	ifstream palavras("./palavras.txt", ios::in);
	while (!palavras.eof()) {
		getline(palavras, str);
		V.push_back(str);
	}
	V.pop_back();

	return V;
}

vector<unsigned> nuvens::getTermosDoCluster(const unsigned& cluster)
{
	list<unsigned> T;

	for (unsigned i = 0; i < this->valores->size(); i++) {
		if (this->isInCluster(cluster, (*this->valores)[i])) {
			for (unsigned j = 0; j < (*this->valores)[i].tf_idf.size(); j++) {
				if ((*this->valores)[i].tf_idf[j].valor > 0)
					T.push_back(j);
			}
		}
	}
	T.sort();
	T.unique();

	vector<unsigned> V;

	for (list<unsigned>::iterator It = T.begin(); It != T.end(); It++) {
		V.push_back(*It);
	}

	return V;
}

double nuvens::getQuality (const unsigned& cluster, const unsigned& term)
{
	vector<document> V = this->getDocumentsPorCluster(cluster);
	double t1 = 0, t2 = 0, t3;
	for (unsigned i = 0; i < V.size(); i++) {
		t1 += pow(V[i].tf[term], 2);
		t2 += V[i].tf[term];
	}
	t3 = (pow(t2,2))/double(V.size());
	return t1 - t3;
}

vector<document> nuvens::getDocumentsPorCluster (const unsigned& cluster)
{
	vector<document> V;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if (this->isInCluster(cluster, (*this->valores)[i]))
			V.push_back((*this->valores)[i]);
	}
	return V;
}

bool nuvens::isInCluster(const unsigned& cluster, document& d)
{
	for (list<unsigned>::iterator It = d.inferidas.begin(); It != d.inferidas.end(); It++) {
		if (*It == cluster)
			return true;
	}
	return false;
}

string nuvens::getLabelParaCluster(const unsigned& cluster, int n)
{
	map<unsigned, double> Quality;
	vector<unsigned> Termos = this->getTermosDoCluster(cluster);

	for (unsigned i = 0; i < Termos.size(); i++) {
		Quality[Termos[i]] = this->getQuality(cluster, Termos[i]);
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

	stack<string> pilha;
	ostringstream out;

	for (unsigned i = 0; i < Termos.size(); i++) {
		pilha.push(this->termos[Termos[i]]);
	}

	while (n > 0) {
		if (!(pilha.empty())) {
			if (n != 1) {
				out << pilha.top() << ",";
				pilha.pop();
			} else {
				out << pilha.top();
				pilha.pop();
			}
		}
		n--;
	}
	return out.str();
}

string nuvens::getLabelParaClusterC(const unsigned& cluster)
{
	return this->labels[cluster];
}

vector<list<unsigned> > nuvens::getLeaves ()
{
	return this->Vetor_Leaves;
}

tree<unsigned> nuvens::getHierarquia()
{
	return this->Hierarquia;
}

unsigned nuvens::getNumDocsPorClasse (const unsigned& classe)
{
	unsigned n = 0;
	for (unsigned k = 0; k < this->valores->size(); k++) {
		if ((*this->valores)[k].classe == classe)
			n++;
	}
	return n;
}

/*
double nuvens::getRecall (const unsigned& i, const unsigned& j)
{
	double ni = 0, nij = 0;
	for (unsigned k = 0; k < this->valores.size(); k++) {
		if ((this->valores[k].classe == i))
			ni++;
		if ((this->valores[k].classe == i) && (this->isInCluster(j, valores[k])))
			nij++;
	}
	return nij / ni;
}

double nuvens::getPrecision (const unsigned& i, const unsigned& j)
{
	double nj = 0, nij = 0;
	for (unsigned k = 0; k < this->valores.size(); k++) {
		if (this->isInCluster(j, valores[k]))
			nj++;
		if ((this->valores[k].classe == i) && ((this->isInCluster(j, valores[k]))))
			nij++;
	}
	return nij / nj;
}

double nuvens::getFMeasure(const unsigned& i, const unsigned& j)
{
	double recall = this->getRecall(i,j);
	double precision = this->getPrecision(i,j);
	return ((2 * recall * precision) / (recall + precision));
}

double nuvens::getFMeasurePorClasse(const unsigned& classe)
{
	double max = 0;
	double meas;
	for (unsigned k = 1; k <= this->nclusters; k++) {
		meas = this->getFMeasure(classe,k);
		if (meas > max) {
			max = meas;
		}
	}
	return max;
}

double nuvens::getRecallPorClasse(const unsigned& classe)
{
	double max = 0, ret = 0;
	double meas;
	for (unsigned k = 1; k <= this->nclusters; k++) {
		meas = this->getFMeasure(classe,k);
		if (meas > max) {
			max = meas;
			ret = this->getRecall(classe,k);
		}
	}
	return ret;
}

double nuvens::getPrecisionPorClasse(const unsigned& classe)
{
	double max = 0, ret = 0;
	double meas;
	for (unsigned k = 1; k <= this->nclusters; k++) {
		meas = this->getFMeasure(classe,k);
		if (meas > max) {
			max = meas;
			ret = this->getPrecision(classe,k);
		}
	}
	return ret;
}

double nuvens::getFMeasureGeral()
{
	double recall = this->getRecallGeral();
	double precision = this->getPrecisionGeral();
	return ((2 * recall * precision) / (recall + precision));
}

double nuvens::getRecallGeral()
{
	double f = 0;
	list<unsigned> classes;
	for (unsigned k = 0; k < this->valores.size(); k++) {
		classes.push_back(this->valores[k].classe);
	}
	classes.sort();
	classes.unique();
	for(list<unsigned>::iterator It = classes.begin(); It != classes.end(); It++) {
		f += ((double(this->getNumDocsPorClasse(*It))/double(this->valores.size())) * this->getRecallPorClasse(*It));
	}
	return f;
}

double nuvens::getPrecisionGeral()
{
	double f = 0;
	list<unsigned> classes;
	for (unsigned k = 0; k < this->valores.size(); k++) {
		classes.push_back(this->valores[k].classe);
	}
	classes.sort();
	classes.unique();
	for(list<unsigned>::iterator It = classes.begin(); It != classes.end(); It++) {
		f += ((double(this->getNumDocsPorClasse(*It))/double(this->valores.size())) * this->getPrecisionPorClasse(*It));
	}
	return f;
}

double nuvens::getIDF (const unsigned& cluster, const unsigned& term, vector<document>& V)
{
	double count = 0;
	for (unsigned i = 0; i < V.size(); i++) {
		if (V[i].tf_idf[term] > 0)
			count++;
	}
	return log(double(V.size()) / count);
}
*/
string nuvens::imprimirLabels()
{
	ostringstream fora;
/*	list<unsigned> L;
	for (unsigned i = 0; i < this->valores.size(); i++) {
		L.push_back(this->valores[i].inferida);
	}
	L.sort();
	L.unique();

	for (list<unsigned>::iterator It = L.begin(); It != L.end(); It++) {
		fora << "Cluster " << *It << ": " << this->getLabelParaClusterC(*It) << endl;
	}

*/
	for (unsigned k = 0; k <= this->nclusters; k++) {
		fora << "Cluster " << k << ": " << this->getLabelParaClusterC(k) << endl;
	}
	return fora.str();
}

void nuvens::imprimirHierarquia ()
{
	for (tree<unsigned>::pre_order_iterator It = this->Hierarquia.begin();It != this->Hierarquia.end();It++) {
		cout << *It;
		if (!(*It == 0)) {
			cout << "[" << *(this->Hierarquia.parent(It)) << "] ";
		} else cout << " ";
	}
	cout  << endl;
}
