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
└── algebraic/                         # Algebraic Simplification (Koleginica)
    ├── AlgebraicSimplification.cpp   # Glavni kod
    └── test-examples/                 # Test primeri


### Potrebni alati
- LLVM 14+
- Clang
- g++ ili clang++
