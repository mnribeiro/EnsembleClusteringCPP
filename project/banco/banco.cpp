#include "banco.h"

banco::banco()
{
}

banco::banco(const char* u,const char* p,const char* d, const char* h)
{
	this->host = h; //host do mysql
	this->user = u; //usuario do mysql
	this->pass = p; //senha do mysql
	this->db = d; //database que deseja selecionar
	
	this->sock = mysql_init(0); //aloca ou inicializa um objeto mysql
	if (!(this->sock)) {
		throw handleErro();
	//	cout << "handle ok!" << endl; //verifica se sock não contem erros
	}
	if (!(mysql_real_connect(this->sock, this->host, this->user, this->pass, this->db, 0, NULL, 0))) {
		throw conexaoErro();
//		cout << "Conectado ao esquema " << this->db << " em " << this->host << "." << endl;
	}
}

banco::~banco()
{
	mysql_close(sock);
}

bd_result banco::executaQuery(const string& q)
{
	MYSQL_ROW linha;
	MYSQL_FIELD* colunas;
	unsigned int i;
	string col;
	bd_result R;
	map <string, string> RES;
	
	if(mysql_query(sock,q.c_str())) {
		ostringstream er("");
		er << mysql_error(sock);
		throw queryErro(er.str().c_str(), q,  mysql_errno(sock));
	} else {
//		cout << "Query executada!" << endl;
		result = mysql_store_result(sock);
		if (result)
		{
			colunas = mysql_fetch_fields(result);
			while ((linha=mysql_fetch_row(result)) != NULL)
			{
				for (i=0; i<mysql_num_fields(result); i++) {
					col = colunas[i].name;
					RES[col] = linha[i];
				}
				R.push_back(RES);
			}
		}
	}
	mysql_free_result(result);
	return R;
}
