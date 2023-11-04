#include <PID_v1.h>   // PID library

// DC24V40RPM DC motor  and MD30C R2 30A DC Motor Driver
#define DIR 10         // DIR motor Driver
#define PWM_OUTPUT 12  // PWM motor Driver

// PIR
double Setpoint, Input, Output;
float kecepatan;
double Kp = 2, Ki = 5, Kd = 1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
const int PPR = 100;  //define number of pulses in one round of encoder

//Time variables
long previousMillis = 0;
long currentMillis = 0;

void setup() {
  //Input = analogRead(PIN_INPUT);
  Setpoint = 100;
  Serial.begin(9600);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255);
}

void loop() {
  //Input = analogRead(PIN_INPUT);
  Input = kecepatan;

  myPID.Compute();
  analogWrite(PWM_OUTPUT, Output);

  Serial.print("Input = ");
  Serial.print(Input);
  Serial.println();
  Serial.print("Output = ");
  Serial.print(Output);
  Serial.println();
}
