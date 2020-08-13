#include <BTS7960.h>

int button_up = 2;
int button_down = 7;
int R_EN = 3;
int L_EN = 4;
int R_PWM = 5;
int L_PWM = 6;
int triggerPin = 8;
int echoPin = 9;

int upButtonCounter = 0;

int lastUpButtonState = HIGH;
int lastDownButtonState = HIGH;

int storedHeight = 80;

unsigned long lastUpDebounceTime = 0;
unsigned long lastDownDebounceTime = 0;
unsigned long moveDelay = 200;

int maxHeight = 93;
int minHeight = 62;

int addr = 0;

bool midPress = false;
bool longPress = false;

BTS7960 motorController(L_EN, R_EN, L_PWM, R_PWM);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {

  int up_is_pressed = digitalRead(button_up);
  int down_is_pressed = digitalRead(button_down);

  if ((up_is_pressed != lastUpButtonState)) {
    lastUpDebounceTime = millis();
    if (up_is_pressed == LOW) {
      upButtonCounter++;

      
      if (upButtonCounter == 5) {
        upButtonCounter = 0;
        storedHeight = (int)find_dist_cm(triggerPin, echoPin);
        Serial.println(storedHeight);

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

bool check_safe_distance(int triggerPin, int echoPin) {
float duration, distance;
  
  digitalWrite(triggerPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2; 
  delay(100);
  
  if (((int)distance < 83) && ((int)distance > 10)) {
    return true;
  } else {
    return false;
  }
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
