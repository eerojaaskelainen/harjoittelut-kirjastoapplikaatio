#pragma once
 class Kayttoliittyma
{
	 /*
	 Sis�lt�� kaikki k�ytt�j�lt� kysytt�v�t ja sille n�ytett�v�t toiminnat
	 */
public:
	struct Tiedot{							// Tietojen kyselyyn k�ytett�v� struct
		string kysymys ="";					// Selkokielinen kysymys k�ytt�j�lle
		string vastaus ="";					// K�ytt�j�n antama vastaus talletetaan t�h�n
		bool pakollinen = false;			// Onko k�ytt�j�n annettava vastaus?
	};
public:
	Kayttoliittyma();
	~Kayttoliittyma();

	static bool KysySuodatus(map<string, string> &parametrit, string otsikko = "");			// Yksil�n suodatuslomakkeen n�ytt�v� funktio. Esim. kirjan hakulomake, asiakkaan hakulomake...
	static bool KysyTiedot(unordered_map<string, Tiedot> &parametrit, string otsikko = "");	// Uuden yksil�n lis�yslomake: Kun lis�t��n uusi kirja/asiakas/..., kysyt��n siihen arvot t��ll�
	static int Yksilovalinta(vector<map<string, string>> &yksilot, string valittavaOminaisuus, string valinnanNimike, string otsikko = "");	//Yksil�n valinta lyhyest� listasta
	static void Virhe(const string &teksti);												// Virhetiedon n�ytt�
	static void Informaatio(const string &teksti);											// Informaatiotiedon n�ytt�
	static int Valikko(const map<int, string> &valinnat, const string &otsikko = "", const string &lisatieto ="");	//Valikot
	static bool MuutaTietoja(unordered_map<string, Tiedot> &tiedot, string otsikko);		// Yksil�n tietojen muutoslomake
	static bool Kysy(string kysymys);														// Kysyt��n k�ytt�j�lt� yksinkertainen kysymys
};

