#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

class ProcessUtils
{
public:
    static DWORD GetProcessIdByName(const char* processName);
    static uintptr_t GetModuleBaseAddress(DWORD pid, const char* moduleName);
    static bool IsProcessRunning(DWORD pid);  // Yeni fonksiyon
    static std::string GetProcessName(DWORD pid);  // Yeni fonksiyon
};