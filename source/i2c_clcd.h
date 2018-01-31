/*
 * This is a library for using i2c lcd in raspberry Pi Qt
 *
 * It was tested on Raspberry Pi3, PCF8574T Chip and 16x2 CLCD
 *
 * PCF8574T Chip uses I2C to communicate, 2 pins are required to interface
 *
 * This source code is made by GreenNature (nobu981015@gmail.com)
 *
 * BSD license, all text above must be included in any redistribution
 */
#ifndef I2C_CLCD_H
#define I2C_CLCD_H

#include <QObject>
#include <stdio.h>
#include "wiringPi.h"
#include "wiringPiI2C.h"

#ifndef I2C_LCD_ADDR
#define I2C_LCD_ADDR    0x3f
#endif

#define LCD_MODE_DATA   0x01
#define LCD_MODE_CMD    0x00

#define LCD_LINE1   0x80
#define LCD_LINE2   0xC0

#define LCD_BACKLIGHT       0x08

/* ############ LCD Instruction(Command) */
#define LCD_CMD_CLEAR       0x01 //clear LCD
#define LCD_CMD_HOME        0x02 //return home

#define LCD_CMD_ENTRY       0x04 //Entry mode set
#define LCD_CMD_ENTRY_ID    0x02
#define LCD_CMD_ENRTY_SH    0x01
/*
 * LCD_CMD_ENTRY description
 * 0 0 0 0 0 1 I S
 *
 * Assign cursor moving direction and blinking of entire display
 *
 */

#define LCD_CMD_DISPLAY     0x08 // Display On/Off Control
#define LCD_CMD_DISPLAY_DISPLAY 0x04
#define LCD_CMD_DISPLAY_CURSOR  0x02
#define LCD_CMD_DISPLAY_BLINK   0x01
/*
 * LCD_CMD_DISPLAY description
 * 0 0 0 0 1 D C B
 *
 * Set display(D)
 * Cursor(C)
 * Blinking of cursor(B)
 * On/Off Control Bit
 */

#define LCD_CMD_FUNCTION    0x20
#define LCD_CMD_FUNCTION_DL 0x10
#define LCD_CMD_FUNCTION_N  0x80
#define LCD_CMD_FUNCTION_F  0x40
/*
 * Set interface data length(DL:8-Bit/4-Bit),
 * Numbers of display line (N:2-line/1line) and
 * Display font type (F:5x11?/5x8)
 */

#define LCD_CMD_CGRAMADDR   0x40
#define LCD_CMD_DDRAMADDR   0x80
/* ##################################### */

#define LCD_ENABLE  0x04

class i2c_clcd : public QObject
{
    Q_OBJECT
public:
    explicit i2c_clcd(QObject *parent = 0);
    ~i2c_clcd();

    void init();

    void clear();
    void home();

    void setCursor(int col, int row);

    void print(QString d);
    void print(QByteArray d);
    void print(const char *d);
    void write(char d);

private:
    void writeByte(int data, int mode);
    void toggleEnable(int bits);

    int gFileDescriptor;

signals:

public slots:
};

#endif // I2C_CLCD_H
