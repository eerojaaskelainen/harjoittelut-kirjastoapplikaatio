#pragma once
class Henkilo
{
public:
	int ID;
	string etunimi = "";
	string sukunimi = "";
	string hetu = "";
	string osoite = "";

	string kayttajatunnus = "";
	string salasana = "";

public:
	Henkilo();
	~Henkilo();
	bool Lisaa( int uusiID);
	void Lisaa(map<string, string>&tiedot); 
	bool Muokkaa();
	
	map<string, string> haeAssoc();
	string HaeRooli(string salasana);

protected:
	
	static int hae_ai_ID(void);
};

// ::::::::::::::::::::::: ASIAKAS:::::::::::::::::::::::::::::::::::::::::.

class Asiakas : public Henkilo {
public:
	static struct lainanTiedot {
		int kirja_ID;
		time_t lainauspvm = time(NULL);
		time_t tarkistuspvm = 0;
		int lainaAika;
	};
	vector<lainanTiedot> lainat;					//
	double sakot = 0;
private:
	bool lainakielto = false;

public:
	Asiakas();
	Asiakas(string uusiEtunimi, string uusiSukunimi, string uusiHetu, string uusiOsoite, int uusiID);
	bool lisaaLainaus(int kirjaID);
	bool poistaLainaus(int kirjaID);
	double lisaaSakko(vector<Kirja>& kirjasto, double yksikkohinta);
	void asetaLainakielto(bool asetus);
	bool haeLainakielto();
	int haeLainat(vector<lainanTiedot> &lainat, bool aktiiviset);
	string naytaTiedot(void);
	map<string, string> haeAssoc(); //Palauttaa instanssin ominaisuudet ja niihin asetetut arvot tyylillä [ominaisuus] = arvo.
	void lisaaTiedot(map<string, string> tiedot);

	virtual string HaeRooli(string salasana);

protected:
	static int hae_ai_ID(void) {
		static int ai_ID = 1;  // Asiakkaan ID-numeron luontiin käytettävä, joka kerralla yhdellä lisääntyvä ID-numero
		return ai_ID++;
	}
};


// ::::::. TYÖNTEKIJÄ ::::::::::::::::::::::::::::::::::::::::::::::::::::
class Tyontekija : public Henkilo {
public:
	map<int,string> yksikko;			// Mappi, jossa avaimena yksikön ID ja arvona työntekijän käyttäjärooli (sillä sama työntekijä voi olla Turussa admin, ja Salossa vain perustyöläinen)
	
	string HaeRooli(string salasana, int yksikkoID);
public:
	bool Lisaa(int uusiID);
	map<string, string> haeAssoc(); //Palauttaa instanssin ominaisuudet ja niihin asetetut arvot tyylillä ominaisuus = arvo.
	void lisaaTiedot(map<string, string> tiedot);
};