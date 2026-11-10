# smart-access-control-system
Arduino Smart Access Control System

An Arduino-based smart access control prototype that uses RFID authentication and a keypad-based PIN system to control access to a simulated electronic door lock.

The project demonstrates the integration of embedded systems, electronic components, user input interfaces, and state-based programming to implement a basic access control mechanism.

Project Overview

The Smart Access Control System provides two alternative authentication methods:

- PIN Authentication: Users enter a predefined four-digit PIN through a 4×4 matrix keypad.
- RFID Authentication: Users present a registered RFID card to the MFRC522 reader.

When authentication is successful, the system displays an access-granted message and activates a servo motor to simulate unlocking a door. The door automatically returns to its locked position after five seconds.

Project Images

Physical Prototype

"Smart Access Control System Prototype" (images/prototype.jpg)

Circuit Wiring Diagram

"Smart Access Control System Wiring Diagram" (images/wiring-diagram.png)

Hardware Components

Component| Purpose
Arduino Uno R3 (ATmega328P)| Main microcontroller
4×4 Matrix Keypad| PIN entry and menu navigation
MFRC522 RFID Module| RFID card identification
RFID Card| User authentication
16×2 I2C LCD| Displays menus and system status
SG90 Servo Motor| Simulates the door locking mechanism
Breadboard| Circuit prototyping
Jumper Wires| Electrical connections

System Features

- Two alternative authentication methods: PIN and RFID.
- Interactive menu displayed on a 16×2 LCD.
- Keypad-based navigation and PIN entry.
- RFID card identification and verification.
- Servo-controlled door-lock simulation.
- Automatic relocking after five seconds.
- Separate access-granted and access-denied responses.
- State-based program structure for managing system operations.

How the System Works

1. System Initialization

When powered on, the Arduino initializes the LCD, keypad, RFID reader, and servo motor. The servo starts in the locked position.

2. Main Menu

The LCD displays the main menu:

SMART ACCESS
1.Access 2.Settings

The user selects an option using the keypad.

3. Authentication Selection

Under the Access System menu, the user can choose either:

- PIN authentication
- RFID authentication

The two methods operate independently. A user does not need to provide both credentials.

4. PIN Authentication

The user enters the four-digit PIN using the keypad.

- "#" submits the entered PIN.
- "*" clears the entered digits.
- "D" cancels the operation.

The predefined PIN is "1234".

5. RFID Authentication

The user presents an RFID card to the MFRC522 reader.

The system reads the card's Unique Identifier (UID) and compares it with the registered UID.

Registered card UID:

20 9E D1 3E

6. Access Granted

When either authentication method is successful, the LCD displays:

ACCESS GRANTED
Door Unlocked

The servo moves to 90°, simulating the unlocking of the door. After five seconds, it returns to 0°, simulating automatic relocking.

7. Access Denied

If an incorrect PIN or an unauthorized RFID card is presented, the LCD displays:

ACCESS DENIED
Try Again

The servo remains in the locked position. After approximately 1.5 seconds, the system returns to the main menu.

Circuit Connections

The following tables provide the pin connections used in the prototype.

1. LCD (I2C Interface)

LCD Pin| Arduino Uno
GND| GND
VCC| 5V
SDA| A4
SCL| A5

I2C address: "0x27"

2. 4×4 Matrix Keypad

Keypad Connection| Arduino Uno
Row 1| D2
Row 2| D3
Row 3| D4
Row 4| D5
Column 1| D6
Column 2| D7
Column 3| D8
Column 4| D9

3. MFRC522 RFID Module

RFID Pin| Arduino Uno
SDA / SS| D10
SCK| D13
MOSI| D11
MISO| D12
RST| A0
GND| GND
3.3V| 3.3V
IRQ| Not connected

Important: The MFRC522 operates at 3.3V. Do not connect its power pin to the Arduino's 5V supply. Appropriate level shifting should also be used on the Arduino's 5V output signals going to the RFID module.

4. SG90 Servo Motor

Servo Wire| Connection
Brown| GND
Red| 5V supply
Orange / Yellow| A1

For reliable operation, an appropriate external 5V supply may be required for the servo. If an external supply is used, connect its ground to the Arduino ground.

Software and Libraries

The project was developed using the Arduino IDE and the following libraries:

- "Wire.h" – I2C communication.
- "SPI.h" – SPI communication with the RFID module.
- "MFRC522.h" – RFID card reading and identification.
- "Keypad.h" – Matrix keypad interfacing.
- "Servo.h" – Servo motor control.
- "hd44780.h" and "hd44780_I2Cexp.h" – LCD interfacing.
- "string.h" – String handling.

Program Architecture

The firmware uses a Finite State Machine (FSM) approach to organize the different operating modes of the access control system.

The main states are:

State| Function
"MAIN_MENU"| Displays the main menu
"ACCESS_MENU"| Allows selection of authentication method
"PIN_MODE"| Handles keypad PIN authentication
"RFID_MODE"| Handles RFID authentication
"SETTINGS_MODE"| Displays the settings screen
"ACCESS_GRANTED"| Handles successful authentication
"ACCESS_DENIED"| Handles failed authentication

The "switch" statement manages transitions between states.

The Arduino "millis()" function is used to manage the automatic relocking interval without relying on a long blocking delay.

Key Concepts Demonstrated

Through this project, I explored and applied several embedded systems concepts:

- Digital input and output interfacing.
- I2C and SPI communication protocols.
- RFID identification and authentication.
- Matrix keypad scanning.
- LCD interfacing.
- Servo motor control.
- Finite State Machine (FSM) implementation.
- Non-blocking time management using "millis()".
- Integration of multiple hardware peripherals.

Current Limitations

This project is an educational prototype and is not intended for deployment as a production security system.

Current limitations include:

- The PIN is hardcoded in the firmware.
- Only one RFID card UID is registered.
- The Settings menu is currently a placeholder.
- The servo motor provides a physical simulation of a door lock.
- Additional security measures would be required for real-world access control.

Possible Future Improvements

- Allow users to change their PIN.
- Support multiple registered RFID cards.
- Implement an administrator authentication mode.
- Store credentials in non-volatile memory.
- Add an alarm or notification system for repeated failed attempts.
- Design a dedicated PCB and enclosure.
- Improve credential management and system security.

Repository Structure

smart-access-control-system/
│
├── Smart_Access_Control.ino
│
├── README.md
│
└── images/
    ├── prototype.jpg
    └── wiring-diagram.png

Author

Joseph Matthew Oluwatayo

Electrical and Electronics Engineering Undergraduate

Areas of Focus: Embedded Systems, Robotics and Control Systems.

---

This project was developed as part of my practical exploration of embedded systems, hardware interfacing, and intelligent electronic control systems.
