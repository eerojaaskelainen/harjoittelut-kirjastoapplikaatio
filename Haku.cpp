#include "stdafx.h"
#include "Haku.h"




// Yleiset hakumetodit alkaa:
void Haku::PaivitaTilanne() {
	// Tutkii, paljonko t‰ll‰ hetkell‰ on hakutuloksia. Haun tilanne talletetaan olion tilanne -ominaisuuteen.
	int tuloksia = osumat.size();

	if      (tuloksia == 0) tilanne = HAKU_EI_OSUMIA;
	else if (tuloksia == 1) tilanne = HAKU_OK_YKSI;
	else                    tilanne = HAKU_OK_MONTA;
}


void Haku::KarsiOsumat(pair<string, string> kriteeri, bool tiukka) {
	// Karsitaan hakulistaa perustuen haluttuun kriteeriin:
	/* ARGUMENTIT:
		kriteeri	= Pari, jossa 1. on avaimen nimi, 2. annettu m‰‰rite. Esim. kriteeri["Teoksen nimi"] = "Talvisota"
		tiukka		= Boolean. M‰‰ritt‰‰ haun tiukkuuden: True = Ominaisuus tulee olla juuri kuten kirjoitettu. False= Riitt‰‰ kunhan ominaisuudessa on jossain kohtaa annettu m‰‰rite.
	*/

	//Tutkitaan, annettiinko m‰‰ritett‰:
	if (kriteeri.second  == "") {
		//Ei m‰‰ritett‰ annettu. Ei k‰yd‰ l‰pi karsintaa t‰m‰n enemp‰‰.
		return;
	}


	// Tehd‰‰n tarkistus. Jos t‰m‰ yksilˆ ei osu kriteeriin, kytket‰‰n poistovipu p‰‰lle:

	for (vector<map<string, string>>::iterator yksilo = osumat.begin(); yksilo != osumat.end();) {	// K‰yd‰‰n jokainen yksilˆ l‰pi
		bool poistetaanko = true;			//Poistovipu: Jos yksilˆ ei osunut kriteereihin, poistetaan se listasta. Oletus: Poistetaan. Paitsi jos kriteeri osuu.
		
		map<string, string> ominaisuudet = *yksilo;
		
		if (tiukka == true){		// Haku on tiukka
			if (ominaisuudet[kriteeri.first] == kriteeri.second) {
				poistetaanko = false;
			}
		}

		else {	//Haku on vapaahaku: Osan kriteerist‰ tulee olla sama
			if (strstr(ominaisuudet[kriteeri.first].c_str(), kriteeri.second.c_str()) != nullptr) {
				poistetaanko = false;
			}
		}


		//-----------------------------------------------------------------------------------------------------------
		// Tarkistetaan vipu: Jos t‰m‰ yksilˆ ei osunut kriteeriin, otetaan se pois tuloslistalta. Muuten jatketaan loopissa seuraavaan.
		if (poistetaanko == true) {
			yksilo = osumat.erase(yksilo);
		}
		else
		{
			++yksilo;
		}

	}

	//------------ Karsinnat t‰m‰n kriteerin osalta on tehty. Viel‰ ajetaan tarkistus, mik‰ p‰ivitt‰‰ olion "tilanne" -ominaisuuden
	PaivitaTilanne();


}

//-------------------------------------------------------------------------------------------------------------------------------------------------

// Kirjan hakumetodit alkaa:

KirjaHaku::KirjaHaku(vector<Kirja>& myHakulista)
{ 	// Asiakashaun constructor: Haetaan hakulista -ominaisuuteen pointterit kirjoista.

	for (unsigned int i = 0; i < myHakulista.size(); i++) {
		osumat.push_back((myHakulista[i]).haeAssoc());			// Talletetaan kaikkien kirjojen ominaisuudet map:iin
		hakulista.push_back(&(myHakulista[i]));					// Talletetaan osoitteet kaikista kirjoista pointterivektoriin
	}
	PaivitaTilanne();											// P‰ivitet‰‰n haun tilanne-attribuutti oikeaksi
}




Kirja* KirjaHaku::NoudaKirja() {
	// Kirjan haun p‰‰metodi: Kysyt‰‰n k‰ytt‰j‰lt‰ suodatusehdot
	// Palautetaan pointteri valitusta kirjasta.


	Kirja* palauta = nullptr;								// Palautettava pointteri

	// Haetaan hakukriteerit: Koska t‰m‰ instanssi on tyhj‰ versio kirjasta, saadaan sielt‰ assosiatiivinen map tyhjill‰ arvoilla
	Kirja tyhjaKirja;
	hakukriteerit = tyhjaKirja.haeAssoc();					


	// Vied‰‰n k‰yttˆliittym‰lle kriteerit t‰ytett‰v‰ksi:
	if (Kayttoliittyma::KysySuodatus(hakukriteerit, "Kirjan haku") == false)
	{
	// :::::::::::::::::::::::::::::::: JOS K‰ytt‰j‰ keskeytti haun...::::::::::::::::::::::::::::::::::::::::
		return palauta;
	}
	// ::::::::::::::::::::::::::::::::. Muuten aloitamme tulosten karsinnan ::::::::::::::::::::::::::::::::::::::::::::...

	
	// K‰yd‰‰n l‰pi k‰yttˆliittym‰lt‰ palautunut kriteeristˆ yksi kerrallaan:
	for (map<string, string>::iterator i = hakukriteerit.begin(); i != hakukriteerit.end(); ++i) {
		if (!(tilanne == HAKU_OK_YKSI || tilanne == HAKU_EI_OSUMIA)){		// Karsintaa edet‰‰n, mik‰li on jotain karsittavaa
			
			
			// M‰‰ritell‰‰n t‰m‰n ominaisuuden haun tiukkuus:
			bool tiukkaHaku = false;										// Normaalisti haku on vapaasanahaku.

			if      (i->first == "ID-numero") tiukkaHaku = true;			// ID numeron haku on oltava tiukka: Jos k‰ytt‰j‰ hakee kirjaa 3, h‰nelle ei tarjota kirjoja 33, 13, 31... 
			else if (i->first == "UDK-koodi") tiukkaHaku = true;			// UDK-koodin haku on tiukka: K‰ytt‰j‰n tulee antaa tarkasti sama koodi mit‰ haluamansa luokan kirjoilla on.

			else if (strstr(i->second.c_str(), "\"") != nullptr) tiukkaHaku = true;			// Jos k‰ytt‰j‰ on kirjoittanut kriteerin lainausmerkkeihin, haetaan tarkalla haulla.


			// Karsitaan listaa. Argumenttina kriteeri&tulos -pari (esim. ["Kirjan nimi"] = "Talvisota"):
			KarsiOsumat(make_pair(i->first, i->second),tiukkaHaku);			
		}
	}

	//:::::::::::::::::::::::::::::: Suurin karsinta tehty. Jatko riippuu siit‰, mik‰ on tulosten lukum‰‰r‰ :::::::::::::::::::::::::
	
	if (tilanne == HAKU_OK_MONTA) {
		// ------------------------- Viel‰ on liikaa vaihtoehtoja.... K‰ytt‰j‰lt‰ pit‰‰ kysy‰ valinta. ------------------------------

		// Muutetaan jokaisen osuneen kirjan ominaisuus "Lainassa" olemaan hieman enemm‰n k‰ytt‰j‰yst‰v‰llisempi:
		for (auto osuma = osumat.begin(); osuma != osumat.end(); ++osuma){
			if ((*osuma).at("Lainassa") == "1") {
				(*osuma).at("Lainassa") = "Kirja on lainassa";
			}
			else {
				(*osuma).at("Lainassa") = "";
			}
		}

		// K‰yttˆliittym‰ hoitaa kirjayksilˆn kysymisen. Palauttaa kirjan ID-numeron:
		int valinta = Kayttoliittyma::Yksilovalinta(osumat, "ID-numero","Teoksen nimi", "Valitse n‰ist‰ kirjoista haluamasi");

		if (valinta == 0) {	// Ei valittu yht‰k‰‰n...
			return palauta;
		}
		else {
			// K‰ytt‰j‰ teki valinnan, eli tied‰mme kirjan ID-numeron. Haemme kirjan pointterin k‰ytt‰m‰ll‰ NoudaKirja(int) -funktiota:
			palauta = NoudaKirja(valinta);
		}
	}




	else if (tilanne == HAKU_OK_YKSI) {
		// ---------------------- Vain yksi hakutulos: Voidaan palauttaa suoraan se. -------------------------------------------------
		palauta = NoudaKirja(stoi(osumat[0]["ID-numero"]));
	}

	return palauta;
}



Kirja* KirjaHaku::NoudaKirja(unsigned int kirjaID) {
	//Noutaa kirjan pointterin perustuen kirjan ID-tietoon:
	Kirja* palauta = nullptr;

	for (unsigned int i = 0; i < this->hakulista.size(); i++) {
		if (this->hakulista[i]->kirjaID == kirjaID) {
			palauta =this->hakulista[i];
			break;
		}
	}
	return palauta;
}

// Kirjan hakumetodit p‰‰ttyy
//-------------------------------------------------------------------------------------------------------------------------------------------------------

// Asiakkaan hakumetodit alkaa

AsiakasHaku::AsiakasHaku(vector<Asiakas>& myHakulista)
{
	// Asiakashaun constructor: Haetaan hakulista -ominaisuuteen pointterit kirjoista.

	for (unsigned int i = 0; i < myHakulista.size(); i++) {
		osumat.push_back((myHakulista[i]).haeAssoc());			// Talletetaan kaikkien kirjojen ominaisuudet map:iin
		hakulista.push_back(&(myHakulista[i]));					//Talletetaan osoitteet kaikista asiakkaista pointterivektoriin
	}

	PaivitaTilanne();											// P‰ivitet‰‰n haun tilanne-attribuutti oikeaksi

}

Asiakas* AsiakasHaku::NoudaAsiakas() {
	// Asiakkaan haun p‰‰metodi: Kysyt‰‰n k‰ytt‰j‰lt‰ suodatusehdot
	// Palautetaan pointteri valitusta asiakkaasta.

	Asiakas* palauta = nullptr;

	Asiakas tyhjaAsiakas;
	hakukriteerit = tyhjaAsiakas.haeAssoc();					// Haetaan hakukriteerit: Koska t‰m‰ instanssi on tyhj‰ versio asiakkaasta, saadaan sielt‰ assosiatiivinen map tyhjill‰ arvoilla
	hakukriteerit.erase("Koko nimi");							// Koko nime‰ ei tarvitse hakea - etunimi ja sukunimi erikseen riitt‰‰.
	hakukriteerit.erase("Salasana");							// Salasanaa ei kriteerin‰

	// K‰ytet‰‰n kriteerit k‰yttˆliittym‰ll‰, jossa k‰ytt‰j‰ lis‰‰ haluamansa suodatusehdot.
		if (Kayttoliittyma::KysySuodatus(hakukriteerit, "Asiakkaan haku") == false)
		{// K‰ytt‰j‰ keskeytti haun...
			return palauta;
		}

	// ::::::::::::::::::::::::::::::::. Muuten aloitamme tulosten karsinnan ::::::::::::::::::::::::::::::::::::::::::::...

		// K‰yd‰‰n jokainen kriteeri l‰pi yksi kerrallaan:
		for (map<string, string>::iterator i = hakukriteerit.begin(); i != hakukriteerit.end(); ++i) {
			if (!(tilanne == HAKU_OK_YKSI || tilanne == HAKU_EI_OSUMIA)){					// Karsintaa ei suoriteta, mik‰li ei ole mit‰‰n karsittavaa
				// M‰‰ritell‰‰n t‰m‰n ominaisuuden haun tiukkuus:
				bool tiukkaHaku = false;
				if (i->first == "ID-numero") tiukkaHaku = true;								// ID-numeron tulee olla tiukka, ts. karsitaan muut pois paitsi se yksilˆ, jonka ID on juuri sama mit‰ k‰ytt‰j‰ haluaa
				else if (i->first == "Henkilˆtunnus") tiukkaHaku = true;					// Henkilˆtunnuksen tulee olla tiukka

				else if (strstr(i->second.c_str(), "\"") != nullptr) tiukkaHaku = true;			// Jos k‰ytt‰j‰ on kirjoittanut kriteerin lainausmerkkeihin, haetaan tarkalla haulla.


				// Karsitaan listaa. Argumenttina kriteeri&tulos -pari (esim. ["Etunimi"] = "Jaakko" sek‰ ‰sken m‰‰ritetty haun tiukkuus):
				KarsiOsumat(make_pair(i->first, i->second), tiukkaHaku);
			}
		}

		//:::::::::::::::::::::::::::::: Suurin karsinta tehty. Jatko riippuu siit‰, mik‰ on tulosten lukum‰‰r‰ :::::::::::::::::::::::::
		
		if (tilanne == HAKU_OK_MONTA) {
			// ::::::::::::::::::::::::: Viel‰ on liikaa vaihtoehtoja.... K‰ytt‰j‰lt‰ pit‰‰ kysy‰ valinta :::::::::::::::::::::::::::::::
			// Siistit‰‰n hieman k‰ytt‰j‰lle n‰ytett‰v‰‰ listausta:
			for (auto osuma = osumat.begin(); osuma != osumat.end(); ++osuma){
				(*osuma).erase("Etunimi");										//Etunimi -kentt‰‰ ei n‰ytet‰, koska tarjolla on "Koko nimi" -ominaisuus
				(*osuma).erase("Sukunimi");										//Sukunimi -kentt‰‰ ei n‰ytet‰, koska tarjolla on "Koko nimi" -ominaisuus
				(*osuma).erase("Salasana");										// Ei n‰ytet‰ salasanaa
				
				// Poistetaan listasta lainausten tiedot -ominaisuudet: (K‰yd‰‰n ominaisuudet l‰pi ja poistetaan kaikki ominaisuudet, jotka alkavat tekstill‰ "Laina_".)
				for (auto ominaisuus = (*osuma).begin(); ominaisuus != (*osuma).end();) {
					if (strstr((*ominaisuus).first.c_str(), "Laina_") != nullptr) {
						ominaisuus = (*osuma).erase(ominaisuus);
					}
					else {
						++ominaisuus;
					}
				}
				
				// Siistit‰‰n lainakielto -ominaisuus n‰timm‰ksi:
				if ((*osuma).at("Lainakielto") == "1") {						
					(*osuma).at("Lainakielto") = "Asiakas on lainakiellossa";
				}
				else {
					(*osuma).at("Lainakielto") = "";
				}
				// Tutkitaan sakot, jos on, n‰ytet‰‰n:
				if (stod((*osuma).at("Sakot")) == 0) {
					(*osuma).erase("Sakot");		//Otetaan rivi pois, kun ei ole sakkoja
				}
				else {	// Muunnetaan sakkom‰‰r‰ k‰ytt‰j‰yst‰v‰lliseen muotoon:
					char sakko[50] ;
					sprintf(sakko,"%4.2f eur", stod((*osuma).at("Sakot")) );
					(*osuma).at("Sakot") = sakko;

					//delete sakko;
				}
			}

			// Vied‰‰n kriteereihin osuneet kirjat k‰yttˆliittym‰lle, jossa k‰ytt‰j‰ saa valita mieluisensa:
			int valinta = Kayttoliittyma::Yksilovalinta(osumat, "ID-numero", "Koko nimi", "Valitse n‰ist‰ asiakkaista haluamasi");

			if (valinta == 0) {	// Ei valittu yht‰k‰‰n...
				return palauta;
			}
			else {
				// K‰ytt‰j‰ teki valinnan, eli tied‰mme asiakkaan ID-numeron. Haemme pointterin k‰ytt‰m‰ll‰ NoudaAsiakas(int) -funktiota.
				palauta = NoudaAsiakas(valinta);
			}
		}


		else if (tilanne == HAKU_OK_YKSI) {
			// ::::::::::::::::::::::: Vain yksi hakutulos: Voidaan palauttaa suoraan se. :::::::::::::::::::::::::::::::::::::::::::::::
			palauta = NoudaAsiakas(stoi(osumat[0]["ID-numero"]));
		}

		return palauta;
	}

Asiakas* AsiakasHaku::NoudaAsiakas(unsigned int asiakasID) {
		//Noutaa kirjan pointterin perustuen kirjan ID-tietoon:
		Asiakas* palauta = nullptr;

		for (unsigned int i = 0; i < this->hakulista.size(); i++) {
			if (this->hakulista[i]->ID == asiakasID) {
				palauta = this->hakulista[i];
				break;
			}
		}
		return palauta;
	}

//Asiakkaan hakumetodit p‰‰ttyy
//-------------------------------------------------------------------------------------------------------------------------------------------------------

// Tyˆntekij‰n hakumetodit alkaa

TyontekijaHaku::TyontekijaHaku(vector<Tyontekija>& myHakulista)
{
	// Tyontekijahaun constructor: Haetaan hakulista -ominaisuuteen pointterit kirjoista.

	for (unsigned int i = 0; i < myHakulista.size(); i++) {
		osumat.push_back((myHakulista[i]).haeAssoc());			// Talletetaan kaikkien kirjojen ominaisuudet map:iin
		hakulista.push_back(&(myHakulista[i]));					//Talletetaan osoitteet kaikista asiakkaista pointterivektoriin
	}

	PaivitaTilanne();											// P‰ivitet‰‰n haun tilanne-attribuutti oikeaksi

}

Tyontekija* TyontekijaHaku::NoudaTyontekija() {
	// Tyˆntekij‰n haun p‰‰metodi: Kysyt‰‰n k‰ytt‰j‰lt‰ suodatusehdot
	// Palautetaan pointteri valitusta tyˆntekij‰st‰.

	Tyontekija* palauta = nullptr;

	Tyontekija tyhjaTyontekija;
	hakukriteerit = tyhjaTyontekija.haeAssoc();					// Haetaan hakukriteerit: Koska t‰m‰ instanssi on tyhj‰ versio tyˆntekij‰st‰, saadaan sielt‰ assosiatiivinen map tyhjill‰ arvoilla
	hakukriteerit.erase("Koko nimi");							// Koko nime‰ ei tarvitse hakea - etunimi ja sukunimi erikseen riitt‰‰.
	hakukriteerit.erase("Salasana");							// Salasanaa ei kriteerin‰
	hakukriteerit.erase("Yksikˆiden lkm");

	// K‰ytet‰‰n kriteerit k‰yttˆliittym‰ll‰, jossa k‰ytt‰j‰ lis‰‰ haluamansa suodatusehdot.
	if (Kayttoliittyma::KysySuodatus(hakukriteerit, "Tyˆntekij‰n haku") == false)
	{// K‰ytt‰j‰ keskeytti haun...
		return palauta;
	}

	// ::::::::::::::::::::::::::::::::. Muuten aloitamme tulosten karsinnan ::::::::::::::::::::::::::::::::::::::::::::...

	// K‰yd‰‰n jokainen kriteeri l‰pi yksi kerrallaan:
	for (map<string, string>::iterator i = hakukriteerit.begin(); i != hakukriteerit.end(); ++i) {
		if (!(tilanne == HAKU_OK_YKSI || tilanne == HAKU_EI_OSUMIA)){					// Karsintaa ei suoriteta, mik‰li ei ole mit‰‰n karsittavaa
			// M‰‰ritell‰‰n t‰m‰n ominaisuuden haun tiukkuus:
			bool tiukkaHaku = false;
			if (i->first == "ID-numero") tiukkaHaku = true;								// ID-numeron tulee olla tiukka, ts. karsitaan muut pois paitsi se yksilˆ, jonka ID on juuri sama mit‰ k‰ytt‰j‰ haluaa
			else if (i->first == "Henkilˆtunnus") tiukkaHaku = true;					// Henkilˆtunnuksen tulee olla tiukka

			else if (strstr(i->second.c_str(), "\"") != nullptr) tiukkaHaku = true;			// Jos k‰ytt‰j‰ on kirjoittanut kriteerin lainausmerkkeihin, haetaan tarkalla haulla.


			// Karsitaan listaa. Argumenttina kriteeri&tulos -pari (esim. ["Etunimi"] = "Jaakko" sek‰ ‰sken m‰‰ritetty haun tiukkuus):
			KarsiOsumat(make_pair(i->first, i->second), tiukkaHaku);
		}
	}

	//:::::::::::::::::::::::::::::: Suurin karsinta tehty. Jatko riippuu siit‰, mik‰ on tulosten lukum‰‰r‰ :::::::::::::::::::::::::

	if (tilanne == HAKU_OK_MONTA) {
		// ::::::::::::::::::::::::: Viel‰ on liikaa vaihtoehtoja.... K‰ytt‰j‰lt‰ pit‰‰ kysy‰ valinta :::::::::::::::::::::::::::::::
		// Siistit‰‰n hieman k‰ytt‰j‰lle n‰ytett‰v‰‰ listausta:
		for (auto osuma = osumat.begin(); osuma != osumat.end(); ++osuma){
			(*osuma).erase("Etunimi");										//Etunimi -kentt‰‰ ei n‰ytet‰, koska tarjolla on "Koko nimi" -ominaisuus
			(*osuma).erase("Sukunimi");										//Sukunimi -kentt‰‰ ei n‰ytet‰, koska tarjolla on "Koko nimi" -ominaisuus
			(*osuma).erase("Salasana");										// Ei n‰ytet‰ salasanaa
		}

		// Vied‰‰n kriteereihin osuneet tyˆntekij‰t k‰yttˆliittym‰lle, jossa k‰ytt‰j‰ saa valita mieluisensa:
		int valinta = Kayttoliittyma::Yksilovalinta(osumat, "ID-numero", "Koko nimi", "Valitse n‰ist‰ tyˆntekijˆist‰ haluamasi");

		if (valinta == 0) {	// Ei valittu yht‰k‰‰n...
			return palauta;
		}
		else {
			// K‰ytt‰j‰ teki valinnan, eli tied‰mme asiakkaan ID-numeron. Haemme pointterin k‰ytt‰m‰ll‰ NoudaTyontekija(int) -funktiota.
			palauta = NoudaTyontekija(valinta);
		}
	}


	else if (tilanne == HAKU_OK_YKSI) {
		// ::::::::::::::::::::::: Vain yksi hakutulos: Voidaan palauttaa suoraan se. :::::::::::::::::::::::::::::::::::::::::::::::
		palauta = NoudaTyontekija(stoi(osumat[0]["ID-numero"]));
	}

	return palauta;
}

Tyontekija* TyontekijaHaku::NoudaTyontekija(unsigned int tyontekijaID) {
	//Noutaa tyˆntekij‰n pointterin perustuen ID-tietoon:
	Tyontekija* palauta = nullptr;

	for (unsigned int i = 0; i < this->hakulista.size(); i++) {
		if (this->hakulista[i]->ID == tyontekijaID) {
			palauta = this->hakulista[i];
			break;
		}
	}
	return palauta;
}

//Tyˆntekij‰n hakumetodit p‰‰ttyy




// Haku tiedostosta
//int Haku::nouda_kirjat_tiedostosta(string tiedosto, vector<Kirja> &kirjat)
int Haku::nouda_kirjat_tiedostosta(string tiedosto, vector<Kirja> &kirjat)
{
	cout << "Haetaan kirjat::" << endl;
	Kirja kirja1;
	map<string, string> kirjanTiedot;

	string input;
	int kentta = 0;

	ifstream inputfile(tiedosto);				// Avataan tiedosto

	if (!inputfile)								// Jos ei saatu tiedostoa auki
		return 0;

	while (!inputfile.eof())					// Kunnes tulee tiedoston lopetus merkki
	{
		getline(inputfile, input, '|');			// P‰tkii tiedot '|' -merkin perusteella

		switch (kentta)
		{
		case 0:	// kirjaID
			kirjanTiedot["ID-numero"] = input;
			break;
		case 1:	// tekija
			kirjanTiedot["Tekij‰"] = input;
			break;
		case 2:	// teos
			kirjanTiedot["Teoksen nimi"] = input;
			break;
		case 3:	// UDK_koodi
			kirjanTiedot["UDK-koodi"] = input;
			break;
		case 4: // Laina-aika
			kirjanTiedot["Laina_aika"] = input; 
			break;
		case 5: // Lainaaja
			kirjanTiedot["Lainaaja"] = input;
			break;
		case 6: // Onko lainassa
			kirjanTiedot["Lainassa"] = input;
			
			//Kaikki tiedot saatu
			kirja1.lisaaTiedot(kirjanTiedot);
			kirjat.push_back(kirja1);	// Siirr‰ yhden kirjan tiedot vectoriin
			kentta = -1;
			break;
		default:
			break;
		}
		kentta++;
	}

	inputfile.close();					// Tiedoston sulkeminen
	return kirja1.kirjaID;				// Palautetaan tallennettujen kirjojen j‰rjestysnumero
}


int Haku::nouda_asiakkaat_tiedostosta(string tiedosto, vector<Asiakas> &asiakkaat)
{
	cout << "Haetaan asiakkaat::" << endl;
	Asiakas asiakas1;
	map<string, string> asiakkaanTiedot;

	string input;
	int kentta = 0;

	ifstream inputfile(tiedosto);			// Avataan tiedosto

	if (!inputfile)							// Jos ei saatu tiedostoa auki
		return 0;

	while (!inputfile.eof())					// Kunnes tulee tiedoston lopetus merkki
	{
		getline(inputfile, input, '|');			// P‰tkii tiedot '|' -merkin perusteella
		
		switch (kentta)
		{
		case 0:	// asiakasID
			asiakkaanTiedot["ID-numero"] = input;
			break;
		case 1:	// a_etunimi
			asiakkaanTiedot["Etunimi"] = input;
			break;
		case 2:	// tekija
			asiakkaanTiedot["Sukunimi"] = input;
			break;
		case 3:	// a_hetu
			asiakkaanTiedot["Henkilˆtunnus"] = input;
			break;
		case 4:	// a_osoite
			asiakkaanTiedot["Osoite"] = input;
			break;
		case 5:	// lainojen lukum‰‰r‰
		{asiakkaanTiedot["Lainojen lkm"] = input;

		string iteraattori = "";
		

		for (int i = 1; i <= stoi(asiakkaanTiedot["Lainojen lkm"]); i++) {
			for (int ominaisuuksienLkm = 1; ominaisuuksienLkm <= 4; ominaisuuksienLkm++) {
				getline(inputfile, input, '|');

				switch (ominaisuuksienLkm)
				{
				case 1:
					iteraattori = "Laina_" + to_string(i) + "_";
					asiakkaanTiedot[iteraattori + string("KirjaID")] = input;
					break;
				case 2:
					asiakkaanTiedot[iteraattori + string("Lainauspvm")] = input;
					break;
				case 3:
					asiakkaanTiedot[iteraattori + string("Tarkistuspvm")] = input;
					break;
				case 4:
					asiakkaanTiedot[iteraattori + string("Laina-aika")] = input;
					break;
				}
			}
		}

		break; }
		case 6:	// lainakielto
			asiakkaanTiedot["Lainakielto"] = input;
			break;
		case 7:	// Sakkojen m‰‰r‰
			asiakkaanTiedot["Sakot"] = input;
			break;
		case 8:	//K‰ytt‰j‰tunnus
			asiakkaanTiedot["K‰ytt‰j‰tunnus"] = input;
			break;
		case 9: // Salasana
			asiakkaanTiedot["Salasana"] = input;
			
		// Kaikki tiedot saatu
			asiakas1.lisaaTiedot(asiakkaanTiedot);
			asiakkaat.push_back(asiakas1);	// Siirr‰ yhden asiakkaan tiedot vectoriin
			asiakkaanTiedot.clear();
			asiakas1.lainat.clear();		// Tuhotaan lainavektori, jottei j‰ljemmille asiakkaille tule haamulainauksia
			kentta = -1;
			break;
		default:
			break;
		}
		kentta++;
	}

	inputfile.close();					// Tiedoston sulkeminen
	return asiakas1.ID;					// Palautetaan tallennettujen kirjojen j‰rjestysnumero
}

void Haku::tallenna_kirjat_tiedostoon(string tiedosto, vector<Kirja> &kirjat)
{
	ofstream outputfile(tiedosto);

	if (!outputfile)
		return;
	
	for each (auto kirja in kirjat)
	{
		map<string, string> kirja_tiedot = kirja.haeAssoc();

		outputfile << kirja_tiedot["ID-numero"] << "|"
			<< kirja_tiedot["Tekij‰"] << "|"
			<< kirja_tiedot["Teoksen nimi"] << "|"
			<< kirja_tiedot["UDK-koodi"] << "|"
			<< kirja_tiedot["Laina_aika"] << "|"
			<< kirja_tiedot["Lainaaja"] << "|"
			<< kirja_tiedot["Lainassa"] << "|" << endl;
	}
	outputfile.close();
}

void Haku::tallenna_asiakkaat_tiedostoon(string tiedosto, vector<Asiakas> &asiakkaat)
{
	ofstream outputfile(tiedosto);

	if (!outputfile)
		return;

	for each (auto asiakas in asiakkaat)
	{
		map<string, string> asiakas_tiedot = asiakas.haeAssoc();

		outputfile << asiakas_tiedot["ID-numero"] << "|"
			<< asiakas_tiedot["Etunimi"] << "|"
			<< asiakas_tiedot["Sukunimi"] << "|"
			<< asiakas_tiedot["Henkilˆtunnus"] << "|"
			<< asiakas_tiedot["Osoite"] << "|"
			<< asiakas_tiedot["Lainojen lkm"] << "|";
		
		
		for (int i = 1; i <= stoi(asiakas_tiedot["Lainojen lkm"]); i++) {
			string laina_i = "Laina_" + to_string(i) + "_";

			outputfile << asiakas_tiedot[laina_i + string("KirjaID")] << "|"
				<< asiakas_tiedot[laina_i + string("Lainauspvm")] << "|"
				<< asiakas_tiedot[laina_i + string("Tarkistuspvm")] << "|"
				<< asiakas_tiedot[laina_i + string("Laina-aika")] << "|";
			}

		outputfile << asiakas_tiedot["Lainakielto"] << "|" 
					<< asiakas_tiedot["Sakot"] << "|"
					<< asiakas_tiedot["K‰ytt‰j‰tunnus"] << "|" 
					<< asiakas_tiedot["Salasana"] << "|" << endl;
	}
	outputfile.close();
}

void Haku::tallenna_tyontekijat_tiedostoon(string tiedosto, vector<Tyontekija> &tyontekijat)
{
	ofstream outputfile(tiedosto);

	if (!outputfile)
		return;

	for each (auto tyontekija in tyontekijat)
	{
		map<string, string> henkilo_tiedot = tyontekija.haeAssoc();

		outputfile << henkilo_tiedot["ID-numero"] << "|"
			<< henkilo_tiedot["Etunimi"] << "|"
			<< henkilo_tiedot["Sukunimi"] << "|"
			<< henkilo_tiedot["Henkilˆtunnus"] << "|"
			<< henkilo_tiedot["Osoite"] << "|"
			<< henkilo_tiedot["Yksikˆiden lkm"] << "|";


		for (int i = 1; i <= stoi(henkilo_tiedot["Yksikˆiden lkm"]); i++) {
			string yksikko_i = "Yksikkˆ_" + to_string(i) + "_";

			outputfile << henkilo_tiedot[yksikko_i + "ID"] << "|"
				<< henkilo_tiedot[yksikko_i + "Rooli"] << "|"
				<< henkilo_tiedot["K‰ytt‰j‰tunnus"] << "|"
				<< henkilo_tiedot["Salasana"] << "|" << endl;
		}
	}
	outputfile.close();
}

int Haku::nouda_tyontekijat_tiedostosta(string tiedosto, vector<Tyontekija> &tyontekijat)
{
	cout << "Haetaan tyˆntekij‰t::" << endl;
	Tyontekija tyontekija;
	map<string, string> tyontekijanTiedot;

	string input;
	int kentta = 0;

	ifstream inputfile(tiedosto);			// Avataan tiedosto

	if (!inputfile)							// Jos ei saatu tiedostoa auki
		return 0;

	while (!inputfile.eof())					// Kunnes tulee tiedoston lopetus merkki
	{
		getline(inputfile, input, '|');			// P‰tkii tiedot '|' -merkin perusteella

		switch (kentta)
		{
		case 0:	// asiakasID
			tyontekijanTiedot["ID-numero"] = input;
			break;
		case 1:	// a_etunimi
			tyontekijanTiedot["Etunimi"] = input;
			break;
		case 2:	// tekija
			tyontekijanTiedot["Sukunimi"] = input;
			break;
		case 3:	// a_hetu
			tyontekijanTiedot["Henkilˆtunnus"] = input;
			break;
		case 4:	// a_osoite
			tyontekijanTiedot["Osoite"] = input;
			break;
		case 5:	// yksikˆiden lukum‰‰r‰
		{tyontekijanTiedot["Yksikˆiden lkm"] = input;

		string iteraattori = "";


		for (int i = 1; i <= stoi(tyontekijanTiedot["Yksikˆiden lkm"]); i++) {
			for (int ominaisuuksienLkm = 1; ominaisuuksienLkm <= 2; ominaisuuksienLkm++) {
				getline(inputfile, input, '|');

				switch (ominaisuuksienLkm)
				{
				case 1:
					iteraattori = "Yksikkˆ_" + to_string(i) + "_";
					tyontekijanTiedot[iteraattori + string("ID")] = input;
					break;
				case 2:
					tyontekijanTiedot[iteraattori + string("Rooli")] = input;
					break;
				}
			}
		}
		}
		case 6:	//K‰ytt‰j‰tunnus
			tyontekijanTiedot["K‰ytt‰j‰tunnus"] = input;
			break;
		case 7: // Salasana
			tyontekijanTiedot["Salasana"] = input;

		// Kaikki tiedot saatu
		tyontekija.lisaaTiedot(tyontekijanTiedot);
		tyontekijat.push_back(tyontekija);	// Siirr‰ yhden tyˆntekij‰n tiedot vectoriin
		tyontekijanTiedot.clear();
		tyontekija.yksikko.clear();		// Tuhotaan yksikkˆ, jottei j‰ljemmille tule haaamuja
		kentta = -1;
		break;
		
		default:
			break;
		}
		kentta++;
	}

	inputfile.close();					// Tiedoston sulkeminen
	return tyontekija.ID;					// Palautetaan tallennettujen tyˆntekijˆiden viimeisin ID-numero
}

