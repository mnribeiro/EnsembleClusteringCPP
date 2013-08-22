#include "calculos.h"

calculos::calculos()
{
}

calculos::~calculos()
{
}

unsigned calculos::random (const unsigned& a, const unsigned& b)
{
	return a + unsigned(this->random() * double(b - a));
}

double calculos::random ()
{
	extern boost::variate_generator<boost::mt19937, uniform_real<> > r;
	return r();
}

double calculos::stod(const string& str) // converter string em double
{
	double r;
	istringstream i(str);
	i >> r;
	return r;
}

string calculos::toLower(string& s)
{
	string dest;
	dest.resize(s.length());
	transform(s.begin(), s.end(), dest.begin(), tolow);
	return dest;
}

wstring calculos::stow (string& s)
{
	wchar_t* UnicodeTextBuffer = new wchar_t[s.length()+1];
	wmemset(UnicodeTextBuffer, 0, s.length()+1);
	mbstowcs(UnicodeTextBuffer, s.c_str(), s.length());
	return UnicodeTextBuffer;
}

string calculos::wtos (wstring& s)
{
	string dest;
	dest.resize(s.length());
	transform(s.begin(), s.end(), dest.begin(), wide_to_narrow);
	return dest;
}

char wide_to_narrow(wchar_t& w)
{
	return char(w);
}

char tolow(char& w)
{
	return tolower(w);
}

double calculos::variancia (vector<double> v)
{
	double media = 0;
	for (unsigned i = 0; i < v.size(); i++)
		media += v[i];
	media /= v.size();
	double soma = 0;
	for (unsigned i = 0; i < v.size(); i++)
		soma += pow(media - v[i], 2);
	return soma / v.size();
}

double calculos::covariancia (vector<double> v1, vector<double> v2)
{
	double media1 = 0, media2 = 0;
	for (unsigned i = 0; i < v1.size(); i++) {
		media1 += v1[i];
		media2 += v2[i];
	}
	media1 /= v1.size();
	media2 /= v2.size();

	double soma = 0;
	for (unsigned i = 0; i < v1.size(); i++)
		soma += (media1 - v1[i])*(media2 - v2[i]);
	return soma / v1.size();
}

double calculos::correlacao (vector<double> v1, vector<double> v2)
{
	return (calculos::covariancia(v1,v2)) / sqrt(calculos::variancia(v1) * calculos::variancia(v2));
}
