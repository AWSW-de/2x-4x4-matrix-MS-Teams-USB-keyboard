// ###########################################################################################################################################
// #
// # Code for the printables 2x 4x4 matrix MS Teams USB keyboard project:
// # https://www.printables.com/de/model/402081-2x-4x4-matrix-ms-teams-usb-keyboard
// #
// # Code by https://github.com/AWSW-de using the great library "HID" from NicoHood =)
// #
// # Released under license: GNU General Public License v3.0: https://github.com/AWSW-de/2x-4x4-matrix-MS-Teams-USB-keyboard/blob/main/LICENSE
// #
// ###########################################################################################################################################
/*

      ___           ___                    ___           ___           ___           ___           ___                                                             
     /\__\         /\  \                  /\  \         /\  \         /\  \         /\__\         /\  \                                                            
    /::|  |       /::\  \                 \:\  \       /::\  \       /::\  \       /::|  |       /::\  \                                                           
   /:|:|  |      /:/\ \  \                 \:\  \     /:/\:\  \     /:/\:\  \     /:|:|  |      /:/\ \  \                                                          
  /:/|:|__|__   _\:\~\ \  \                /::\  \   /::\~\:\  \   /::\~\:\  \   /:/|:|__|__   _\:\~\ \  \                                                         
 /:/ |::::\__\ /\ \:\ \ \__\              /:/\:\__\ /:/\:\ \:\__\ /:/\:\ \:\__\ /:/ |::::\__\ /\ \:\ \ \__\                                                        
 \/__/~~/:/  / \:\ \:\ \/__/             /:/  \/__/ \:\~\:\ \/__/ \/__\:\/:/  / \/__/~~/:/  / \:\ \:\ \/__/                                                        
       /:/  /   \:\ \:\__\              /:/  /       \:\ \:\__\        \::/  /        /:/  /   \:\ \:\__\                                                          
      /:/  /     \:\/:/  /              \/__/         \:\ \/__/        /:/  /        /:/  /     \:\/:/  /                                                          
     /:/  /       \::/  /                              \:\__\         /:/  /        /:/  /       \::/  /                                                           
     \/__/         \/__/                                \/__/         \/__/         \/__/         \/__/                                                            
      ___           ___           ___                    ___           ___           ___           ___           ___           ___           ___           ___     
     /\__\         /\  \         /\  \                  /\__\         /\  \         |\__\         /\  \         /\  \         /\  \         /\  \         /\  \    
    /:/  /        /::\  \       /::\  \                /:/  /        /::\  \        |:|  |       /::\  \       /::\  \       /::\  \       /::\  \       /::\  \   
   /:/  /        /:/\ \  \     /:/\:\  \              /:/__/        /:/\:\  \       |:|  |      /:/\:\  \     /:/\:\  \     /:/\:\  \     /:/\:\  \     /:/\:\  \  
  /:/  /  ___   _\:\~\ \  \   /::\~\:\__\            /::\__\____   /::\~\:\  \      |:|__|__   /::\~\:\__\   /:/  \:\  \   /::\~\:\  \   /::\~\:\  \   /:/  \:\__\ 
 /:/__/  /\__\ /\ \:\ \ \__\ /:/\:\ \:|__|          /:/\:::::\__\ /:/\:\ \:\__\     /::::\__\ /:/\:\ \:|__| /:/__/ \:\__\ /:/\:\ \:\__\ /:/\:\ \:\__\ /:/__/ \:|__|
 \:\  \ /:/  / \:\ \:\ \/__/ \:\~\:\/:/  /          \/_|:|~~|~    \:\~\:\ \/__/    /:/~~/~    \:\~\:\/:/  / \:\  \ /:/  / \/__\:\/:/  / \/_|::\/:/  / \:\  \ /:/  /
  \:\  /:/  /   \:\ \:\__\    \:\ \::/  /              |:|  |      \:\ \:\__\     /:/  /       \:\ \::/  /   \:\  /:/  /       \::/  /     |:|::/  /   \:\  /:/  / 
   \:\/:/  /     \:\/:/  /     \:\/:/  /               |:|  |       \:\ \/__/     \/__/         \:\/:/  /     \:\/:/  /        /:/  /      |:|\/__/     \:\/:/  /  
    \::/  /       \::/  /       \::/__/                |:|  |        \:\__\                      \::/__/       \::/  /        /:/  /       |:|  |        \::/__/   
     \/__/         \/__/         ~~                     \|__|         \/__/                       ~~            \/__/         \/__/         \|__|         ~~       

*/


// ###########################################################################################################################################
// # Includes:
// #
// # You will need to add the following libraries to your Arduino IDE to use the project:
// # - HID            // by NicoHood:            https://github.com/NicoHood/HID + WIKI: https://github.com/NicoHood/HID/wiki
// #
// ###########################################################################################################################################

// Version: V1.0.0

// Keyboard layout switch from default ENGLISCH to GERMAN:
// https://github.com/NicoHood/HID/wiki/Keyboard-API#keyboard-layouts
#define HID_CUSTOM_LAYOUT
#define LAYOUT_GERMAN

#include <HID-Settings.h>
#include <HID-Project.h>
#include <HID-APIs\ConsumerAPI.h>

// Debug output on/off in Arduino serial monitor:
int testdebug = 0;  // 1 = on and 0 = off

// Hardware definition:
#define NUM_COLS1 4  // Number of switch columns of keyboard 1
#define NUM_ROWS1 4  // Number of switch rows of keyboard 1
#define NUM_COLS2 4  // Number of switch columns of keyboard 2
#define NUM_ROWS2 4  // Number of switch rows of keyboard 2

// Debounce keys:
#define MAX_DEBOUNCE 3

// Keyboard matrix pins 1:
static const uint8_t RowPins1[NUM_ROWS1] = { 2, 3, 4, 5 };
static const uint8_t ColPins1[NUM_COLS1] = { 6, 7, 8, 9 };

// Keyboard matrix pins 2:
static const uint8_t RowPins2[NUM_ROWS2] = { 10, 16, 14, 15 };
static const uint8_t ColPins2[NUM_COLS2] = { A0, A1, A2, A3 };

// Debounce counter - how long a button has been pressed
static uint8_t debounce_count1[NUM_ROWS1][NUM_COLS1];  // keyboard 1
static uint8_t debounce_count2[NUM_ROWS2][NUM_COLS2];  // keyboard 2

void setup() {
  // Counter to set up pins
  uint8_t i;
  uint8_t y;

  // Set row scan pins to output and then to HIGH (not active)
  for (i = 0; i < NUM_ROWS1; i++) {
    pinMode(RowPins1[i], OUTPUT);
    digitalWrite(RowPins1[i], HIGH);
  }

  for (y = 0; y < NUM_ROWS2; y++) {
    pinMode(RowPins2[y], OUTPUT);
    digitalWrite(RowPins2[y], HIGH);
  }


  // Set column pins to input with pull-up resistors (no need for external pull-up resistors)
  for (i = 0; i < NUM_COLS1; i++) {
    pinMode(ColPins1[i], INPUT_PULLUP);
  }

  for (y = 0; y < NUM_COLS2; y++) {
    pinMode(ColPins2[y], INPUT_PULLUP);
  }

  // Turn off LEDs on the Arduino Pro Micro:
  pinMode(LED_BUILTIN_TX, INPUT);
  pinMode(LED_BUILTIN_RX, INPUT);

  // Initialize USB keyboard
  Keyboard.begin();

  // Initialize the debounce counter array
  memset(debounce_count1, 0, sizeof(debounce_count1));
  memset(debounce_count2, 0, sizeof(debounce_count2));

  Consumer.begin();
}
// ##########################################################################################################################################################

void loop() {
  static uint8_t currentRow1 = 0;  // keyboard 1
  static uint8_t currentCol1;      // keyboard 1
  static uint8_t currentRow2 = 0;  // keyboard 2
  static uint8_t currentCol2;      // keyboard 2

  digitalWrite(RowPins1[currentRow1], LOW);  // keyboard 1
  digitalWrite(RowPins2[currentRow2], LOW);  // keyboard 2

  // Scan button presses on keyboard 1:
  for (currentCol1 = 0; currentCol1 < NUM_COLS1; currentCol1++) {
    if (digitalRead(ColPins1[currentCol1]) == LOW) {
      if (debounce_count1[currentRow1][currentCol1] < MAX_DEBOUNCE) {
        debounce_count1[currentRow1][currentCol1]++;
        if (debounce_count1[currentRow1][currentCol1] == MAX_DEBOUNCE) {
          pressKey1(currentRow1, currentCol1);
          if (testdebug == 1) Serial.println("Keyboard 1: " + String(currentRow1) + "-" + String(currentCol1));  // keyboard 1 output on serial monitor for testing
          delay(500);                                                                                            // Avoid double pressed buttons of a squre button set
        }
      }
    } else {  // Button is released
      if (debounce_count1[currentRow1][currentCol1] > 0) {
        debounce_count1[currentRow1][currentCol1]--;
        if (debounce_count1[currentRow1][currentCol1] == 0) {
          // Not needed currently - release function
        }
      }
    }
  }

  // Scan button presses on keyboard 2:
  for (currentCol2 = 0; currentCol2 < NUM_COLS2; currentCol2++) {
    if (digitalRead(ColPins2[currentCol2]) == LOW) {
      if (debounce_count2[currentRow2][currentCol2] < MAX_DEBOUNCE) {
        debounce_count2[currentRow2][currentCol2]++;
        if (debounce_count2[currentRow2][currentCol2] == MAX_DEBOUNCE) {
          pressKey2(currentRow2, currentCol2);
          if (testdebug == 1) Serial.println("Keyboard 2: " + String(currentRow2) + "-" + String(currentCol2));  // keyboard 2 output on serial monitor for testing
          delay(500);                                                                                            // Avoid double pressed buttons of a squre button set
        }
      }
    } else {  // Button is released
      if (debounce_count2[currentRow2][currentCol2] > 0) {
        debounce_count2[currentRow2][currentCol2]--;
        if (debounce_count2[currentRow2][currentCol2] == 0) {
          // Not needed currently - release function
        }
      }
    }
  }

  // Once done scanning, de-select the row pin
  digitalWrite(RowPins1[currentRow1], HIGH);
  digitalWrite(RowPins2[currentRow2], HIGH);

  // Increment currentRow, so next time we scan the next row
  currentRow1 = (currentRow1 > NUM_ROWS1 - 2) ? 0 : currentRow1 + 1;
  currentRow2 = (currentRow2 > NUM_ROWS2 - 2) ? 0 : currentRow2 + 1;
}
// ##########################################################################################################################################################

// keyboard 1 button assignment to keyboard actions:
// 4 real keybord buttons in a square are bound together to a single button to be pressed
// So f.e. S1+S5+S2+S6 build the combined upper left button and
// the buttons S9+S13+S10+S14 build the combined upper right button...
void pressKey1(uint8_t row, uint8_t col) {
  switch (row) {
    case 0:
      switch (col) {
        case 0:       // S1:
          keysS01();  // --> Button 01
          break;
        case 1:       // S2:
          keysS01();  // --> Button 01
          break;
        case 2:       // S3:
          keysS02();  // --> Button 02
          break;
        case 3:       // S4:
          keysS02();  // --> Button 02
          break;
      }
      break;
    case 1:
      switch (col) {
        case 0:       // S5:
          keysS01();  // --> Button 01
          break;
        case 1:       // S6:
          keysS01();  // --> Button 01
          break;
        case 2:       // S7:
          keysS02();  // --> Button 02
          break;
        case 3:       // S8:
          keysS02();  // --> Button 02
          break;
      }
      break;
    case 2:
      switch (col) {
        case 0:       // S9:
          keysS03();  // --> Button 03
          break;
        case 1:       // S10:
          keysS03();  // --> Button 03
          break;
        case 2:       // S11:
          keysS04();  // --> Button 04
          break;
        case 3:       // S12:
          keysS04();  // --> Button 04
          break;
      }
      break;
    case 3:
      switch (col) {
        case 0:       // S13:
          keysS03();  // --> Button 03
          break;
        case 1:       // S14:
          keysS03();  // --> Button 03
          break;
        case 2:       // S15:
          keysS04();  // --> Button 04
          break;
        case 3:       // S16:
          keysS04();  // --> Button 04
          break;
      }
      break;
  }
}
// ##########################################################################################################################################################

// keyboard 2 button assignment to keyboard actions:
// 4 real keybord buttons in a square are bound together to a single button to be pressed
// So f.e. S1+S5+S2+S6 build the combined upper left button and
// the buttons S9+S13+S10+S14 build the combined upper right button...
void pressKey2(uint8_t row, uint8_t col) {
  switch (row) {
    case 0:
      switch (col) {
        case 0:       // S1:
          keysS05();  // --> Button 05
          break;
        case 1:       // S2:
          keysS05();  // --> Button 05
          break;
        case 2:       // S3:
          keysS06();  // --> Button 06
          break;
        case 3:       // S4:
          keysS06();  // --> Button 06
          break;
      }
      break;
    case 1:
      switch (col) {
        case 0:       // S5:
          keysS05();  // --> Button 05
          break;
        case 1:       // S6:
          keysS05();  // --> Button 05
          break;
        case 2:       // S7:
          keysS06();  // --> Button 06
          break;
        case 3:       // S8:
          keysS06();  // --> Button 06
          break;
      }
      break;
    case 2:
      switch (col) {
        case 0:       // S9:
          keysS07();  // --> Button 07
          break;
        case 1:       // S10:
          keysS07();  // --> Button 07
          break;
        case 2:       // S11:
          keysS08();  // --> Button 08
          ;
          break;
        case 3:       // S12:
          keysS08();  // --> Button 08
          break;
      }
      break;
    case 3:
      switch (col) {
        case 0:       // S13:
          keysS07();  // --> Button 07
          break;
        case 1:       // S14:
          keysS07();  // --> Button 07
          break;
        case 2:       // S15:
          keysS08();  // --> Button 08
          break;
        case 3:       // S16:
          keysS08();  // --> Button 08
          break;
      }
      break;
  }
}
// ##########################################################################################################################################################

// Here are the 8 possible keyboard actions:
// ##########################################################################################################################################################
// For more keybord layout see: https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayouts.h#L61
void keysS01() {
  if (testdebug == 1) Serial.println("Keyboard 1: Button 1");

  // TEAMS: Accept audio call:
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_S);
  Keyboard.release(KEY_LEFT_CTRL);
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_S);
}

void keysS02() {
  if (testdebug == 1) Serial.println("Keyboard 1: Button 2");

  // TEMAS: Mute microphone:
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_M);
  Keyboard.release(KEY_LEFT_CTRL);
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_M);
}

void keysS03() {
  if (testdebug == 1) Serial.println("Keyboard 1: Button 3");

  // TEMAS: Accept video call:
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_A);
  Keyboard.release(KEY_LEFT_CTRL);
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_A);
}

void keysS04() {
  if (testdebug == 1) Serial.println("Keyboard 1: Button 4");

  // TEAMS: Video on/off:
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_O);
  Keyboard.release(KEY_LEFT_CTRL);
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_O);
}

void keysS05() {
  if (testdebug == 1) Serial.println("Keyboard 2: Button 5");

  // WINDOWS: Speaker VOLUME DOWN:
  Consumer.write(MEDIA_VOLUME_DOWN);
}

void keysS06() {
  if (testdebug == 1) Serial.println("Keyboard 2: Button 6");

  // WINDOWS: Speaker VOLUME UP:
  Consumer.write(MEDIA_VOLUME_UP);
}

void keysS07() {
  if (testdebug == 1) Serial.println("Keyboard 2: Button 7");

  // TEAMS: Raise hand:
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_K);
  Keyboard.release(KEY_LEFT_CTRL);
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_K);
}

void keysS08() {
  if (testdebug == 1) Serial.println("Keyboard 2: Button 8");

  // TEAMS: Quit meeting:
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_H);
  Keyboard.release(KEY_LEFT_CTRL);
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_H);
}
// ##########################################################################################################################################################


// Not used actions - kept as examples:
// ##########################################################################################################################################################
// START EXPLORER:
//   Keyboard.press(KEY_LEFT_WINDOWS);
//   Keyboard.press(KEY_E);
//   Keyboard.release(KEY_LEFT_WINDOWS);
//   Keyboard.release(KEY_E);
// ##########################################################################################################################################################
// START EDITOR:
//   Keyboard.press(KEY_LEFT_WINDOWS);
//   Keyboard.release(KEY_LEFT_WINDOWS);
//   delay(100);
//   Keyboard.press(KEY_E);
//   Keyboard.release(KEY_E);
//   Keyboard.press(KEY_D);
//   Keyboard.release(KEY_D);
//   Keyboard.press(KEY_I);
//   Keyboard.release(KEY_I);
//   Keyboard.press(KEY_T);
//   Keyboard.release(KEY_T);
//   Keyboard.press(KEY_O);
//   Keyboard.release(KEY_O);
//   Keyboard.press(KEY_R);
//   Keyboard.release(KEY_R);
//   delay(100);
//   Keyboard.press(KEY_ENTER);
//   Keyboard.release(KEY_ENTER);
// ##########################################################################################################################################################
// RUN COMMAND:
//   Keyboard.press(KEY_LEFT_WINDOWS);
//   Keyboard.press(KEY_R);
//   Keyboard.release(KEY_LEFT_WINDOWS);
//   Keyboard.release(KEY_R);
// ##########################################################################################################################################################
// LOCK WINDOWS:
//   Keyboard.press(KEY_LEFT_WINDOWS);
//   Keyboard.press(KEY_L);
//   Keyboard.release(KEY_LEFT_WINDOWS);
//   Keyboard.release(KEY_L);
// ##########################################################################################################################################################
// TEMAS share:
//   Keyboard.press(KEY_LEFT_CTRL);
//   Keyboard.press(KEY_LEFT_SHIFT);
//   Keyboard.press(KEY_E);
//   Keyboard.release(KEY_LEFT_CTRL);
//   Keyboard.release(KEY_LEFT_SHIFT);
//   Keyboard.release(KEY_E);
// ##########################################################################################################################################################
// TEMAS background settings:
//   Keyboard.press(KEY_LEFT_CTRL);
//   Keyboard.press(KEY_LEFT_SHIFT);
//   Keyboard.press(KEY_P);
//   Keyboard.release(KEY_LEFT_CTRL);
//   Keyboard.release(KEY_LEFT_SHIFT);
//   Keyboard.release(KEY_P);
// ##########################################################################################################################################################
// Speaker MUTE:
//   Consumer.write(MEDIA_VOLUME_MUTE);
// ##########################################################################################################################################################
