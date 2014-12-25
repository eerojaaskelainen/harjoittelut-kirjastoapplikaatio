// Kirjastoharjoitus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"




int _tmain(int argc, _TCHAR* argv[])
{
	// luo kirjasto-tyyppinen instanssi (UI), käskytä sitä.
	
	setlocale(LC_ALL, "");

		Kirjasto laitoskirjasto;

		// Kysytään käyttäjätunnus/salasana.
		Kirjasto::Kayttajaprofiili kayttajanProfiili;

		while (true) { // Kysytään tunnuksia kunnes tulee kelpaavat, tai käytttäjä lopettaa.

			// ---------------------- Tehdään valikko:
			unordered_map<string, Kayttoliittyma::Tiedot> logini;
			logini["1"].kysymys = "Käyttäjätunnus (jätä tyhjäksi jos ei tunnusta)";
			logini["2"].kysymys = "Salasana";

			Kayttoliittyma::KysyTiedot(logini, "Kirjaudu sisään kirjastoon");

			// ------------------------ Tarkastetaan annetut tunnukset ja poimitaan käyttäjä
			kayttajanProfiili = laitoskirjasto.TarkistaOikeudet(logini["1"].vastaus, logini["2"].vastaus);

			if (kayttajanProfiili == Kirjasto::USER_INVALID) {
				// Käyttäjää ei löytynyt annetuilla tunnuksilla, tai tunnus/salasana oli väärin.
				Kayttoliittyma::Informaatio("Käyttäjätunnus tai salasana väärin.");
			}
			else {
				// Käyttäjä löytyi, voidaan aloittaa itse ohjelma:
				break;
			}
			system("pause");
		}

		// Looppia käydään läpi kunnes käyttäjä ei enää halua jatkaa:
		do {
			if (kayttajanProfiili == Kirjasto::USER_VIRKAILIJA || kayttajanProfiili == Kirjasto::USER_ADMIN) {
				if (TulostaTyontekija(laitoskirjasto, kayttajanProfiili) == false) {
					break;
				}
			}
			else if (kayttajanProfiili == Kirjasto::USER_ASIAKAS) {
				if (TulostaAsiakas(laitoskirjasto) == false) {
					break;
				}
			}

			else {
				if (TulostaAnonyymi(laitoskirjasto) == false) {
					break;
				}
			}
		

			system("pause");
			system("CLS");
		} while (true);

	return 0;
}

bool TulostaTyontekija(Kirjasto &laitoskirjasto, Kirjasto::Kayttajaprofiili &profiili) {
	// Täydet oikeudet omaava käyttäjä:

	// Muodostetaan valikossa näytettävät kohdat:
	map<int, string> valinnat;
	valinnat[1] = "hae kirjoja";
	valinnat[2] = "uusi kirja";
	valinnat[3] = "uusi asiakas";
	valinnat[4] = "hae asiakkaita";
	valinnat[5] = "uusi lainaus";
	if (profiili == Kirjasto::USER_ADMIN) {		// Jos nykyinen käyttäjä omaa paremmat oikeudet, saa hän lisää valittavaa
		valinnat[6] = "uusi virkailija";
		valinnat[7] = "hae virkailijaa";
	}

	int toiminto = 0;

	
		// Käyttöliittymä-luokka hoitaa valikon näytön ja kyselyn. Palauttaa toiminnon numeron
		toiminto = Kayttoliittyma::Valikko(valinnat, "Kirjasto");

		switch (toiminto)
		{
		case 1:													//hae kirjoja
			laitoskirjasto.main_haeKirja();
			break;

		case 2:			laitoskirjasto.main_lisaaKirja();			//lisää kirja

			break;


		case 3:			laitoskirjasto.lisaaAsiakas();			//lisää asiakas   

			break;

		case 4:			laitoskirjasto.main_haeAsiakas();			//poista asiakas

			break;

		case 5:			laitoskirjasto.lisaaLainaus();			//lisää lainaus

			break;


		case 6:			
			if (profiili == Kirjasto::USER_ADMIN) {
				laitoskirjasto.lisaaTyontekija();		//lisää uusi työntekijä
			}
			break;

		case 7:
			if (profiili == Kirjasto::USER_ADMIN) {
				laitoskirjasto.main_haeTyontekija();		//hae asiakasta
			}
			break;
		case 0:

			return false;

		default:
			Kayttoliittyma::Virhe("En tunnistanut toimintoa!");


			break;
		}
		return true;
}

bool TulostaAsiakas(Kirjasto &laitoskirjasto) {
	// Tulostetaan valikko asiakkaalle

	// Muodostetaan valikossa näytettävät kohdat:
	map<int, string> valinnat;
	valinnat[1] = "hae kirjoja";
	valinnat[2] = "muuta tietojasi";

	int toiminto = 0;


	// Käyttöliittymä-luokka hoitaa valikon näytön ja kyselyn. Palauttaa toiminnon numeron
	toiminto = Kayttoliittyma::Valikko(valinnat, "Kirjasto",laitoskirjasto.naytaAsiakkaanTiedot(laitoskirjasto.nykyinenKayttajaAsiakas) ) ;

	switch (toiminto)
	{
	case 1:										//hae kirjoja
		laitoskirjasto.lisaaLainaus(nullptr, laitoskirjasto.nykyinenKayttajaAsiakas);
		break;

	case 2:			laitoskirjasto.nykyinenKayttajaAsiakas->Muokkaa();			//Muokkaa tietoja

		break;
	case 0:
		return false;
	default:
		Kayttoliittyma::Virhe("En tunnistanut toimintoa!");
		break;
	}

	return true;
}

bool TulostaAnonyymi(Kirjasto &laitoskirjasto) {
	// Tulostetaan anonyymi valikko

	// Muodostetaan valikossa näytettävät kohdat:
	map<int, string> valinnat;
	valinnat[1] = "hae kirjoja";

	int toiminto = 0;

	// Käyttöliittymä-luokka hoitaa valikon näytön ja kyselyn. Palauttaa toiminnon numeron
	toiminto = Kayttoliittyma::Valikko(valinnat, "Kirjasto");

	switch (toiminto)
	{
	case 1:										//hae kirjoja
		laitoskirjasto.main_haeKirja();
		break;

	case 0:
		return false;
	default:
		Kayttoliittyma::Virhe("En tunnistanut toimintoa!");
		break;
	}

	return true;
}