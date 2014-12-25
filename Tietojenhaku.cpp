#include "stdafx.h"
#include "Tietojenhaku.h"


Tietojenhaku::Tietojenhaku()
{
}


Tietojenhaku::~Tietojenhaku()
{
}


int Tietojenhaku::NoudaKirjat(vector<Kirja> &kirjat)
{
	int lukumaara = 0;
	
	Kirja kirja1;
		kirja1.kirjaID = 1;
		kirja1.teos = "C++ For Beginners";
		kirja1.tekija = "Andrev Brokolli";
		kirja1.UDK_koodi = "4";

	kirjat.push_back(kirja1);
	lukumaara = 1;

	Kirja kirja2;
		kirja2.kirjaID = 2;
		kirja2.teos = "Pupu Tupuna Neuvolassa";
		kirja2.tekija = "Koskimies Pirkko";
		kirja2.UDK_koodi = "379.8";
	
	kirjat.push_back(kirja2);
	lukumaara = 2;

	Kirja kirja3;
	kirja3.kirjaID = 3;
	kirja3.teos = "C++ For Beginners";
	kirja3.tekija = "Andrev Brokolli";
	kirja3.UDK_koodi = "379.8";

	kirjat.push_back(kirja3);
	lukumaara = 3;




	return lukumaara;
}

int Tietojenhaku::NoudaAsiakkaat(vector<Asiakas>& asiakkaat) {
	Asiakas a1;
	a1.etunimi = "Erkki";
	a1.sukunimi = "Eka";
	a1.hetu = "120475-134K";
	a1.osoite = "Torikatu 5, 24100 Salo";

	asiakkaat.push_back(a1);

	Asiakas a2;
	a2.etunimi = "Tiina";
	a2.sukunimi = "Toka";
	a2.hetu = "120475-1542";
	a2.osoite = "Helsingintie 5, 24100 Salo";

	asiakkaat.push_back(a2);

	return 2;
}
