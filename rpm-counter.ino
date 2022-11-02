#include <Arduino.h>

unsigned long rpmtime;
float rpmfloat;
unsigned int rpm;
bool toslow = 1;
float vbat_digital;
float vbat;
float vbat_min;


void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);  // RPM pulse on D2
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12);   // Set prescaler to 256
  TIMSK1 |= (1 << TOIE1);  // Enable timer overflow  
  attachInterrupt(0, rpmCount, FALLING);
  sei();
  Serial.println("Start");
  vbat_min = 99;
}


void loop() {
  delay(250);
  vbat_digital = analogRead(A1);
  vbat = vbat_digital / 61.50;

  if (vbat_min > vbat) {
    vbat_min = vbat;
  }
  
  if (toslow == 0) {
    rpmfloat = 120 / (rpmtime / 31250.00);
    rpm = round(rpmfloat);
    Serial.print(vbat);
    Serial.print(" - ");
    Serial.print(vbat_digital);
    Serial.print(" - ");
    Serial.println(rpm);
  } else {
    Serial.println("-");
  }
  
}

ISR(TIMER1_OVF_vect) {
  toslow = 1;
}

void rpmCount() {
  rpmtime = TCNT1;
  TCNT1 = 0;
  toslow = 0;  
}
