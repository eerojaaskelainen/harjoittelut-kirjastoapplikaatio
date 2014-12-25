#pragma once
class Asiakas
{
public:
	Asiakas();
	Asiakas(string uusiEtunimi, string uusiSukunimi, string uusiHetu, string uusiOsoite, int uusiID);
	~Asiakas();


public:
	bool lisaaLainaus(int kirjaID);
	void asetaLainakielto(bool asetus);
	void lisaaAsiakas(void);
	void naytaTiedot(void);

public:
	int asiakasID;
	string a_etunimi;
	string a_sukunimi;
	string a_hetu;
	string a_osoite;
	vector< int > lainat;
	bool lainakielto = false;

private:
	//int ai_asiakasID = 1;

	static int hae_ai_ID(void) {
		static int ai_ID = 1;  // Kirjan ID-numeron luontiin k‰ytett‰v‰, joka kerralla yhdell‰ lis‰‰ntyv‰ ID-numero
		return ai_ID++;
	}
};

