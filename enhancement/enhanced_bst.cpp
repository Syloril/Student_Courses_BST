#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

// Data Structures

struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;

        Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int size;

    // Recursive helper to delete nodes and prevent memory leaks
    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    // Recursive helper for InOrder traversal
    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.courseId << ", " << node->course.courseName << endl;
        inOrder(node->right);
    }

public:
    BinarySearchTree() : root(nullptr), size(0) {}

    // Destructor clears memory automatically
    ~BinarySearchTree() {
        destroyTree(root);
    }

    int Size() { return size; }

    void InOrder() {
        inOrder(root);
    }

    void Insert(Course aCourse) {
        if (root == nullptr) {
            root = new Node(aCourse);
        } else {
            Node* cur = root;
            while (cur != nullptr) {
                if (aCourse.courseId < cur->course.courseId) {
                    if (cur->left == nullptr) {
                        cur->left = new Node(aCourse);
                        break;
                    }
                    cur = cur->left;
                } else {
                    if (cur->right == nullptr) {
                        cur->right = new Node(aCourse);
                        break;
                    }
                    cur = cur->right;
                }
            }
        }
        size++;
    }

    Course Search(string courseId) {
        Node* cur = root;
        while (cur != nullptr) {
            if (cur->course.courseId == courseId) {
                return cur->course;
            }
            if (courseId < cur->course.courseId) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return Course(); // Return empty course if not found
    }
};

// Helper Functions

vector<string> Split(string line) {
    vector<string> tokens;
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) {
        // Remove potential carriage return from Windows-style line endings
        if (!item.empty() && item.back() == '\r') item.pop_back();
        tokens.push_back(item);
    }
    return tokens;
}

void convertCase(string& toConvert) {
    for (char& c : toConvert) {
        c = toupper(static_cast<unsigned char>(c));
    }
}

void displayCourse(Course aCourse) {
    cout << "\n" << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";
    if (aCourse.preList.empty()) {
        cout << "none";
    } else {
        for (size_t i = 0; i < aCourse.preList.size(); ++i) {
            cout << aCourse.preList[i] << (i < aCourse.preList.size() - 1 ? ", " : "");
        }
    }
    cout << endl;
}

void loadCourses(string csvPath, BinarySearchTree& bst) {
    ifstream inFS(csvPath);
    if (!inFS.is_open()) {
        cout << "Error: Could not open file " << csvPath << endl;
        return;
    }

    string line;
    int count = 0;
    while (getline(inFS, line)) {
        if (line.empty()) continue;

        vector<string> tokens = Split(line);
        if (tokens.size() < 2) {
            cout << "Error: Skipping malformed line." << endl;
            continue;
        }

        Course c;
        c.courseId = tokens[0];
        c.courseName = tokens[1];
        for (size_t i = 2; i < tokens.size(); ++i) {
            c.preList.push_back(tokens[i]);
        }
        bst.Insert(c);
        count++;
    }
    inFS.close();
    cout << count << " courses read successfully." << endl;
}

// Main Execution

int main(int argc, char* argv[]) {
    string csvPath = "ABCU_Advising_Program_Input.csv";
    if (argc >= 2) csvPath = argv[1];

    BinarySearchTree courseList;
    int choice = 0;

    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit." << endl;
        cout << "What would you like to do? ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number (1, 2, 3, or 9)." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            loadCourses(csvPath, courseList);
            break;
        case 2:
            cout << "\nHere the available courses:" << endl;
            courseList.InOrder();
            break;
        case 3: {
            string searchId;
            cout << "What course do you want to know about? ";
            cin >> searchId;
            convertCase(searchId);
            Course found = courseList.Search(searchId);
            if (!found.courseId.empty()) {
                displayCourse(found);
            } else {
                cout << "Course ID " << searchId << " not found." << endl;
            }
            break;
        }
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}