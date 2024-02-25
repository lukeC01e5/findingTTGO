#include <Arduino.h>
#include <TFT_eSPI.h>       // Include the TFT library
#include <SPI.h>            // Include the SPI library
#include <HardwareSerial.h> // Include the HardwareSerial library
#include <string>
#include <iostream>
#include <sstream>
#include "Trex.h"

TFT_eSPI tft; // Create an instance of the TFT class

HardwareSerial mySerial(1); // Use the second hardware serial port

// volatile bool buttonPressed = false;
volatile int buttonValue = -1; // Global variable to hold the button value

void whatAnimal();

void yesButtonPressed()
{
    buttonValue = 1;
}

void noButtonPressed()
{
    buttonValue = 0;
}

const int yesButtonPin = 35; // Button1 on the TTGO T-Display
const int noButtonPin = 0;   // Button2 on the TTGO T-Display
/*
void displayErrorMessage(int argNumber)
{
    tft.fillScreen(TFT_BLACK); // Clear the screen

    tft.setTextSize(2);
    tft.setTextColor(TFT_RED);
    tft.setCursor(0, 0);
    tft.println("Error: Argument " + String(argNumber));

    delay(2000); // Wait for 3 seconds
}
*/

void displayErrorMessage(const std::string &message)
{
    tft.fillScreen(TFT_BLACK); // Clear the screen

    tft.setTextSize(3);
    tft.setTextColor(TFT_RED);
    tft.setCursor(0, 0);
    tft.println(("Error:\n" + message).c_str());

    delay(1000); // Wait for 3 seconds
    return;
}

void displayTrex()
{
    tft.fillScreen(TFT_BLACK); // Clear the screen

    int16_t x = tft.width() / 2;                     // Calculate the x coordinate of the center of the screen
    int16_t y = tft.height() / 2;                    // Calculate the y coordinate of the center of the screen
    tft.pushImage(x - 30, y - 50, 128, 128, Trex); // Draw the T-Rex image at the center of the screen

    delay(3000); // Wait for 3 seconds
    return;
}

void scan4challange()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    // tft.setCursor(0, tft.getCursorY() + 10); // Move the cursor down
    tft.setCursor(0, 0);
    tft.println("\nScan\nchallange\ncard.....");
}

void displayX()
{
    tft.fillScreen(TFT_BLACK); // Clear the screen

    tft.setTextDatum(MC_DATUM); // Set the datum to be the center of the screen
    tft.setTextSize(9);         // Set the text size to 5
    tft.setTextColor(TFT_RED);  // Set the text color to red

    int16_t x = tft.width() / 2;  // Calculate the x coordinate of the center of the screen
    int16_t y = tft.height() / 2; // Calculate the y coordinate of the center of the screen

    tft.drawString("X", x, y); // Draw the 'X' at the center of the screen

    delay(1000); // Wait for 3 seconds

    tft.fillScreen(TFT_BLACK); // Clear the screen
}

void displayCircle()
{
    tft.fillScreen(TFT_BLACK); // Clear the screen

    int16_t x = tft.width() / 2;  // Calculate the x coordinate of the center of the screen
    int16_t y = tft.height() / 2; // Calculate the y coordinate of the center of the screen
    int16_t radius = 50 / 2;      // Calculate the radius of the circle

    tft.fillCircle(x, y, radius, TFT_GREEN); // Draw the green circle

    delay(1000); // Wait for 3 seconds

    tft.fillScreen(TFT_BLACK); // Clear the screen
    // tft.drawSmoothArc(x, y, radius, 0, 180, 200, TFT_GREEN);
}

void animateEyes() {
    int16_t x = tft.width() / 2;  // Calculate the x coordinate of the center of the screen
    int16_t y = tft.height() / 2; // Calculate the y coordinate of the center of the screen
    int16_t radius = 50 / 2;      // Calculate the radius of the circle
    int16_t pupilRadius = radius / 2; // Calculate the radius of the pupil

    // Draw the white circles
    tft.fillCircle(x - radius - 10, y, radius, TFT_WHITE);
    tft.fillCircle(x + radius + 10, y, radius, TFT_WHITE);

    for (int i = -radius; i <= radius; i += 5) {
        // Clear the previous pupils
        tft.fillCircle(x - radius - 10 + i - 5, y, pupilRadius, TFT_WHITE);
        tft.fillCircle(x + radius + 10 + i - 5, y, pupilRadius, TFT_WHITE);

        // Draw the new pupils
        tft.fillCircle(x - radius - 10 + i, y, pupilRadius, TFT_BLACK);
        tft.fillCircle(x + radius + 10 + i, y, pupilRadius, TFT_BLACK);

        delay(100); // Wait for a short time
    }
}
/*
void waiting()
{
    tft.fillScreen(TFT_BLACK); // Clear the screen

    int16_t x = tft.width() / 2;  // Calculate the x coordinate of the center of the screen
    int16_t y = tft.height() / 2; // Calculate the y coordinate of the center of the screen
    int16_t radius = 50 / 2;      // Calculate the radius of the circle

    // Fill the upper half of the circle
    tft.fillCircleHelper(x, y, radius, 1, 0, TFT_ORANGE);
    delay(500); // Wait for half a second

    // Fill the entire circle
    tft.fillCircle(x, y, radius, TFT_ORANGE);
    delay(300); // Wait for half a second

    // Clear the screen
    tft.fillScreen(TFT_BLACK);
    delay(300); // Wait for half a second

    // Fill the left half of the circle
    tft.fillCircleHelper(x, y, radius, 2, 0, TFT_ORANGE);
    delay(300); // Wait for half a second

    // Fill the entire circle
    tft.fillCircle(x, y, radius, TFT_ORANGE);
    delay(300); // Wait for half a second

    // Clear the screen
    tft.fillScreen(TFT_BLACK);
    // delay(2000); // Wait for 2 seconds
}
*/

std::pair<std::string, int> extractWordAndNumberString(const std::string &str)
{
    std::string startDelimiter = "11--";
    std::string endDelimiter = "--11";

    // Find the start and end positions of the word
    size_t startPos = str.find(startDelimiter);
    size_t endPos = str.find(endDelimiter);

    // If the start or end delimiter was not found, return an empty string and 0
    if (startPos == std::string::npos || endPos == std::string::npos)
    {
        return {"", 0};
    }

    // Adjust the start position to be after the start delimiter
    startPos += startDelimiter.length();

    // Calculate the length of the word
    size_t length = endPos - startPos;

    // Extract the word
    std::string wordAndNumber = str.substr(startPos, length);

    // Split the word and number at the full stop
    size_t dotPos = wordAndNumber.find('.');
    if (dotPos == std::string::npos)
    {
        return {"", 0}; // Return an empty string and 0 if the full stop was not found
    }

    std::string word = wordAndNumber.substr(0, dotPos);
    int number = std::stoi(wordAndNumber.substr(dotPos + 1));

    return {word, number};
}

void buttonReadText()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);
    tft.println("      yes--->\n\n\n      no--->");
}
/*
void setupTFT(const char *message)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(0, 0);
    tft.println(message);
}
*/
int buttonConfirm()
{
    attachInterrupt(digitalPinToInterrupt(yesButtonPin), yesButtonPressed, FALLING);
    attachInterrupt(digitalPinToInterrupt(noButtonPin), noButtonPressed, FALLING);

    // Wait for a button press
    while (buttonValue == -1)
    {
        delay(100); // Optional delay to prevent the loop from running too fast
    }

    int returnValue = buttonValue; // Save the button value

    // Reset the button value
    buttonValue = -1;

    detachInterrupt(digitalPinToInterrupt(yesButtonPin));
    detachInterrupt(digitalPinToInterrupt(noButtonPin));

    return returnValue; // Return the button value
}

void setup()
{
    Serial.begin(115200);
    mySerial.begin(9600, SERIAL_8N1, 27, 26); // Initialize serial communication on pins 27 (RX) and 26 (TX)
    tft.init();                               // Initialize the TFT display
    tft.setRotation(1);                       // Set the display rotation (if needed)
    tft.fillScreen(TFT_BLACK);                // Clear the display

    // Initialize the button pins as input
    pinMode(yesButtonPin, INPUT_PULLUP);
    pinMode(noButtonPin, INPUT_PULLUP);

    String word = "";
    scan4challange();
}

void loop()
{

    if (mySerial.available())
    {

        String barcode = mySerial.readString();
        if (!barcode.isEmpty())
        {
            // tft.fillScreen(TFT_BLACK);
            tft.setTextSize(3);
            tft.setTextColor(TFT_WHITE);

            std::string extractWord(const std::string &str);

            tft.setCursor(0, 0);

            std::string myText = barcode.c_str(); // Convert Arduino String to std::string

            std::pair<std::string, int> result = extractWordAndNumberString(myText);

            std::string word = result.first;
            int number = result.second;

            if (word == "challenge")
            {
                displayCircle();
                tft.setCursor(0, 0);
                tft.println("challenge \n card loaded \n" + number);
            }
            else if (word == "creatureCapture")
            {
                tft.fillScreen(TFT_BLACK);
                tft.setCursor(0, 0);
                tft.println("Seek Creature\nto be your.. ");
                delay(1000); // Wait for 2 seconds
                whatAnimal();
            }
            else
            {
                displayX();
                const std::string message = "Invalid\nword: " + word;
                displayErrorMessage(message);
                delay(1000); // Wait for 2 seconds
                scan4challange();
            }
        }
        else
        {
            Serial.println("Empty barcode");
            const std::string message = "Empty barcode: + barcode.c_str()";
            displayErrorMessage(message);
        }
    }
}

void whatAnimal()
{
    tft.println("champion");
    delay(1000); // Wait for 1 seconds

    String input = ""; // Declare the variable "input"

    while (!mySerial.available())
    {
        // wait for data to be available
        delay(100); // optional delay to prevent the loop from running too fast
        animateEyes();
    }

    // now read from Serial
    input = mySerial.readString();

    String word = "";

    tft.setCursor(0, 0);
    tft.fillScreen(TFT_BLACK);

    std::string myText = input.c_str(); // Convert Arduino String to std::string

    std::pair<std::string, int> result = extractWordAndNumberString(myText);

    std::string animal = result.first;
    int number = result.second;

    tft.println(("Animal:\n" + animal).c_str()); // Print the word to the serial monitor
    tft.println(("\nPower-Level:\n" + number));  // Print the number to the serial monitor
    delay(3000);                                 // Wait for 1 seconds
    tft.fillScreen(TFT_BLACK);

    if (animal == "babyTrex")
    {
        tft.setCursor(0, 0);
        tft.println("Keep T-rex\nas your\nchampion");
        delay(2000); // Wait for 2 seconds
        buttonReadText();
        buttonConfirm();

        if (buttonConfirm() == 1)
        {
            displayTrex();
        }
        else
        {
            tft.fillScreen(TFT_BLACK);
            tft.setCursor(0, 0);
            tft.println("pick another\ncreature");
            delay(1000); // Wait for 2 seconds
        }
    }

    else
    {
        Serial.println("Invalid input: " + input);
        // displayErrorMessage("Invalid input", input);
        //  Add a comment or a dummy statement here
        delay(1500); // Wait for 2 seconds
        return;
    }
}