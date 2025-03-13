/**********************************************************************
  Description : The class handling LCD print
  Author      : Vitaly Sharovatov
**********************************************************************/
#ifndef LCD_H
#define LCD_H
#include <LiquidCrystal_I2C.h>

class LCD
{
private:
    LiquidCrystal_I2C lcd;
    byte speakerIcon[8] = {
        0b00001,
        0b00011,
        0b00111,
        0b01111,
        0b01111,
        0b00111,
        0b00011,
        0b00001};

public:
    LCD(uint8_t address = 0x27, uint8_t cols = 16, uint8_t rows = 2)
        : lcd(address, cols, rows) {}

    void begin()
    {
        lcd.init();
        lcd.backlight();

        // store the icon in memory
        lcd.createChar(0, speakerIcon);
    }

    void print(const char *topMessage, const char *bottomMessage)
    {
        lcd.setCursor(0, 0);
        lcd.print(padMessage(topMessage));
        lcd.setCursor(0, 1);
        lcd.print(padMessage(bottomMessage));
    }

    void print(const char *message)
    {
        print(message, "");
    }

    void printWithIcon(const char *message)
    {
        lcd.setCursor(0, 0);
        lcd.write(byte(0)); // the speaker icon
        lcd.print(" ");
        lcd.print(padMessage(message));
    }

private:
    const char *padMessage(const char *message)
    {
        static char padded[17]; // 16 chars + null terminator
        snprintf(padded, 17, "%-16s", message);
        return padded;
    }
};

#endif // LCD_H