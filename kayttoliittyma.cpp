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
	//Tulostetaan ruudulle lista haettavan asian ominaisuuksista, joihin k�ytt�j� voi antaa suodatuskriteerins�
	/*
		ARGUT:
			parametrit = nimi/arvopari, johon k�ytt�j� antaa arvon. Esim. ["Kirjan nimi"] = "Talvisota". Tullessa arvo on tyhj�, mutta se p�ivittyy asiakkaan vastauksella.
			otsikko    = Suodatustoiminnon otsikko

		RETURN:
			Boolean: Jos k�ytt�j� keskeytti, palautetaan false. Muuten true.
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
	//Tulostetaan ruudulle lista kentti�, joihin k�ytt�j� voi kirjoittaa ominaisuuksia
	/*
		ARGUT:
			parametrit = Nimi/arvopari (esim. ["Etunimi"] = "Jaakko"), johon k�ytt�j� antaa arvon. Tullessa arvo on tyhj�, mutta se p�ivittyy asiakkaan vastauksella.
			otsikko	   = Otsikko joka n�ytet��n osion yl�osassa

		RETURN:
			Boolean: Jos k�ytt�j� keskeytti, palautetaan false. Muuten true.
	*/

	// Tulee poistumaan, kun saadaan uusi UI kuntoon:
	system("CLS");

	cout << "----------------------------------------------------------------" << endl
		<< "					" << otsikko << endl
		<< "----------------------------------------------------------------" << endl << endl
		<< "Lis�� tiedot (Tyhj� jos haluat ohittaa kriteerin, 0 jos haluat keskeytt�� haun):" << endl << endl;


	
	for (auto &i = parametrit.begin(); i != parametrit.end();) {
		string syote = "";
		cout << i->second.kysymys << ": ";
		getline(cin, syote);
		cout << endl;

		if (syote == "0") {
			//cout << "Sy�tt� keskeytetty..." << endl;
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
	// Tulostetaan ruudulle lista yksil�it�, joista k�ytt�j� valitsee yhden.
	/*
		ARGUT:
			yksilot				= Avain/arvoparit (esim. ["Etunimi"]= "Arto") jokaiselle yksil�lle. Esimerkiksi [1]["Etunimi"] = "Arto".
			valittavaOminaisuus	= Avaimen nimi, josta l�ytyy yksil�n yksil�iv� arvo (esim ID). T�m�n nimen T�YTYY OLLA kaikkien yksil�iden (ks yksilot) yksi avain!
			otsikko				= Listan otsikko
	
		RETURN:
			int : Yksil�n yksil�iv� arvo (esim. ID), jonka k�ytt�j� valitsi. T�m� arvo on sen yksil�n avaimen "valittavaOmaisuus" arvo, jonka k�ytt�j� valitsi.
	*/
	int palauta = 0;

	system("CLS");
	cout << otsikko << endl
		<< "------------------------------" << endl;

	for each (auto yksilo in yksilot) {
		cout << yksilo[valittavaOminaisuus] << ": " << yksilo[valinnanNimike] << endl;
		for each (auto ominaisuus in yksilo) {
			if (ominaisuus.first == valittavaOminaisuus || ominaisuus.first == valinnanNimike || ominaisuus.second == "") {
				// Ei n�ytet� sellaisia jotka jo on n�yt�ll�, tai on tyhj�:
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
		if (teksti[i] == -28) msgboxviesti += 228;		// �

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
		if (teksti[i] == -28) msgboxviesti += 228;		// �
		
		else
			msgboxviesti += wchar_t(teksti[i]);
	}
	
	MessageBox(0, msgboxviesti.c_str(), L"Informaatio", MB_OK | MB_ICONINFORMATION);
	
}

int Kayttoliittyma::Valikko(const map<int, string> &valinnat, const string &otsikko, const string &lisatieto) {
	// Tulostaa valikon:
	/*
	ARGUT:	
		valinnat	= Valikon kohteet nimi/arvoparina. Esim [2] = "Lis�� kirja".
		otsikko		= Valikon otsikko
		lisatieto	= Tarvittaessa n�yt�lle voidaan tulostaa lis�tietoikkuna ennen valintoja. Lis�tietona esimerkiksi valitun kirjan tiedot, tai valitun asiakkaan tiedot
	RETURN:
		int			= K�ytt�j�n tekem� valinta.
	*/
	
	system("CLS");		// Tyhj�t��n ruutu
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
	//Tulostetaan ruudulle lista yksil�n ominaisuuksista, joita k�ytt�j� voi halutessaan muuttaa.
	/*
	ARGUT:
	parametrit = Nimi/arvopari (esim. ["Etunimi"] = "Jaakko"), johon k�ytt�j� antaa arvon. Tullessa arvo on se mit� yksil�ll� on ominaisuutena, p�ivittyy asiakkaan vastauksella, jos vastaus ei ole tyhj�.
	otsikko	   = Otsikko joka n�ytet��n osion yl�osassa

	RETURN:
	Boolean: Jos k�ytt�j� keskeytti, palautetaan false. Muuten true.
	*/

	// Tulee poistumaan, kun saadaan uusi UI kuntoon:
	system("CLS");

	cout << "----------------------------------------------------------------" << endl
		<< "					" << otsikko << endl
		<< "----------------------------------------------------------------" << endl << endl
		<< "Lis�� tiedot (Tyhj� jos et halua muuttaa arvoa, 0 jos haluat keskeytt�� muutosprosessin):" << endl << endl;



	for (auto &i = parametrit.begin(); i != parametrit.end();) {
		string syote = "";
		cout << i->second.kysymys << " ("<< i->second.vastaus <<"): ";
		getline(cin, syote);
		cout << endl;

		if (syote == "0") {
			//cout << "Sy�tt� keskeytetty..." << endl;
			return false;
		}
		else if (syote == "") {
			//K�ytt�j� ei halua muuttaa arvoa.
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
	// Kysyt��n k�ytt�j�lt� yksinkertainen kysymys
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