/*
Code for the advent calendar on Arduino.

Idea is to read current day of month (assuming it is December)
and light up leds up to this number. Current date comes from the RTC module.
*/

#include <Wire.h>

#define RTC_ADDRESS 0x68

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int numOfRegisters = 4;
byte* registerState;

struct datetime {
    int second;
    int minute;
    int hour;
    int weekDay;
    int monthDay;
    int month;
    int year;
};

void setup() {
    // Initialize array
    registerState = new byte[numOfRegisters];
    for (size_t i = 0; i < numOfRegisters; i++) {
        registerState[i] = 0;
    }

    // Set pins to output so you can control the shift register
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    Wire.begin();
    Serial.begin(9600);
}

void loop() {
    int timeout = 500;
    struct datetime rtc_datetime = readDate();
    int currentDayOfMonth = rtc_datetime.monthDay;
    oneByOne(currentDayOfMonth, timeout);
    clearAll();
}

void oneByOne(int stopPin, int timeout) {
    // Light up leds one by one until the stop pin
    // Long delay after the last pin
    for (int i = 0; i < stopPin; i++){
        regWrite(i, HIGH);
        delay(timeout);
    }


    delay(5 * timeout);
}

void clearAll() {
    // Clear all pins
    for (int i = 0; i <= 31; i++){
        regWrite(i, LOW);
    }
}

void regWrite(int pin, bool state){
    // Determines register
    int reg = (pin / 8);
    // Determines pin for actual register
    int actualPin = pin - (8 * reg);

    //Begin session
    digitalWrite(latchPin, LOW);

    for (int i = 0; i < numOfRegisters; i++){
        //Get actual states for register
        byte* states = &registerState[i];

        // Update state
        // For some reason without this coversion registers are
        // traversed in "backwards" order
        if (i == (numOfRegisters - 1) - reg){
            bitWrite(*states, actualPin, state);
        }

        //Write
        shiftOut(dataPin, clockPin, MSBFIRST, *states);
    }

    //End session
    digitalWrite(latchPin, HIGH);
}

byte bcdToDec(byte val) {
    // Convert binary coded decimal to normal decimal numbers
    return ( (val/16*10) + (val%16) );
}


datetime readDate() {
    // Read the date and time from RTC and return the datetime structure

    // Reset the register pointer
    Wire.beginTransmission(RTC_ADDRESS);

    byte zero = 0x00;
    Wire.write(zero);
    Wire.endTransmission();

    Wire.requestFrom(RTC_ADDRESS, 7);

    struct datetime currentDateTime;

    currentDateTime.second = bcdToDec(Wire.read());
    currentDateTime.minute = bcdToDec(Wire.read());
    currentDateTime.hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
    currentDateTime.weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
    currentDateTime.monthDay = bcdToDec(Wire.read());
    currentDateTime.month = bcdToDec(Wire.read());
    currentDateTime.year = bcdToDec(Wire.read());

    return currentDateTime;
}
