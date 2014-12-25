#include "stdafx.h"
#include "Henkilot.h"


Henkilo::Henkilo()
{
}


Henkilo::~Henkilo()
{
}

bool Henkilo::Lisaa( int uusiID) {
	// Uuden asiakkaan lisäys: Tämä funktio kysyy käyttäjältä syötteitä asiakkaan ominaisuuksiin
	// Palauttaa boolean arvon onnistuiko asiakkaan tietojen lisäys.

	unordered_map<string, Kayttoliittyma::Tiedot> parametrit;

	Kayttoliittyma::Tiedot para1;

	para1.kysymys = "Sukunimi";
	para1.pakollinen = true;
	parametrit["Sukunimi"] = para1;

	para1.kysymys = "Etunimi";
	para1.pakollinen = true;
	parametrit["Etunimi"] = para1;

	para1.kysymys = "Henkilötunnus";
	para1.pakollinen = false;
	parametrit["Henkilötunnus"] = para1;

	para1.kysymys = "Osoite";
	para1.pakollinen = false;
	parametrit["Osoite"] = para1;

	para1.kysymys = "Käyttäjätunnus";
	para1.pakollinen = false;
	parametrit["Käyttäjätunnus"] = para1;

	para1.kysymys = "Salasana";
	para1.pakollinen = false;
	parametrit["Salasana"] = para1;

	if (Kayttoliittyma::KysyTiedot(parametrit, "Uuden henkilön tiedot") == true) {
		this->ID = uusiID;
		this->sukunimi = parametrit["Sukunimi"].vastaus;
		this->etunimi = parametrit["Etunimi"].vastaus;
		this->hetu = parametrit["Henkilötunnus"].vastaus;
		this->osoite = parametrit["Osoite"].vastaus;
		this->kayttajatunnus = parametrit["Käyttäjätunnus"].vastaus;
		this->salasana = parametrit["Salasana"].vastaus;

		return true;
	}
	else { return false; }
}

void Henkilo::Lisaa(map<string, string> &tiedot) {
	// Setteri: Asettaa argumenttina tuodut ominaisuudet tämän olion ominaisuuksiksi.
	this->ID = stoi(tiedot["ID-numero"]);
	this->etunimi = tiedot["Etunimi"];
	this->sukunimi = tiedot["Sukunimi"];
	this->hetu = tiedot["Henkilötunnus"];
	this->osoite = tiedot["Osoite"];
	this->kayttajatunnus = tiedot["Käyttäjätunnus"];
	this->salasana = tiedot["Salasana"];
}

map<string, string> Henkilo::haeAssoc(void) {
	// Henkilö-olion getteri: Palauttaa Mapin, jossa avaimena on olion ominaisuudet käyttäjäystävällisessä muodossa

	map<string, string> tiedot;

	tiedot["ID-numero"] = to_string(this->ID);
	tiedot["Etunimi"] = this->etunimi;
	tiedot["Sukunimi"] = this->sukunimi;
	tiedot["Henkilötunnus"] = this->hetu;
	tiedot["Osoite"] = this->osoite;
	tiedot["Koko nimi"] = this->etunimi + string(" ") + this->sukunimi;
	tiedot["Käyttäjätunnus"] = this->kayttajatunnus;
	tiedot["Salasana"] = this->salasana;
	return tiedot;
}

bool Henkilo::Muokkaa() {
	//Muokataan tämän olion tietoja: Tulostetaan käyttäjälle listaus henkilön ominaisuuksista, joita käyttäjä voi halutessaan muokata tai jättää ennalleen.

	bool onnistui = false;

	unordered_map<string, Kayttoliittyma::Tiedot> tiedot;
	
	tiedot["1"].kysymys = "Etunimi"; tiedot["1"].vastaus = this->etunimi;
	tiedot["2"].kysymys = "Sukunimi"; tiedot["2"].vastaus = this->sukunimi;
	tiedot["3"].kysymys = "Henkilötunnus"; tiedot["3"].vastaus = this->hetu;
	tiedot["4"].kysymys = "Osoite"; tiedot["4"].vastaus = this->osoite;
	
	tiedot["5"].kysymys = "Käyttäjätunnus"; tiedot["5"].vastaus = this->kayttajatunnus;
	tiedot["6"].kysymys = "Salasana"; tiedot["6"].vastaus = this->salasana;


	if (Kayttoliittyma::MuutaTietoja(tiedot, "Muuta henkilön " + this->etunimi + " " +this->sukunimi + " tietoja")) {
		
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
	// Asiakas-luokan constructor: Tämä funktio tallettaa olioon argumentteina tuodut ominaisuudet:
	this->etunimi = uusiEtunimi;
	this->sukunimi = uusiSukunimi;
	this->hetu = uusiHetu;
	this->osoite = uusiOsoite;
	this->ID = uusiID;
}


bool Asiakas::lisaaLainaus(int kirjaID) {
	// Lisää asiakkaalle lainauksen perustuen kirjan ID-numeroon.
	// KESKEN: Miten tallettaa kirjasta laina-ajan?

	if (lainakielto == true) {
		return false;			//Lainaus ei onnistu, sillä asiakkaalla on lainakielto
	}

	else {						// Lisätään lainaus asiakkaan listalle.
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
	// Lisätään asiakkaalle sakko yli ajan olleista kirjoista:
	/* Myöhemmin automatisoidaan niin, että funktio itse tutkii asiakkaan lainat ja lisää tarvittaessa sakon
	(ja kenties lainakiellon?) esimerkiksi kirjaston ajastettujen tehtävien yhteydessä.
	*/
	time_t nyt = time(0);
	

	KirjaHaku* hakukone = new KirjaHaku(kirjat);

	for (auto laina = lainat.begin(); laina != lainat.end(); ++laina) {
		// Käydään jokainen laina lainauslistassa läpi. Jos lainauspäivä on enemmän mitä
		// ko. kirjan laina-aika + lainauspäivämäärä, lisätään sakkoihin (sakko*ylimenneet päivät)
		Kirja* lainakirja = hakukone->NoudaKirja(laina->kirja_ID);
		

		// Muodostetaan muuttuja, jossa lainapäiviä jäljellä tähän päivään: Lainan eräpäivä (lainauspäivä + laina-aika) - tämä päivä:
		int laina_aikaaJaljella = static_cast<int>((((laina->lainauspvm + (lainakirja->laina_aika * 24 * 60 * 60)) - nyt) / (60 * 60 * 24)) +1 );

		if (laina_aikaaJaljella < 0) {
			// Laina-aika tälle kirjalle on ylitetty.
			
			this->asetaLainakielto(true);	// Asetetaan varmuuden vuoksi lainakielto ainakin nyt

			// Kerätään dataa infotulostusta varten:
			tm lainauspaiva;						// Tehdään lainauspäivästä tm-strukti strftime:lle
			localtime_s(&lainauspaiva, &laina->lainauspvm);

			/* Microsoftin haluama tyyli yllä. (warning C4996: 'localtime': This function or variable may be unsafe. Consider using localtime_s instead.) Alla oleva ei käy
			tm* lainauspaiva= localtime(&laina->lainauspvm);						// Tehdään lainauspäivästä tm-strukti strftime:lle
			*/


			char tulostettavaLainaAika[100];									// strftime tekee char-arrayn näytettävästä lainauspäivästä
			strftime(tulostettavaLainaAika, sizeof(tulostettavaLainaAika), "%x", &lainauspaiva);		// Tehdään lainauspäivästä selkeälukuinen teksti

			// Tulostetaan informaatio erääntyneestä lainasta
			Kayttoliittyma::Informaatio("Lainaajan " + this->sukunimi + " " + this->etunimi + "(id:" + to_string(this->ID) + ") kirjan " + lainakirja->teos +
				" (id:" + to_string(lainakirja->kirjaID) + ") on erääntynyt!\n\n" +
				"Lainapäivä: " + tulostettavaLainaAika + ", kirjan laina-aika " + to_string(lainakirja->laina_aika) + " pv.");
			
			// Tehdään sakon lisäys saldoon:
			//---------------------------------

			if (laina->tarkistuspvm == 0) {	// Lainan sakkoa ei ole vielä kertaakaan lisätty
				this ->sakot += yksikkohinta * (-laina_aikaaJaljella);
				
			}
			else {
				// Tästä lainauksesta on ainakin kerran annettu sakkoa. Nyt katsotaan, tuleeko sakkoa antaa lisää?
				int tarkistusajanVali = static_cast<int>((nyt - laina->tarkistuspvm) / (60 * 60 * 24));			// Verrataan nykypäivää viimeisimpään tarkistuspäivään
				if (tarkistusajanVali > 0) {
					// Viimeisestä sakon tarkistuksesta on kulunut yli päivä: Lisätään sakkomäärää:
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
	// Asettaa asiakkaan lainoista kirjan ID-numerot "Lainat" argumenttiin. Lisäksi palauttaa lainojen lukumäärän
	// Aktiiviset -argumentti määrittää, haetaanko vain asiakkaalla parhaillaan olevat lainat

	// EI PALAUTA VIELÄ VANHOJA!
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
		string("Henkilötunnus: ")+ this->hetu + string("\n")+
		string("Osoite: ")+ this->osoite + string("\n\n");

	if (this->sakot > 0) {
		palauta += "Asiakkaalla on maksamattomia sakkoja " + to_string(this->sakot) + " e edestä!!!\n";
	}
	if (this->lainakielto == true) {
		palauta += string("Asiakkaalla on lainakielto!!!\n\n");
	}

	return palauta;
}


map<string,string> Asiakas::haeAssoc(void) {
	// Asiakas-olion getteri: Palauttaa Mapin, jossa avaimena on olion ominaisuudet käyttäjäystävällisessä muodossa

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

// -------------------------------------------- TYÖNTEKIJÄ ----------------------------------------------------------------

bool Tyontekija::Lisaa(int uusiID)
{//Lisää uuden asiakkaan kirjastoon.
	
	// Ajetaan pääluokan constructori. Jos palauttaa falsen (käyttäjä perui), keskeytämme luonnin.
	if (Henkilo::Lisaa(uusiID) == false) {
		return false;
	}

	// Muuten kysytään myös yksikön tiedot:

	int v_yksikko = 0;
	string rooli = "";

	do {
		unordered_map<string, Kayttoliittyma::Tiedot> parametrit;

		Kayttoliittyma::Tiedot para1;

		para1.kysymys = "Yksikön ID-numero";
		para1.pakollinen = true;
		parametrit["YksikkoID"] = para1;

		para1.kysymys = "Henkilön käyttäjärooli yksikössä";
		para1.pakollinen = true;
		parametrit["Rooli"] = para1;

		if (Kayttoliittyma::KysyTiedot(parametrit, "Työntekijän " + this->etunimi + string(" ") + this->sukunimi + string(" yksiköt")) == true) {
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
		Kayttoliittyma::Virhe("Yhtään yksikköä ei annettu! Käyttäjän tulee olla edes yhdessä yksikössä.");
		return false;
	}
}


map<string, string> Tyontekija::haeAssoc() {
	// Työntekijä-olion getteri: Palauttaa Mapin, jossa avaimena on olion ominaisuudet käyttäjäystävällisessä muodossa

	map<string, string> palauta = Henkilo::haeAssoc();
	int i = 1;

	for (auto y = this->yksikko.begin(); y != this-> yksikko.end(); ++y) {

		palauta["Yksikkö_" + to_string(i) + "_ID"] = to_string(y->first);
		palauta["Yksikkö_" + to_string(i) + "_Rooli"] = y->second;
		i++;
	}
	palauta["Yksiköiden lkm"] = to_string(i-1);
	return palauta;

}
void Tyontekija::lisaaTiedot(map<string, string> tiedot) {
	// Työntekijä-olion setteri: Asettaa mapissa olevat arvot olion ominaisuuksiksi


	this->ID = stoi(tiedot["ID-numero"]);
	this->etunimi = tiedot["Etunimi"];
	this->sukunimi = tiedot["Sukunimi"];
	this->hetu = tiedot["Henkilötunnus"];
	this->osoite = tiedot["Osoite"];
	this->kayttajatunnus = tiedot["Käyttäjätunnus"];
	this->salasana = tiedot["Salasana"];

	for (int i = 1; i <= stoi(tiedot["Yksiköiden lkm"]); i++) {
		string yksikko_i = "Yksikkö_" + to_string(i) + "_";
		int yid = stoi(tiedot[yksikko_i + "ID"]);
		this->yksikko[yid] = tiedot[yksikko_i + "Rooli"];
	}

}

string Tyontekija::HaeRooli(string salasana, int yksikkoID) {
	string palauta = "";

	if (this->salasana == salasana) {
		if (this->yksikko.find(yksikkoID) != this->yksikko.end()) {
			if (this->yksikko[yksikkoID] == "")
				palauta = "TYÖNTEKIJÄ";
			else {
				palauta = yksikko[yksikkoID];
			}
		}
		
	}

	return palauta;
}