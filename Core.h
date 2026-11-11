#ifndef CORE_H
#define CORE_H

#include "Models.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

class Vault {
private:
    std::vector<std::shared_ptr<Note>> notes;

public:
    void addNote(std::shared_ptr<Note> note);
    std::vector<std::shared_ptr<Note>> listAllNotes() const;
    std::shared_ptr<Note> requireNoteById(const std::string& id) const;
    Note* getRawNotePointer(const std::string& id) const;
};

class Indexer {
private:
    const Vault& vault;
    std::unordered_map<std::string, std::vector<std::string>> backlinks;
    std::unordered_map<std::string, std::vector<std::string>> forwardLinks;

public:
    explicit Indexer(const Vault& vault);
    void rebuildIndex();
    std::vector<std::string> searchNotes(const std::string& query) const;
    std::vector<std::string> getBacklinks(const std::string& title) const;
    std::vector<std::string> getReachableNotesBFS(const std::string& startTitle) const;
};

#endif