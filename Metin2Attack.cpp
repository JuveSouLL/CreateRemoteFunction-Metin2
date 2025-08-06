#include "Metin2Attack.h"
#include "ProcessUtils.h"
#include <iostream>

Metin2Attack::Metin2Attack() : hProcess(nullptr), pid(0), moduleBase(0) 
{
    netPointerAddr = 0;
    battleCall = 0;
    cPythonPlayer = 0;
    targetVIDAddr = 0;
}

Metin2Attack::~Metin2Attack()
{
    if (hProcess)
        CloseHandle(hProcess);
}

bool Metin2Attack::Initialize()
{
    // PID veya Process adına göre işlem yap
    if (GameConfig::USE_PID)
    {
        // PID kullan
        pid = GameConfig::TARGET_PID;

        // PID'nin geçerli olup olmadığını kontrol et
        if (!ProcessUtils::IsProcessRunning(pid))
        {
            std::cout << "[!] PID " << pid << " bulunamadi veya process calismıyor!" << std::endl;
            return false;
        }

        // Process adını al ve doğrula (opsiyonel)
        std::string processName = ProcessUtils::GetProcessName(pid);
        std::cout << "[+] Process bulundu: " << processName << " (PID: " << pid << ")" << std::endl;
    }
    else
    {
        // Process adı kullan (eski yöntem)
        pid = ProcessUtils::GetProcessIdByName(GameConfig::PROCESS_NAME);
        if (!pid)
        {
            std::cout << "[!] " << GameConfig::PROCESS_NAME << " bulunamadi!" << std::endl;
            return false;
        }
    }

    // Process aç
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
    {
        std::cout << "[!] Process acilamadi! Admin olarak calistirin." << std::endl;
        return false;
    }

    // Module base bul
    moduleBase = ProcessUtils::GetModuleBaseAddress(pid, GameConfig::MODULE_NAME);
    if (!moduleBase)
    {
        std::cout << "[!] " << GameConfig::MODULE_NAME << " modulu bulunamadi!" << std::endl;
        return false;
    }

    // Adresleri hesapla
    netPointerAddr = moduleBase + GameConfig::NET_POINTER_OFFSET;
    battleCall = moduleBase + GameConfig::BATTLE_CALL_OFFSET;
    cPythonPlayer = moduleBase + GameConfig::CPYTHON_PLAYER_OFFSET;
    targetVIDAddr = GameConfig::TARGET_VID_OFFSET;

    std::cout << "[+] PID: " << pid << std::endl;
    std::cout << "[+] Module Base: 0x" << std::hex << moduleBase << std::dec << std::endl;
    std::cout << "[+] Initialization successful!" << std::endl;

    return true;
}

void Metin2Attack::SendAttack()
{
    // cPythonPlayer pointer'ını oku
    DWORD cPythonBase = 0;
    if (!ReadProcessMemory(hProcess, (LPCVOID)cPythonPlayer, &cPythonBase, sizeof(DWORD), nullptr))
    {
        std::cout << "[!] cPythonPlayer okunamadi!" << std::endl;
        return;
    }
    
    // Target VID'yi oku
    DWORD targetVID = 0;
    if (!ReadProcessMemory(hProcess, (LPCVOID)(cPythonBase + targetVIDAddr), &targetVID, sizeof(DWORD), nullptr))
    {
        std::cout << "[!] targetVID okunamadi!" << std::endl;
        return;
    }
    
    if (targetVID == 0)
        return;
    
    // Net pointer'ı oku
    DWORD realNetPointer = 0;
    if (!ReadProcessMemory(hProcess, (LPCVOID)netPointerAddr, &realNetPointer, sizeof(DWORD), nullptr))
    {
        std::cout << "[!] netPointer okunamadi!" << std::endl;
        return;
    }
    
    ExecuteRemoteCode(targetVID, realNetPointer);
}

std::vector<BYTE> Metin2Attack::GenerateAttackShellcode(DWORD targetVID, DWORD realNetPointer, uintptr_t remoteBaseAddr)
{
    std::vector<BYTE> shellcode;
    
    // mov ecx, realNetPointer
    shellcode.push_back(0xB9);
    shellcode.insert(shellcode.end(), (BYTE*)&realNetPointer, (BYTE*)&realNetPointer + 4);
    
    // push targetVID
    shellcode.push_back(0x68);
    shellcode.insert(shellcode.end(), (BYTE*)&targetVID, (BYTE*)&targetVID + 4);
    
    // push 0
    shellcode.push_back(0x6A);
    shellcode.push_back(0x00);
    
    // call battleCall (relative)
    shellcode.push_back(0xE8);
    DWORD relativeCall = battleCall - (remoteBaseAddr + shellcode.size() + 4);
    shellcode.insert(shellcode.end(), (BYTE*)&relativeCall, (BYTE*)&relativeCall + 4);
    
    // ret
    shellcode.push_back(0xC3);
    
    return shellcode;
}

bool Metin2Attack::ExecuteRemoteCode(DWORD targetVID, DWORD realNetPointer)
{
    SIZE_T size = 1024;
    LPVOID pRemoteCode = VirtualAllocEx(hProcess, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!pRemoteCode)
        return false;

    std::vector<BYTE> shellcode = GenerateAttackShellcode(targetVID, realNetPointer, (uintptr_t)pRemoteCode);

    if (!WriteProcessMemory(hProcess, pRemoteCode, shellcode.data(), shellcode.size(), nullptr))
    {
        VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pRemoteCode, nullptr, 0, nullptr);
    if (!hThread)
    {
        VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);
        return false;
    }

    // Config'den timeout değerini al
    WaitForSingleObject(hThread, GameConfig::THREAD_TIMEOUT_MS);
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteCode, 0, MEM_RELEASE);

    return true;
}