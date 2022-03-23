// ======================================================= libraries

#include <LiquidCrystal_I2C.h> // Library for LCD
#include <ezButton.h> // Button library
#include <EEPROM.h> // Save and read data library

// ======================================================= constants and vars

// constants won't change
const int LED1Pin = 5;
const int relayPin = 2;

// vars will change
int buttonCount;

// create a ezbutton instance on pin 8
ezButton K1Pin(8);
ezButton K2Pin(9);
ezButton readPin(10);
ezButton savePin(11);

// liquid crystal instance
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ======================================================= set up

// put your setup code here, to run once:
void setup() {
  pinMode(LED1Pin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  // set debounce time to 50 milliseconds
  K1Pin.setDebounceTime(50);
  K2Pin.setDebounceTime(50);
  readPin.setDebounceTime(50);
  savePin.setDebounceTime(50);

  // init lcd
  lcd.init();
  lcd.backlight();  
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido!");
}

// ======================================================= loop

void loop() {
  // must set loop for buttons
  K1Pin.loop();
  K2Pin.loop();
  readPin.loop();
  savePin.loop();

  // if K1Pin pressed
  if (K1Pin.isPressed()) {
    digitalWrite(LED1Pin, HIGH);
    digitalWrite(relayPin, HIGH);
    lcd.clear();
    lcd.print("Running relay 1");
  } else if (K1Pin.isReleased()) {
    digitalWrite(LED1Pin, LOW);
    digitalWrite(relayPin, LOW);
    lcd.clear();
    lcd.print("Delay stopped");
  }

  // if K2Pin pressed
  if (K2Pin.isPressed()) {
    digitalWrite(LED1Pin, HIGH);
    buttonCount++;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Button count:");
    lcd.setCursor(0, 1);
    lcd.print(buttonCount);
  } else if (K2Pin.isReleased()) {
    digitalWrite(LED1Pin, LOW);
  }

  if (readPin.isPressed()) {
    buttonCount = EEPROM.read(0);

    digitalWrite(LED1Pin, HIGH);
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Button pressed:");
    lcd.setCursor(0, 1);
    lcd.print(buttonCount);
  } else if (readPin.isReleased()) {
    digitalWrite(LED1Pin, LOW);
  }

    if (savePin.isPressed()) {
    EEPROM.write(0, buttonCount);
    digitalWrite(LED1Pin, HIGH);
  } else if (savePin.isReleased()) {
    digitalWrite(LED1Pin, LOW);
  }
}