#include "Models.h"
#include "Templates.h"
#include <regex>

using namespace std;

int Note::totalNotesCreated = 0;

int Note::getTotalNotesCreated() {
    return totalNotesCreated;
}

Note::Note(string id, string title, string content, vector<string> tags)
    : id(move(id)), title(move(title)), content(move(content)), tags(move(tags)) {
    totalNotesCreated++;
}

Note::Note(const char* quickText)
    : Note("quick-" + to_string(totalNotesCreated + 1), "Quick Note", quickText, {"quick"}) {}

Note::operator int() const {
    return static_cast<int>(content.length());
}

void Note::editContent(const string& newContent) {
    this->content = newContent;
    this->meta.recordEdit();
}

Note& Note::addTag(const string& tag) {
    this->tags.push_back(tag);
    this->meta.recordEdit();
    return *this;
}

void Note::display() const {
    cout << "  +--------------------------------------------------\n";
    cout << "  | [" << getTypeName() << "] " << title << " (ID: " << id << ")\n";
    if (!tags.empty()) {
        cout << "  | Tags   : #" << joinItems(tags, " #") << "\n";
    }
    cout << "  | Content: " << content << " (" << static_cast<int>(*this) << " chars, " << meta.editCount << " edits)\n";
    cout << "  +--------------------------------------------------\n";
}

string Note::getTypeName() const { return "Note"; }

ostream& operator<<(ostream& os, const Note& note) {
    os << "[" << note.getTypeName() << "] " << note.title << ": " << note.content;
    return os;
}

istream& operator>>(istream& is, Note& note) {
    cout << "  Title  : ";
    getline(is, note.title);
    cout << "  Content: ";
    getline(is, note.content);
    return is;
}

Note operator+(const Note& lhs, const Note& rhs) {
    vector<string> mergedTags = lhs.tags;
    mergedTags.insert(mergedTags.end(), rhs.tags.begin(), rhs.tags.end());
    return Note(lhs.id + "+" + rhs.id, lhs.title + " & " + rhs.title, lhs.content + " | " + rhs.content, mergedTags);
}

TaskNote::TaskNote(string id, string title, string content, vector<string> tags)
    : Note(move(id), move(title), move(content), move(tags)) {}

TaskNote::TaskNote(const Note& source)
    : Note(source.getId(), source.getTitle(), source.getContent(), source.getTags()) {
    addTask("Follow up on: " + source.getTitle(), false);
}

void TaskNote::addTask(const string& task, bool isDone) {
    tasks.push_back({task, isDone});
}

void TaskNote::display() const {
    Note::display();
    if (!tasks.empty()) {
        for (const auto& t : tasks) {
            cout << "  |   " << (t.second ? "[x] " : "[ ] ") << t.first << "\n";
        }
        cout << "  +--------------------------------------------------\n";
    }
}

DailyTaskNote::DailyTaskNote(string id, string title, string content, vector<string> tags, string date)
    : TaskNote(move(id), move(title), move(content), move(tags)), date(move(date)) {
    static const regex isoDate(R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])$)");
    if (!regex_match(this->date, isoDate)) {
        throw InvalidDateException(this->date);
    }
}

void DailyTaskNote::display() const {
    cout << "  +--------------------------------------------------\n";
    cout << "  | [Daily - " << date << "] " << title << " (ID: " << id << ")\n";
    cout << "  | Content: " << content << "\n";
    for (const auto& t : tasks) {
        cout << "  |   " << (t.second ? "[x] " : "[ ] ") << t.first << "\n";
    }
    cout << "  +--------------------------------------------------\n";
}

ShareableNote::ShareableNote(string id, string title, string content, vector<string> tags)
    : Note(move(id), move(title), move(content), move(tags)) {
    this->entityId = "shared-" + this->id;
}

void ShareableNote::display() const {
    Note::display();
    cout << "  | Access : " << readStatus() << ", " << writeStatus() << "\n";
    cout << "  +--------------------------------------------------\n";
}