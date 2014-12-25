#include "stdafx.h"
#include "Tyontekija.h"


Tyontekija::Tyontekija()
{
	string uusiEtunimi = t_etunimi;
	string uusiSukunimi = t_sukunimi;
	string uusiHetu = t_hetu;
	string uusiOsoite = t_osoite;
}


Tyontekija::~Tyontekija()
{
}

//Lisää kirjastoon uuden työntekijän.
void Tyontekija::lisaaTyontekija(string)
{
	cout << "Anna uuden työntekijän sukunimi." << endl;
	cin >> t_sukunimi;
	cout << "Anna uuden työntekijän etunimi." << endl;
	cin >> t_etunimi;
	cout << "Anna työntekijän henkilötunnus" << endl;
	cin >> t_hetu;
	cout << "Anna työntekijän osoite." << endl;
	cin >> t_osoite;
}
