// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
// EJ: Lis�tty iostream
#include <iostream>

#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <cstring>	//EJ: Lis�tty stringin k�sittelykirjasto
#include <ctime>	// Asiakkaat: Lainan lis�ys
#include <fstream>
//#include <sstream>
#include <iomanip>

#include <future>
#include <thread>	// S�ikeidenk�sittely: Asiakkaiden ja kirjojen tiedostok�sittelyt.
using namespace std;

#define DEBUGI true	//EJ: Lis�tty definition DEBUGI: Voi k�ytt�� if-lauseena. Muuta true/false haluamasi mukaan.


#include "kayttoliittyma.h"  // EJ: Lis�tty k�ytt�liittym�� hoitava luokka

#include "Kirja.h"
#include "Henkilot.h"		//Korvaa Asiakas.h sek� Tyontekija.h -tiedostot
#include "Kirjasto.h"

// EJ: Lis�tty Tietojenhaku-luokka joka noutaa kirjat, asiakkaat ja ty�ntekij�t Kirjastoon:
#include "Tietojenhaku.h"

//EJ: Lis�tty Haku-luokka jossa kaikki hakumetorit
#include "Haku.h"


// Mainin funktiot:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
bool TulostaTyontekija(Kirjasto &laitoskirjasto, Kirjasto::Kayttajaprofiili &profiili);
bool TulostaAsiakas(Kirjasto &laitoskirjasto);
bool TulostaAnonyymi(Kirjasto &laitoskirjasto);
