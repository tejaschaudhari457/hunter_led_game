// ------------------ USER SETTINGS ------------------
int ledPins[] = {12, 13, 14, 15, 16, 17, 18, 19};
int numLEDs = 8;
int middleLED = 4; // Index of target LED (0-based)
int touchPin = 33; // digital touch pin input

int level = 1;
int score = 0;
int delayTime = 300; // LED movement speed in ms
int levelTime = 10;  // Time per level in seconds

unsigned long startTime;
unsigned long lastMoveTime = 0;
unsigned long buttonDebounceTime = 0;
unsigned long debounceDelay = 150;   // increased for touch stability

bool gameEnded = false;
int currentLED = 0;
int direction = 1; // 1 = forward, -1 = backward

// ------------------ SETUP ------------------
void setup() {
  Serial.begin(115200);

  // LED pin setup
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Touch pin setup
  pinMode(touchPin, INPUT);

  Serial.println("Hunter LED Game Started!");
  startTime = millis();
}

// ------------------ MAIN LOOP ------------------
void loop() {
  if (gameEnded) return;

  // Level timer
  unsigned long elapsedTime = (millis() - startTime) / 1000;
  int remainingTime = levelTime - elapsedTime;
  if (remainingTime <= 0) {
    endGame();
    return;
  }

  // LED movement timing
  if (millis() - lastMoveTime >= delayTime) {
    moveLED();
    lastMoveTime = millis();
  }

  // Touch detection
  int touchState = digitalRead(touchPin);
  
  if (touchState == HIGH && millis() - buttonDebounceTime > debounceDelay) {
    buttonDebounceTime = millis();

    if (currentLED == middleLED) {
      score++;
      Serial.println("🎯 HIT!");
      levelUp();
    } else {
      Serial.println("❌ MISS!");
    }
  }

  // Display status once per second
  static int lastDisplay = -1;
  if (remainingTime != lastDisplay) {
    Serial.print("Level: "); Serial.print(level);
    Serial.print(" | Score: "); Serial.print(score);
    Serial.print(" | Time left: "); Serial.println(remainingTime);
    lastDisplay = remainingTime;
  }
}

// ------------------ FUNCTIONS ------------------

void moveLED() {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[currentLED], HIGH);

  currentLED += direction;
  if (currentLED == numLEDs - 1 || currentLED == 0) {
    direction = -direction;
  }
}

void levelUp() {
  level++;
  Serial.print("Level Up! New Level: ");
  Serial.println(level);

  delayTime -= 30;
  if (delayTime < 50) delayTime = 50;

  startTime = millis(); // reset timer
}

void endGame() {
  gameEnded = true;
  Serial.println("===== GAME OVER =====");
  Serial.print("Final Score: ");
  Serial.println(score);

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
  Serial.println("===== NEW GAME STARTED =====");
}
