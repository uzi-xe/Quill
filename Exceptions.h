#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class QuillException : public std::runtime_error {
public:
    explicit QuillException(const std::string& msg) : std::runtime_error(msg) {}
};

class NoteNotFoundException final : public QuillException {
public:
    explicit NoteNotFoundException(const std::string& id)
        : QuillException("Note ID '" + id + "' not found.") {}
};

class InvalidDateException final : public QuillException {
public:
    explicit InvalidDateException(const std::string& date)
        : QuillException("Invalid date '" + date + "'. Expected YYYY-MM-DD.") {}
};

class StorageException final : public QuillException {
public:
    explicit StorageException(const std::string& path)
        : QuillException("File error: '" + path + "'.") {}
};

#endif