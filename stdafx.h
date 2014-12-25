// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
// EJ: Lisätty iostream
#include <iostream>

#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <cstring>	//EJ: Lisätty stringin käsittelykirjasto
#include <ctime>	// Asiakkaat: Lainan lisäys
#include <fstream>
//#include <sstream>
#include <iomanip>

#include <future>
#include <thread>	// Säikeidenkäsittely: Asiakkaiden ja kirjojen tiedostokäsittelyt.
using namespace std;

#define DEBUGI true	//EJ: Lisätty definition DEBUGI: Voi käyttää if-lauseena. Muuta true/false haluamasi mukaan.


#include "kayttoliittyma.h"  // EJ: Lisätty käyttöliittymää hoitava luokka

#include "Kirja.h"
#include "Henkilot.h"		//Korvaa Asiakas.h sekä Tyontekija.h -tiedostot
#include "Kirjasto.h"

// EJ: Lisätty Tietojenhaku-luokka joka noutaa kirjat, asiakkaat ja työntekijät Kirjastoon:
#include "Tietojenhaku.h"

//EJ: Lisätty Haku-luokka jossa kaikki hakumetorit
#include "Haku.h"


// Mainin funktiot:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
bool TulostaTyontekija(Kirjasto &laitoskirjasto, Kirjasto::Kayttajaprofiili &profiili);
bool TulostaAsiakas(Kirjasto &laitoskirjasto);
bool TulostaAnonyymi(Kirjasto &laitoskirjasto);
