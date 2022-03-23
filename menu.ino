#include <LiquidCrystal_I2C.h>
#include <ezButton.h>
#include <EEPROM.h>

ezButton minusButton(53);
ezButton plusButton(51);
ezButton nextButton(49);
ezButton enterOrSaveButton(47);


// ======================================== menu global vars and data
boolean insideMenu = false;
int menuData[] = {5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6};
int currentProductS = 0;
int currentProductP = 0;
int currentScreen;
// ============================================================= ^^^

LiquidCrystal_I2C lcd(0x27, 16, 2);

void(* resetFunc) (void) = 0;

void setup() {
    minusButton.setDebounceTime(50);
    plusButton.setDebounceTime(50);
    nextButton.setDebounceTime(50);
    enterOrSaveButton.setDebounceTime(50);

    readData(12);

    lcd.init();
    lcd.backlight();
    lcd.print("Bienvenidos!");
    lcd.setCursor(0, 1);
    lcd.print("Inserte monedas");
}

void loop() {
    minusButton.loop();
    plusButton.loop();
    nextButton.loop();
    enterOrSaveButton.loop();
    
    // ======================================================= enter menu
    if (enterOrSaveButton.getCount() == 3 && !insideMenu) {
        insideMenu = true;
        enterOrSaveButton.resetCount();
        printMenuSeconds();
    }
    // ============================================================= ^^^

    // ===================================================== move in menu
    if (insideMenu && nextButton.isPressed() && currentScreen < 11) {
        currentScreen++;
        if (currentScreen < 6) {
            printMenuSeconds();
        } else if (currentScreen > 5) {
            printMenuPrices();
        }
    }
    // ============================================================= ^^^

    // ========================================== add or subtract in menu
    if (insideMenu && minusButton.isPressed() && menuData[currentScreen] > 0) {
        menuData[currentScreen]--;
        printMenuData();
    } else if (insideMenu && plusButton.isPressed() && menuData[currentScreen] < 51) {
        menuData[currentScreen]++;
        printMenuData();
    }
    // ============================================================= ^^^

    // ============================================= save and reset menu
    if (enterOrSaveButton.getCount() == 1 && insideMenu) {
        saveData(menuData, 12);
        resetFunc();
    }
    // ============================================================= ^^^
}

void printMenuSeconds() {
    currentProductS++;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Segundos P");
    lcd.print(currentProductS);
    lcd.setCursor(0, 1);
    lcd.print(menuData[currentScreen]);
    lcd.print(" segundos");
}

void printMenuPrices() {
    currentProductP++;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Precio P");
    lcd.print(currentProductP);
    lcd.setCursor(0, 1);
    lcd.print(menuData[currentScreen]);
    lcd.print(" pesos");

}

void printMenuData() {
    lcd.setCursor(0, 1);
    lcd.print(menuData[currentScreen]);
}

void saveData(int data[], int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        EEPROM.write(i, menuData[i]);
    }
}

void readData(int arraySize) {
    // change to 0 on new arduino
    if (EEPROM.read(2) != 0xff) {
        for (int i = 0; i < arraySize; i++) {
            menuData[i] = EEPROM.read(i);
        }
    }
}