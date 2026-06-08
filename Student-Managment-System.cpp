/*
 * Project: Advanced Student Management System (Dynamic Analytics Edition)
 * Language: C++
 * Due Date: June 08, 2026
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits> 
#include <cstring> // Required for safe character array copying (strncpy)

using namespace std;

// Allocate enough memory for a maximum capacity up to 10 subjects
const int MAX_SUBJECTS = 10;
const string FILE_NAME = "students.dat";

// Class representing an Advanced Student Record
class Student {
public:
    int rollNumber;
    char name[50];
    char course[50];
    
    // Academic Data Elements
    int activeSubjects; 
    char subjects[MAX_SUBJECTS][50]; 
    float marks[MAX_SUBJECTS];
    
    int totalClasses;
    int attendedClasses;
    float cgpa;

    // Automatically computes 10.0 scale CGPA
    void calculateCGPA() {
        if (activeSubjects == 0) {
            cgpa = 0.0f;
            return;
        }
        float totalMarksObtained = 0;
        for (int i = 0; i < activeSubjects; i++) {
            totalMarksObtained += marks[i];
        }
        float averageMarks = totalMarksObtained / activeSubjects;
        cgpa = averageMarks / 10.0f; 
    }

    // Calculates current attendance rate percentage
    float getAttendancePercentage() const {
        if (totalClasses == 0) return 0.0f;
        return (static_cast<float>(attendedClasses) / totalClasses) * 100.0f;
    }

    // ADDITION 1: Automated Letter Grade Mapping Engine
    string getLetterGrade() const {
        if (cgpa >= 9.0f) return "A+";
        if (cgpa >= 8.0f) return "A";
        if (cgpa >= 7.0f) return "B";
        if (cgpa >= 6.0f) return "C";
        if (cgpa >= 5.0f) return "D";
        return "F (Fail)";
    }

    void inputStudent(bool askRollNumber = true) {
        if (askRollNumber) {
            cout << "Enter Roll Number: ";
            while (!(cin >> rollNumber)) { 
                cout << "Invalid input! Enter a valid numerical Roll Number: ";
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
        
        string tempStudentName = "";
        cout << "Enter Name: ";
        getline(cin, tempStudentName);
        if (tempStudentName.empty()) {
            getline(cin, tempStudentName); 
        }
        strncpy(name, tempStudentName.c_str(), 49);
        name[49] = '\0';
        
        string tempCourseName = "";
        cout << "Enter Course: ";
        getline(cin, tempCourseName);
        if (tempCourseName.empty()) {
            getline(cin, tempCourseName);
        }
        strncpy(course, tempCourseName.c_str(), 49);
        course[49] = '\0';

        cout << "Enter Number of Subjects for this student (Max " << MAX_SUBJECTS << "): ";
        while (!(cin >> activeSubjects) || activeSubjects < 1 || activeSubjects > MAX_SUBJECTS) {
            cout << "Invalid entry! Please enter a number between 1 and " << MAX_SUBJECTS << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "\n--- Entering Details for " << activeSubjects << " Subjects ---\n";
        
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < activeSubjects; i++) {
            string tempSubjectName = "";
            
            cout << "Enter Name for Subject " << i + 1 << ": ";
            getline(cin, tempSubjectName); 
            
            if (tempSubjectName.empty()) {
                getline(cin, tempSubjectName);
            }

            strncpy(subjects[i], tempSubjectName.c_str(), 49);
            subjects[i][49] = '\0'; 
            
            cout << "Enter Marks Obtained in " << subjects[i] << " (0-100): ";
            while (!(cin >> marks[i]) || marks[i] < 0 || marks[i] > 100) {
                cout << "Invalid Marks! Enter a value between 0 and 100: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "\nEnter Total Classes Conducted: ";
        while (!(cin >> totalClasses) || totalClasses < 0) {
            cout << "Invalid input! Enter a positive integer: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter Total Classes Attended: ";
        while (!(cin >> attendedClasses) || attendedClasses < 0 || attendedClasses > totalClasses) {
            cout << "Invalid input! Cannot exceed total classes (" << totalClasses << "): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        calculateCGPA(); 
    }

    void displayStudentSummary() const {
        float attendancePct = getAttendancePercentage();
        cout << left << setw(10) << rollNumber 
             << setw(18) << name 
             << setw(12) << course 
             << setw(8) << fixed << setprecision(2) << cgpa
             << setw(12) << setprecision(1) << attendancePct << "%";
             
        if (attendancePct < 75.0f) {
            cout << " [! Low Attendance]";
        }
        cout << endl;
    }

    void displayDetailedReport() const {
        cout << "\n=============================================";
        cout << "\n          DETAILED ACADEMIC REPORT           ";
        cout << "\n=============================================";
        cout << "\nRoll Number : " << rollNumber;
        cout << "\nName        : " << name;
        cout << "\nCourse      : " << course;
        cout << "\n---------------------------------------------";
        cout << "\nSUBJECT WISE MARKS CARD:";
        for (int i = 0; i < activeSubjects; i++) {
            cout << "\n  - " << left << setw(25) << subjects[i] << ": " << marks[i] << " / 100";
        }
        cout << "\n---------------------------------------------";
        cout << "\nCalculated CGPA       : " << fixed << setprecision(2) << cgpa << " / 10.00";
        cout << "\nAssigned Merit Grade  : " << getLetterGrade(); // Prints the custom scale letter grade
        cout << "\nAttendance Percentage : " << setprecision(1) << getAttendancePercentage() << "% (" 
             << attendedClasses << "/" << totalClasses << " Classes)";
        cout << "\n=============================================\n";
    }
};

// Function declarations
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void displayToppers();      // ADDITION 2: Toppers Filter
void generateAnalytics();   // ADDITION 3: Batch Performance Metrics Calculator
void resetDatabase();
bool checkDuplicateRoll(int roll); 

int main() {
    int choice;
    do {
        cout << "\n=========================================";
        cout << "\n   ANALYTICAL STUDENT MANAGEMENT SYSTEM  ";
        cout << "\n=========================================";
        cout << "\n1. Add New Student & Academic Record";
        cout << "\n2. Display All Performance Summaries";
        cout << "\n3. Generate Detailed Academic Report";
        cout << "\n4. Update Student Information";
        cout << "\n5. Delete Student Record";
        cout << "\n6. View High Performers (Toppers Dashboard)";
        cout << "\n7. Generate Institutional Performance Analytics";
        cout << "\n8. Reset Database (Wipe All Data)";
        cout << "\n9. Exit";
        cout << "\n=========================================";
        cout << "\nEnter your choice (1-9): ";
        
        while (!(cin >> choice)) {
            cout << "Invalid option! Enter a number (1-9): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent(); break; 
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: displayToppers(); break;
            case 7: generateAnalytics(); break;
            case 8: resetDatabase(); break;
            case 9: cout << "\nExiting system. Goodbye!\n"; break;
            default: cout << "\nInvalid choice! Please select 1-9.\n";
        }
    } while (choice != 9);
    return 0;
}

bool checkDuplicateRoll(int roll) {
    Student s;
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) return false; 

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNumber == roll) {
            inFile.close();
            return true; 
        }
    }
    inFile.close();
    return false;
}

void addStudent() {
    Student s;
    int inputRoll;

    cout << "\n--- Register Student Profile ---\n";
    
    while (true) {
        cout << "Enter Roll Number: ";
        while (!(cin >> inputRoll)) { 
            cout << "Invalid input! Enter a valid numerical Roll Number: ";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (checkDuplicateRoll(inputRoll)) {
            cout << "Error: Roll Number " << inputRoll << " already exists!\n\n";
        } else {
            s.rollNumber = inputRoll; 
            break;
        }
    }

    cin.clear();
    s.inputStudent(false);

    ofstream outFile(FILE_NAME, ios::binary | ios::app);
    if (!outFile) {
        cout << "\nError opening system storage!\n";
        return;
    }
    outFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
    outFile.close();
    cout << "\nAcademic profile added successfully!\n";
}

void displayAllStudents() {
    Student s;
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "\nNo student records found.\n";
        return;
    }
    cout << "\n-----------------------------------------------------------------------\n";
    cout << left << setw(10) << "Roll No" << setw(18) << "Name" << setw(12) << "Course" << setw(8) << "CGPA" << setw(12) << "Attendance" << "Status" << endl;
    cout << "-----------------------------------------------------------------------\n";
    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        s.displayStudentSummary();
    }
    cout << "-----------------------------------------------------------------------\n";
    inFile.close();
}

void searchStudent() {
    Student s;
    ifstream inFile(FILE_NAME, ios::binary);
    int targetRoll;
    bool found = false;
    if (!inFile) {
        cout << "\nStorage data unavailable.\n";
        return;
    }
    cout << "\nEnter Student Roll Number to generate report card: ";
    while (!(cin >> targetRoll)) {
        cout << "Invalid entry! Enter a numerical Roll Number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNumber == targetRoll) {
            found = true;
            s.displayDetailedReport();
            break;
        }
    }
    if (!found) cout << "\nStudent with Roll Number " << targetRoll << " not found.\n";
    inFile.close();
}

void updateStudent() {
    Student s;
    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    int targetRoll;
    bool found = false;
    if (!file) {
        cout << "\nStorage data unavailable.\n";
        return;
    }
    cout << "\nEnter Roll Number of the record to alter: ";
    while (!(cin >> targetRoll)) {
        cout << "Invalid entry! Enter a numerical Roll Number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNumber == targetRoll) {
            found = true;
            cout << "\nCurrent Academic Profile Summary:\n";
            s.displayDetailedReport();
            
            cout << "\nEnter New Profiling/Academic Metrics Details:\n";
            
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            s.inputStudent(false);
            s.rollNumber = targetRoll; 
            
            int pos = static_cast<int>(file.tellg()) - sizeof(Student);
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&s), sizeof(Student));
            cout << "\nAcademic profile modified securely!\n";
            break;
        }
    }
    if (!found) cout << "\nRecord with Roll Number " << targetRoll << " not found.\n";
    file.close();
}

void deleteStudent() {
    Student s;
    ifstream inFile(FILE_NAME, ios::binary);
    int targetRoll;
    bool found = false;
    if (!inFile) {
        cout << "\nStorage data unavailable.\n";
        return;
    }
    cout << "\nEnter Roll Number to delete: ";
    while (!(cin >> targetRoll)) {
        cout << "Invalid entry! Enter a numerical Roll Number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    ofstream outFile("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNumber == targetRoll) {
            found = true;
        } else {
            outFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
        }
    }
    inFile.close();
    outFile.close();
    remove(FILE_NAME.c_str());
    rename("temp.dat", FILE_NAME.c_str());
    if (found) cout << "\nRecord wiped from memory successfully!\n";
    else cout << "\nRecord with Roll Number " << targetRoll << " not found.\n";
}

// ADDITION 2: High Performers Filter Dashboard
void displayToppers() {
    Student s;
    ifstream inFile(FILE_NAME, ios::binary);
    bool found = false;

    if (!inFile) {
        cout << "\nNo data records found to calculate filters.\n";
        return;
    }

    cout << "\n=======================================================================\n";
    cout << "             HIGH PERFORMING MERIT LIST (CGPA >= 8.50)                 \n";
    cout << "=======================================================================\n";
    cout << left << setw(10) << "Roll No" << setw(18) << "Name" << setw(12) << "Course" << setw(8) << "CGPA" << setw(12) << "Attendance" << "Status" << endl;
    cout << "-----------------------------------------------------------------------\n";

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.cgpa >= 8.5f) {
            s.displayStudentSummary();
            found = true;
        }
    }
    cout << "-----------------------------------------------------------------------\n";
    if (!found) {
        cout << "            No records currently match the elite CGPA bracket.\n";
    }
    inFile.close();
}

// ADDITION 3: Class Performance Analytics (Averages)
void generateAnalytics() {
    Student s;
    ifstream inFile(FILE_NAME, ios::binary);
    
    if (!inFile) {
        cout << "\nNo metrics data available yet.\n";
        return;
    }

    int totalRecords = 0;
    int lowAttendanceCount = 0;
    float cumulativeCGPA = 0.0f;
    float cumulativeAttendance = 0.0f;
    float highestCGPA = 0.0f;
    char topScorerName[50] = "None";

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        totalRecords++;
        cumulativeCGPA += s.cgpa;
        float currentAttendance = s.getAttendancePercentage();
        cumulativeAttendance += currentAttendance;

        if (currentAttendance < 75.0f) {
            lowAttendanceCount++;
        }

        if (s.cgpa > highestCGPA) {
            highestCGPA = s.cgpa;
            strcpy(topScorerName, s.name);
        }
    }
    inFile.close();

    if (totalRecords == 0) {
        cout << "\nDatabase is empty. Add records to parse insights.\n";
        return;
    }

    cout << "\n=============================================";
    cout << "\n       BATCH HEALTH & METRICS REPORT        ";
    cout << "\n=============================================";
    cout << "\n Total Registered Profiles : " << totalRecords;
    cout << "\n Classroom Mean CGPA       : " << fixed << setprecision(2) << (cumulativeCGPA / totalRecords) << " / 10.00";
    cout << "\n Classroom Mean Attendance : " << setprecision(1) << (cumulativeAttendance / totalRecords) << "%";
    cout << "\n Low Attendance Shortages  : " << lowAttendanceCount << " Student(s)";
    cout << "\n Current Batch Top Scorer  : " << topScorerName << " (" << fixed << setprecision(2) << highestCGPA << " CGPA)";
    cout << "\n=============================================\n";
}

void resetDatabase() {
    char confirmation;
    cout << "\n[CRITICAL WARNING] Clear all database records permanently? (y/n): ";
    cin >> confirmation;
    if (confirmation == 'y' || confirmation == 'Y') {
        remove(FILE_NAME.c_str());
        cout << "\nAll data wiped cleanly. Ready for fresh profiling.\n";
    } else {
        cout << "\nOperation aborted safely.\n";
    }
}