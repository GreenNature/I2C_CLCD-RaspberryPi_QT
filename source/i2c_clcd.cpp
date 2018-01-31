#include "i2c_clcd.h"
#include <QDebug>

i2c_clcd::i2c_clcd(QObject *parent) : QObject(parent)
{
    wiringPiSetup ();
    gFileDescriptor = wiringPiI2CSetup(I2C_LCD_ADDR);

    this->init();
}

i2c_clcd::~i2c_clcd()
{
}

void i2c_clcd::init()
{
    // Initialise display
    writeByte(LCD_CMD_ENTRY | LCD_CMD_ENTRY_ID, LCD_MODE_CMD); // Cursor move direction
    writeByte(LCD_CMD_DISPLAY | LCD_CMD_DISPLAY_DISPLAY, LCD_MODE_CMD); // Blink Off
    writeByte(LCD_CMD_FUNCTION | LCD_CMD_FUNCTION_DL | LCD_CMD_FUNCTION_N, LCD_MODE_CMD); // Data length, number of lines, font size
    clear();
    delayMicroseconds(500);
}

void i2c_clcd::clear()
{
    writeByte(LCD_CMD_CLEAR, LCD_MODE_CMD);
    delayMicroseconds(500);
}

void i2c_clcd::home()
{
    writeByte(LCD_CMD_HOME, LCD_MODE_CMD);
    delayMicroseconds(500);
}

void i2c_clcd::setCursor(int col, int row)
{
    int row_offsets[2] = {0x00, 0x40};
    if(row >= 2)
        row = 0;

    writeByte(LCD_CMD_DDRAMADDR | (col + row_offsets[row]), LCD_MODE_CMD);
}

void i2c_clcd::print(QString d)
{
    print((const char*)(d.toLocal8Bit().data()));
}

void i2c_clcd::print(QByteArray d)
{
    print((const char*)(d.data()));
}

void i2c_clcd::print(const char* d)
{
    int i;
    int length = strlen(d);
    for(i=0; i<length; i++)
    {
        write((char)*d);
        d++;
    }
}

void i2c_clcd::write(char d)
{
    writeByte((int)d, LCD_MODE_DATA);
}

void i2c_clcd::writeByte(int data, int mode)
{
    int highBits = 0, lowBits = 0;

    highBits = mode | ((data << 0) & 0xF0) | LCD_BACKLIGHT;
    lowBits = mode | ((data << 4) & 0xF0) | LCD_BACKLIGHT;

    //High Bits
    wiringPiI2CReadReg8(gFileDescriptor, highBits);
    toggleEnable(highBits);

    //Low Bits
    wiringPiI2CReadReg8(gFileDescriptor, lowBits);
    toggleEnable(lowBits);
}

void i2c_clcd::toggleEnable(int bits)
{
    wiringPiI2CReadReg8(gFileDescriptor, (bits | LCD_ENABLE));
    delayMicroseconds(100);
    wiringPiI2CReadReg8(gFileDescriptor, (bits | ~LCD_ENABLE));
    delayMicroseconds(50);
}
