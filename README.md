# Process Monitor

Windows process monitoring utility written in C++ using WinAPI.

---

## Features

- Lists running processes
- Displays process IDs (PID)
- Process name filtering
- Unicode support
- Formatted console output

---

## Example Output

```text
PID           Process
---------------------------------------------------
0             [System Process]
4             System
1240          explorer.exe
5820          chrome.exe

Total processes: 4
```

---

## Filtering

Show only a specific process:

```
monitor.exe chrome.exe
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

---

## Build

```
cmake -B build
cmake --build build
```