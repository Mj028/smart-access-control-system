# smart-access-control-system.
Arduino Smart Access Control System

An Arduino-based Smart Access Control System that implements two alternative authentication methods: RFID card authentication and keypad-based PIN authentication.

The system verifies user credentials and controls a servo motor to simulate an electronic door-lock mechanism. It also provides an LCD interface for menu navigation and real-time authentication feedback.

Project Overview

The Smart Access Control System was developed to explore the practical integration of microcontrollers, electronic peripherals, communication protocols, and embedded programming.

Users can authenticate themselves using either a predefined four-digit PIN or a registered RFID card. When authentication is successful, the system grants access and activates the servo motor. Otherwise, access is denied.

Project Images

Physical Prototype

"Smart Access Control System Prototype" (images/Prototype.jpg)

Circuit Wiring Diagram

"Smart Access Control System Wiring Diagram" (images/wiring%20diagram.png)

Features

- Dual authentication using PIN or RFID.
- Interactive menu displayed on a 16×2 I2C LCD.
- 4×4 matrix keypad for PIN entry and menu navigation.
- RFID card identification using the MFRC522 module.
- Servo-controlled door-lock simulation.
- Automatic relocking after five seconds.
- Access-granted and access-denied feedback.
- Finite State Machine (FSM) program architecture.
- Non-blocking time management using "millis()".

Hardware Components

Component| Function
Arduino Uno R3 (ATmega328P)| Main microcontroller
4×4 Matrix Keypad| PIN entry and navigation
MFRC522 RFID Module| RFID card identification
RFID Card| User authentication
16×2 I2C LCD| User interface and status display
SG90 Servo Motor| Simulates the door lock
Breadboard| Circuit prototyping
Jumper Wires| Electrical connections

How the System Works

1. System Initialization

When powered on, the Arduino initializes the LCD, keypad, RFID reader, and servo motor. The servo starts in the locked position.

2. Main Menu

The LCD displays the main menu, allowing the user to access the authentication system or settings.

The user navigates the menu using the keypad.

3. Authentication Selection

The Access System menu provides two independent authentication options:

- PIN Authentication
- RFID Authentication

The user can select either method without needing to complete both.

4. PIN Authentication

The user enters a predefined four-digit PIN through the keypad.

The configured PIN is:

"1234"

Keypad functions:

Key| Function
"#"| Submit PIN
"*"| Clear entered digits
"D"| Cancel operation

The system compares the entered PIN with the stored PIN.

5. RFID Authentication

The user presents an RFID card to the MFRC522 reader.

The reader obtains the card's Unique Identifier (UID), which the Arduino compares with the registered UID.

Registered RFID UID:

"20 9E D1 3E"

6. Successful Authentication

When the entered PIN or presented RFID card is valid, the LCD displays:

ACCESS GRANTED
Door Unlocked

The servo moves to 90°, simulating an unlocked door.

After five seconds, the servo returns to 0°, simulating automatic relocking.

7. Failed Authentication

When an incorrect PIN or unauthorized RFID card is presented, the LCD displays:

ACCESS DENIED
Try Again

The servo remains in the locked position. After approximately 1.5 seconds, the system returns to the main menu.

Circuit Connections

The following tables describe the connections used in the prototype.

1. I2C LCD

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

Electrical note: The MFRC522 is a 3.3V device. Do not power it from the Arduino's 5V pin. Use appropriate logic-level shifting for the Arduino's 5V output signals connected to the RFID module.

4. SG90 Servo Motor

Servo Wire| Arduino Uno
Brown| GND
Red| 5V supply
Orange / Yellow| A1

An external regulated 5V supply may be needed for reliable servo operation. When using one, connect its ground to the Arduino ground.

Software and Libraries

The project was developed using the Arduino IDE.

Libraries used:

- "Wire.h" – I2C communication.
- "SPI.h" – SPI communication.
- "MFRC522.h" – RFID communication and card identification.
- "Keypad.h" – Matrix keypad interfacing.
- "Servo.h" – Servo motor control.
- "hd44780.h" – LCD library.
- "hd44780_I2Cexp.h" – I2C LCD interface.
- "string.h" – String handling.

Program Architecture

The firmware uses a Finite State Machine (FSM) approach to organize the different operating modes of the system.

The main states are:

State| Description
"MAIN_MENU"| Displays the main menu
"ACCESS_MENU"| Allows authentication method selection
"PIN_MODE"| Handles PIN authentication
"RFID_MODE"| Handles RFID authentication
"SETTINGS_MODE"| Displays the settings interface
"ACCESS_GRANTED"| Handles successful authentication
"ACCESS_DENIED"| Handles failed authentication

A "switch" statement manages the transitions between these states.

The Arduino "millis()" function is used to manage timed operations, including automatic relocking, without relying on long blocking delays.

Concepts Demonstrated

This project provided practical experience with:

- Microcontroller programming.
- Digital input and output interfacing.
- I2C and SPI communication protocols.
- Matrix keypad interfacing.
- RFID identification and authentication.
- LCD interfacing.
- Servo motor control.
- Finite State Machine implementation.
- Non-blocking timing using "millis()".
- Integration of multiple electronic peripherals.

Current Limitations

This project is an educational prototype, not a production-ready security system.

Its current limitations include:

- The PIN is hardcoded in the firmware.
- Only one RFID card UID is registered.
- The Settings menu is currently a placeholder.
- The servo motor simulates a door-lock mechanism rather than operating a full physical door lock.
- Additional security mechanisms would be required for real-world deployment.

Possible Future Improvements

- Allow users to change their PIN.
- Support multiple registered RFID cards.
- Implement administrator authentication.
- Store credentials in non-volatile memory.
- Add an alarm or notification system for repeated failed attempts.
- Design a dedicated PCB and enclosure.
- Improve credential management and overall system security.

Repository Structure

smart-access-control-system/
│
├── Smart_Access_Control.ino
│
├── README.md
│
└── images/
    ├── .gitkeep
    ├── Prototype.jpg
    └── wiring diagram.png

Author

Joseph Matthew Oluwatayo

Electrical and Electronics Engineering Undergraduate

Areas of Focus: Embedded Systems, Robotics and Control Systems.

---

This project represents part of my practical exploration of embedded systems, hardware interfacing, and intelligent electronic control systems.
