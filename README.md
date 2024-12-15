# SMS based home automation project using SIM900A GSM Module

## Pin Diagram

* Arduino (4,5) --> SIM900A (RX,TX)
* Arduino (8) --> RELAY1
* Arduino (9) --> RELAY2
* Arduino (10) --> RELAY3

## Files

* `GSM.ino` main code for the project
* `Serial.ino` Serial console for SIM900A management
* `SIM900_AT.pdf` SIM900A AT Command Reference

## SMS Commands

* `101` To turn on 1st and 3rd pin, and turn off 2nd pin
* `100` To turn on only the 1st pin, turn off 2nd and 3rd pin
* `01X` To turn off 1st pin, turn off 2nd pin and ignore 3rd pin
* `X01` To ignore 1st pin, turn off 2nd pin and turn on 3rd pin
* `111` To turn on all pins
* `000` To turn off all pins
* `get` to get the state of all pins status via SMS

Related Post: [Receive SMS from SIM900A Kit using Arduino](https://blog.mdminhazulhaque.io/receive-sms-from-sim900a-kit-using-arduino)
