 #include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int flamePin = 6;      // Flame sensor analog pin
const int buzzer = 7;
const int greenLED = 8;
const int yellowLED = 9;
const int redLED = 10;
const int trigPin = 4;       // Ultrasonic sensor trig pin
const int echoPin = 3;       // Ultrasonic sensor echo pin

int Flame = HIGH;
int distanceThreshold = 20;  // Distance threshold for proximity
int nearDistance = 10;       // Define a nearer distance threshold

bool displayFireMessage = true;  // Flag to switch between messages

void setup() {
  // Pin configurations
  pinMode(flamePin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initial state for outputs
  digitalWrite(buzzer, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);

  // LCD initialization
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);

  Serial.begin(9600);
}

void slowBeep() {
  digitalWrite(buzzer, HIGH);
  delay(500);  // Beep for 0.5 seconds
  digitalWrite(buzzer, LOW);
  delay(500);  // Silence for 0.5 seconds
}

void loop() {
  Flame = digitalRead(flamePin);

  lcd.clear();

  if (Flame == LOW) {
    // Flame detected
    digitalWrite(buzzer, HIGH);
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Flame Detected");
    delay(1000);  // Beep for 1 second
  } else {
    // No flame detected
    digitalWrite(greenLED, HIGH);
    
    if (displayFireMessage) {
      lcd.setCursor(0, 0);
      lcd.print("No Fire Detected");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("                "); 
    }

    displayFireMessage = !displayFireMessage;  // Toggle the message display

    delay(1000);  // Display message for 1 second

    // Ultrasonic sensor distance measurement
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    float distanceCm = duration * 0.034 / 2;

    if (distanceCm < distanceThreshold) {
      lcd.setCursor(0, 1);
      lcd.print("Close: ");
      lcd.print(distanceCm);
      lcd.print(" cm    ");

      if (distanceCm < nearDistance) {
        digitalWrite(yellowLED, HIGH);
        slowBeep();
      } else {
        digitalWrite(yellowLED, LOW);
      }

      delay(1000);
    } else {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      digitalWrite(yellowLED, LOW);
    }
  }

  // Turn off LEDs and buzzer after displaying the message
  digitalWrite(buzzer, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);

  delay(1000); // Adjust delay as needed
}
