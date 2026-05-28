#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <iomanip>
#include <string>
#include <cstring>
#include <tchar.h>
#include <algorithm>


template<typename CharT>
CharT toLower(CharT ch) {
        if constexpr (std::is_same_v<CharT, char>) {
                return static_cast<CharT>(std::tolower(static_cast<unsigned char>(ch)));
        } else if constexpr (std::is_same_v<CharT, wchar_t>) {
                return static_cast<CharT>(std::towlower(ch));
        } else {
                static_assert(
                        std::is_same_v<CharT, char> ||
                        std::is_same_v<CharT, wchar_t>,
                        "Unsupported character type"
                        );
        }
}

template<typename StringT>
bool find(const StringT& text, const StringT& search) {
        if (search.empty()) return true;
        if (text.size() < search.size()) return false;

        auto it = std::search(
                text.begin(), text.end(),
                search.begin(), search.end(),
                [](auto ch1, auto ch2) {
                return toLower(ch1) == toLower(ch2);
                }
        );

        return it != text.end();
}


bool KillProcessById(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open process. Error: " << GetLastError() << "\n";
        return false;
    }

    BOOL result = TerminateProcess(hProcess, 1);
    CloseHandle(hProcess);

    if (!result) {
        std::cerr << "Failed to terminate process. Error: " << GetLastError() << "\n";
        return false;
    }

    return true;
}


int main(int argc, char* argv[]) {

        bool filter = (argc >= 2);

        bool kill = false;
        for (int i = 2; i < argc; i++) {
                if (std::string(argv[i]) == "--kill") {
                        kill = true;
                }
        }

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
                        << "Process";

                if (kill) {
                        std::cout << "Status";
                }

                std::cout << "\n--------------------------------------------------------------------------------\n";

                do {

                #ifdef UNICODE
                        std::wstring exeName(processEntry.szExeFile);
                #else
                        std::string exeName(processEntry.szExeFile);
                #endif

                bool matches = !filter || find(exeName, filterExeName);

                if (!matches) {
                        continue;
                }

                bool killed = false;

                if (kill) {
                        if (processEntry.th32ProcessID == GetCurrentProcessId()) {
                                std::cout << "[SKIPPED SELF]\n";
                        } else {
                                killed = KillProcessById(processEntry.th32ProcessID);
                        }
                }

                std::cout << std::left
                        << std::setw(14)
                        << processEntry.th32ProcessID
                        << std::setw(42)
                        << processEntry.szExeFile;

                if (kill) {

                        if (killed) {
                                std::cout << "[TERMINATED]";
                        } else {
                                std::cout << "[FAILED]";
                        }
                }

                std::cout << '\n';

                totalProcesses++;

                } while (Process32Next(snapshot, &processEntry));

        } else {
                std::cerr << "Failed to get first process\n";
        }

        std::cout << "Total processes: " << totalProcesses;

        CloseHandle(snapshot);

        return 0;
}