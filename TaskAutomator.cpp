#include "TaskAutomator.h"
#include "ProcessManager.h"


// Queue using the linked list

// Linked List Node
struct Node {
    std::string data;
    Node* next;

    Node(const std::string& value) : data(value), next(nullptr) {}
};

// Linked List
class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    // Function to add a node to the end of the linked list
    void addNode(const std::string& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }


    std::string front() const {
        return head ? head->data : "";
    }


    void popFront() {
        if (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Function to check if the linked list is empty
    bool isEmpty() const {
        return !head;
    }

    // Destructor to free allocated memory
    ~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class Queue {
private:
    LinkedList list;

public:
    // Function to enqueue an element
    void enqueue(const std::string& value) {
        list.addNode(value);
    }

    // Function to dequeue an element
    void dequeue() {
        list.popFront();
    }

    // Function to check if the queue is empty
    bool isEmpty() const {
        return list.isEmpty();
    }

    // Function to get the front element of the queue
    std::string front() const {
        return list.front();
    }
};

class HashTable {
public:
    static const int TABLE_SIZE = 1000;

    struct Node {
        std::size_t key;
        std::string value;
        Node* next;
        Node(std::size_t k, const std::string& v) : key(k), value(v), next(nullptr) {}
    };

    Node* table[TABLE_SIZE];

    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
        }
    }

    void insert(std::size_t key, const std::string& value) {
        int index = key % TABLE_SIZE;
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
    }

    std::string* find(std::size_t key) {
        int index = key % TABLE_SIZE;
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
    }
};

//uninstall multple programmes
void ListInstalledPrograms() {
    // Use a command to list installed programs
    system("wmic product get name,version");
}

// Function to uninstall a program
bool UninstallProgram(const std::string& programName) {
    // Use a command to uninstall the program
    std::string uninstallCommand = "wmic product where name='" + programName + "' call uninstall /nointeractive";

    int result = system(uninstallCommand.c_str());

    if (result == 0) {
        return true; // Successfully uninstalled
    } else {
        std::cerr << "Failed to uninstall " << programName << ". Error code: " << result << "\n";
        return false;
    }
}



std::size_t calculateHash(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return 0;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    file.close();


    std::size_t hash = 0;
    for (char c : ss.str()) {
        hash = hash * 31 + static_cast<std::size_t>(c);
    }
    return hash;
}

bool isImageFile(const std::string& filePath) {

    std::string imageExtensions[] = {".jpg", ".jpeg", ".png", ".gif", ".bmp"};
    for (const auto& ext : imageExtensions) {
        if (filePath.length() >= ext.length() &&
            filePath.compare(filePath.length() - ext.length(), ext.length(), ext) == 0) {
            return true;
        }
    }
    return false;
}

void deleteFile(const std::string& filePath) {
    if (remove(filePath.c_str()) != 0) {
        std::cerr << "Error deleting file: " << filePath << std::endl;
    }
}

void TaskAutomator::automatorMenu() {
    int choice;
    do {
        printAutomatorMenu();
        std::cout << "Enter your choice (1-5): ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                endMultipleTasks();
                break;
            case 2:
                clearTempFolder();
                break;
            case 3:
                uninstallMultiplePrograms();
                break;
            case 4:
                findAndDeleteDuplicateImages();  // New option
                break;
            case 5:
                std::cout << "Returning to main menu..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 5." << std::endl;
                break;
        }

    } while (choice != 5);
}



void TaskAutomator::printAutomatorMenu() {
    system("cls");
    std::cout << "\n===== Automator Menu =====\n";
    std::cout << "1. End Multiple Tasks\n";
    std::cout << "2. Clear Temporary Folder\n";
    std::cout << "3. Uninstall Multiple Programs\n";
    std::cout << "4. Duplicate Image Deleter\n";  //
    std::cout << "5. Return to Main Menu\n";
}



void TaskAutomator::endMultipleTasks() {
    ProcessManager processManager;

    processManager.ListProcesses();

    std::cout << "Enter process names to end (separated by space) or -1 to exit:" << std::endl;

    std::string processNames;
    while (true) {
        std::getline(std::cin, processNames);

        if (processNames == "-1") {
            break;
        }

        processManager.EndProcesses(processNames);

        processManager.ListProcesses();
        std::cout << "Enter process names to end (separated by space) or -1 to exit:" << std::endl;
    }
}
void TaskAutomator::clearTempFolder() {
    // Get the user's home directory
    wchar_t userProfile[MAX_PATH];
    if (SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, userProfile) != S_OK) {
        std::wcerr << L"Error getting user profile directory." << std::endl;
        return;
    }

    // Construct the path to the temporary folder
    std::wstring tempPath = userProfile;
    tempPath += L"\\AppData\\Local\\Temp";

    // Convert the wide string to narrow string
    std::string narrowTempPath = wstring2string(tempPath);

    // Construct the command to clear the temporary folder
    std::string clearCommand = "rd /s /q " + narrowTempPath;

    // Execute the command
    std::cout << "Executing command: " << clearCommand << std::endl;
    system(clearCommand.c_str());

    std::cout << "Temporary folder cleared." << std::endl;
    Sleep(5000);
}
void TaskAutomator::uninstallMultiplePrograms() {
    // List installed programs
    std::cout << "Installed Programs:\n";
    ListInstalledPrograms();

    // Get programs to uninstall from the user
    std::cout << "\nEnter the names of programs to uninstall (comma-separated) or -1 to exit:\n";
    std::string input;
    std::cin >> input;


    if (input == "-1") {
            std::cout << "Returning to Automator Menu...\n";
            return;
        }




    Queue uninstallQueue;

    // Tokenize the input and enqueue programs
    size_t pos = 0;
    while ((pos = input.find(',')) != std::string::npos) {
        uninstallQueue.enqueue(input.substr(0, pos));
        input.erase(0, pos + 1);
    }
    uninstallQueue.enqueue(input); // Add the last program

    // Uninstall programs one by one
    while (!uninstallQueue.isEmpty()) {
        std::string program = uninstallQueue.front();
        std::cout << "\nUninstalling " << program << "...\n";

        if (UninstallProgram(program)) {
            std::cout << program << " successfully uninstalled.\n";
        } else {
            std::cout << "Failed to uninstall " << program << ".\n";
        }

        uninstallQueue.dequeue();
    }
}

void TaskAutomator::findAndDeleteDuplicateImages() {
    std::cout << "Enter the directory path or -1 to exit :";

    std::string directoryPath;
    std::cin >> directoryPath;


    if (directoryPath == "-1") {
            std::cout << "Returning to Automator Menu...\n";
            return;
        }

    HashTable hashTable;

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directoryPath + "\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening directory: " << directoryPath << std::endl;
        return;
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::string filePath = directoryPath + "\\" + findFileData.cFileName;
            if (isImageFile(filePath)) {
                std::size_t fileHash = calculateHash(filePath);
                // Insert the hash and file path into the hashtable
                hashTable.insert(fileHash, filePath);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    // Iterate over the hashtable and handle duplicates
    bool duplicateFound = false;

    for (int i = 0; i < HashTable::TABLE_SIZE; ++i) {
        HashTable::Node* current = hashTable.table[i];
        while (current != nullptr) {
            HashTable::Node* next = current->next;
            if (next != nullptr) {
                // Delete the duplicate file
                std::cout << "Duplicate found: " << current->value << " (deleted)\n";
                deleteFile(current->value);
                duplicateFound = true;
            }
            current = next;
        }
    }

    if (!duplicateFound) {
        std::cout << "No duplicate images found in the directory.\n";
    }
    std::string userInput;
    do {
        std::cout << "Enter -1 to return to the main menu: ";
        std::cin >> userInput;

        if (userInput != "-1") {
            std::cout << "Invalid input. Please enter -1 to return to the main menu.\n";
        }
    } while (userInput != "-1");

    std::cout << "Returning to the automator menu..." << std::endl;
}
