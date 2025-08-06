# Metin2 External Attack Tool

[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows/)
[![License](https://img.shields.io/badge/License-Educational-yellow.svg)](#license)

> **⚠️ DİKKAT: Bu araç sadece eğitim amaçlıdır. Canlı sunucularda kullanımı yasaktır ve hesap banına neden olabilir.**

## 📖 Proje Hakkında

Bu proje, Metin2 oyunu için geliştirilmiş external bir attack automation aracıdır. Memory injection ve process manipulation teknikleri kullanarak oyun içi attack fonksiyonlarını otomatikleştirir.

### ✨ Özellikler

- **External Memory Manipulation**: DLL injection gerektirmez
- **Real-time Attack Automation**: Hedef seçildiğinde otomatik saldırı
- **Configurable Settings**: Kolayca özelleştirilebilir ayarlar
- **Process Detection**: PID veya process adı ile hedef belirleme
- **Safe Memory Operations**: Güvenli memory read/write işlemleri
- **Hotkey Support**: F2/F3/F4 tuş kombinasyonları

## 🛠️ Teknik Detaylar

### Kullanılan Teknolojiler

- **C++17**: Modern C++ özellikleri
- **Windows API**: Process ve memory yönetimi
- **Assembly Shellcode**: Runtime kod enjeksiyonu
- **Virtual Memory Management**: Güvenli memory allocation

### Mimari Yapısı

```
ExternalAsm/
├── main.cpp              # Ana uygulama ve UI
├── Metin2Attack.h/cpp    # Attack işlemleri ve memory manipulation
├── ProcessUtils.h/cpp    # Process yönetimi ve utility fonksiyonları
├── GameConfig.h          # Konfigürasyon ayarları
└── ExternalASm.sln       # Visual Studio solution
```

## 🔧 Kurulum ve Derleme

### Gereksinimler

- **Visual Studio 2019/2022** (MSVC v143 toolset)
- **Windows 10/11** (x86/x64)
- **Administrator yetkisi** (process manipulation için)

### Derleme Adımları

1. **Build Configuration:**
   - Configuration: `Release`
   - Platform: `x86` (Metin2 32-bit için)

2. **Build:**
   - `Ctrl + Shift + B` veya `Build > Build Solution`

## ⚙️ Konfigürasyon

`GameConfig.h` dosyasından ayarları özelleştirebilirsiniz:

### Process Ayarları
```cpp
constexpr bool USE_PID = true;                    // PID kullanımı
constexpr DWORD TARGET_PID = 12976;               // Hedef process ID
constexpr const char* PROCESS_NAME = "nethan2.bin"; // Process adı
```

### Memory Offset'leri
```cpp
constexpr uintptr_t NET_POINTER_OFFSET = 0x6C3320;
constexpr uintptr_t BATTLE_CALL_OFFSET = 0x0E25B0;
constexpr uintptr_t CPYTHON_PLAYER_OFFSET = 0x6FD4C4;
constexpr uintptr_t TARGET_VID_OFFSET = 0x00A3A74;
```

### Timing Ayarları
```cpp
constexpr int ATTACK_DELAY_MS = 30;    // Attack aralığı (ms)
constexpr int IDLE_DELAY_MS = 50;      // Boşta bekleme süresi
constexpr int THREAD_TIMEOUT_MS = 1000; // Thread timeout
```

## 🎮 Kullanım

### Başlatma
1. **Yönetici olarak çalıştırın**
2. **Metin2 istemcisini başlatın**
3. **ExternalASm.exe'yi çalıştırın**

### Hotkey'ler
- **F2**: Attack başlat
- **F3**: Attack durdur  
- **F4**: Programdan çık

### Çalışma Mantığı
1. Program Metin2 process'ini bulur
2. Memory'den gerekli pointer'ları okur
3. Hedef seçildiğinde otomatik attack başlar
4. Shellcode injection ile attack fonksiyonu çağrılır

## 🔍 Kod Analizi

### Ana Sınıflar

#### `Metin2Attack`
- **Görev**: Attack işlemleri ve memory manipulation
- **Önemli Metodlar**:
  - `Initialize()`: Process bağlantısı ve memory setup
  - `SendAttack()`: Attack fonksiyonu çağrısı
  - `ExecuteRemoteCode()`: Shellcode injection

#### `ProcessUtils`
- **Görev**: Process yönetimi ve utility fonksiyonları
- **Önemli Metodlar**:
  - `GetProcessIdByName()`: Process adından PID bulma
  - `GetModuleBaseAddress()`: Module base address
  - `IsProcessRunning()`: Process durumu kontrolü

### Memory İşlemleri

```cpp
// Shellcode generation
std::vector<BYTE> GenerateAttackShellcode(DWORD targetVID, DWORD realNetPointer, uintptr_t remoteBaseAddr)
{
    // mov ecx, realNetPointer
    // push targetVID  
    // push 0
    // call battleCall (relative)
    // ret
}

// Remote execution
bool ExecuteRemoteCode(DWORD targetVID, DWORD realNetPointer)
{
    // VirtualAllocEx -> WriteProcessMemory -> CreateRemoteThread
}
```

## 🛡️ Güvenlik Notları

- **Antivirus**: Çoğu antivirus tarafından false positive olarak algılanabilir
- **Game Guard**: Metin2'nin koruma sistemi tarafından tespit edilebilir
- **Ban Riski**: Canlı sunucularda kullanım ban riskine neden olur
- **Admin Yetkisi**: Process manipulation için admin yetkisi gereklidir

## 🔧 Geliştirme

### Debug Ayarları
```cpp
#ifdef _DEBUG
    std::cout << "[DEBUG] Target VID: " << targetVID << std::endl;
    std::cout << "[DEBUG] Net Pointer: 0x" << std::hex << realNetPointer << std::endl;
#endif
```

### Yeni Özellik Ekleme
1. `GameConfig.h`'ye yeni ayarlar ekleyin
2. `Metin2Attack` sınıfına yeni metodlar ekleyin
3. `main.cpp`'de hotkey binding'leri güncelleyin

## 📋 Sistemsel Gereksinimler

### Minimum Sistem
- **OS**: Windows 10 x64
- **RAM**: 512 MB
- **CPU**: Intel Core 2 Duo veya AMD Athlon 64
- **Disk**: 10 MB boş alan

### Önerilen Sistem  
- **OS**: Windows 11 x64
- **RAM**: 2 GB+
- **CPU**: Intel Core i3 veya AMD Ryzen 3
- **Disk**: 50 MB boş alan

## 🐛 Sorun Giderme

### Yaygın Hatalar

#### "Process açılamadı!"
- **Çözüm**: Yönetici olarak çalıştırın
- **Sebep**: Process manipulation yetkisi yok

#### "PID bulunamadı!"
- **Çözüm**: `GameConfig.h`'de PID'yi güncelleyin
- **Sebep**: Yanlış PID veya kapalı process

#### "Module bulunamadı!"
- **Çözüm**: Module adını kontrol edin
- **Sebep**: Yanlış module name veya versiyon farkı

## 📚 Kaynaklar

- [Windows API Documentation](https://docs.microsoft.com/en-us/windows/win32/api/)
- [Process and Thread Functions](https://docs.microsoft.com/en-us/windows/win32/procthread/process-and-thread-functions)
- [Virtual Memory Functions](https://docs.microsoft.com/en-us/windows/win32/memory/virtual-memory-functions)

## 🤝 Katkıda Bulunma

1. Fork edin
2. Feature branch oluşturun (`git checkout -b feature/yeni-ozellik`)
3. Commit yapın (`git commit -am 'Yeni özellik eklendi'`)
4. Push edin (`git push origin feature/yeni-ozellik`)
5. Pull Request oluşturun

## 📄 Lisans

Bu proje eğitim amaçlıdır. Ticari kullanım yasaktır.

```
MIT License - Educational Use Only

Bu yazılım sadece eğitim ve araştırma amaçlarıyla kullanılabilir.
Canlı oyun sunucularında kullanımı kesinlikle yasaktır.
```

## ⚠️ Yasal Uyarı

Bu araç sadece **eğitim ve araştırma amaçlıdır**. Aşağıdaki durumlarda sorumluluk kabul edilmez:

- Canlı sunucularda kullanım
- Hesap banı durumları  
- Oyun kurallarının ihlali
- Üçüncü taraf zararları

**Kullanıcı bu aracı kullanarak tüm sorumluluğu kabul eder.**

## 📧 İletişim

- **GitHub Issues**: Bug report ve feature request
- **Discord**: juvesoul

---

> **Geliştirici Notu**: Bu proje Windows memory manipulation ve process injection tekniklerini öğrenmek isteyenler için hazırlanmıştır. Etik kurallara uygun şekilde kullanılması gerekmektedir.
