//
include <Adafruit_NeoPixel.h>

#define PIN 9 // Pin connected to the Neopixel data input
#define NUM_LEDS 60 // Number of Neopixels in the strip

// Initialize the Neopixel library
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

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
    Serial.print(hours);
    Serial.print(':');
    Serial.print(minutes);
    Serial.print(':');
    Serial.println(seconds);
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
    if (hours == alarmHour && minutes == alarmMinute) {
        alarmTransition();
    }
}

void controlLed() {
    if (ledState == 0) {
        ledFlag = 0;
        for (int j = 0; j < 40; j++) {
            strip.setPixelColor(j, strip.Color(0, 0, 0));
        }
        strip.show();
    }
}

void alarmTransition() {
    for (int i = 0; i < 255; i++) {
        for (int j = 0; j < 40; j++) {
            strip.setPixelColor(j, strip.Color(i, i, i));
        }
        strip.show();
        delay(200);
    }
    for (int j = 0; j < 40; j++) {
        strip.setPixelColor(j, strip.Color(0, 0, 0));
    }
    strip.show();
}

int displaySetHour() {
    int hourSet = 0;
    while (digitalRead(buttonPin) == LOW) {
        int value = analogRead(A0);
        hourSet = map(value, 0, 1023, 0, 23);
        Serial.print("Set Hour: ");
        Serial.println(hourSet);
    }
    return hourSet;
}

int displaySetMin() {
    int minSet = 0;
    while (digitalRead(buttonPin) == HIGH) {
        int value = analogRead(A0);
        minSet = map(value, 0, 1023, 0, 59);
        Serial.print("Set Minute: ");
        Serial.println(minSet);
    }
    return minSet;
}