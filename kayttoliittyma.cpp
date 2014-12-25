#include "stdafx.h"
#include "kayttoliittyma.h"
#include <Windows.h>


Kayttoliittyma::Kayttoliittyma()
{
}


Kayttoliittyma::~Kayttoliittyma()
{
}


bool Kayttoliittyma::KysySuodatus(map<string, string> &parametrit, string otsikko) {
	//Tulostetaan ruudulle lista haettavan asian ominaisuuksista, joihin käyttäjä voi antaa suodatuskriteerinsä
	/*
		ARGUT:
			parametrit = nimi/arvopari, johon käyttäjä antaa arvon. Esim. ["Kirjan nimi"] = "Talvisota". Tullessa arvo on tyhjä, mutta se päivittyy asiakkaan vastauksella.
			otsikko    = Suodatustoiminnon otsikko

		RETURN:
			Boolean: Jos käyttäjä keskeytti, palautetaan false. Muuten true.
	*/

	// Tulee poistumaan, kun saadaan uusi UI kuntoon:
	system("CLS");

	cout << "----------------------------------------------------------------" << endl
		<< "					"<<otsikko 								      << endl
		<< "----------------------------------------------------------------" << endl << endl
		<< "Anna hakukriteerit (Tyhja jos haluat ohittaa kriteerin, 0 jos haluat keskeyttaa haun):" << endl << endl;

	for each (auto kriteeri in parametrit) {
		string syote = "";
		cout << kriteeri.first << ": ";
		getline(cin, syote);
		cout << endl;

		if (syote == "0") {
			cout << "Haku keskeytetty..." << endl;
			return false;
		}
		else {
			parametrit[kriteeri.first] = syote;
		}
	}
	return true;
	// End of Uusi UI
}
bool Kayttoliittyma::KysyTiedot(unordered_map<string, Kayttoliittyma::Tiedot> &parametrit, string otsikko) {
	//Tulostetaan ruudulle lista kenttiä, joihin käyttäjä voi kirjoittaa ominaisuuksia
	/*
		ARGUT:
			parametrit = Nimi/arvopari (esim. ["Etunimi"] = "Jaakko"), johon käyttäjä antaa arvon. Tullessa arvo on tyhjä, mutta se päivittyy asiakkaan vastauksella.
			otsikko	   = Otsikko joka näytetään osion yläosassa

		RETURN:
			Boolean: Jos käyttäjä keskeytti, palautetaan false. Muuten true.
	*/

	// Tulee poistumaan, kun saadaan uusi UI kuntoon:
	system("CLS");

	cout << "----------------------------------------------------------------" << endl
		<< "					" << otsikko << endl
		<< "----------------------------------------------------------------" << endl << endl
		<< "Lisää tiedot (Tyhjä jos haluat ohittaa kriteerin, 0 jos haluat keskeyttää haun):" << endl << endl;


	
	for (auto &i = parametrit.begin(); i != parametrit.end();) {
		string syote = "";
		cout << i->second.kysymys << ": ";
		getline(cin, syote);
		cout << endl;

		if (syote == "0") {
			//cout << "Syöttö keskeytetty..." << endl;
			return false;
		}
		else if (i->second.pakollinen == true && syote == "") {
			cout << "Arvo tulee olla!" << endl;
		}
		else {
			i->second.vastaus = syote;
			++i;
		}
	}
	return true;
	// End of Uusi UI
}




int Kayttoliittyma::Yksilovalinta(vector<map<string, string>> &yksilot, string valittavaOminaisuus, string valinnanNimike, string otsikko){
	// Tulostetaan ruudulle lista yksilöitä, joista käyttäjä valitsee yhden.
	/*
		ARGUT:
			yksilot				= Avain/arvoparit (esim. ["Etunimi"]= "Arto") jokaiselle yksilölle. Esimerkiksi [1]["Etunimi"] = "Arto".
			valittavaOminaisuus	= Avaimen nimi, josta löytyy yksilön yksilöivä arvo (esim ID). Tämän nimen TÄYTYY OLLA kaikkien yksilöiden (ks yksilot) yksi avain!
			otsikko				= Listan otsikko
	
		RETURN:
			int : Yksilön yksilöivä arvo (esim. ID), jonka käyttäjä valitsi. Tämä arvo on sen yksilön avaimen "valittavaOmaisuus" arvo, jonka käyttäjä valitsi.
	*/
	int palauta = 0;

	system("CLS");
	cout << otsikko << endl
		<< "------------------------------" << endl;

	for each (auto yksilo in yksilot) {
		cout << yksilo[valittavaOminaisuus] << ": " << yksilo[valinnanNimike] << endl;
		for each (auto ominaisuus in yksilo) {
			if (ominaisuus.first == valittavaOminaisuus || ominaisuus.first == valinnanNimike || ominaisuus.second == "") {
				// Ei näytetä sellaisia jotka jo on näytöllä, tai on tyhjä:
				continue;
			}
			cout << "\t" << ominaisuus.first << ": " << ominaisuus.second << endl;
		}
		cout << endl;
	}
	cout << endl << "Valintasi: ";
	cin >> palauta;
	cin.sync();



	return palauta;
}
void Kayttoliittyma::Virhe(const string &teksti)
{
	// Tulostaa virheilmoituksen, joka on argumenttina
	wstring msgboxviesti;
	for (unsigned int i = 0; i < teksti.length(); i++) {
		if (teksti[i] == -28) msgboxviesti += 228;		// ä

		else
			msgboxviesti += wchar_t(teksti[i]);
	}

	MessageBox(0, msgboxviesti.c_str(), L"Virhe", MB_OK | MB_ICONERROR);

}
void Kayttoliittyma::Informaatio(const string &teksti)
{
	// Tulostaa informaatioilmoituksen, joka on argumenttina
	wstring msgboxviesti ;
	for (unsigned int i = 0; i < teksti.length(); i++) {
		if (teksti[i] == -28) msgboxviesti += 228;		// ä
		
		else
			msgboxviesti += wchar_t(teksti[i]);
	}
	
	MessageBox(0, msgboxviesti.c_str(), L"Informaatio", MB_OK | MB_ICONINFORMATION);
	
}

int Kayttoliittyma::Valikko(const map<int, string> &valinnat, const string &otsikko, const string &lisatieto) {
	// Tulostaa valikon:
	/*
	ARGUT:	
		valinnat	= Valikon kohteet nimi/arvoparina. Esim [2] = "Lisää kirja".
		otsikko		= Valikon otsikko
		lisatieto	= Tarvittaessa näytölle voidaan tulostaa lisätietoikkuna ennen valintoja. Lisätietona esimerkiksi valitun kirjan tiedot, tai valitun asiakkaan tiedot
	RETURN:
		int			= Käyttäjän tekemä valinta.
	*/
	
	system("CLS");		// Tyhjätään ruutu
	int palauta = 0;

	cout <<otsikko <<endl
		<<"====================="<<endl<<endl
		<< lisatieto << endl << endl
		<< "Valitse toiminto:" << endl
		<< "-----------------------------" << endl;
		

	for each (auto valinta in valinnat) {
		cout << valinta.first << ": "<< valinta.second <<endl ;
	}
		cout << endl << "0 : Poistu" << endl << endl
		<< "Anna valinta: ";

	cin >> palauta;
	cin.sync();
	return palauta;
}

bool Kayttoliittyma::MuutaTietoja(unordered_map<string, Kayttoliittyma::Tiedot> &parametrit, string otsikko) {
	//Tulostetaan ruudulle lista yksilön ominaisuuksista, joita käyttäjä voi halutessaan muuttaa.
	/*
	ARGUT:
	parametrit = Nimi/arvopari (esim. ["Etunimi"] = "Jaakko"), johon käyttäjä antaa arvon. Tullessa arvo on se mitä yksilöllä on ominaisuutena, päivittyy asiakkaan vastauksella, jos vastaus ei ole tyhjä.
	otsikko	   = Otsikko joka näytetään osion yläosassa

	RETURN:
	Boolean: Jos käyttäjä keskeytti, palautetaan false. Muuten true.
	*/

	// Tulee poistumaan, kun saadaan uusi UI kuntoon:
	system("CLS");

	cout << "----------------------------------------------------------------" << endl
		<< "					" << otsikko << endl
		<< "----------------------------------------------------------------" << endl << endl
		<< "Lisää tiedot (Tyhjä jos et halua muuttaa arvoa, 0 jos haluat keskeyttää muutosprosessin):" << endl << endl;



	for (auto &i = parametrit.begin(); i != parametrit.end();) {
		string syote = "";
		cout << i->second.kysymys << " ("<< i->second.vastaus <<"): ";
		getline(cin, syote);
		cout << endl;

		if (syote == "0") {
			//cout << "Syöttö keskeytetty..." << endl;
			return false;
		}
		else if (syote == "") {
			//Käyttäjä ei halua muuttaa arvoa.
			++i;
		}
		else
		{
			i->second.vastaus = syote;
			++i;
		}
	}
	return true;
	// End of Uusi UI
}
bool Kayttoliittyma::Kysy(string kysymys) {
	// Kysytään käyttäjältä yksinkertainen kysymys
	char vastaus = NULL;

	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl
		<< kysymys <<  "(K/E)"<<endl
		<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	cin >> vastaus;

	if (vastaus == 'K' || vastaus == 'k' || vastaus == 'Y' || vastaus == 'y') {
		return true;
	}
	else return false;
	
}