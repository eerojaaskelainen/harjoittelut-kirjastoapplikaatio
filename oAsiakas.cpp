#include "stdafx.h"
#include "Asiakas.h"




oAsiakas::oAsiakas() {

	
	asiakasID = hae_ai_ID();
	//asiakasID = ai_asiakasID++;
}
Asiakas::Asiakas(string uusiEtunimi, string uusiSukunimi, string uusiHetu, string uusiOsoite, int uusiID)
{
	a_etunimi = uusiEtunimi;
	a_sukunimi = uusiSukunimi;
	a_hetu = uusiHetu;
	a_osoite = uusiOsoite;
	asiakasID = uusiID;
}


Asiakas::~Asiakas()
{
}

bool Asiakas::lisaaLainaus(int kirjaID) {
	if (lainakielto == true) {
		return false;			//Lainaus ei onnistu, sill‰ asiakkaalla on lainakielto
	}

	else {						// Lis‰t‰‰n lainaus asiakkaan listalle.
		lainat.push_back(kirjaID);
		return true;
	}
}
void Asiakas::asetaLainakielto(bool asetus)

{
}

//Lis‰‰ uuden asiakkaan kirjastoon.
void Asiakas::lisaaAsiakas(void)
{
	cout << "Anna uuden asiakkaan sukunimi." << endl;
	cin >> a_sukunimi;
	cout << "Anna uuden asiakkaan etunimi." << endl;
	cin >> a_etunimi;
	cout << "Anna asiakkaan henkilˆtunnus" << endl;
	cin >> a_hetu;
	cout << "Anna asiakkaan osoite." << endl;
	cin >> a_osoite;
	
	
}
void Asiakas::naytaTiedot(void) {


}

