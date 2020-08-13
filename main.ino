#include <BTS7960.h>

int button_up = 2;
int button_down = 7;
int R_EN = 3;
int L_EN = 4;
int R_PWM = 5;
int L_PWM = 6;
int triggerPin = 8;
int echoPin = 9;

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

  if (up_is_pressed == LOW && find_dist_cm(triggerPin, echoPin) < 93) {

    motorController.Enable();
    motorController.TurnLeft(255);

    Serial.println((int)find_dist_cm(triggerPin, echoPin));

    
  } else if (down_is_pressed == LOW && find_dist_cm(triggerPin, echoPin) > 62) {

    motorController.Enable();
    motorController.TurnRight(255);

    Serial.println((int)find_dist_cm(triggerPin, echoPin));


    
  } else if (up_is_pressed == LOW && down_is_pressed == LOW){

    motorController.Disable();
  } else {
    motorController.Disable();
  }

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
