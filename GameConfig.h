#pragma once
#include <cstdint>

namespace GameConfig
{
    // Process bilgileri - PID veya Process Name kullanılabilir
    constexpr bool USE_PID = true;              // true = PID kullan, false = Process adı kullan
    constexpr DWORD TARGET_PID = 12976;         // Hedef process ID
    constexpr const char* PROCESS_NAME = "nethan2.bin";  // Alternatif: process adı
    constexpr const char* MODULE_NAME = "nethan2.bin";

    // Module base'e eklenecek offset'ler
    constexpr uintptr_t NET_POINTER_OFFSET = 0x6C3320;
    constexpr uintptr_t BATTLE_CALL_OFFSET = 0x0E25B0;
    constexpr uintptr_t CPYTHON_PLAYER_OFFSET = 0x6FD4C4;

    // Pointer'a eklenecek offset
    constexpr uintptr_t TARGET_VID_OFFSET = 0x00A3A74;

    // Timing ayarları
    constexpr int ATTACK_DELAY_MS = 30;
    constexpr int IDLE_DELAY_MS = 50;
    constexpr int THREAD_TIMEOUT_MS = 1000;
}