// ======================================================= libraries

#include <ezButton.h> // Button library

// ======================================================= constants and vars

// constants won't change
const int in1Pin = 37;
const int in2Pin = 35;

// create a ezbutton instance on pin 52
ezButton buttonPin(52);

void setup() {
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    buttonPin.setDebounceTime(50);
}

void loop() {
    buttonPin.loop();

    if (buttonPin.isPressed()) {
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);
    } else if (buttonPin.isReleased()) {
        digitalWrite(in1Pin, LOW); 
        digitalWrite(in2Pin, LOW);
    }
}



