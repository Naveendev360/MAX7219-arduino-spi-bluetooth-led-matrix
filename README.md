# MAX7219-arduino-spi-bluetooth-led-matrix

An innovative **IoT project** using **Arduino Nano** as an SPI Master to control two **MAX7219-based 8x8 LED matrix displays** (SPI Slaves) with **wireless Bluetooth communication (HC-05)**. This setup enables real-time text updates from a phone or PC over Bluetooth.

---

## 🌟 Project Overview

This prototype demonstrates:
- Master-Slave architecture using SPI
- Dual display update via Bluetooth
- Real-time, wireless text transmission
- Embedded systems + communication protocols integration

---

## 🔧 Hardware Used

| Component              | Quantity |
|------------------------|----------|
| Arduino Nano           | 1        |
| MAX7219 8x8 LED Matrix | 2        |
| HC-05 Bluetooth Module | 1        |
| Jumper wires, breadboard, power supply | ✅ |

---

## 📡 System Architecture

```
Phone/PC (Bluetooth Terminal)
        ↓
     [HC-05]
        ↓ UART
   [Arduino Nano]
        ↓ SPI
[LED Matrix 1]  [LED Matrix 2]
```

- **SPI**: Nano → MAX7219 (2 devices)
- **UART**: Nano ↔ HC-05 for wireless control

---

## 🚀 How It Works

### ✅ Default Text
- Matrix 1 shows: `"LEFT"`
- Matrix 2 shows: `"RIGHT"`

### ✅ Bluetooth Commands

| Command Format | Action                        |
|----------------|-------------------------------|
| `M1:Hello`     | Displays "Hello" on matrix 1  |
| `M2:World`     | Displays "World" on matrix 2  |
| `RST`          | Resets both displays to default |

Example:  
`M1:Welcome` → Matrix 1 will scroll "Welcome"

---

## 🧠 Skills Gained

- SPI Bus Handling with Multiple Slaves  
- Wireless Communication via Bluetooth  
- Real-Time Data Display in Embedded Systems  
- Debugging & Optimization of Microcontroller Code  

---

## 💻 Arduino Code

[📂 View Full Code in `/src/main.ino`](./src/main.ino)

Or read below:

<details>
<summary>Click to Expand</summary>

```cpp
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SoftwareSerial.h>

#define BT_RX 2
#define BT_TX 3
SoftwareSerial bluetooth(BT_RX, BT_TX);

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN_1 10
#define CS_PIN_2 9

MD_Parola matrix1 = MD_Parola(HARDWARE_TYPE, CS_PIN_1, MAX_DEVICES);
MD_Parola matrix2 = MD_Parola(HARDWARE_TYPE, CS_PIN_2, MAX_DEVICES);

char msg1[32] = "LEFT";
char msg2[32] = "RIGHT";

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);

  matrix1.begin();
  matrix2.begin();
  matrix1.setZoneEffect(0, true, PA_FLIP_UD);
  matrix2.setZoneEffect(0, true, PA_FLIP_UD);
  matrix1.setZoneEffect(0, true, PA_FLIP_LR);
  matrix2.setZoneEffect(0, true, PA_FLIP_LR);

  matrix1.setIntensity(8);
  matrix2.setIntensity(8);

  resetDisplays();

  bluetooth.println("READY - Send commands:");
  bluetooth.println("M1:Text - Left matrix");
  bluetooth.println("M2:Text - Right matrix");
  bluetooth.println("RST - Reset displays");
}

void resetDisplays() {
  matrix1.displayClear();
  matrix2.displayClear();
  matrix1.displayText(msg1, PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  matrix2.displayText(msg2, PA_CENTER, 100, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
}

void loop() {
  if (bluetooth.available()) {
    String input = bluetooth.readStringUntil('\n');
    input.trim();

    if (input.startsWith("M1:")) {
      input.substring(3).toCharArray(msg1, 32);
      updateDisplay(1);
      bluetooth.print("LEFT: ");
      bluetooth.println(msg1);
    } else if (input.startsWith("M2:")) {
      input.substring(3).toCharArray(msg2, 32);
      updateDisplay(2);
      bluetooth.print("RIGHT: ");
      bluetooth.println(msg2);
    } else if (input.equalsIgnoreCase("RST")) {
      resetDisplays();
      bluetooth.println("Displays reset");
    }
  }

  matrix1.displayAnimate();
  matrix2.displayAnimate();
}

void updateDisplay(byte matrixNum) {
  if (matrixNum == 1) {
    matrix1.displayReset();
    matrix1.displayText(msg1, PA_CENTER, 100, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
  } else {
    matrix2.displayReset();
    matrix2.displayText(msg2, PA_CENTER, 100, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
  }
}
```

</details>

---

## 🌍 Application Ideas

- Smart dashboards with wireless updates  
- Remote alert or sensor display panels  
- Interactive Bluetooth-based text boards  
- Custom IoT interfaces using displays

---
## 🎥 Demo Video

[![Watch the demo](https://img.youtube.com/vi/YOUR_VIDEO_ID/0.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID)

## 👥 Collaborators

Special thanks to [Naveen Kumar](https://github.com/naveendev360) for engineering support and prototyping efforts.

---
