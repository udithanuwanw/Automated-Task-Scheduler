// TaskScheduler.h
#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <string>
#include <mutex>
#include <thread>
#include <ctime>
#include <Windows.h>
#include <iostream>
#include "MyStack.h"
#include "MyPriorityQueue.h"


void showNotification(const char* title, const char* message) {
    MessageBoxA(nullptr, message, title, MB_OK | MB_ICONINFORMATION);
}

void openZoom(const std::string& meetingId, const std::string& password) {
    std::string zoomUrl = "zoommtg://zoom.us/join?confno=" + meetingId + "&pwd=" + password;
    ShellExecute(NULL, "open", zoomUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

class TaskScheduler {
public:
    TaskScheduler() : is_running(true) {}

    void scheduleNotification(const std::string& message, const std::string& scheduled_time) {
        std::lock_guard<std::mutex> lock(mutex);
        Task task;
        task.scheduled_time = scheduled_time;
        task.type = "notification";
        task.message = message;
        tasks.push(task);
    }

    void scheduleAlarm(const std::string& scheduled_time) {
        std::lock_guard<std::mutex> lock(mutex);
        Task task;
        task.scheduled_time = scheduled_time;
        task.type = "alarm";
        tasks.push(task);
    }

    void scheduleZoomMeeting(const std::string& meetingId, const std::string& password, const std::string& scheduled_time) {
        std::lock_guard<std::mutex> lock(mutex);
        Task task;
        task.scheduled_time = scheduled_time;
        task.type = "zoom";
        task.meetingId = meetingId;
        task.password = password;
        tasks.push(task);
    }

    void scheduleShutdownRestart(int choice, const std::string& scheduled_time) {
        std::lock_guard<std::mutex> lock(mutex);
        Task task;
        task.scheduled_time = scheduled_time;
        task.type = (choice == 1) ? "shutdown" : "restart";
        tasks.push(task);
    }

    void runScheduler() {
        while (is_running) {
            std::lock_guard<std::mutex> lock(mutex);
            std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::string current_time_str = std::ctime(&current_time);
            current_time_str = current_time_str.substr(11, 5);

            while (!tasks.empty() && tasks.top().scheduled_time <= current_time_str) {
                Task task = tasks.top();
                tasks.pop();

                history.push({task.type, task.scheduled_time, current_time_str});

                if (task.type == "notification") {
                    showNotification("Notification", task.message.c_str());
                } else if (task.type == "alarm") {
                    playAlarmSound();
                } else if (task.type == "zoom") {
                    openZoom(task.meetingId, task.password);
                } else if (task.type == "shutdown") {
                    std::cout << "Performing Shutdown at " << task.scheduled_time << std::endl;
                    system("shutdown /s /t 1");
                } else if (task.type == "restart") {
                    std::cout << "Performing Restart at " << task.scheduled_time << std::endl;
                    system("shutdown /r /t 1");
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void viewRecentExecutions() {
        std::lock_guard<std::mutex> lock(mutex);

        if (history.empty()) {
            std::cout << "No recent executions." << std::endl;
            return;
        }

        std::cout << "\n===== Recent Executions =====\n";
        MyStack<HistoryEntry> temp;

        while (!history.empty()) {
            HistoryEntry entry = history.top();
            history.pop();
            temp.push(entry);
            std::cout << entry.type << " task executed at " << entry.current_time
                      << " scheduled for " << entry.scheduled_time << std::endl;
        }

        while (!temp.empty()) {
            history.push(temp.top());
            temp.pop();
        }
        Sleep(8000);
    }

    void addTask() {
        int choice;
        do {
            printMenu();
            std::cout << "Enter your choice (1-6): ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    addNotificationTask();
                    break;
                case 2:
                    addAlarmTask();
                    break;
                case 3:
                    addZoomMeetingTask();
                    break;
                case 4:
                    addShutdownRestartTask();
                    break;
                case 5:
                    viewRecentExecutions();
                    break;
                case 6:
                    std::cout << "Returning to main menu..." << std::endl;
                    return;
                default:
                    std::cout << "Invalid choice. Please enter a number between 1 and 6." << std::endl;
            }

        } while (choice != 6);
    }

    void stopScheduler() {
        is_running = false;
    }

private:
    struct Task {
        std::string scheduled_time;
        std::string type;
        std::string message;
        std::string meetingId;
        std::string password;
        bool operator<(const Task& other) const {
            return scheduled_time > other.scheduled_time;
        }
    };

    struct HistoryEntry {
        std::string type;
        std::string scheduled_time;
        std::string current_time;
    };

    MyPriorityQueue<Task> tasks;
    MyStack<HistoryEntry> history;
    std::mutex mutex;
    bool is_running;

    static void playAlarmSound() {
        const char* wavFilePath = "C:\\Users\\Uditha\\Desktop\\file_example_WAV_1MG.wav";

        if (mciSendStringA(("open \"" + std::string(wavFilePath) + "\" type waveaudio alias wave").c_str(), nullptr, 0, nullptr) != 0) {
            std::cerr << "Failed to open WAV file" << std::endl;
            return;
        }

        if (mciSendStringA("play wave", nullptr, 0, nullptr) != 0) {
            std::cerr << "Failed to play WAV file" << std::endl;
            return;
        }

        Sleep(5000);

        if (mciSendStringA("close wave", nullptr, 0, nullptr) != 0) {
            std::cerr << "Failed to close WAV file" << std::endl;
        }
    }

    void printMenu() {
        system("cls");
        std::cout << "\n===== Menu =====\n";
        std::cout << "1. Schedule Notification\n";
        std::cout << "2. Schedule Alarm\n";
        std::cout << "3. Schedule Zoom Meeting\n";
        std::cout << "4. Schedule Shutdown or Restart\n";
        std::cout << "5. View Recent Executions\n";
        std::cout << "6. Exit\n";
    }

    void addNotificationTask() {
        std::string scheduled_time, message;
        getCommonTaskInputs(scheduled_time);
        std::cout << "Enter notification message: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, message);
        scheduleNotification(message, scheduled_time);
    }

    void addAlarmTask() {
        std::string scheduled_time;
        getCommonTaskInputs(scheduled_time);
        scheduleAlarm(scheduled_time);
    }

    void addZoomMeetingTask() {
        std::string scheduled_time, meetingId, password;
        getCommonTaskInputs(scheduled_time);
        std::cout << "Enter Zoom Meeting ID: ";
        std::cin >> meetingId;
        std::cout << "Enter Meeting Password: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, password);
        scheduleZoomMeeting(meetingId, password, scheduled_time);
    }

    void addShutdownRestartTask() {
        std::string scheduled_time;
        getCommonTaskInputs(scheduled_time);

        int choice;
        do {
            system("cls");
            std::cout << "Select an operation:\n";
            std::cout << "1. Schedule Shutdown\n";
            std::cout << "2. Schedule Restart\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (choice < 1 || choice > 2) {
                std::cout << "Invalid choice. Please enter 1 or 2.\n";
            }
        } while (choice < 1 || choice > 2);

        scheduleShutdownRestart(choice, scheduled_time);
    }

    void getCommonTaskInputs(std::string& scheduled_time) {
        while (true) {
            std::cout << "Enter scheduled time (HH:MM): ";
            std::cin >> std::ws >> scheduled_time;
            if (scheduled_time.size() == 5 && scheduled_time[2] == ':' &&
                std::isdigit(scheduled_time[0]) && std::isdigit(scheduled_time[1]) &&
                std::isdigit(scheduled_time[3]) && std::isdigit(scheduled_time[4])) {
                break;
            } else {
                std::cout << "Invalid scheduled time format. Please enter in the format 'HH:MM'.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
};


#endif // TASKSCHEDULER_H
