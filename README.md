# 🚀 Mehrabtronic Printer Control Center (PCC-4)

> A compact desktop monitor and controller for managing up to **4 network-connected 3D printers** from a single device.

<p align="center">
  <img src="docs/images/device.png" width="700">
</p>

---

## ✨ Features

✅ Monitor up to **4 printers simultaneously**

✅ Real-time printer status

✅ Rotary encoder interface

✅ Character LCD display

✅ Wi-Fi connectivity

✅ ESP32 powered

✅ Modular firmware

✅ Future-ready architecture

---

# 📺 Front Panel

```
┌────────────────────────────────────────────┐

 P1                     P2

 Printing               Fault
 Heating                Heating
 Paused                 Paused
 Finish                 Finish

         LCD 16×2

                 ◉

 P3                     P4

 Printing               Printing
 Heating                Heating
 Paused                 Paused
 Finish                 Finish

└────────────────────────────────────────────┘
```

---

# 💡 Status Indicators

Each printer has its own illuminated status panel.

| Status | Color |
|---------|-------|
| 🟢 Printing | Green |
| 🟡 Heating | Yellow |
| 🟠 Paused | Amber |
| 🔵 Finished | Blue |
| 🔴 Fault | Red |
| ⚫ Offline | Off |

The labels are **backlit** using LEDs mounted behind the front panel, providing excellent visibility from several meters away.

---

# 📟 LCD Status Pages

Rotate the encoder to switch between pages.

---

## Page 1

### Print Progress

```
1:100% 2:65%
3:32%  4:0%
```

Shows the progress of all printers.

---

## Page 2

### Nozzle / Bed Temperature

```
1:215/60 2:220/65
3:0/0    4:205/55
```

Format:

```
Nozzle / Bed
```

---

## Page 3

### Remaining Time

```
1:2h14 2:45m
3:--   4:8h32
```

---

## Page 4

### Speed / Flow

```
1:100/95 2:80/100
3:--/--  4:120/95
```

Format:

```
Speed / Flow
```

---

## Page 5

### Selected Printer File

```
P2

gearbox.gcode
```

Displays the active file for the currently selected printer.

---

# 🚨 Smart Alert Screen

Whenever an important event occurs, the LCD automatically interrupts the current page and displays an alert.

Examples:

```
P2

PRINT DONE
```

```
P3

FILAMENT RUNOUT
```

```
P1

FAULT
```

After acknowledgment, the device returns to the previous page.

---

# 🎛 User Interface

## Status Layer

Rotate encoder

↓

Browse information pages

---

## Command Layer

Press encoder

↓

```
Select Printer

>P1
 P2
 P3
 P4
```

↓

```
Resume

Pause

Stop

Preheat PLA

Preheat PETG

Cooldown

Light

Settings
```

---

# 🔄 Encoder Controls

| Action | Function |
|----------|----------|
| Rotate | Change page / Move cursor |
| Short Press | Select |
| Hold 2 s | Return Home |
| Hold 5 s | Lock Device |

---

# ⚙ Hardware

| Component | Description |
|-----------|-------------|
| MCU | ESP32-S3 |
| Display | 16×2 Character LCD (I²C) |
| Control | Rotary Encoder |
| Connectivity | Wi-Fi |
| Power | USB-C 5V |
| Audio | Active Buzzer |
| LEDs | Backlit Status LEDs |

---

# 🛣 Roadmap

## Version 1.0

- [x] 4 Printer Monitoring
- [x] LCD Interface
- [x] Rotary Navigation
- [x] Alert Screen

---

## Version 1.1

- [ ] RGB Animations
- [ ] Brightness Control
- [ ] Sound Settings
- [ ] OTA Update

---

## Version 2.0

- [ ] Multi-brand Support
- [ ] Bambu Lab
- [ ] Klipper
- [ ] OctoPrint
- [ ] Moonraker

---

## Version 3.0

- [ ] Touch Display Edition
- [ ] Mobile App
- [ ] MQTT Integration
- [ ] Home Assistant

---

# ❤️ Built with

ESP32 • Arduino • 3D Printing • Fusion 360 • C++

---

<p align="center">

Designed and developed by

# Mehrabtronic

Making 3D Printing Smarter.

</p>
