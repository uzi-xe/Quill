# Quill C++ CLI

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey)
![Threads](https://img.shields.io/badge/Concurrency-std%3A%3Athread%20%26%20std%3A%3Amutex-blue)

**Quill** is a lightweight, local-first, graph-based note-taking command-line application written in modern **C++17**. Inspired by knowledge-management tools like Obsidian, Quill supports WikiLink syntax (`[[Note Title]]`), multithreaded backlink indexing, Breadth-First Search (BFS) graph reachability, task checklists, and Markdown export.

---

## Features

- **Multiple Note Types:** Create and manage standard `Note` items, checklist-enabled `TaskNote` items, ISO-validated `DailyTaskNote` entries, and `ShareableNote` documents.
- **Bidirectional WikiLinks (`[[...]]`):** Automatically parses `[[Note Title]]` references to build forward-link and backlink indices.
- **Multithreaded Indexing:** Splits vault notes across parallel worker threads (`std::thread`) synchronized with `std::lock_guard<std::mutex>`.
- **Graph Reachability (BFS):** Computes and displays all transitively connected notes from any starting note using Breadth-First Search.
- **Interactive Editing & Tagging:** Open notes by ID to edit content, chain tags fluently, and track revision counts via embedded metadata.
- **Local Markdown Backup:** Exports notes to `.md` files with RAII-safe file streams.

---

## Project Structure

```text
Quill/
├── main.cpp         # Interactive 7-option CLI loop and diagnostics suite
├── Models.h         # Note class hierarchy, nested Metadata, and virtual base classes
├── Models.cpp       # Note implementations, typecasting, and friend operators
├── Core.h           # Vault container and Indexer graph engine declarations
├── Core.cpp         # Multithreaded regex link indexer and BFS graph traversal
├── Templates.h      # Generic function templates and Registry<K, V> class template
├── Exceptions.h     # Custom QuillException hierarchy (NoteNotFound, InvalidDate, Storage)
├── Storage.h        # LocalStorage interface for Markdown file I/O
├── Storage.cpp      # RAII-based file reading and Markdown export
└── README.md        # Project documentation
```

---

## Getting Started

### Prerequisites

- A C++17 compatible compiler:
  - **Windows:** MinGW-w64 (`g++`) or MSVC
  - **macOS:** Xcode Command Line Tools (`clang++` or `g++`)
  - **Linux:** GCC (`g++`) or Clang (`clang++`)

### Build & Run

#### Windows (PowerShell)
```powershell
g++ -std=c++17 -Wall -Wextra Core.cpp Models.cpp Storage.cpp main.cpp -o Quill.exe
.\Quill.exe
```

#### Windows (Command Prompt)
```cmd
g++ -std=c++17 -Wall -Wextra Core.cpp Models.cpp Storage.cpp main.cpp -o Quill.exe && Quill.exe
```

#### macOS / Linux (Bash / Zsh)
```bash
g++ -std=c++17 -Wall -Wextra -pthread Core.cpp Models.cpp Storage.cpp main.cpp -o quill
./quill
```

---

## CLI Preview

```text
====================================================
                     QUILL CLI                      
====================================================
  [1] List All Notes
  [2] Create Note
  [3] Open & Edit Note
  [4] Add Task to Note
  [5] Search Notes
  [6] View Links & Graph
  [7] Vault Info & Diagnostics
  [0] Exit
----------------------------------------------------
  Select option > 1

  Notes in Vault (5):
  +--------------------------------------------------
  | [Note] Project Alpha (ID: 1)
  | Tags   : #work #priority #q4
  | Content: Integrate with [[Meeting Notes]]. (33 chars, 2 edits)
  +--------------------------------------------------
  | [TaskNote] Grocery List (ID: 3)
  | Tags   : #home
  | Content: Weekly restock. (15 chars, 0 edits)
  +--------------------------------------------------
  |   [ ] Milk
  |   [x] Eggs
  +--------------------------------------------------
```

---

## C++17 & OOP Concepts Implemented

This project is designed as a clean, practical reference covering core Object-Oriented Programming and modern C++17 features:

| Concept | Implementation in Quill | File Location |
| :--- | :--- | :--- |
| **Typecasting (Basic to Class)** | Converting constructor `Note(const char* quickText)` | `Models.h`, `Models.cpp` |
| **Typecasting (Class to Basic)** | `explicit operator int() const` returning note character count | `Models.h`, `Models.cpp` |
| **Typecasting (Class to Class)** | `TaskNote(const Note& source)` promoting a `Note` to a `TaskNote` | `Models.h`, `Models.cpp` |
| **Function Templates** | `clampValue<T>(val, low, high)` and `joinItems<T>(items, sep)` | `Templates.h` |
| **Class Templates** | `Registry<K, V>` generic key-value dictionary | `Templates.h`, `main.cpp` |
| **Friend Operator Overloading** | `operator+` (merge notes), `operator<<` (print), `operator>>` (input) | `Models.h`, `Models.cpp` |
| **Nested Class** | `Note::Metadata` tracking per-note `editCount` | `Models.h` |
| **Static Variable & Function** | `Note::totalNotesCreated` and `Note::getTotalNotesCreated()` | `Models.h`, `Models.cpp` |
| **`this` Pointer & Chaining** | `Note& Note::addTag()` using `this->` and returning `*this` | `Models.cpp` |
| **Virtual Functions** | Polymorphic `display()` and `getTypeName()` across note types | `Models.h`, `Models.cpp` |
| **`final` Specifier** | `class DailyTaskNote final` and leaf exception classes | `Models.h`, `Exceptions.h` |
| **Virtual Base Class (Diamond)** | `Entity` inherited virtually by `Readable` and `Writable` into `ShareableNote` | `Models.h`, `Models.cpp` |
| **Object Slicing** | Demonstrated via pass-by-value in `previewNoteByValue(Note slicedNote)` | `main.cpp` |
| **Pointers to Objects & Derived** | `Vault::getRawNotePointer()` (`Note*`) and `dynamic_cast<TaskNote*>` | `Core.cpp`, `main.cpp` |
| **Multithreading & Mutex** | Parallel link indexing with `std::thread`, `join()`, and `std::lock_guard` | `Core.cpp` |
| **Exception Handling & RAII** | Custom `QuillException` hierarchy and scoped file stream management | `Exceptions.h`, `Storage.cpp` |
