#include "representacao.h"

representacao::representacao()
{
}

representacao::~representacao()
{
}

double representacao::getTF (unsigned& doc_id, const string& term)
{
	list<unsigned> L = this->Corpus[term];
	unsigned count = 0;

	for (list<unsigned>::iterator It = L.begin(); It != L.end(); It++) {
		if (*It == doc_id)
			count++;
	}
	if (count == 0) {
		return 0;
	} else {
		return 1 + log(double(count));
	}
}

double representacao::getIDF (const string& term)
{
	list<unsigned> L = this->Corpus[term];
	L.sort();
	L.unique();
	return log(double(this->ndocs)/double(L.size()));
}
/*
double representacao::getQuality (const string& term)
{
	double t1 = 0, t2 = 0, t3;
	for (unsigned i = 1; i <= ndocs; i++) {
		t1 += pow(this->getTF(i, term), 2);
	}
	for (unsigned i = 1; i <= ndocs; i++) {
		t2 += this->getTF(i, term);
	}
	t3 = (pow(t2,2))/double(ndocs);
	return t1 - t3;
}
*/

double representacao::getQuality (const string& term)
{
	return this->Corpus[term].size();
}

list<string> representacao::getStopwords()
{
	list<string> L;
	string str;
	ifstream fd("./stopwords.txt", ios::in);
	while (! fd.eof() ) {
		getline(fd, str);
		L.push_back(str);
	}
	return L;
}

bool representacao::isStopword(const string& s, list<string>& l)
{
	for (list<string>::iterator It = l.begin(); It != l.end(); It++) {
		if (*It == s)
			return true;
	}
	return false;
}

void representacao::criarArquivo(const double& por)
{
	typedef tokenizer<char_separator<char> > tokenizer;
	char_separator<char> sep("%&+-`*'<>,:!.?=\\\"()/0123456789\n; ");
	stemming::english_stem<> StemEnglish;
	ofstream file("./tf-idf.txt", ios::out);
	ofstream test("./teste.txt", ios::out);
	ofstream file_tf("./tf.txt", ios::out);

//	wstring word;
//	string oword, p;

	this->ndocs = 0;
	unsigned doc;
	list<string> stopwords = this->getStopwords();

	try {
		banco* bd = new banco("rcv1-v2","rcv1-v2","rcv1-v2");
		bd_result res = bd->executaQuery("SELECT * FROM conteudo");
		bd_result::iterator It;
		for (It = res.begin(); It != res.end(); It++) { // Criação do corpus
			tokenizer tokensDescricao((*It)["descricao"], sep);
//			tokenizer tokensTitulo((*It)["titulo"], sep);
			doc = unsigned(calculos::stod((*It)["id"]));
			this->ndocs++;

			for (tokenizer::iterator tok_iter = tokensDescricao.begin(); tok_iter != tokensDescricao.end(); tok_iter++) {
//				word = calculos::stow(*(new string(*tok_iter)));
//				StemEnglish(word);
//				oword = calculos::toLower(*(new string(calculos::wtos(word))));
//				if (!(this->isStopword(oword, stopwords)))
					this->Corpus[*tok_iter].push_back(doc);
					test << *tok_iter << " -> " << this->Corpus[*tok_iter].size() << " -> " << doc << endl;
			}

			test << this->Corpus.size() << endl;
/*
			for (tokenizer::iterator tok_iter = tokensTitulo.begin(); tok_iter != tokensTitulo.end(); tok_iter++) {
				word = calculos::stow(*(new string(*tok_iter)));
				StemEnglish(word);
				oword = calculos::toLower(*(new string(calculos::wtos(word))));
				if (!(this->isStopword(oword, stopwords)))
					this->Corpus[oword].push_back(doc);
			}
*/
		}

		for (map<string, list<unsigned> >::iterator Itmap = this->Corpus.begin(); Itmap != this->Corpus.end(); Itmap++) {
//			cout << Itmap->first << " " << this->Corpus[Itmap->first].size() << endl;
			this->Quality[Itmap->first] = this->getQuality(Itmap->first);
		}

		this->criarArquivoQualitys(por);

		double tf;

		for (It = res.begin(); It != res.end(); It++) { // Cálculo do TF-IDF
			doc = unsigned(calculos::stod((*It)["id"]));
			this->ntermoselecionados = 0;
			for (map<string, list<unsigned> >::iterator Itmap = this->Corpus.begin(); Itmap != this->Corpus.end(); Itmap++) {
				if (this->Quality[Itmap->first] > por) {
					tf = this->getTF(doc, Itmap->first);
					file_tf << tf << ";";
					file << tf * getIDF(Itmap->first) << ";";
					this->ntermoselecionados++;
				}
			}
			file_tf << "," << doc << "," << (*It)["classe"] << endl;
			file << "," << doc << "," << (*It)["classe"] << endl;
		}

		file.close();
		file_tf.close();
		delete bd;
	} catch (bancoExcecoes& e) {
		cout << e.what() << endl;
	}
}

void representacao::criarArquivoQualitys(const double& p)
{
	ofstream fpalavras("./palavras.txt", ios::out);
	ofstream fquality("./quality.txt", ios::out);

	for (map<string, list<unsigned> >::iterator Itmap = this->Corpus.begin(); Itmap != this->Corpus.end(); Itmap++) {
		if (this->Quality[Itmap->first] > p) {
			fpalavras << Itmap->first << endl;
			fquality << this->Quality[Itmap->first] << endl;
		}
	}
}

double representacao::getQualityPorPorcentagem(const double& p)
{
	unsigned quant = unsigned(double(this->ndocs) * p);
	list<double> L;
	for (map<string, double>::iterator Itmap = this->Quality.begin(); Itmap != this->Quality.end(); Itmap++) {
		L.push_back(Itmap->second);
	}
	L.sort();
	vector<double> V;
	for(list<double>::iterator It = L.begin(); It != L.end(); It++) {
		V.push_back(*It);
	}
	return V[this->ndocs - quant];
}

unsigned representacao::getNumTermosSelecionados()
{
	return this->ntermoselecionados;
}
