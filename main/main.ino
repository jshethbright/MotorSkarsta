#include <EEPROM.h>
#include <BTS7960.h>

// GPIO Definitions
int button_up = 2; 
int button_down = 7;
int R_EN = 3;
int L_EN = 4;
int R_PWM = 5;
int L_PWM = 6;
int triggerPin = 8;
int echoPin = 9;

// Counter for up button presses
int upButtonCounter = 0;

// Default state for up and down buttons due to using INPUT_PULLUP
int lastUpButtonState = HIGH;
int lastDownButtonState = HIGH;

// Establish stored heights and current height
unsigned char storedHeight = 0;
unsigned int currHeight = 0;

// Current EEPROM memory address to store height
int addr = 0;

// Debounce variables
unsigned long lastUpDebounceTime = 0;
unsigned long lastDownDebounceTime = 0;
unsigned long moveDelay = 200;

// Establish min and max heights
int maxHeight = 93;
int minHeight = 62;

// Init motor controller
BTS7960 motorController(L_EN, R_EN, L_PWM, R_PWM);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);

  storedHeight = EEPROM.read(addr);

  
  Serial.begin(9600);
}

void loop() {

  int up_is_pressed = digitalRead(button_up);
  int down_is_pressed = digitalRead(button_down);

  if (up_is_pressed != lastUpButtonState) {
    lastUpDebounceTime = millis();
    if (up_is_pressed == LOW) {
      upButtonCounter++;

      
      if (upButtonCounter == 5) {
        
        upButtonCounter = 0;
        currHeight = (int)find_dist_cm(triggerPin, echoPin);
        
        if (0 < currHeight < 255) {
          storedHeight = (char)currHeight;
          EEPROM.update(addr, storedHeight);
        }
      }
    }
  }
  


  if (up_is_pressed == LOW && down_is_pressed == LOW){
    moveToPosition(storedHeight);
    
    motorController.Disable();
  } else if (up_is_pressed == LOW && find_dist_cm(triggerPin, echoPin) < maxHeight) {

    motorController.Enable();
    motorController.TurnLeft(255);

    // Serial.println((int)find_dist_cm(triggerPin, echoPin));

    
  } else if (down_is_pressed == LOW && find_dist_cm(triggerPin, echoPin) > minHeight) {

    motorController.Enable();
    motorController.TurnRight(255);

    // Serial.println((int)find_dist_cm(triggerPin, echoPin));
    
  } else {
    
    motorController.Disable();
    
  }

  if ((millis() - lastUpDebounceTime) > moveDelay) {
    upButtonCounter = 0;
  }

  lastUpButtonState = up_is_pressed;

}



float find_dist_cm(int triggerPin, int echoPin) {
  float duration, distance;
  
  digitalWrite(triggerPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2; 
  delay(10);

  return distance;
  
}


void moveToPosition(int pos) {
  bool positionReached = false;

  while (positionReached == false) {
    int currentPos = (int)find_dist_cm(triggerPin, echoPin);
    
    if ((currentPos > maxHeight) || (currentPos < minHeight)) {
      positionReached = true;
      break;
    }

    if (currentPos > pos) {
      motorController.TurnRight(255);
    } else if (currentPos < pos) {
      motorController.TurnLeft(255);
    } else if (currentPos == pos) {
      positionReached = true;
      break;
    }
  }
}
