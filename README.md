ESP32 RFID Door Access System

A smart door access control system based on ESP32 and RFID module. This project allows access to a door using RFID cards (such as student IDs) and is implemented on a custom-designed PCB for compactness and real-world integration.

Features

- Based on ESP32 for fast and wireless-capable processing
- Reads RFID tags using MFRC522 module (13.56MHz)
- Matches UIDs against a list of authorized users
- Controls relay to unlock a door or activate electronic access
- Serial feedback for debugging and card registration
- Built on a custom PCB (designed with KiCad/Eagle/etc.)

 Hardware Overview

- ESP32 module (e.g. DevKitC or custom design)
- MFRC522 RFID Reader
- 3.3V Voltage Regulator (for MFRC522)
- Custom PCB (Designed and manufactured)
- LED / Buzzer for feedback
- Button for reset

🔌 Circuit Diagram & PCB

- The project runs on a custom-designed PCB.  
- You can find schematic and board layout files in the hardware/ folder.  


## 💻 How to Use

1. Power the ESP32 and connect the RFID module (SPI interface).
2. Upload the code from RFID_Access_ESP32.ino using  PlatformIO.
3. Open Serial Monitor (baud 115200) to scan new RFID tags.
4. Add authorized UID tags into the code.
5. On detection of an authorized tag, relay activates to unlock the door.
