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

//Lis�� kirjastoon uuden ty�ntekij�n.
void Tyontekija::lisaaTyontekija(string)
{
	cout << "Anna uuden ty�ntekij�n sukunimi." << endl;
	cin >> t_sukunimi;
	cout << "Anna uuden ty�ntekij�n etunimi." << endl;
	cin >> t_etunimi;
	cout << "Anna ty�ntekij�n henkil�tunnus" << endl;
	cin >> t_hetu;
	cout << "Anna ty�ntekij�n osoite." << endl;
	cin >> t_osoite;
}
