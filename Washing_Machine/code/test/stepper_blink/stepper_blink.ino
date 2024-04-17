#define DIR_PIN 3
#define STEP_PIN 4

int speedDelay = 500;

void setup() {
  // put your setup code here, to run once:
   pinMode(DIR_PIN, OUTPUT);
   pinMode(STEP_PIN, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT);

   digitalWrite(STEP_PIN, HIGH);
   digitalWrite(DIR_PIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(STEP_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(speedDelay);
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(speedDelay);
}
