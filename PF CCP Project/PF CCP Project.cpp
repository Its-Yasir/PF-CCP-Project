#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> // For sort
#include <cstdlib>   // For system("cls")
#include <limits>    // For numeric_limits

using namespace std;

struct Student {
    string id;
    string name;
    float gpa;
};

vector<Student> students;
const string FILENAME = "students.txt";

// --- HELPER FUNCTIONS ---

// Comparator for Sorting (Highest GPA first)
bool compareGPA(const Student& a, const Student& b) {
    return a.gpa > b.gpa;
}

float calculateGPA(float obtained, float total) {
    float percentage = (obtained / total) * 100;

    if (percentage >= 85) return 4.0;
    if (percentage >= 80) return 3.7;
    if (percentage >= 75) return 3.3;
    if (percentage >= 70) return 3.0;
    if (percentage >= 65) return 2.7;
    if (percentage >= 60) return 2.3;
    if (percentage >= 55) return 2.0;
    if (percentage >= 50) return 1.7;
    return 0.0;
}

// --- FILE FUNCTIONS ---

void loadData() {
    students.clear();
    ifstream inFile(FILENAME);
    if (!inFile) return;

    Student s;
    while (inFile >> s.id >> s.name >> s.gpa) {
        students.push_back(s);
    }
    inFile.close();
}

void rewriteFile() {
    ofstream outFile(FILENAME);
    if (outFile.is_open()) {
        for (const auto& s : students) {
            outFile << s.id << " " << s.name << " " << s.gpa << endl;
        }
        outFile.close();
    }
}

void appendToFile(const Student& s) {
    ofstream outFile(FILENAME, ios::app);
    if (outFile.is_open()) {
        outFile << s.id << " " << s.name << " " << s.gpa << endl;
        outFile.close();
    }
}

// --- MENU FUNCTIONS ---

void addStudent() {
    system("cls");
    Student s;
    cout << "\n--- Add New Student ---\n";
    cout << "Enter Roll No (e.g., 2025-SE-450): ";
    cin >> s.id;

    for (const auto& existing : students) {
        if (existing.id == s.id) {
            cout << "Error: Roll Number already exists!\n";
            return;
        }
    }

    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, s.name);

    // Convert spaces to underscores
    for (int i = 0; i < s.name.length(); i++) {
        if (s.name[i] == ' ') s.name[i] = '_';
    }

    float marks[5];
    float totalObtained = 0;
    cout << "Enter marks for 5 subjects (0-100 only):\n";

    for (int i = 0; i < 5; i++) {
        while (true) {
            cout << "Subject " << i + 1 << ": ";
            cin >> marks[i];

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number.\n";
            }
            else if (marks[i] < 0 || marks[i] > 100) {
                cout << "Error: Marks must be between 0 and 100!\n";
            }
            else {
                break;
            }
        }
        totalObtained += marks[i];
    }

    s.gpa = calculateGPA(totalObtained, 500.0);

    cout << "Total Marks: " << totalObtained << "/500\n";
    cout << "Calculated GPA: " << s.gpa << "\n";

    students.push_back(s);
    appendToFile(s);

    cout << "\nSuccess: Student added and saved.\n";
}

void rankStudents() {
    system("cls");
    if (students.empty()) {
        cout << "\nNo records found.\n";
        return;
    }

    // Create a temporary copy to sort so we don't mess up file order
    vector<Student> sortedList = students;
    sort(sortedList.begin(), sortedList.end(), compareGPA);

    cout << "\n--- STUDENT RANKINGS (By GPA) ---\n";
    cout << "Rank\tRoll No\t\tGPA\tName\n";
    cout << "------------------------------------------------\n";

    for (size_t i = 0; i < sortedList.size(); i++) {
        cout << "#" << i + 1 << "\t"
            << sortedList[i].id << "\t"
            << sortedList[i].gpa << "\t"
            << sortedList[i].name << endl;
    }
    cout << "------------------------------------------------\n";
}

void viewTopper() {
    system("cls");
    if (students.empty()) {
        cout << "\nNo records found.\n";
        return;
    }

    float maxGPA = -1.0;
    for (const auto& s : students) {
        if (s.gpa > maxGPA) maxGPA = s.gpa;
    }

    cout << "\n--- CLASS TOPPER(S) ---\n";
    for (const auto& s : students) {
        if (s.gpa == maxGPA) {
            cout << "Roll No: " << s.id << " | GPA: " << s.gpa << " | Name: " << s.name << endl;
        }
    }
}

void displayStudents() {
    system("cls");
    if (students.empty()) {
        cout << "\nNo records found.\n";
        return;
    }
    cout << "\n--- All Student Records ---\n";
    cout << "Roll No\t\tName\t\tGPA\n";
    cout << "----------------------------------------\n";
    for (const auto& s : students) {
        cout << s.id << "\t" << s.name << "\t\t" << s.gpa << endl;
    }
}

void searchStudent() {
    system("cls");
    string searchId;
    cout << "\n--- Search Student ---\n";
    cout << "Enter Roll No: ";
    cin >> searchId;

    bool found = false;
    for (const auto& s : students) {
        if (s.id == searchId) {
            cout << "\nFound: " << s.id << " | " << s.name << " | GPA: " << s.gpa << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << "\nStudent not found.\n";
}

void updateStudent() {
    system("cls");
    string updateId;
    cout << "\n--- Update Student ---\n";
    cout << "Enter Roll No: ";
    cin >> updateId;

    bool found = false;
    for (auto& s : students) {
        if (s.id == updateId) {
            cin.ignore();
            cout << "Enter New Name: ";
            getline(cin, s.name);
            for (int i = 0; i < s.name.length(); i++) {
                if (s.name[i] == ' ') s.name[i] = '_';
            }

            cout << "Enter New GPA: ";
            while (!(cin >> s.gpa) || s.gpa < 0 || s.gpa > 4.0) {
                cout << "Invalid GPA. Enter 0.0 - 4.0: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            rewriteFile();
            cout << "\nUpdated successfully.\n";
            found = true;
            break;
        }
    }
    if (!found) cout << "\nStudent not found.\n";
}

void deleteStudent() {
    system("cls");
    string deleteId;
    cout << "\n--- Delete Student ---\n";
    cout << "Enter Roll No: ";
    cin >> deleteId;

    bool found = false;
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].id == deleteId) {
            students.erase(students.begin() + i);
            rewriteFile();
            cout << "\nDeleted successfully.\n";
            found = true;
            break;
        }
    }
    if (!found) cout << "\nStudent not found.\n";
}

int main() {
    loadData();

    int choice;
    while (true) {
        system("cls");
        cout << "\n=== University Management System ===\n";
        cout << "1. Add Student\n";
        cout << "2. Display All\n";
        cout << "3. Search Student\n";
        cout << "4. View Topper\n";
        cout << "5. Rank Students (Leaderboard)\n";
        cout << "6. Update Student\n";
        cout << "7. Delete Student\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";

        // --- MENU VALIDATION ---
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
            cout << "\nInvalid input! Please enter a number.\n";
            system("pause");
            continue;
        }

        switch (choice) {
        case 1: addStudent(); break;
        case 2: displayStudents(); break;
        case 3: searchStudent(); break;
        case 4: viewTopper(); break;
        case 5: rankStudents(); break;
        case 6: updateStudent(); break;
        case 7: deleteStudent(); break;
        case 8: cout << "Exiting...\n"; return 0;
        default: cout << "Invalid choice!\n";
        }
        cout << "\n";
        system("pause");
    }

    return 0;
}