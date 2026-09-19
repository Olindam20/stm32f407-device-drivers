/*
 * SPI pin numbers:
 * SCK   13  // Serial Clock.
 * MISO  12  // Master In Slave Out.
 * MOSI  11  // Master Out Slave In.
 * SS    10  // Slave Select
 */

#include <SPI.h>

const byte led = 13;           // Slave LED digital I/O pin.
boolean ledState = HIGH;      // LED state flag.

uint8_t dataBuff[255];
uint8_t board_id[11] = "ARDUINOUNO";

#define NACK 0xA5
#define ACK 0xF5

// Command codes
#define COMMAND_LED_CTRL          0x50
#define COMMAND_SENSOR_READ       0x51
#define COMMAND_LED_READ          0x52
#define COMMAND_PRINT             0x53
#define COMMAND_ID_READ           0x54

#define LED_ON     1
#define LED_OFF    0

// Arduino analog pins
#define ANALOG_PIN0   0
#define ANALOG_PIN1   1
#define ANALOG_PIN2   2
#define ANALOG_PIN3   3
#define ANALOG_PIN4   4

// Initialize SPI slave
void SPI_SlaveInit(void) 
{ 
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT);
  
  // Enable SPI as slave
  SPCR = (1 << SPE);
}

// Returns SPDR Contents 
uint8_t SPI_SlaveReceive(void)
{
  while(!(SPSR & (1 << SPIF)));
  return SPDR;
}

// Sends one byte of data 
void SPI_SlaveTransmit(uint8_t data)
{
  SPDR = data;
  while(!(SPSR & (1 << SPIF)));
}

void setup() 
{
  Serial.begin(9600);
  
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  SPI_SlaveInit();
  Serial.println("Slave Initialized");
}

byte checkData(byte command)
{
  if(command >= COMMAND_LED_CTRL && command <= COMMAND_ID_READ)
  {
    return ACK;
  }
  return NACK;
}

void loop() 
{
  byte data, command, len, ackornack = NACK;
  
  // 1. Wait until SS goes LOW
  Serial.println("Slave waiting for ss to go low");
  while(digitalRead(SS));
  
  // 2. Read command byte
  command = SPI_SlaveReceive();
  ackornack = checkData(command);
  
  // 3. Send ACK/NACK
  SPI_SlaveTransmit(ackornack);
  
  // 4. Dummy read for the clock that fetched ACK
  len = SPI_SlaveReceive(); 
  
  if(command == COMMAND_LED_CTRL)
  {
    uint8_t pin = SPI_SlaveReceive(); 
    uint8_t value = SPI_SlaveReceive(); 
    Serial.println("RCVD:COMMAND_LED_CTRL");
    if(value == (uint8_t)LED_ON)
    {
      digitalWrite(pin, HIGH);
    }
    else if (value == (uint8_t)LED_OFF)
    {
      digitalWrite(pin, LOW);
    }
  }
  else if (command == COMMAND_SENSOR_READ)
  {
    uint16_t aread;
    uint8_t pin = SPI_SlaveReceive(); 
    uint8_t val;
    aread = analogRead(pin + 14);
    val = map(aread, 0, 1023, 0, 255);
    
    SPI_SlaveTransmit(val);
    val = SPI_SlaveReceive(); // dummy read
    
    Serial.println("RCVD:COMMAND_SENSOR_READ");
  }
  else if (command == COMMAND_LED_READ)
  {
    uint8_t pin = SPI_SlaveReceive(); 
    uint8_t val = digitalRead(pin);
    SPI_SlaveTransmit(val);
    val = SPI_SlaveReceive(); // dummy read
    Serial.println("RCVD:COMMAND_LED_READ");
  }
  else if (command == COMMAND_PRINT)
  {
    uint8_t len = SPI_SlaveReceive(); 
    for(int i = 0; i < len; i++)
    {
      dataBuff[i] = SPI_SlaveReceive();
    }
    dataBuff[len] = '\0'; // 👈 Null-terminate so it knows where string ends!
    
    Serial.println((char*)dataBuff);
    Serial.println("RCVD:COMMAND_PRINT");
  }
  else if (command == COMMAND_ID_READ)
  {
    for(int i = 0; i < strlen((char*)board_id); i++)
    {
      SPI_SlaveTransmit(board_id[i]);
    }
    SPI_SlaveReceive();
    Serial.println("RCVD:COMMAND_ID_READ");
  }
}