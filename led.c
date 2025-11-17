// LED pins
int ledPins[] = {12, 13, 14, 15, 16, 17, 18, 19};
int numLEDs = 8;
int middleLED = 4; // Index of target LED (0-based)
int buttonPin = 2;

int level = 1;
int score = 0;
int delayTime = 300; // LED movement speed in ms
int levelTime = 10;  // Time per level in seconds

unsigned long startTime;
unsigned long lastMoveTime = 0;
unsigned long buttonDebounceTime = 0;
unsigned long debounceDelay = 50;

bool gameEnded = false;
int currentLED = 0;
int direction = 1; // 1 = forward, -1 = backward

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("Hunter Game Started!");
  startTime = millis();
}

void loop() {
  if (gameEnded) return;

  // Timer
  unsigned long elapsedTime = (millis() - startTime) / 1000;
  int remainingTime = levelTime - elapsedTime;
  if (remainingTime <= 0) {
    endGame();
    return;
  }

  // Move LED sequence
  if (millis() - lastMoveTime >= delayTime) {
    moveLED();
    lastMoveTime = millis();
  }

  // Check button press
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && millis() - buttonDebounceTime > debounceDelay) {
    buttonDebounceTime = millis();
    if (currentLED == middleLED) {
      score++;
      Serial.println("Hit!");
      levelUp();
    } else {
      Serial.println("Miss!");
    }
  }

  // Print status every second
  static int lastDisplay = -1;
  if (remainingTime != lastDisplay) {
    Serial.print("Level: "); Serial.print(level);
    Serial.print(" | Score: "); Serial.print(score);
    Serial.print(" | Time left: "); Serial.println(remainingTime);
    lastDisplay = remainingTime;
  }
}

void moveLED() {
  // Turn off all LEDs
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  // Light current LED
  digitalWrite(ledPins[currentLED], HIGH);

  // Move to next LED
  currentLED += direction;
  if (currentLED == numLEDs - 1 || currentLED == 0) {
    direction = -direction;
  }
}

void levelUp() {
  level++;
  Serial.print("Level Up! Now at Level: "); Serial.println(level);

  // Increase speed (decrease delayTime)
  delayTime -= 30;
  if (delayTime < 50) delayTime = 50;

  startTime = millis(); // Reset timer for next level
}

void endGame() {
  gameEnded = true;
  Serial.println("Game Over!");
  Serial.print("Final Score: "); Serial.println(score);

  // Reset game after 3 seconds
  delay(3000);
  resetGame();
}

void resetGame() {
  gameEnded = false;
  score = 0;
  level = 1;
  delayTime = 300;
  currentLED = 0;
  direction = 1;
  startTime = millis();
  Serial.println("New Game Started!");
}
