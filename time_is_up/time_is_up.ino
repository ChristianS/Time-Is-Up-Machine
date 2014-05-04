// PINs
const int y1LedPin = 12;
const int y2LedPin = 11;
const int y3LedPin = 10;
const int y4LedPin = 9;
const int y5LedPin = 8;
const int greenLedPin = 13;     // green LED pin
const int buttonPin = 6;      // button input pin
const int buttonPowerPin = 7;
const int speakerPin = 5;    // speaker output pin

// Button Debounce
int buttonState = 0;
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;

// Timers
unsigned long startTime = 0;
unsigned long alarmTimeInterval = 2000;

// Timer state
#define TS_Set5 5
#define TS_Set4 4
#define TS_Set3 3
#define TS_Set2 2
#define TS_Set1 1
#define TS_Rest 0

int timerState;

void setup()
{
  pinMode(y1LedPin, OUTPUT);  
  pinMode(y2LedPin, OUTPUT);  
  pinMode(y3LedPin, OUTPUT);  
  pinMode(y4LedPin, OUTPUT);  
  pinMode(y5LedPin, OUTPUT);  
  pinMode(greenLedPin, OUTPUT);      
  pinMode(buttonPin, INPUT);
  pinMode(buttonPowerPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  
  timerState = TS_Rest;
  digitalWrite(buttonPowerPin, HIGH);
  
  Serial.begin(9600);
}

// set all LEDs according to timer state
void refreshLEDs()
{
  switch (timerState)
  {
    case TS_Set5:
      digitalWrite(y1LedPin, HIGH);
      digitalWrite(y2LedPin, HIGH);
      digitalWrite(y3LedPin, HIGH);
      digitalWrite(y4LedPin, HIGH);
      digitalWrite(y5LedPin, HIGH);
      digitalWrite(greenLedPin, LOW);
      break;
    case TS_Set4:
      digitalWrite(y1LedPin, HIGH);
      digitalWrite(y2LedPin, HIGH);
      digitalWrite(y3LedPin, HIGH);
      digitalWrite(y4LedPin, HIGH);
      digitalWrite(y5LedPin, LOW);
      digitalWrite(greenLedPin, LOW);
      break;
    case TS_Set3:
      digitalWrite(y1LedPin, HIGH);
      digitalWrite(y2LedPin, HIGH);
      digitalWrite(y3LedPin, HIGH);
      digitalWrite(y4LedPin, LOW);
      digitalWrite(y5LedPin, LOW);
      digitalWrite(greenLedPin, LOW);
      break;
    case TS_Set2:
      digitalWrite(y1LedPin, HIGH);
      digitalWrite(y2LedPin, HIGH);
      digitalWrite(y3LedPin, LOW);
      digitalWrite(y4LedPin, LOW);
      digitalWrite(y5LedPin, LOW);
      digitalWrite(greenLedPin, LOW);
      break;
    case TS_Set1:
      digitalWrite(y1LedPin, HIGH);
      digitalWrite(y2LedPin, LOW);
      digitalWrite(y3LedPin, LOW);
      digitalWrite(y4LedPin, LOW);
      digitalWrite(y5LedPin, LOW);
      digitalWrite(greenLedPin, LOW);
      break;
    case TS_Rest:
      digitalWrite(y1LedPin, LOW);
      digitalWrite(y2LedPin, LOW);
      digitalWrite(y3LedPin, LOW);
      digitalWrite(y4LedPin, LOW);
      digitalWrite(y5LedPin, LOW);
      digitalWrite(greenLedPin, HIGH);
      break;
  }
}

void playAlarmSound()
{
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

      if (buttonState == HIGH) { // On button press ...
        Serial.println("button");
        
        timerState = (timerState % 5) + 1; //... add one time interval
        startTime = millis();
        
        refreshLEDs();
      }
    }
  }
  
  lastButtonState = reading;
  
  if (timerState > TS_Rest)
  {
    unsigned long currentTime = millis();
    unsigned long timePassed = currentTime - startTime;
    
    int oldState = timerState;
    
    // check if time interval is up
    if (timePassed > alarmTimeInterval)
    {
      timerState--;
      
      // restart timer for another interval
      startTime = millis();
    }
    
    // see if state changed
    if (timerState != oldState)
    {
      refreshLEDs();
      
      if (timerState == TS_Rest) // Alarm - State just changed to Rest!
      {
        Serial.println("alarm");
        
        // play melody
        playAlarmSound();
      }
    }
  }
}
