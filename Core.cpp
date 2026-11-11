#include "Core.h"
#include "Exceptions.h"
#include <regex>
#include <queue>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <algorithm>

using namespace std;

void Vault::addNote(shared_ptr<Note> note) {
    notes.push_back(note);
}

vector<shared_ptr<Note>> Vault::listAllNotes() const {
    return notes;
}

shared_ptr<Note> Vault::requireNoteById(const string& id) const {
    for (const auto& note : notes) {
        if (note->getId() == id) return note;
    }
    throw NoteNotFoundException(id);
}

Note* Vault::getRawNotePointer(const string& id) const {
    for (const auto& note : notes) {
        if (note->getId() == id) return note.get();
    }
    return nullptr;
}

Indexer::Indexer(const Vault& vault) : vault(vault) {}

void Indexer::rebuildIndex() {
    backlinks.clear();
    forwardLinks.clear();

    const auto notes = vault.listAllNotes();
    if (notes.empty()) return;

    const regex linkRegex(R"(\[\[(.*?)\]\])");
    mutex mtx;

    auto worker = [&](size_t start, size_t end) {
        for (size_t i = start; i < end; ++i) {
            string content = notes[i]->getContent();
            for (sregex_iterator it(content.begin(), content.end(), linkRegex), regEnd; it != regEnd; ++it) {
                string target = (*it)[1].str();
                lock_guard<mutex> lock(mtx);
                forwardLinks[notes[i]->getTitle()].push_back(target);
                backlinks[target].push_back(notes[i]->getTitle());
            }
        }
    };

    size_t mid = notes.size() / 2;
    thread t1(worker, 0, mid);
    thread t2(worker, mid, notes.size());

    t1.join();
    t2.join();
}

vector<string> Indexer::searchNotes(const string& query) const {
    vector<string> results;
    for (const auto& note : vault.listAllNotes()) {
        if (note->getTitle().find(query) != string::npos ||
            note->getContent().find(query) != string::npos) {
            results.push_back(note->getTitle());
        }
    }
    return results;
}

vector<string> Indexer::getBacklinks(const string& title) const {
    auto it = backlinks.find(title);
    return (it != backlinks.end()) ? it->second : vector<string>{};
}

vector<string> Indexer::getReachableNotesBFS(const string& startTitle) const {
    unordered_set<string> visited;
    queue<string> q;
    vector<string> reachable;

    q.push(startTitle);
    visited.insert(startTitle);

    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current != startTitle) reachable.push_back(current);

        auto it = forwardLinks.find(current);
        if (it != forwardLinks.end()) {
            for (const auto& neighbor : it->second) {
                if (visited.insert(neighbor).second) q.push(neighbor);
            }
        }
    }
    return reachable;
}