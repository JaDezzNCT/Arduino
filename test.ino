// Definiere die Pins für die Steuerung des Motors
const int motorPin1 = 7;  // IN1 (für Vorwärtsbewegung)
const int motorPin2 = 12;  // IN2 (für Rückwärtsbewegung)
const int enablePin = 13;  // ENA (PWM-Pin, um Geschwindigkeit zu steuern)
const int trigPin = 32;    // Trig Pin für Ultraschallsensor
const int echoPin = 33;    // Echo Pin für Ultraschallsensor
// Zeitintervalle für Vorwärts- und Rückwärtsbewegung
unsigned long forwardTime = 2000; // Motor dreht für 2 Sekunden vorwärts
unsigned long reverseTime = 2000; // Motor dreht für 2 Sekunden rückwärts
long duration;
int distance;

void setup() {
  // Setze die Pins als Ausgänge
  Serial.begin(9600);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  // Aktivieren des Motors durch Setzen des Enable-Pins
  digitalWrite(enablePin, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("Ultrasonic Sensor + Motorsteuerung gestartet...");
}

void loop() {
  // Ultraschallsensor auslesen
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Ultraschall: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Motorsteuerung basierend auf dem Abstand
  Motor();

  Serial.println("------------------------");
  delay(200);  // Kurze Verzögerung
}

// Motorsteuerung basierend auf dem Abstand
void Motor() {
  if (distance > 10) {
    // Motor vorwärts
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    Serial.println("Motor fährt vorwärts");
  } 
  else {
    // Motor rückwärts
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    Serial.println("Motor fährt rückwärts");
  }
}

