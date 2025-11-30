## Stadium & Stands Ticketing API

This repository contains my solution to Exercise 3 from the **Intro to Systems Programming** course.  
The goal here was to design a small **C API for managing a stadium**, its stands, and seat allocation.

### What this project does
- Loads a stadium description from a text file (stands, capacities, names, etc.).
- Exposes operations to:
  - Query the number of stands and overall capacity.
  - Inspect each stand’s name and capacity.
  - Buy individual tickets, consecutive seats, or batches of seats.
- Includes a test driver in `main.c` that exercises the API and prints human‑readable results.

### Design highlights
- **Language**: C
- **Key ideas**:
  - Modular design using headers (`.h`) and implementation files (`.c`).
  - Encapsulating the internal representation of a stadium behind a clean API.
  - Careful use of pointers and dynamic memory (allocation, ownership, cleanup).
- **Main components**:
  - `Stadium.h` / `Stadium.c` – creation/destruction of a stadium and high‑level operations.
  - `Stand.h` / `Stand.c` – represents a single stand and its seats.
  - `main.c` – runs several tests that mimic realistic usage (single ticket purchase, consecutive tickets, bulk tickets, etc.).
