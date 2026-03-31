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
#include <thread>

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
    adaugaDrum(2,  4,  195, "CR", 60);    // Matanzas - Cienfuegos
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
    if (tip == "A")  return "#c45a4a";
    if (tip == "CN") return "#4a8ab5";
    if (tip == "CR") return "#d4944a";
    if (tip == "CL") return "#7a8a7a";
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
    cout << GRAY << "  +-----+------------------------+-------------------+------+" << RESET << endl;
    cout << GRAY << "  | Nr. | Oras                   | Coordonate GPS    | Conn |" << RESET << endl;
    cout << GRAY << "  +-----+------------------------+-------------------+------+" << RESET << endl;
    for (int i = 0; i < NR_ORASE; i++) {
        cout << GRAY << "  | " << RESET << setw(3) << (i + 1)
             << GRAY << " | " << RESET
             << GREEN << setw(22) << left << numeOrase[i] << RESET
             << GRAY << " | " << RESET
             << right << fixed << setprecision(4)
             << setw(7) << latitudine[i] << ", "
             << setw(8) << longitudine[i]
             << GRAY << " | " << RESET << CYAN << setw(4) << getNrConexiuni(i) << RESET
             << GRAY << " |" << RESET << endl;
    }
    cout << GRAY << "  +-----+------------------------+-------------------+------+" << RESET << endl;
}

void afiseazaSpinner(string mesaj) {
    const string frames[] = {"\xe2\xa0\x8b","\xe2\xa0\x99","\xe2\xa0\xb9","\xe2\xa0\xb8",
                             "\xe2\xa0\xbc","\xe2\xa0\xb4","\xe2\xa0\xa6","\xe2\xa0\xa7",
                             "\xe2\xa0\x87","\xe2\xa0\x8f"};
    for (int i = 0; i < 8; i++) {
        cout << "\r  " << YELLOW << frames[i % 10] << " " << mesaj << RESET << flush;
        this_thread::sleep_for(chrono::milliseconds(60));
    }
    cout << "\r  " << GREEN << "\xe2\x9c\x93 " << mesaj << " Gata!          " << RESET << endl;
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
// ║   v3.0 — Retro-futuristic cartography + Cuban vintage design      ║
// ╚══════════════════════════════════════════════════════════════════════╝
void genereazaHTML(string titlu, int traseuEvidentiiat) {
    ofstream fout("rezultat.html");
    if (!fout.is_open()) {
        cerr << "  [!] Nu s-a putut crea fisierul rezultat.html!" << endl;
        return;
    }

    bool modMultiplu = (traseuEvidentiiat == -1);

    // Culori pentru trasee multiple
    string culoriMultiple[5] = {"#c45a4a","#4a8ab5","#4a9e6a","#d4944a","#9a6abf"};

    // Determine if gradient polylines should be used (options 1 and 4: Distanta or Timp)
    bool useGradient = false;
    if (!modMultiplu) {
        if (titlu.find("Distanta") != string::npos || titlu.find("Timp") != string::npos) {
            useGradient = true;
        }
    }

    // Compute road type percentages for donut chart (single mode)
    double kmA = 0, kmCN = 0, kmCR = 0, kmCL = 0;
    if (!modMultiplu && nrTrasee > 0) {
        int idx = (traseuEvidentiiat >= 0) ? traseuEvidentiiat : 0;
        int nrSeg = lungimeTraseu[idx] - 1;
        for (int i = 0; i < nrSeg; i++) {
            int c1 = trasee[idx][i];
            int c2 = trasee[idx][i + 1];
            string tip = tipDrum[c1][c2];
            double d = distanta[c1][c2];
            if (tip == "A") kmA += d;
            else if (tip == "CN") kmCN += d;
            else if (tip == "CR") kmCR += d;
            else if (tip == "CL") kmCL += d;
        }
    }
    double kmTotal = kmA + kmCN + kmCR + kmCL;
    double pctA = (kmTotal > 0) ? (kmA / kmTotal * 100) : 0;
    double pctCN = (kmTotal > 0) ? (kmCN / kmTotal * 100) : 0;
    double pctCR = (kmTotal > 0) ? (kmCR / kmTotal * 100) : 0;
    double pctCL = (kmTotal > 0) ? (kmCL / kmTotal * 100) : 0;

    // Get generation timestamp
    auto now = chrono::system_clock::now();
    time_t nowT = chrono::system_clock::to_time_t(now);
    char timestampBuf[64];
    strftime(timestampBuf, sizeof(timestampBuf), "%Y-%m-%d %H:%M:%S", localtime(&nowT));
    string timestamp(timestampBuf);

    // ══════════════════════════════════
    //  SECTIUNEA 1: HTML HEAD + CSS
    // ══════════════════════════════════
    fout << R"html(<!DOCTYPE html>
<html lang="ro">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Cuba Navigator - Rezultat</title>
<link rel="icon" type="image/svg+xml" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'><text y='.9em' font-size='90'>&#x1F9ED;</text></svg>">
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Playfair+Display:ital,wght@0,400;0,700;0,900;1,400&family=Source+Sans+3:wght@300;400;600;700;800&display=swap" rel="stylesheet">
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
<style>
:root{
    --bg-deep:#0a1a1f;
    --bg-card:rgba(15,35,42,0.75);
    --bg-card-solid:#0f232a;
    --bg-inner:#081318;
    --accent-primary:#c87533;
    --accent-secondary:#d4944a;
    --accent-tertiary:#e8b87a;
    --text-cream:#f5e6d0;
    --text-muted:#8a9a8a;
    --text-dim:#5a6a5a;
    --border-subtle:rgba(200,117,51,0.15);
    --border-accent:rgba(200,117,51,0.3);
    --glass-bg:rgba(15,35,42,0.65);
    --glass-border:rgba(200,117,51,0.12);
    --road-a:#c45a4a;
    --road-cn:#4a8ab5;
    --road-cr:#d4944a;
    --road-cl:#7a8a7a;
    --shadow-heavy:0 8px 40px rgba(0,0,0,0.5),0 0 0 1px var(--border-subtle);
    --shadow-light:0 4px 20px rgba(0,0,0,0.3);
}

*{margin:0;padding:0;box-sizing:border-box}
html{scroll-behavior:smooth}

body{
    font-family:'Source Sans 3','Segoe UI',Tahoma,Geneva,Verdana,sans-serif;
    background:var(--bg-deep);
    color:var(--text-cream);
    min-height:100vh;
    position:relative;
    overflow-x:hidden;
}

/* Noise grain overlay */
body::after{
    content:'';
    position:fixed;
    top:0;left:0;width:100%;height:100%;
    pointer-events:none;
    z-index:9999;
    opacity:0.035;
    background:url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='300' height='300'%3E%3Cfilter id='noise'%3E%3CfeTurbulence type='fractalNoise' baseFrequency='0.9' numOctaves='4' stitchTiles='stitch'/%3E%3C/filter%3E%3Crect width='100%25' height='100%25' filter='url(%23noise)'/%3E%3C/svg%3E");
}

/* Light theme overrides */
body.light{
    --bg-deep:#f5e6d0;
    --bg-card:rgba(255,248,240,0.8);
    --bg-card-solid:#fff8f0;
    --bg-inner:#f0dcc6;
    --text-cream:#1a1a1f;
    --text-muted:#5a5040;
    --text-dim:#8a8070;
    --border-subtle:rgba(200,117,51,0.2);
    --border-accent:rgba(200,117,51,0.35);
    --glass-bg:rgba(255,248,240,0.7);
    --glass-border:rgba(200,117,51,0.18);
    --shadow-heavy:0 8px 40px rgba(0,0,0,0.12),0 0 0 1px var(--border-subtle);
    --shadow-light:0 4px 20px rgba(0,0,0,0.08);
}
body.light::after{opacity:0.02}
body.light .leaflet-tooltip{background:rgba(255,248,240,0.95)!important;color:#1a1a1f!important;border-color:var(--border-accent)!important}
body.light .leaflet-tooltip-top:before{border-top-color:rgba(255,248,240,0.95)!important}
body.light .leaflet-tooltip-bottom:before{border-bottom-color:rgba(255,248,240,0.95)!important}
body.light .leaflet-tooltip-left:before{border-left-color:rgba(255,248,240,0.95)!important}
body.light .leaflet-tooltip-right:before{border-right-color:rgba(255,248,240,0.95)!important}
body.light .leaflet-popup-content-wrapper{background:#fff8f0!important;color:#1a1a1f!important}
body.light .leaflet-popup-tip{background:#fff8f0!important}
body.light .leaflet-control-zoom a{background:#fff8f0!important;color:var(--accent-primary)!important;border-color:var(--border-accent)!important}
body.light .leaflet-control-zoom a:hover{background:#f0dcc6!important}
body.light .leaflet-control-scale-line{background:rgba(255,248,240,0.9)!important;border-color:var(--accent-primary)!important;color:#1a1a1f!important}

.container{max-width:1300px;margin:0 auto;padding:20px}

/* ── Keyframes ── */
@keyframes revealUp{
    from{opacity:0;transform:translateY(30px);filter:blur(6px)}
    to{opacity:1;transform:translateY(0);filter:blur(0)}
}
@keyframes drawLine{
    from{width:0}
    to{width:100%}
}
@keyframes fadeIn{
    from{opacity:0}
    to{opacity:1}
}
@keyframes pulse{
    0%,100%{transform:scale(1);opacity:1}
    50%{transform:scale(1.8);opacity:0.3}
}

/* ── Header ── */
header{
    text-align:center;padding:40px 0 28px;
    animation:revealUp 0.7s ease-out 0s both;
}
header .decorative-line{
    width:0;height:2px;margin:0 auto 18px;
    background:linear-gradient(90deg,transparent,var(--accent-primary),var(--accent-secondary),var(--accent-tertiary),transparent);
    animation:drawLine 1.2s ease-out 0.3s forwards;
}
header .compass-ornament{
    font-size:1.4em;margin-bottom:8px;display:block;
    color:var(--accent-secondary);letter-spacing:4px;
    opacity:0;animation:fadeIn 0.8s ease-out 0.2s forwards;
}
header h1{
    font-family:'Playfair Display',Georgia,serif;
    font-size:2.6em;font-weight:900;
    background:linear-gradient(135deg,var(--accent-primary),var(--accent-secondary),var(--accent-tertiary),var(--text-cream));
    -webkit-background-clip:text;-webkit-text-fill-color:transparent;
    background-clip:text;margin-bottom:8px;letter-spacing:2px;
}
header p{
    font-family:'Source Sans 3',sans-serif;
    font-size:1.05em;color:var(--text-muted);letter-spacing:3px;
    text-transform:uppercase;font-weight:300;
}
.theme-toggle{
    position:absolute;top:20px;right:20px;
    background:var(--glass-bg);
    backdrop-filter:blur(12px);-webkit-backdrop-filter:blur(12px);
    border:1px solid var(--glass-border);
    color:var(--text-cream);font-size:1.3em;
    width:44px;height:44px;border-radius:50%;cursor:pointer;
    display:flex;align-items:center;justify-content:center;
    transition:all 0.3s ease;z-index:100;
}
.theme-toggle:hover{
    background:var(--accent-primary);
    border-color:var(--accent-secondary);
    transform:scale(1.1);
}

/* ── Map Card ── */
.map-card{
    background:var(--glass-bg);
    backdrop-filter:blur(16px);-webkit-backdrop-filter:blur(16px);
    border:1px solid var(--glass-border);
    border-radius:16px;
    box-shadow:var(--shadow-heavy);
    padding:20px;margin-bottom:24px;position:relative;overflow:hidden;
    animation:revealUp 0.7s ease-out 0.2s both;
}
#map{width:100%;height:500px;border-radius:12px;border:2px solid var(--border-accent);z-index:0}
#map.fullscreen-map{
    position:fixed!important;top:0!important;left:0!important;
    width:100vw!important;height:100vh!important;
    border-radius:0!important;border:none!important;
    z-index:10000!important;
}
.fullscreen-btn{
    position:absolute;top:28px;right:28px;z-index:5;
    background:var(--glass-bg);
    backdrop-filter:blur(8px);-webkit-backdrop-filter:blur(8px);
    border:1px solid var(--glass-border);
    color:var(--text-cream);font-size:1.2em;
    width:38px;height:38px;border-radius:8px;cursor:pointer;
    display:flex;align-items:center;justify-content:center;
    transition:all 0.3s ease;
}
.fullscreen-btn:hover{background:var(--accent-primary);border-color:var(--accent-secondary)}

/* Leaflet tooltip and popup themed */
.leaflet-tooltip{
    background:rgba(10,26,31,0.92);color:var(--text-cream);
    border:1px solid var(--border-accent);font-size:11px;font-weight:bold;
    font-family:'Source Sans 3',sans-serif;padding:3px 8px;border-radius:4px;
    box-shadow:0 2px 8px rgba(0,0,0,0.4);
}
.leaflet-tooltip-top:before{border-top-color:rgba(10,26,31,0.92)}
.leaflet-tooltip-bottom:before{border-bottom-color:rgba(10,26,31,0.92)}
.leaflet-tooltip-left:before{border-left-color:rgba(10,26,31,0.92)}
.leaflet-tooltip-right:before{border-right-color:rgba(10,26,31,0.92)}
.leaflet-popup-content-wrapper{background:var(--bg-card-solid);color:var(--text-cream);border-radius:10px;box-shadow:0 4px 20px rgba(0,0,0,0.5)}
.leaflet-popup-content{margin:10px 14px;font-size:13px;line-height:1.5}
.leaflet-popup-tip{background:var(--bg-card-solid)}
.leaflet-control-layers{
    background:rgba(10,26,31,0.92)!important;color:var(--text-cream)!important;
    border:1px solid var(--border-accent)!important;border-radius:10px!important;
    box-shadow:0 4px 20px rgba(0,0,0,0.4)!important;padding:8px 12px!important;
    font-family:'Source Sans 3',sans-serif!important;
}
.leaflet-control-layers-base label,.leaflet-control-layers-overlays label{color:var(--text-cream)!important;margin-bottom:4px!important;font-size:13px}
.leaflet-control-layers-separator{border-top-color:var(--border-accent)!important}
.leaflet-control-zoom a{background:var(--bg-card-solid)!important;color:var(--accent-primary)!important;border-color:var(--border-accent)!important}
.leaflet-control-zoom a:hover{background:var(--bg-inner)!important}
.leaflet-control-scale-line{
    background:rgba(10,26,31,0.85)!important;border-color:var(--accent-primary)!important;
    color:var(--text-cream)!important;font-size:11px!important;padding:2px 8px!important;
}

/* Pulsating markers */
.pulse-marker{border-radius:50%;animation:pulse 2.5s ease-in-out infinite}

/* ── Stats Panel ── */
.stats-row{
    display:grid;grid-template-columns:repeat(4,1fr);gap:16px;
    margin-bottom:24px;
}
.stat-card{
    background:var(--glass-bg);
    backdrop-filter:blur(16px);-webkit-backdrop-filter:blur(16px);
    border:1px solid var(--glass-border);
    border-top:3px solid var(--accent-primary);
    border-radius:12px;padding:20px 18px;text-align:center;
    box-shadow:var(--shadow-light);
    transition:transform 0.2s;
}
.stat-card:nth-child(1){animation:revealUp 0.6s ease-out 0.4s both}
.stat-card:nth-child(2){animation:revealUp 0.6s ease-out 0.5s both}
.stat-card:nth-child(3){animation:revealUp 0.6s ease-out 0.6s both}
.stat-card:nth-child(4){animation:revealUp 0.6s ease-out 0.7s both}
.stat-card:hover{transform:translateY(-3px)}
.stat-icon{font-size:1.6em;margin-bottom:6px;display:block}
.stat-value{font-size:1.5em;font-weight:800;color:var(--accent-primary);display:block;margin-bottom:2px;font-family:'Playfair Display',serif}
.stat-label{font-size:0.78em;color:var(--text-muted);text-transform:uppercase;letter-spacing:1px}

/* ── Info Card ── */
.info-card{
    background:var(--glass-bg);
    backdrop-filter:blur(16px);-webkit-backdrop-filter:blur(16px);
    border:1px solid var(--glass-border);
    border-radius:16px;
    box-shadow:var(--shadow-heavy);
    padding:30px 35px;margin-bottom:24px;
    animation:revealUp 0.7s ease-out 0.6s both;
}
.info-card h2{
    font-family:'Playfair Display',Georgia,serif;
    color:var(--accent-primary);margin-bottom:20px;font-size:1.45em;
    border-bottom:2px solid var(--border-accent);padding-bottom:14px;
}
.route-summary{
    background:var(--bg-inner);border-radius:10px;padding:16px 22px;margin-bottom:18px;
    border-left:4px solid var(--accent-primary);font-size:1.08em;line-height:1.7;
}
.route-summary .dist{color:var(--accent-primary);font-weight:700}
.route-summary .timp{color:var(--accent-secondary);font-size:0.92em}
table{width:100%;border-collapse:collapse;margin:18px 0}
th{
    background:var(--bg-inner);color:var(--accent-primary);padding:12px 16px;
    text-align:left;font-weight:600;text-transform:uppercase;
    font-size:0.78em;letter-spacing:1.2px;
}
td{padding:11px 16px;border-bottom:1px solid var(--border-subtle);font-size:0.95em}
tbody tr:nth-child(even) td{background:rgba(200,117,51,0.03)}
tbody tr:nth-child(odd) td{background:transparent}
tr:hover td{background:rgba(200,117,51,0.07)!important}
.road-badge{display:inline-block;padding:3px 12px;border-radius:14px;font-size:0.82em;font-weight:700;color:#fff;margin-right:6px}
.total-row{background:var(--bg-inner);font-weight:700;font-size:1.05em}
.total-row td{padding:15px 16px;color:var(--accent-primary);border-bottom:none}

/* ── Route List ── */
.route-list{margin-top:16px}
.route-item{
    background:var(--bg-inner);border-radius:10px;padding:13px 20px;margin-bottom:8px;
    display:flex;align-items:center;border-left:4px solid var(--text-dim);
    transition:transform 0.15s,box-shadow 0.15s,background 0.2s;
    cursor:default;
}
.route-item:hover{transform:translateX(4px);box-shadow:0 2px 12px rgba(0,0,0,0.2)}
.route-item.highlight{background:rgba(200,117,51,0.12);box-shadow:0 2px 16px rgba(200,117,51,0.2)}
.route-num{
    background:var(--accent-primary);color:var(--bg-deep);width:30px;height:30px;border-radius:50%;
    display:inline-flex;align-items:center;justify-content:center;
    font-weight:800;font-size:0.85em;margin-right:14px;flex-shrink:0;
}
.route-path{flex:1;font-size:0.93em;color:var(--text-cream)}
.route-path .arrow{color:var(--text-dim);margin:0 4px}
.route-dist{color:var(--accent-primary);font-weight:700;white-space:nowrap;margin-left:14px;font-size:0.95em}
.route-more{text-align:center;color:var(--text-muted);font-style:italic;padding:12px;font-size:0.9em}

/* ── Donut Chart ── */
.donut-container{
    display:flex;align-items:center;gap:30px;margin:20px 0;
    padding:20px;background:var(--bg-inner);border-radius:12px;
}
.donut-chart{
    width:140px;height:140px;border-radius:50%;position:relative;flex-shrink:0;
}
.donut-hole{
    position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);
    width:80px;height:80px;border-radius:50%;background:var(--bg-inner);
    display:flex;align-items:center;justify-content:center;
    font-family:'Playfair Display',serif;font-size:0.85em;color:var(--accent-primary);
    font-weight:700;text-align:center;line-height:1.2;
}
.donut-legend{display:flex;flex-direction:column;gap:8px}
.donut-legend-item{display:flex;align-items:center;gap:10px;font-size:0.9em}
.donut-legend-swatch{width:14px;height:14px;border-radius:3px;flex-shrink:0}

/* ── Footer ── */
footer{
    text-align:center;padding:32px 0;color:var(--text-muted);font-size:0.82em;
    margin-top:8px;letter-spacing:0.3px;
    animation:revealUp 0.7s ease-out 0.8s both;
}
footer .footer-line{
    width:0;height:1px;margin:0 auto 16px;
    background:linear-gradient(90deg,transparent,var(--accent-primary),var(--accent-secondary),transparent);
    animation:drawLine 1s ease-out 1s forwards;
}
footer .footer-brand{
    font-family:'Playfair Display',Georgia,serif;
    font-size:1.3em;font-weight:700;
    color:var(--accent-primary);
    display:block;margin-bottom:6px;
}
footer .footer-timestamp{color:var(--text-dim);font-size:0.9em;margin-top:6px}
footer span{color:var(--accent-primary)}
footer .sep{margin:0 8px;color:var(--text-dim)}

/* ── Responsive ── */
@media(max-width:768px){
    .container{padding:14px}
    .stats-row{grid-template-columns:repeat(2,1fr)}
    #map{height:350px}
    header h1{font-size:1.8em}
    .info-card{padding:20px}
    .donut-container{flex-direction:column;align-items:flex-start}
}
@media(max-width:480px){
    .stats-row{grid-template-columns:1fr}
    #map{height:280px}
    header h1{font-size:1.5em}
    header p{font-size:0.85em;letter-spacing:1.5px}
    .info-card{padding:16px 14px}
    .route-item{padding:10px 14px;flex-wrap:wrap}
    .route-dist{margin-left:0;margin-top:6px;width:100%;text-align:right}
    .donut-chart{width:110px;height:110px}
    .donut-hole{width:60px;height:60px;font-size:0.75em}
}
</style>
</head>
<body>
<div class="container" style="position:relative">
)html";

    // ══════════════════════════════════
    //  SECTIUNEA 2: HEADER with theme toggle
    // ══════════════════════════════════
    fout << "<button class=\"theme-toggle\" id=\"themeToggle\" title=\"Comuta tema\">&#9789;</button>\n";
    fout << "<header>\n";
    fout << "<span class=\"compass-ornament\">&#9678; &#9670; &#9678;</span>\n";
    fout << "<div class=\"decorative-line\"></div>\n";
    fout << "<h1>&#127464;&#127482; Cuba Navigator</h1>\n";
    fout << "<p>Sistem de Navigare Inteligent</p>\n";
    fout << "</header>\n\n";

    // ══════════════════════════════════
    //  SECTIUNEA 3: HARTA LEAFLET
    // ══════════════════════════════════
    fout << "<div class=\"map-card\">\n";
    fout << "<button class=\"fullscreen-btn\" id=\"fullscreenBtn\" title=\"Fullscreen\">&#x26F6;</button>\n";
    fout << "<div id=\"map\"></div>\n";
    fout << "</div>\n\n";

    // Leaflet JavaScript — map init, markers, roads, routes
    fout << "<script>\n";

    // Fullscreen toggle
    fout << R"html(
var mapEl=document.getElementById('map');
var fsBtn=document.getElementById('fullscreenBtn');
var isFullscreen=false;
fsBtn.addEventListener('click',function(){
    isFullscreen=!isFullscreen;
    if(isFullscreen){
        mapEl.classList.add('fullscreen-map');
        fsBtn.style.position='fixed';fsBtn.style.top='12px';fsBtn.style.right='12px';fsBtn.style.zIndex='10001';
        fsBtn.innerHTML='&#x2716;';
    }else{
        mapEl.classList.remove('fullscreen-map');
        fsBtn.style.position='absolute';fsBtn.style.top='28px';fsBtn.style.right='28px';fsBtn.style.zIndex='5';
        fsBtn.innerHTML='&#x26F6;';
    }
    setTimeout(function(){map.invalidateSize()},200);
});
)html";

    // Map setup with dual tile layers (dark + satellite)
    fout << "var darkTileUrl='https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png';\n";
    fout << "var lightTileUrl='https://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}{r}.png';\n";
    fout << "var map=L.map('map',{center:[21.5,-79.5],zoom:7,zoomControl:true,scrollWheelZoom:true});\n";
    fout << "var currentTileLayer=L.tileLayer(darkTileUrl,{"
         << "attribution:'\\u00a9 OpenStreetMap contributors \\u00a9 CARTO',maxZoom:19}).addTo(map);\n";
    fout << "var satelliteLayer=L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}',{"
         << "attribution:'\\u00a9 Esri, Maxar, Earthstar Geographics',maxZoom:19});\n";
    fout << "L.control.layers({'\\ud83c\\udf19 Dark':currentTileLayer,'\\ud83d\\udef0\\ufe0f Satelit':satelliteLayer},null,"
         << "{position:'topleft',collapsed:false}).addTo(map);\n";
    fout << "L.control.scale({metric:true,imperial:false,position:'bottomleft',maxWidth:150}).addTo(map);\n\n";

    // Theme toggle JS
    fout << R"html(
var themeBtn=document.getElementById('themeToggle');
var isLight=false;
themeBtn.addEventListener('click',function(){
    isLight=!isLight;
    document.body.classList.toggle('light',isLight);
    themeBtn.innerHTML=isLight?'&#9788;':'&#9789;';
    map.removeLayer(currentTileLayer);
    currentTileLayer=L.tileLayer(isLight?lightTileUrl:darkTileUrl,{
        attribution:'\u00a9 OpenStreetMap contributors \u00a9 CARTO',maxZoom:19
    }).addTo(map);
});
)html";

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
                     << "]],{color:'rgba(200,117,51,0.1)',weight:2,dashArray:'5,10'}).addTo(map);\n";
            }
    fout << "\n";

    // Route drawing
    if (modMultiplu) {
        // Multiple routes mode — up to 5 on map with layer control + hover highlights
        int nrPeHarta = (nrTrasee < 5) ? nrTrasee : 5;
        fout << "var overlays={};\n";
        fout << "var routeLayerGroups=[];\n";
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
            fout << "routeLayerGroups.push({group:rg" << r << ",color:'" << culoare << "',origWeight:" << weight << ",origOpacity:" << opac << "});\n";
            fout << "overlays['Traseu " << (r + 1) << " (" << fmt(distanteTrasee[r], 0) << " km)']=rg" << r << ";\n";
        }
        fout << "L.control.layers(null,overlays,{collapsed:false,position:'bottomright',sortLayers:false}).addTo(map);\n\n";

        // Hover highlight functions for multiple mode
        fout << R"html(
function highlightMapRoute(idx){
    routeLayerGroups.forEach(function(rl,i){
        rl.group.eachLayer(function(layer){
            if(i===idx){
                layer.setStyle({weight:7,opacity:1});
            }else{
                layer.setStyle({weight:2,opacity:0.15});
            }
        });
    });
}
function resetMapRoutes(){
    routeLayerGroups.forEach(function(rl){
        rl.group.eachLayer(function(layer){
            layer.setStyle({weight:rl.origWeight,opacity:parseFloat(rl.origOpacity)});
        });
    });
}
function highlightListItem(idx){
    var items=document.querySelectorAll('.route-item');
    items.forEach(function(el,i){
        if(i===idx) el.classList.add('highlight');
        else el.classList.remove('highlight');
    });
}
function resetListItems(){
    document.querySelectorAll('.route-item').forEach(function(el){
        el.classList.remove('highlight');
    });
}
)html";
        // Attach map hover events to route layers
        for (int r = 0; r < nrPeHarta; r++) {
            fout << "rg" << r << ".eachLayer(function(layer){\n";
            fout << "  layer.on('mouseover',function(){highlightMapRoute(" << r << ");highlightListItem(" << r << ");});\n";
            fout << "  layer.on('mouseout',function(){resetMapRoutes();resetListItems();});\n";
            fout << "});\n";
        }
        fout << "\n";

    } else if (traseuEvidentiiat >= 0 && traseuEvidentiiat < nrTrasee) {
        // Single route mode — animated drawing with segments array
        int idx = traseuEvidentiiat;
        int nrSeg = lungimeTraseu[idx] - 1;

        // Build segments array for animation
        fout << "var segments=[\n";
        for (int i = 0; i < nrSeg; i++) {
            int c1 = trasee[idx][i];
            int c2 = trasee[idx][i + 1];
            string tip = tipDrum[c1][c2];
            string col = culoareDrum(tip);
            int w = (tip == "A") ? 5 : (tip == "CL") ? 3 : 4;
            fout << "  {lat1:" << fmt(latitudine[c1], 4) << ",lng1:" << fmt(longitudine[c1], 4)
                 << ",lat2:" << fmt(latitudine[c2], 4) << ",lng2:" << fmt(longitudine[c2], 4)
                 << ",color:'" << col << "',weight:" << w
                 << ",name1:'" << numeOrase[c1] << "',name2:'" << numeOrase[c2] << "'"
                 << ",dist:" << distanta[c1][c2]
                 << ",tip:'" << getNumeTipDrum(tip) << "'"
                 << ",viteza:" << vitezaMax[c1][c2]
                 << "}";
            if (i < nrSeg - 1) fout << ",";
            fout << "\n";
        }
        fout << "];\n";

        // Fly-to animation: start on source, fly to dest, then fitBounds
        fout << "var srcCity=cities[src];\n";
        fout << "var dstCity=cities[dst];\n";
        fout << "map.setView([srcCity.lat,srcCity.lng],11);\n\n";

        // Animated segment drawing
        if (useGradient) {
            // Gradient polylines: HSL green→red by progress
            fout << R"html(
var drawnLayers=[];
var arrowLayers=[];
var segIdx=0;
function drawNextSegment(){
    if(segIdx>=segments.length){
        var allPts=segments.map(function(s){return [[s.lat1,s.lng1],[s.lat2,s.lng2]];}).flat();
        setTimeout(function(){map.flyToBounds(L.latLngBounds(allPts).pad(0.15),{duration:1.5});},400);
        return;
    }
    var s=segments[segIdx];
    var progress=segIdx/(segments.length-1||1);
    var hue=Math.round(120*(1-progress));
    var gradColor='hsl('+hue+',70%,50%)';
    var line=L.polyline([[s.lat1,s.lng1],[s.lat2,s.lng2]],{
        color:gradColor,weight:s.weight,opacity:0
    }).bindPopup('<b>'+s.name1+' \u2192 '+s.name2+'</b><br>'+s.dist+' km | '+s.tip+' | '+s.viteza+' km/h').addTo(map);
    drawnLayers.push(line);
    var startOpacity=0;
    var fadeInterval=setInterval(function(){
        startOpacity+=0.1;
        if(startOpacity>=1){startOpacity=1;clearInterval(fadeInterval);}
        line.setStyle({opacity:startOpacity});
    },30);
    // Direction arrow after segment draws
    var mid=[(s.lat1+s.lat2)/2,(s.lng1+s.lng2)/2];
    var ang=Math.atan2(s.lng2-s.lng1,s.lat2-s.lat1)*180/Math.PI;
    var arrow=L.divIcon({className:'',html:'<div style="color:#fff;font-size:16px;transform:rotate('+(90-ang)+'deg);text-shadow:0 0 6px rgba(0,0,0,0.8)">&#9650;</div>',iconSize:[16,16],iconAnchor:[8,8]});
    setTimeout(function(){
        var am=L.marker(mid,{icon:arrow,interactive:false}).addTo(map);
        arrowLayers.push(am);
    },300);
    if(segIdx===0){
        setTimeout(function(){
            map.flyTo([dstCity.lat,dstCity.lng],11,{duration:1.2});
            setTimeout(function(){segIdx++;drawNextSegment();},1400);
        },600);
    }else{
        segIdx++;
        setTimeout(drawNextSegment,400);
    }
}
setTimeout(function(){drawNextSegment();},800);
)html";
        } else {
            // Road-type colored polylines with animation (option 3)
            fout << R"html(
var drawnLayers=[];
var arrowLayers=[];
var segIdx=0;
function drawNextSegment(){
    if(segIdx>=segments.length){
        var allPts=segments.map(function(s){return [[s.lat1,s.lng1],[s.lat2,s.lng2]];}).flat();
        setTimeout(function(){map.flyToBounds(L.latLngBounds(allPts).pad(0.15),{duration:1.5});},400);
        return;
    }
    var s=segments[segIdx];
    var line=L.polyline([[s.lat1,s.lng1],[s.lat2,s.lng2]],{
        color:s.color,weight:s.weight,opacity:0
    }).bindPopup('<b>'+s.name1+' \u2192 '+s.name2+'</b><br>'+s.dist+' km | '+s.tip+' | '+s.viteza+' km/h').addTo(map);
    drawnLayers.push(line);
    var startOpacity=0;
    var fadeInterval=setInterval(function(){
        startOpacity+=0.1;
        if(startOpacity>=1){startOpacity=1;clearInterval(fadeInterval);}
        line.setStyle({opacity:startOpacity});
    },30);
    // Direction arrow after segment draws
    var mid=[(s.lat1+s.lat2)/2,(s.lng1+s.lng2)/2];
    var ang=Math.atan2(s.lng2-s.lng1,s.lat2-s.lat1)*180/Math.PI;
    var arrow=L.divIcon({className:'',html:'<div style="color:#fff;font-size:16px;transform:rotate('+(90-ang)+'deg);text-shadow:0 0 6px rgba(0,0,0,0.8)">&#9650;</div>',iconSize:[16,16],iconAnchor:[8,8]});
    setTimeout(function(){
        var am=L.marker(mid,{icon:arrow,interactive:false}).addTo(map);
        arrowLayers.push(am);
    },300);
    if(segIdx===0){
        setTimeout(function(){
            map.flyTo([dstCity.lat,dstCity.lng],11,{duration:1.2});
            setTimeout(function(){segIdx++;drawNextSegment();},1400);
        },600);
    }else{
        segIdx++;
        setTimeout(drawNextSegment,400);
    }
}
setTimeout(function(){drawNextSegment();},800);
)html";
        }
    }

    // City markers — in single mode use numbered markers for route cities
    if (!modMultiplu && traseuEvidentiiat >= 0 && traseuEvidentiiat < nrTrasee) {
        int idx = traseuEvidentiiat;
        // Build route city set with step numbers
        fout << "var routeSteps={};\n";
        for (int i = 0; i < lungimeTraseu[idx]; i++) {
            fout << "routeSteps[" << trasee[idx][i] << "]=" << (i + 1) << ";\n";
        }
        fout << "var allCoords=[];\n";
        fout << "cities.forEach(function(c,i){\n";
        fout << "  allCoords.push([c.lat,c.lng]);\n";
        fout << "  var isSrc=(i===src),isDst=(i===dst);\n";
        fout << "  var stepNum=routeSteps[i];\n";
        fout << "  if(stepNum!==undefined){\n";
        fout << "    var bgCol=isSrc?'#2ecc71':isDst?'#e74c3c':'var(--accent-primary,#c87533)';\n";
        // Pulsating ring for src/dst
        fout << "    if(isSrc||isDst){\n";
        fout << "      var pulseIcon=L.divIcon({className:'',html:'<div class=\"pulse-marker\" style=\"width:24px;height:24px;background:'+bgCol+';opacity:0.4;border-radius:50%\"></div>',iconSize:[24,24],iconAnchor:[12,12]});\n";
        fout << "      L.marker([c.lat,c.lng],{icon:pulseIcon,interactive:false}).addTo(map);\n";
        fout << "    }\n";
        // Numbered circle marker
        fout << "    var numIcon=L.divIcon({className:'',html:'<div style=\"width:28px;height:28px;border-radius:50%;background:'+bgCol+';color:#fff;display:flex;align-items:center;justify-content:center;font-weight:800;font-size:12px;border:2px solid #fff;box-shadow:0 2px 8px rgba(0,0,0,0.4);font-family:Source Sans 3,sans-serif\">'+stepNum+'</div>',iconSize:[28,28],iconAnchor:[14,14]});\n";
        fout << "    L.marker([c.lat,c.lng],{icon:numIcon})"
             << ".bindPopup('<b>'+c.name+'</b><br>Pas '+stepNum+'<br>'+c.lat+', '+c.lng+'<br>'+c.conn+' conexiuni')"
             << ".bindTooltip(c.name,{permanent:true,direction:'top',offset:[0,-18]}).addTo(map);\n";
        fout << "  } else {\n";
        fout << "    var dx=tDir[i][0]*30,dy=tDir[i][1]*12;\n";
        fout << "    var dir=dx>0?'right':'left';\n";
        fout << "    L.circleMarker([c.lat,c.lng],{radius:5,color:'#1e293b',weight:1.5,fillColor:'rgba(200,117,51,0.5)',fillOpacity:0.6})"
             << ".bindPopup('<b>'+c.name+'</b><br>'+c.lat+', '+c.lng+'<br>'+c.conn+' conexiuni')"
             << ".bindTooltip(c.name,{permanent:true,direction:dir,offset:[dx/3,dy/3]}).addTo(map);\n";
        fout << "  }\n";
        fout << "});\n\n";
    } else {
        // Standard markers for multiple mode
        fout << "var allCoords=[];\n";
        fout << "cities.forEach(function(c,i){\n";
        fout << "  allCoords.push([c.lat,c.lng]);\n";
        fout << "  var isSrc=(i===src),isDst=(i===dst);\n";
        fout << "  if(isSrc||isDst){\n";
        fout << "    var col=isSrc?'#2ecc71':'#e74c3c';\n";
        fout << "    var pulseIcon=L.divIcon({className:'',html:'<div class=\"pulse-marker\" style=\"width:24px;height:24px;background:'+col+';opacity:0.4;border-radius:50%\"></div>',iconSize:[24,24],iconAnchor:[12,12]});\n";
        fout << "    L.marker([c.lat,c.lng],{icon:pulseIcon,interactive:false}).addTo(map);\n";
        fout << "    L.circleMarker([c.lat,c.lng],{radius:10,color:'#fff',weight:2,fillColor:col,fillOpacity:1})"
             << ".bindPopup('<b>'+c.name+'</b><br>'+c.lat+', '+c.lng+'<br>'+c.conn+' conexiuni')"
             << ".bindTooltip(c.name,{permanent:true,direction:'top',offset:[0,-14]}).addTo(map);\n";
        fout << "  } else {\n";
        fout << "    var dx=tDir[i][0]*30,dy=tDir[i][1]*12;\n";
        fout << "    var dir=dx>0?'right':'left';\n";
        fout << "    L.circleMarker([c.lat,c.lng],{radius:6,color:'#1e293b',weight:1.5,fillColor:'#c87533',fillOpacity:0.8})"
             << ".bindPopup('<b>'+c.name+'</b><br>'+c.lat+', '+c.lng+'<br>'+c.conn+' conexiuni')"
             << ".bindTooltip(c.name,{permanent:true,direction:dir,offset:[dx/3,dy/3]}).addTo(map);\n";
        fout << "  }\n";
        fout << "});\n\n";
    }

    // Fit bounds — multiple mode uses fitBounds immediately, single mode handled in animation
    if (modMultiplu) {
        fout << "map.fitBounds(L.latLngBounds(allCoords).pad(0.1));\n\n";
    }

    // Legend as Leaflet control
    fout << "var legend=L.control({position:'topright'});\n";
    fout << "legend.onAdd=function(){\n";
    fout << "  var d=L.DomUtil.create('div');\n";
    fout << "  d.innerHTML='";

    // Legend inner HTML — glassmorphism styled
    fout << "<div style=\"background:rgba(10,26,31,0.88);padding:15px;border-radius:10px;color:var(--text-cream,#f5e6d0);font-size:13px;border:1px solid rgba(200,117,51,0.2);box-shadow:0 4px 20px rgba(0,0,0,0.35);backdrop-filter:blur(12px);-webkit-backdrop-filter:blur(12px);font-family:Source Sans 3,sans-serif\">";
    if (modMultiplu) {
        fout << "<div style=\"font-weight:bold;color:#c87533;margin-bottom:8px;font-size:11px;text-transform:uppercase;letter-spacing:1.5px;font-family:Playfair Display,serif\">TRASEE</div>";
        int nrLegenda = (nrTrasee < 5) ? nrTrasee : 5;
        for (int i = 0; i < nrLegenda; i++) {
            fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\">"
                 << "<div style=\"width:30px;height:4px;border-radius:2px;margin-right:10px;background:" << culoriMultiple[i % 5]
                 << (i > 0 ? ";opacity:0.6" : "") << "\"></div>"
                 << "Traseu " << (i + 1) << " (" << fmt(distanteTrasee[i], 0) << " km)</div>";
        }
    } else {
        fout << "<div style=\"font-weight:bold;color:#c87533;margin-bottom:8px;font-size:11px;text-transform:uppercase;letter-spacing:1.5px;font-family:Playfair Display,serif\">TIPURI DE DRUM</div>";
        fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\"><div style=\"width:30px;height:4px;border-radius:2px;margin-right:10px;background:#c45a4a\"></div>A \\u2014 Autopista (100 km/h)</div>";
        fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\"><div style=\"width:30px;height:3px;border-radius:2px;margin-right:10px;background:#4a8ab5\"></div>CN \\u2014 Carretera Nacional (80 km/h)</div>";
        fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\"><div style=\"width:30px;height:3px;border-radius:2px;margin-right:10px;background:#d4944a\"></div>CR \\u2014 Carretera Regional (60 km/h)</div>";
        fout << "<div style=\"display:flex;align-items:center;margin-bottom:6px\"><div style=\"width:30px;height:2px;border-radius:2px;margin-right:10px;background:#7a8a7a\"></div>CL \\u2014 Carretera Local (40 km/h)</div>";
    }
    fout << "<div style=\"border-top:1px solid rgba(200,117,51,0.2);margin:8px 0 6px\"></div>";
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
    fout << "  d.innerHTML='<div style=\"background:rgba(10,26,31,0.9);border-radius:50%;width:60px;height:60px;"
         << "display:flex;align-items:center;justify-content:center;border:2px solid rgba(200,117,51,0.25);margin-top:10px\">"
         << "<div style=\"position:relative;width:40px;height:40px\">"
         << "<div style=\"position:absolute;top:0;left:50%;transform:translateX(-50%);color:#c45a4a;font-weight:bold;font-size:14px;font-family:Playfair Display,serif\">N</div>"
         << "<div style=\"position:absolute;bottom:0;left:50%;transform:translateX(-50%);color:#8a9a8a;font-size:10px\">S</div>"
         << "<div style=\"position:absolute;left:0;top:50%;transform:translateY(-50%);color:#8a9a8a;font-size:10px\">V</div>"
         << "<div style=\"position:absolute;right:0;top:50%;transform:translateY(-50%);color:#8a9a8a;font-size:10px\">E</div>"
         << "<div style=\"position:absolute;top:50%;left:50%;transform:translate(-50%,-50%)\">"
         << "<div style=\"width:0;height:0;border-left:5px solid transparent;border-right:5px solid transparent;"
         << "border-bottom:15px solid #c45a4a;position:absolute;top:-15px;left:-5px\"></div>"
         << "<div style=\"width:0;height:0;border-left:5px solid transparent;border-right:5px solid transparent;"
         << "border-top:15px solid #8a9a8a;position:absolute;top:0;left:-5px\"></div>"
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
        fout << "<p style=\"color:var(--text-muted);margin-bottom:16px\">S-au gasit <strong style=\"color:var(--accent-primary)\">"
             << nrTrasee << "</strong> trasee posibile (sortate dupa distanta)</p>\n";
        if (nrTrasee > 5) {
            fout << "<p style=\"color:var(--text-dim);margin-bottom:12px;font-size:0.9em\">"
                 << "Pe harta sunt afisate primele 5 trasee. Folositi controlul de straturi din dreapta-jos pentru a le activa/dezactiva.</p>\n";
        }
        fout << "<div class=\"route-list\">\n";
        int nrAfis = (nrTrasee < 30) ? nrTrasee : 30;
        int nrPeHarta = (nrTrasee < 5) ? nrTrasee : 5;
        for (int i = 0; i < nrAfis; i++) {
            string borderCol = (i < 5) ? culoriMultiple[i % 5] : "#5a6a5a";
            fout << "<div class=\"route-item\" data-route=\"" << i << "\" style=\"border-left-color:" << borderCol << "\"";
            // Hover events for items on the map (first 5)
            if (i < nrPeHarta) {
                fout << " onmouseover=\"highlightMapRoute(" << i << ");this.classList.add('highlight');\""
                     << " onmouseout=\"resetMapRoutes();this.classList.remove('highlight');\"";
            }
            fout << ">"
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
        fout << "<tr class=\"total-row\">"
             << "<td colspan=\"3\">TOTAL</td>"
             << "<td>" << fmt(distanteTrasee[idx], 0) << " km</td>"
             << "<td colspan=\"2\"></td>"
             << "<td>" << formateazaTimp(timpTrasee[idx]) << "</td>"
             << "</tr>\n";
        fout << "</tbody>\n";
        fout << "</table>\n";

        // Donut chart for road type distribution (single mode)
        if (kmTotal > 0) {
            double degA = pctA * 3.6;
            double degCN = pctCN * 3.6;
            double degCR = pctCR * 3.6;
            (void)pctCL; // used implicitly as remainder in conic-gradient

            double endA = degA;
            double endCN = endA + degCN;
            double endCR = endCN + degCR;

            fout << "<div class=\"donut-container\">\n";
            fout << "<div class=\"donut-chart\" style=\"background:conic-gradient("
                 << "#c45a4a 0deg " << fmt(endA, 1) << "deg,"
                 << "#4a8ab5 " << fmt(endA, 1) << "deg " << fmt(endCN, 1) << "deg,"
                 << "#d4944a " << fmt(endCN, 1) << "deg " << fmt(endCR, 1) << "deg,"
                 << "#7a8a7a " << fmt(endCR, 1) << "deg 360deg"
                 << ")\">\n";
            fout << "<div class=\"donut-hole\">" << fmt(kmTotal, 0) << "<br>km</div>\n";
            fout << "</div>\n";
            fout << "<div class=\"donut-legend\">\n";
            if (pctA > 0)
                fout << "<div class=\"donut-legend-item\"><div class=\"donut-legend-swatch\" style=\"background:#c45a4a\"></div>"
                     << "Autopista: " << fmt(kmA, 0) << " km (" << fmt(pctA, 1) << "%)</div>\n";
            if (pctCN > 0)
                fout << "<div class=\"donut-legend-item\"><div class=\"donut-legend-swatch\" style=\"background:#4a8ab5\"></div>"
                     << "Carretera Nacional: " << fmt(kmCN, 0) << " km (" << fmt(pctCN, 1) << "%)</div>\n";
            if (pctCR > 0)
                fout << "<div class=\"donut-legend-item\"><div class=\"donut-legend-swatch\" style=\"background:#d4944a\"></div>"
                     << "Carretera Regional: " << fmt(kmCR, 0) << " km (" << fmt(pctCR, 1) << "%)</div>\n";
            if (pctCL > 0)
                fout << "<div class=\"donut-legend-item\"><div class=\"donut-legend-swatch\" style=\"background:#7a8a7a\"></div>"
                     << "Carretera Local: " << fmt(kmCL, 0) << " km (" << fmt(pctCL, 1) << "%)</div>\n";
            fout << "</div>\n";
            fout << "</div>\n";
        }
    }

    fout << "</div>\n\n";

    // ══════════════════════════════════
    //  SECTIUNEA 6: FOOTER
    // ══════════════════════════════════
    fout << "<footer>\n";
    fout << "<div class=\"footer-line\"></div>\n";
    fout << "<span class=\"footer-brand\">Cuba Navigator</span>\n";
    fout << "v3.0 &mdash; Proiect Backtracking C++\n";
    fout << "<span class=\"sep\">|</span> Algoritm: Backtracking Iterativ\n";
    fout << "<span class=\"sep\">|</span> Harta: Leaflet.js + OpenStreetMap\n";
    fout << "<div class=\"footer-timestamp\">Generat: " << timestamp << "</div>\n";
    fout << "</footer>\n";
    fout << "</div>\n";
    fout << "</body>\n";
    fout << "</html>\n";

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
            afiseazaSpinner("Se calculeaza traseele...");
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
            afiseazaSpinner("Se calculeaza traseele...");
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
            afiseazaSpinner("Se calculeaza traseele...");
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
            afiseazaSpinner("Se calculeaza traseele...");
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
                cout << "  \xF0\x9F\x9A\x97 " << CYAN  << "Cu masina (viteza max): " << formateazaTimp(timpTrasee[0]) << RESET << endl;
                cout << "  \xF0\x9F\x9A\xB4 " << GREEN << "Cu bicicleta (15 km/h): " << formateazaTimp(distKm / 15.0) << RESET << endl;
                cout << "  \xF0\x9F\x9A\xB6 " << YELLOW << "Pe jos (5 km/h):        " << formateazaTimp(distKm / 5.0) << RESET << endl;

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
