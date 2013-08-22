#include "avaliacao.h"

avaliacao::avaliacao()
{
}

avaliacao::avaliacao(vector<document>* valores)
{
	this->valores = valores;
	this->calcularRepresentantes();
}

avaliacao::~avaliacao()
{
}

unsigned avaliacao::majoritario (list<unsigned>& L)
{
	map <unsigned, list<unsigned> > M;
	for (list<unsigned>::iterator It = L.begin(); It != L.end(); It++) {
		M[*It].push_back(*It);
	}
	unsigned aux = 0;
	unsigned escolhido = 0;
	for (map<unsigned, list<unsigned> >::iterator It = M.begin(); It != M.end(); It++) {
		if (M[It->first].size() > aux) {
			escolhido = It->first;
			aux = M[It->first].size();
		}
	}
	return escolhido;
}

void avaliacao::calcularRepresentantes()
{
	map <unsigned, list<unsigned> > M;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		M[(*this->valores)[i].inferida].push_back((*this->valores)[i].classe);
	}

	for (map<unsigned, list<unsigned> >::iterator It = M.begin(); It != M.end(); It++) {
		this->RepresentantesGC[It->first] = this->majoritario(It->second);
	}
}

double avaliacao::alpha (const unsigned& c)
{
	double count = 0;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((this->RepresentantesGC[(*this->valores)[i].inferida] == (*this->valores)[i].classe)
				&& (this->RepresentantesGC[(*this->valores)[i].inferida] == c))
			count++;
	}
	return count;
}

double avaliacao::beta (const unsigned& c)
{
	double count = 0;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((this->RepresentantesGC[(*this->valores)[i].inferida] != (*this->valores)[i].classe)
				&& (this->RepresentantesGC[(*this->valores)[i].inferida] == c))
			count++;
	}
	return count;
}

double avaliacao::gamma (const unsigned& c)
{
	double count = 0;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((this->RepresentantesGC[(*this->valores)[i].inferida] != (*this->valores)[i].classe)
				&& ((*this->valores)[i].classe == c))
			count++;
	}
	return count;
}

double avaliacao::alphaTotal ()
{
	list<unsigned> L;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		L.push_back((*this->valores)[i].classe);
	}
	L.sort();
	L.unique();

	double saida = 0;

	for (list<unsigned>::iterator It = L.begin(); It != L.end(); It++) {
		saida += this->alpha(*It);
	}
	return saida;
}

double avaliacao::betaTotal ()
{
	list<unsigned> L;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		L.push_back((*this->valores)[i].classe);
	}
	L.sort();
	L.unique();

	double saida = 0;

	for (list<unsigned>::iterator It = L.begin(); It != L.end(); It++) {
		saida += this->beta(*It);
//		cout << "Beta para classe " << *It << ": " << this->beta(*It) << endl;
	}
	return saida;
}

double avaliacao::gammaTotal ()
{
	list<unsigned> L;
	for (unsigned i = 0; i < this->valores->size(); i++) {
		L.push_back((*this->valores)[i].classe);
	}
	L.sort();
	L.unique();

	double saida = 0;

	for (list<unsigned>::iterator It = L.begin(); It != L.end(); It++) {
		saida += this->gamma(*It);
//		cout << "Gamma para classe " << *It << ": " << this->gamma(*It) << endl;
	}
	return saida;
}

double avaliacao::getPrecisao()
{
	double alpha = this->alphaTotal();
	double beta = this->betaTotal();
	double saida = alpha / (alpha + beta);
	return saida;
}

double avaliacao::getCobertura()
{
	double alpha = this->alphaTotal();
	double gamma = this->gammaTotal();
	double saida = alpha / (alpha + gamma);
	return saida;
}

void avaliacao::imprimirRepresentantes()
{
	for (map<unsigned, unsigned>::iterator It = this->RepresentantesGC.begin(); It != this->RepresentantesGC.end(); It++) {
		cout << "Grupo " << It->first << " Classe " << It->second << endl;
	}
}

void avaliacao::inserirInferidasTreeSOM(const char* c)
{
	ifstream fd(c, ios::in);
	typedef tokenizer<char_separator<char> > tokenizer;
	char_separator<char> sep(" 	");

	vector<unsigned> v;
	string str;
	map<unsigned,unsigned> doc_to_class;

	while (!(fd.eof())) {
		getline(fd, str);
		if (!fd.eof()) {
			tokenizer tokens(str, sep);
			for (tokenizer::iterator tok = tokens.begin(); tok != tokens.end(); tok++)
				v.push_back(unsigned(calculos::stod(string(*tok))));
			for (unsigned i = 1; i < v.size(); i++)
				doc_to_class[v[i]] = v[0];
			v.clear();
		}
	}
	for (unsigned i = 0; i < this->valores->size(); i++) {
		(*this->valores)[i].inferida = doc_to_class[i+1];
	}
	for (unsigned i = 0; i < this->valores->size(); i++) {
		cout << (*this->valores)[i].inferida << " ";
	} cout << endl;
	fd.close();
}

document avaliacao::getCentroide (const int& grupo)
{
	vector<termo> V;
	V.resize((*this->valores)[0].tf_idf.size());
	for (unsigned i = 0; i < V.size(); i++) {
		V[i].valor = 0;
	}
	double c = 0;
    for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((*this->valores)[i].inferida == grupo) {
			for (unsigned j = 0; j < V.size(); j++) {
				V[j].valor += (*this->valores)[i].tf_idf[j].valor;
				V[j].pertence = (*this->valores)[i].tf_idf[j].pertence;
			}
			c++;
		}
    }

	for (unsigned i = 0; i < V.size(); i++) {
		V[i].valor /= c;
	}
	document saida;
	saida.tf_idf = V;
	saida.inferida = grupo;

    return saida;
}

double avaliacao::similaridadeGeral ()
{
	list<unsigned> grupos;
    for (unsigned i = 0; i < this->valores->size(); i++)
    	grupos.push_back((*this->valores)[i].inferida);
    grupos.sort();
    grupos.unique();

    double soma = 0;
    document aux;
    for (list<unsigned>::iterator It = grupos.begin(); It != grupos.end(); It++) {
    	aux = this->getCentroide(*It);
    	soma += this->similaridadeGeralPorGrupo(aux);
    }
    return soma / grupos.size();
}

double avaliacao::similaridadeGeralPorGrupo (document& centroide)
{
	double b = 0;
	double c = 0;
	nuvens aux;
    for (unsigned i = 0; i < this->valores->size(); i++) {
		if ((*this->valores)[i].inferida == centroide.inferida) {
			b += aux.similaridade(centroide, (*this->valores)[i]);
			c++;
		}
    }
    return b/c;
}
