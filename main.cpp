// Cuba Navigator v2.0 — Backtracking Iterativ
// Stil de cod: structura BKT din clasa (INIT/EXISTA/VALID/SOLUTIE/TIPAR)
// Toate datele in variabile globale, fara clase, fara STL containers

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <limits>
#include <chrono>
#include <cstdlib>

using namespace std;

// ╔════════════════════════════════════════╗
// ║         CODURI CULORI ANSI            ║
// ╚════════════════════════════════════════╝
const string RESET  = "\033[0m";
const string BOLD   = "\033[1m";
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN   = "\033[36m";
const string GRAY   = "\033[90m";

// ╔════════════════════════════════════════╗
// ║         DATE GLOBALE                  ║
// ╚════════════════════════════════════════╝
const int NR_ORASE = 15;

// Date orase
string numeOrase[NR_ORASE];
double latitudine[NR_ORASE];
double longitudine[NR_ORASE];

// Matricea de adiacenta
int distanta[NR_ORASE][NR_ORASE];
string tipDrum[NR_ORASE][NR_ORASE];
int vitezaMax[NR_ORASE][NR_ORASE];

// Vectorul solutie BKT
int x[100];
int vizitat[NR_ORASE];

// Sursa si destinatia (globale, folosite de functiile BKT)
int sursa, destinatia;

// Stocarea tuturor traseelor gasite
int trasee[500][20];
int lungimeTraseu[500];
double distanteTrasee[500];
double timpTrasee[500];
int nrTrasee;

// ╔════════════════════════════════════════╗
// ║      INITIALIZARE DATE CUBA           ║
// ╚════════════════════════════════════════╝

void adaugaDrum(int o1, int o2, int dist, string tip, int viteza) {
    distanta[o1][o2] = dist;    distanta[o2][o1] = dist;
    tipDrum[o1][o2] = tip;      tipDrum[o2][o1] = tip;
    vitezaMax[o1][o2] = viteza; vitezaMax[o2][o1] = viteza;
}

void initializeazaDate() {
    // Resetare matrice
    for (int i = 0; i < NR_ORASE; i++)
        for (int j = 0; j < NR_ORASE; j++) {
            distanta[i][j] = 0;
            tipDrum[i][j] = "";
            vitezaMax[i][j] = 0;
        }

    // Orase cu coordonate GPS reale
    numeOrase[0]  = "Havana";           latitudine[0]  = 23.1136; longitudine[0]  = -82.3666;
    numeOrase[1]  = "Varadero";         latitudine[1]  = 23.1394; longitudine[1]  = -81.2863;
    numeOrase[2]  = "Matanzas";         latitudine[2]  = 23.0510; longitudine[2]  = -81.5775;
    numeOrase[3]  = "Santa Clara";      latitudine[3]  = 22.4065; longitudine[3]  = -79.9553;
    numeOrase[4]  = "Cienfuegos";       latitudine[4]  = 22.1456; longitudine[4]  = -80.4364;
    numeOrase[5]  = "Trinidad";         latitudine[5]  = 21.8027; longitudine[5]  = -79.9843;
    numeOrase[6]  = "Sancti Spiritus";  latitudine[6]  = 21.9304; longitudine[6]  = -79.4425;
    numeOrase[7]  = "Ciego de Avila";   latitudine[7]  = 21.8405; longitudine[7]  = -78.7621;
    numeOrase[8]  = "Camaguey";         latitudine[8]  = 21.3891; longitudine[8]  = -77.9160;
    numeOrase[9]  = "Holguin";          latitudine[9]  = 20.8872; longitudine[9]  = -76.2630;
    numeOrase[10] = "Santiago de Cuba";  latitudine[10] = 20.0247; longitudine[10] = -75.8219;
    numeOrase[11] = "Bayamo";           latitudine[11] = 20.3792; longitudine[11] = -76.6434;
    numeOrase[12] = "Guantanamo";       latitudine[12] = 20.1449; longitudine[12] = -75.2091;
    numeOrase[13] = "Pinar del Rio";    latitudine[13] = 22.4175; longitudine[13] = -83.6978;
    numeOrase[14] = "Las Tunas";        latitudine[14] = 20.9601; longitudine[14] = -76.9542;

    // Drumuri bidirectionale (20 drumuri)
    adaugaDrum(0,  2,  102, "A",  100);   // Havana - Matanzas
    adaugaDrum(0,  13, 162, "CN", 80);    // Havana - Pinar del Rio
    adaugaDrum(2,  1,  42,  "CR", 60);    // Matanzas - Varadero
    adaugaDrum(2,  3,  195, "A",  100);   // Matanzas - Santa Clara
    adaugaDrum(3,  4,  84,  "CN", 80);    // Santa Clara - Cienfuegos
    adaugaDrum(4,  5,  80,  "CR", 60);    // Cienfuegos - Trinidad
    adaugaDrum(5,  6,  74,  "CR", 60);    // Trinidad - Sancti Spiritus
    adaugaDrum(3,  6,  89,  "CN", 80);    // Santa Clara - Sancti Spiritus
    adaugaDrum(6,  7,  110, "CN", 80);    // Sancti Spiritus - Ciego de Avila
    adaugaDrum(7,  8,  110, "A",  100);   // Ciego de Avila - Camaguey
    adaugaDrum(8,  14, 120, "CN", 80);    // Camaguey - Las Tunas
    adaugaDrum(14, 9,  76,  "CN", 80);    // Las Tunas - Holguin
    adaugaDrum(14, 11, 92,  "CN", 80);    // Las Tunas - Bayamo
    adaugaDrum(11, 10, 127, "CN", 80);    // Bayamo - Santiago de Cuba
    adaugaDrum(10, 12, 86,  "CR", 60);    // Santiago de Cuba - Guantanamo
    adaugaDrum(9,  10, 135, "CN", 80);    // Holguin - Santiago de Cuba
    adaugaDrum(11, 9,  72,  "CR", 60);    // Bayamo - Holguin
    adaugaDrum(2,  4,  157, "CR", 60);    // Matanzas - Cienfuegos
    adaugaDrum(3,  7,  188, "A",  100);   // Santa Clara - Ciego de Avila
    adaugaDrum(8,  9,  206, "CN", 80);    // Camaguey - Holguin
}

// ╔════════════════════════════════════════╗
// ║      ALGORITM BACKTRACKING            ║
// ║   INIT / EXISTA / VALID / SOLUTIE /   ║
// ║          TIPAR / BKT                  ║
// ╚════════════════════════════════════════╝

void INIT(int k) {
    x[k] = -1;
}

int EXISTA(int k) {
    if (x[k] < NR_ORASE - 1) return 1;
    return 0;
}

int VALID(int k) {
    // Orasul precedent: sursa daca k==1, altfel x[k-1]
    int orasPrecedent;
    if (k == 1) orasPrecedent = sursa;
    else orasPrecedent = x[k - 1];

    // Verificare 1: exista drum intre orasul precedent si orasul curent
    if (distanta[orasPrecedent][x[k]] == 0) return 0;

    // Verificare 2: orasul nu este deja vizitat
    if (vizitat[x[k]] == 1) return 0;

    // Verificare 3: orasul nu este sursa
    if (x[k] == sursa) return 0;

    return 1;
}

int SOLUTIE(int k) {
    if (x[k] == destinatia) return 1;
    return 0;
}

void TIPAR(int k) {
    if (nrTrasee >= 500) return;

    // Salvam traseul: sursa + x[1]..x[k]
    trasee[nrTrasee][0] = sursa;
    for (int i = 1; i <= k; i++)
        trasee[nrTrasee][i] = x[i];
    lungimeTraseu[nrTrasee] = k + 1;

    // Calculam distanta totala
    double d = 0;
    for (int i = 0; i < k; i++) {
        int c1 = trasee[nrTrasee][i];
        int c2 = trasee[nrTrasee][i + 1];
        d += distanta[c1][c2];
    }
    distanteTrasee[nrTrasee] = d;

    // Calculam timpul total
    double t = 0;
    for (int i = 0; i < k; i++) {
        int c1 = trasee[nrTrasee][i];
        int c2 = trasee[nrTrasee][i + 1];
        t += (double)distanta[c1][c2] / vitezaMax[c1][c2];
    }
    timpTrasee[nrTrasee] = t;

    nrTrasee++;
}

void BKT() {
    int k = 1;
    INIT(k);
    nrTrasee = 0;

    // Resetare vector vizitat, marcam sursa ca vizitata
    for (int i = 0; i < NR_ORASE; i++) vizitat[i] = 0;
    vizitat[sursa] = 1;

    while (k > 0) {
        if (EXISTA(k)) {
            x[k] = x[k] + 1;
            if (VALID(k)) {
                vizitat[x[k]] = 1;
                if (SOLUTIE(k)) {
                    TIPAR(k);
                    vizitat[x[k]] = 0;
                }
                else {
                    k = k + 1;
                    INIT(k);
                }
            }
        }
        else {
            k--;
            if (k >= 1) {
                vizitat[x[k]] = 0;
            }
        }
    }
}

// ╔════════════════════════════════════════╗
// ║        FUNCTII AJUTATOARE             ║
// ╚════════════════════════════════════════╝

// Sorteaza traseele dupa distanta (selection sort)
void sortTrasee() {
    for (int i = 0; i < nrTrasee - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < nrTrasee; j++)
            if (distanteTrasee[j] < distanteTrasee[minIdx])
                minIdx = j;
        if (minIdx != i) {
            // Swap distante
            double tmpD = distanteTrasee[i];
            distanteTrasee[i] = distanteTrasee[minIdx];
            distanteTrasee[minIdx] = tmpD;
            // Swap timpuri
            double tmpT = timpTrasee[i];
            timpTrasee[i] = timpTrasee[minIdx];
            timpTrasee[minIdx] = tmpT;
            // Swap lungimi
            int tmpL = lungimeTraseu[i];
            lungimeTraseu[i] = lungimeTraseu[minIdx];
            lungimeTraseu[minIdx] = tmpL;
            // Swap vectorii de trasee
            for (int r = 0; r < 20; r++) {
                int tmpR = trasee[i][r];
                trasee[i][r] = trasee[minIdx][r];
                trasee[minIdx][r] = tmpR;
            }
        }
    }
}

// Returneaza numarul de conexiuni directe ale unui oras
int getNrConexiuni(int oras) {
    int cnt = 0;
    for (int j = 0; j < NR_ORASE; j++)
        if (distanta[oras][j] > 0) cnt++;
    return cnt;
}

// Returneaza denumirea completa a tipului de drum
string getNumeTipDrum(string tip) {
    if (tip == "A")  return "Autopista Nacional";
    if (tip == "CN") return "Carretera Nacional";
    if (tip == "CR") return "Carretera Regional";
    if (tip == "CL") return "Carretera Local";
    return "Necunoscut";
}

// Formatare numar cu precizie (pentru HTML)
string fmt(double v, int prec) {
    ostringstream oss;
    oss << fixed << setprecision(prec) << v;
    return oss.str();
}

// Formateaza timpul din ore in "X ore si Y minute"
string formateazaTimp(double ore) {
    int h = (int)ore;
    int m = (int)((ore - h) * 60 + 0.5);
    if (m == 60) { h++; m = 0; }
    string r;
    if (h > 0) r += to_string(h) + (h == 1 ? " ora" : " ore");
    if (m > 0) {
        if (!r.empty()) r += " si ";
        r += to_string(m) + (m == 1 ? " minut" : " minute");
    }
    if (r.empty()) r = "sub 1 minut";
    return r;
}

// Returneaza culoarea HTML pentru un tip de drum
string culoareDrum(string tip) {
    if (tip == "A")  return "#e74c3c";
    if (tip == "CN") return "#3498db";
    if (tip == "CR") return "#f39c12";
    if (tip == "CL") return "#95a5a6";
    return "#ffffff";
}

// Returneaza grosimea liniei SVG pentru un tip de drum
string grosimeDrum(string tip) {
    if (tip == "A")  return "4";
    if (tip == "CN") return "3";
    if (tip == "CR") return "3";
    if (tip == "CL") return "2";
    return "2";
}

// ╔════════════════════════════════════════╗
// ║        FUNCTII DE AFISARE             ║
// ╚════════════════════════════════════════╝

void afiseazaBanner() {
    cout << endl;
    cout << CYAN << BOLD;
    cout << R"(  ╔══════════════════════════════════════════════════╗)" << endl;
    cout << R"(  ║                                                  ║)" << endl;
    cout << R"(  ║        CUBA NAVIGATOR - GPS Routing              ║)" << endl;
    cout << R"(  ║        Sistem de Navigare Inteligent              ║)" << endl;
    cout << R"(  ║                                                  ║)" << endl;
    cout << R"(  ╚══════════════════════════════════════════════════╝)" << endl;
    cout << RESET;
}

void afiseazaMeniu() {
    cout << endl;
    cout << GRAY << "  ┌─────────────────────────────────────────┐" << RESET << endl;
    cout << YELLOW << "  │  1. Distanta minima intre 2 localitati  │" << RESET << endl;
    cout << YELLOW << "  │  2. Toate traseele posibile             │" << RESET << endl;
    cout << YELLOW << "  │  3. Tipurile de drum pe un traseu       │" << RESET << endl;
    cout << YELLOW << "  │  4. Timpul de parcurgere                │" << RESET << endl;
    cout << RED    << "  │  5. Iesire din aplicatie                │" << RESET << endl;
    cout << GRAY << "  └─────────────────────────────────────────┘" << RESET << endl;
    cout << endl;
}

void separator() {
    cout << GRAY << "  ──────────────────────────────────────────────────" << RESET << endl;
}

void afiseazaOrase() {
    cout << "\n";
    cout << GRAY << "  +-----+------------------------+-------------------+" << RESET << endl;
    cout << GRAY << "  | Nr. | Oras                   | Coordonate GPS    |" << RESET << endl;
    cout << GRAY << "  +-----+------------------------+-------------------+" << RESET << endl;
    for (int i = 0; i < NR_ORASE; i++) {
        cout << GRAY << "  | " << RESET << setw(3) << (i + 1)
             << GRAY << " | " << RESET
             << GREEN << setw(22) << left << numeOrase[i] << RESET
             << GRAY << " | " << RESET
             << right << fixed << setprecision(4)
             << setw(7) << latitudine[i] << ", "
             << setw(8) << longitudine[i]
             << GRAY << " |" << RESET << endl;
    }
    cout << GRAY << "  +-----+------------------------+-------------------+" << RESET << endl;
}

int citesteOras(string mesaj) {
    int oras;
    while (true) {
        cout << "  " << mesaj;
        cin >> oras;
        if (cin.fail() || oras < 1 || oras > NR_ORASE) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "  [!] Numar invalid! Introduceti un numar intre 1 si " << NR_ORASE << "." << RESET << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return oras - 1;
        }
    }
}

void afiseazaTraseu(int traseIndex) {
    cout << "  ";
    for (int i = 0; i < lungimeTraseu[traseIndex]; i++) {
        cout << numeOrase[trasee[traseIndex][i]];
        if (i < lungimeTraseu[traseIndex] - 1) cout << " -> ";
    }
    cout << " (" << fixed << setprecision(0) << distanteTrasee[traseIndex] << " km)" << endl;
}

void afiseazaDetaliiTraseu(int traseIndex) {
    int nrSeg = lungimeTraseu[traseIndex] - 1;
    if (nrSeg < 1) {
        cout << "  Traseul nu are segmente!" << endl;
        return;
    }

    cout << endl;
    cout << GRAY;
    cout << "  +----+------------------------+------------------------+---------+---------------------------+-----------+" << endl;
    cout << "  | Nr | De la                  | La                     | Dist.   | Tip drum                  | Viteza    |" << endl;
    cout << "  +----+------------------------+------------------------+---------+---------------------------+-----------+" << endl;
    cout << RESET;

    for (int i = 0; i < nrSeg; i++) {
        int c1 = trasee[traseIndex][i];
        int c2 = trasee[traseIndex][i + 1];
        int dist = distanta[c1][c2];
        int viteza = vitezaMax[c1][c2];
        string tip = tipDrum[c1][c2];
        string tipComplet = tip + " - " + getNumeTipDrum(tip);

        cout << "  " << GRAY << "|" << RESET << " " << setw(2) << (i + 1)
             << " " << GRAY << "|" << RESET << " "
             << GREEN << setw(22) << left << numeOrase[c1] << RESET
             << " " << GRAY << "|" << RESET << " "
             << GREEN << setw(22) << left << numeOrase[c2] << RESET
             << " " << GRAY << "|" << RESET
             << right << " " << CYAN << BOLD << setw(5) << dist << " km" << RESET
             << " " << GRAY << "|" << RESET << " "
             << setw(25) << left << tipComplet
             << " " << GRAY << "|" << RESET
             << right << " " << setw(6) << viteza << " km/h"
             << " " << GRAY << "|" << RESET << endl;
    }

    cout << GRAY;
    cout << "  +----+------------------------+------------------------+---------+---------------------------+-----------+" << endl;
    cout << RESET;
    cout << "  " << GRAY << "|" << RESET << " Total: " << CYAN << BOLD
         << setw(92) << right << to_string((int)distanteTrasee[traseIndex]) + " km" << RESET
         << " " << GRAY << "|" << RESET << endl;
    cout << GRAY;
    cout << "  +----+------------------------+------------------------+---------+---------------------------+-----------+" << endl;
    cout << RESET;
}

void deschideHTML() {
    cout << GREEN << BOLD << "  [OK] Harta a fost generata! Deschide fisierul: rezultat.html" << RESET << endl;
    cout << CYAN << "  Se deschide harta in browser..." << RESET << endl;
#ifdef __APPLE__
    system("open rezultat.html 2>/dev/null");
#elif __linux__
    system("xdg-open rezultat.html 2>/dev/null");
#elif _WIN32
    system("start rezultat.html");
#endif
}

// ╔══════════════════════════════════════════════════════════════════════╗
// ║   GENERARE HTML — Harta Leaflet.js + OpenStreetMap a traseelor    ║
// ║   traseuEvidentiiat: -1 = trasee multiple, >=0 = traseu unic      ║
// ╚══════════════════════════════════════════════════════════════════════╝
void genereazaHTML(string titlu, int traseuEvidentiiat) {
    ofstream fout("rezultat.html");
    if (!fout.is_open()) {
        cerr << "  [!] Nu s-a putut crea fisierul rezultat.html!" << endl;
        return;
    }

    bool modMultiplu = (traseuEvidentiiat == -1);

    // Culori pentru trasee multiple
    string culoriMultiple[5] = {"#e74c3c","#3498db","#2ecc71","#f39c12","#9b59b6"};

    // ══════════════════════════════════
    //  SECTIUNEA 1: HTML HEAD + CSS
    // ══════════════════════════════════
    fout << R"html(<!DOCTYPE html>
<html lang="ro">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Cuba Navigator - Rezultat</title>
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{
    font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;
    background:linear-gradient(135deg,#1a1a2e 0%,#16213e 50%,#1a1a2e 100%);
    color:#e0e0e0;min-height:100vh;
}
.container{max-width:1300px;margin:0 auto;padding:20px}

@keyframes fadeDown{from{opacity:0;transform:translateY(-20px)}to{opacity:1;transform:translateY(0)}}
@keyframes fadeUp{from{opacity:0;transform:translateY(20px)}to{opacity:1;transform:translateY(0)}}
@keyframes pulse{0%,100%{transform:scale(1);opacity:1}50%{transform:scale(1.8);opacity:0.3}}

header{text-align:center;padding:30px 0 20px;animation:fadeDown 0.6s ease-out}
header h1{
    font-size:2.4em;
    background:linear-gradient(135deg,#0ea5e9,#38bdf8,#7dd3fc);
    -webkit-background-clip:text;-webkit-text-fill-color:transparent;
    background-clip:text;margin-bottom:6px;letter-spacing:1px;
}
header p{font-size:1.05em;color:#64748b;letter-spacing:2px;text-transform:uppercase;font-weight:300}

.map-card{
    background:#16213e;border-radius:14px;
    box-shadow:0 8px 40px rgba(0,0,0,0.4),0 0 0 1px rgba(14,165,233,0.1);
    padding:20px;margin-bottom:24px;position:relative;overflow:hidden;
    animation:fadeDown 0.8s ease-out;
}
#map{width:100%;height:500px;border-radius:12px;border:2px solid #2a3a5c;z-index:0}

/* Leaflet tooltip and popup dark theme */
.leaflet-tooltip{background:rgba(22,33,62,0.92);color:#e2e8f0;border:1px solid #2a3a5c;font-size:11px;font-weight:bold;font-family:'Segoe UI',sans-serif;padding:3px 8px;border-radius:4px;box-shadow:0 2px 8px rgba(0,0,0,0.4)}
.leaflet-tooltip-top:before{border-top-color:rgba(22,33,62,0.92)}
.leaflet-tooltip-bottom:before{border-bottom-color:rgba(22,33,62,0.92)}
.leaflet-tooltip-left:before{border-left-color:rgba(22,33,62,0.92)}
.leaflet-tooltip-right:before{border-right-color:rgba(22,33,62,0.92)}
.leaflet-popup-content-wrapper{background:#16213e;color:#e0e0e0;border-radius:10px;box-shadow:0 4px 20px rgba(0,0,0,0.5)}
.leaflet-popup-content{margin:10px 14px;font-size:13px;line-height:1.5}
.leaflet-popup-tip{background:#16213e}
.leaflet-control-layers{background:rgba(22,33,62,0.92) !important;color:#e0e0e0 !important;border:1px solid #2a3a5c !important;border-radius:10px !important;box-shadow:0 4px 20px rgba(0,0,0,0.4) !important;padding:8px 12px !important;font-family:'Segoe UI',sans-serif !important}
.leaflet-control-layers-base label,.leaflet-control-layers-overlays label{color:#e0e0e0 !important;margin-bottom:4px !important;font-size:13px}
.leaflet-control-layers-separator{border-top-color:#2a3a5c !important}
.leaflet-control-zoom a{background:#16213e !important;color:#0ea5e9 !important;border-color:#2a3a5c !important}
.leaflet-control-zoom a:hover{background:#1e3a5f !important}
.leaflet-control-scale-line{background:rgba(22,33,62,0.85) !important;border-color:#0ea5e9 !important;color:#e0e0e0 !important;font-size:11px !important;padding:2px 8px !important}

/* Pulsating markers */
.pulse-marker{border-radius:50%;animation:pulse 2.5s ease-in-out infinite}

/* Stats panel */
.stats-row{
    display:grid;grid-template-columns:repeat(4,1fr);gap:16px;
    margin-bottom:24px;animation:fadeUp 0.9s ease-out;
}
.stat-card{
    background:#16213e;border-radius:12px;padding:20px 18px;text-align:center;
    box-shadow:0 4px 20px rgba(0,0,0,0.3),0 0 0 1px rgba(14,165,233,0.08);
    transition:transform 0.2s;
}
.stat-card:hover{transform:translateY(-3px)}
.stat-icon{font-size:1.6em;margin-bottom:6px;display:block}
.stat-value{font-size:1.5em;font-weight:800;color:#0ea5e9;display:block;margin-bottom:2px}
.stat-label{font-size:0.78em;color:#64748b;text-transform:uppercase;letter-spacing:1px}

.info-card{
    background:#16213e;border-radius:14px;
    box-shadow:0 8px 40px rgba(0,0,0,0.4),0 0 0 1px rgba(14,165,233,0.1);
    padding:30px 35px;margin-bottom:24px;
    animation:fadeUp 1s ease-out 0.3s both;
}
.info-card h2{color:#0ea5e9;margin-bottom:20px;font-size:1.45em;border-bottom:2px solid #1e3a5f;padding-bottom:14px}
.route-summary{
    background:#1a1a2e;border-radius:10px;padding:16px 22px;margin-bottom:18px;
    border-left:4px solid #0ea5e9;font-size:1.08em;line-height:1.7;
}
.route-summary .dist{color:#0ea5e9;font-weight:700}
.route-summary .timp{color:#38bdf8;font-size:0.92em}
table{width:100%;border-collapse:collapse;margin:18px 0}
th{background:#1a1a2e;color:#0ea5e9;padding:12px 16px;text-align:left;font-weight:600;text-transform:uppercase;font-size:0.78em;letter-spacing:1.2px}
td{padding:11px 16px;border-bottom:1px solid #1e3a5f;font-size:0.95em}
tr:hover td{background:rgba(14,165,233,0.04)}
.road-badge{display:inline-block;padding:3px 12px;border-radius:14px;font-size:0.82em;font-weight:700;color:#fff;margin-right:6px}
.total-row{background:#1a1a2e;font-weight:700;font-size:1.05em}
.total-row td{padding:15px 16px;color:#0ea5e9;border-bottom:none}

.route-list{margin-top:16px}
.route-item{
    background:#1a1a2e;border-radius:10px;padding:13px 20px;margin-bottom:8px;
    display:flex;align-items:center;border-left:4px solid #2c3e50;
    transition:transform 0.15s,box-shadow 0.15s;
}
.route-item:hover{transform:translateX(4px);box-shadow:0 2px 12px rgba(0,0,0,0.2)}
.route-num{
    background:#0ea5e9;color:#1a1a2e;width:30px;height:30px;border-radius:50%;
    display:inline-flex;align-items:center;justify-content:center;
    font-weight:800;font-size:0.85em;margin-right:14px;flex-shrink:0;
}
.route-path{flex:1;font-size:0.93em;color:#cbd5e1}
.route-path .arrow{color:#475569;margin:0 4px}
.route-dist{color:#0ea5e9;font-weight:700;white-space:nowrap;margin-left:14px;font-size:0.95em}
.route-more{text-align:center;color:#64748b;font-style:italic;padding:12px;font-size:0.9em}

footer{
    text-align:center;padding:28px 0;color:#475569;font-size:0.82em;
    border-top:1px solid #1e3a5f;margin-top:8px;letter-spacing:0.3px;
}
footer span{color:#0ea5e9}
footer .sep{margin:0 8px;color:#2c3e50}

@media(max-width:768px){
    .stats-row{grid-template-columns:repeat(2,1fr)}
    #map{height:350px}
}
</style>
</head>
<body>
<div class="container">
)html";

    // ══════════════════════════════════
    //  SECTIUNEA 2: HEADER
    // ══════════════════════════════════
    fout << "<header>\n";
    fout << "<h1>&#127464;&#127482; Cuba Navigator</h1>\n";
    fout << "<p>Sistem de Navigare Inteligent</p>\n";
    fout << "</header>\n\n";

    // ══════════════════════════════════
    //  SECTIUNEA 3: HARTA LEAFLET
    // ══════════════════════════════════
    fout << "<div class=\"map-card\">\n";
    fout << "<div id=\"map\"></div>\n";
    fout << "</div>\n\n";

    // Leaflet JavaScript — map init, markers, roads, routes
    fout << "<script>\n";

    // Map setup with dual tile layers (dark + satellite)
    fout << "var map=L.map('map',{center:[21.5,-79.5],zoom:7,zoomControl:true,scrollWheelZoom:true});\n";
    fout << "var darkLayer=L.tileLayer('https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png',{"
         << "attribution:'\\u00a9 OpenStreetMap contributors \\u00a9 CARTO',maxZoom:19});\n";
    fout << "var satelliteLayer=L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}',{"
         << "attribution:'\\u00a9 Esri, Maxar, Earthstar Geographics',maxZoom:19});\n";
    fout << "darkLayer.addTo(map);\n";
    fout << "L.control.layers({'\\ud83c\\udf19 Dark':darkLayer,'\\ud83d\\udef0\\ufe0f Satelit':satelliteLayer},null,"
         << "{position:'topleft',collapsed:false}).addTo(map);\n";
    fout << "L.control.scale({metric:true,imperial:false,position:'bottomleft',maxWidth:150}).addTo(map);\n\n";

    // City coordinates array
    fout << "var cities=[\n";
    for (int i = 0; i < NR_ORASE; i++) {
        fout << "  {name:'" << numeOrase[i] << "',lat:" << fmt(latitudine[i], 4)
             << ",lng:" << fmt(longitudine[i], 4)
             << ",conn:" << getNrConexiuni(i) << "}";
        if (i < NR_ORASE - 1) fout << ",";
        fout << "\n";
    }
    fout << "];\n";
    fout << "var src=" << sursa << ",dst=" << destinatia << ";\n\n";

    // Tooltip offset directions to avoid overlap
    fout << "var tDir=[[-1,-1],[1,-1],[1,1],[1,0],[-1,0],[-1,1],[1,-1],[1,-1],[1,-1],[1,-1],[-1,1],[-1,0],[1,0],[-1,-1],[1,1]];\n\n";

    // Background roads (all 20)
    fout << "// Background roads\n";
    for (int i = 0; i < NR_ORASE; i++)
        for (int j = i + 1; j < NR_ORASE; j++)
            if (distanta[i][j] > 0) {
                fout << "L.polyline([[" << fmt(latitudine[i], 4) << "," << fmt(longitudine[i], 4)
                     << "],[" << fmt(latitudine[j], 4) << "," << fmt(longitudine[j], 4)
                     << "]],{color:'rgba(255,255,255,0.15)',weight:2,dashArray:'5,10'}).addTo(map);\n";
            }
    fout << "\n";

    // Route drawing
    if (modMultiplu) {
        // Multiple routes mode — up to 5 on map with layer control
        int nrPeHarta = (nrTrasee < 5) ? nrTrasee : 5;
        fout << "var overlays={};\n";
        for (int r = 0; r < nrPeHarta; r++) {
            string culoare = culoriMultiple[r % 5];
            string opac = (r == 0) ? "1.0" : "0.4";
            int weight = (r == 0) ? 5 : 3;
            string dash = (r == 0) ? "" : (r == 1 ? "10,8" : r == 2 ? "15,5,5,5" : r == 3 ? "5,5" : "20,10");
            fout << "var rg" << r << "=L.layerGroup();\n";
            int nrSeg = lungimeTraseu[r] - 1;
            for (int i = 0; i < nrSeg; i++) {
                int c1 = trasee[r][i];
                int c2 = trasee[r][i + 1];
                fout << "L.polyline([[" << fmt(latitudine[c1], 4) << "," << fmt(longitudine[c1], 4)
                     << "],[" << fmt(latitudine[c2], 4) << "," << fmt(longitudine[c2], 4)
                     << "]],{color:'" << culoare << "',weight:" << weight
                     << ",opacity:" << opac;
                if (!dash.empty()) fout << ",dashArray:'" << dash << "'";
                fout << "}).bindPopup('<b>" << numeOrase[c1] << " \\u2192 " << numeOrase[c2]
                     << "</b><br>" << distanta[c1][c2] << " km | " << getNumeTipDrum(tipDrum[c1][c2])
                     << " | " << vitezaMax[c1][c2] << " km/h').addTo(rg" << r << ");\n";
            }
            fout << "rg" << r << ".addTo(map);\n";
            fout << "overlays['Traseu " << (r + 1) << " (" << fmt(distanteTrasee[r], 0) << " km)']=rg" << r << ";\n";
        }
        fout << "L.control.layers(null,overlays,{collapsed:false,position:'bottomright',sortLayers:false}).addTo(map);\n\n";
    } else if (traseuEvidentiiat >= 0 && traseuEvidentiiat < nrTrasee) {
        // Single route mode — colored by road type with segment popups
        int idx = traseuEvidentiiat;
        int nrSeg = lungimeTraseu[idx] - 1;
        for (int i = 0; i < nrSeg; i++) {
            int c1 = trasee[idx][i];
            int c2 = trasee[idx][i + 1];
            string tip = tipDrum[c1][c2];
            string col = culoareDrum(tip);
            int w = (tip == "A") ? 5 : (tip == "CL") ? 3 : 4;
            fout << "L.polyline([[" << fmt(latitudine[c1], 4) << "," << fmt(longitudine[c1], 4)
                 << "],[" << fmt(latitudine[c2], 4) << "," << fmt(longitudine[c2], 4)
                 << "]],{color:'" << col << "',weight:" << w
                 << ",opacity:1}).bindPopup('<b>" << numeOrase[c1] << " \\u2192 " << numeOrase[c2]
                 << "</b><br>" << distanta[c1][c2] << " km | " << getNumeTipDrum(tip)
                 << " | " << vitezaMax[c1][c2] << " km/h').addTo(map);\n";
        }

        // Direction arrows along the route
        fout << "var routeCoords=[";
        for (int i = 0; i < lungimeTraseu[idx]; i++) {
            int c = trasee[idx][i];
            fout << "[" << fmt(latitudine[c], 4) << "," << fmt(longitudine[c], 4) << "]";
            if (i < lungimeTraseu[idx] - 1) fout << ",";
        }
        fout << "];\n";
        fout << "for(var i=0;i<routeCoords.length-1;i++){"
             << "var a=routeCoords[i],b=routeCoords[i+1];"
             << "var mid=[(a[0]+b[0])/2,(a[1]+b[1])/2];"
             << "var ang=Math.atan2(b[1]-a[1],b[0]-a[0])*180/Math.PI;"
             << "var arrow=L.divIcon({className:'',html:'<div style=\"color:#fff;font-size:16px;transform:rotate('+(90-ang)+'deg);text-shadow:0 0 6px rgba(0,0,0,0.8)\">&#9650;</div>',iconSize:[16,16],iconAnchor:[8,8]});"
             << "L.marker(mid,{icon:arrow,interactive:false}).addTo(map);}\n\n";
    }

    // City markers
    fout << "var allCoords=[];\n";
    fout << "cities.forEach(function(c,i){\n";
    fout << "  allCoords.push([c.lat,c.lng]);\n";
    fout << "  var isSrc=(i===src),isDst=(i===dst);\n";
    fout << "  if(isSrc||isDst){\n";
    fout << "    var col=isSrc?'#2ecc71':'#e74c3c';\n";
    // Pulsating outer ring
    fout << "    var pulseIcon=L.divIcon({className:'',html:'<div class=\"pulse-marker\" style=\"width:24px;height:24px;background:'+col+';opacity:0.4;border-radius:50%\"></div>',iconSize:[24,24],iconAnchor:[12,12]});\n";
    fout << "    L.marker([c.lat,c.lng],{icon:pulseIcon,interactive:false}).addTo(map);\n";
    // Solid inner circle
    fout << "    L.circleMarker([c.lat,c.lng],{radius:10,color:'#fff',weight:2,fillColor:col,fillOpacity:1})"
         << ".bindPopup('<b>'+c.name+'</b><br>'+c.lat+', '+c.lng+'<br>'+c.conn+' conexiuni')"
         << ".bindTooltip(c.name,{permanent:true,direction:'top',offset:[0,-14]}).addTo(map);\n";
    fout << "  } else {\n";
    fout << "    var dx=tDir[i][0]*30,dy=tDir[i][1]*12;\n";
    fout << "    var dir=dx>0?'right':'left';\n";
    fout << "    L.circleMarker([c.lat,c.lng],{radius:6,color:'#1e293b',weight:1.5,fillColor:'#3498db',fillOpacity:0.8})"
         << ".bindPopup('<b>'+c.name+'</b><br>'+c.lat+', '+c.lng+'<br>'+c.conn+' conexiuni')"
         << ".bindTooltip(c.name,{permanent:true,direction:dir,offset:[dx/3,dy/3]}).addTo(map);\n";
    fout << "  }\n";
    fout << "});\n\n";

    // Fit bounds
    fout << "map.fitBounds(L.latLngBounds(allCoords).pad(0.1));\n\n";

    // Legend as Leaflet control
    fout << "var legend=L.control({position:'topright'});\n";
    fout << "legend.onAdd=function(){\n";
    fout << "  var d=L.DomUtil.create('div');\n";
    fout << "  d.innerHTML='";

    // Legend inner HTML
    fout << "<div style=\"background:rgba(22,33,62,0.92);padding:15px;border-radius:10px;color:#e0e0e0;font-size:13px;border:1px solid #2a3a5c;box-shadow:0 4px 20px rgba(0,0,0,0.35);backdrop-filter:blur(8px)\">";
    if (modMultiplu) {
        fout << "<div style=\"font-weight:bold;color:#0ea5e9;margin-bottom:8px;font-size:11px;text-transform:uppercase;letter-spacing:1.5px\">TRASEE</div>";
        int nrLegenda = (nrTrasee < 5) ? nrTrasee : 5;
        for (int i = 0; i < nrLegenda; i++) {
            fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\">"
                 << "<div style=\"width:30px;height:4px;border-radius:2px;margin-right:10px;background:" << culoriMultiple[i % 5]
                 << (i > 0 ? ";opacity:0.6" : "") << "\"></div>"
                 << "Traseu " << (i + 1) << " (" << fmt(distanteTrasee[i], 0) << " km)</div>";
        }
    } else {
        fout << "<div style=\"font-weight:bold;color:#0ea5e9;margin-bottom:8px;font-size:11px;text-transform:uppercase;letter-spacing:1.5px\">TIPURI DE DRUM</div>";
        fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\"><div style=\"width:30px;height:4px;border-radius:2px;margin-right:10px;background:#e74c3c\"></div>A \\u2014 Autopista (100 km/h)</div>";
        fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\"><div style=\"width:30px;height:3px;border-radius:2px;margin-right:10px;background:#3498db\"></div>CN \\u2014 Carretera Nacional (80 km/h)</div>";
        fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\"><div style=\"width:30px;height:3px;border-radius:2px;margin-right:10px;background:#f39c12\"></div>CR \\u2014 Carretera Regional (60 km/h)</div>";
        fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\"><div style=\"width:30px;height:2px;border-radius:2px;margin-right:10px;background:#95a5a6\"></div>CL \\u2014 Carretera Local (40 km/h)</div>";
    }
    fout << "<div style=\"border-top:1px solid #2c3e50;margin:8px 0 6px\"></div>";
    fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\"><div style=\"width:12px;height:12px;border-radius:50%;margin-right:10px;background:#2ecc71;border:2px solid #fff\"></div>Plecare</div>";
    fout << "<div style=\"display:flex;align-items:center\"><div style=\"width:12px;height:12px;border-radius:50%;margin-right:10px;background:#e74c3c;border:2px solid #fff\"></div>Destinatie</div>";
    fout << "</div>';\n";

    fout << "  L.DomEvent.disableClickPropagation(d);\n";
    fout << "  L.DomEvent.disableScrollPropagation(d);\n";
    fout << "  return d;\n";
    fout << "};\n";
    fout << "legend.addTo(map);\n\n";

    // Compass rose as Leaflet control (topright, below legend)
    fout << "var compass=L.control({position:'topright'});\n";
    fout << "compass.onAdd=function(){\n";
    fout << "  var d=L.DomUtil.create('div');\n";
    fout << "  d.innerHTML='<div style=\"background:rgba(22,33,62,0.9);border-radius:50%;width:60px;height:60px;"
         << "display:flex;align-items:center;justify-content:center;border:2px solid #2a3a5c;margin-top:10px\">"
         << "<div style=\"position:relative;width:40px;height:40px\">"
         << "<div style=\"position:absolute;top:0;left:50%;transform:translateX(-50%);color:#e74c3c;font-weight:bold;font-size:14px\">N</div>"
         << "<div style=\"position:absolute;bottom:0;left:50%;transform:translateX(-50%);color:#8892a0;font-size:10px\">S</div>"
         << "<div style=\"position:absolute;left:0;top:50%;transform:translateY(-50%);color:#8892a0;font-size:10px\">V</div>"
         << "<div style=\"position:absolute;right:0;top:50%;transform:translateY(-50%);color:#8892a0;font-size:10px\">E</div>"
         << "<div style=\"position:absolute;top:50%;left:50%;transform:translate(-50%,-50%)\">"
         << "<div style=\"width:0;height:0;border-left:5px solid transparent;border-right:5px solid transparent;"
         << "border-bottom:15px solid #e74c3c;position:absolute;top:-15px;left:-5px\"></div>"
         << "<div style=\"width:0;height:0;border-left:5px solid transparent;border-right:5px solid transparent;"
         << "border-top:15px solid #8892a0;position:absolute;top:0;left:-5px\"></div>"
         << "</div></div></div>';\n";
    fout << "  return d;\n";
    fout << "};\n";
    fout << "compass.addTo(map);\n";
    fout << "</script>\n\n";

    // ══════════════════════════════════
    //  SECTIUNEA 4: STATS PANEL
    // ══════════════════════════════════
    if (!modMultiplu && nrTrasee > 0) {
        int idx = (traseuEvidentiiat >= 0) ? traseuEvidentiiat : 0;
        int nrSeg = lungimeTraseu[idx] - 1;
        double vitezaMedie = (timpTrasee[idx] > 0) ? distanteTrasee[idx] / timpTrasee[idx] : 0;

        fout << "<div class=\"stats-row\">\n";
        fout << "<div class=\"stat-card\"><span class=\"stat-icon\">&#128207;</span>"
             << "<span class=\"stat-value\">" << fmt(distanteTrasee[idx], 0) << " km</span>"
             << "<span class=\"stat-label\">Distanta totala</span></div>\n";
        fout << "<div class=\"stat-card\"><span class=\"stat-icon\">&#9201;</span>"
             << "<span class=\"stat-value\">" << formateazaTimp(timpTrasee[idx]) << "</span>"
             << "<span class=\"stat-label\">Timp estimat</span></div>\n";
        fout << "<div class=\"stat-card\"><span class=\"stat-icon\">&#128739;</span>"
             << "<span class=\"stat-value\">" << nrSeg << " segmente</span>"
             << "<span class=\"stat-label\">Nr. segmente</span></div>\n";
        fout << "<div class=\"stat-card\"><span class=\"stat-icon\">&#127946;</span>"
             << "<span class=\"stat-value\">" << fmt(vitezaMedie, 0) << " km/h</span>"
             << "<span class=\"stat-label\">Viteza medie</span></div>\n";
        fout << "</div>\n\n";
    }

    // ══════════════════════════════════
    //  SECTIUNEA 5: PANOUL DE INFORMATII
    // ══════════════════════════════════
    fout << "<div class=\"info-card\">\n";
    fout << "<h2>" << titlu << ": " << numeOrase[sursa]
         << " &#8594; " << numeOrase[destinatia] << "</h2>\n";

    if (modMultiplu) {
        fout << "<p style=\"color:#94a3b8;margin-bottom:16px\">S-au gasit <strong style=\"color:#0ea5e9\">"
             << nrTrasee << "</strong> trasee posibile (sortate dupa distanta)</p>\n";
        if (nrTrasee > 5) {
            fout << "<p style=\"color:#64748b;margin-bottom:12px;font-size:0.9em\">"
                 << "Pe harta sunt afisate primele 5 trasee. Folositi controlul de straturi din dreapta-jos pentru a le activa/dezactiva.</p>\n";
        }
        fout << "<div class=\"route-list\">\n";
        int nrAfis = (nrTrasee < 30) ? nrTrasee : 30;
        for (int i = 0; i < nrAfis; i++) {
            string borderCol = (i < 5) ? culoriMultiple[i % 5] : "#2c3e50";
            fout << "<div class=\"route-item\" style=\"border-left-color:" << borderCol << "\">"
                 << "<span class=\"route-num\">" << (i + 1) << "</span>"
                 << "<span class=\"route-path\">";
            for (int j = 0; j < lungimeTraseu[i]; j++) {
                fout << numeOrase[trasee[i][j]];
                if (j < lungimeTraseu[i] - 1)
                    fout << " <span class=\"arrow\">&#8594;</span> ";
            }
            fout << "</span>"
                 << "<span class=\"route-dist\">" << fmt(distanteTrasee[i], 0) << " km</span>"
                 << "</div>\n";
        }
        if (nrTrasee > 30) {
            fout << "<div class=\"route-more\">... si alte "
                 << (nrTrasee - 30) << " trasee</div>\n";
        }
        fout << "</div>\n";
    } else if (nrTrasee > 0) {
        int idx = (traseuEvidentiiat >= 0) ? traseuEvidentiiat : 0;

        fout << "<div class=\"route-summary\">";
        for (int i = 0; i < lungimeTraseu[idx]; i++) {
            fout << "<strong>" << numeOrase[trasee[idx][i]] << "</strong>";
            if (i < lungimeTraseu[idx] - 1) fout << " &#8594; ";
        }
        fout << "<br><span class=\"dist\">" << fmt(distanteTrasee[idx], 0) << " km</span>"
             << " &nbsp;&bull;&nbsp; <span class=\"timp\">" << formateazaTimp(timpTrasee[idx]) << "</span>"
             << "</div>\n";

        fout << "<table>\n<thead><tr>"
             << "<th>#</th><th>De la</th><th>La</th>"
             << "<th>Distanta</th><th>Tip drum</th><th>Viteza</th><th>Timp</th>"
             << "</tr></thead>\n<tbody>\n";

        int nrSeg = lungimeTraseu[idx] - 1;
        for (int i = 0; i < nrSeg; i++) {
            int c1 = trasee[idx][i];
            int c2 = trasee[idx][i + 1];
            int dist = distanta[c1][c2];
            int viteza = vitezaMax[c1][c2];
            string tip = tipDrum[c1][c2];
            double timpSeg = (viteza > 0) ? (double)dist / viteza : 0;

            fout << "<tr>"
                 << "<td>" << (i + 1) << "</td>"
                 << "<td>" << numeOrase[c1] << "</td>"
                 << "<td>" << numeOrase[c2] << "</td>"
                 << "<td>" << dist << " km</td>"
                 << "<td><span class=\"road-badge\" style=\"background:"
                 << culoareDrum(tip) << "\">" << tip << "</span>"
                 << getNumeTipDrum(tip) << "</td>"
                 << "<td>" << viteza << " km/h</td>"
                 << "<td>" << formateazaTimp(timpSeg) << "</td>"
                 << "</tr>\n";
        }
        fout << "</tbody>\n";
        fout << "<tr class=\"total-row\">"
             << "<td colspan=\"3\">TOTAL</td>"
             << "<td>" << fmt(distanteTrasee[idx], 0) << " km</td>"
             << "<td colspan=\"2\"></td>"
             << "<td>" << formateazaTimp(timpTrasee[idx]) << "</td>"
             << "</tr>\n";
        fout << "</table>\n";
    }

    fout << "</div>\n\n";

    // ══════════════════════════════════
    //  SECTIUNEA 6: FOOTER
    // ══════════════════════════════════
    fout << R"html(<footer>
<span>Cuba Navigator</span> v2.2 &mdash; Proiect Backtracking C++
<span class="sep">|</span> Algoritm: Backtracking Iterativ
<span class="sep">|</span> Harta: Leaflet.js + OpenStreetMap
</footer>
</div>
</body>
</html>
)html";

    fout.close();
}

// ╔════════════════════════════════════════╗
// ║           FUNCTIA MAIN                ║
// ╚════════════════════════════════════════╝
int main() {
    // Initializam datele Cubei
    initializeazaDate();

    // Afisam bannerul
    afiseazaBanner();

    // Bucla principala a meniului
    int optiune;
    bool rulare = true;

    while (rulare) {
        // Afisam lista de orase si meniul
        afiseazaOrase();
        afiseazaMeniu();

        cout << BOLD << "  Alegeti optiunea: " << RESET;
        cin >> optiune;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "  [!] Optiune invalida! Introduceti un numar intre 1 si 5." << RESET << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (optiune) {

        // ═══════════════════════════════════════════
        // Optiunea 1: Distanta minima intre 2 orase
        // ═══════════════════════════════════════════
        case 1: {
            separator();
            cout << CYAN << BOLD << "  >>> DISTANTA MINIMA INTRE 2 LOCALITATI <<<" << RESET << endl;
            separator();

            sursa = citesteOras("Oras de plecare (numar): ");
            destinatia = citesteOras("Oras de destinatie (numar): ");

            if (sursa == destinatia) {
                cout << RED << "  [!] Sursa si destinatia sunt identice!" << RESET << endl;
                break;
            }

            cout << endl;
            cout << YELLOW << "  Se calculeaza traseele..." << RESET << endl;
            auto start = chrono::high_resolution_clock::now();
            BKT();
            sortTrasee();
            auto end = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            if (nrTrasee == 0) {
                cout << RED << "  [!] Nu exista niciun traseu intre aceste localitati!" << RESET << endl;
            } else {
                cout << GREEN << "  Calculat in " << ms << " ms!" << RESET << endl;
                cout << endl;
                cout << "  Traseul optim (distanta minima):" << endl;
                cout << "  " << CYAN << BOLD;
                afiseazaTraseu(0);
                cout << RESET;

                // Generare HTML
                genereazaHTML("Distanta minima", 0);
                cout << endl;
                deschideHTML();
            }
            separator();
            break;
        }

        // ═══════════════════════════════════════════
        // Optiunea 2: Toate traseele posibile
        // ═══════════════════════════════════════════
        case 2: {
            separator();
            cout << CYAN << BOLD << "  >>> TOATE TRASEELE POSIBILE <<<" << RESET << endl;
            separator();

            sursa = citesteOras("Oras de plecare (numar): ");
            destinatia = citesteOras("Oras de destinatie (numar): ");

            if (sursa == destinatia) {
                cout << RED << "  [!] Sursa si destinatia sunt identice!" << RESET << endl;
                break;
            }

            cout << endl;
            cout << YELLOW << "  Se calculeaza traseele..." << RESET << endl;
            auto start = chrono::high_resolution_clock::now();
            BKT();
            sortTrasee();
            auto end = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            if (nrTrasee == 0) {
                cout << RED << "  [!] Nu exista niciun traseu intre aceste localitati!" << RESET << endl;
            } else {
                cout << GREEN << "  S-au gasit " << nrTrasee << " trasee in " << ms << " ms!" << RESET << endl;
                cout << endl;

                // Limita de afisare in consola
                int deAfisat = (nrTrasee < 10) ? nrTrasee : 10;
                for (int i = 0; i < deAfisat; i++) {
                    cout << "  " << setw(3) << (i + 1) << ". ";
                    for (int j = 0; j < lungimeTraseu[i]; j++) {
                        cout << GREEN << numeOrase[trasee[i][j]] << RESET;
                        if (j < lungimeTraseu[i] - 1) cout << GRAY << " -> " << RESET;
                    }
                    cout << " " << CYAN << BOLD << "(" << fixed << setprecision(0)
                         << distanteTrasee[i] << " km)" << RESET << endl;
                }
                if (nrTrasee > 10) {
                    cout << GRAY << "  ... si inca " << (nrTrasee - 10)
                         << " trasee (vezi HTML pentru lista completa)" << RESET << endl;
                }

                // Generare HTML
                genereazaHTML("Toate traseele posibile", -1);
                cout << endl;
                deschideHTML();
            }
            separator();
            break;
        }

        // ═══════════════════════════════════════════
        // Optiunea 3: Tipurile de drum pe un traseu
        // ═══════════════════════════════════════════
        case 3: {
            separator();
            cout << CYAN << BOLD << "  >>> TIPURILE DE DRUM PE UN TRASEU <<<" << RESET << endl;
            separator();

            sursa = citesteOras("Oras de plecare (numar): ");
            destinatia = citesteOras("Oras de destinatie (numar): ");

            if (sursa == destinatia) {
                cout << RED << "  [!] Sursa si destinatia sunt identice!" << RESET << endl;
                break;
            }

            cout << endl;
            cout << YELLOW << "  Se calculeaza traseele..." << RESET << endl;
            auto start = chrono::high_resolution_clock::now();
            BKT();
            sortTrasee();
            auto end = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            if (nrTrasee == 0) {
                cout << RED << "  [!] Nu exista niciun traseu intre aceste localitati!" << RESET << endl;
                break;
            }

            cout << GREEN << "  S-au gasit " << nrTrasee << " trasee in " << ms << " ms!" << RESET << endl;
            cout << endl;

            // Afisam traseele numerotate (limitat la 20 in consola)
            int deAfisat = (nrTrasee < 20) ? nrTrasee : 20;
            for (int i = 0; i < deAfisat; i++) {
                cout << "  " << setw(3) << (i + 1) << ". ";
                for (int j = 0; j < lungimeTraseu[i]; j++) {
                    cout << GREEN << numeOrase[trasee[i][j]] << RESET;
                    if (j < lungimeTraseu[i] - 1) cout << GRAY << " -> " << RESET;
                }
                cout << " " << CYAN << "(" << fixed << setprecision(0)
                     << distanteTrasee[i] << " km)" << RESET << endl;
            }
            if (nrTrasee > 20) {
                cout << GRAY << "  ... si inca " << (nrTrasee - 20) << " trasee." << RESET << endl;
            }

            // Utilizatorul alege un traseu
            cout << endl;
            int alegere;
            while (true) {
                cout << "  Alegeti numarul traseului pentru detalii: ";
                cin >> alegere;
                if (cin.fail() || alegere < 1 || alegere > nrTrasee) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "  [!] Numar invalid! Alegeti intre 1 si " << nrTrasee << "." << RESET << endl;
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            }

            // Afisam detaliile traseului ales
            cout << endl;
            cout << BOLD << "  Detalii traseu #" << alegere << ":" << RESET << endl;
            afiseazaDetaliiTraseu(alegere - 1);

            // Generare HTML
            genereazaHTML("Tipuri de drum", alegere - 1);
            cout << endl;
            deschideHTML();

            separator();
            break;
        }

        // ═══════════════════════════════════════════
        // Optiunea 4: Timpul de parcurgere
        // ═══════════════════════════════════════════
        case 4: {
            separator();
            cout << CYAN << BOLD << "  >>> TIMPUL DE PARCURGERE <<<" << RESET << endl;
            separator();

            sursa = citesteOras("Oras de plecare (numar): ");
            destinatia = citesteOras("Oras de destinatie (numar): ");

            if (sursa == destinatia) {
                cout << RED << "  [!] Sursa si destinatia sunt identice!" << RESET << endl;
                break;
            }

            cout << endl;
            cout << YELLOW << "  Se calculeaza traseele..." << RESET << endl;
            auto start = chrono::high_resolution_clock::now();
            BKT();
            sortTrasee();
            auto end = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            if (nrTrasee == 0) {
                cout << RED << "  [!] Nu exista niciun traseu intre aceste localitati!" << RESET << endl;
            } else {
                cout << GREEN << "  Calculat in " << ms << " ms!" << RESET << endl;
                cout << endl;
                cout << "  Traseul optim (distanta minima):" << endl;
                cout << "  " << CYAN << BOLD;
                afiseazaTraseu(0);
                cout << RESET;
                cout << endl;
                cout << "  " << CYAN << BOLD << "Distanta totala: " << fixed << setprecision(0)
                     << distanteTrasee[0] << " km" << RESET << endl;
                cout << "  " << CYAN << BOLD << "Timp estimat:    " << formateazaTimp(timpTrasee[0]) << RESET << endl;

                // Afisam detaliile pe segmente
                afiseazaDetaliiTraseu(0);

                // Comparatie moduri de transport
                cout << endl;
                double distKm = distanteTrasee[0];
                cout << BOLD << "  Comparatie moduri de transport:" << RESET << endl;
                cout << "  " << CYAN  << "Cu masina (viteza max): " << formateazaTimp(timpTrasee[0]) << RESET << endl;
                cout << "  " << GREEN << "Cu bicicleta (15 km/h): " << formateazaTimp(distKm / 15.0) << RESET << endl;
                cout << "  " << YELLOW << "Pe jos (5 km/h):        " << formateazaTimp(distKm / 5.0) << RESET << endl;

                // Generare HTML
                genereazaHTML("Timpul de parcurgere", 0);
                cout << endl;
                deschideHTML();
            }
            separator();
            break;
        }

        // ═══════════════════════════════════════════
        // Optiunea 5: Iesire
        // ═══════════════════════════════════════════
        case 5: {
            cout << endl;
            cout << CYAN << BOLD;
            cout << R"(  ╔══════════════════════════════════════════════════╗)" << endl;
            cout << R"(  ║   La revedere! Drum bun prin Cuba!              ║)" << endl;
            cout << R"(  ║   Multumim ca ati folosit Cuba Navigator.       ║)" << endl;
            cout << R"(  ╚══════════════════════════════════════════════════╝)" << endl;
            cout << RESET << endl;
            rulare = false;
            break;
        }

        default:
            cout << RED << "  [!] Optiune invalida! Alegeti intre 1 si 5." << RESET << endl;
            break;
        }
    }

    return 0;
}
