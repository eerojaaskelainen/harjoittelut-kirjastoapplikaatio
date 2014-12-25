#include "stdafx.h"
#include "Kirja.h"

Kirja::Kirja()
{
	//kirjaID = hae_ai_ID();
	lainassa = false;	// Kirja on oletuksena paikalla (ei lainassa)
	lainaaja = 0;		// Kirjalla ei oletuksena ole lainaajaa
}


Kirja::~Kirja()
{
}



bool Kirja::onkoLainassa()
{
	// Tekee tarkistuksen, onko kirja lainassa tällä hetkellä. Tulevaisuudessa tehdään tarkistus kirjan lainaajasta.
	if (lainassa == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Kirja::lainaa(int asiakasID)
{
	// Tekee kirjan lainauksen argumenttina tulevalle asiakas-ID:lle. Palauttaa boolean-vastauksen onnistuiko haku vai ei


	//Tarkistetaan, onko kirja jo lainattu:
	if (onkoLainassa() == true) {
		// Kirja on jo lainattu!!! Ei voida jatkaa
		Kayttoliittyma::Informaatio("Kirja on jo lainassa!");
		return false;
	}
	else {
		// Lainaus voi jatkua:
		// Lisätään kirjan tietoihin lainaavan asiakkaan yksilöllinen tunnus
		lainaaja = asiakasID;
		lainassa = true;
		return true;
	}

}

void Kirja::palauta()
{
	// Tekee kirjan palautuksen, eli asettaa lainaajaksi nollan ja lainassa olemaan false. Tulevaisuudessa "Lainassa" -ominaisuus poistuu

	if (lainaaja != 0)	// Eli kirja on jollakin asiakkaalla lainassa
	{
		lainaaja = 0;

		lainassa = false;
		//cout << "Kirjan palautus onnistui." << endl;
	}
	else
	{
		//cout << "Virhe! Kirja ei ole ollut lainassa." << endl;
	}
}

string Kirja::naytaTiedot()
{
	// Tekee tulostuslistan kirjan tiedoista esim. käyttöliittymän tarpeisiin.


	string palauta =
		"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n" +
		this->teos + string("\n") +
		string("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n") +
		string("Tekijä: ") + this->tekija + string("\n") +
		string("UDK-koodi: ") + this->UDK_koodi + string("\n\n");

	if (this->onkoLainassa() == true) {
		palauta += "Kirja on tällä hetkellä lainassa\n\n";

	}
	return palauta;

}

bool Kirja::lisaaTiedot(int &viimeisinID)
{
	// Uuden kirjan lisäys: Tämä funktio kysyy käyttäjältä syötteitä kirjan ominaisuuksiin
	// Palauttaa boolean arvon onnistuiko kirjan tietojen lisäys.
	ofstream outputfile("resurssit\\Kirjat.txt",ios::app);
	if (!outputfile)
		return false;

	unordered_map<string, Kayttoliittyma::Tiedot> parametrit;

	Kayttoliittyma::Tiedot para1;
	
	para1.kysymys = "Tekijä";
	parametrit["Tekijä"] = para1;

	para1.kysymys = "Teoksen nimi";
	parametrit["Teos"] = para1;

	para1.kysymys = "UDK_koodi";
	parametrit["UDK"] = para1;

	para1.kysymys = "Laina aika";
	parametrit["Laina_aika"] = para1;
	

	if (Kayttoliittyma::KysyTiedot(parametrit, "Uuden kirjan tiedot") == true) {

		this->kirjaID = ++viimeisinID;
		this->tekija = parametrit["Tekijä"].vastaus;
		this->teos = parametrit["Teos"].vastaus;
		this->UDK_koodi = parametrit["UDK"].vastaus;

		// Onko tähän viisaampaa ratkaisua?
		this->laina_aika = stoi(parametrit["Laina_aika"].vastaus);
		
		// Tiedostoon lisäys
		outputfile << this->kirjaID<<"|"
			<<this->tekija << "|"
			<<this->teos   <<"|"
			<<this->UDK_koodi << "|"
			<<this->laina_aika << "|" 
			<<this->lainaaja << "|" 
			<<this->lainassa << "|" << endl;
		outputfile.close();

		return true;
	}
	else { return false; }

}
map<string, string> Kirja::haeAssoc(void) {
	// Kirja-olion getteri: Palauttaa Mapin, jossa avaimena on olion ominaisuudet käyttäjäystävällisessä muodossa

	map<string, string> tiedot;

	tiedot["ID-numero"] = to_string(this->kirjaID);
	tiedot["Teoksen nimi"] = this->teos;
	tiedot["Tekijä"] = this->tekija;
	tiedot["UDK-koodi"] = this->UDK_koodi;
	tiedot["Laina_aika"] = to_string(this->laina_aika);
	tiedot["Lainaaja"] = to_string(this->lainaaja);
	if (this->onkoLainassa() == true) {
		tiedot["Lainassa"] = "1";
	}
	else {
		tiedot["Lainassa"] = "0";
	}

	return tiedot;
}

void Kirja::lisaaTiedot(map<string, string> tiedot) {
	// Kirja-olion setteri:
	// Tämä funktio muuttaa olionsa ominaisuuksia perustuen argumenttina tuotuun map:iin.
	// Mapin avaimina samat käyttäjäystävälliset ominaisuusnimet kuin Kirja-olion getteristä palautuu.

	this->kirjaID = stoi(tiedot["ID-numero"]);
	this->teos = tiedot["Teoksen nimi"];
	this->tekija = tiedot["Tekijä"];
	this->UDK_koodi = tiedot["UDK-koodi"];
	this->laina_aika = stoi(tiedot["Laina_aika"]);

	if (stoi(tiedot["Lainassa"]) == 1) {
		this->lainassa = true;
	}
	else {
		this->lainassa = false;
	}

	this->lainaaja = stoi(tiedot["Lainaaja"]);


}

bool Kirja::muutaTietoja() {
	bool onnistui = false;

	map<string, string> ominaisuudet;
	ominaisuudet = this->haeAssoc();
	unordered_map<string, Kayttoliittyma::Tiedot> uudet;

	for each (auto ominaisuus in ominaisuudet) {
		if (ominaisuus.first == "ID-numero") continue;
		if (ominaisuus.first == "Lainassa") continue;
		if (ominaisuus.first == "Lainaaja") continue;

		Kayttoliittyma::Tiedot uusi;
		uusi.kysymys = ominaisuus.first;
		uusi.vastaus = ominaisuus.second;
		uudet[ominaisuus.first] = uusi;
	}

	if (Kayttoliittyma::MuutaTietoja(uudet, "Muuta kirjan " + this->teos + " tietoja")) {
		for each (auto muutettu in uudet) {
			ominaisuudet[muutettu.first] = muutettu.second.vastaus;
		}

		this->lisaaTiedot(ominaisuudet);
		onnistui = true;
	}
	return onnistui;
}