#pragma once
 class Kayttoliittyma
{
	 /*
	 Sisältää kaikki käyttäjältä kysyttävät ja sille näytettävät toiminnat
	 */
public:
	struct Tiedot{							// Tietojen kyselyyn käytettävä struct
		string kysymys ="";					// Selkokielinen kysymys käyttäjälle
		string vastaus ="";					// Käyttäjän antama vastaus talletetaan tähän
		bool pakollinen = false;			// Onko käyttäjän annettava vastaus?
	};
public:
	Kayttoliittyma();
	~Kayttoliittyma();

	static bool KysySuodatus(map<string, string> &parametrit, string otsikko = "");			// Yksilön suodatuslomakkeen näyttävä funktio. Esim. kirjan hakulomake, asiakkaan hakulomake...
	static bool KysyTiedot(unordered_map<string, Tiedot> &parametrit, string otsikko = "");	// Uuden yksilön lisäyslomake: Kun lisätään uusi kirja/asiakas/..., kysytään siihen arvot täällä
	static int Yksilovalinta(vector<map<string, string>> &yksilot, string valittavaOminaisuus, string valinnanNimike, string otsikko = "");	//Yksilön valinta lyhyestä listasta
	static void Virhe(const string &teksti);												// Virhetiedon näyttö
	static void Informaatio(const string &teksti);											// Informaatiotiedon näyttö
	static int Valikko(const map<int, string> &valinnat, const string &otsikko = "", const string &lisatieto ="");	//Valikot
	static bool MuutaTietoja(unordered_map<string, Tiedot> &tiedot, string otsikko);		// Yksilön tietojen muutoslomake
	static bool Kysy(string kysymys);														// Kysytään käyttäjältä yksinkertainen kysymys
};

