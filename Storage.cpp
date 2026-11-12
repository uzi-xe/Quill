#include "Storage.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>

using namespace std;

void LocalStorage::saveNote(const Note& note, const std::string& path) {
    ofstream file(path);
    if (!file.is_open()) throw StorageException(path);
    file << "# " << note.getTitle() << "\n\n" << note.getContent() << "\n";
}

string LocalStorage::loadText(const string& path) {
    ifstream file(path);
    if (!file.is_open()) throw StorageException(path);
    ostringstream text;
    text << file.rdbuf();
    return text.str();
}