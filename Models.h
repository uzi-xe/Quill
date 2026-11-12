#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include "Exceptions.h"

class Note {
public:
    class Metadata {
    public:
        int editCount = 0;
        void recordEdit() { editCount++; }
    };

protected:
    std::string id;
    std::string title;
    std::string content;
    std::vector<std::string> tags;
    static int totalNotesCreated;

public:
    Metadata meta;

    static int getTotalNotesCreated();

    Note(std::string id, std::string title, std::string content, std::vector<std::string> tags = {});
    virtual ~Note() = default;

    Note(const char* quickText);
    explicit operator int() const;

    void editContent(const std::string& newContent);
    Note& addTag(const std::string& tag);

    virtual void display() const;
    virtual std::string getTypeName() const;

    std::string getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getContent() const { return content; }
    std::vector<std::string> getTags() const { return tags; }

    friend std::ostream& operator<<(std::ostream& os, const Note& note);
    friend std::istream& operator>>(std::istream& is, Note& note);
    friend Note operator+(const Note& lhs, const Note& rhs);
};

class TaskNote : public Note {
protected:
    std::vector<std::pair<std::string, bool>> tasks;

public:
    TaskNote(std::string id, std::string title, std::string content, std::vector<std::string> tags = {});
    TaskNote(const Note& source);

    void addTask(const std::string& task, bool isDone = false);
    void display() const override;
    std::string getTypeName() const override { return "TaskNote"; }
};

class DailyTaskNote final : public TaskNote {
private:
    std::string date;

public:
    DailyTaskNote(std::string id, std::string title, std::string content, std::vector<std::string> tags, std::string date);
    void display() const final override;
    std::string getTypeName() const override { return "DailyTaskNote"; }
};

class Entity {
public:
    std::string entityId = "entity-0";
    virtual ~Entity() = default;
};

class Readable : virtual public Entity {
public:
    std::string readStatus() const { return "Readable[" + entityId + "]"; }
};

class Writable : virtual public Entity {
public:
    std::string writeStatus() const { return "Writable[" + entityId + "]"; }
};

class ShareableNote : public Note, public Readable, public Writable {
public:
    ShareableNote(std::string id, std::string title, std::string content, std::vector<std::string> tags = {});
    void display() const override;
    std::string getTypeName() const override { return "ShareableNote"; }
};

#endif