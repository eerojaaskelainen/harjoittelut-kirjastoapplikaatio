#include "stdafx.h"
#include "asiakasHaku.h"


asiakasHaku::asiakasHaku(vector<Asiakas>& myHakulista)
{
	// Constructor: Haetaan hakulista -ominaisuuteen pointterit kirjoista.

	for (unsigned int i = 0; i < myHakulista.size(); i++) {
		hakulista.push_back(&(myHakulista[i]));
	}

	if (hakulista.size() == 0) {
		cout << "Kirjaston tiedossa ei ole asiakkaita!";
	}
}


asiakasHaku::~asiakasHaku()
{
	
}

asiakasHaku::AsiakashaunTulos asiakasHaku::MuodostaHakukriteerit(asiakasHaku::AsiakkaanKentat kriteeri, string kysymysKayttajalle) {
	// Asiakasn hakukriteerien muodostus. K‰ytt‰j‰lt‰ kysyt‰‰n hakukriteeri (argumenttina) ja seulotaan hakulista-ominaisuudesta pois ne mitk‰ eiv‰t kriteeriin sovi.
	// Palauttaa hakutulosten tilan.

	AsiakashaunTulos palauta;					// Palautettava hakutulosten tila
	string syote = "";						// K‰ytt‰j‰n antama kriteeri

	//Kysyt‰‰n k‰ytt‰j‰lt‰ kriteeri‰:
	cout << kysymysKayttajalle << ":";
	getline(cin, syote);

	//Jos k‰ytt‰j‰ haluaa keskeytt‰‰:
	if (syote == "0") {
		palauta = ASIAKASHAKU_LOPETA;
		return palauta;
	}

	//Jos t‰t‰ kriteeri‰ ei k‰ytet‰ (K‰ytt‰j‰ ei antanut mit‰‰n):
	else if (syote == "") {
		// Tutkitaan mik‰ on t‰m‰n hetkinen haun tilanne
		if (hakulista.size() == 1){
			palauta = ASIAKASHAKU_OK_YKSI;
		}
		else if (hakulista.size() > 1) {
			palauta = ASIAKASHAKU_OK_MONTA;
		}
		else {
			palauta = ASIAKASHAKU_EI_ASIAKKAITA;
		}
		return palauta;
	}


	// Muussa tapauksessa jatketaan:

	try {
		for (vector<Asiakas*>::iterator i = hakulista.begin(); i != hakulista.end();) {

			bool poistetaanko = false;			//Poistovipu: Jos kirja ei osunut kriteereihin, poistetaan se listasta.

			if (DEBUGI) cout << (*i)->etunimi << " " << (*i)->sukunimi << endl;

			// Tehd‰‰n tarkistus. Jos kirja ei osu kriteeriin, kytket‰‰n poistovipu p‰‰lle:

			switch (kriteeri) {
			case KENTTA_ID:
				// Kriteerin‰ kirjan ID:
				if ((*i)->ID == stoi(syote)) {				//stoi : c++11 parempi ratkaisu kuin aiempi atoi (http://stackoverflow.com/questions/7663709/convert-string-to-int-c )

					//Koska ID on uniikki, ei tarvitse en‰‰ jatkaa hakua. poistetaan muut kirjat ja palautetaan ookoo:
					Asiakas* temp = (*i);
					hakulista.clear();
					hakulista.push_back(temp);
					return ASIAKASHAKU_OK_YKSI;
				}
				break;

			case KENTTA_SUKUNIMI:
				//Kriteerin‰ kirjan nimi:				
				if (strstr((*i)->sukunimi.c_str(), syote.c_str()) == nullptr) {
					poistetaanko = true;
				}
				break;

			case KENTTA_HETU:
				//Kriteerin‰ kirjan tekij‰:
				if (strstr((*i)->hetu.c_str(), syote.c_str()) == nullptr) {
					poistetaanko = true;
				}
				break;

			default:
				// Jos kriteeriksi ei tunnistettu mit‰‰n, on se ohjelman kaatavan poikkeuksen paikka:
				runtime_error("Asiakas:Hakukriteeria ei tunnistettu");		//Jos kriteeri‰ ei tunnettu, keskeytet‰‰n ohjelma

			}

			//-----------------------------------------------------------------------------------------------------------
			// Tarkistetaan vipu: Jos t‰m‰ asiakas ei osunut kriteeriin, otetaan se pois tuloslistalta. Muuten jatketaan loopissa seuraavaan.
			if (poistetaanko == true) {
				i = hakulista.erase(i);
			}
			else
			{
				++i;
			}

		}
	}
	//Jos ei onnistuttu muuntamaan annettua syˆtett‰, poistutaan loopista, kerrotaan k‰ytt‰j‰lle ett‰ syˆte ei kelvannut, mutta jatkamme kuitenkin suodatusta.
	catch (invalid_argument e) {
		cout << "Antamasi syote ei kelpaa. Vain kokonaislukuja!" << endl;

	}

	// Tutkitaan milt‰ tuloslista n‰ytt‰‰ suodatuksen j‰lkeen:

	if (hakulista.size() == 0) {
		//Listaan ei j‰‰nyt kirjoja, eli kriteereill‰ ei lˆytynyt yht‰‰n kirjaa.
		palauta = ASIAKASHAKU_EI_ASIAKKAITA;
	}
	else if (hakulista.size() >1){
		//Listaan j‰i useampi kirja.
		palauta = ASIAKASHAKU_OK_MONTA;
	}
	else {
		//Listaan j‰i yksi kirja.
		palauta = ASIAKASHAKU_OK_YKSI;
	}


	return palauta;
}


asiakasHaku::AsiakashaunTulos asiakasHaku::PyydaValitsemaanYksi() {
	// Kirjoista on haettu kriteereihin sopivat, mutta viel‰ pit‰‰ kysy‰ k‰ytt‰j‰lt‰ se tietty kirja.
	// K‰yd‰‰n l‰pi haetut, pyydet‰‰n k‰ytt‰j‰‰ valitsemaan yksi, ja palautetaan se.

	AsiakashaunTulos tulos;
	int valinta = 0;



	cout << "Valitse naista asiakkaista yksi:" << endl
		<< "------------------------------" << endl;

	for (unsigned int i = 0; i < hakulista.size(); i++) {

		cout << i + 1 << " : ";
		hakulista[i]->naytaTiedot();
		cout << endl;
	}
	cin >> valinta;
	cin.sync();

	if (valinta == 0) {//K‰ytt‰j‰ halusi lopettaa valitsematta.
		tulos = ASIAKASHAKU_LOPETA;
	}
	else if (valinta < 0 || valinta > hakulista.size()) {
		// K‰ytt‰j‰ antoi virheellisen vastauksen
		cout << "Antamallasi lukua ei ole listassa. Anna haluamasi asiakasrivin numero.";
		tulos = ASIAKASHAKU_VAARA_HAKU;
	}
	else {

		Asiakas* valittu = hakulista[valinta - 1];
		hakulista.clear();
		hakulista.push_back(valittu);

		tulos = ASIAKASHAKU_OK_YKSI;
	}
	return tulos;
}


Asiakas* asiakasHaku::NoudaAsiakas() {
	// Haetaan kirjaa k‰ytt‰j‰n muodostamien kriteerien perusteella.
	//Funktio palauttaa pointteriosoittajan kohti alkuper‰ist‰ kirjat-muuttujassa olevaa instanssia


	Asiakas* palauta = nullptr;


	cout << "----------------------------------------------------------------" << endl
		<< "					Asiakkaan haku									" << endl
		<< "----------------------------------------------------------------" << endl << endl
		<< "Anna hakukriteerit (Tyhja jos haluat ohittaa kriteerin, 0 jos haluat keskeyttaa haun):" << endl << endl;

	//Karsitaan haku-instanssissa olevaa kirjataulukkoa per kriteeri:
	//----------------------------------------------------------------

	for (int i = 0; i < 3; i++) {
		asiakasHaku::AsiakashaunTulos karsinnanTulos ;

		switch (i) {
		case 0:
			karsinnanTulos = MuodostaHakukriteerit(asiakasHaku::KENTTA_ID, "Asiakkaan ID-numero");
			break;
		case 1:
			karsinnanTulos = MuodostaHakukriteerit(asiakasHaku::KENTTA_HETU, "Asiakkaan henkilotunnus");
			break;
		case 2:
			karsinnanTulos = MuodostaHakukriteerit(asiakasHaku::KENTTA_SUKUNIMI, "Asiakkaan sukunimi");
			break;
		default:
			runtime_error("Asiakashaku:Hakukriteereja liian monta!");
		}

		switch (karsinnanTulos) {
		case asiakasHaku::ASIAKASHAKU_EI_ASIAKKAITA: // Haun tuloksena ei j‰‰nyt kirjoja, eli kriteerej‰ vastavia kirjoja ei ole. Siksi ei tarvitse en‰‰ lis‰‰ kriteerej‰.
			cout << "Hakukriteereja vastaavia asiakkaita ei loytynyt.";
			return palauta;

		case asiakasHaku::ASIAKASHAKU_LOPETA:	 // K‰ytt‰j‰ haluaa keskeytt‰‰...
			cout << "Kayttajakeskeytys...";
			return palauta;

		case asiakasHaku::ASIAKASHAKU_OK_YKSI:	// Yksi tulos lˆytyi, ei tarvitse enemp‰‰ kriteerej‰.
			return hakulista[0];
		}
	}

	// ---------------------------Kriteerit k‰yty l‰pi, tutkitaan tulokset ------------------------------
	if (hakulista.size() == 1) {
		// Yksi tulos lˆytyi. Palautamme sen:
		palauta = hakulista[0];
	}

	// Lˆytyi useampi kirja... Joudutaan kysym‰‰n p‰‰tˆst‰
	else if (PyydaValitsemaanYksi() == asiakasHaku::ASIAKASHAKU_OK_YKSI) {
		palauta = hakulista[0];
	}

	return palauta;

}




Asiakas* asiakasHaku::NoudaAsiakas(unsigned int kirjaID) {
	//Noutaa kirjan pointterin perustuen kirjan ID-tietoon:
	Asiakas* palauta = nullptr;

	for (unsigned int i = 0; i < hakulista.size(); i++) {
		if (hakulista[i]->ID == kirjaID) {
			palauta = hakulista[i];
			break;
		}
	}



	return palauta;
}