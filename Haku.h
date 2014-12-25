#pragma once

class Haku {
	/*
		Hakutoiminnot sis�lt�v� luokka. Kirjan- sek� asiakkaan hakuluokat tarvitsevat t�m�n p��luokan.
	*/

public:		//Julkiset ominaisuudet
	
	enum HaunTulos{
		HAKU_OK_MONTA,			//Tuloksia l�ytyi monta
		HAKU_OK_YKSI,			// Yksi tulos l�ytyi	
		HAKU_EI_OSUMIA,			// Osumia ei ole
		HAKU_LOPETA,			// K�ytt�j� haluaa lopettaa
		HAKU_VAARA_HAKU			// Annettu tuntematon hakukriteeri				
	};

	
	HaunTulos tilanne;			//Mik� on t�m�n hetkinen tilanne hakuoliossa?

protected:	//Vain aliluokille tarjottavat ominaisuudet
	map<string, string>  hakukriteerit;						// T�ss� haussa k�ytett�v�t hakukriteerit, jotka sis�lt�v�t k�ytt�j�n antamat suodatusarvot
	vector<map<string, string>> osumat;						// Kaikkien (karsinnassa hakukriteereihin osuneiden) yksil�iden ominaisuudet: esim. [1]["Kirjan nimi"] = "Talvisota"



public:		// Julkiset metodit
	static int nouda_kirjat_tiedostosta(string tiedosto, vector<Kirja> &kirjat);			// Tiedostonk�sittely: Kirjojen nouto T�m� versio toimii threadien kanssa (tulee olla static...)
	static int nouda_asiakkaat_tiedostosta(string tiedosto, vector<Asiakas> &asiakkaat);	// Tiedostonk�sittely: Asiakkaiden nouto
	
	static void tallenna_kirjat_tiedostoon(string tiedosto, vector<Kirja> &kirjat);		// Tiedostonk�sittely: Kirjojen tallennus
	static void tallenna_asiakkaat_tiedostoon(string tiedosto, vector<Asiakas> &asiakkaat); // Tiedostonk�sittely: Asiakkaiden tallennus

	static int nouda_tyontekijat_tiedostosta(string tiedosto, vector<Tyontekija> &tyontekijat);	// Tiedostonk�sittely: Ty�ntekij�iden nouto
	static void tallenna_tyontekijat_tiedostoon(string tiedosto, vector<Tyontekija> &tyontekijat); // Tiedostonk�sittely: Ty�ntekij�iden tallennus


protected:	// Vain aliluokille tarjottavat metodit
	void PaivitaTilanne();												// P�ivitt�� tilanne-ominaisuuden sen mukaan kuinka paljon osumat -ominaisuudessa on yksil�it�.
	void KarsiOsumat(pair<string, string> kriteeri, bool tiukka);		// Itse haku: Karsii Osumat -ominaisuuden yksil�it� annetun kriteerin mukaan
};

//::::::::::::::::: KIRJAN HAKU ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::.

class KirjaHaku : public Haku {
private:
	vector<Kirja*>hakulista;							// Vektori Kirjasto-luokan kirjojen osoittimia

public:
	KirjaHaku(vector<Kirja>&);							//Kirjahaun constructor
	Kirja* NoudaKirja();								// Kirja-pointterin hakumetodi. Kysyy k�ytt�j�lt� hakukriteerit ja palauttaa kriteereihin osuvan kirjan
	Kirja* NoudaKirja(unsigned int kirjaID);			//Kirja-pointterin haku ID-tiedolla

};

//:::::::::::::::: ASIAKKAAN HAKU ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
class AsiakasHaku : public Haku {
private:
	vector<Asiakas*> hakulista;							// Vektori Kirjasto-luokan asiakkaiden osoittimia
public:
	AsiakasHaku(vector<Asiakas>& myHakulista);			//Asiakashaun constructor
	Asiakas* NoudaAsiakas();							//Asiakas-pointterin hakumetodi. Kysyy k�ytt�j�lt� hakukriteerit ja palauttaa kriteereihin osuvan asiakkaan
	Asiakas* NoudaAsiakas(unsigned int asiakasID);		//Asiakas-pointterin haku ID-tiedolla
};


//:::::::::::::::: TY�NTEKIJ�N HAKU ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
class TyontekijaHaku : public Haku {
private:
	vector<Tyontekija*> hakulista;							// Vektori Kirjasto-luokan ty�ntekij�iden osoittimia
public:
	TyontekijaHaku(vector<Tyontekija>& myHakulista);			//Ty�ntekij�haun constructor
	Tyontekija* NoudaTyontekija();								//Ty�ntekij�-pointterin hakumetodi. Kysyy k�ytt�j�lt� hakukriteerit ja palauttaa kriteereihin osuvan ty�ntekij�n
	Tyontekija* NoudaTyontekija(unsigned int tyontekijaID);		//Ty�ntekij�-pointterin haku ID-tiedolla
};