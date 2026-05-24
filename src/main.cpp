#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <iomanip>
#include <string>
#include <cstring>

int main(int argc, char* argv[]) {

        bool filter = (argc >= 2);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE) {
                std::cerr << "Failed to create snapshot";
                return 1;
        }

        PROCESSENTRY32 processEntry{};
        processEntry.dwSize = sizeof(PROCESSENTRY32);


        int totalProcesses{};

        #ifdef UNICODE
                std::wstring filterExeName;
                if (filter) {
                        filterExeName = std::wstring(argv[1], argv[1] + std::strlen(argv[1]));
                }
        #else
                std::string filterExeName;
                if (filter) {
                        filterExeName = argv[1];
                }
        #endif
        

        if (Process32First(snapshot, &processEntry)) {

                std::cout << std::left
                        << std::setw(14)
                        << "PID"
                        << std::setw(42)
                        << "Process"
                        << "\n---------------------------------------------------\n";

                do {
                        #ifdef UNICODE
                                std::wstring exeName(processEntry.szExeFile);
                        #else
                                std::string exeName(processEntry.szExeFile);
                        #endif

                        if (!filter) {
                                std::cout << std::left
                                        << std::setw(14)
                                        << processEntry.th32ProcessID
                                        << std::setw(42)
                                        << processEntry.szExeFile
                                        << "\n";
                                
                                totalProcesses++;
                        } else {
                                if (exeName == filterExeName) {
                                        std::cout << std::left
                                                << std::setw(14)
                                                << processEntry.th32ProcessID
                                                << std::setw(42)
                                                << processEntry.szExeFile
                                                << "\n";
                                        
                                        totalProcesses++;
                                }
                        }

                } while (Process32Next(snapshot, &processEntry));

        } else {
                std::cerr << "Failed to get first process\n";
        }

        std::cout << "Total processes: " << totalProcesses;

        CloseHandle(snapshot);

        return 0;
}