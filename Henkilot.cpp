#include "stdafx.h"
#include "Henkilot.h"


Henkilo::Henkilo()
{
}


Henkilo::~Henkilo()
{
}

bool Henkilo::Lisaa( int uusiID) {
	// Uuden asiakkaan lis�ys: T�m� funktio kysyy k�ytt�j�lt� sy�tteit� asiakkaan ominaisuuksiin
	// Palauttaa boolean arvon onnistuiko asiakkaan tietojen lis�ys.

	unordered_map<string, Kayttoliittyma::Tiedot> parametrit;

	Kayttoliittyma::Tiedot para1;

	para1.kysymys = "Sukunimi";
	para1.pakollinen = true;
	parametrit["Sukunimi"] = para1;

	para1.kysymys = "Etunimi";
	para1.pakollinen = true;
	parametrit["Etunimi"] = para1;

	para1.kysymys = "Henkil�tunnus";
	para1.pakollinen = false;
	parametrit["Henkil�tunnus"] = para1;

	para1.kysymys = "Osoite";
	para1.pakollinen = false;
	parametrit["Osoite"] = para1;

	para1.kysymys = "K�ytt�j�tunnus";
	para1.pakollinen = false;
	parametrit["K�ytt�j�tunnus"] = para1;

	para1.kysymys = "Salasana";
	para1.pakollinen = false;
	parametrit["Salasana"] = para1;

	if (Kayttoliittyma::KysyTiedot(parametrit, "Uuden henkil�n tiedot") == true) {
		this->ID = uusiID;
		this->sukunimi = parametrit["Sukunimi"].vastaus;
		this->etunimi = parametrit["Etunimi"].vastaus;
		this->hetu = parametrit["Henkil�tunnus"].vastaus;
		this->osoite = parametrit["Osoite"].vastaus;
		this->kayttajatunnus = parametrit["K�ytt�j�tunnus"].vastaus;
		this->salasana = parametrit["Salasana"].vastaus;

		return true;
	}
	else { return false; }
}

void Henkilo::Lisaa(map<string, string> &tiedot) {
	// Setteri: Asettaa argumenttina tuodut ominaisuudet t�m�n olion ominaisuuksiksi.
	this->ID = stoi(tiedot["ID-numero"]);
	this->etunimi = tiedot["Etunimi"];
	this->sukunimi = tiedot["Sukunimi"];
	this->hetu = tiedot["Henkil�tunnus"];
	this->osoite = tiedot["Osoite"];
	this->kayttajatunnus = tiedot["K�ytt�j�tunnus"];
	this->salasana = tiedot["Salasana"];
}

map<string, string> Henkilo::haeAssoc(void) {
	// Henkil�-olion getteri: Palauttaa Mapin, jossa avaimena on olion ominaisuudet k�ytt�j�yst�v�llisess� muodossa

	map<string, string> tiedot;

	tiedot["ID-numero"] = to_string(this->ID);
	tiedot["Etunimi"] = this->etunimi;
	tiedot["Sukunimi"] = this->sukunimi;
	tiedot["Henkil�tunnus"] = this->hetu;
	tiedot["Osoite"] = this->osoite;
	tiedot["Koko nimi"] = this->etunimi + string(" ") + this->sukunimi;
	tiedot["K�ytt�j�tunnus"] = this->kayttajatunnus;
	tiedot["Salasana"] = this->salasana;
	return tiedot;
}

bool Henkilo::Muokkaa() {
	//Muokataan t�m�n olion tietoja: Tulostetaan k�ytt�j�lle listaus henkil�n ominaisuuksista, joita k�ytt�j� voi halutessaan muokata tai j�tt�� ennalleen.

	bool onnistui = false;

	unordered_map<string, Kayttoliittyma::Tiedot> tiedot;
	
	tiedot["1"].kysymys = "Etunimi"; tiedot["1"].vastaus = this->etunimi;
	tiedot["2"].kysymys = "Sukunimi"; tiedot["2"].vastaus = this->sukunimi;
	tiedot["3"].kysymys = "Henkil�tunnus"; tiedot["3"].vastaus = this->hetu;
	tiedot["4"].kysymys = "Osoite"; tiedot["4"].vastaus = this->osoite;
	
	tiedot["5"].kysymys = "K�ytt�j�tunnus"; tiedot["5"].vastaus = this->kayttajatunnus;
	tiedot["6"].kysymys = "Salasana"; tiedot["6"].vastaus = this->salasana;


	if (Kayttoliittyma::MuutaTietoja(tiedot, "Muuta henkil�n " + this->etunimi + " " +this->sukunimi + " tietoja")) {
		
		map<string, string> ominaisuudet;
		for each (auto muutettu in tiedot) {
			ominaisuudet[muutettu.second.kysymys] = muutettu.second.vastaus;
		}

		ominaisuudet["ID-numero"] = to_string(this->ID);
		this->Lisaa(ominaisuudet);
		onnistui = true;
	}
	return onnistui;
}

// -------------------------------------------- ASIAKAS ----------------------------------------------------------------
Asiakas::Asiakas() {
}

Asiakas::Asiakas(string uusiEtunimi, string uusiSukunimi, string uusiHetu, string uusiOsoite, int uusiID)
{
	// Asiakas-luokan constructor: T�m� funktio tallettaa olioon argumentteina tuodut ominaisuudet:
	this->etunimi = uusiEtunimi;
	this->sukunimi = uusiSukunimi;
	this->hetu = uusiHetu;
	this->osoite = uusiOsoite;
	this->ID = uusiID;
}


bool Asiakas::lisaaLainaus(int kirjaID) {
	// Lis�� asiakkaalle lainauksen perustuen kirjan ID-numeroon.
	// KESKEN: Miten tallettaa kirjasta laina-ajan?

	if (lainakielto == true) {
		return false;			//Lainaus ei onnistu, sill� asiakkaalla on lainakielto
	}

	else {						// Lis�t��n lainaus asiakkaan listalle.
		Asiakas::lainanTiedot uusilaina;

		uusilaina.kirja_ID = kirjaID;
		uusilaina.lainaAika = 14;
		uusilaina.lainauspvm = time(NULL);

		lainat.push_back(uusilaina);
		return true;
	}
}

bool Asiakas::poistaLainaus(int kirjaID) {
	// Poistetaan asiakkaan tiedoista argumenttina annetun kirja-id:n lainatiedot.

	bool onnistui = false;

	for (auto laina = lainat.begin(); laina != lainat.end(); ++laina) {
		if (laina->kirja_ID == kirjaID) {
			lainat.erase(laina);
			onnistui = true;
			break;
		}
	}
	return onnistui;
}

double Asiakas::lisaaSakko(vector<Kirja>&kirjat, double yksikkohinta) {
	// Lis�t��n asiakkaalle sakko yli ajan olleista kirjoista:
	/* My�hemmin automatisoidaan niin, ett� funktio itse tutkii asiakkaan lainat ja lis�� tarvittaessa sakon
	(ja kenties lainakiellon?) esimerkiksi kirjaston ajastettujen teht�vien yhteydess�.
	*/
	time_t nyt = time(0);
	

	KirjaHaku* hakukone = new KirjaHaku(kirjat);

	for (auto laina = lainat.begin(); laina != lainat.end(); ++laina) {
		// K�yd��n jokainen laina lainauslistassa l�pi. Jos lainausp�iv� on enemm�n mit�
		// ko. kirjan laina-aika + lainausp�iv�m��r�, lis�t��n sakkoihin (sakko*ylimenneet p�iv�t)
		Kirja* lainakirja = hakukone->NoudaKirja(laina->kirja_ID);
		

		// Muodostetaan muuttuja, jossa lainap�ivi� j�ljell� t�h�n p�iv��n: Lainan er�p�iv� (lainausp�iv� + laina-aika) - t�m� p�iv�:
		int laina_aikaaJaljella = static_cast<int>((((laina->lainauspvm + (lainakirja->laina_aika * 24 * 60 * 60)) - nyt) / (60 * 60 * 24)) +1 );

		if (laina_aikaaJaljella < 0) {
			// Laina-aika t�lle kirjalle on ylitetty.
			
			this->asetaLainakielto(true);	// Asetetaan varmuuden vuoksi lainakielto ainakin nyt

			// Ker�t��n dataa infotulostusta varten:
			tm lainauspaiva;						// Tehd��n lainausp�iv�st� tm-strukti strftime:lle
			localtime_s(&lainauspaiva, &laina->lainauspvm);

			/* Microsoftin haluama tyyli yll�. (warning C4996: 'localtime': This function or variable may be unsafe. Consider using localtime_s instead.) Alla oleva ei k�y
			tm* lainauspaiva= localtime(&laina->lainauspvm);						// Tehd��n lainausp�iv�st� tm-strukti strftime:lle
			*/


			char tulostettavaLainaAika[100];									// strftime tekee char-arrayn n�ytett�v�st� lainausp�iv�st�
			strftime(tulostettavaLainaAika, sizeof(tulostettavaLainaAika), "%x", &lainauspaiva);		// Tehd��n lainausp�iv�st� selke�lukuinen teksti

			// Tulostetaan informaatio er��ntyneest� lainasta
			Kayttoliittyma::Informaatio("Lainaajan " + this->sukunimi + " " + this->etunimi + "(id:" + to_string(this->ID) + ") kirjan " + lainakirja->teos +
				" (id:" + to_string(lainakirja->kirjaID) + ") on er��ntynyt!\n\n" +
				"Lainap�iv�: " + tulostettavaLainaAika + ", kirjan laina-aika " + to_string(lainakirja->laina_aika) + " pv.");
			
			// Tehd��n sakon lis�ys saldoon:
			//---------------------------------

			if (laina->tarkistuspvm == 0) {	// Lainan sakkoa ei ole viel� kertaakaan lis�tty
				this ->sakot += yksikkohinta * (-laina_aikaaJaljella);
				
			}
			else {
				// T�st� lainauksesta on ainakin kerran annettu sakkoa. Nyt katsotaan, tuleeko sakkoa antaa lis��?
				int tarkistusajanVali = static_cast<int>((nyt - laina->tarkistuspvm) / (60 * 60 * 24));			// Verrataan nykyp�iv�� viimeisimp��n tarkistusp�iv��n
				if (tarkistusajanVali > 0) {
					// Viimeisest� sakon tarkistuksesta on kulunut yli p�iv�: Lis�t��n sakkom��r��:
					double sakko = yksikkohinta * ((-laina_aikaaJaljella) - tarkistusajanVali);
					this->sakot += sakko;
				}
			}
			laina->tarkistuspvm = nyt;
			
		}
		
		cout << "";
	}

	delete hakukone;

	return 0;
}

void Asiakas::asetaLainakielto(bool asetus)

{
	this->lainakielto = asetus;

}

bool Asiakas::haeLainakielto() {
	// Tarkistaa, onko asiakkaalla lainakieltoa. Palauttaa boolean arvon sen mukaan, onko asiakkaaalla lainakieltoa vai ei.
	return this->lainakielto;
}


int Asiakas::haeLainat(vector<Asiakas::lainanTiedot> &lainat, bool aktiiviset) {
	// Asettaa asiakkaan lainoista kirjan ID-numerot "Lainat" argumenttiin. Lis�ksi palauttaa lainojen lukum��r�n
	// Aktiiviset -argumentti m��ritt��, haetaanko vain asiakkaalla parhaillaan olevat lainat

	// EI PALAUTA VIEL� VANHOJA!
	/*if (aktiiviset == true) {
	}
	else {*/
		 lainat = this->lainat;
		 return this->lainat.size();
}
string Asiakas::naytaTiedot() {
	string palauta =
		"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n" +
		this->etunimi + string(" ") + this->sukunimi + string("\n") +
		string("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n")+
		string("Henkil�tunnus: ")+ this->hetu + string("\n")+
		string("Osoite: ")+ this->osoite + string("\n\n");

	if (this->sakot > 0) {
		palauta += "Asiakkaalla on maksamattomia sakkoja " + to_string(this->sakot) + " e edest�!!!\n";
	}
	if (this->lainakielto == true) {
		palauta += string("Asiakkaalla on lainakielto!!!\n\n");
	}

	return palauta;
}


map<string,string> Asiakas::haeAssoc(void) {
	// Asiakas-olion getteri: Palauttaa Mapin, jossa avaimena on olion ominaisuudet k�ytt�j�yst�v�llisess� muodossa

	map<string, string> tiedot = Henkilo::haeAssoc() ;

	tiedot["Lainakielto"] = to_string(this->lainakielto);

	int laina_i = 1;
	for each (auto laina in lainat) {
		string iteraattori = "Laina_" + to_string(laina_i) + string("_");

		tiedot[iteraattori + string("KirjaID")] = to_string(laina.kirja_ID);
		tiedot[iteraattori + string("Lainauspvm")] = to_string(laina.lainauspvm);
		tiedot[iteraattori + string("Tarkistuspvm")] = to_string(laina.tarkistuspvm); 
		tiedot[iteraattori + string("Laina-aika")] = to_string(laina.lainaAika);
		laina_i++;
	}

	tiedot["Lainojen lkm"] = to_string(--laina_i);
	tiedot["Sakot"] = to_string(this->sakot);

	return tiedot;
}

void Asiakas::lisaaTiedot(map<string, string> tiedot) {
	// Asiakas-luokan setteri
	
	this->Lisaa(tiedot);

	if (tiedot["Lainakielto"] == "1") {
		this->lainakielto = true;
	}
	else {
		this->lainakielto = false;
	}
	this->sakot = stod(tiedot["Sakot"]);

	for (int i = 1; i <= stoi(tiedot["Lainojen lkm"]); i++) {
		string laina_i = "Laina_" + to_string(i) + "_";

		lainanTiedot l;
			l.kirja_ID = stoi(tiedot[laina_i + "KirjaID"]);
			l.lainauspvm = stoi(tiedot[laina_i + "Lainauspvm"]);
			l.tarkistuspvm = stoi(tiedot[laina_i + "Tarkistuspvm"]);
			l.lainaAika = stoi(tiedot[laina_i + "Laina-aika"]);

		this->lainat.push_back(l);
	}

}

string Asiakas::HaeRooli(string salasana) {
	if (this->salasana == salasana) {
		return "ASIAKAS";
	}
	else return "";
}

// -------------------------------------------- TY�NTEKIJ� ----------------------------------------------------------------

bool Tyontekija::Lisaa(int uusiID)
{//Lis�� uuden asiakkaan kirjastoon.
	
	// Ajetaan p��luokan constructori. Jos palauttaa falsen (k�ytt�j� perui), keskeyt�mme luonnin.
	if (Henkilo::Lisaa(uusiID) == false) {
		return false;
	}

	// Muuten kysyt��n my�s yksik�n tiedot:

	int v_yksikko = 0;
	string rooli = "";

	do {
		unordered_map<string, Kayttoliittyma::Tiedot> parametrit;

		Kayttoliittyma::Tiedot para1;

		para1.kysymys = "Yksik�n ID-numero";
		para1.pakollinen = true;
		parametrit["YksikkoID"] = para1;

		para1.kysymys = "Henkil�n k�ytt�j�rooli yksik�ss�";
		para1.pakollinen = true;
		parametrit["Rooli"] = para1;

		if (Kayttoliittyma::KysyTiedot(parametrit, "Ty�ntekij�n " + this->etunimi + string(" ") + this->sukunimi + string(" yksik�t")) == true) {
			v_yksikko = stoi(parametrit["YksikkoID"].vastaus);
			rooli = parametrit["Rooli"].vastaus;

			this->yksikko[v_yksikko] = rooli;
		}
		else {break; }

	} while (true);
	
	if (yksikko.size() > 0) {
		return true;
	}
	else {
		Kayttoliittyma::Virhe("Yht��n yksikk�� ei annettu! K�ytt�j�n tulee olla edes yhdess� yksik�ss�.");
		return false;
	}
}


map<string, string> Tyontekija::haeAssoc() {
	// Ty�ntekij�-olion getteri: Palauttaa Mapin, jossa avaimena on olion ominaisuudet k�ytt�j�yst�v�llisess� muodossa

	map<string, string> palauta = Henkilo::haeAssoc();
	int i = 1;

	for (auto y = this->yksikko.begin(); y != this-> yksikko.end(); ++y) {

		palauta["Yksikk�_" + to_string(i) + "_ID"] = to_string(y->first);
		palauta["Yksikk�_" + to_string(i) + "_Rooli"] = y->second;
		i++;
	}
	palauta["Yksik�iden lkm"] = to_string(i-1);
	return palauta;

}
void Tyontekija::lisaaTiedot(map<string, string> tiedot) {
	// Ty�ntekij�-olion setteri: Asettaa mapissa olevat arvot olion ominaisuuksiksi


	this->ID = stoi(tiedot["ID-numero"]);
	this->etunimi = tiedot["Etunimi"];
	this->sukunimi = tiedot["Sukunimi"];
	this->hetu = tiedot["Henkil�tunnus"];
	this->osoite = tiedot["Osoite"];
	this->kayttajatunnus = tiedot["K�ytt�j�tunnus"];
	this->salasana = tiedot["Salasana"];

	for (int i = 1; i <= stoi(tiedot["Yksik�iden lkm"]); i++) {
		string yksikko_i = "Yksikk�_" + to_string(i) + "_";
		int yid = stoi(tiedot[yksikko_i + "ID"]);
		this->yksikko[yid] = tiedot[yksikko_i + "Rooli"];
	}

}

string Tyontekija::HaeRooli(string salasana, int yksikkoID) {
	string palauta = "";

	if (this->salasana == salasana) {
		if (this->yksikko.find(yksikkoID) != this->yksikko.end()) {
			if (this->yksikko[yksikkoID] == "")
				palauta = "TY�NTEKIJ�";
			else {
				palauta = yksikko[yksikkoID];
			}
		}
		
	}

	return palauta;
}