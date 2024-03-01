#ifndef PROCESSMANAGER_H_INCLUDED
#define PROCESSMANAGER_H_INCLUDED


class SimpleUnorderedSet {
private:
    static const int MAX_SIZE = 1024;
    std::string elements[MAX_SIZE];
    int numElements;

public:
    SimpleUnorderedSet() : numElements(0) {}

    bool contains(const std::string& element) const {
        for (int i = 0; i < numElements; ++i) {
            if (elements[i] == element) {
                return true;
            }
        }
        return false;
    }

    void insert(const std::string& element) {
        if (!contains(element) && numElements < MAX_SIZE) {
            elements[numElements++] = element;
        }
    }
};
class SimpleQueue {
private:
    static const int MAX_SIZE = 1024;
    std::string elements[MAX_SIZE];
    int front, rear;

public:
    SimpleQueue() : front(-1), rear(-1) {}

    bool isEmpty() const {
        return front == -1;
    }

    bool isFull() const {
        return (front == 0 && rear == MAX_SIZE - 1) || (front == rear + 1);
    }

    void enqueue(const std::string& element) {
        if (isFull()) {
            std::cerr << "Queue is full." << std::endl;
            return;
        }

        if (front == -1) {
            front = rear = 0;
        } else if (rear == MAX_SIZE - 1) {
            rear = 0;
        } else {
            rear++;
        }

        elements[rear] = element;
    }

    std::string dequeue() {
        if (isEmpty()) {
            std::cerr << "Queue is empty." << std::endl;
            return "";
        }

        std::string element = elements[front];

        if (front == rear) {
            front = rear = -1;
        } else if (front == MAX_SIZE - 1) {
            front = 0;
        } else {
            front++;
        }

        return element;
    }
};
class ProcessManager {
private:
    SimpleUnorderedSet uniqueProcessNames;
    SimpleQueue processQueue;

public:
    void ListProcesses();
    void EndProcesses(const std::string& processNames);
};

void ProcessManager::ListProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating process snapshot." << std::endl;
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Error getting process information." << std::endl;
        CloseHandle(hProcessSnap);
        return;
    }

    std::cout << "Current Processes:" << std::endl;

    do {
        std::string processName = pe32.szExeFile;

        // Check for duplicates using the custom unordered set
        if (!uniqueProcessNames.contains(processName)) {
            std::cout << processName << std::endl;
            uniqueProcessNames.insert(processName);
        }

    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}

void ProcessManager::EndProcesses(const std::string& processNames) {
    std::istringstream iss(processNames);
    std::string processName;

    while (iss >> processName) {
        processQueue.enqueue(processName);
    }

    while (!processQueue.isEmpty()) {
        std::string currentProcessName = processQueue.dequeue();
        HANDLE hProcessSnap;
        PROCESSENTRY32 pe32;

        hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hProcessSnap == INVALID_HANDLE_VALUE) {
            std::cerr << "Error creating process snapshot." << std::endl;
            continue;
        }

        pe32.dwSize = sizeof(PROCESSENTRY32);

        while (Process32Next(hProcessSnap, &pe32)) {
            std::string processExe = pe32.szExeFile;
            if (processExe == currentProcessName) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 0);
                    std::cout << "Process " << currentProcessName << " ended successfully." << std::endl;
                    CloseHandle(hProcess);
                } else {
                    std::cerr << "Error ending process " << currentProcessName << std::endl;
                }
            }
        }

        CloseHandle(hProcessSnap);
    }
}



#endif // PROCESSMANAGER_H_INCLUDED
