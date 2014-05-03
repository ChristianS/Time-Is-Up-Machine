// PINs
const int redLedPin = 13;     // red LED pin
const int greenLedPin = 9;     // green LED pin
const int buttonPin = 4;      // button input pin
const int speakerPin = 3;    // speaker output pin

int buttonState = 0;

unsigned long startTime = 0;

boolean counting = false;

void setup()
{
  pinMode(redLedPin, OUTPUT);  
  pinMode(greenLedPin, OUTPUT);      
  pinMode(buttonPin, INPUT);
  pinMode(speakerPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH && counting == false)
  {
    counting = true;
    startTime = millis();
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    Serial.println("button");
  }
  
  unsigned long currentTime = millis();
 
  if ((currentTime - startTime) > 5000 && counting == true)
  {
    counting = false;
    digitalWrite(redLedPin, HIGH); 
    digitalWrite(greenLedPin, LOW);
    tone(speakerPin, 300, 100);
    Serial.println("beep");
  }
}
