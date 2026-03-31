# Cuba Navigator — Conținut Prezentare
## Ghid complet pentru crearea prezentării PowerPoint / Canva (~19 slide-uri)

---

## SLIDE 1 — Coperta

**Titlu:** "Cuba Navigator — Sistem de Navigare Inteligent"

**Subtitlu:** "Proiect Backtracking C++ | Clasa a XI-a"

**Autor:** David Amariei

**Sugestie vizuală:** Screenshot cu harta din `rezultat.html` (vizualizare satelit, zoom pe Cuba cu toate traseele vizibile)

---

## SLIDE 2 — Ce este Cuba Navigator?

**Bullet 1:** O aplicație care găsește trasee între orașe din Cuba

**Bullet 2:** Folosește algoritmul Backtracking pentru a explora TOATE căile posibile

**Bullet 3:** Generează o hartă interactivă reală cu rezultatele

**Sugestie vizuală:** Split screen — consolă (cu meniul colorat) în stânga, hartă Leaflet în dreapta

**Note pentru prezentator:**
> "Cuba Navigator este un program C++ care simulează un GPS. Utilizatorul alege două orașe, iar programul găsește toate traseele posibile între ele, le sortează, și afișează rezultatele atât în consolă cât și pe o hartă interactivă reală. Programul are 1199 de linii de cod, 15 orașe, 20 de drumuri și 21 de funcții."

---

## SLIDE 3 — Tehnologii folosite (EXPLICAT SIMPLU)

**Conținut pentru slide — explică FIECARE concept simplu:**

### C++
Limbajul de programare în care am scris logica aplicației. E ca "creierul" — face toate calculele.

### HTML
Limbajul care creează pagini web. Programul nostru C++ GENEREAZĂ un fișier HTML — adică scrie automat codul unei pagini web care afișează harta.

### Ce este un API?
API (Application Programming Interface) = o "ușă de comunicare" între două programe. Imaginați-vă un chelner la restaurant: voi (programul) dați comanda chelnerului (API-ul), el merge la bucătărie (serverul) și vă aduce mâncarea (datele). Nu intrați voi în bucătărie — comunicați prin chelner.

### Ce este un Endpoint?
Endpoint = adresa exactă la care trimitem cererea. Dacă API-ul e restaurantul, endpoint-ul e "masa 5, secțiunea terasă". În cazul nostru: `https://server.arcgisonline.com/...` e endpoint-ul de unde luăm imaginile satelit ale hărții.

### Ce este Leaflet.js?
O bibliotecă JavaScript (cod scris de alții pe care îl folosim noi) care știe să afișeze hărți interactive în browser. Noi îi spunem: "pune un punct la coordonatele Havana" și ea se ocupă de tot.

### Ce este OpenStreetMap?
O hartă gratuită a lumii, făcută de voluntari (ca Wikipedia, dar pentru hărți). Leaflet.js ia "bucățile" de hartă de la OpenStreetMap și le afișează.

### Ce sunt tile-urile?
Harta nu se încarcă toată odată — vine în "plăcuțe" (tiles) mici, ca un puzzle. Când faci zoom, se încarcă plăcuțe mai detaliate. De asta ai nevoie de internet.

### Ce sunt coordonatele GPS?
Două numere (latitudine și longitudine) care identifică EXACT un punct pe Pământ. Ex: Havana = (23.1136, -82.3666). Latitudinea = cât de sus/jos (N/S), Longitudinea = cât de stânga/dreapta (E/V).

**Sugestie vizuală:** Diagramă simplă cu săgeți:
```
C++ → generează → HTML → folosește → Leaflet.js → cere tile-uri de la → OpenStreetMap
```

**Note pentru prezentator:**
> "Fiecare tehnologie are un rol specific. C++ face calculele, HTML afișează rezultatele, Leaflet.js desenează harta, iar OpenStreetMap furnizează imaginile hărții. Toate lucrează împreună ca o echipă."

---

## SLIDE 4 — Structura datelor: Harta ca matrice

**Titlu:** "Cum îi spunem computerului cum arată harta Cubei?"

**Explicație:** "Folosim o MATRICE (tabel) unde rândul = orașul de plecare, coloana = orașul de sosire, valoarea = distanța în km"

**Exemplu cu 4 orașe (tabel mic):**

|                | Havana | Matanzas | Varadero | Pinar del Río |
|----------------|--------|----------|----------|---------------|
| **Havana**     | 0      | 102      | 0        | 162           |
| **Matanzas**   | 102    | 0        | 42       | 0             |
| **Varadero**   | 0      | 42       | 0        | 0             |
| **Pinar del Río** | 162 | 0        | 0        | 0             |

- `0` înseamnă **NU** există drum direct
- `102` înseamnă există drum de **102 km**

**Fragment de cod:**
```cpp
int distanta[NR_ORASE][NR_ORASE];    // 0 = fara drum
distanta[0][2] = 102;  // Havana → Matanzas = 102 km
distanta[2][0] = 102;  // Matanzas → Havana = 102 km (bidirectional)
```

**Informații complete:** Matricea reală are 15×15 = 225 de celule, din care 40 sunt nenule (20 drumuri × 2 direcții).

**Sugestie vizuală:** Tabel colorat — celulele cu 0 = gri deschis, celulele cu valori = verde

**Note pentru prezentator:**
> "Matricea de adiacență este modul în care reprezentăm graful în memorie. Fiecare drum este bidirectional — dacă poți merge de la Havana la Matanzas, poți merge și invers. De aceea matricea este simetrică. Funcția `adaugaDrum()` (linia 60) se ocupă automat de ambele direcții."

---

## SLIDE 5 — Cum funcționează Backtracking? (Concept)

**Titlu:** "Algoritmul care încearcă TOTUL"

**Analogie din viața reală:**
> "Imaginați-vă un labirint. Mergeți pe un coridor. Dacă ajungeți într-un fundal, vă ÎNTOARCEȚI la ultima intersecție și încercați altă cale. Backtracking = exact asta, dar automat. Computerul încearcă TOATE căile posibile și le memorează pe cele care ajung la destinație."

**Diagrama arbore (text — de recreat în Canva):**
```
                    Havana
                   /      \
            Matanzas    Pinar del Río
            /    \         (fundătură → backtrack!)
      Varadero  Santa Clara
      (nu e dest.)   \
                   Cienfuegos
                      \
                    Trinidad
                    (GĂSIT! ✓)
```

- Fiecare ramură = o cale posibilă
- Backtracking explorează TOATE ramurile
- Ramurile verzi = trasee găsite
- Ramurile roșii = fundături (backtrack!)

**Sugestie vizuală:** Arbore cu ramuri verzi (trasee găsite) și roșii (fundături), cu animație dacă e posibil

**Note pentru prezentator:**
> "Backtracking-ul este ca un explorator metodic. Nu sare la întâmplare — merge pas cu pas, și când nu mai poate avansa, se întoarce la ultima intersecție și încearcă alt drum. Când găsește o cale completă, o salvează și continuă să caute altele."

---

## SLIDE 6 — Structura BKT — Cele 6 funcții

**Titlu:** "Scheletul algoritmului — 6 funcții simple"

**Diagramă cu cele 6 funcții:**

| Funcție | Linia | Ce face |
|---------|-------|---------|
| `INIT(k)` | 121 | Pregătește poziția k (`x[k] = -1`) |
| `EXISTA(k)` | 125 | Mai avem orașe de încercat? (`x[k] < 14?`) |
| `VALID(k)` | 130 | E valid? (există drum + nu e vizitat + nu e sursa) |
| `SOLUTIE(k)` | 148 | Am ajuns la destinație? (`x[k] == destinatia?`) |
| `TIPAR(k)` | 153 | Salvează traseul găsit + calculează distanța și timpul |
| `BKT()` | 183 | Bucla principală — `while(k > 0) { ... }` |

**Codul funcției BKT() cu comentarii:**
```cpp
void BKT() {
    int k = 1;              // Începem de la poziția 1
    INIT(k);                // Pregătim prima poziție
    nrTrasee = 0;           // Resetăm contorul de trasee

    // Resetăm vectorul de vizitat, marcăm sursa
    for (int i = 0; i < NR_ORASE; i++) vizitat[i] = 0;
    vizitat[sursa] = 1;     // Sursa e deja vizitată

    while (k > 0) {         // Cât timp mai avem unde să ne întoarcem
        if (EXISTA(k)) {    // Mai sunt orașe de încercat?
            x[k] = x[k] + 1;     // Încercăm următorul oraș
            if (VALID(k)) {       // E o alegere validă?
                vizitat[x[k]] = 1;     // Marcăm ca vizitat
                if (SOLUTIE(k)) {      // Am ajuns la destinație?
                    TIPAR(k);          // Salvăm traseul
                    vizitat[x[k]] = 0; // Demarcăm (căutăm mai departe)
                }
                else {
                    k = k + 1;   // Avansăm la următoarea poziție
                    INIT(k);     // Pregătim noua poziție
                }
            }
        }
        else {
            k--;                  // BACKTRACK! Ne întoarcem
            if (k >= 1) {
                vizitat[x[k]] = 0;  // Demarcăm orașul
            }
        }
    }
}
```

**Sugestie vizuală:** Flowchart cu cele 6 funcții conectate cu săgeți, fiecare într-un dreptunghi colorat diferit

**Note pentru prezentator:**
> "Structura asta este STANDARD pentru orice problemă de backtracking din clasa a XI-a. Cele 6 funcții se regăsesc identic în orice problemă — diferă doar conținutul lor. Dacă înțelegeți cum funcționează aici, puteți rezolva orice problemă de backtracking."

---

## SLIDE 7 — BKT() — Flowchart vizual

**Titlu:** "Algoritmul pas cu pas — Flowchart"

**Flowchart (text — de recreat cu forme standard în Canva):**

```
[START (oval)]
    ↓
k = 1, INIT(k)
    ↓
┌──────────────┐
│  k > 0 ?     │ ──NO──→ [STOP — am terminat (oval)]
│  (romb)      │
└──────┬───────┘
       │ YES
       ↓
┌──────────────┐         ┌──────────────┐
│  EXISTA(k)?  │ ──NO──→ │  k-- (back-  │
│  (romb)      │         │  track)      │
└──────┬───────┘         │  demarcăm    │
       │ YES             │  vizitat     │
       ↓                 └──────┬───────┘
┌──────────────┐                │
│  x[k]++     │                ↑ (revine la k > 0?)
│  (dreptunghi)│
└──────┬───────┘
       ↓
┌──────────────┐
│  VALID(k)?   │ ──NO──→ (revine la EXISTA)
│  (romb)      │
└──────┬───────┘
       │ YES
       ↓
┌──────────────┐
│  marcăm      │
│  vizitat     │
└──────┬───────┘
       ↓
┌──────────────┐         ┌──────────────┐
│  SOLUTIE(k)? │ ──YES─→ │  TIPAR(k)    │
│  (romb)      │         │  demarcăm    │
└──────┬───────┘         └──────┬───────┘
       │ NO                     │
       ↓                  (revine la EXISTA)
┌──────────────┐
│  k = k + 1   │
│  INIT(k)     │
└──────┬───────┘
       │
 (revine la EXISTA)
```

**Legendă forme:**
- Oval = Start / Stop
- Romb = Decizie (DA/NU)
- Dreptunghi = Acțiune

**Sugestie vizuală:** Flowchart colorat cu forme standard. Folosiți: verde = acțiuni pozitive, roșu = backtrack, albastru = decizii

**Note pentru prezentator:**
> "Urmăriți săgețile pas cu pas. Observați cum algoritmul se mișcă mereu între EXISTA și VALID — încearcă orașul următor, verifică dacă e valid, și fie avansează, fie se întoarce. Când EXISTA spune NU, se face backtrack — k scade cu 1 și ne întoarcem la orașul anterior."

---

## SLIDE 8 — VALID() — Cele 3 verificări

**Titlu:** "Cum decide programul dacă un pas e valid?"

### Verificare 1: Există drum?
```cpp
if (distanta[orasPrecedent][x[k]] == 0) return 0;
```
→ "Dacă în matrice e 0, nu există drum direct — **INVALID**"

### Verificare 2: Nu e vizitat?
```cpp
if (vizitat[x[k]] == 1) return 0;
```
→ "Dacă am mai trecut pe acolo, nu mergem iar — **evităm ciclurile**"

### Verificare 3: Nu e sursa?
```cpp
if (x[k] == sursa) return 0;
```
→ "Nu ne întoarcem la orașul de plecare"

**Exemplu concret:**
> "Suntem în Matanzas (orașul 2). Încercăm să mergem la Varadero (orașul 1): ✅ există drum (42 km), ✅ nu e vizitat, ✅ nu e sursa → VALID! Încercăm Santa Clara (orașul 3): ✅ există drum (195 km), ✅ nu e vizitat, ✅ nu e sursa → VALID! Încercăm Havana (orașul 0): ✅ există drum (102 km), dar ❌ e sursa → INVALID!"

**Sugestie vizuală:** 3 carduri colorate — verde (trece verificarea), roșu (nu trece) cu exemple concrete

**Note pentru prezentator:**
> "Funcția VALID este cheia eficienței. Fără ea, algoritmul ar încerca să meargă pe drumuri inexistente sau ar intra în bucle infinite vizitând același oraș de mai multe ori. Cele 3 verificări sunt simple dar cruciale."

---

## SLIDE 9 — Exemplu concret: Havana → Trinidad

**Titlu:** "Să urmărim algoritmul pas cu pas"

**Traseu de demonstrat: Havana (0) → Trinidad (5)**

| Pas | k | Acțiune | Explicație |
|-----|---|---------|------------|
| 1 | 1 | x[1]=0 (Havana) | E sursa → INVALID |
| 2 | 1 | x[1]=1 (Varadero) | Nu există drum Havana→Varadero → INVALID |
| 3 | 1 | x[1]=2 (Matanzas) | Există drum 102km, nu e vizitat → VALID! Marcăm, k=2 |
| 4 | 2 | x[2]=1 (Varadero) | Drum 42km, nu e vizitat → VALID! Marcăm, k=3 |
| 5 | 3 | Din Varadero... | Nu are drum spre nimeni nevizitat (doar Matanzas, dar e vizitat) |
| 6 | 3 | BACKTRACK | k=2, demarcăm Varadero |
| 7 | 2 | x[2]=3 (Santa Clara) | Drum 195km → VALID! k=3 |
| 8 | 3 | x[3]=4 (Cienfuegos) | Drum 84km → VALID! k=4 |
| 9 | 4 | x[4]=5 (Trinidad) | Drum 80km → VALID! E DESTINAȚIA → **SOLUȚIE!** |
| 10 | — | TIPAR | Salvăm: Havana→Matanzas→Santa Clara→Cienfuegos→Trinidad = 461 km |
| 11 | — | Continuăm... | Backtrack și căutăm alte trasee |

**Rezultat final:** 6 trasee găsite de la Havana la Trinidad, cel mai scurt = 339 km (Havana → Matanzas → Cienfuegos → Trinidad)

**Sugestie vizuală:** Hartă a Cubei cu pași numerotați pe fiecare cale, sau animație cu săgeți care apar pas cu pas

**Note pentru prezentator:**
> "Observați cum algoritmul nu sare direct la cel mai scurt traseu — el le încearcă pe TOATE, le salvează, și la final le sortează. De aceea opțiunea 2 poate afișa toate cele 6 trasee."

---

## SLIDE 10 — Cele 5 opțiuni ale meniului

**Titlu:** "Ce poate face utilizatorul?"

| Nr. | Opțiune | Descriere |
|-----|---------|-----------|
| 1 | Distanța minimă | Găsește cel mai scurt traseu între două orașe |
| 2 | Toate traseele | Afișează TOATE căile posibile, sortate |
| 3 | Tipuri de drum | Arată Autopista/Carretera pe fiecare segment |
| 4 | Timp de parcurgere | Calculează timpul cu mașina, bicicleta, pe jos |
| 5 | Ieșire | Închide aplicația |

**Fragment de cod — afișarea meniului (linia 323):**
```cpp
void afiseazaMeniu() {
    cout << "  │  1. Distanta minima intre 2 localitati  │" << endl;
    cout << "  │  2. Toate traseele posibile             │" << endl;
    cout << "  │  3. Tipurile de drum pe un traseu       │" << endl;
    cout << "  │  4. Timpul de parcurgere                │" << endl;
    cout << "  │  5. Iesire din aplicatie                │" << endl;
}
```

**Sugestie vizuală:** 5 carduri colorate cu iconuri mari, fiecare opțiune evidențiată

**Note pentru prezentator:**
> "Fiecare opțiune rulează BKT cu aceiași parametri dar prezintă rezultatele diferit. Opțiunea 1 arată doar traseul cel mai scurt, opțiunea 2 le arată pe toate, opțiunea 3 adaugă informații despre tipul de drum, iar opțiunea 4 calculează și timpii de parcurgere pentru diferite moduri de transport."

---

## SLIDE 11 — Generarea HTML-ului

**Titlu:** "Cum devine codul C++ o hartă interactivă?"

**Fluxul în 5 pași:**
1. Utilizatorul alege opțiunea și orașele în consolă
2. C++ rulează BKT și găsește traseele
3. Funcția `genereazaHTML()` (linia 445) SCRIE un fișier `rezultat.html`
4. Fișierul conține cod HTML + JavaScript + CSS (466 linii generate!)
5. Browserul deschide fișierul și afișează harta

**Fragment de cod care arată cum C++ scrie HTML:**
```cpp
ofstream fout("rezultat.html");        // Deschidem fișierul
fout << "<!DOCTYPE html><html>..."     // Scriem cod HTML
     << endl;
fout << "<script>"                     // Scriem cod JavaScript
     << "var map = L.map('map');"      // care va rula în browser!
     << "</script>" << endl;
fout.close();                          // Închidem fișierul
```

**Fragment real din cod (linia 446-460):**
```cpp
ofstream fout("rezultat.html");
if (!fout.is_open()) {
    cerr << "  [!] Nu s-a putut crea fisierul rezultat.html!" << endl;
    return;
}
fout << R"html(<!DOCTYPE html>
<html lang="ro">
<head>
<meta charset="UTF-8">
<title>Cuba Navigator - Rezultat</title>
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
```

**Sugestie vizuală:** Diagramă: `C++ [box]` → scrie → `rezultat.html [box]` → deschide → `Browser [box cu harta]`

**Note pentru prezentator:**
> "E ca și cum programul C++ scrie o scrisoare (HTML) pe care browser-ul o citește. Funcția genereazaHTML() are 466 de linii — aproape 40% din tot codul! Ea construiește tot ce vedeți pe hartă: CSS-ul, JavaScript-ul, markerele, traseele, legenda."

---

## SLIDE 12 — Harta interactivă — Features

**Titlu:** "Ce conține harta generată?"

**Lista completă de funcționalități:**

1. **Hartă reală** — OpenStreetMap cu dark theme elegant
2. **Toggle Dark / Satelit** — buton stânga-sus (CARTO Dark + Esri Satellite)
3. **15 markere interactive** — click = popup cu numele orașului, coordonate GPS, număr de conexiuni
4. **Markere animate** — sursa (verde pulsant) și destinația (roșu pulsant)
5. **Drumuri pe culori:**
   - Roșu = Autopista Nacional (100 km/h)
   - Albastru = Carretera Nacional (80 km/h)
   - Portocaliu = Carretera Regional (60 km/h)
6. **Săgeți direcție** pe traseu (arată sensul de mers)
7. **Legendă detaliată** — tipuri de drum sau trasee multiple
8. **Busolă** (compass rose) — N/S/E/V
9. **Scală** metrică — distanța reală pe hartă
10. **Toggle rute individuale** (opțiunea 2) — activare/dezactivare per traseu
11. **Panel cu statistici:** distanță, timp, segmente, viteză medie
12. **Tabel detaliat** cu fiecare segment al traseului
13. **Design responsive** — se adaptează la ecran mic (telefon)
14. **Animații** — fade-in la încărcarea paginii

**Sugestie vizuală:** Screenshot anotat cu săgeți spre fiecare feature descris mai sus

**Note pentru prezentator:**
> "Aici facem un LIVE DEMO. Deschideți rezultat.html și arătați fiecare funcționalitate. Schimbați pe satelit — wow effect garantat. Faceți zoom pe un traseu. Dați click pe un marker."

---

## SLIDE 13 — Tipuri de drumuri din Cuba

**Titlu:** "Rețeaua de drumuri a Cubei"

| Cod | Denumire | Viteză max. | Descriere |
|-----|----------|-------------|-----------|
| **A** | Autopista Nacional | 100 km/h | Autostrada principală (Havana—Santiago) |
| **CN** | Carretera Nacional | 80 km/h | Drumuri naționale pavimentate |
| **CR** | Carretera Regional | 60 km/h | Drumuri regionale (mai înguste) |
| **CL** | Carretera Local | 40 km/h | Drumuri locale (nepavimentate uneori) |

**În proiect:**
- 4 drumuri de tip A (Autopista) — Havana-Matanzas, Matanzas-Santa Clara, Ciego de Avila-Camaguey, Santa Clara-Ciego de Avila
- 10 drumuri de tip CN (Carretera Nacional)
- 6 drumuri de tip CR (Carretera Regional)
- 0 drumuri de tip CL (nu sunt în rețeaua principală)

**Formula timp:**
```
timp = distanță / viteză
```
**Exemplu:** Havana → Matanzas: 102 km / 100 km/h = 1.02 ore = **1 oră și 1 minut**

**Sugestie vizuală:** 4 benzi colorate (roșu/albastru/portocaliu/gri) cu iconuri auto și viteze

**Note pentru prezentator:**
> "Viteza maximă legală influențează direct TIMPUL de parcurgere. De aceea opțiunea 4 calculează timpul pe fiecare segment individual — un traseu poate avea segmente pe autopistă (rapid) și pe drumuri regionale (lent)."

---

## SLIDE 14 — Comparație transport

**Titlu:** "Cât durează Havana → Santiago de Cuba?"

**Distanța totală (cel mai scurt traseu): ~916 km**

| Mod transport | Viteză | Timp estimat |
|---------------|--------|--------------|
| Cu mașina (viteză maximă) | variabilă (60-100 km/h) | ~10 ore |
| Cu bicicleta | 15 km/h | ~61 ore (2.5 zile!) |
| Pe jos | 5 km/h | ~183 ore (7.6 zile!) |

**Fragment de cod (linia 1162-1166):**
```cpp
double distKm = distanteTrasee[0];
cout << "Cu masina (viteza max): " << formateazaTimp(timpTrasee[0]) << endl;
cout << "Cu bicicleta (15 km/h): " << formateazaTimp(distKm / 15.0) << endl;
cout << "Pe jos (5 km/h):        " << formateazaTimp(distKm / 5.0) << endl;
```

**Observație:** Timpul cu mașina NU e distanță/viteză_constantă — e calculat pe fiecare segment cu viteza specifică tipului de drum! De aceea e mai precis.

**Sugestie vizuală:** 3 iconuri mari (mașină, bicicletă, persoană mergând) cu timpii afișați dramatic, contrast vizual mare

**Note pentru prezentator:**
> "Aceeași distanță, dar timpul variază enorm. Timpul cu mașina este calculat precis — pe autopistă mergi cu 100 km/h, pe drum regional cu 60 km/h. Bicicleta și mersul pe jos folosesc o viteză constantă, deci sunt estimări mai simple."

---

## SLIDE 15 — Complexitate și performanță

**Titlu:** "Cât de rapid este algoritmul?"

**Complexitate teoretică:** O(n!) — în cel mai rău caz, n! permutări
- 15 orașe → 15! = 1,307,674,368,000 posibilități TEORETICE
- Dar `VALID()` taie ramurile: verifică drum + vizitat → **mult mai puțin în practică**

**De ce e rapid?**
- Matricea de adiacență are doar 40 conexiuni nenule din 225 posibile (18%)
- `VALID()` elimină imediat orașele fără drum direct → tăiere masivă
- `vizitat[]` previne ciclurile → nu se repetă orașe

**Rezultate reale:**
- Havana → Trinidad: 6 trasee, calculat în <1 ms
- Havana → Santiago de Cuba: 28+ trasee, calculat în <5 ms

**Fragment de cod — măsurarea timpului (linia 962-966):**
```cpp
auto start = chrono::high_resolution_clock::now();
BKT();
sortTrasee();
auto end = chrono::high_resolution_clock::now();
long long ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
```

**Sugestie vizuală:** Grafic cu două bare — complexitate teoretică (imensă) vs complexitate reală (mică), arătând efectul "tăierii ramurilor"

**Note pentru prezentator:**
> "Backtracking-ul e inteligent — nu încearcă căi imposibile. Când VALID spune NU, toată ramura aia e ignorată. Din cele 1.3 trilioane de combinații teoretice, algoritmul verifică efectiv doar câteva sute. De aceea rezultatul apare instant."

---

## SLIDE 16 — Structura proiectului

**Titlu:** "Ce conține proiectul?"

**Statistici:**
- **1199 linii** de cod total
- **~733 linii** logică C++ (61%)
- **~466 linii** generare HTML în genereazaHTML() (39%)
- **21 funcții**
- **15 orașe**, **20 drumuri** bidirectionale
- **Binar compilat:** 108 KB
- **Un singur fișier:** main.cpp

**Structura fișierului:**
```
CubaNavigator/
├── main.cpp          → Tot codul C++ (1199 linii)
│   ├── Linii 1-55    → Include-uri, constante, variabile globale
│   ├── Linii 56-113  → initializeazaDate() — orașe + drumuri
│   ├── Linii 115-214 → INIT, EXISTA, VALID, SOLUTIE, TIPAR, BKT
│   ├── Linii 216-306 → Funcții ajutătoare (sort, format, culori)
│   ├── Linii 307-439 → Funcții de afișare (banner, meniu, tabele)
│   ├── Linii 441-910 → genereazaHTML() — generează harta (466 linii!)
│   └── Linii 912-1199 → main() cu meniul principal
├── rezultat.html     → Generat AUTOMAT de program
├── README.md         → Documentație
└── .gitignore        → Fișiere ignorate de Git
```

**Sugestie vizuală:** File tree cu iconuri, plus o diagramă circulară (pie chart) cu 61% C++ / 39% HTML

**Note pentru prezentator:**
> "Totul e într-un singur fișier main.cpp — conform cerințelor de la clasă. Funcția genereazaHTML() este cea mai mare funcție, ocupând aproape 40% din cod. Ea generează tot ce vedeți pe hartă: stilurile CSS, codul JavaScript pentru Leaflet, markerele, traseele, panoul de statistici."

---

## SLIDE 17 — Demo live

**Titlu:** "Demo — Cuba Navigator în acțiune"

**Instrucțiuni pentru prezentator:**
1. Deschide terminalul
2. `cd ~/CubaNavigator && ./cuba_navigator`
3. Arată meniul cu culori ANSI (banner + tabel orașe + opțiuni)
4. **Opțiunea 1:** Havana → Santiago de Cuba → arată distanța minimă și timpul
5. Switch la browser → arată harta cu traseul evidențiat pe culori
6. Toggle pe **Satelit** → wow effect cu imagine reală!
7. Click pe un marker → popup cu info
8. **Opțiunea 2:** Havana → Trinidad → arată toate cele 6 trasee
9. Pe hartă: toggle rute individuale folosind controlul din dreapta-jos
10. **Opțiunea 3:** arată tabelul cu tipuri de drum pe fiecare segment
11. **Opțiunea 4:** arată comparația mașină / bicicletă / pe jos

**Sugestie vizuală:** FULL SCREEN — consolă și browser side by side (50/50 sau 70/30)

**Note pentru prezentator:**
> "Acum o să vedeți aplicația live. Urmăriți cum rezultatele din consolă apar instant pe hartă. Fiecare opțiune generează un HTML diferit, adaptat la tipul de informație cerut."

---

## SLIDE 18 — Ce am învățat

**Titlu:** "Concluzii și ce am învățat"

1. **Backtracking-ul poate rezolva probleme reale** — nu doar exerciții abstracte din manual. Am aplicat algoritmul pe o rețea reală de drumuri cu coordonate GPS reale.

2. **Matricea de adiacență = modul de a reprezenta o rețea de drumuri** — un concept simplu (tabel cu 0 și distanțe) care codifică o hartă întreagă.

3. **C++ poate genera pagini web (HTML)** — programul nu doar calculează, ci și prezintă vizual rezultatele folosind `ofstream`.

4. **Importanța validării datelor** — input corect, verificare drum existent, evitare cicluri — fiecare verificare previne o eroare.

5. **Cum funcționează hărțile online** — tile-uri, coordonate GPS, API-uri, biblioteci JavaScript — totul lucrează împreună.

**Sugestie vizuală:** 5 bullets cu iconuri tematice (creier, tabel, cod, scut, glob pământ)

**Note pentru prezentator:**
> "Acest proiect demonstrează că algoritmii pe care îi învățăm la informatică au aplicații practice directe. Backtracking-ul din manual devine un navigator GPS. Matricea de adiacență devine o hartă a Cubei. Iar C++, deși e un limbaj de programare, poate genera pagini web interactive."

---

## SLIDE 19 — Întrebări?

**Titlu:** "Întrebări?"

**Subtitlu:** `github.com/amarieidavid26-byte/cubanavigator`

**Sugestie vizuală:** Screenshot hartă satelit fullscreen ca fundal, cu text alb semi-transparent pe fundal întunecat

**Text adițional (jos):**
> Cuba Navigator v2.2 — Proiect Backtracking C++ | David Amariei | 2025

---

## ANEXĂ — Statistici rapide pentru referință

| Metrică | Valoare |
|---------|---------|
| Linii totale | 1199 |
| Funcții | 21 |
| Orașe | 15 |
| Drumuri | 20 (bidirectionale) |
| Tipuri drum | 4 (A, CN, CR, CL) |
| Variabile globale | 12 |
| Include-uri | 8 |
| Dimensiune fișier | 56 KB (sursă), 108 KB (binar) |
| Linii C++ logică | ~733 (61%) |
| Linii generare HTML | ~466 (39%) |
| Algoritm | Backtracking iterativ |
| Sortare | Selection sort |
| Complexitate | O(n!) teoretic, mult mai puțin în practică |

### Lista completă a funcțiilor:

| # | Funcție | Linia | Tip |
|---|---------|-------|-----|
| 1 | `adaugaDrum()` | 60 | Inițializare |
| 2 | `initializeazaDate()` | 66 | Inițializare |
| 3 | `INIT()` | 121 | BKT |
| 4 | `EXISTA()` | 125 | BKT |
| 5 | `VALID()` | 130 | BKT |
| 6 | `SOLUTIE()` | 148 | BKT |
| 7 | `TIPAR()` | 153 | BKT |
| 8 | `BKT()` | 183 | BKT |
| 9 | `sortTrasee()` | 221 | Ajutătoare |
| 10 | `getNrConexiuni()` | 251 | Ajutătoare |
| 11 | `getNumeTipDrum()` | 259 | Ajutătoare |
| 12 | `fmt()` | 268 | Ajutătoare |
| 13 | `formateazaTimp()` | 275 | Ajutătoare |
| 14 | `culoareDrum()` | 290 | Ajutătoare |
| 15 | `grosimeDrum()` | 299 | Ajutătoare |
| 16 | `afiseazaBanner()` | 311 | Afișare |
| 17 | `afiseazaMeniu()` | 323 | Afișare |
| 18 | `separator()` | 335 | Afișare |
| 19 | `afiseazaOrase()` | 339 | Afișare |
| 20 | `citesteOras()` | 357 | Afișare/Input |
| 21 | `afiseazaTraseu()` | 373 | Afișare |
| 22 | `afiseazaDetaliiTraseu()` | 382 | Afișare |
| 23 | `deschideHTML()` | 429 | Utilitar |
| 24 | `genereazaHTML()` | 445 | Generare HTML |
| 25 | `main()` | 915 | Principal |

### Cele 15 orașe cu coordonate GPS:

| Nr. | Oraș | Latitudine | Longitudine |
|-----|------|------------|-------------|
| 0 | Havana | 23.1136 | -82.3666 |
| 1 | Varadero | 23.1394 | -81.2863 |
| 2 | Matanzas | 23.0510 | -81.5775 |
| 3 | Santa Clara | 22.4065 | -79.9553 |
| 4 | Cienfuegos | 22.1456 | -80.4364 |
| 5 | Trinidad | 21.8027 | -79.9843 |
| 6 | Sancti Spiritus | 21.9304 | -79.4425 |
| 7 | Ciego de Avila | 21.8405 | -78.7621 |
| 8 | Camaguey | 21.3891 | -77.9160 |
| 9 | Holguin | 20.8872 | -76.2630 |
| 10 | Santiago de Cuba | 20.0247 | -75.8219 |
| 11 | Bayamo | 20.3792 | -76.6434 |
| 12 | Guantanamo | 20.1449 | -75.2091 |
| 13 | Pinar del Rio | 22.4175 | -83.6978 |
| 14 | Las Tunas | 20.9601 | -76.9542 |

### Cele 20 de drumuri:

| De la | La | Distanță | Tip | Viteză |
|-------|-----|----------|-----|--------|
| Havana | Matanzas | 102 km | A | 100 km/h |
| Havana | Pinar del Rio | 162 km | CN | 80 km/h |
| Matanzas | Varadero | 42 km | CR | 60 km/h |
| Matanzas | Santa Clara | 195 km | A | 100 km/h |
| Santa Clara | Cienfuegos | 84 km | CN | 80 km/h |
| Cienfuegos | Trinidad | 80 km | CR | 60 km/h |
| Trinidad | Sancti Spiritus | 74 km | CR | 60 km/h |
| Santa Clara | Sancti Spiritus | 89 km | CN | 80 km/h |
| Sancti Spiritus | Ciego de Avila | 110 km | CN | 80 km/h |
| Ciego de Avila | Camaguey | 110 km | A | 100 km/h |
| Camaguey | Las Tunas | 120 km | CN | 80 km/h |
| Las Tunas | Holguin | 76 km | CN | 80 km/h |
| Las Tunas | Bayamo | 92 km | CN | 80 km/h |
| Bayamo | Santiago de Cuba | 127 km | CN | 80 km/h |
| Santiago de Cuba | Guantanamo | 86 km | CR | 60 km/h |
| Holguin | Santiago de Cuba | 135 km | CN | 80 km/h |
| Bayamo | Holguin | 72 km | CR | 60 km/h |
| Matanzas | Cienfuegos | 157 km | CR | 60 km/h |
| Santa Clara | Ciego de Avila | 188 km | A | 100 km/h |
| Camaguey | Holguin | 206 km | CN | 80 km/h |
