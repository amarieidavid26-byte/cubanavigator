# Cuba Navigator — Fragmente de Cod pentru Prezentare
## Cod copy-paste din main.cpp cu explicații în română

---

## 1. Declararea variabilelor globale (liniile 30-54)

```cpp
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
```

**Explicație:** Toate datele sunt globale — accesibile din orice funcție. Matricea `distanta[15][15]` stochează distanțele între orașe (0 = nu există drum). Vectorul `x[100]` este vectorul soluție al BKT, iar `vizitat[15]` marchează orașele deja parcurse.

---

## 2. initializeazaDate() — exemple representative (liniile 66-113)

```cpp
void initializeazaDate() {
    // Resetare matrice
    for (int i = 0; i < NR_ORASE; i++)
        for (int j = 0; j < NR_ORASE; j++) {
            distanta[i][j] = 0;
            tipDrum[i][j] = "";
            vitezaMax[i][j] = 0;
        }

    // Orase cu coordonate GPS reale
    numeOrase[0]  = "Havana";       latitudine[0]  = 23.1136; longitudine[0]  = -82.3666;
    numeOrase[2]  = "Matanzas";     latitudine[2]  = 23.0510; longitudine[2]  = -81.5775;
    numeOrase[10] = "Santiago de Cuba"; latitudine[10] = 20.0247; longitudine[10] = -75.8219;

    // Drumuri bidirectionale (20 drumuri)
    adaugaDrum(0,  2,  102, "A",  100);   // Havana - Matanzas (Autopista, 100 km/h)
    adaugaDrum(0,  13, 162, "CN", 80);    // Havana - Pinar del Rio (Carretera Nacional)
    adaugaDrum(2,  3,  195, "A",  100);   // Matanzas - Santa Clara (Autopista)
    adaugaDrum(4,  5,  80,  "CR", 60);    // Cienfuegos - Trinidad (Carretera Regional)
    // ... alte 16 drumuri
}
```

**Explicație:** Funcția inițializează toate datele: resetează matricea la 0, apoi setează numele și coordonatele GPS pentru fiecare oraș, și adaugă cele 20 de drumuri bidirecționale cu distanță, tip și viteză maximă.

---

## 3. Funcția INIT (liniile 121-123)

```cpp
void INIT(int k) {
    x[k] = -1;
}
```

**Explicație:** Pregătește poziția `k` din vectorul soluție. Valoarea -1 înseamnă "încă nu am încercat niciun oraș" — prima incrementare (`x[k]++`) va da 0, adică primul oraș.

---

## 4. Funcția EXISTA (liniile 125-128)

```cpp
int EXISTA(int k) {
    if (x[k] < NR_ORASE - 1) return 1;
    return 0;
}
```

**Explicație:** Verifică dacă mai sunt orașe de încercat la poziția `k`. Orașele sunt numerotate 0-14, deci dacă `x[k] < 14`, mai avem ce încerca. Returnează 1 (da) sau 0 (nu).

---

## 5. Funcția VALID (liniile 130-146)

```cpp
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
```

**Explicație:** Cele 3 condiții de validitate: (1) trebuie să existe drum direct, (2) orașul nu trebuie să fie deja vizitat (evităm cicluri), (3) orașul nu trebuie să fie sursa (nu ne întoarcem la start). Dacă oricare condiție nu e îndeplinită, returnează 0 (invalid).

---

## 6. Funcția SOLUTIE (liniile 148-151)

```cpp
int SOLUTIE(int k) {
    if (x[k] == destinatia) return 1;
    return 0;
}
```

**Explicație:** Verifică dacă am ajuns la destinație. Dacă orașul curent (`x[k]`) este orașul destinație, am găsit un traseu complet.

---

## 7. Funcția TIPAR (liniile 153-181)

```cpp
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
```

**Explicație:** Salvează traseul găsit în matricea `trasee[][]`. Calculează distanța totală (suma km-ilor pe fiecare segment) și timpul total (suma distanță/viteză pe fiecare segment). Formula timp: `t = d/v` pentru fiecare segment.

---

## 8. Funcția BKT (liniile 183-214)

```cpp
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
```

**Explicație:** Bucla principală a algoritmului. Începe de la k=1, marcând sursa ca vizitată. În `while(k>0)`: încearcă următorul oraș (`x[k]++`), verifică validitatea, și fie avansează (k++), fie face backtrack (k--). Când găsește o soluție, o salvează și continuă căutarea.

---

## 9. Codul afișării meniului (liniile 323-333)

```cpp
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
```

**Explicație:** Afișează meniul principal cu culori ANSI: galben pentru opțiuni, roșu pentru ieșire, gri pentru borduri. Codurile ANSI (`\033[33m` etc.) colorează textul în terminal.

---

## 10. Calculul timpului — formateazaTimp() (liniile 275-287)

```cpp
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
```

**Explicație:** Convertește un număr de ore (ex: 1.85) în text lizibil ("1 ora si 51 minute"). Tratează corect pluralul: "1 ora" vs "2 ore", "1 minut" vs "30 minute".

---

## 11. genereazaHTML() — deschiderea fișierului și structura HTML (liniile 445-468)

```cpp
void genereazaHTML(string titlu, int traseuEvidentiiat) {
    ofstream fout("rezultat.html");
    if (!fout.is_open()) {
        cerr << "  [!] Nu s-a putut crea fisierul rezultat.html!" << endl;
        return;
    }

    bool modMultiplu = (traseuEvidentiiat == -1);

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
```

**Explicație:** Funcția deschide fișierul `rezultat.html` cu `ofstream`, apoi scrie cod HTML, CSS și JavaScript. Folosește raw string literals (`R"html(...)html"`) pentru a scrie blocuri mari de HTML fără probleme cu ghilimelele. Parametrul `traseuEvidentiiat` controlează modul: -1 = trasee multiple, >=0 = traseu unic.

---

## 12. Comparația moduri de transport (liniile 1162-1166)

```cpp
double distKm = distanteTrasee[0];
cout << BOLD << "  Comparatie moduri de transport:" << RESET << endl;
cout << "  " << CYAN  << "Cu masina (viteza max): " << formateazaTimp(timpTrasee[0]) << RESET << endl;
cout << "  " << GREEN << "Cu bicicleta (15 km/h): " << formateazaTimp(distKm / 15.0) << RESET << endl;
cout << "  " << YELLOW << "Pe jos (5 km/h):        " << formateazaTimp(distKm / 5.0) << RESET << endl;
```

**Explicație:** Calculează timpul pentru 3 moduri de transport. Timpul cu mașina (`timpTrasee[0]`) este precis — calculat pe fiecare segment cu viteza specifică tipului de drum. Bicicleta (15 km/h) și mersul pe jos (5 km/h) folosesc formule simple: `timp = distanță / viteză`.

---

## 13. Măsurarea timpului cu chrono (liniile 962-966)

```cpp
auto start = chrono::high_resolution_clock::now();
BKT();
sortTrasee();
auto end = chrono::high_resolution_clock::now();
long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

cout << GREEN << "  Calculat in " << ms << " ms!" << RESET << endl;
```

**Explicație:** Folosește biblioteca `<chrono>` din C++11 pentru a măsura cu precizie cât durează algoritmul. `high_resolution_clock` oferă cea mai bună rezoluție disponibilă pe sistem. Rezultatul este afișat în milisecunde.

---

## 14. Funcția adaugaDrum() (linia 60-64)

```cpp
void adaugaDrum(int o1, int o2, int dist, string tip, int viteza) {
    distanta[o1][o2] = dist;    distanta[o2][o1] = dist;
    tipDrum[o1][o2] = tip;      tipDrum[o2][o1] = tip;
    vitezaMax[o1][o2] = viteza; vitezaMax[o2][o1] = viteza;
}
```

**Explicație:** Adaugă un drum bidirectional între două orașe. Setează distanța, tipul de drum și viteza maximă în ambele direcții ale matricei (de la o1 la o2 și de la o2 la o1).

---

## 15. Inițializarea hărții Leaflet (liniile 606-614)

```cpp
fout << "var map=L.map('map',{center:[21.5,-79.5],zoom:7});\n";
fout << "var darkLayer=L.tileLayer("
     << "'https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png',"
     << "{attribution:'© OpenStreetMap © CARTO',maxZoom:19});\n";
fout << "var satelliteLayer=L.tileLayer("
     << "'https://server.arcgisonline.com/ArcGIS/rest/services/"
     << "World_Imagery/MapServer/tile/{z}/{y}/{x}',"
     << "{attribution:'© Esri, Maxar',maxZoom:19});\n";
fout << "darkLayer.addTo(map);\n";
fout << "L.control.layers("
     << "{'Dark':darkLayer,'Satelit':satelliteLayer}).addTo(map);\n";
```

**Explicație:** Codul C++ scrie cod JavaScript care va rula în browser. Creează harta Leaflet centrată pe Cuba, cu două straturi: dark (CARTO) și satelit (Esri/ArcGIS). Controlul de straturi permite switch-ul între cele două vizualizări.
