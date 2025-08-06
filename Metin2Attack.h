#pragma once
#include <Windows.h>
#include <vector>
#include "GameConfig.h"  // Config dosyasýný include et

class Metin2Attack
{
private:
    // Process bilgileri
    HANDLE hProcess;
    DWORD pid;
    uintptr_t moduleBase;

    // Oyun adresleri
    uintptr_t netPointerAddr;
    uintptr_t battleCall;
    uintptr_t cPythonPlayer;
    uintptr_t targetVIDAddr;

    // Private metodlar
    std::vector<BYTE> GenerateAttackShellcode(DWORD targetVID, DWORD realNetPointer, uintptr_t remoteBaseAddr);
    bool ExecuteRemoteCode(DWORD targetVID, DWORD realNetPointer);

public:
    Metin2Attack();
    ~Metin2Attack();

    bool Initialize();
    void SendAttack();
};