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

public:
    LCD(uint8_t address = 0x27, uint8_t cols = 16, uint8_t rows = 2)
        : lcd(address, cols, rows) {}

    void begin()
    {
        lcd.init();
        lcd.backlight();
    }

    void print(const char *topMessage, const char *bottomMessage)
    {
        char topPadded[17];    // 16 chars + null terminator
        char bottomPadded[17]; // 16 chars + null terminator

        snprintf(topPadded, 17, "%-16s", topMessage);       // Left-align and pad with spaces
        snprintf(bottomPadded, 17, "%-16s", bottomMessage); // Left-align and pad with spaces

        lcd.setCursor(0, 0);
        lcd.print(topPadded);
        lcd.setCursor(0, 1);
        lcd.print(bottomPadded);
    }

    void print(const char *message)
    {
        print(message, "");
    }
};

#endif // LCD_H