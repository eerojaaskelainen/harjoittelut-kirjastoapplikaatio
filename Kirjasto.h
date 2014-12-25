#pragma once

class Kirjasto
{
public:
	enum Kayttajaprofiili {
		USER_ANONYYMI =0,
		USER_ASIAKAS = 5,
		USER_VIRKAILIJA = 50,
		USER_ADMIN = 100,
		USER_INVALID = -1
	};
	int yksikko = 0;			// Kirjaston yksikkö-ID
	Asiakas* nykyinenKayttajaAsiakas = nullptr;
	Tyontekija* nykyinenKayttajaTyontekija = nullptr;


private:
	vector<Kirja> kirjat;
	int viimeisinKirjaID = 0;
	vector<Asiakas> asiakkaat;
	int viimeisinAsiakasID = 0;
	vector<Tyontekija> tyontekijat;
	int viimeisinTyontekijaID = 0;
	const double sakonYksikkohinta = 1.00;



public:
	Kirjasto();
	~Kirjasto();

	void lisaaAsiakas();	
	void lisaaLainaus(Kirja *kirja = nullptr, Asiakas *asiakas = nullptr);
	Tyontekija lisaaTyontekija();

	string naytaAsiakkaanTiedot(Asiakas*);
	
	//EJ: Interfacet Main-funktiolle:
	void main_haeAsiakas();
	void main_lisaaKirja();		//EJ:Siirretty public:ksi ja yhtenäistetty nimeä (+main_)
	void main_haeKirja();
	void main_haeTyontekija();

	Kayttajaprofiili TarkistaOikeudet(string kayttajatunnus, string salasana);


protected:
	void poistaKirja(int kirjaID);
	Asiakas* haeAsiakas();
	void poistaAsiakas(unsigned int asiakasID);
	void poistaTyontekija(unsigned int tyontekijaID);
	void poistaLainaus(Kirja *kirja);
	Kirja* haeKirja();			// EJ:Siirretty protected:ksi ja muutettu palauttamaan pointer



};

