#include <Servo.h>             // Include the Servo library
#include <Wire.h>              // Include the Wire library for I2C communication
#include <LiquidCrystal_I2C.h> // Include the LiquidCrystal_I2C library

// Define pins
const int trigPin = 9;       // Ultrasonic sensor trigger pin
const int echoPin = 8;       // Ultrasonic sensor echo pin
const int servoPin = 7;      // Servo motor control pin
const int redPin = 13;       // RGB LED red channel pin (digital pin 13)

// Distance threshold
const long distanceThreshold = 14; // Distance threshold for servo activation in cm

// Create a Servo object
Servo servo;

// Create an LCD object with the I2C address 0x27
// and dimensions based on your LCD display (16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize count variable
int count = 0;

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(9600);
    
    // Initialize the LCD
    lcd.init();
    lcd.backlight();  // Turn on the backlight
    
    // Set pin modes
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(redPin, OUTPUT);  // Set the red channel pin as an output
    
    // Attach the servo to its pin
    servo.attach(servoPin);
    servo.write(0);  // Move servo to closed position (0 degrees)
    
    // Print debug message
    Serial.println("Servo attached and initialized to closed position.");
    
    // Display initial message on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready");
}

long measureDistance() {
    // Trigger the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Measure the echo duration
    long duration = pulseIn(echoPin, HIGH);
    
    // Calculate the distance in centimeters
    long distance = (duration / 2.0) / 29.1;
    
    return distance;
}

void loop() {
    // Measure the distance
    long distance = measureDistance();
    
    // Debugging output
    Serial.print("Distance: ");
    Serial.println(distance);
    
    // Check if the distance is within the threshold
    if (distance <= distanceThreshold) {
        // Increment count and update LCD display
        count++;
        
        // Move the servo to the open position (180 degrees)
        Serial.println("Moving servo to open position (180 degrees).");
        servo.write(180);
        
        // Display "Welcome" on the LCD with the count
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SmartBin Welcome");
        lcd.setCursor(0, 1);
        lcd.print("Count: ");
        lcd.print(count);
        
        // Turn on the red light of the RGB LED
        digitalWrite(redPin, HIGH);
        
    } else {
        // Move the servo to the closed position (0 degrees)
        Serial.println("Moving servo to closed position (0 degrees).");
        servo.write(0);
        
        // Display "Thank You" on the LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SmartBin");
        lcd.setCursor(0, 1);
        lcd.print("Thank You");
        
        // Turn off the red light of the RGB LED
        digitalWrite(redPin, LOW);
    }
    
    // Add a delay to allow the servo time to move
    delay(1000);
}
