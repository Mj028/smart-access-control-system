#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Servo.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <string.h>

// ==========================================
// 1. LCD CONFIGURATION
// ==========================================

hd44780_I2Cexp lcd;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;


// ==========================================
// 2. RFID CONFIGURATION
// ==========================================

#define SS_PIN 10
#define RST_PIN A0

MFRC522 rfid(SS_PIN, RST_PIN);

byte authorizedUID[4] = {
  0x20, 0x9E, 0xD1, 0x3E
};


// ==========================================
// 3. KEYPAD CONFIGURATION
// ==========================================

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'3', '2', '1', 'A'},
  {'6', '5', '4', 'B'},
  {'9', '8', '7', 'C'},
  {'#', '0', '*', 'D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);


// ==========================================
// 4. SERVO CONFIGURATION
// ==========================================

Servo doorServo;

const int SERVO_PIN = A1;

const int LOCKED_POSITION = 0;
const int UNLOCKED_POSITION = 90;

const unsigned long UNLOCK_DURATION = 5000;

unsigned long unlockTime = 0;


// ==========================================
// 5. PIN CONFIGURATION
// ==========================================

const char correctPIN[] = "1234";

char enteredPIN[5] = "";
byte pinIndex = 0;


// ==========================================
// 6. SYSTEM STATES
// ==========================================

enum SystemState {
  MAIN_MENU,
  ACCESS_MENU,
  PIN_MODE,
  RFID_MODE,
  SETTINGS_MODE,
  ACCESS_GRANTED,
  ACCESS_DENIED
};

SystemState currentState = MAIN_MENU;

unsigned long resultTime = 0;

const unsigned long DENIED_DURATION = 1500;


// ==========================================
// 7. FUNCTION DECLARATIONS
// ==========================================

void showMainMenu();
void showAccessMenu();
void showPINScreen();
void showRFIDScreen();
void showSettings();

void handlePIN(char key);
void resetPIN();

void checkRFID();
bool checkAuthorizedCard();

void grantAccess();
void denyAccess();
void updateAccessStatus();


// ==========================================
// 8. SETUP
// ==========================================

void setup() {

  Serial.begin(9600);

  // Initialize LCD
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.backlight();

  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();

  // Initialize servo
  doorServo.attach(SERVO_PIN);
  doorServo.write(LOCKED_POSITION);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SMART ACCESS");

  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  delay(1500);

  showMainMenu();
}


// ==========================================
// 9. MAIN LOOP
// ==========================================

void loop() {

  updateAccessStatus();

  char key = keypad.getKey();

  switch (currentState) {

    // MAIN MENU
    case MAIN_MENU:

      if (key == '1') {

        currentState = ACCESS_MENU;
        showAccessMenu();

      }

      else if (key == '2') {

        currentState = SETTINGS_MODE;
        showSettings();

      }

      break;


    // ACCESS SELECTION
    case ACCESS_MENU:

      if (key == '1') {

        resetPIN();

        currentState = PIN_MODE;
        showPINScreen();

      }

      else if (key == '2') {

        currentState = RFID_MODE;
        showRFIDScreen();

      }

      else if (key == '*') {

        currentState = MAIN_MENU;
        showMainMenu();

      }

      break;


    // PIN AUTHENTICATION
    case PIN_MODE:

      if (key != NO_KEY) {

        handlePIN(key);

      }

      break;


    // RFID AUTHENTICATION
    case RFID_MODE:

      if (key == '*') {

        currentState = ACCESS_MENU;
        showAccessMenu();

      }

      else {

        checkRFID();

      }

      break;


    // SETTINGS
    case SETTINGS_MODE:

      if (key == '*') {

        currentState = MAIN_MENU;
        showMainMenu();

      }

      break;


    // ACCESS RESULT STATES
    case ACCESS_GRANTED:
    case ACCESS_DENIED:

      // updateAccessStatus() handles these states.

      break;

  }

}


// ==========================================
// 10. DISPLAY MAIN MENU
// ==========================================

void showMainMenu() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("1.Access System");

  lcd.setCursor(0, 1);
  lcd.print("2.Settings");

}


// ==========================================
// 11. DISPLAY ACCESS MENU
// ==========================================

void showAccessMenu() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("1. Use PIN");

  lcd.setCursor(0, 1);
  lcd.print("2. Use RFID");

}


// ==========================================
// 12. DISPLAY PIN SCREEN
// ==========================================

void showPINScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ENTER YOUR PIN");

  lcd.setCursor(0, 1);
  lcd.print("PIN: ");

}


// ==========================================
// 13. DISPLAY RFID SCREEN
// ==========================================

void showRFIDScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("RFID ACCESS");

  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card");

}


// ==========================================
// 14. DISPLAY SETTINGS
// ==========================================

void showSettings() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SETTINGS");

  lcd.setCursor(0, 1);
  lcd.print("* Back to Menu");

}


// ==========================================
// 15. RESET PIN
// ==========================================

void resetPIN() {

  pinIndex = 0;
  enteredPIN[0] = '\0';

}


// ==========================================
// 16. HANDLE PIN INPUT
// ==========================================

void handlePIN(char key) {

  // CANCEL
  if (key == 'D') {

    resetPIN();

    currentState = ACCESS_MENU;
    showAccessMenu();

    return;

  }


  // CLEAR
  if (key == '*') {

    resetPIN();
    showPINScreen();

    return;

  }


  // SUBMIT
  if (key == '#') {

    if (pinIndex == 4 &&
        strcmp(enteredPIN, correctPIN) == 0) {

      Serial.println("PIN Authentication Successful");

      grantAccess();

    }

    else {

      Serial.println("PIN Authentication Failed");

      denyAccess();

    }

    resetPIN();

    return;

  }


  // ACCEPT NUMBERS ONLY
  if (key >= '0' && key <= '9') {

    if (pinIndex < 4) {

      enteredPIN[pinIndex] = key;

      lcd.setCursor(5 + pinIndex, 1);
      lcd.print('*');

      pinIndex++;

      enteredPIN[pinIndex] = '\0';

    }

  }

}


// ==========================================
// 17. CHECK RFID
// ==========================================

void checkRFID() {

  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card UID: ");

  for (byte i = 0; i < rfid.uid.size; i++) {

    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");

  }

  Serial.println();


  if (checkAuthorizedCard()) {

    Serial.println("RFID Authentication Successful");

    grantAccess();

  }

  else {

    Serial.println("RFID Authentication Failed");

    denyAccess();

  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

}


// ==========================================
// 18. VERIFY RFID UID
// ==========================================

bool checkAuthorizedCard() {

  if (rfid.uid.size != 4) {
    return false;
  }

  for (byte i = 0; i < 4; i++) {

    if (rfid.uid.uidByte[i] != authorizedUID[i]) {

      return false;

    }

  }

  return true;

}


// ==========================================
// 19. GRANT ACCESS
// ==========================================

void grantAccess() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ACCESS GRANTED");

  lcd.setCursor(0, 1);
  lcd.print("Door Unlocked");

  doorServo.write(UNLOCKED_POSITION);

  unlockTime = millis();

  currentState = ACCESS_GRANTED;

}


// ==========================================
// 20. DENY ACCESS
// ==========================================

void denyAccess() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ACCESS DENIED");

  lcd.setCursor(0, 1);
  lcd.print("Try Again");

  doorServo.write(LOCKED_POSITION);

  resultTime = millis();

  currentState = ACCESS_DENIED;

}


// ==========================================
// 21. AUTOMATIC RELOCKING
// ==========================================

void updateAccessStatus() {

  // Successful authentication
  if (currentState == ACCESS_GRANTED) {

    if (millis() - unlockTime >= UNLOCK_DURATION) {

      doorServo.write(LOCKED_POSITION);

      currentState = MAIN_MENU;

      showMainMenu();

      Serial.println("Door Locked");

    }

  }


  // Failed authentication
  else if (currentState == ACCESS_DENIED) {

    if (millis() - resultTime >= DENIED_DURATION) {

      currentState = MAIN_MENU;

      showMainMenu();

    }

  }

}
