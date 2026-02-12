#ifndef TIPURI_H
#define TIPURI_H

#include <string>
#include <vector>

using namespace std;

// Reprezinta un drum (muchie) catre un oras vecin
struct Drum {
    int destinatie;      // indexul orasului destinatie
    double distanta;     // distanta in km
    string tipDrum;      // "A" (autopista), "CN" (carretera nacional), "CR" (carretera regional), "CL" (carretera local)
    int vitezaMax;       // viteza maxima permisa in km/h
};

// Reprezinta un oras (nod) cu coordonate GPS reale
struct Oras {
    string nume;
    double latitudine;
    double longitudine;
};

// Reprezinta un traseu complet intre doua orase
struct Traseu {
    vector<int> noduri;        // secventa de orase (indecsi)
    double distantaTotala;     // distanta totala in km
    double timpTotal;          // timpul total in ore
    vector<string> tipuriDrum; // tipul fiecarui segment de drum
};

#endif // TIPURI_H
