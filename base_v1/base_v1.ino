void wait();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Serial init
  Serial.begin(115200);
  delay(1000);
  Serial.print("\r\nCLV-DIY-V1:");
  Serial.print(VER);
  Serial.print("\r\n");
}

int mode = 0;

void loop() {
  if (mode == 0)
    wait();
}

void wait() {
    digitalWrite(LED_BUILTIN, 1);
  delay(60);
  digitalWrite(LED_BUILTIN, 0);
  delay(60);
  digitalWrite(LED_BUILTIN, 1);
  delay(60);
  digitalWrite(LED_BUILTIN, 0);
  delay(1820);
}