# Process Monitor

A lightweight Windows process monitoring utility written in C++ using WinAPI.

---

## Features

- List running processes
- Display process IDs (PID)
- Filter processes by name
- Case-insensitive search
- Optional process termination
- Unicode support
- Clean formatted console output

---

## Example Output

```
PID           Process
---------------------------------------------------
0             [System Process]
4             System
1240          explorer.exe
5820          chrome.exe

Total processes: 4
```

---

## Usage
**Show all processes**

```
monitor.exe
```

**Filter processes**

```
monitor.exe chrome
```

Example:

```
PID           Process
---------------------------------------------------
5820          chrome.exe
7124          chrome.exe
8452          chrome.exe

Total processes: 3
```

**Kill matching processes**

```
monitor.exe chrome.exe --kill
```

Example:

```
PID           Process                                  Status
---------------------------------------------------------------------------------
5820          chrome.exe                              [TERMINATED]
7124          chrome.exe                              [TERMINATED]
```

## Build

```
cmake -B build
cmake --build build
```