# AVL vs. Splay Trees – Performance Comparison Project

*CSE 2138 – Data Structures & Algorithms · Project 2*

This program implements two self‑balancing binary search trees—**AVL** and **Splay**—then measures their **total access cost** on identical insertion + search workloads so you can observe how access patterns influence efficiency.

> Author: **Ahmet Abdullah Gültekin**

---

## ✨ What It Does
1. **Reads** a sequence of integer keys from `input1.txt` and `input2.txt`.
2. **Inserts** each key into *both* trees (duplicates are ignored).
3. **Re‑reads** the same file, performing a `find` on every key while summing the depth reached (≈ cost).
4. Prints the **pre‑order traversal** of each tree and the **total cost**.

Example snippet from `outputs.txt` (dataset 1):
```
AVL tree:
25 12 6 3 2 5 10 … 52
Total cost: 260

Splay tree:
24 21 20 17 10 … 50
Total cost 549
```

Interpretation: for a random, non‑locality workload, AVL’s strict balance yields ~2× cheaper access than Splay.

---

## 📂 Repository Layout
```
Comparison-of-AVL-and-BST/
├── 150121025_Prj2.c      # Single-file implementation
├── input1.txt            # 100 integers (uniform)
├── input2.txt            # Different distribution
├── outputs.txt           # Reference results
└── README.md             # ← you are here
```

---

## 🛠 Build & Run
### Linux / macOS
```bash
gcc -std=c11 -O2 -o treecmp 150121025_Prj2.c
./treecmp input1.txt > my_out1.txt
./treecmp input2.txt > my_out2.txt
```
### Windows (MinGW)
```bash
gcc -std=c11 -O2 -o treecmp.exe 150121025_Prj2.c
```
---

## ⚙️ Implementation Notes
| Component | AVL | Splay |
|-----------|-----|-------|
| **Insert** | classic rotations (`LL, RR, LR, RL`) maintain `height` field | standard *top‑down splaying* during insert
| **Search** | simple BST search | splay operation moves hit node to root, benefitting locality
| **Cost metric** | depth + 1 for each visited node | same metric; note splaying changes future tree shape
| **Duplicates** | ignored | ignored

All dynamic allocation done with `malloc`; `free` called at program exit.

---

## 📈 Results Summary
| Dataset | Keys | AVL cost | Splay cost | Δ (%) |
|---------|------|----------|-----------|-------|
| input1  | 100  | 260 | 549 | **+111 %** |
| input2  | 100  | 309 | 622 | **+101 %** |

*Random inserts favour AVL because splaying on every search seldom pays off.*

---

## 📚 Further Experiments
* Feed **Zipfian‑distributed** keys to highlight Splay’s advantage on temporal locality.
* Compare against **Red‑Black** or **Treap** implementations.
* Instrument number of rotations and memory usage.


