// Kirjastoharjoitus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"




int _tmain(int argc, _TCHAR* argv[])
{
	// luo kirjasto-tyyppinen instanssi (UI), k�skyt� sit�.
	
	setlocale(LC_ALL, "");

		Kirjasto laitoskirjasto;

		// Kysyt��n k�ytt�j�tunnus/salasana.
		Kirjasto::Kayttajaprofiili kayttajanProfiili;

		while (true) { // Kysyt��n tunnuksia kunnes tulee kelpaavat, tai k�yttt�j� lopettaa.

			// ---------------------- Tehd��n valikko:
			unordered_map<string, Kayttoliittyma::Tiedot> logini;
			logini["1"].kysymys = "K�ytt�j�tunnus (j�t� tyhj�ksi jos ei tunnusta)";
			logini["2"].kysymys = "Salasana";

			Kayttoliittyma::KysyTiedot(logini, "Kirjaudu sis��n kirjastoon");

			// ------------------------ Tarkastetaan annetut tunnukset ja poimitaan k�ytt�j�
			kayttajanProfiili = laitoskirjasto.TarkistaOikeudet(logini["1"].vastaus, logini["2"].vastaus);

			if (kayttajanProfiili == Kirjasto::USER_INVALID) {
				// K�ytt�j�� ei l�ytynyt annetuilla tunnuksilla, tai tunnus/salasana oli v��rin.
				Kayttoliittyma::Informaatio("K�ytt�j�tunnus tai salasana v��rin.");
			}
			else {
				// K�ytt�j� l�ytyi, voidaan aloittaa itse ohjelma:
				break;
			}
			system("pause");
		}

		// Looppia k�yd��n l�pi kunnes k�ytt�j� ei en�� halua jatkaa:
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
	// T�ydet oikeudet omaava k�ytt�j�:

	// Muodostetaan valikossa n�ytett�v�t kohdat:
	map<int, string> valinnat;
	valinnat[1] = "hae kirjoja";
	valinnat[2] = "uusi kirja";
	valinnat[3] = "uusi asiakas";
	valinnat[4] = "hae asiakkaita";
	valinnat[5] = "uusi lainaus";
	if (profiili == Kirjasto::USER_ADMIN) {		// Jos nykyinen k�ytt�j� omaa paremmat oikeudet, saa h�n lis�� valittavaa
		valinnat[6] = "uusi virkailija";
		valinnat[7] = "hae virkailijaa";
	}

	int toiminto = 0;

	
		// K�ytt�liittym�-luokka hoitaa valikon n�yt�n ja kyselyn. Palauttaa toiminnon numeron
		toiminto = Kayttoliittyma::Valikko(valinnat, "Kirjasto");

		switch (toiminto)
		{
		case 1:													//hae kirjoja
			laitoskirjasto.main_haeKirja();
			break;

		case 2:			laitoskirjasto.main_lisaaKirja();			//lis�� kirja

			break;


		case 3:			laitoskirjasto.lisaaAsiakas();			//lis�� asiakas   

			break;

		case 4:			laitoskirjasto.main_haeAsiakas();			//poista asiakas

			break;

		case 5:			laitoskirjasto.lisaaLainaus();			//lis�� lainaus

			break;


		case 6:			
			if (profiili == Kirjasto::USER_ADMIN) {
				laitoskirjasto.lisaaTyontekija();		//lis�� uusi ty�ntekij�
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

	// Muodostetaan valikossa n�ytett�v�t kohdat:
	map<int, string> valinnat;
	valinnat[1] = "hae kirjoja";
	valinnat[2] = "muuta tietojasi";

	int toiminto = 0;


	// K�ytt�liittym�-luokka hoitaa valikon n�yt�n ja kyselyn. Palauttaa toiminnon numeron
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

	// Muodostetaan valikossa n�ytett�v�t kohdat:
	map<int, string> valinnat;
	valinnat[1] = "hae kirjoja";

	int toiminto = 0;

	// K�ytt�liittym�-luokka hoitaa valikon n�yt�n ja kyselyn. Palauttaa toiminnon numeron
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