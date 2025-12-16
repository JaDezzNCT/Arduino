//Konstanten/Variabeln
#include <Stepper.h> // Bibliothek für 28BYJ-48 Stepper verwenden

// ------------------------
// Stepper Setup
// ------------------------
const int STEPS_PER_REV = 2048; // Anzahl Schritte pro Umdrehung des Motors
Stepper steeringStepper(STEPS_PER_REV, 2, 4, 3, 5); // Stepper an Pins IN1-IN3-IN2-IN4

int currentAngle = 90; // Aktueller Lenk-Winkel (Start in der Mitte)
const int MAX_LEFT = 60; // Maximale Lenkung links
const int MAX_RIGHT = 120; // Maximale Lenkung rechts

// ------------------------
// Motor Pins (L298N H-Bridge)
// ------------------------
int M_IN1 = 7; // Motor Richtungspin 1
int M_IN2 = 8; // Motor Richtungspin 2
int M_PWM = 6; // Motor PWM-Geschwindigkeit

// ------------------------
// IR-Sensor (1 Sensor)
// ------------------------
const int IR_SENSOR = A0; // Analogpin A0 für Linien-Sensor

// ------------------------
// Potentiometer
// ------------------------
const int POTI = A3; // Drehpoti für Geschwindigkeit

// ------------------------
// Buttons
// ------------------------
const int powerButton = 22; // Power On/Off-Taster (mit Pullup)
const int modeButton = 21; // Modus-Taster
const int autoButton = 24; // Autonom-Modus Sofort-Taster

// ------------------------
// Ultraschall (1 Sensor)
// ------------------------
const int trig = 30; // Trig-Pin für HC-SR04
const int echo = 31; // Echo-Pin für HC-SR04

// ------------------------
// STATES
// ------------------------
bool autoOn = false; // Merkt ob System an/aus ist
bool lastPower = HIGH; // Letzter Status Power-Button
bool lastMode = HIGH; // Letzter Status Mode-Button
bool lastAuto = HIGH; // Letzter Status Autobutton

int mode = 0; // 0=Linie, 1=Autonom, 2=Vorsichtig


//--------------------------------------------------------------------
// MOTOR (L298N H-Bridge)
//--------------------------------------------------------------------
void motor(int speed) {
speed = constrain(speed, -255, 255); // Geschwindigkeit begrenzen

if (speed > 0) { // Vorwärtsfahrt
digitalWrite(M_IN1, HIGH); // Richtung vorwärts
digitalWrite(M_IN2, LOW);
analogWrite(M_PWM, speed); // PWM-Speed setzen
}
else if (speed < 0) { // Rückwärtsfahrt
digitalWrite(M_IN1, LOW);
digitalWrite(M_IN2, HIGH); // Richtung rückwärts
analogWrite(M_PWM, -speed); // Geschwindigkeit positiv machen
}
else { // Speed == 0 → Motor stoppen
digitalWrite(M_IN1, LOW);
digitalWrite(M_IN2, LOW); // Beide LOW = Kein Drehen
analogWrite(M_PWM, 0); // PWM = 0 = Motor off
}
}

void stopAll() {
motor(0); // Hilfsfunktion: Motor komplett stoppen
}

//--------------------------------------------------------------------
// Ultraschall (verbesserte Timeout-Behandlung)
//--------------------------------------------------------------------
long getDist() {
digitalWrite(trig, LOW); // Ultraschall Trigger resetten
delayMicroseconds(2);
digitalWrite(trig, HIGH); // Trigger senden
delayMicroseconds(10);
digitalWrite(trig, LOW);

long dur = pulseIn(echo, HIGH, 30000UL); // Warte auf Echo (max 30ms)

if (dur == 0) return 300; // Timeout → max. Distanz zurückgeben

long dist = dur * 0.0343 / 2; // Mikrosekunden in cm umrechnen
return constrain(dist, 2, 300); // Rauschen vermeiden
}

//--------------------------------------------------------------------
// Stepper Steering
//--------------------------------------------------------------------
void setSteering(int angle) {
angle = constrain(angle, MAX_LEFT, MAX_RIGHT); // Begrenzen auf erlaubte Lenkung

int diff = angle - currentAngle; // Unterschied zum aktuellen Winkel
if (diff == 0) return; // Keine Änderung → Ende

int steps = abs(diff) * (STEPS_PER_REV / 360.0); // Winkel in Steps berechnen
steps = constrain(steps, 1, 50); // Max. 50 Steps pro Aufruf (Smooth)

if (diff > 0) steeringStepper.step(steps); // Nach rechts lenken
else steeringStepper.step(-steps); // Nach links lenken

currentAngle = angle; // Neuen Winkel speichern
}

//--------------------------------------------------------------------
// SETUP
//--------------------------------------------------------------------
void setup() {
Serial.begin(9600); // Serielle Ausgabe starten

pinMode(IR_SENSOR, INPUT); // IR-Sensor Eingang
pinMode(POTI, INPUT); // Potentiometer Eingang

pinMode(powerButton, INPUT_PULLUP); // Buttons mit Pullup
pinMode(modeButton, INPUT_PULLUP);
pinMode(autoButton, INPUT_PULLUP);

pinMode(M_IN1, OUTPUT); // Motor Pins als Output
pinMode(M_IN2, OUTPUT);
pinMode(M_PWM, OUTPUT);

pinMode(trig, OUTPUT); // Ultraschall Trigger
pinMode(echo, INPUT); // Ultraschall Echo

steeringStepper.setSpeed(15); // Stepper Speed (RPM)
setSteering(90); // Start in Mitte
}

//--------------------------------------------------------------------
// LOOP
//--------------------------------------------------------------------
void loop() {

// ---------- BUTTONS ----------
bool p = digitalRead(powerButton); // Power Button lesen
if (p == LOW && lastPower == HIGH) { // Flanken-Erkennung
autoOn = !autoOn; // Toggle On/Off
Serial.println(autoOn ? "EIN" : "AUS");
delay(250); // Debounce
}
lastPower = p;

bool m = digitalRead(modeButton);
if (m == LOW && lastMode == HIGH) {
mode = (mode + 1) % 3; // Modi durchschalten
Serial.print("Modus: ");
Serial.println(mode);
delay(250);
}
lastMode = m;

bool a = digitalRead(autoButton);
if (a == LOW && lastAuto == HIGH) {
mode = 1; // Direkt in Autonom-Modus
Serial.println("Autonom!");
delay(250);
}
lastAuto = a;

// ---------- Wenn AUS ----------
if (!autoOn) { // Wenn Power OFF
stopAll(); // Motor stoppen
setSteering(90); // Lenkung zentrieren
delay(50);
return;
}

// ---------- Geschwindigkeit über Poti ----------
int potiVal = analogRead(POTI); // Poti lesen (0–1023)
int baseSpeed = map(potiVal, 0, 1023, 80, 200); // Geschwindigkeit skalieren


//--------------------------------------------------------------------
// MODE 0 – LINE FOLLOWER
//--------------------------------------------------------------------
if (mode == 0) {
int S = digitalRead(IR_SENSOR); // Linie erkannt? (0=Linie, 1=weiß)

if (S == 0) { // Linie direkt drunter
motor(baseSpeed);
setSteering(90);
}
else { // Linie verloren
motor(baseSpeed * 0.5); // Langsamer suchen
static bool left = true;

if (left) setSteering(75); // Links suchen
else setSteering(105); // Rechts suchen

left = !left; // Richtungswechsel
delay(50);
}
}

//--------------------------------------------------------------------
// MODE 1 – AUTONOM
//--------------------------------------------------------------------
else if (mode == 1) {
long dist = getDist(); // Entfernung messen

if (dist < 18) { // Sehr nah → ausweichen
stopAll();
delay(150);

setSteering(65); // Links checken
delay(200);
long leftDist = getDist();

setSteering(115); // Rechts checken
delay(200);
long rightDist = getDist();

if (leftDist > rightDist && leftDist > 25) {
setSteering(65); // Links frei
motor(baseSpeed * 0.8);
delay(400);
}
else if (rightDist > 25) {
setSteering(115); // Rechts frei
motor(baseSpeed * 0.8);
delay(400);
}
else {
motor(-baseSpeed * 0.3); // Rückwärts wenn beide blockiert
delay(300);
}

setSteering(90); // Zurück in Mitte
return;
}

motor(baseSpeed); // Geradeaus weiter
setSteering(90);
}

//--------------------------------------------------------------------
// MODE 2 – VORSICHTIG
//--------------------------------------------------------------------
else if (mode == 2) {
long dist = getDist(); // Entfernung messen
int slowSpeed = baseSpeed * 0.35; // Sehr reduziert fahren

if (dist < 25) {
stopAll(); // Sofort stoppen
setSteering(90);
}
else if (dist < 45) {
motor(slowSpeed * 0.7); // Sehr langsam
setSteering(90);
}
else {
motor(slowSpeed); // Normal langsam
setSteering(90);
}
}

delay(20); // Stabiler Loop
}
