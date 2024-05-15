#include <Adafruit_NeoPixel.h>
#include<Wire.h>
#include<Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PIN 9 // Pin connected to the Neopixel
data input
#define NUM_LEDS 60 // Number of Neopixels in the
strip
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1 // Reset pin # (or -1 if
sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH,
SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize the Neopixel library
Adafruit_NeoPixel strip =
Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB +
NEO_KHZ800);

const int buttonPin = 7;
int buttonState = 0;
int ledState = 0;
int ledFlag = 0;
int hours = 0;

int minutes = 0;
int seconds = 0;
int alarmHour = 25;
int alarmMinute = 62;

void setup() {
pinMode(buttonPin, INPUT_PULLUP);
strip.begin(); // Initialize Neopixel strip
strip.show();
Serial.begin(9600);
displayTime();

Wire.begin();

if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
{ // Address 0x3C for 128x64
Serial.println(F("SSD1306 allocation failed"));
for(;;);}

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 10);
}

void loop() {
buttonState = digitalRead(buttonPin);

unsigned long currentMillis = millis();

seconds = currentMillis / 1000;

// Increment time
updateAndDisplayTime();

handleButtonPress();
checkAlarm();
controlLed();

}

void updateAndDisplayTime() {
// Increment time and handle overflow
if (seconds >= 60) {
seconds = 0;
minutes++;
if (minutes >= 60) {
minutes = 0;
hours++;
if (hours >= 24) {
hours = 0;
}
}
}
displayTime();
}

void displayTime() {
display.clearDisplay(); // Clear the display

before printing new content
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 10); // Set cursor
position
display.print(hours);
display.print(':');
display.print(minutes);
display.print(':');
display.println(seconds);
display.display();
}

void handleButtonPress() {
if (buttonState == LOW) {
// Button is pressed
if (ledFlag == 1) {
ledState = 0;
}
alarmHour = displaySetHour();
delay(500);
if (buttonState == LOW) {
alarmMinute = displaySetMin();
delay(2000);
}
}
}

void checkAlarm() {

if (hours == alarmHour && minutes ==
alarmMinute) {
alarmTransition();
}
}

void controlLed() {
if (ledState == 0) {
ledFlag = 0;
for (int j = 0; j < 40; j++) {
strip.setPixelColor(j, strip.Color(0,
0, 0));
}
strip.show();
}
}

void alarmTransition() {
for (int i = 0; i < 255; i++) {
for (int j = 0; j < 40; j++) {
strip.setPixelColor(j, strip.Color(i,
i, i));
}
strip.show();
delay(200);
}
for (int j = 0; j < 40; j++) {
strip.setPixelColor(j, strip.Color(0, 0,
0));

}
strip.show();
}

int displaySetHour() {
int hourSet = 0;
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 10);
display.print("Set Hour: ");
display.display(); // Refresh the display
delay(100); // Add a small delay for stability
while (digitalRead(buttonPin) == HIGH) {
int value = analogRead(A0);
hourSet = map(value, 0, 1023, 0, 23);
display.setCursor(70, 10); // Move cursor
to update the value
display.print(hourSet);
display.display(); // Refresh the display
delay(100); // Add a small delay for
stability
}
return hourSet;
}

int displaySetMin() {
int minSet = 0;
display.clearDisplay();

display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 10);
display.print("Set Minute: ");
display.display(); // Refresh the display
delay(100); // Add a small delay for stability
while (digitalRead(buttonPin) == HIGH) {
int value = analogRead(A0);
minSet = map(value, 0, 1023, 0, 59);
display.setCursor(70, 10); // Move cursor
to update the value
display.print(minSet);
display.display(); // Refresh the display
delay(100); // Add a small delay for
stability
}
return minSet;
}