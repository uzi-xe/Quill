# Quill
 
A local-first, extensible productivity desktop app — notes, tasks, and knowledge management, offline by default.
 
Think Notion's block editor + Obsidian's bi-directional linking, but fully local: your data lives as plain markdown files on your own disk, no account required, no cloud dependency, and no network calls happening behind your back.
 
## Why
 
Most note-taking tools force a choice: rich block editing (Notion) *or* file portability and ownership (Obsidian), and almost none are built to be extended without forking the whole app. Quill is designed core-first around a plugin architecture, so new block types, panels, and integrations can be added without touching the core editor, storage, or data model.
 
## Core features (target)
 
- **Block-based editing** — pages built from rearrangeable blocks: text, headings, checklists, code, tables, images, embeds
- **Bi-directional linking** — `[[page name]]` links with autocomplete, a backlinks panel, and a visual graph view
- **Local-first storage** — every page is a plain `.md` file with YAML frontmatter; a SQLite index is a derived, disposable cache — never the source of truth
- **Full-text search & command palette** — `Cmd/Ctrl+K` for everything, keyboard-first throughout
- **Tags, daily notes, and a task view** — aggregate open checkboxes across the whole vault
- **Plugin system** — new block types, panels, and commands register through a defined API; nothing is hardcoded into core
 
## Status
 
🚧 Early development — Phase 1 (foundation) in progress. See [`PROGRESS.md`](./PROGRESS.md) for the running build log and [`docs/`](./docs) for architecture notes.
 
## Tech stack
 
- **Shell**: Electron
- **UI**: React
- **Storage**: Plain markdown files (vault) + SQLite (`.app/index.sqlite`) as a rebuildable search/backlink cache
- **Language**: JavaScript/TypeScript throughout — main process, renderer, and plugin API
 
See [`docs/ARCHITECTURE.md`](./docs/ARCHITECTURE.md) *(coming soon)* for the full core/plugin design and reasoning behind these choices.

MADE BY VISHAL!