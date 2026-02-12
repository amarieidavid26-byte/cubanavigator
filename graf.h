#ifndef GRAF_H
#define GRAF_H

#include "tipuri.h"
#include <vector>
#include <string>

using namespace std;

class Graf {
private:
    int nrOrase;                          // numarul total de orase
    vector<Oras> orase;                   // lista de orase
    vector<vector<Drum>> listaAdiacenta;  // lista de adiacenta

    // Returneaza viteza maxima pentru un tip de drum
    int getVitezaMaxima(const string& tipDrum) const;

    // Returneaza denumirea completa a tipului de drum
    string getNumeTipDrum(const string& tipDrum) const;

public:
    Graf();

    // Adauga un oras in graf
    void adaugaOras(const string& nume, double lat, double lon);

    // Adauga un drum bidirectional intre doua orase
    void adaugaDrum(int oras1, int oras2, double distanta, const string& tipDrum);

    // Initializeaza graful cu orasele si drumurile Cubei
    void initializeazaCuba();

    // Afiseaza lista de orase
    void afiseazaOrase() const;

    // --- Algoritm Backtracking ---

    // Genereaza toate caile posibile intre 2 noduri fara cicluri
    void backtracking(int nodCurent, int destinatie,
                      vector<bool>& vizitat,
                      Traseu& traseulCurent,
                      vector<Traseu>& toateTraseele);

    // Returneaza traseul cu distanta minima intre sursa si destinatie
    Traseu gasesteTraseuMinim(int sursa, int destinatie);

    // Returneaza toate traseele posibile intre sursa si destinatie
    vector<Traseu> gasesteToateTraseele(int sursa, int destinatie);

    // Calculeaza timpul de parcurgere in ore pentru un traseu dat
    double calculeazaTimp(const Traseu& traseu) const;

    // Afiseaza un traseu formatat frumos
    void afiseazaTraseu(const Traseu& traseu) const;

    // Afiseaza detaliile segmentelor unui traseu (tipuri de drum)
    void afiseazaDetaliiTraseu(const Traseu& traseu) const;

    // Returneaza numele unui oras dupa index
    string getNumeOras(int index) const;

    // Returneaza numarul de orase
    int getNrOrase() const;

    // Returneaza numarul de conexiuni directe ale unui oras
    int getNrConexiuni(int index) const;

    // Genereaza fisierul HTML cu harta interactiva a traseelor
    // traseuEvidentiiat: -1 = mod trasee multiple, >=0 = index traseu unic evidentiiat
    void genereazaHTML(const vector<Traseu>& trasee, int sursa, int destinatie,
                       const string& titlu, int traseuEvidentiiat = -1);
};

#endif // GRAF_H
