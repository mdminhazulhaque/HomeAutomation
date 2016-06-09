#include <SoftwareSerial.h>

SoftwareSerial SIM900A(4, 5);
const String number = "+8801742648628";

const int pins[3] = {8, 9, 10};
const String devs[3] = {"FAN", "LIGHT", "TV"};
int state[3] = {0, 0, 0};

/**
 * @brief Reads the content of GSM Module at a time into a string and returns it
 */
String readString()
{
    String buffer;

    while (SIM900A.available())
    {
        char c = SIM900A.read();
        buffer.concat(c);
        delay(10);
    }

    return buffer;
}

/**
 * @brief For all pins, read the pin's corresponding state value, if 0 turns off that pin, else turns it on
 */
void updateState()
{
    for (int i = 0; i < 3; i++)
    {
        if (state[i] == 0)
        {
            digitalWrite(pins[i], LOW);
        }
        else
        {
            digitalWrite(pins[i], HIGH);
        }
    }
}

/**
 * @brief Composes a string containing all pins' state and device name connected to the corresponding pin
 */
void getState()
{
    // Set SMS mode to plain text
    SIM900A.println("AT+CMGF=1");
    delay(500);
    // Set SMS destination number
    SIM900A.println("AT+CMGS=\"" + number + "\"\r");
    delay(500);

    String sms = "";

    for (int i = 0; i < 3; i++)
    {
        sms += devs[i];
        sms += " ";

        if (state[i] == 0)
        {
            sms += "OFF";
        }
        else
        {
            sms += "ON";
        }
        sms += "\n";
    }
    
    // Show SMS body to Serial
    Serial.print(sms);

    // Write SMS body to GSM Module
    SIM900A.println(sms);
    delay(500);
    // Write 0x1A ot SMS termination signal to GSM Module
    // SMS will be sent immediately
    SIM900A.println((char)26);
    delay(500);
    Serial.println("*** SENT STATUS VIA SMS ***");
}

/**
 * @brief Extracts pin state from SMS text. 110 will turn on first 2 pins, and turn off the 3rd one
 */
void parseSMS(String &text)
{
    for (int i = 0; i < text.length(); i++)
    {
        Serial.print(devs[i]);
        Serial.print(" = ");

        if (text[i] == '0')
        {
            Serial.println("OFF");
            state[i] = 0;
        }
        else if (text[i] == '1')
        {
            Serial.println("ON");
            state[i] = 1;
        }
        else
        {
            Serial.println("UNCHANGED");
        }
    }

    Serial.println("*** UPDATING STATUS ***");
    updateState();
}

void setup()
{
    Serial.begin(9600);
    Serial.println("setup()");

    SIM900A.begin(9600);
    delay(500);
    SIM900A.println("AT");
    delay(1000);
    
    // Check if AT command returns OK
    if (readString() == "AT\r\n\r\nOK\r\n")
    {
        Serial.println("*** INIT GSM_MODULE SUCCESS ***");
        
        // Set SMS mode to plain text
        SIM900A.println("AT+CMGF=1");
        delay(100);
        // Show SMS immediately on receive
        // 1 = Do not show
        // 2 = Show immediately
        // 3 = Put in buffer
        SIM900A.println("AT+CNMI=2,2,0,0,0");
        delay(100);
        
        // Set pin modes, all pins will be output to operate relays
        for (int i = 0; i < 3; i++)
        {
            pinMode(pins[i], OUTPUT);
        }
        
        // Turn on or off relays based on state
        updateState();
    }
    else
    {
        Serial.println("*** INIT GSM_MODULE FAILED ***");
    }
}

void loop()
{
    String buffer = readString();

    // Incoming sms starts with +CMT at the beginning
    if (buffer.startsWith("\r\n+CMT: "))
    {
        Serial.println("*** RECEIVED SMS ***");
        
        // Remove first 52 characters, they contain sender number, date, time, delivery info etc.
        // +CMT: "+8801674770321","","16/05/24,07:40:32+24"
        // length of this line is 48
        // \r\n at start and end makes it 2+48+2 = 52
        buffer.remove(0, 52);
        int len = buffer.length();
        // Remove \r\n from SMS tail
        buffer.remove(len - 2, 2);
        // Get and GET or get will be same
        buffer.toLowerCase();

        // If sms is "get", send status report
        if (buffer.substring(0, 3) == "get")
        {
            getState();
        }
        // Else parse the sms and turn on/off relays
        else
        {
            parseSMS(buffer);
        }
    }

    delay(100);
}


