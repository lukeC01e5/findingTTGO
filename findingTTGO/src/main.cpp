#include <Arduino.h>
#include <TFT_eSPI.h>       // Include the TFT library
#include <SPI.h>            // Include the SPI library
#include <HardwareSerial.h> // Include the HardwareSerial library
#include <string>
#include <iostream>

TFT_eSPI tft; // Create an instance of the TFT class

HardwareSerial mySerial(1); // Use the second hardware serial port

String qr = "";

void scan4challange();
void displayX();
void displayCircle();

void setup()
{
    Serial.begin(115200);
    mySerial.begin(9600, SERIAL_8N1, 27, 26); // Initialize serial communication on pins 27 (RX) and 26 (TX)
    tft.init();                               // Initialize the TFT display
    tft.setRotation(1);                       // Set the display rotation (if needed)
    tft.fillScreen(TFT_BLACK);                // Clear the display

    scan4challange();
    String word = "";
}

void loop()
{
    if (mySerial.available())
    {
        String barcode = mySerial.readString();
        if (!barcode.isEmpty())
        {
            tft.fillScreen(TFT_BLACK);
            tft.setTextSize(2);
            tft.setTextColor(TFT_WHITE);

            std::string extractWord(const std::string &str);

            tft.setCursor(0, 0);

            tft.setCursor(0, tft.getCursorY() + 10); // Move the cursor down

            std::string str = barcode.c_str();
            std::string word = extractWord(str);
            if (word == "challange")
            {
                displayCircle();
            }
            else
            {
                displayX();
            }
        }
        else
        {
            Serial.println("Empty barcode");
        }
    }
}

void scan4challange()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    // tft.setCursor(0, tft.getCursorY() + 10); // Move the cursor down
    tft.setCursor(0, 0);
    tft.println("scanning for \n challange card.....");
}

void displayX()
{
    tft.fillScreen(TFT_BLACK); // Clear the screen

    tft.setTextDatum(MC_DATUM); // Set the datum to be the center of the screen
    tft.setTextSize(8);         // Set the text size to 5
    tft.setTextColor(TFT_RED);  // Set the text color to red

    int16_t x = tft.width() / 2;  // Calculate the x coordinate of the center of the screen
    int16_t y = tft.height() / 2; // Calculate the y coordinate of the center of the screen

    tft.drawString("X", x, y); // Draw the 'X' at the center of the screen

    delay(3000); // Wait for 3 seconds
}

std::string extractWord(const std::string &str)
{
    std::string startDelimiter = "(((";
    std::string endDelimiter = ")))";

    // Find the start and end positions of the word
    size_t startPos = str.find(startDelimiter);
    size_t endPos = str.find(endDelimiter);

    // If the start or end delimiter was not found, return an empty string
    if (startPos == std::string::npos || endPos == std::string::npos)
    {
        return "";
    }

    // Adjust the start position to be after the start delimiter
    startPos += startDelimiter.length();

    // Calculate the length of the word
    size_t length = endPos - startPos;

    // Extract and return the word
    return str.substr(startPos, length);
}

void displayCircle()
{
    tft.fillScreen(TFT_BLACK); // Clear the screen

    int16_t x = tft.width() / 2;  // Calculate the x coordinate of the center of the screen
    int16_t y = tft.height() / 2; // Calculate the y coordinate of the center of the screen
    int16_t radius = 50 / 2;      // Calculate the radius of the circle

    tft.fillCircle(x, y, radius, TFT_GREEN); // Draw the green circle

    delay(3000); // Wait for 3 seconds

    tft.fillScreen(TFT_BLACK); // Clear the screen
}
