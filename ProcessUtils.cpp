#include "ProcessUtils.h"
#include <cstring>

DWORD ProcessUtils::GetProcessIdByName(const char* processName)
{
    DWORD pid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32W pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32W);
        
        if (Process32FirstW(hSnapshot, &pe32))
        {
            do
            {
                char szExeFile[MAX_PATH];
                size_t convertedChars = 0;
                wcstombs_s(&convertedChars, szExeFile, pe32.szExeFile, MAX_PATH);
                
                if (_stricmp(szExeFile, processName) == 0)
                {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32NextW(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return pid;
}

uintptr_t ProcessUtils::GetModuleBaseAddress(DWORD pid, const char* moduleName)
{
    uintptr_t moduleBase = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32W me32;
        me32.dwSize = sizeof(MODULEENTRY32W);
        
        if (Module32FirstW(hSnapshot, &me32))
        {
            do
            {
                char szModule[MAX_PATH];
                size_t convertedChars = 0;
                wcstombs_s(&convertedChars, szModule, me32.szModule, MAX_PATH);
                
                if (_stricmp(szModule, moduleName) == 0)
                {
                    moduleBase = (uintptr_t)me32.modBaseAddr;
                    break;
                }
            } while (Module32NextW(hSnapshot, &me32));
        }
        CloseHandle(hSnapshot);
    }
    return moduleBase;
}
bool ProcessUtils::IsProcessRunning(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (hProcess)
    {
        DWORD exitCode;
        bool running = GetExitCodeProcess(hProcess, &exitCode) && (exitCode == STILL_ACTIVE);
        CloseHandle(hProcess);
        return running;
    }
    return false;
}

std::string ProcessUtils::GetProcessName(DWORD pid)
{
    std::string processName;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32W pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32W);

        if (Process32FirstW(hSnapshot, &pe32))
        {
            do
            {
                if (pe32.th32ProcessID == pid)
                {
                    char szExeFile[MAX_PATH];
                    size_t convertedChars = 0;
                    wcstombs_s(&convertedChars, szExeFile, pe32.szExeFile, MAX_PATH);
                    processName = szExeFile;
                    break;
                }
            } while (Process32NextW(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return processName;
}