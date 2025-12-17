// Definiere die Pins für die Steuerung des Motors
#include <Stepper.h>
const int motorPin1 = 7;  // IN1 (für Vorwärtsbewegung)
const int motorPin2 = 12;  // IN2 (für Rückwärtsbewegung)
const int enablePin = 13;  // ENA (PWM-Pin, um Geschwindigkeit zu steuern)
const int SchalterPin = 0;
const int stepsPerRevolution = 4096;
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5); //Pins
// Zeitintervalle für Vorwärts- und Rückwärtsbewegung
unsigned long forwardTime = 2000; // Motor dreht für 2 Sekunden vorwärts
unsigned long reverseTime = 2000; // Motor dreht für 2 Sekunden rückwärts
const int trigPin = 32;
const int echoPin = 33;
long duration;
int distance;
int Lenkungsschritte; 
void setup() {
  // Setze die Pins als Ausgänge
  Serial.begin(9600);
  myStepper.setSpeed(15);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(SchalterPin, INPUT_PULLUP);
  Serial.println("IR + Ultrasonic Sensor gestartet...");
  // Aktivieren des Motors durch Setzen des Enable-Pins
  digitalWrite(enablePin, HIGH);
}
void fahren() {
  if(SchalterPin == true) {
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);  // Motor dreht vorwärts
  }
}
void lenkung() {
  if(distance =< 10) {
      digitalWrite(motorPin1, LOW); // Motor dreht rückwärts
      digitalWrite(motorPin2, HIGH);
      int stepsPerRevolution = 4096;
      int stepsFor15Deg = round((15.0 / 360.0) * stepsPerRevolution);  // Ergibt 171
      myStepper.step(stepsFor15Deg);
  }
  

}
void loop() {
  // Vorwärtsbewegung
int irState = digitalRead(irPin);

  if (irState == HIGH) {
    Serial.println("IR: Kein Hindernis erkannt");
  } else {
    Serial.println("IR: Hindernis erkannt!");
  }

  // ------------------------------------------
  // Ultraschallsensor auslesen
  // ------------------------------------------
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

  Serial.println("------------------------");
  delay(200);
}

