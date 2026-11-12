#ifndef STORAGE_H
#define STORAGE_H

#include "Models.h"
#include <string>

class LocalStorage {
public:
    static void saveNote(const Note& note, const std::string& path);
    static std::string loadText(const std::string& path);
};

#endif
