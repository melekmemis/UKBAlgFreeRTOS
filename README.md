# UKBAlgFreeRTOS — Roket Uçuş Bilgisayarı Yazılımı

FreeRTOS tabanlı, STM32F407 üzerinde çalışan gerçek zamanlı roket uçuş kontrol ve veri toplama sistemi. Sensör okumaları, uçuş fazı tespiti, paraşüt tetikleme ve LoRa ile telemetri iletimi eş zamanlı görevler halinde yönetilir.

---

## İçindekiler

- [Donanım](#donanım)
- [Yazılım Mimarisi](#yazılım-mimarisi)
- [Görevler (Tasks)](#görevler-tasks)
- [Uçuş State Machine](#uçuş-state-machine)
- [Kalman Filtresi](#kalman-filtresi)
- [Proje Yapısı](#proje-yapısı)
- [Derleme ve Yükleme](#derleme-ve-yükleme)
- [Bağımlılıklar](#bağımlılıklar)

---

## Donanım

| Bileşen | Açıklama |
|---|---|
| **MCU** | STM32F407VGTx (ARM Cortex-M4F, 168 MHz) |
| **IMU** | BNO055 — 9-eksen ivme, jiroskop, oryantasyon (I2C) |
| **Barometre** | MS5611 — yüksek çözünürlüklü basınç/irtifa sensörü (I2C) |
| **GPS** | Queltel L86-M33 GPS Modülü (USART1) |
| **LoRa** | LoRa E220-400T30S RF Modülü (USART2) |
| **SD Kart** | FatFS ile SPI üzerinden uçuş verisi kaydı (SPI3) |

---

## Yazılım Mimarisi

```
main.c
 ├── KalmanInitAll()       — Tüm filtreler başlatılır
 ├── HAL / Çevre birimi init
 ├── osKernelInitialize()
 ├── createTasks()         — 8 FreeRTOS görevi oluşturulur
 └── osKernelStart()
```

Görevler arası iletişim FreeRTOS bildirim mekanizmaları (`vTaskNotifyGiveFromISR`, `ulTaskNotifyTake`) ile sağlanır. GPS ve RS görevleri DMA + UART boş satır kesmesiyle (`HAL_UARTEx_RxEventCallback`) tetiklenir.

---

## Görevler (Tasks)

| Görev | Öncelik | Yığın | Açıklama |
|---|---|---|---|
| `BnoTask` | Normal | 1 KB | BNO055'ten ivme, jiroskop ve açı verisi okur (100 ms) |
| `GPSTask` | Normal | 2 KB | DMA ile NMEA cümlelerini alır, ayrıştırır |
| `MsTask` | Normal | 1 KB | MS5611'den basınç ve irtifa hesaplar (90 ms) |
| `RsTask` | Realtime | 1 KB | RS seri porttan komut alır, mod kontrolü yapar |
| `RsSendTask` | Realtime | 1 KB | SIT modunda test cihazına, SUT modunda durum paketi gönderir (100 ms) |
| `LoRaTask` | Normal | 1 KB | Tüm sensör verisini LoRa ile iletir (200 ms) |
| `flightStatTask` | Normal | 1 KB | Uçuş fazlarını kontrol eder, paraşütleri tetikler (100 ms) |
| `sdCardTask` | Normal | 2 KB | FatFS ile SD karta uçuş verisi yazar (100 ms) |

---

## Uçuş State Machine

`flightStatus.c` içinde tanımlı ardışık faz tespiti:

```
[Bekleme]
    │  accZ ≥ 1.6 g
    ▼
[Kalkış — liftOff]
    │  İvme ani düşüşü (burnOut)
    ▼
[Motor Yanması Bitti — burnOut]
    │  irtifa < -20 m (altThrs)
    ▼
[İrtifa Eşiği Aşıldı]
    │  pitch/roll > ±45° AND irtifa düşüyor (altCond + angleCond)
    ▼
[Sürükleme Paraşütü — dragStep]
    │  GPIO: drogueOut → 800 ms aktif
    │  irtifa < 600 m
    ▼
[Ana Paraşüt — mainStep]
    │  GPIO: mainOut → 800 ms aktif
    ▼
[İniş]
```

Her faz geçişi `statusPackData1` bayrağına bit olarak işlenir ve LoRa ile yer istasyonuna iletilir.

---

## Kalman Filtresi

`kalmanFilter.c` içinde skaler (1D) Kalman filtresi uygulaması bulunur. Her sensör kanalı için bağımsız filtre örneği:

| Filtre Grubu | Q | R |
|---|---|---|
| İvme (Ax, Ay, Az) | 0.02 | 0.20 |
| Jiroskop (Gx, Gy, Gz) | 0.02 | 0.20 |
| Açılar (pitch, roll, yaw) | 0.04 | 0.40 |
| Basınç / İrtifa | 0.005 | 0.05 |

Filtreler `main()` başında, RTOS başlamadan önce `KalmanInitAll()` ile başlatılır.

---

## Proje Yapısı

```
UKBAlgFreeRTOS/
├── Core/
│   ├── Inc/                  # Başlık dosyaları
│   └── Src/
│       ├── bno055/           # BNO055 sürücüsü
│       ├── ms5611/           # MS5611 barometre sürücüsü
│       ├── gps/              # NMEA ayrıştırıcı
│       ├── kalmanfilter/     # Kalman filtresi
│       ├── flightstatus/     # Uçuş fazı tespiti
│       ├── freertos/         # FreeRTOS görev tanımları
│       ├── lora/             # LoRa iletişim katmanı
│       ├── sdcard/           # SD kart yönetimi
│       ├── system/           # HAL MSP, IRQ, sistem init
│       └── main.c
├── Drivers/
│   ├── CMSIS/                # ARM CMSIS çekirdek başlıkları
│   └── STM32F4xx_HAL_Driver/ # ST HAL kütüphanesi
├── Middlewares/
│   └── Third_Party/
│       ├── FreeRTOS/         # FreeRTOS v10 (CMSIS-RTOS v2)
│       └── FatFs/            # FatFS dosya sistemi
├── FATFS/                    # FatFS yapılandırması ve SPI sürücüsü
└── UKBAlgFreeRTOS.ioc        # STM32CubeIDE pin yapılandırması
```

---

## Derleme ve Yükleme

**Gereksinimler:**
- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) (v1.13+)
- ST-LINK/V2 veya uyumlu debug adaptörü

**Adımlar:**

1. Repoyu klonlayın:
   ```bash
   git clone https://github.com/melekmemis/UKBAlgFreeRTOS.git
   ```

2. STM32CubeIDE'yi açın → `File > Open Projects from File System` → proje klasörünü seçin.

3. `Debug` yapılandırmasını seçip **Build** edin (Ctrl+B).

4. STM32F407 kartını ST-LINK ile bağlayın ve **Run** / **Debug** ile yükleyin.

---

## Bağımlılıklar

| Kütüphane | Sürüm | Lisans |
|---|---|---|
| FreeRTOS (CMSIS-RTOS v2) | 10.x | MIT |
| STM32F4xx HAL Driver | ST BSP | BSD-3 |
| FatFs | R0.14b | FatFs Lisansı |
| CMSIS Core | 5.x | Apache-2.0 |

---

## Lisans

Bu proje MIT lisansı ile dağıtılmaktadır. Üçüncü taraf kütüphaneler kendi lisanslarına tabidir; detaylar ilgili `LICENSE.txt` dosyalarında yer almaktadır.
