#ifndef CALCULOS_H_
#define CALCULOS_H_

#include <iostream>

#include <list>
#include <cmath>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <sstream>
#include <boost/tr1/random.hpp>

using namespace std;
using namespace std::tr1;
using namespace boost;

struct termo {
	bool pertence;
	double valor;
};

struct document {
	bool ehExcluido;
	string descricao, titulo, url;
	unsigned classe;
	int inferida;
	unsigned inferida_antes;
	list<unsigned> inferidas;
	unsigned id;
	vector<termo> tf_idf;
	vector<double> tf;
	double densidade;
	list<unsigned> representantes;
};

char wide_to_narrow(wchar_t& w);
char tolow(char& w);

class calculos
{
public:
	calculos();
	virtual ~calculos();

	static double stod(const string&);
	static string toLower(string&);
	static wstring stow (string&);
	static string wtos (wstring&);
	unsigned random (const unsigned&, const unsigned&);
	double random ();
	static double variancia (vector<double>);
	static double covariancia (vector<double>, vector<double>);
	static double correlacao (vector<double>, vector<double>);
};

#endif /*CALCULOS_H_*/
