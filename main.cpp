#include "Models.h"
#include "Core.h"
#include "Templates.h"
#include "Exceptions.h"
#include "Storage.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <limits>

using namespace std;

void previewNoteByValue(Note slicedNote) {
    cout << "  [Preview (Sliced to Base Note)] " << slicedNote << "\n";
}

void printMenu() {
    cout << "\n====================================================\n";
    cout << "                     QUILL CLI                      \n";
    cout << "====================================================\n";
    cout << "  [1] List All Notes\n";
    cout << "  [2] Create Note\n";
    cout << "  [3] Open & Edit Note\n";
    cout << "  [4] Add Task to Note\n";
    cout << "  [5] Search Notes\n";
    cout << "  [6] View Links & Graph\n";
    cout << "  [7] Vault Info & Diagnostics\n";
    cout << "  [0] Exit\n";
    cout << "----------------------------------------------------\n";
    cout << "  Select option > ";
}

string promptLine(const string& label) {
    cout << "  " << label << ": ";
    string input;
    getline(cin, input);
    return input;
}

int main() {
    Vault vault;
    Indexer indexer(vault);
    Registry<string, int> tagRegistry;

    auto n1 = make_shared<Note>("1", "Project Alpha", "Integrate with [[Meeting Notes]].", vector<string>{"work"});
    n1->addTag("priority").addTag("q4");

    auto n2 = make_shared<Note>("2", "Meeting Notes", "Discussed strategy. See [[Project Alpha]].", vector<string>{"work"});

    auto tn1 = make_shared<TaskNote>("3", "Grocery List", "Weekly restock.", vector<string>{"home"});
    tn1->addTask("Milk", false);
    tn1->addTask("Eggs", true);

    auto dtn1 = make_shared<DailyTaskNote>("4", "Daily Standup", "Team sync.", vector<string>{"daily"}, "2026-09-23");
    dtn1->addTask("Review PRs", true);

    auto shn1 = make_shared<ShareableNote>("5", "Public Roadmap", "Shared goals.", vector<string>{"shared"});

    vault.addNote(n1);
    vault.addNote(n2);
    vault.addNote(tn1);
    vault.addNote(dtn1);
    vault.addNote(shn1);

    indexer.rebuildIndex();

    int nextId = 6;
    int choice;

    while (true) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Please enter a valid number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) {
            cout << "\n  Goodbye!\n\n";
            break;
        } else if (choice == 1) {
            auto notes = vault.listAllNotes();
            cout << "\n  Notes in Vault (" << notes.size() << "):\n";
            for (const auto& n : notes) {
                n->display();
            }
        } else if (choice == 2) {
            cout << "\n  --- Create Note ---\n";
            Note temp(to_string(nextId++), "", "");
            cin >> temp;

            string type = promptLine("Convert to TaskNote? (y/n)");
            if (type == "y" || type == "Y") {
                TaskNote promoted = temp;
                vault.addNote(make_shared<TaskNote>(promoted));
            } else {
                vault.addNote(make_shared<Note>(temp));
            }
            indexer.rebuildIndex();
            cout << "  [ok] Note saved and indexed.\n";
        } else if (choice == 3) {
            string id = promptLine("\n  Enter Note ID to open (e.g., 1, 2, 3)");
            try {
                auto note = vault.requireNoteById(id);
                cout << "\n  --- Opened Note ---\n";
                note->display();

                string newContent = promptLine("New content (leave blank to keep current)");
                if (!newContent.empty()) {
                    note->editContent(newContent);
                    indexer.rebuildIndex();
                    cout << "  [ok] Content updated.\n";
                }

                string newTag = promptLine("Add a tag (leave blank to skip)");
                if (!newTag.empty()) {
                    note->addTag(newTag);
                    cout << "  [ok] Tag #" << newTag << " added.\n";
                }

                cout << "\n  --- Updated Note ---\n";
                note->display();
            } catch (const QuillException& e) {
                cout << "  [!] " << e.what() << "\n";
            }
        } else if (choice == 4) {
            string id = promptLine("\n  Enter Note ID (e.g., 3 or 4)");
            Note* basePtr = vault.getRawNotePointer(id);
            if (!basePtr) {
                cout << "  [!] Note not found.\n";
                continue;
            }
            TaskNote* taskPtr = dynamic_cast<TaskNote*>(basePtr);
            if (!taskPtr) {
                cout << "  [!] Note '" << basePtr->getTitle() << "' is not a TaskNote.\n";
            } else {
                string task = promptLine("Task description");
                taskPtr->addTask(task, false);
                cout << "  [ok] Task added to '" << taskPtr->getTitle() << "'.\n";
            }
        } else if (choice == 5) {
            string query = promptLine("\n  Search query");
            auto results = indexer.searchNotes(query);
            cout << "  Matches: " << (results.empty() ? "None" : joinItems(results)) << "\n";
        } else if (choice == 6) {
            indexer.rebuildIndex();
            string title = promptLine("\n  Note title (Enter for 'Project Alpha')");
            if (title.empty()) title = "Project Alpha";

            auto bl = indexer.getBacklinks(title);
            auto bfs = indexer.getReachableNotesBFS(title);
            cout << "  Backlinks : " << (bl.empty() ? "None" : joinItems(bl)) << "\n";
            cout << "  BFS Graph : " << (bfs.empty() ? "None" : joinItems(bfs, " -> ")) << "\n";
        } else if (choice == 7) {
            cout << "\n  --- Vault Info & Diagnostics ---\n";
            cout << "  Total Notes Instantiated : " << Note::getTotalNotesCreated() << "\n";

            for (const auto& n : vault.listAllNotes()) {
                for (const auto& t : n->getTags()) {
                    tagRegistry.set(t, clampValue(tagRegistry.getOrDefault(t, 0) + 1, 1, 99));
                }
            }
            cout << "  Unique Tags Tracked      : " << tagRegistry.size() << "\n";

            Note quick = "Quick memo from const char*";
            int charCount = static_cast<int>(quick);
            cout << "  Basic<->Class Cast Check : '" << quick.getTitle() << "' (" << charCount << " chars)\n";

            Note merged = *n1 + *n2;
            cout << "  Merged Note (+ and <<)   : " << merged << "\n";

            previewNoteByValue(*tn1);

            try {
                LocalStorage::saveNote(*n1, "backup.md");
                vault.requireNoteById("invalid-id");
            } catch (const QuillException& e) {
                cout << "  Exception Guard          : Caught expected error (" << e.what() << ")\n";
            }
        } else {
            cout << "  [!] Invalid option.\n";
        }
    }

    return 0;
}