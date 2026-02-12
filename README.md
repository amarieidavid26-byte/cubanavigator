# Cuba Navigator

Aplicatie de navigare GPS pentru insula Cuba, implementata in C++ folosind algoritmul **backtracking iterativ** cu structura INIT/EXISTA/VALID/SOLUTIE/TIPAR.

Programul gaseste toate traseele posibile intre doua orase din Cuba, determina traseul optim (distanta minima) si genereaza o harta HTML interactiva cu vizualizarea rezultatelor.

## Compilare

```bash
g++ -std=c++17 -Wall -Wextra -o cuba_navigator main.cpp
```

## Rulare

```bash
./cuba_navigator
```

La pornire se afiseaza lista celor 15 orase si un meniu cu 5 optiuni:
1. **Distanta minima** — gaseste traseul cel mai scurt intre 2 orase
2. **Toate traseele** — genereaza toate traseele posibile (backtracking complet)
3. **Tipuri de drum** — detalii pe segmente (Autopista, Carretera Nacional/Regional/Local)
4. **Timpul de parcurgere** — timp estimat + comparatie moduri de transport
5. **Iesire**

Dupa fiecare operatie se genereaza automat `rezultat.html` — o harta interactiva SVG cu dark theme profesional, care se deschide in browser.

## Structura proiectului

| Fisier | Descriere |
|--------|-----------|
| `main.cpp` | Tot codul: variabile globale, BKT iterativ, meniu, generare HTML |
| `rezultat.html` | Fisier generat — harta SVG cu trasee, legenda, statistici |
| `archive_v1_oop/` | Backup-ul versiunii v1.0 cu clase si STL |

## Algoritm

- **Metoda**: Backtracking iterativ (structura BKT din clasa)
- **Functii**: INIT, EXISTA, VALID, SOLUTIE, TIPAR, BKT
- **Date**: variabile globale, matrice de adiacenta, vectori clasici (fara STL)
- **Complexitate**: O(N!) in cazul cel mai defavorabil (toate permutarile de noduri)
- **Graful**: 15 noduri (orase), 20 muchii (drumuri bidirectionale)

## Date geografice

- 15 orase cu coordonate GPS reale
- 20 drumuri bazate pe reteaua rutiera reala a Cubei
- 4 tipuri de drum: Autopista (100 km/h), Carretera Nacional (80 km/h), Carretera Regional (60 km/h), Carretera Local (40 km/h)

## Autor

[de completat]
