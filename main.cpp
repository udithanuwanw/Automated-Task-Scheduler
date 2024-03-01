#include <iostream>
#include <limits>
#include <mutex>
#include <thread>
#include <chrono>
#include <ctime>
#include <Windows.h>
#include <string>
#include <mmsystem.h>
#include <fstream>
#include <sstream>
#include <TlHelp32.h>
#include <locale>
#include <ShlObj.h>
#include "TaskScheduler.h"
#include "TaskAutomator.h"

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}


void setConsoleFontSize(int size) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 8;
    cfi.dwFontSize.Y = size;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas");

    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}





int mainMenu() {
    system("cls");
    setConsoleColor(10);
    setConsoleFontSize(18);

    std::cout << "\n===== Main Menu =====\n";
    std::cout << "1. Task Scheduler\n";
    std::cout << "2. Task Automator\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice (1-3): ";

    int choice;
    std::cin >> choice;
    return choice;
}

int main() {
    int choice;
    TaskScheduler scheduler;
    TaskAutomator automator;

    std::thread scheduler_thread(&TaskScheduler::runScheduler, &scheduler);

    do {
        choice = mainMenu();

        switch (choice) {
            case 1:
                scheduler.addTask();
                break;
            case 2:
                automator.automatorMenu();
                break;
            case 3:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 3.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }

    } while (choice != 3);


    scheduler.stopScheduler();


    scheduler_thread.join();

    return 0;
}
