#pragma once
class asiakasHaku
{
public:
	enum AsiakashaunTulos{
		ASIAKASHAKU_OK_MONTA,			//Asiakkaita l�ytyi
		ASIAKASHAKU_OK_YKSI,			// Yksi asiakas l�ytyi	
		ASIAKASHAKU_EI_ASIAKKAITA,		// Asiakkaita ei ole
		ASIAKASHAKU_LOPETA,			//K�ytt�j� haluaa lopettaa
		ASIAKASHAKU_VAARA_HAKU		// Annettu tuntematon hakukriteeri				
	};

	enum AsiakkaanKentat {				//Asiakkaan tiedoista haettavia kentti�
		KENTTA_ID,
		KENTTA_SUKUNIMI,
		KENTTA_HETU
	};
	vector<Asiakas*> hakulista;


public:
	asiakasHaku(vector<Asiakas>&);
	~asiakasHaku();
	AsiakashaunTulos MuodostaHakukriteerit(AsiakkaanKentat kriteeri, string kysymysKayttajalle);
	AsiakashaunTulos PyydaValitsemaanYksi();
	Asiakas* NoudaAsiakas();
	Asiakas* NoudaAsiakas(unsigned int asiakasID);


};

