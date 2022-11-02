#include <Arduino.h>

unsigned long rpmtime;
float rpmfloat;
unsigned int rpm;
bool toslow = 1;
float vbat;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12);   // Prescaler 256
  TIMSK1 |= (1 << TOIE1);  // Enable timer overflow  
  attachInterrupt(0, rpmCount, FALLING);
  sei();
  Serial.println("Start");
}


void loop() {
  delay(250);
  
  if (toslow == 0) {
    rpmfloat = 120 / (rpmtime / 31250.00);
    rpm = round(rpmfloat);
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
