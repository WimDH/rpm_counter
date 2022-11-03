#include <Arduino.h>

unsigned long rpmtime;
float rpmfloat;
unsigned int rpm;
bool toslow = 1;
float vbat_digital;
float vbat = 0;
float vbat_min = 0;
char vbat_str [8];
char vbat_min_str [8];
char lcd_line1 [18];
char lcd_line2 [18];


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
  vbat_min = 99;
  Serial.write(22);
}


void loop() {
  delay(250);
  vbat_digital = analogRead(A1);
  vbat = vbat_digital / 61.50;

  if (vbat_min > vbat or vbat_min == 0) {
    vbat_min = vbat;
  }
  
  if (toslow == 0) {
    rpmfloat = 120 / (rpmtime / 31250.00);
    rpm = round(rpmfloat);
  } else {
    rpm = 0;
  }
  Display();
}

ISR(TIMER1_OVF_vect) {
  toslow = 1;
}

void rpmCount() {
  rpmtime = TCNT1;
  TCNT1 = 0;
  toslow = 0;  
}

void clearScreen() {
  Serial.write(128); // First line
  Serial.print("                ");
  Serial.write(148); // Second line
  Serial.print("                ");
}

void Display() {
  dtostrf(vbat, 4, 2, vbat_str);
  dtostrf(vbat_min, 4, 2, vbat_min_str);
  sprintf (lcd_line1, "%4u RPM", rpm);
  sprintf (lcd_line2, "V: %s (%s)", vbat_str, vbat_min_str);
  clearScreen();
  Serial.write(128);
  Serial.print(lcd_line1);
  Serial.write(148);
  Serial.print(lcd_line2);
}
