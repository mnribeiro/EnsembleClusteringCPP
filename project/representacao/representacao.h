#ifndef REPRESENTACAO_H_
#define REPRESENTACAO_H_

#include "../banco/banco.h"
#include "../stemmer/english_stem.h"
#include "../calculos/calculos.h"

class representacao
{
	map<string, list<unsigned> > Corpus;
	map<string, double> Quality;
	map <string, unsigned> Termos;
	unsigned ndocs;
	unsigned ntermos, ntermoselecionados;
	
	double getTF (unsigned& doc_id, const string& term);
	double getIDF (const string& term);
	double getQuality (const string& term);

	void criarArquivoQualitys(const double&);
	
	bool isStopword(const string& s, list<string>& l);
	list<string> getStopwords();
	
	double getQualityPorPorcentagem(const double&);
		
public:

	representacao();
	virtual ~representacao();
	
	void criarArquivo(const double&);
	
	unsigned getNumTermosSelecionados();
	
};

#endif /*REPRESENTACAO_H_*/
