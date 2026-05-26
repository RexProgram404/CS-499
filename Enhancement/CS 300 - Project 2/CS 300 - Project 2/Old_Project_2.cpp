#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;


const unsigned int DEFAULT_SIZE = 179;


// Bid info
struct Course {
    string courseID;
    string title;
    vector<string> prerequisite;
};
class HashTable
{
private:
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        // initialize with a bid and a key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(string key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void PrintAll();
    Course Search(string courseId);
    void Insert(Course course);
    void displayCourse(Course course);
};

HashTable::HashTable() {
    nodes.resize(tableSize);
    // Initalize node structure by resizing tableSize
}

HashTable::HashTable(unsigned int size) {
    // resize nodes size
    this->tableSize = size;
    nodes.resize(tableSize);//sets nodes size to new tablesize
}

HashTable::~HashTable() {
    nodes.erase(nodes.begin());
    // erase nodes beginning
}

unsigned int HashTable::hash(string key) {
    unsigned int hashKey = 0;
    for (char c : key) {
        hashKey += c;
    }

    // return key tableSize
    return hashKey % tableSize;
}

void HashTable::Insert(Course course) {
    unsigned thisKey = hash(course.courseID);
    // retrieve node using key
    Node* oldNode = &(nodes.at(thisKey));

    if (oldNode == nullptr) {    // if no entry found for the key
        // assign this node to the key position
        Node* newNode = new Node(course, thisKey);//creates node

        nodes.insert(nodes.begin() + thisKey, (*newNode));//inserts new node
    }
    // else if node is not used
    else {
        if (oldNode->key == UINT_MAX) {
            oldNode->key = thisKey;
            oldNode->course = course;
            oldNode->next = nullptr;
        }
        // else find the next open node
        else {
            // add new newNode to end
            while (oldNode->next != nullptr) {
                oldNode = oldNode->next;
            }
            oldNode->next = new Node(course, thisKey);
        }
    }
}

void HashTable::PrintAll() {
    for (const auto& node : nodes) {
        if (node.key != UINT_MAX) {
            cout << node.course.courseID << ", " << node.course.title << endl;
        }
        Node* currNode = new Node(node.course, node.key);
        while(currNode != nullptr) {
            currNode = currNode->next;
            cout << currNode->course.courseID << ", " << currNode->course.title << endl;
        }
    }
}

void loadCourses(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    ifstream inputFile(csvPath);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file " << csvPath << std::endl;
    }
    
    string line;
    while (getline(inputFile, line)) {
        vector<string> parts;
        stringstream currLine(line);
        string segment;
        while (getline(currLine, segment, ',')) {
            if (!segment.empty()) {
                parts.push_back(segment);
            }
        }
        Course course;
        if (parts.size() < 2) {
            cout << "Error: Course |" << parts[0] << "| not formated correctly";
        }
        else {
            course.courseID = parts[0];
            parts.erase(parts.begin());
            course.title = parts[0];
            parts.erase(parts.begin());
            if (parts.size() > 0) {  
                course.prerequisite.insert(course.prerequisite.end(), parts.begin(), parts.end());
            }
        }
        hashTable->Insert(course);
    }
    inputFile.close();
}

Course HashTable::Search(string courseId) {
    Course course;
    unsigned int thisKey = hash(courseId);
    Node* node = &(nodes.at(thisKey));
    // if entry found for the key
    if (node != nullptr && node->key != UINT_MAX && node->course.courseID.compare(courseId) == 0) {//if entry is found
        //return node course
        return node->course;
    }
    // if no entry found for the key
    if (node == nullptr || node->key == UINT_MAX) {//entry is not found
        // return bid
        return course;
    }
    // while node not equal to nullptr

    while (node != nullptr) {
        // if the current node matches, return it
        if (node->key != UINT_MAX && node->course.courseID.compare(courseId) == 0) {
            return node->course;//return bid
        }
        //node is equal to next node
        node = node->next;
    }

    return course;
}

void HashTable::displayCourse(Course course) {
    cout << course.courseID << ", " << course.title << endl;
    if (course.prerequisite.size() > 0) {
        cout << "Prerequisite:";
            for (int i = 0; i < course.prerequisite.size(); i++) {
                cout << " " << course.prerequisite[i];
                if (i + 1 < course.prerequisite.size()) {
                    cout << ",";
                }
            }
            cout << endl;
    }
}

int main(int argc, char* argv[]) {
    string file, courseKey;
    switch (argc) {
    case 2:
        file = argv[1];
        courseKey = "CSCI100";
        break;
    case 3:
        file = argv[1];
        courseKey = argv[2];
        break;
    default:
        file = "CS 300 ABCU_Advising_Program_Input.csv";
        courseKey = "CSCI100";
    }
    HashTable* courseTable;
    Course course;
    courseTable = new HashTable();

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            // Complete the method call to load the bids
            loadCourses(file, courseTable);
            break;

        case 2:
            courseTable->PrintAll();
            break;

        case 3:

            cout << "Which course are you looking for(Case Sensitive)?";
            cin >> courseKey;
            course = courseTable->Search(courseKey);
            if (!course.courseID.empty()) {
                courseTable->displayCourse(course);
            }
            else {
                cout << "Course Id " << courseKey << " not found." << endl;
            }
            break;

        default:
            cout << choice << " is not a valid option";
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}