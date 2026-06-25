# CHESS-GAME-PART-2-HUMAN-VS-COMPUTER-HUMAN-VS-STOKFISH-ENGINE
# ♟️ ConsolChess — C++ Terminal Chess Engine

<div align="center">

![C++](https://img.shields.io/badge/Language-C%2B%2B-blue?style=for-the-badge&logo=cplusplus)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows)
![Engine](https://img.shields.io/badge/AI-Stockfish%20Integrated-green?style=for-the-badge)
![Semester](https://img.shields.io/badge/Project-1st%20Semester%20Final-orange?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen?style=for-the-badge)

> **A fully scratch-built chess game in C++ running entirely in the Windows console — no libraries, no frameworks, just raw code.**

</div>

---

## 🎮 Game Modes

```
╔══════════════════════════════════════════╗
║            ♟  ConsolChess  ♛             ║
╠══════════════════════════════════════════╣
║  1.  Human  vs  My AI Computer           ║
║  2.  Human  vs  Stockfish Engine  ★      ║
║  0.  Quit                                ║
╚══════════════════════════════════════════╝
```

| Mode | Description |
|------|-------------|
| 🧑‍💻 **Human vs AI** | Play against a custom-built minimax AI written from scratch |
| 🤖 **Human vs Stockfish** | Face one of the world's strongest chess engines via UCI protocol |

---

## ✨ Features

### 🎨 Visual & UI
- **ASCII art pieces** — Every piece (Pawn, Rook, Knight, Bishop, Queen, King) is hand-drawn using console block characters
- **Mouse-click input** — Click squares directly on the console to select and move pieces
- **Color-coded board** — Windows console color API used for dark/light square contrast and piece highlighting
- **Hidden cursor** — Clean, flicker-free rendering with no visible cursor

### ♟️ Chess Logic (Built from Scratch)
- Full legal move validation for all 6 piece types
- **Check** detection — alerts displayed in real time
- **Checkmate** detection — game ends with winner announcement
- **Stalemate** detection — draw game declared automatically
- Self-check prevention — illegal moves that expose your own king are blocked
- Pawn promotion support (via Stockfish move notation)

### 💾 Game Management
- **Save & Load** — resume any game from where you left off
- **Undo / Redo** — press `U` to undo, `R` to redo any move mid-game
- **Replay system** — watch a full move-by-move replay of the game every 10 moves

### 🤖 AI Engines
- **Phase 1 — Custom AI:** A hand-coded computer opponent using smart move selection
- **Phase 2 — Stockfish:** Full UCI protocol integration using pipes (Windows `CreateProcess` / POSIX `fork`+`exec`)
  - Converts board to **FEN notation** in real time
  - Sends positions to Stockfish, parses `bestmove` response
  - Supports pawn promotion moves (`q`, `r`, `b`, `n`)

---

## 📁 Project Structure

```
📦 ConsolChess/
├── 📄 BSAI25033-CHESS-PART_2.cpp   ← Main game logic, board rendering, AI
├── 📄 pf_stockfish.h                ← Stockfish interface header
├── 📄 pf_stockfish.cpp              ← Stockfish process management (Win32 + POSIX)
└── 📄 stockfish.exe                 ← Stockfish binary (place in project root)
```

---

## 🛠️ How to Build & Run

### Prerequisites
- Windows OS (primary target — uses `<windows.h>`, `<conio.h>`)
- A C++ compiler: **MSVC** (Visual Studio) or **MinGW/g++**
- [Stockfish](https://stockfishchess.org/download/) binary placed in the same folder as the executable

### Compile with g++ (MinGW)
```bash
g++ BSAI25033-CHESS-PART_2.cpp pf_stockfish.cpp -o chess.exe
```

### Compile with MSVC
```bash
cl BSAI25033-CHESS-PART_2.cpp pf_stockfish.cpp /Fe:chess.exe
```

### Run
```bash
chess.exe
```
> ⚠️ Make sure `stockfish.exe` is in the **same directory** as `chess.exe` before selecting Mode 2.

---

## 🕹️ Controls

| Action | Input |
|--------|-------|
| Select & move piece | **Left Mouse Click** |
| Undo last move | Press `U` |
| Redo move | Press `R` |
| Replay game | Press `Y` when prompted |
| Quit to menu | Close window / `Ctrl+C` |

---

## 🧠 Technical Highlights

```
Board Representation  →  Piece* board[8][8]   (pointer-based 2D array)
Move Validation       →  Function pointers stored inside each Piece struct
Rendering             →  Windows COORD + SetConsoleCursorPosition
Input                 →  ReadConsoleInput with MOUSE_EVENT capture
AI Communication      →  Named pipes (Win32) / fork+exec (POSIX)
Position Encoding     →  Real-time FEN string generation from board state
```

---

## 📸 Chess Piece Preview (Console Art)

```
  ♟ PAWN      ♜ ROOK      ♞ KNIGHT
   ███          ███          ░█░
  █████        █████        ░███
  █████        █████       ██░░░
  █████        █████       ██░░░
 ███████      ███████       ███
```
*(Actual pieces rendered with ASCII block characters directly in the Windows console)*

---

## 🗂️ Concepts Demonstrated

- ✅ Object-Oriented Design with `struct` and function pointers
- ✅ Dynamic memory management (`new` / `delete`)
- ✅ Recursion for check/checkmate detection
- ✅ File I/O for game save & load
- ✅ Process creation and inter-process communication (IPC via pipes)
- ✅ Windows API: Console handles, mouse events, cursor control, color output
- ✅ UCI protocol parsing and FEN notation generation

---

## 👨‍🎓 Project Info

| Field | Details |
|-------|---------|
| **Student ID** | BSAI25033 |
| **Course** | Programming Fundamentals (PF) |
| **Semester** | 1st Semester |
| **Project** | Final Project — Part 2 |
| **Language** | C++ (Standard + Windows API) |

---

## 📜 License

This project was developed as a university final project. Feel free to study, fork, and learn from it.

---

<div align="center">

Made with ♟️ and a lot of `cout` statements

**⭐ Star this repo if you found it impressive!**

</div>
