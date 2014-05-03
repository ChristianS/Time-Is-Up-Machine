// PINs
const int redLedPin = 9;     // red LED pin
const int yellowLedPin = 7;  // yellow LED pin
const int greenLedPin = 6;     // green LED pin
const int buttonPin = 4;      // button input pin
const int speakerPin = 3;    // speaker output pin

// Button Debounce
int buttonState = 0;
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;

// Timers
unsigned long startTime = 0;
unsigned long alarmTimeInterval = 1000;

boolean counting = false;

void setup()
{
  pinMode(redLedPin, OUTPUT);  
  pinMode(greenLedPin, OUTPUT);   
  pinMode(yellowLedPin, OUTPUT);     
  pinMode(buttonPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop()
{
  int reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        Serial.println("button");
        
        startTime = millis();
        counting = true;
        
        digitalWrite(redLedPin, HIGH);
        digitalWrite(yellowLedPin, HIGH);
        digitalWrite(greenLedPin, LOW);
      }
    }
  }
  
  lastButtonState = reading;
  
  unsigned long currentTime = millis();
  
  // half the time is up
  if ((currentTime - startTime) > (alarmTimeInterval / 2) && counting == true)
  {
    digitalWrite(redLedPin, LOW);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  }
 
  // time is up
  if ((currentTime - startTime) > alarmTimeInterval && counting == true)
  {
    Serial.println("alarm");
    
    digitalWrite(redLedPin, LOW); 
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    
    // play melody
    for (int i = 330; i < 523; i++)
    {
      tone(speakerPin, i, 7);
      delay(7);
    }
    for (int i = 523; i > 440; i--)
    {
      tone(speakerPin, i, 10);
      delay(10);
    }
    
    counting = false;
  }
}
