#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <chrono>
#include <cstdlib>
#include "graf.h"

using namespace std;

// === Coduri culori ANSI ===
const string RESET  = "\033[0m";
const string BOLD   = "\033[1m";
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE   = "\033[34m";
const string CYAN   = "\033[36m";
const string GRAY   = "\033[90m";

// Afiseaza bannerul principal al aplicatiei
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

// Afiseaza meniul principal
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

// Afiseaza un separator vizual
void separator() {
    cout << GRAY << "  ──────────────────────────────────────────────────" << RESET << endl;
}

// Citeste si valideaza un numar de oras (1..nrOrase), returneaza indexul (0-based)
int citesteOras(const string& mesaj, int nrOrase) {
    int oras;
    while (true) {
        cout << "  " << mesaj;
        cin >> oras;
        if (cin.fail() || oras < 1 || oras > nrOrase) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "  [!] Numar invalid! Introduceti un numar intre 1 si " << nrOrase << "." << RESET << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return oras - 1;
        }
    }
}

// Formateaza timpul din ore in "X ore si Y minute"
string formateazaTimp(double ore) {
    int oreIntregi = (int)ore;
    int minute = (int)((ore - oreIntregi) * 60 + 0.5);
    if (minute == 60) {
        oreIntregi++;
        minute = 0;
    }

    string rezultat;
    if (oreIntregi > 0) {
        rezultat += to_string(oreIntregi) + (oreIntregi == 1 ? " ora" : " ore");
    }
    if (minute > 0) {
        if (!rezultat.empty()) rezultat += " si ";
        rezultat += to_string(minute) + (minute == 1 ? " minut" : " minute");
    }
    if (rezultat.empty()) {
        rezultat = "sub 1 minut";
    }
    return rezultat;
}

// Deschide fisierul HTML in browser-ul implicit
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

int main() {
    // Initializam graful cu datele Cubei
    Graf cuba;
    cuba.initializeazaCuba();

    // Afisam bannerul
    afiseazaBanner();

    // Bucla principala a meniului
    int optiune;
    bool rulare = true;

    while (rulare) {
        // Afisam lista de orase si meniul
        cuba.afiseazaOrase();
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

            int sursa = citesteOras("Oras de plecare (numar): ", cuba.getNrOrase());
            int dest  = citesteOras("Oras de destinatie (numar): ", cuba.getNrOrase());

            if (sursa == dest) {
                cout << RED << "  [!] Sursa si destinatia sunt identice!" << RESET << endl;
                break;
            }

            cout << endl;
            cout << YELLOW << "  Se calculeaza traseele..." << RESET << endl;
            auto start = chrono::high_resolution_clock::now();
            Traseu optim = cuba.gasesteTraseuMinim(sursa, dest);
            auto end = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            if (optim.noduri.empty()) {
                cout << RED << "  [!] Nu exista niciun traseu intre aceste localitati!" << RESET << endl;
            } else {
                cout << GREEN << "  Calculat in " << ms << " ms!" << RESET << endl;
                cout << endl;
                cout << "  Traseul optim (distanta minima):" << endl;
                cout << "  " << CYAN << BOLD;
                cuba.afiseazaTraseu(optim);
                cout << RESET;

                // Generare HTML
                cuba.genereazaHTML({optim}, sursa, dest, "Distanta minima", 0);
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

            int sursa = citesteOras("Oras de plecare (numar): ", cuba.getNrOrase());
            int dest  = citesteOras("Oras de destinatie (numar): ", cuba.getNrOrase());

            if (sursa == dest) {
                cout << RED << "  [!] Sursa si destinatia sunt identice!" << RESET << endl;
                break;
            }

            cout << endl;
            cout << YELLOW << "  Se calculeaza traseele..." << RESET << endl;
            auto start = chrono::high_resolution_clock::now();
            vector<Traseu> trasee = cuba.gasesteToateTraseele(sursa, dest);
            auto end = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            if (trasee.empty()) {
                cout << RED << "  [!] Nu exista niciun traseu intre aceste localitati!" << RESET << endl;
            } else {
                cout << GREEN << "  S-au gasit " << trasee.size() << " trasee in " << ms << " ms!" << RESET << endl;
                cout << endl;

                // Limita de afisare in consola
                const size_t LIMITA_CONSOLA = 10;
                size_t deAfisat = min(trasee.size(), LIMITA_CONSOLA);
                for (size_t i = 0; i < deAfisat; i++) {
                    cout << "  " << setw(3) << (i + 1) << ". ";
                    for (size_t j = 0; j < trasee[i].noduri.size(); j++) {
                        cout << GREEN << cuba.getNumeOras(trasee[i].noduri[j]) << RESET;
                        if (j < trasee[i].noduri.size() - 1) cout << GRAY << " -> " << RESET;
                    }
                    cout << " " << CYAN << BOLD << "(" << fixed << setprecision(0)
                         << trasee[i].distantaTotala << " km)" << RESET << endl;
                }
                if (trasee.size() > LIMITA_CONSOLA) {
                    cout << GRAY << "  ... si inca " << (trasee.size() - LIMITA_CONSOLA)
                         << " trasee (vezi HTML pentru lista completa)" << RESET << endl;
                }

                // Generare HTML
                cuba.genereazaHTML(trasee, sursa, dest, "Toate traseele posibile", -1);
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

            int sursa = citesteOras("Oras de plecare (numar): ", cuba.getNrOrase());
            int dest  = citesteOras("Oras de destinatie (numar): ", cuba.getNrOrase());

            if (sursa == dest) {
                cout << RED << "  [!] Sursa si destinatia sunt identice!" << RESET << endl;
                break;
            }

            cout << endl;
            cout << YELLOW << "  Se calculeaza traseele..." << RESET << endl;
            auto start = chrono::high_resolution_clock::now();
            vector<Traseu> trasee = cuba.gasesteToateTraseele(sursa, dest);
            auto end = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            if (trasee.empty()) {
                cout << RED << "  [!] Nu exista niciun traseu intre aceste localitati!" << RESET << endl;
                break;
            }

            cout << GREEN << "  S-au gasit " << trasee.size() << " trasee in " << ms << " ms!" << RESET << endl;
            cout << endl;

            // Afisam traseele numerotate (limitat la 20 in consola)
            size_t deAfisat = min(trasee.size(), (size_t)20);
            for (size_t i = 0; i < deAfisat; i++) {
                cout << "  " << setw(3) << (i + 1) << ". ";
                for (size_t j = 0; j < trasee[i].noduri.size(); j++) {
                    cout << GREEN << cuba.getNumeOras(trasee[i].noduri[j]) << RESET;
                    if (j < trasee[i].noduri.size() - 1) cout << GRAY << " -> " << RESET;
                }
                cout << " " << CYAN << "(" << fixed << setprecision(0)
                     << trasee[i].distantaTotala << " km)" << RESET << endl;
            }
            if (trasee.size() > 20) {
                cout << GRAY << "  ... si inca " << (trasee.size() - 20) << " trasee." << RESET << endl;
            }

            // Utilizatorul alege un traseu
            cout << endl;
            int alegere;
            while (true) {
                cout << "  Alegeti numarul traseului pentru detalii: ";
                cin >> alegere;
                if (cin.fail() || alegere < 1 || alegere > (int)trasee.size()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "  [!] Numar invalid! Alegeti intre 1 si " << trasee.size() << "." << RESET << endl;
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
            }

            // Afisam detaliile traseului ales
            cout << endl;
            cout << BOLD << "  Detalii traseu #" << alegere << ":" << RESET << endl;
            cuba.afiseazaDetaliiTraseu(trasee[alegere - 1]);

            // Generare HTML
            cuba.genereazaHTML({trasee[alegere - 1]}, sursa, dest, "Tipuri de drum", 0);
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

            int sursa = citesteOras("Oras de plecare (numar): ", cuba.getNrOrase());
            int dest  = citesteOras("Oras de destinatie (numar): ", cuba.getNrOrase());

            if (sursa == dest) {
                cout << RED << "  [!] Sursa si destinatia sunt identice!" << RESET << endl;
                break;
            }

            cout << endl;
            cout << YELLOW << "  Se calculeaza traseele..." << RESET << endl;
            auto start = chrono::high_resolution_clock::now();
            Traseu optim = cuba.gasesteTraseuMinim(sursa, dest);
            auto end = chrono::high_resolution_clock::now();
            long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            if (optim.noduri.empty()) {
                cout << RED << "  [!] Nu exista niciun traseu intre aceste localitati!" << RESET << endl;
            } else {
                cout << GREEN << "  Calculat in " << ms << " ms!" << RESET << endl;
                cout << endl;
                cout << "  Traseul optim (distanta minima):" << endl;
                cout << "  " << CYAN << BOLD;
                cuba.afiseazaTraseu(optim);
                cout << RESET;
                cout << endl;
                cout << "  " << CYAN << BOLD << "Distanta totala: " << fixed << setprecision(0)
                     << optim.distantaTotala << " km" << RESET << endl;
                cout << "  " << CYAN << BOLD << "Timp estimat:    " << formateazaTimp(optim.timpTotal) << RESET << endl;

                // Afisam si detaliile pe segmente
                cuba.afiseazaDetaliiTraseu(optim);

                // Comparatie moduri de transport
                cout << endl;
                double distKm = optim.distantaTotala;
                cout << BOLD << "  Comparatie moduri de transport:" << RESET << endl;
                cout << "  " << CYAN  << "Cu masina (viteza max): " << formateazaTimp(optim.timpTotal) << RESET << endl;
                cout << "  " << GREEN << "Cu bicicleta (15 km/h): " << formateazaTimp(distKm / 15.0) << RESET << endl;
                cout << "  " << YELLOW << "Pe jos (5 km/h):        " << formateazaTimp(distKm / 5.0) << RESET << endl;

                // Generare HTML
                cuba.genereazaHTML({optim}, sursa, dest, "Timpul de parcurgere", 0);
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
