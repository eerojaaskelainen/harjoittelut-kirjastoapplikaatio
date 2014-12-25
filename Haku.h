#pragma once

class Haku {
	/*
		Hakutoiminnot sisältävä luokka. Kirjan- sekä asiakkaan hakuluokat tarvitsevat tämän pääluokan.
	*/

public:		//Julkiset ominaisuudet
	
	enum HaunTulos{
		HAKU_OK_MONTA,			//Tuloksia löytyi monta
		HAKU_OK_YKSI,			// Yksi tulos löytyi	
		HAKU_EI_OSUMIA,			// Osumia ei ole
		HAKU_LOPETA,			// Käyttäjä haluaa lopettaa
		HAKU_VAARA_HAKU			// Annettu tuntematon hakukriteeri				
	};

	
	HaunTulos tilanne;			//Mikä on tämän hetkinen tilanne hakuoliossa?

protected:	//Vain aliluokille tarjottavat ominaisuudet
	map<string, string>  hakukriteerit;						// Tässä haussa käytettävät hakukriteerit, jotka sisältävät käyttäjän antamat suodatusarvot
	vector<map<string, string>> osumat;						// Kaikkien (karsinnassa hakukriteereihin osuneiden) yksilöiden ominaisuudet: esim. [1]["Kirjan nimi"] = "Talvisota"



public:		// Julkiset metodit
	static int nouda_kirjat_tiedostosta(string tiedosto, vector<Kirja> &kirjat);			// Tiedostonkäsittely: Kirjojen nouto Tämä versio toimii threadien kanssa (tulee olla static...)
	static int nouda_asiakkaat_tiedostosta(string tiedosto, vector<Asiakas> &asiakkaat);	// Tiedostonkäsittely: Asiakkaiden nouto
	
	static void tallenna_kirjat_tiedostoon(string tiedosto, vector<Kirja> &kirjat);		// Tiedostonkäsittely: Kirjojen tallennus
	static void tallenna_asiakkaat_tiedostoon(string tiedosto, vector<Asiakas> &asiakkaat); // Tiedostonkäsittely: Asiakkaiden tallennus

	static int nouda_tyontekijat_tiedostosta(string tiedosto, vector<Tyontekija> &tyontekijat);	// Tiedostonkäsittely: Työntekijöiden nouto
	static void tallenna_tyontekijat_tiedostoon(string tiedosto, vector<Tyontekija> &tyontekijat); // Tiedostonkäsittely: Työntekijöiden tallennus


protected:	// Vain aliluokille tarjottavat metodit
	void PaivitaTilanne();												// Päivittää tilanne-ominaisuuden sen mukaan kuinka paljon osumat -ominaisuudessa on yksilöitä.
	void KarsiOsumat(pair<string, string> kriteeri, bool tiukka);		// Itse haku: Karsii Osumat -ominaisuuden yksilöitä annetun kriteerin mukaan
};

//::::::::::::::::: KIRJAN HAKU ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::.

class KirjaHaku : public Haku {
private:
	vector<Kirja*>hakulista;							// Vektori Kirjasto-luokan kirjojen osoittimia

public:
	KirjaHaku(vector<Kirja>&);							//Kirjahaun constructor
	Kirja* NoudaKirja();								// Kirja-pointterin hakumetodi. Kysyy käyttäjältä hakukriteerit ja palauttaa kriteereihin osuvan kirjan
	Kirja* NoudaKirja(unsigned int kirjaID);			//Kirja-pointterin haku ID-tiedolla

};

//:::::::::::::::: ASIAKKAAN HAKU ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
class AsiakasHaku : public Haku {
private:
	vector<Asiakas*> hakulista;							// Vektori Kirjasto-luokan asiakkaiden osoittimia
public:
	AsiakasHaku(vector<Asiakas>& myHakulista);			//Asiakashaun constructor
	Asiakas* NoudaAsiakas();							//Asiakas-pointterin hakumetodi. Kysyy käyttäjältä hakukriteerit ja palauttaa kriteereihin osuvan asiakkaan
	Asiakas* NoudaAsiakas(unsigned int asiakasID);		//Asiakas-pointterin haku ID-tiedolla
};


//:::::::::::::::: TYÖNTEKIJÄN HAKU ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
class TyontekijaHaku : public Haku {
private:
	vector<Tyontekija*> hakulista;							// Vektori Kirjasto-luokan työntekijöiden osoittimia
public:
	TyontekijaHaku(vector<Tyontekija>& myHakulista);			//Työntekijähaun constructor
	Tyontekija* NoudaTyontekija();								//Työntekijä-pointterin hakumetodi. Kysyy käyttäjältä hakukriteerit ja palauttaa kriteereihin osuvan työntekijän
	Tyontekija* NoudaTyontekija(unsigned int tyontekijaID);		//Työntekijä-pointterin haku ID-tiedolla
};