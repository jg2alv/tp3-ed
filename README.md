# Flight Club — Indexed flight filtering and multi-criteria ranking in C++

Flight Club turns route, budget, availability, and travel-time constraints into a ranked shortlist. Eight AVL attribute indexes locate candidate flights, and each query independently defines how price, duration, and stops should influence result order.

## 1. Academic context and scope

- **Institution:** Universidade Federal de Minas Gerais (UFMG)
- **Degree:** Ciência da Computação
- **Course:** Estrutura de Dados
- **Assignment:** Trabalho Prático 3
- **Semester:** 2024-02

This command-line search engine combines balanced trees, linked lists, expression parsing, and pointer sorting. Its shared flight collection is indexed by eight attributes. Records with equal values share a linked-list bucket within the relevant index.

Queries use conjunctive comparison clauses to express restrictions. Matching flights are sorted with a custom QuickSort according to the requested ranking, and the program prints the requested number of results.

## 2. How to run

Prerequisites: GNU Make and `g++`. Run from the repository root using the checked-in [Makefile](Makefile):

```sh
mkdir -p obj bin
make
```

The Makefile creates `bin/tp3.out`; its first positional argument must be an input-file path. The file contains a flight count, flight records, a query count, and the corresponding queries. The parser and data fields are implemented in [Passagem.cpp](src/Passagem.cpp) and [Consulta.cpp](src/Consulta.cpp). No example input is tracked.

The assignment describes a standard-input interface, but the archived entrypoint opens the file supplied as its first argument. Redirecting records to standard input alone does not match this implementation's interface.

## 3. Implementation decisions

- **Shared records, multiple indexes:** the AVL trees keep pointers into a shared flight allocation, avoiding independent copies of every flight for every attribute.
- **Duplicate values in linked lists:** equal index keys share a list of matching records instead of requiring unique tree keys.
- **Filtering before ranking:** index retrieval narrows the candidate set, then every candidate is checked against the full conjunction before sorting.
- **Query-specific comparison:** the ranking trigram controls the order in which price, duration, and stops break ties. Sorting reorders flight pointers rather than the shared records.

The entrypoint coordinates parsing, index construction, filtering, ranking, output, and cleanup; the tree and query modules own the corresponding data-structure operations.

## 4. Learning and limitations

The main learning challenge was keeping comparisons consistent across string and numeric attributes, query operators, and ranking tie-breakers. Coordinating several indexes over the same records also made ownership and lifetime decisions central to correctness.

The output loop assumes that a query's requested result count does not exceed the number of matches; it does not clamp that count before indexing the sorted array. Inputs must respect that condition.
