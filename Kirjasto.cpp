/* KESKENER�ISI� ASIOITA:
---------------------------
- Ty�ntekij�
- Jos asiakas poistetaan, tulee my�s sen lainaamille kirjoille tehd� jotain
- Jos Kirja poistetaan, tulee my�s sen mahdolliselle lainaajalle tehd� jotain?
- */

#include "stdafx.h"
#include "Kirjasto.h"


Kirjasto::Kirjasto()
{
	this->yksikko = 1;					// Kirjaston yksil�llinen ID, t�ss� vaiheessa oletamme ett� kaikki ovat samassa yksik�ss�.
	
	// Noudetaan tietueet asynkronisesti. Joudutaan k�ytt�m��n asynkronista, sill� haluamme palautusarvon funktioilta:

	future<int> vKirja = async(&Haku::nouda_kirjat_tiedostosta, "resurssit\\Kirjat.txt", ref(kirjat) );
	future<int> vAsiakas = async(&Haku::nouda_asiakkaat_tiedostosta,"resurssit\\Asiakkaat.txt", ref(asiakkaat));
	future<int> vTyontekija = async(&Haku::nouda_tyontekijat_tiedostosta, "resurssit\\Tyontekijat.txt", ref(tyontekijat));


	this->viimeisinKirjaID = vKirja.get();
	this->viimeisinAsiakasID = vAsiakas.get();
	this->viimeisinTyontekijaID = vTyontekija.get();

	
}


Kirjasto::~Kirjasto()
{
	// Tallennetaan vektorin sis�lt� tiedostoihin samanaikaisesti:
	Haku tiedot;
	thread kirjaSaie(tiedot.tallenna_kirjat_tiedostoon, "resurssit\\Kirjat.txt", kirjat);
	thread asiakasSaie(tiedot.tallenna_asiakkaat_tiedostoon,"resurssit\\Asiakkaat.txt", asiakkaat);
	thread tyontekijaSaie(tiedot.tallenna_tyontekijat_tiedostoon, "resurssit\\Tyontekijat.txt", tyontekijat);

	kirjaSaie.join(); asiakasSaie.join(); tyontekijaSaie.join();

	// Perinteiseen tapaan tallennus. �L� POISTA: Helpompi debugata...

	//tiedot.tallenna_kirjat_tiedostoon("resurssit\\Kirjat.txt", kirjat);
	//tiedot.tallenna_asiakkaat_tiedostoon("resurssit\\Asiakkaat.txt", asiakkaat);
	//tiedot.tallenna_tyontekijat_tiedostoon("resurssit\\Tyontekijat.txt", tyontekijat);
}

void Kirjasto::main_lisaaKirja(){
	//Lis�t��n uusi asiakas kirjastoon
	Kirja k;

	if (k.lisaaTiedot(this->viimeisinKirjaID) == true) {
		kirjat.push_back(k);
		Kayttoliittyma::Informaatio("Kirja " + k.teos + string(" lis�tty."));
	}
	else {
		Kayttoliittyma::Informaatio("Kirjaa ei lis�tty");
	}

}


void Kirjasto::poistaKirja(int kirjaID)
{// Poistetaan argumenttina m��ritelty kirja pois kirjastosta:
	
	for (vector<Kirja>::iterator i = kirjat.begin(); i != kirjat.end(); ++i) {
		if (i->kirjaID == kirjaID) {
			kirjat.erase(i);
			break;
		}
	}
}

void Kirjasto::lisaaAsiakas()
{
	//Lis�t��n uusi asiakas kirjastoon
	Asiakas a;
	
	if (a.Lisaa(viimeisinAsiakasID) == true) {
		asiakkaat.push_back(a);
		Kayttoliittyma::Informaatio("Asiakas " + a.etunimi + string(" ") + a.sukunimi + string(" lis�tty."));
	}
	else {
		Kayttoliittyma::Informaatio("Asiakasta ei luotu.");
	}

}
Asiakas* Kirjasto::haeAsiakas()
{
	// Haetaan asiakasta k�ytt�j�n muodostamien kriteerien perusteella.
	//Funktio palauttaa pointteriosoittajan kohti alkuper�ist� asiakkaat-muuttujassa olevaa instanssia


	Asiakas* palauta = nullptr;
	AsiakasHaku haku(asiakkaat);						// luodaan hakuolio ja tallennetaan kopio kaikista kirjoista. N�it� aletaan sitten karsimaan.
	palauta = haku.NoudaAsiakas();

	return palauta;

}


void Kirjasto::main_haeAsiakas() {
	//Kysyt��n k�ytt�j�lt� hakuparametrit jotta saadaan asiakas-instanssi tietoon
	Asiakas* valittuAsiakas = haeAsiakas();			//Valitun asiakkaan pointteri talletetaan t�h�n
	int valinta = 0;							// K�ytt�j�lt� kysytt�v� valinta jatkotoimille

	// Tuliko asiakasta?
	if (valittuAsiakas == nullptr){
		Kayttoliittyma::Informaatio("Asiakasta ei valittu");
		return;
	}

	//Nyt kun asiakas on valittuna, niin mit� tehd��n?
	//----------------------------------------------- TULOSTETAAN N�YTT��N VALINTALOMAKE:::
	map<int, string> valinnat;
	valinnat[1] = "Muuta asiakkaan tietoja";
	valinnat[2] = "P�ivit� sakot";
	if (valittuAsiakas->haeLainakielto() == true){
		valinnat[3] = "Poista lainakielto";
	}
	valinnat[4] = "Poista asiakas";
	valinnat[5] = "Lis�� lainaus";

	string asiakastiedot = this->naytaAsiakkaanTiedot(valittuAsiakas);

	
	valinta = Kayttoliittyma::Valikko(valinnat, "Asiakas " + valittuAsiakas->etunimi + valittuAsiakas->sukunimi, asiakastiedot);
	// --------------------------------------------- N�YT�N VALINTALOMAKKEEN TULOSTUS P��TTYI::::


	switch (valinta) {
	case 1: // Asiakkaan tietojen muutos:
		valittuAsiakas->Muokkaa();
		break;
	case 2:
		valittuAsiakas->lisaaSakko(this->kirjat, this->sakonYksikkohinta);
		break;
	case 3:// Lainakiellon asettaminen:
		if (valittuAsiakas->sakot > 0) {
			if (Kayttoliittyma::Kysy("Onko asiakas maksanut sakkonsa (" + to_string(valittuAsiakas->sakot) + " e)?") == true){
				valittuAsiakas->sakot = 0.0;
				valittuAsiakas->asetaLainakielto(false);
				Kayttoliittyma::Informaatio("Lainakielto kumottu ja sakkotili nollattu.\nHuomio! Jos asiakkaalla on er��ntyneit� lainoja, lainakielto astuu voimaan j�lleen tarkistusten yhteydess�!");
			}
		}
		break;
	case 4://Asiakkaan poisto
		poistaAsiakas(valittuAsiakas->ID);
		valittuAsiakas = nullptr;
		Kayttoliittyma::Informaatio("Asiakas poistettu...");
		break;
	case 5: // Lainan lis�ys
		lisaaLainaus(nullptr, valittuAsiakas);
	}


}


void Kirjasto::poistaAsiakas(unsigned int asiakasID)
{
	// Poistetaan asiakas perustuen argumenttina tuotuun asiakkaan ID-numeroon.

	for (vector<Asiakas>::iterator i = asiakkaat.begin(); i != asiakkaat.end(); ++i) {
		if (i->ID == asiakasID) {
			asiakkaat.erase(i);
			break;
		}
	}

}

void Kirjasto::poistaLainaus(Kirja *kirja)
{
	// Poistetaan argumenttina tuodusta kirjasta lainausmerkint�.
	

	Asiakas* lainaaja;
	AsiakasHaku lainaajaHaku(asiakkaat);
	lainaaja = lainaajaHaku.NoudaAsiakas(kirja->lainaaja);

	lainaaja->poistaLainaus(kirja->kirjaID);
	kirja->palauta();

}


Tyontekija Kirjasto::lisaaTyontekija()
{
	Tyontekija uusi;
	if (uusi.Lisaa(++viimeisinTyontekijaID) == true) {
		tyontekijat.push_back(uusi);
		Kayttoliittyma::Informaatio("K�ytt�j� " + uusi.etunimi + string(" ") + uusi.sukunimi + string(" lis�tty."));
		return uusi;
	}
	else {
		Kayttoliittyma::Informaatio("K�ytt�j�� ei luotu.");
		return Tyontekija();
	}
	
}

void Kirjasto::main_haeTyontekija() {
	//Kysyt��n k�ytt�j�lt� hakuparametrit jotta saadaan ty�ntekij�-instanssi tietoon

	TyontekijaHaku haku(tyontekijat);						// luodaan hakuolio ja tallennetaan kopio kaikista kirjoista. N�it� aletaan sitten karsimaan.
	
	Tyontekija* valittuTyontekija = haku.NoudaTyontekija();			//Valitun asiakkaan pointteri talletetaan t�h�n
	int valinta = 0;							// K�ytt�j�lt� kysytt�v� valinta jatkotoimille

	// Tuliko ty�ntekij��?
	if (valittuTyontekija == nullptr){
		Kayttoliittyma::Informaatio("Ty�ntekij�� ei valittu");
		return;
	}

	//Nyt kun asiakas on valittuna, niin mit� tehd��n?
	//----------------------------------------------- TULOSTETAAN N�YTT��N VALINTALOMAKE:::
	map<int, string> valinnat;
	valinnat[1] = "Muuta ty�ntekij�n tietoja";
	valinnat[2] = "Poista ty�ntekij�";

	valinta = Kayttoliittyma::Valikko(valinnat, "Ty�ntekij� " + valittuTyontekija->etunimi +" " + valittuTyontekija->sukunimi);
	// --------------------------------------------- N�YT�N VALINTALOMAKKEEN TULOSTUS P��TTYI::::


	switch (valinta) {
	case 1: // Asiakkaan tietojen muutos:
		valittuTyontekija->Muokkaa();
		break;

	case 2://Asiakkaan poisto
		poistaTyontekija(valittuTyontekija->ID);
		valittuTyontekija = nullptr;
		Kayttoliittyma::Informaatio("Ty�ntekij� poistettu...");
		break;

	}

}

void Kirjasto::poistaTyontekija(unsigned int tyontekijaID)
{
	// Poistetaan ty�ntekij� perustuen argumenttina tuotuun asiakkaan ID-numeroon.

	for (vector<Tyontekija>::iterator i = tyontekijat.begin(); i != tyontekijat.end(); ++i) {
		if (i->ID == tyontekijaID) {
			tyontekijat.erase(i);
			break;
		}
	}

}





Kirja* Kirjasto::haeKirja()
{ 
	// Haetaan kirjaa k�ytt�j�n muodostamien kriteerien perusteella.
	//Funktio palauttaa pointteriosoittajan kohti alkuper�ist� kirjat-muuttujassa olevaa instanssia


	Kirja* palauta = nullptr ;					
	KirjaHaku osumat(kirjat);						// luodaan hakuolio ja tallennetaan kopio kaikista kirjoista. N�it� aletaan sitten karsimaan.
	palauta = osumat.NoudaKirja();
	
	return palauta;

}

void Kirjasto::main_haeKirja() {
	//Kirjan hakumetodi, jota kutsutaan Main-funktiosta
	
	Kirja* valittuKirja = haeKirja();			//Valitun kirjan pointteri talletetaan t�h�n
	int valinta = 0;							// K�ytt�j�lt� kysytt�v� valinta jatkotoimille
	
	// Tuliko kirjaa?
	if (valittuKirja == nullptr){
		Kayttoliittyma::Informaatio("Kirjaa ei valittu.");
		return;
	}

	//Nyt kun kirja on valittuna, niin mit� tehd��n?
	//-----------------------------------------------
	map<int, string> valinnat;
	if (valittuKirja->onkoLainassa() == true){
		valinnat[1] = "Palauta kirja";
	}
	else {
		valinnat[1] = "Lainaa kirja";
	}
	
	valinnat[2] = "Muuta kirjan tietoja";
	valinnat[3] = "Poista kirja";


	string kirjatiedot = valittuKirja->naytaTiedot();

	valinta = Kayttoliittyma::Valikko(valinnat, "Kirja " + valittuKirja->teos, kirjatiedot);


	switch (valinta) {
		case 1: // Kirjan lainaus/palautus:
			if (valittuKirja->onkoLainassa() == true) {
				poistaLainaus(valittuKirja);
			}
			else {
				lisaaLainaus(valittuKirja, nullptr);
			}
			break;
		case 2:	// Kirjan tietojen muutos:
			valittuKirja->muutaTietoja();
			break;

		case 3://Kirjan poisto
			poistaKirja(valittuKirja->kirjaID);
			valittuKirja = nullptr;
			Kayttoliittyma::Informaatio("Kirja poistettu...");
			break;
	}


}

void Kirjasto::lisaaLainaus(Kirja *kirja, Asiakas *asiakas)
{
	//Lis�t��n lainaus asiakkaaseen ja kirjaan:
	
	//Tarkistetaan, onko kirja annettu: Jos ei, pyydet��n k�ytt�j�� valitsemaan yksi.
	if (kirja == nullptr) {

		kirja = haeKirja();
	}
	
	//Tarkistetaan, onko asiakas annettu: Jos ei, kysyt��n asiakasta
	if (asiakas == nullptr) {
		asiakas = haeAsiakas();
	}

	// Tutkitaan viel�, saatiinko edes nyt asiakkaita. Jos ei saatu, ei lainauskaan onnistu.
	if (kirja != nullptr && asiakas != nullptr) {
		if (kirja->lainaa(asiakas->ID) == false) {
			Kayttoliittyma::Virhe("Kirjan lainaus ei onnistunut...");
		}
		else if (asiakas->lisaaLainaus(kirja->kirjaID) == false) {
			Kayttoliittyma::Virhe("Kirjan lainaus ei onnistunut...");
		}
		else {
			Kayttoliittyma::Informaatio(string("Kirja ") + kirja->teos + string(" on nyt lainassa henkil�ll� ") + asiakas->sukunimi + string(" ") + asiakas->etunimi + string("."));
		}

	}

}


Kirjasto::Kayttajaprofiili Kirjasto::TarkistaOikeudet(string kayttajatunnus, string salasana) {
	// Tarkistetaan k�ytt�j�n antamat tunnukset, ja palautetaan k�ytt�j�n rooli.
	if (kayttajatunnus == "" && salasana == "") {
		return USER_ANONYYMI;
	}
	if (kayttajatunnus == "") {
		Kayttoliittyma::Informaatio("Anna k�ytt�j�tunnus");
		return USER_INVALID;
	}
	if (salasana == "") {
		Kayttoliittyma::Informaatio("Anna salasana");
		return USER_INVALID;
	}

	// Muussa tapauksessa l�hdemme hakemaan tunnusta:

	// Ensin ty�ntekij�t (koska lista on lyhyempi):

	for (unsigned int i = 0; i < this->tyontekijat.size(); i++) {
		if (this->tyontekijat[i].kayttajatunnus == kayttajatunnus) {
			string rooli = this->tyontekijat[i].HaeRooli(salasana, this->yksikko);

			if (rooli == "admin") {
				this->nykyinenKayttajaTyontekija = &this->tyontekijat[i];
				return USER_ADMIN;
			}
			else if (rooli != "") {
				this->nykyinenKayttajaTyontekija = &this->tyontekijat[i];
				return USER_VIRKAILIJA;
			}
			else
				return USER_INVALID;
		}
	}

	// Jos ei osunut ty�ntekij�ist�, etsit��n asiakasta:
	for (unsigned int i = 0; i < this->asiakkaat.size(); i++) {
		if (this->asiakkaat[i].kayttajatunnus == kayttajatunnus) {
			if (this->asiakkaat[i].HaeRooli(salasana) != "") {
				this->nykyinenKayttajaAsiakas = &this->asiakkaat[i];
				return USER_ASIAKAS;
			}
			else return USER_INVALID;
		}
	}

	// Ei l�ytynyt asiakkaistakaan. Ei siis ole k�ytt�j�� sill� tunnuksella:

	return USER_INVALID;
}

string Kirjasto::naytaAsiakkaanTiedot(Asiakas* valittuAsiakas) {
	//Tekee string-tyyppisen tekstin, jossa asiakkaan tiedot ja lainat
	if (valittuAsiakas == nullptr) {
		return "";
	}

	string asiakastiedot = valittuAsiakas->naytaTiedot();
	vector<Asiakas::lainanTiedot> aLainat;

	if (valittuAsiakas->haeLainat(aLainat, true) > 0) {
		asiakastiedot += string("Asiakkaan lainat:\n ----------------\n");
		KirjaHaku haku(kirjat);

		for each (auto laina in aLainat) {
			Kirja* lainattuKirja = haku.NoudaKirja(laina.kirja_ID);
			asiakastiedot += to_string(lainattuKirja->kirjaID) + string(": ") + lainattuKirja->teos + string("\n");
		}
	}
	return asiakastiedot;
}