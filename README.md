# Compiler Optimization Project

Projekat iz kursa **Konstrukcija Kompilatora** na Matematičkom Fakultetu u Beogradu

## Tim
- **Zarija Trtović** - Dead Store Elimination
- **Mima Grujić** - Algebraic Simplification


## Struktura projekta
```
compiler-optimization/
│
├── README.md                          # Ovaj fajl
├── .gitignore                         # Git ignore pravila
│
├── dead-store/                        # Dead Store Elimination (Zarija)
│   ├── DeadStoreElimination.cpp      # Glavni kod
│   └── test-examples/                 # Test primeri
│
└── algebraic/                         # Algebraic Simplification (Mima)
    ├── AlgebraicSimplification.cpp   # Glavni kod
    └── test-examples/                 # Test primeri


### Opis
Optimizacija koja detektuje i uklanja "dead stores" - dodele vrednosti promenljivama koje se nikada ne koriste nakon dodele.

Dead store nastaje kada se:
1. Vrednost dodeli promenljivoj, ali se odmah prepiše pre korišćenja
2. Vrednost dodeli promenljivoj koja se nikad ne koristi

### Algoritam

1. Učitaj LLVM IR fajl
2. Za svaku `store` instrukciju, traži sledeću instrukciju sa istom adresom:
   - Ako je sledeća `store` → prva je **DEAD**
   - Ako je sledeća `load` → prva je **LIVE**
   - Ako nema ni jednog → **DEAD**
3. Generiši optimizovani fajl bez dead stores


### Potrebni alati
- LLVM 14+
- Clang
- g++ ili clang++
