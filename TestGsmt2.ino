// -------------------------
// Pins IR-Sensor (digitaler Objektsensor)
// -------------------------
const int irPin = 2;   // OUT-Pin des IR-Sensors

// -------------------------
// Pins Ultraschallsensor HC-SR04
// -------------------------
const int trigPin = 32;
const int echoPin = 33;

// -------------------------
// Variablen Ultraschall
// -------------------------
long duration;     // Dauer des Echo-Signals
long distance;     // Entfernung in cm

void setup() {
  Serial.begin(9600);

  // IR-Sensor
  pinMode(irPin, INPUT);

  // Ultraschallsensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("IR + Ultrasonic Sensor gestartet...");
}

void loop() {
  // ------------------------------------------
  // IR-Sensor auslesen
  // ------------------------------------------
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

  // Timeout nach 30 ms (ca. 5 m)
  duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    Serial.println("Ultraschall: Kein Echo");
  } else {
    distance = duration * 0.034 / 2;
    Serial.print("Ultraschall: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  Serial.println("------------------------");
  delay(200);
}
