#include <Stepper.h>

const int stepsPerRevolution = 2048; // Standard für 28BYJ-48

Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

void setup() {
myStepper.setSpeed(15); // 15 RPM ist sicher
}

void loop() {
// Eine komplette Umdrehung vorwärts
myStepper.step(stepsPerRevolution);
delay(1000);

// Eine komplette Umdrehung rückwärts
myStepper.step(-stepsPerRevolution);
delay(1000);
}
