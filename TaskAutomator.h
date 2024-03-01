#ifndef TASKAUTOMATOR_H_INCLUDED
#define TASKAUTOMATOR_H_INCLUDED

#include <string>

// Function to convert wide string to narrow string
std::string wstring2string(const std::wstring& wideStr) {
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(bufferSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &result[0], bufferSize, nullptr, nullptr);
    return result;
}

class TaskAutomator {
public:
    void automatorMenu();

private:
    void printAutomatorMenu();
    void endMultipleTasks();
    void clearTempFolder();
    void uninstallMultiplePrograms();
    void findAndDeleteDuplicateImages();
};
#include "TaskAutomator.cpp"


#endif // TASKAUTOMATOR_H_INCLUDED
