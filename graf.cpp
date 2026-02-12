#include "graf.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <climits>
#include <cfloat>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

// Coduri ANSI pentru culori in consola
static const string C_RESET  = "\033[0m";
static const string C_GREEN  = "\033[32m";
static const string C_CYAN   = "\033[36m";
static const string C_GRAY   = "\033[90m";
static const string C_BOLD   = "\033[1m";

// Constructor — initializeaza graful gol
Graf::Graf() : nrOrase(0) {}

// Returneaza viteza maxima pentru un tip de drum
int Graf::getVitezaMaxima(const string& tipDrum) const {
    if (tipDrum == "A")  return 100;
    if (tipDrum == "CN") return 80;
    if (tipDrum == "CR") return 60;
    if (tipDrum == "CL") return 40;
    return 50;
}

// Returneaza denumirea completa a tipului de drum
string Graf::getNumeTipDrum(const string& tipDrum) const {
    if (tipDrum == "A")  return "Autopista Nacional";
    if (tipDrum == "CN") return "Carretera Nacional";
    if (tipDrum == "CR") return "Carretera Regional";
    if (tipDrum == "CL") return "Carretera Local";
    return "Necunoscut";
}

// Adauga un oras in graf
void Graf::adaugaOras(const string& nume, double lat, double lon) {
    orase.push_back({nume, lat, lon});
    listaAdiacenta.push_back({});
    nrOrase++;
}

// Adauga un drum bidirectional intre doua orase
void Graf::adaugaDrum(int oras1, int oras2, double distanta, const string& tipDrum) {
    int viteza = getVitezaMaxima(tipDrum);
    listaAdiacenta[oras1].push_back({oras2, distanta, tipDrum, viteza});
    listaAdiacenta[oras2].push_back({oras1, distanta, tipDrum, viteza});
}

// Initializeaza graful cu cele 15 orase si drumurile reale ale Cubei
void Graf::initializeazaCuba() {
    adaugaOras("Havana",            23.1136, -82.3666);  // 0
    adaugaOras("Varadero",          23.1394, -81.2863);  // 1
    adaugaOras("Matanzas",          23.0510, -81.5775);  // 2
    adaugaOras("Santa Clara",       22.4065, -79.9553);  // 3
    adaugaOras("Cienfuegos",        22.1456, -80.4364);  // 4
    adaugaOras("Trinidad",          21.8027, -79.9843);  // 5
    adaugaOras("Sancti Spiritus",   21.9304, -79.4425);  // 6
    adaugaOras("Ciego de Avila",    21.8405, -78.7621);  // 7
    adaugaOras("Camagiiey",         21.3891, -77.9160);  // 8
    adaugaOras("Holguin",           20.8872, -76.2630);  // 9
    adaugaOras("Santiago de Cuba",  20.0247, -75.8219);  // 10
    adaugaOras("Bayamo",            20.3792, -76.6434);  // 11
    adaugaOras("Guantanamo",        20.1449, -75.2091);  // 12
    adaugaOras("Pinar del Rio",     22.4175, -83.6978);  // 13
    adaugaOras("Las Tunas",         20.9601, -76.9542);  // 14

    adaugaDrum(0,  2,  102, "A");
    adaugaDrum(0,  13, 162, "CN");
    adaugaDrum(2,  1,  42,  "CR");
    adaugaDrum(2,  3,  195, "A");
    adaugaDrum(3,  4,  84,  "CN");
    adaugaDrum(4,  5,  80,  "CR");
    adaugaDrum(5,  6,  74,  "CR");
    adaugaDrum(3,  6,  89,  "CN");
    adaugaDrum(6,  7,  110, "CN");
    adaugaDrum(7,  8,  110, "A");
    adaugaDrum(8,  14, 120, "CN");
    adaugaDrum(14, 9,  76,  "CN");
    adaugaDrum(14, 11, 92,  "CN");
    adaugaDrum(11, 10, 127, "CN");
    adaugaDrum(10, 12, 86,  "CR");
    adaugaDrum(9,  10, 135, "CN");
    adaugaDrum(11, 9,  72,  "CR");
    adaugaDrum(2,  4,  157, "CR");
    adaugaDrum(3,  7,  188, "A");
    adaugaDrum(8,  9,  206, "CN");
}

// Afiseaza lista de orase numerotata (cu culori ANSI)
void Graf::afiseazaOrase() const {
    cout << "\n";
    cout << C_GRAY << "  +-----+------------------------+-------------------+" << C_RESET << endl;
    cout << C_GRAY << "  | Nr. | Oras                   | Coordonate GPS    |" << C_RESET << endl;
    cout << C_GRAY << "  +-----+------------------------+-------------------+" << C_RESET << endl;
    for (int i = 0; i < nrOrase; i++) {
        cout << C_GRAY << "  | " << C_RESET << setw(3) << (i + 1)
             << C_GRAY << " | " << C_RESET
             << C_GREEN << setw(22) << left << orase[i].nume << C_RESET
             << C_GRAY << " | " << C_RESET
             << right << fixed << setprecision(4)
             << setw(7) << orase[i].latitudine << ", "
             << setw(8) << orase[i].longitudine
             << C_GRAY << " |" << C_RESET << endl;
    }
    cout << C_GRAY << "  +-----+------------------------+-------------------+" << C_RESET << endl;
}

// === ALGORITMUL BACKTRACKING ===
void Graf::backtracking(int nodCurent, int destinatie,
                        vector<bool>& vizitat,
                        Traseu& traseulCurent,
                        vector<Traseu>& toateTraseele) {
    if (nodCurent == destinatie) {
        traseulCurent.timpTotal = calculeazaTimp(traseulCurent);
        toateTraseele.push_back(traseulCurent);
        return;
    }

    for (const auto& drum : listaAdiacenta[nodCurent]) {
        if (!vizitat[drum.destinatie]) {
            vizitat[drum.destinatie] = true;
            traseulCurent.noduri.push_back(drum.destinatie);
            traseulCurent.distantaTotala += drum.distanta;
            traseulCurent.tipuriDrum.push_back(drum.tipDrum);

            backtracking(drum.destinatie, destinatie, vizitat, traseulCurent, toateTraseele);

            traseulCurent.noduri.pop_back();
            traseulCurent.distantaTotala -= drum.distanta;
            traseulCurent.tipuriDrum.pop_back();
            vizitat[drum.destinatie] = false;
        }
    }
}

// Returneaza traseul cu distanta minima
Traseu Graf::gasesteTraseuMinim(int sursa, int destinatie) {
    vector<Traseu> toateTraseele = gasesteToateTraseele(sursa, destinatie);
    if (toateTraseele.empty()) return {{}, 0, 0, {}};

    Traseu minim = toateTraseele[0];
    for (const auto& traseu : toateTraseele) {
        if (traseu.distantaTotala < minim.distantaTotala)
            minim = traseu;
    }
    return minim;
}

// Returneaza toate traseele posibile
vector<Traseu> Graf::gasesteToateTraseele(int sursa, int destinatie) {
    vector<Traseu> toateTraseele;
    vector<bool> vizitat(nrOrase, false);

    Traseu traseulCurent;
    traseulCurent.noduri.push_back(sursa);
    traseulCurent.distantaTotala = 0;
    traseulCurent.timpTotal = 0;
    vizitat[sursa] = true;

    backtracking(sursa, destinatie, vizitat, traseulCurent, toateTraseele);

    sort(toateTraseele.begin(), toateTraseele.end(),
         [](const Traseu& a, const Traseu& b) {
             return a.distantaTotala < b.distantaTotala;
         });
    return toateTraseele;
}

// Calculeaza timpul de parcurgere in ore
double Graf::calculeazaTimp(const Traseu& traseu) const {
    double timpTotal = 0;
    for (size_t i = 0; i < traseu.tipuriDrum.size(); i++) {
        int nod1 = traseu.noduri[i];
        int nod2 = traseu.noduri[i + 1];
        for (const auto& drum : listaAdiacenta[nod1]) {
            if (drum.destinatie == nod2 && drum.tipDrum == traseu.tipuriDrum[i]) {
                timpTotal += drum.distanta / drum.vitezaMax;
                break;
            }
        }
    }
    return timpTotal;
}

// Afiseaza un traseu formatat frumos
void Graf::afiseazaTraseu(const Traseu& traseu) const {
    if (traseu.noduri.empty()) {
        cout << "  Nu s-a gasit niciun traseu!" << endl;
        return;
    }
    cout << "  ";
    for (size_t i = 0; i < traseu.noduri.size(); i++) {
        cout << orase[traseu.noduri[i]].nume;
        if (i < traseu.noduri.size() - 1) cout << " -> ";
    }
    cout << " (" << fixed << setprecision(0) << traseu.distantaTotala << " km)" << endl;
}

// Afiseaza detaliile fiecarui segment dintr-un traseu
void Graf::afiseazaDetaliiTraseu(const Traseu& traseu) const {
    if (traseu.noduri.size() < 2) {
        cout << "  Traseul nu are segmente!" << endl;
        return;
    }

    cout << endl;
    cout << C_GRAY;
    cout << "  +----+------------------------+------------------------+---------+---------------------------+-----------+" << endl;
    cout << "  | Nr | De la                  | La                     | Dist.   | Tip drum                  | Viteza    |" << endl;
    cout << "  +----+------------------------+------------------------+---------+---------------------------+-----------+" << endl;
    cout << C_RESET;

    for (size_t i = 0; i < traseu.tipuriDrum.size(); i++) {
        int nod1 = traseu.noduri[i];
        int nod2 = traseu.noduri[i + 1];
        double dist = 0;
        int viteza = 0;
        for (const auto& drum : listaAdiacenta[nod1]) {
            if (drum.destinatie == nod2 && drum.tipDrum == traseu.tipuriDrum[i]) {
                dist = drum.distanta;
                viteza = drum.vitezaMax;
                break;
            }
        }
        string tipComplet = traseu.tipuriDrum[i] + " - " + getNumeTipDrum(traseu.tipuriDrum[i]);
        cout << "  " << C_GRAY << "|" << C_RESET << " " << setw(2) << (i + 1)
             << " " << C_GRAY << "|" << C_RESET << " "
             << C_GREEN << setw(22) << left << orase[nod1].nume << C_RESET
             << " " << C_GRAY << "|" << C_RESET << " "
             << C_GREEN << setw(22) << left << orase[nod2].nume << C_RESET
             << " " << C_GRAY << "|" << C_RESET
             << right << " " << C_CYAN << C_BOLD << setw(5) << fixed << setprecision(0) << dist << " km" << C_RESET
             << " " << C_GRAY << "|" << C_RESET << " "
             << setw(25) << left << tipComplet
             << " " << C_GRAY << "|" << C_RESET
             << right << " " << setw(6) << viteza << " km/h"
             << " " << C_GRAY << "|" << C_RESET << endl;
    }

    cout << C_GRAY;
    cout << "  +----+------------------------+------------------------+---------+---------------------------+-----------+" << endl;
    cout << C_RESET;
    cout << "  " << C_GRAY << "|" << C_RESET << " Total: " << C_CYAN << C_BOLD
         << setw(92) << right << to_string((int)traseu.distantaTotala) + " km" << C_RESET
         << " " << C_GRAY << "|" << C_RESET << endl;
    cout << C_GRAY;
    cout << "  +----+------------------------+------------------------+---------+---------------------------+-----------+" << endl;
    cout << C_RESET;
}

// Returneaza numele unui oras dupa index
string Graf::getNumeOras(int index) const {
    if (index >= 0 && index < nrOrase) return orase[index].nume;
    return "Necunoscut";
}

// Returneaza numarul de orase
int Graf::getNrOrase() const {
    return nrOrase;
}

// Returneaza numarul de conexiuni directe ale unui oras
int Graf::getNrConexiuni(int index) const {
    if (index >= 0 && index < nrOrase)
        return (int)listaAdiacenta[index].size();
    return 0;
}

// ═══════════════════════════════════════════════════════════════════
// Genereaza fisierul HTML cu harta interactiva a traseelor pe Cuba
// ═══════════════════════════════════════════════════════════════════
void Graf::genereazaHTML(const vector<Traseu>& trasee, int sursa, int destinatie,
                         const string& titlu, int traseuEvidentiiat) {
    ofstream fout("rezultat.html");
    if (!fout.is_open()) {
        cerr << "  [!] Nu s-a putut crea fisierul rezultat.html!" << endl;
        return;
    }

    // --- Constante proiectie GPS -> SVG ---
    const double MIN_LON = -85.0, MAX_LON = -74.0;
    const double MIN_LAT = 19.5,  MAX_LAT = 23.5;
    const double SVG_W = 1200.0,  SVG_H = 500.0;
    const double PAD = 60.0;
    const double W = SVG_W - 2.0 * PAD;
    const double H = SVG_H - 2.0 * PAD;

    auto toX = [&](double lon) -> double {
        return PAD + (lon - MIN_LON) / (MAX_LON - MIN_LON) * W;
    };
    auto toY = [&](double lat) -> double {
        return PAD + (MAX_LAT - lat) / (MAX_LAT - MIN_LAT) * H;
    };
    auto fmt = [](double v, int prec) -> string {
        ostringstream oss;
        oss << fixed << setprecision(prec) << v;
        return oss.str();
    };
    auto formatTimp = [](double ore) -> string {
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
    };
    auto culoareDrum = [](const string& tip) -> string {
        if (tip == "A")  return "#e74c3c";
        if (tip == "CN") return "#3498db";
        if (tip == "CR") return "#f39c12";
        if (tip == "CL") return "#95a5a6";
        return "#ffffff";
    };
    auto grosimeDrum = [](const string& tip) -> string {
        if (tip == "A")  return "4";
        if (tip == "CN") return "3";
        if (tip == "CR") return "3";
        if (tip == "CL") return "2";
        return "2";
    };

    // Conturul insulei Cuba
    vector<pair<double,double>> contur = {
        {-84.95,21.76},{-84.28,22.05},{-83.70,22.15},{-83.37,22.22},
        {-82.80,22.40},{-82.47,22.45},{-82.10,22.60},{-81.83,22.68},
        {-81.50,23.00},{-81.17,23.05},{-80.63,23.10},{-80.25,22.96},
        {-79.83,22.87},{-79.47,22.75},{-79.14,22.58},{-78.80,22.40},
        {-78.43,22.15},{-78.04,22.08},{-77.67,21.85},{-77.28,21.75},
        {-76.84,21.58},{-76.34,21.24},{-75.83,20.73},{-75.62,20.47},
        {-75.32,20.12},{-74.88,20.04},{-74.88,19.97},{-75.10,19.88},
        {-75.63,19.97},{-76.15,20.10},{-76.58,20.28},{-77.02,20.45},
        {-77.52,20.67},{-77.92,21.08},{-78.25,21.32},{-78.68,21.55},
        {-79.18,21.58},{-79.54,21.73},{-79.92,21.72},{-80.35,21.82},
        {-80.82,21.92},{-81.25,22.10},{-81.67,22.22},{-81.93,22.07},
        {-82.27,22.20},{-82.60,22.07},{-82.93,22.08},{-83.35,22.00},
        {-83.76,21.92},{-84.10,21.84},{-84.50,21.78},{-84.95,21.76}
    };
    string polyPoints;
    for (const auto& p : contur) {
        polyPoints += fmt(toX(p.first), 1) + "," + fmt(toY(p.second), 1) + " ";
    }

    // Offset-uri etichete
    struct LabelOff { double dx; double dy; const char* anchor; };
    vector<LabelOff> lbl = {
        {-12, -10, "end"},    {10, -10, "start"},  {10, 18, "start"},
        {10, 5, "start"},     {-12, 5, "end"},     {-12, 18, "end"},
        {10, -10, "start"},   {10, -10, "start"},  {10, -10, "start"},
        {10, -10, "start"},   {-12, 18, "end"},    {-12, 5, "end"},
        {10, 5, "start"},     {-12, -10, "end"},   {10, 18, "start"},
    };

    vector<string> culoriMultiple = {"#e74c3c","#3498db","#2ecc71","#f39c12","#9b59b6"};
    bool modMultiplu = (traseuEvidentiiat == -1);

    // ══════════════════════════════════
    //  SECTIUNEA 1: HTML HEAD + CSS
    // ══════════════════════════════════
    fout << R"html(<!DOCTYPE html>
<html lang="ro">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Cuba Navigator - Rezultat</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{
    font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;
    background:linear-gradient(135deg,#1a1a2e 0%,#16213e 50%,#1a1a2e 100%);
    color:#e0e0e0;min-height:100vh;
}
.container{max-width:1300px;margin:0 auto;padding:20px}

/* Animatii fade-in */
@keyframes fadeDown{from{opacity:0;transform:translateY(-20px)}to{opacity:1;transform:translateY(0)}}
@keyframes fadeUp{from{opacity:0;transform:translateY(20px)}to{opacity:1;transform:translateY(0)}}
@keyframes drawLine{from{stroke-dashoffset:var(--len)}to{stroke-dashoffset:0}}

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
.map-card svg{width:100%;height:auto;display:block;border-radius:8px}

/* Animatie traseu desenat progresiv */
.route-line{
    stroke-dasharray:var(--len);stroke-dashoffset:var(--len);
    animation:drawLine 1.5s ease-out forwards;
}

.legend{
    position:absolute;top:30px;right:30px;
    background:rgba(22,33,62,0.93);
    backdrop-filter:blur(8px);-webkit-backdrop-filter:blur(8px);
    border:1px solid rgba(14,165,233,0.2);
    border-radius:10px;padding:14px 18px;
    box-shadow:0 4px 20px rgba(0,0,0,0.35);
}
.legend h4{color:#0ea5e9;margin-bottom:10px;font-size:0.82em;text-transform:uppercase;letter-spacing:1.5px}
.legend-item{display:flex;align-items:center;margin-bottom:7px;font-size:0.84em;color:#cbd5e1}
.legend-line{width:30px;height:4px;border-radius:2px;margin-right:10px;flex-shrink:0}
.legend-dot{width:12px;height:12px;border-radius:50%;margin-right:10px;flex-shrink:0}

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

/* Tooltip SVG */
.city-group{cursor:pointer}
.city-group:hover .city-tooltip{display:block}
.city-tooltip{
    display:none;pointer-events:none;
}

@media(max-width:768px){
    .stats-row{grid-template-columns:repeat(2,1fr)}
    .legend{position:static;margin:10px auto;max-width:280px}
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
    //  SECTIUNEA 3: HARTA SVG
    // ══════════════════════════════════
    fout << "<div class=\"map-card\">\n";
    fout << "<svg viewBox=\"0 0 1200 500\" xmlns=\"http://www.w3.org/2000/svg\">\n";

    // Defs
    fout << R"svg(<defs>
<linearGradient id="ocean" x1="0%" y1="0%" x2="100%" y2="100%">
<stop offset="0%" style="stop-color:#0c2d48"/>
<stop offset="100%" style="stop-color:#14445e"/>
</linearGradient>
<filter id="glow"><feGaussianBlur stdDeviation="3" result="blur"/>
<feMerge><feMergeNode in="blur"/><feMergeNode in="SourceGraphic"/></feMerge></filter>
</defs>
)svg";

    // Fundal ocean
    fout << "<rect width=\"1200\" height=\"500\" fill=\"url(#ocean)\" rx=\"8\"/>\n";

    // Conturul insulei Cuba
    fout << "<polygon points=\"" << polyPoints
         << "\" fill=\"#2d6a4f\" fill-opacity=\"0.55\" "
         << "stroke=\"#52b788\" stroke-width=\"2\" stroke-linejoin=\"round\"/>\n";

    // Toate drumurile din graf ca linii subtiri de fundal
    for (int i = 0; i < nrOrase; i++) {
        for (const auto& drum : listaAdiacenta[i]) {
            if (drum.destinatie > i) {
                fout << "<line x1=\"" << fmt(toX(orase[i].longitudine), 1)
                     << "\" y1=\"" << fmt(toY(orase[i].latitudine), 1)
                     << "\" x2=\"" << fmt(toX(orase[drum.destinatie].longitudine), 1)
                     << "\" y2=\"" << fmt(toY(orase[drum.destinatie].latitudine), 1)
                     << "\" stroke=\"#475569\" stroke-width=\"1.5\" stroke-opacity=\"0.4\"/>\n";
            }
        }
    }

    // Traseele evidentiiate
    if (modMultiplu) {
        size_t nrPeHarta = min(trasee.size(), (size_t)5);
        for (int r = (int)nrPeHarta - 1; r >= 0; r--) {
            const Traseu& t = trasee[r];
            string culoare = culoriMultiple[r % culoriMultiple.size()];
            string opac = (r == 0) ? "1.0" : "0.55";
            string lw = (r == 0) ? "4.5" : "3";
            string filtru = (r == 0) ? " filter=\"url(#glow)\"" : "";
            for (size_t i = 0; i < t.tipuriDrum.size(); i++) {
                double x1 = toX(orase[t.noduri[i]].longitudine);
                double y1 = toY(orase[t.noduri[i]].latitudine);
                double x2 = toX(orase[t.noduri[i+1]].longitudine);
                double y2 = toY(orase[t.noduri[i+1]].latitudine);
                double segLen = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
                fout << "<line x1=\"" << fmt(x1,1) << "\" y1=\"" << fmt(y1,1)
                     << "\" x2=\"" << fmt(x2,1) << "\" y2=\"" << fmt(y2,1)
                     << "\" stroke=\"" << culoare << "\" stroke-width=\"" << lw
                     << "\" stroke-opacity=\"" << opac
                     << "\" stroke-linecap=\"round\"" << filtru
                     << " class=\"route-line\" style=\"--len:" << fmt(segLen,0) << "\"/>\n";
            }
        }
    } else if (traseuEvidentiiat >= 0 && traseuEvidentiiat < (int)trasee.size()) {
        const Traseu& t = trasee[traseuEvidentiiat];
        for (size_t i = 0; i < t.tipuriDrum.size(); i++) {
            double x1 = toX(orase[t.noduri[i]].longitudine);
            double y1 = toY(orase[t.noduri[i]].latitudine);
            double x2 = toX(orase[t.noduri[i+1]].longitudine);
            double y2 = toY(orase[t.noduri[i+1]].latitudine);
            double segLen = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
            fout << "<line x1=\"" << fmt(x1,1) << "\" y1=\"" << fmt(y1,1)
                 << "\" x2=\"" << fmt(x2,1) << "\" y2=\"" << fmt(y2,1)
                 << "\" stroke=\"" << culoareDrum(t.tipuriDrum[i])
                 << "\" stroke-width=\"" << grosimeDrum(t.tipuriDrum[i])
                 << "\" stroke-linecap=\"round\" filter=\"url(#glow)\""
                 << " class=\"route-line\" style=\"--len:" << fmt(segLen,0)
                 << ";animation-delay:" << fmt(i * 0.25, 2) << "s\"/>\n";
        }
    }

    // Cercuri si etichete pentru orase (cu tooltip SVG <title>)
    for (int i = 0; i < nrOrase; i++) {
        double cx = toX(orase[i].longitudine);
        double cy = toY(orase[i].latitudine);
        bool esteSursa = (i == sursa);
        bool esteDest  = (i == destinatie);
        int nrCon = (int)listaAdiacenta[i].size();

        // Grup cu tooltip
        fout << "<g class=\"city-group\">\n";
        fout << "<title>" << orase[i].nume
             << " (" << fmt(orase[i].latitudine,4) << ", " << fmt(orase[i].longitudine,4)
             << ") - " << nrCon << " conexiuni</title>\n";

        if (esteSursa || esteDest) {
            string col = esteSursa ? "#22c55e" : "#ef4444";
            fout << "<circle cx=\"" << fmt(cx,1) << "\" cy=\"" << fmt(cy,1)
                 << "\" fill=\"" << col << "\" opacity=\"0.25\">"
                 << "<animate attributeName=\"r\" values=\"9;18;9\" dur=\"2.5s\" repeatCount=\"indefinite\"/>"
                 << "<animate attributeName=\"opacity\" values=\"0.4;0.05;0.4\" dur=\"2.5s\" repeatCount=\"indefinite\"/>"
                 << "</circle>\n";
            fout << "<circle cx=\"" << fmt(cx,1) << "\" cy=\"" << fmt(cy,1)
                 << "\" r=\"9\" fill=\"" << col << "\" stroke=\"#fff\" stroke-width=\"2\"/>\n";
        } else {
            fout << "<circle cx=\"" << fmt(cx,1) << "\" cy=\"" << fmt(cy,1)
                 << "\" r=\"6\" fill=\"#94a3b8\" stroke=\"#1e293b\" stroke-width=\"1.5\"/>\n";
        }

        // Eticheta
        double dx = lbl[i].dx, dy = lbl[i].dy;
        fout << "<text x=\"" << fmt(cx + dx, 1) << "\" y=\"" << fmt(cy + dy, 1)
             << "\" text-anchor=\"" << lbl[i].anchor
             << "\" fill=\"#e2e8f0\" font-size=\"11\" font-weight=\"bold\" font-family=\"'Segoe UI',sans-serif\">"
             << orase[i].nume << "</text>\n";
        fout << "</g>\n";
    }

    fout << "</svg>\n";

    // Legenda
    fout << "<div class=\"legend\">\n";
    if (modMultiplu) {
        fout << "<h4>Trasee</h4>\n";
        size_t nrLegenda = min(trasee.size(), (size_t)5);
        for (size_t i = 0; i < nrLegenda; i++) {
            fout << "<div class=\"legend-item\">"
                 << "<div class=\"legend-line\" style=\"background:" << culoriMultiple[i % culoriMultiple.size()]
                 << (i == 0 ? "" : ";opacity:0.6") << "\"></div>"
                 << "Traseu " << (i + 1)
                 << " (" << fmt(trasee[i].distantaTotala, 0) << " km)</div>\n";
        }
    } else {
        fout << "<h4>Tipuri de drum</h4>\n";
        fout << "<div class=\"legend-item\"><div class=\"legend-line\" style=\"background:#e74c3c;height:4px\"></div>A &mdash; Autopista (100 km/h)</div>\n";
        fout << "<div class=\"legend-item\"><div class=\"legend-line\" style=\"background:#3498db;height:3px\"></div>CN &mdash; Carretera Nacional (80 km/h)</div>\n";
        fout << "<div class=\"legend-item\"><div class=\"legend-line\" style=\"background:#f39c12;height:3px\"></div>CR &mdash; Carretera Regional (60 km/h)</div>\n";
        fout << "<div class=\"legend-item\"><div class=\"legend-line\" style=\"background:#95a5a6;height:2px\"></div>CL &mdash; Carretera Local (40 km/h)</div>\n";
    }
    fout << "<div style=\"border-top:1px solid #2c3e50;margin:8px 0 6px\"></div>\n";
    fout << "<div class=\"legend-item\"><div class=\"legend-dot\" style=\"background:#22c55e;border:2px solid #fff\"></div>Plecare</div>\n";
    fout << "<div class=\"legend-item\"><div class=\"legend-dot\" style=\"background:#ef4444;border:2px solid #fff\"></div>Destinatie</div>\n";
    fout << "</div>\n";
    fout << "</div>\n\n";

    // ══════════════════════════════════
    //  SECTIUNEA 4: STATS PANEL
    // ══════════════════════════════════
    if (!modMultiplu && !trasee.empty()) {
        int idx = max(0, traseuEvidentiiat);
        const Traseu& t = trasee[idx];
        int nrSeg = (int)t.tipuriDrum.size();
        double vitezaMedie = (t.timpTotal > 0) ? t.distantaTotala / t.timpTotal : 0;

        fout << "<div class=\"stats-row\">\n";
        fout << "<div class=\"stat-card\"><span class=\"stat-icon\">&#128207;</span>"
             << "<span class=\"stat-value\">" << fmt(t.distantaTotala, 0) << " km</span>"
             << "<span class=\"stat-label\">Distanta totala</span></div>\n";
        fout << "<div class=\"stat-card\"><span class=\"stat-icon\">&#9201;</span>"
             << "<span class=\"stat-value\">" << formatTimp(t.timpTotal) << "</span>"
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
    fout << "<h2>" << titlu << ": " << orase[sursa].nume
         << " &#8594; " << orase[destinatie].nume << "</h2>\n";

    if (modMultiplu) {
        fout << "<p style=\"color:#94a3b8;margin-bottom:16px\">S-au gasit <strong style=\"color:#0ea5e9\">"
             << trasee.size() << "</strong> trasee posibile (sortate dupa distanta)</p>\n";
        if (trasee.size() > 5) {
            fout << "<p style=\"color:#64748b;margin-bottom:12px;font-size:0.9em\">"
                 << "Pe harta sunt afisate primele 5 trasee.</p>\n";
        }
        fout << "<div class=\"route-list\">\n";
        size_t nrAfis = min(trasee.size(), (size_t)30);
        for (size_t i = 0; i < nrAfis; i++) {
            string borderCol = (i < 5) ? culoriMultiple[i % culoriMultiple.size()] : "#2c3e50";
            fout << "<div class=\"route-item\" style=\"border-left-color:" << borderCol << "\">"
                 << "<span class=\"route-num\">" << (i + 1) << "</span>"
                 << "<span class=\"route-path\">";
            for (size_t j = 0; j < trasee[i].noduri.size(); j++) {
                fout << orase[trasee[i].noduri[j]].nume;
                if (j < trasee[i].noduri.size() - 1)
                    fout << " <span class=\"arrow\">&#8594;</span> ";
            }
            fout << "</span>"
                 << "<span class=\"route-dist\">" << fmt(trasee[i].distantaTotala, 0) << " km</span>"
                 << "</div>\n";
        }
        if (trasee.size() > 30) {
            fout << "<div class=\"route-more\">... si alte "
                 << (trasee.size() - 30) << " trasee</div>\n";
        }
        fout << "</div>\n";
    } else if (!trasee.empty()) {
        int idx = max(0, traseuEvidentiiat);
        const Traseu& t = trasee[idx];

        fout << "<div class=\"route-summary\">";
        for (size_t i = 0; i < t.noduri.size(); i++) {
            fout << "<strong>" << orase[t.noduri[i]].nume << "</strong>";
            if (i < t.noduri.size() - 1) fout << " &#8594; ";
        }
        fout << "<br><span class=\"dist\">" << fmt(t.distantaTotala, 0) << " km</span>"
             << " &nbsp;&bull;&nbsp; <span class=\"timp\">" << formatTimp(t.timpTotal) << "</span>"
             << "</div>\n";

        fout << "<table>\n<thead><tr>"
             << "<th>#</th><th>De la</th><th>La</th>"
             << "<th>Distanta</th><th>Tip drum</th><th>Viteza</th><th>Timp</th>"
             << "</tr></thead>\n<tbody>\n";

        for (size_t i = 0; i < t.tipuriDrum.size(); i++) {
            int n1 = t.noduri[i], n2 = t.noduri[i + 1];
            double dist = 0;
            int viteza = 0;
            for (const auto& drum : listaAdiacenta[n1]) {
                if (drum.destinatie == n2 && drum.tipDrum == t.tipuriDrum[i]) {
                    dist = drum.distanta;
                    viteza = drum.vitezaMax;
                    break;
                }
            }
            double timpSeg = (viteza > 0) ? dist / viteza : 0;
            fout << "<tr>"
                 << "<td>" << (i + 1) << "</td>"
                 << "<td>" << orase[n1].nume << "</td>"
                 << "<td>" << orase[n2].nume << "</td>"
                 << "<td>" << fmt(dist, 0) << " km</td>"
                 << "<td><span class=\"road-badge\" style=\"background:"
                 << culoareDrum(t.tipuriDrum[i]) << "\">" << t.tipuriDrum[i] << "</span>"
                 << getNumeTipDrum(t.tipuriDrum[i]) << "</td>"
                 << "<td>" << viteza << " km/h</td>"
                 << "<td>" << formatTimp(timpSeg) << "</td>"
                 << "</tr>\n";
        }
        fout << "</tbody>\n";
        fout << "<tr class=\"total-row\">"
             << "<td colspan=\"3\">TOTAL</td>"
             << "<td>" << fmt(t.distantaTotala, 0) << " km</td>"
             << "<td colspan=\"2\"></td>"
             << "<td>" << formatTimp(t.timpTotal) << "</td>"
             << "</tr>\n";
        fout << "</table>\n";
    }

    fout << "</div>\n\n";

    // ══════════════════════════════════
    //  SECTIUNEA 6: FOOTER
    // ══════════════════════════════════
    fout << R"html(<footer>
<span>Cuba Navigator</span> v1.0 &mdash; Proiect Backtracking C++
<span class="sep">|</span> Algoritm: DFS cu Backtracking
<span class="sep">|</span> Graf: 15 noduri, 20 muchii
</footer>
</div>
</body>
</html>
)html";

    fout.close();
}
