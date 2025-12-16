// Definiere die Pins für die Steuerung des Motors
const int motorPin1 = 7;  // IN1 (für Vorwärtsbewegung)
const int motorPin2 = 12;  // IN2 (für Rückwärtsbewegung)
const int enablePin = 13;  // ENA (PWM-Pin, um Geschwindigkeit zu steuern)

// Zeitintervalle für Vorwärts- und Rückwärtsbewegung
unsigned long forwardTime = 2000; // Motor dreht für 2 Sekunden vorwärts
unsigned long reverseTime = 2000; // Motor dreht für 2 Sekunden rückwärts

void setup() {
  // Setze die Pins als Ausgänge
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Aktivieren des Motors durch Setzen des Enable-Pins
  digitalWrite(enablePin, HIGH);
}

void loop() {
  // Vorwärtsbewegung
  digitalWrite(motorPin1, HIGH); // Motor dreht vorwärts
  digitalWrite(motorPin2, LOW);
  delay(forwardTime); // Warte für die definierte Zeit (z.B. 2 Sekunden)

  // Rückwärtsbewegung
  digitalWrite(motorPin1, LOW); // Motor dreht rückwärts
  digitalWrite(motorPin2, HIGH);
  delay(reverseTime); // Warte für die definierte Zeit (z.B. 2 Sekunden)
}

