// Define the analog pin, the LM35's Vout pin is connected to
#define sensorPin A0
#define interruptPinTemp D0

float calTemp = 5.0;
static int maxTemp = 80;

void setup() {
  // Begin serial communication at 9600 baud rate
  attachInterrupt(digitalPinToInterrupt(interruptPinTemp), isrTemp, RISING);
  Serial.begin(9600);
}

void loop() {
  float tempReading = tempCelsius();

  if (tempReading >= maxTemp) {
    digitalWrite(interruptPinTemp, HIGH);
  }
  else{
    digitalWrite(interruptPinTemp, LOW);
  }
}

float tempCelsius() {
  // Get the voltage reading from the LM35
  int reading = analogRead(sensorPin);

  // Convert that reading into voltage
  float voltage = reading * (calTemp / 1024.0);

  // Convert the voltage into the temperature in Celsius
  float temperatureC = voltage * 100;

  // Check sensor calibration value

  return temperatureC;
}

void isrTemp(){
  digitalWrite(D0, HIGH);
}