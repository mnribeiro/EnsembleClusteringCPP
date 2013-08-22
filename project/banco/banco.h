//autor: Marcelo Nunes Ribeiro

#ifndef BANCO_H_
#define BANCO_H_

#include "windows.h"
#include "mysql/mysql.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <list>

using namespace std;

typedef list<map<string, string> > bd_result;

class banco
{
	const char *host, *user, *pass, *db;
	MYSQL_RES *result;
	MYSQL *sock;

public:
	banco(const char*,const char*,const char*,const char* = "localhost");
	banco();
	virtual ~banco();
	bd_result executaQuery(const string&);
}; 

// Tratamento de exceções

class bancoExcecoes {
public:
	bancoExcecoes (){}
	virtual ~bancoExcecoes () {}
	virtual const char* what () = 0;
	virtual int getErro () {return 0;}
};

class handleErro : public bancoExcecoes {
public:
	handleErro(){}
	virtual ~handleErro() {}
	const char* what () {return "handle falhou!";}
};

class conexaoErro : public bancoExcecoes {
public:
	conexaoErro(){}
	virtual ~conexaoErro() {}
	const char* what () {return "Conexão falhou! MySQL off?";}
};

class queryErro : public bancoExcecoes {
	string a, e;
	unsigned int b;
public:
	queryErro(){}
	virtual ~queryErro() {} 
	queryErro (const string& c, const string& f, unsigned int d)
	{
		a = c;
		b = d;
		e = f;
	}
	const char* what ()
	{
		ostringstream sai("");
		sai << "Foi encontrado um erro na query: " << e << endl
		<< a << endl
		<< "Erro número: " << b;
		return sai.str().c_str();
	}
	int getErro () {return b;}
};

#endif /*BANCO_H_*/
