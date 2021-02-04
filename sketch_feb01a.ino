#include <OneWire.h>

// Button for increase temperature
#define PIN_BUTTON_LEFT 3
// Button for decrese temperature
#define PIN_BUTTON_RIGHT 2

// DS18S20 Temperature sensor
#define PIN_DS18S20 6

// relay control
#define PIN_RELAY 5

// Switch debouncing gap
#define DEBOUNCE_TIME 300 

// Max & min temperature
#define MAX_TEMP 100
#define MIN_TEMP 55

// Initial target temperature
char temp_target = 60;

// Object for DS18S20
OneWire ds(PIN_DS18S20);

void setup()
{
  // Setup pin mode
  pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);     
  pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(PIN_RELAY, OUTPUT);
  // Setup ISR for buttons
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_LEFT), button1_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_RIGHT), button2_isr, FALLING);

  Serial.begin(9600);
}

void loop()
{
  // Debug print
  char buf[0x10];
  float temp_curr = getTemp(); 
  sprintf(buf, "target: %d", temp_target);
  Serial.println(buf);
  sprintf(buf, "curr: %d.%d", (int)temp_curr, (int)(temp_curr * 10) % 10);
  Serial.println(buf);
  if(temp_curr < temp_target){
    digitalWrite(PIN_RELAY,HIGH);
  }
  else{
    digitalWrite(PIN_RELAY,LOW);
  }
}

void button1_isr(){
  // Software debouncing
  static unsigned long last = 0;
  unsigned long now = millis();
  if((now - last) > DEBOUNCE_TIME && temp_target < MAX_TEMP){
    temp_target++;
    last = now;
  }
}

void button2_isr(){
  // Software debouncing
  static unsigned long last = 0;
  unsigned long now = millis();
  if((now - last) > DEBOUNCE_TIME && temp_target > MIN_TEMP){
    temp_target--;
    last = now;
  }
}

float getTemp(){
 byte data[12];
 byte addr[8];
 if ( !ds.search(addr)) {
   ds.reset_search();
   return -1000;
 }
 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }
 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
 }
 ds.reset();
 ds.select(addr);
 ds.write(0x44,1);                                   
 byte present = ds.reset();
 ds.select(addr);  
 ds.write(0xBE); 
 
 for (int i = 0; i < 9; i++) { 
  data[i] = ds.read();                                                          
 }
 
 ds.reset_search(); 
 byte MSB = data[1];
 byte LSB = data[0];
 float tempRead = ((MSB << 8) | LSB); 
 float TemperatureSum = tempRead / 16; 
 return TemperatureSum;                                                                    
}
