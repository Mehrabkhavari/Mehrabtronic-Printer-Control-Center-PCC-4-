\# 🖨️ Mehrabtronic Smart 3D Printer Dashboard



> ⚡ An ESP32-based desktop dashboard for real-time monitoring and visual status indication of multiple network-connected 3D printers.



\---



\## 🚀 Project Overview



The \*\*Mehrabtronic Smart 3D Printer Dashboard\*\* is a dedicated hardware and software system designed to monitor multiple network-connected 3D printers from a single desktop interface.



The system uses an \*\*ESP32\*\* as the main controller and communicates with Creality-compatible printers over Wi-Fi through the \*\*CrealityKE\*\* software library.



The first firmware version supports \*\*four printers\*\* and provides immediate visual status feedback using dedicated RGB-style status indicators.



\---



\## ✨ Current Features



🖨️ \*\*4 Printer Support\*\*



Monitor up to four network-connected 3D printers simultaneously.



📡 \*\*Wi-Fi Communication\*\*



The ESP32 communicates with each printer through the local network.



🌡️ \*\*Real-Time Printer Data\*\*



The system can retrieve information such as:



\- Nozzle temperature

\- Bed temperature

\- Target temperatures

\- Print progress

\- Current layer

\- Total layers

\- Print time

\- Remaining print time

\- Printer state

\- Fan status

\- Filament status

\- Connection status

\- Printer model

\- Firmware version



💡 \*\*Visual Status Indicators\*\*



Each printer has three dedicated LEDs:



🟢 Green → Printing / Finished  

🟡 Yellow → Paused  

🔴 Red → Fault / Disconnected  

⚫ LEDs OFF → Idle / Ready



\---



\## 🧠 Printer Status Logic



The dashboard uses a simple logical state model:



| State | Indicator |

|---|---|

| Idle / Ready | ⚫ All LEDs OFF |

| Printing | 🟢 Green ON |

| Paused | 🟡 Yellow ON |

| Finished | 🟢 Green Blinking |

| Fault | 🔴 Red ON |

| Disconnected | 🔴 Red ON |



\### ⚠️ Status Priority



Fault and disconnection always have the highest priority.



```text

FAULT / DISCONNECTED

&#x20;       ↓

&#x20;     🔴 RED

&#x20;       ↓

&#x20;    PAUSED

&#x20;       ↓

&#x20;   🟡 YELLOW

&#x20;       ↓

&#x20;   PRINTING

&#x20;       ↓

&#x20;   🟢 GREEN

&#x20;       ↓

&#x20;     IDLE

&#x20;       ↓

&#x20;     OFF

