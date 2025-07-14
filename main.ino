#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SoftwareSerial.h>

// Bluetooth Configuration
#define BT_RX 2   // HC-05 TX → Arduino D2
#define BT_TX 3   // HC-05 RX → Arduino D3
SoftwareSerial bluetooth(BT_RX, BT_TX);

// LED Matrix Configuration
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4       // 4x 8x8 matrices per display
#define CS_PIN_1 10         // Matrix 1 CS pin
#define CS_PIN_2 9          // Matrix 2 CS pin

MD_Parola matrix1 = MD_Parola(HARDWARE_TYPE, CS_PIN_1, MAX_DEVICES);
MD_Parola matrix2 = MD_Parola(HARDWARE_TYPE, CS_PIN_2, MAX_DEVICES);

// Message buffers
char msg1[32] = "LEFT";  // Default text
char msg2[32] = "RIGHT";

void setup() {
  // Initialize serial
  bluetooth.begin(9600);
  Serial.begin(9600);
  
  // Initialize matrices with proper orientation
  matrix1.begin();
  matrix2.begin();
  
  // Display orientation settings
  matrix1.setZoneEffect(0, true, PA_FLIP_UD);  // Flip upside-down
  matrix2.setZoneEffect(0, true, PA_FLIP_UD);  // Flip upside-down
  matrix1.setZoneEffect(0, true, PA_FLIP_LR);
  matrix2.setZoneEffect(0, true, PA_FLIP_LR);


  // Set brightness (0-15)
  matrix1.setIntensity(8);
  matrix2.setIntensity(8);
  
  // Initialize displays with LEFT-TO-RIGHT scrolling
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
  // Check Bluetooth input
  if (bluetooth.available()) {
    String input = bluetooth.readStringUntil('\n');
    input.trim();
    
    if (input.startsWith("M1:")) {
      input.substring(3).toCharArray(msg1, 32);
      updateDisplay(1);
      bluetooth.print("LEFT: ");
      bluetooth.println(msg1);
    }
    else if (input.startsWith("M2:")) {
      input.substring(3).toCharArray(msg2, 32);
      updateDisplay(2);
      bluetooth.print("RIGHT: ");
      bluetooth.println(msg2);
    }
    else if (input.equalsIgnoreCase("RST")) {
      resetDisplays();
      bluetooth.println("Displays reset");
    }
  }

  // Animate displays
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
