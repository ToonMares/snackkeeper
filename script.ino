#define TRIG_PIN 6
#define ECHO_PIN 5
#define BUZZER_PIN_ALERT 11
#define LED_PIN_1 8
#define LED_PIN_2 9
#define MOVEMENT_THRESHOLD 50
#define ALERT_DURATION 1500
#define BUZZER_INTENSITY 20

long previousDistance = 0;
unsigned long alertStartTime = 0;
bool alertTriggered = false;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN_ALERT, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;

  if (abs(distance - previousDistance) >= 3 && !alertTriggered) {
    if (distance < previousDistance) {
      alertStartTime = millis();
      activateAlert();
      alertTriggered = true;
    }
  } else if (abs(distance - previousDistance) < 3 && alertTriggered) {
    alertTriggered = false;
  }

  if (millis() - alertStartTime >= ALERT_DURATION) {
    deactivateAlert();
    alertTriggered = false;
  }

  previousDistance = distance;
  delay(100);
}

void activateAlert() {
  while (millis() - alertStartTime < ALERT_DURATION) {
    analogWrite(BUZZER_PIN_ALERT, BUZZER_INTENSITY);
    delay(100);
    analogWrite(BUZZER_PIN_ALERT, 0);
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, HIGH);
    delay(250);
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    delay(250);
  }
}

void deactivateAlert() {
  analogWrite(BUZZER_PIN_ALERT, 0);
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);
}
