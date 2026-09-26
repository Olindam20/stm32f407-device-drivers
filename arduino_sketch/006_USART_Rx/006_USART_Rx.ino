#include <SoftwareSerial.h>

// Use Pin 2 as RX from STM32
SoftwareSerial stm32Serial(2, 3); 

void setup() {
  Serial.begin(115200);        // PC Serial Monitor
  stm32Serial.begin(115200);   // STM32 input
  pinMode(13, OUTPUT);
  Serial.println("--- Arduino Ready ---");
}

void loop() {
  digitalWrite(13, LOW); 
  
  if (stm32Serial.available() > 0) {
    digitalWrite(13, HIGH); 
    char in_read = stm32Serial.read();
    Serial.print(in_read);
  }
}