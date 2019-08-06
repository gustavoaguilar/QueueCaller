#include <Arduino.h>
#include <SPI.h>

#define DISPLAY_CS 10
#define BUZZER_PIN 8

//Setup the MAX7219 to use the display
void setupDisplay();

//Clear all 4 digits of the display
void clearDisplay();

/*
  Sends data to MAX7219 through SPI protocol
  first the address of the register
  then the data to the register
*/
void sendData(uint8_t addr, uint8_t data, uint8_t cs);

//Sends the number to the display
void writeOrder(uint8_t num[4]);

//Play a beep using CPU (will be used as a delay too)
void myTone(byte pin, uint16_t frequency, uint16_t duration);

// 4 bytes to the number and 1 byte to the beep flag
uint8_t order[5] = {0,0,0,0,1};

void setup() {
  
  //Setup the CS pin for MAX7219, High as default
  pinMode(DISPLAY_CS, OUTPUT);
  pinMode(BUZZER_PIN, INPUT); // set buzzer to input to avoid noise
  digitalWrite(DISPLAY_CS, HIGH);
   
  //Configure and start SPI
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  //Start Serial communication
  Serial.begin(9600);

  //Start the display
  setupDisplay();
  writeOrder(order);
}
//
void loop() {
  //if information is received from the Serial Port
  if(Serial.available() > 0){
    Serial.readBytes(order, 5); // read all 5 bytes
    
    //Cleans Buffer
    while(Serial.available() > 0)
      Serial.read();

    //if the 'b' flag is present, Blink and Beep while update the display
    if(order[4] == 'b'){
      clearDisplay();
      delay(300);
      writeOrder(order);
      myTone(8,880,500); // myTone is used as a delay while generate sound
      clearDisplay();
      myTone(8,550,500);
      writeOrder(order);
    }
    //if the 'b' is NOT present, just update the display
    else{
      writeOrder(order);
    }

  }
  //wait 500ms
  delay(500);
}

void writeOrder(uint8_t num[4]){
  /*
    This logic must be used because the order the digits are flipped
    at the display, the digit 1 is the Less Significant
    at the num[] the digit 1 is the Most Significant
  */
  uint8_t j=3; // start with the Most significant digit of the display
  for (uint8_t i=1; i<=4; i++, j--)
  {
    sendData(i, num[j], DISPLAY_CS);
  }
 
}

void sendData(uint8_t addr, uint8_t data, uint8_t cs){
  digitalWrite(cs, LOW);
  SPI.transfer(addr);
  SPI.transfer(data);
  digitalWrite(cs, HIGH);
}

void clearDisplay(){
  for(int i=1; i<=4; i++){
    sendData(i, 0x0F, DISPLAY_CS);
  }
}

void setupDisplay(){
  sendData(0x0A, 0x02, DISPLAY_CS);// Intensity - Reduce the intensity
  sendData(0x0B, 0x03, DISPLAY_CS);// Scan Limit - Scan only 4 digits
  sendData(0x09, 0xFF, DISPLAY_CS);// Decode - Decode the information received
  sendData(0x0C, 0x01, DISPLAY_CS);// Shutdown - Operate normaly
  
  /*NOTE: I don't know why, but this prevents the display from malfunctioning*/
  sendData(0x0F, 0x01, DISPLAY_CS);// Test Mode - Enables test mode
  sendData(0x0F, 0x00, DISPLAY_CS);// Test Mode - Disables test mode
}

void myTone(byte pin, uint16_t frequency, uint16_t duration){
  unsigned long startTime=millis();
  unsigned long halfPeriod= 1000000L/frequency/2;
  pinMode(pin, OUTPUT);
  while (millis()-startTime < duration){
    digitalWrite(pin, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(pin, LOW);
    delayMicroseconds(halfPeriod);
  }
  pinMode(pin, INPUT);
}