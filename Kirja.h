#pragma once

class Kirja
{
//Ominaisuudet

public:

	int kirjaID;			// Kirja oliokohtainen ID
	string tekija;
	string teos;
	string UDK_koodi;
	int laina_aika;		  //Kirjan laina-aika p‰ivin‰
	int lainaaja;			// Lainaaja = Asiakkaan ID

private:
	bool lainassa = false;



// Metodit:

private:

	/* static int Kirja::hae_ai_ID(void) {
		 static int ai_ID = 1;  // Kirjan ID-numeron luontiin k‰ytett‰v‰, joka kerralla yhdell‰ lis‰‰ntyv‰ ID-numero

		 return ai_ID++;
	 }*/

public:
	Kirja();
	~Kirja();

	bool onkoLainassa();
	bool lainaa(int asiakasID);
	void palauta();
	string naytaTiedot();
	bool lisaaTiedot(int &viimeisinID);

	map<string, string> haeAssoc();				//Palauttaa instanssin ominaisuudet ja niihin asetetut arvot tyylill‰ [ominaisuus] = arvo.
	void lisaaTiedot(map<string, string> tiedot);	// Tallettaa instanssiin argumenttina tuodut ominaisuudet
	bool muutaTietoja();							// K‰ytt‰j‰lle tulostetaan kirjan ominaisuudet, joita h‰n voi halutessaan muokata
};

