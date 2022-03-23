#include <LiquidCrystal_I2C.h> // Library for LCD

const int coinPin = 2;
volatile int totalAmount = 0;
boolean coinIn = false;
boolean firstCoin = true;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    pinMode(coinPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(coinPin), updateCredit, FALLING);

    lcd.init();
    lcd.backlight();
    lcd.print("Bienvenidos!");
    lcd.setCursor(0, 1);
    lcd.print("Inserte monedas");
}

void loop() {

    if (coinIn) {
        coinIn = false;

        if (firstCoin) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Su credito:");
            firstCoin = false;
        }

        if (totalAmount == 1) {
            lcd.setCursor(0, 1);
            lcd.print("$1 peso");
        } else if (totalAmount > 1) {
            lcd.setCursor(0, 1);
            lcd.print("$");
            lcd.print(totalAmount);
            lcd.print(" pesos");
        }
    }

}

void updateCredit() {
    totalAmount++;
    coinIn = true;
}

