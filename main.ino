int SENSOR_PIN = A0;
const int LED_PID  = 3;

#define DEBUG false;

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  
  pinMode(LED_PID, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
}

int val = 0;
int brightness = 0;
int fadeAmount = 5;

int samples[] = {0, 0, 0, 0, 0};
int sampleIndex = 0;
const int MAX_SAMPLES = 5;

const int MAX_THRESHHOLD = 50;

bool shouldDisable() {
  int sum = 0;
  for(int i=0; i<MAX_SAMPLES; i++) {
    sum += samples[i];
  }

  #ifdef DEBUG
    Serial.println(sum);
  #endif
  
  if (sum < 0) {
    return true;
  }

  return false;
}


void loop() {
  val = analogRead(SENSOR_PIN);

  float voltage = val * (5.0 / 1023.0);
   // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
 
  boolean isAtThreshhold = val > MAX_THRESHHOLD ? true : false;

  if (isAtThreshhold) {
    analogWrite(LED_PID, brightness);
  } else if (shouldDisable()) { 
    digitalWrite(LED_PID, LOW);
  }

  // check if we need to reset index
  if (sampleIndex > MAX_SAMPLES) {
    sampleIndex = 0;
  }

  // insert a sample
  if (isAtThreshhold) {
    samples[sampleIndex] = 1;
  } else {
    samples[sampleIndex] = 0;
  }
  
  sampleIndex++;
  
  delay(30);


}