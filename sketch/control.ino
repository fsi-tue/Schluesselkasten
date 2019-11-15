void doorStatus() {
  int switchValue;
  switchValue = analogRead(SWITCH_PIN);
  switchValue = map(switchValue, 0, 1024, 0, 2);
  if (switchValue == 2) {
    closed = true;
  }
  else if (switchValue == 1) {
    //Error undefined
  }
  else {
    closed = false;
  }
  Serial.print("Door closed: ");
  Serial.println(closed);
}

void closeDoor() {
  Serial.println( "Closing Door");
  display.print(3004);
  int fallbackSteps = 0;
  for (int i = 0; i < maxSteps; i++) {
    step_right();

    //Check if Door was opened while closing
    fallbackSteps++;
    doorStatus();

    if (!closed) {
      i = maxSteps;
      openDoor(fallbackSteps);
    }
  }
  reset_pins();
  delay(500);

}

void openDoor(int fallbackSteps) {
  Serial.println( "Opening Door");
  display.print(3003);
  for (int i = 0; i < maxSteps && i < fallbackSteps; i++) {
    step_left();
  }
  reset_pins();
  delay(500);

}

void reset_pins() {
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
  digitalWrite(MOTOR_PIN3, LOW);
  digitalWrite(MOTOR_PIN4, LOW);
}

void step_right() {
  digitalWrite(MOTOR_PIN1, HIGH);
  digitalWrite(MOTOR_PIN2, LOW);
  digitalWrite(MOTOR_PIN3, LOW);
  digitalWrite(MOTOR_PIN4, LOW);
  delay(STEP_DELAY);
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, HIGH);
  digitalWrite(MOTOR_PIN3, LOW);
  digitalWrite(MOTOR_PIN4, LOW);
  delay(STEP_DELAY);
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
  digitalWrite(MOTOR_PIN3, HIGH);
  digitalWrite(MOTOR_PIN4, LOW);
  delay(STEP_DELAY);
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
  digitalWrite(MOTOR_PIN3, LOW);
  digitalWrite(MOTOR_PIN4, HIGH);
  delay(STEP_DELAY);
}

void step_left() {
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
  digitalWrite(MOTOR_PIN3, LOW);
  digitalWrite(MOTOR_PIN4, HIGH);
  delay(STEP_DELAY);
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
  digitalWrite(MOTOR_PIN3, HIGH);
  digitalWrite(MOTOR_PIN4, LOW);
  delay(STEP_DELAY);
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, HIGH);
  digitalWrite(MOTOR_PIN3, LOW);
  digitalWrite(MOTOR_PIN4, LOW);
  delay(STEP_DELAY);
  digitalWrite(MOTOR_PIN1, HIGH);
  digitalWrite(MOTOR_PIN2, LOW);
  digitalWrite(MOTOR_PIN3, LOW);
  digitalWrite(MOTOR_PIN4, LOW);
  delay(STEP_DELAY);
}
