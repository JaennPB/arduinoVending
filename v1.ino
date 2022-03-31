// ======================================================= libraries
// =================================================================

#include <LiquidCrystal_I2C.h>
#include <ezButton.h>
#include <EEPROM.h>

// ======================================================= pins/data/gobal variables 
// =================================================================================

// ----------------------------- coin acceptor

const int coinPin = 19;
boolean firstCoin = true;
boolean coinIn = false;
volatile int totalAmount = 0;

// ----------------------------- menu module

ezButton minusButton(53);
ezButton plusButton(51);
ezButton nextButton(49);
ezButton enterOrSaveButton(47);

int menuData[12] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
int currentProductS = 0;
int currentProductP = 0;
int currentScreen;
boolean insideMenu = false;

// ----------------------------- pumps

//TODO: change on a new arduino
ezButton button1(39);
ezButton button2(37);
ezButton button3(35);
ezButton button4(33);
ezButton button5(31);
ezButton button6(29);

// ----------------------------- L298N driver

const int in1Pin = 22;
const int in2Pin = 24;

const int in3Pin = 26;
const int in4Pin = 28;

const int in5Pin = 30;
const int in6Pin = 32;

const int in7Pin = 34;
const int in8Pin = 36;

const int in9Pin = 38;
const int in10Pin = 40;

const int in11Pin = 42;
const int in12Pin = 44;

// ----------------------------- lcd display

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----------------------------- lcd display

int timesRun = 0;

// ======================================================= reset helper
// ====================================================================

void(* resetFunc) (void) = 0;

// ======================================================= set up
// ==============================================================

void setup() {
    // ----------------------------- read data on startup

    readData(12);

    // ----------------------------- menu module

    minusButton.setDebounceTime(50);
    plusButton.setDebounceTime(50);
    nextButton.setDebounceTime(50);
    enterOrSaveButton.setDebounceTime(50);

    // ----------------------------- L298N driver

    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);

    pinMode(in3Pin, OUTPUT);
    pinMode(in4Pin, OUTPUT);

    pinMode(in5Pin, OUTPUT);
    pinMode(in6Pin, OUTPUT);

    pinMode(in7Pin, OUTPUT);
    pinMode(in8Pin, OUTPUT);

    pinMode(in9Pin, OUTPUT);
    pinMode(in10Pin, OUTPUT);

    pinMode(in11Pin, OUTPUT);
    pinMode(in12Pin, OUTPUT);
   
    // ----------------------------- init display

    lcd.init();
    lcd.backlight();
    lcd.print("Maquinas vending");
    lcd.setCursor(0, 1);
    lcd.print("ROME");

    delay(3000);

    lcd.clear();
    lcd.print("Bienvenido!");
    lcd.setCursor(0, 1);
    lcd.print("Inserte monedas.");

    // ----------------------------- coin setup

    pinMode(coinPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(coinPin), updateCredit, FALLING);
    totalAmount = 0;
    coinIn = false;
}

// ======================================================= loop
// ============================================================

void loop() {
    // ----------------------------- menu buttons
    // ------------------------------------------

    minusButton.loop();
    plusButton.loop();
    nextButton.loop();
    enterOrSaveButton.loop();

    // ----------------------------- pump buttons
    // ------------------------------------------

    button1.loop();
    button2.loop();
    button3.loop();
    button4.loop();
    button5.loop();
    button6.loop();

    // ----------------------------- coin loop
    // ---------------------------------------

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

    // ----------------------------- menu loop
    // ---------------------------------------

    // enter menu
    if (enterOrSaveButton.getCount() == 3 && !insideMenu) {
        detachInterrupt(digitalPinToInterrupt(coinPin));

        insideMenu = true;
        enterOrSaveButton.resetCount();
        printSecondsScreen();
    }

    // move in menu
    if (insideMenu && nextButton.isPressed() && currentScreen < 11) {
        currentScreen++;
        if (currentScreen < 6) {
            printSecondsScreen();
        } else if (currentScreen > 5) {
            printPricesScreen();
        }
    }

    // add or subtract data
    if (insideMenu && minusButton.isPressed() && menuData[currentScreen] > 1) {
        menuData[currentScreen]--;
        if (currentScreen < 6) {
            printSecondsData();
        } else if (currentScreen > 5) {
            printPricesData();
        }
        
    } else if (insideMenu && plusButton.isPressed() && menuData[currentScreen] < 99) {
        menuData[currentScreen]++;
        if (currentScreen < 6) {
            printSecondsData();
        } else if (currentScreen > 5) {
            printPricesData();
        }
    }

    // save and reset
    if (enterOrSaveButton.getCount() == 1 && insideMenu) {
        saveData(menuData, 12); 
    }

    // ----------------------------- buttons loop
    // ------------------------------------------

    if (button1.isPressed() && totalAmount >= menuData[6]) {
        runPump(in1Pin, in2Pin, menuData[0], menuData[6]);
        return;
    }

    if (button2.isPressed() && totalAmount >= menuData[7]) {
        runPump(in3Pin, in4Pin, menuData[1], menuData[7]);
        return;
    }

    if (button3.isPressed() && totalAmount >= menuData[8]) {
        runPump(in5Pin, in6Pin, menuData[2], menuData[8]);
        return;
    }

    if (button4.isPressed() && totalAmount >= menuData[10]) {
        runPump(in9Pin, in10Pin, menuData[4], menuData[10]);
        return;
    }

    if (button5.isPressed() && totalAmount >= menuData[9]) {
        runPump(in7Pin, in8Pin, menuData[3], menuData[9]);
        return;
    }

    if (button6.isPressed() && totalAmount >= menuData[11]) {
        runPump(in11Pin, in12Pin, menuData[5], menuData[11]);
        return;
    }
}

// ======================================================= coin interrupt
// ======================================================================

void updateCredit() {
    totalAmount++;
    coinIn = true;
}

// ======================================================= menu functions
// ======================================================================

void printSecondsScreen() {
    currentProductS++;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Segundos P");
    lcd.print(currentProductS);
    lcd.setCursor(0, 1);
    lcd.print(menuData[currentScreen]);
    lcd.print(" segundos");
}

void printPricesScreen() {
    currentProductP++;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Precio P");
    lcd.print(currentProductP);
    lcd.setCursor(0, 1);
    lcd.print(menuData[currentScreen]);
    lcd.print(" pesos");

}

void printPricesData() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Precio P");
    lcd.print(currentProductP);
    lcd.setCursor(0, 1);
    lcd.print(menuData[currentScreen]);
    lcd.print(" pesos");

}

void printSecondsData() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Segundos P");
    lcd.print(currentProductS);
    lcd.setCursor(0, 1);
    lcd.print(menuData[currentScreen]);
    lcd.print(" segundos");
}

// ======================================================= save and read functions
// ===============================================================================

void saveData(int data[], int arraySize) {
    lcd.clear();
    lcd.print("Guardando");
    lcd.setCursor(0, 1);
    lcd.print("configuracion...");

    for (int i = 0; i < arraySize; i++) {
        EEPROM.write(i, menuData[i]);
    }

    delay(3000);

    resetFunc();
}

void readData(int arraySize) {
    if (EEPROM.read(0) != 0xff) {
        for (int i = 0; i < arraySize; i++) {
            menuData[i] = EEPROM.read(i);
        }
    }
}

// ======================================================= run pump function
// =========================================================================

void runPump(int pin1, int pin2, int time, int price) {
    detachInterrupt(digitalPinToInterrupt(coinPin));

    totalAmount = totalAmount - price;
    timesRun++;

    const int pumpTimeInMillis = time * 1000;

    lcd.clear();
    lcd.print("Coloque su");
    lcd.setCursor(0, 1);
    lcd.print("envase...");

    delay(3000);

    lcd.clear();
    lcd.print("Cargando");
    lcd.setCursor(0, 1);
    lcd.print("producto...");

    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);

    delay(pumpTimeInMillis);

    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);

    lcd.clear();
    lcd.print("Gracias");
    lcd.setCursor(0, 1);
    lcd.print("por su compra!");

    delay(3000);

    // reset after pumps run 3 times
    if (timesRun > 2 && totalAmount == 0) {
        lcd.clear();
        lcd.print("Reiniciando");
        lcd.setCursor(0, 1);
        lcd.print("espere...");

        delay(3000);

        resetFunc();
    }

    // coin reset
    if (totalAmount == 0) {
        lcd.clear();
        lcd.print("Bienvenido!");
        lcd.setCursor(0, 1);
        lcd.print("Inserte monedas.");
    } else if (totalAmount > 0) {
        coinIn = true;
    }

    attachInterrupt(digitalPinToInterrupt(coinPin), updateCredit, FALLING);
    firstCoin = true;

    return;
}
