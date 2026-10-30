#include <iostream>
#include <Windows.h>
#include <string>

using namespace std;

const int max_processes = 200;

int procCount = 0;

struct processInfo {
    string name;
    PROCESS_INFORMATION pi = {};
    bool activ = false;
};
processInfo processes[max_processes];

void startProcess(wchar_t* appName, wchar_t* cmdL, const char* displ) {
    if (procCount >= max_processes) {
        cout << "Достигнут лимит процессорв\n";
        return;
    }
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (CreateProcess(appName, cmdL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &processes[procCount].pi)) {
        processes[procCount].name = displ;
        processes[procCount].activ = true;
        cout << "Запущен " << displ << " PID " << processes[procCount].pi.dwProcessId << "\n";
        procCount++;
    }
    else {
        cout << "Процесс не создан, ошибка: " << GetLastError();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int choice;
    do {
                cout << "\n1. Запустить диспетчер задач(taskmgr.exe)\n";
        cout << "2. Запустить ping с вводом параметров\n";
        cout << "3. Запустить программу из вариата\n";
        cout << "4. Показать список процессов\n";
        cout << "5. Завершить процесс\n";
        cout << "0. Выход\n";
        cout << "Введите выбор:\n";
        cin >> choice;

        switch (choice)
        {
        case 1: {
            wchar_t lpszAppName[] = L"C:\\Windows\\System32\\taskmgr.exe";
            wchar_t lpszCmdLine[] = L"C:\\Windows\\System32\\taskmgr.exe";
            startProcess(lpszAppName, lpszCmdLine, "Диспетчер задач");
            break;
        }
        case 2: {
            int mode;
            string ip;
            string args;
            cout << "1. Обычный (4 запроса)\n";
            cout << "2. -n [N] (N запросов)\n";
            cout << "3. -t (бесконечно)\n";
            cout << "Выберите режим: ";
            cin >> mode;
            cout << "Введите IP-адрес: ";
            cin >> ip;

            if (mode == 1) {
                args = ip;
            }
            else if (mode == 2) {
                int n;
                cout << "Введите количество запросов: ";
                cin >> n;
                args = "-n " + to_string(n) + " " + ip;
            }
            else {
                args = "-t " + ip;
            }

            string fullPingCmd = "PING.EXE " + args;
            wstring wPingCmd(fullPingCmd.begin(), fullPingCmd.end());

            wchar_t lpszAppName[] = L"C:\\Windows\\System32\\PING.EXE";
            startProcess(lpszAppName, &wPingCmd[0], "Ping");
            break;
        }

        case 3: {
            int num1;
            int num2;
            cout << "Введите начало диапазона: ";
            cin >> num1;
            cout << "Введите конец диапазона: ";
            cin >> num2;

            wchar_t lpszAppName[] = L"C:\\Users\\st310-02\\Desktop\\Politov PR-31\\pract12\\x64\\Debug\\pract12.exe";

            wstring cmd = L"pract12.exe " + to_wstring(num1) + L" " + to_wstring(num2);

            startProcess(lpszAppName, &cmd[0], "Вариант 3");
            break;
        }
        case 4: {
            int activeCount = 0;
            for (int i = 0; i < procCount; i++)
            {
                if (processes[i].activ) {
                    DWORD state = WaitForSingleObject(processes[i].pi.hProcess, 0);
                    if (state == WAIT_TIMEOUT) {
                        cout << "[" << i + 1 << "] " << processes[i].name << " PID: " << processes[i].pi.dwProcessId << " состояние: Работает\n";
                        activeCount++;
                    }
                    else {
                        processes[i].activ = false;
                        CloseHandle(processes[i].pi.hProcess);
                        CloseHandle(processes[i].pi.hThread);
                        cout << "[" << i + 1 << "] " << processes[i].name << " состояние: завершен\n";
                    }
                }
            }
            cout << "Всего работающих процессов: " << activeCount << "\n";
            break;
        }
        case 5: {
            for (int i = 0; i < procCount; i++) {
                if (processes[i].activ) {
                    cout << "[" << i + 1 << "] " << processes[i].name << "\n";
                }
            }
            int delId;
            cout << "Введите номер для завершения: ";
            cin >> delId;
            int idx = delId - 1;
            if (idx >= 0 && idx < procCount && processes[idx].activ) {
                if (TerminateProcess(processes[idx].pi.hProcess, 1)) {
                    cout << "Процесс " << processes[idx].name << " завершен\n";
                    processes[idx].activ = false;
                    CloseHandle(processes[idx].pi.hProcess);
                    CloseHandle(processes[idx].pi.hThread);
                }
                else {
                    cout << "Ошибка: " << GetLastError();
                }
            }
            else {
                cout << "Неверный номер";
            }
            break;
        }
        case 0: { cout << "Выход\n"; break; }
        default:  cout << "Неверный выбор"; break;
        }
    } while (choice != 0);
    for (int i = 0; i < procCount; i++)
    {
        if (processes[i].activ) {
            CloseHandle(processes[i].pi.hProcess);
            CloseHandle(processes[i].pi.hThread);
        }
    }
}
