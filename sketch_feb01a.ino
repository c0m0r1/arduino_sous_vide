// Button for increase temperature
#define PIN_BUTTON_LEFT 3
// Button for decrese temperature
#define PIN_BUTTON_RIGHT 2

// Switch debouncing gap
#define DEBOUNCE_TIME 300 

// Max & min temperature
#define MAX_TEMP 100
#define MIN_TEMP 55

// Initial target temperature
char temp_target = 60;

void setup()
{
  // Setup pin mode
  pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);     
  pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
  // Setup ISR for buttons
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_LEFT), button1_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_RIGHT), button2_isr, FALLING);

  Serial.begin(9600);
}

void loop()
{
  // Debug print
  char buf[0x10];
  sprintf(buf, "target: %d", temp_target);
  Serial.println(buf);
  
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

   
