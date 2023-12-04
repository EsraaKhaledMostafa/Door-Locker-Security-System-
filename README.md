# Door-Locker-Security-System-

## Used Hardware:
- ATmega32 Microcontroller.
  
### ECU 1 -> HMI_ECU
- 2x16 LCD with 8-bit mode.
- 4x4 Keypad

### ECU 2 -> Control_ECU
- EEPROM.
- Buzzer.
- DC Motor and H-Bridge.

## SW Drivers:
### ECU 1 -> HMI_ECU
- MCAL:
GPIO
UART
Timer

- HAL:
LCD
Keypad

- APP

- ### ECU 2 -> Control_ECU
- MCAL:
GPIO
UART
Timer
I2C

- HAL:
EEPROM.
Buzzer.
DC Motor and H-Bridge.

- APP


## Project on Proteus: 
![image](https://github.com/EsraaKhaledMostafa/Door-Locker-Security-System-/assets/87395019/0a1cfedb-ad2b-47ba-92ed-26ef81756284)


## Specifications:
- HMI_ECU: is just responsible for interaction with the user taking inputs through the keypad and displaying messages on the LCD.

- CONTROL_ECU: is responsible for all the processing and decisions in the system password
checking, opening the door, and activating the system alarm.

### Step1 – Create a System Password
- The LCD should display “Please Enter Password” 
- Enter a password consisting of 5 numbers, Display * on the screen for each number.
- Press the enter button.
- Ask the user to renter the same password for confirmation by displaying this message
“Please re-enter the same Pass”:
- Enter a password consisting of 5 numbers, Display * on the screen for each number.
- Press the enter button.
  
- HMI_ECU should send the two passwords to the Control_ECU through the UART.
  1- If the two passwords are matched then the system has a password now save it
inside the EEPROM and go to Step 2.

 2- If the two passwords are unmatched then repeat step 1.
 
### Step2 - Main Options
- The LCD will always display the main system option: " Open Door + Change Password -"

### Step3 - Open Door +
- The LCD should display “Please Enter Password” 
- Enter the password then press the enter button.
  
- HMI_ECU should send the Password to the Control_ECU and it should compare it
with the one saved in the EEPROM.

  - if two passwords are matched:
• rotates motor for 15 seconds CW and displays a message on the screen
“Door is Unlocking”
• hold the motor for 3 seconds.
• rotates motor for 15 seconds A-CW and displays a message on the screen
“Door is Locking”

### Step 4 - Change Password -
- The LCD should display “Please Enter Password”
- Enter the password then press enter button.
- HMI_ECU should send the Password to the Control_ECU and it should compare it
with the one saved in the EEPROM.

- if two passwords are matched:
• Repeat Step 1.

### Step 5
- if the two passwords are unmatched at step 3 (+ : Open Door) or step 4 (- : Change
Password)
- Ask the user one more time for the password.
- The LCD should display “Please Enter Password” 
- Enter the password then press the enter button.
- HMI_ECU should send the password to the Control_ECU and it should compare it
with the one saved in the EEPROM.

- if two passwords are matched then open the door or change the password in steps
3 and 4.
  
- If the two passwords are not matched again then ask the user one last time for the
password.

- if two passwords are matched then open the door or change the password in steps
3 and 4.

  - If the two passwords are not matched for the third consecutive time, then:
• Activate Buzzer for 1 minute.
• Display error message on LCD for 1 minute.
• System should be locked no inputs from Keypad will be accepted during
this period.
• Go to Step 2 the main options again.


