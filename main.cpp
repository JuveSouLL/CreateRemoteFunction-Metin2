#include <iostream>
#include <Windows.h>
#include "Metin2Attack.h"
#include "GameConfig.h"  // Config'i include et

int main()
{
    std::cout << "=== Metin2 External Attack Tool ===" << std::endl;

    Metin2Attack attack;
    if (!attack.Initialize())
    {
        system("pause");
        return 1;
    }

    std::cout << "\n[F2] Attack Baslat" << std::endl;
    std::cout << "[F3] Attack Durdur" << std::endl;
    std::cout << "[F4] Cikis" << std::endl;

    bool running = true;
    bool attacking = false;

    while (running)
    {
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            attacking = true;
            std::cout << "[+] Attack baslatildi!" << std::endl;
        }

        if (GetAsyncKeyState(VK_F3) & 1)
        {
            attacking = false;
            std::cout << "[-] Attack durduruldu!" << std::endl;
        }

        if (GetAsyncKeyState(VK_F4) & 1)
        {
            running = false;
            std::cout << "[!] Cikis yapiliyor..." << std::endl;
        }

        if (attacking)
        {
            attack.SendAttack();
            Sleep(GameConfig::ATTACK_DELAY_MS);  // Config'den delay al
        }
        else
        {
            Sleep(GameConfig::IDLE_DELAY_MS);  // Config'den delay al
        }
    }

    return 0;
}