int lastPotValue; //Save the last potentiometer value
int state = -1;

bool lastButton1State = HIGH;
bool lastButton2State = HIGH;
long lastButton1Time = 0;
long lastButton2Time = 0;
long button1DoubleClickTime = 500; // Double click interval
long button2DoubleClickTime = 500; // Double click interval

bool button1DoubleClicked = false;
bool button2DoubleClicked = false;

void setup() {

  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); //button 1
  pinMode(3, INPUT_PULLUP); //button 2
  pinMode(5, OUTPUT); // yellow light
  pinMode(6, OUTPUT); // green light
  pinMode(A0,INPUT);
  lastPotValue = analogRead(A0); // Initialize the potentiometer value
}

void loop() {
  
  int potValue = analogRead(A0);
  Serial.println(potValue);
  bool isTurningLeft = false;
  bool isTurningRight = false;
  if (potValue < lastPotValue) {
    isTurningLeft = true;
  } else if (potValue > lastPotValue) {
    isTurningRight = true;
  }
  lastPotValue = potValue; // Update the value of the potentiometer


  bool button1Pressed = false;
  if (digitalRead(2) == LOW) {
    button1Pressed = true;
  }
  bool button2Pressed = false;
  if (digitalRead(3) == LOW) {
    button2Pressed = true;
  }

  // Detect button 1 double click
  if (button1Pressed && !lastButton1State) {
    if (millis() - lastButton1Time < button1DoubleClickTime) {
      button1DoubleClicked = true;
      Serial.println("bt1 double click");
    }
    Serial.println("bt1  click");
    lastButton1Time = millis();
  }
  lastButton1State = button1Pressed;

  // Detect button 2 double click
  if (button2Pressed && !lastButton2State) {
    if (millis() - lastButton2Time < button2DoubleClickTime) {
      button2DoubleClicked = true;
      Serial.println("bt2 double click");
    }
    lastButton2Time = millis();
    Serial.println("bt2  click");
  }
  lastButton2State = button2Pressed;
  switch (state) {
    case -1:
      if(isTurningLeft){
        state=0;
      }
    case 0: // Step 1
      if (isTurningLeft && button1DoubleClicked) {
        state = 1;
      }
      if(potValue==0){
         digitalWrite(5,HIGH);
        delay(1000);
        digitalWrite(5,LOW);
        Serial.println("mission failed");
      }
      break;
    case 1: // Step 2
      if (potValue <= 10 && button2Pressed) {
        state = 2;
      }
      if(isTurningRight){
        digitalWrite(5,HIGH);
        delay(1000);
        digitalWrite(5,LOW);
        Serial.println("mission failed");
      }
      break;
    case 2: // Step 3
      if (isTurningRight && button2DoubleClicked) {
        state = 3;
      }
      if(isTurningLeft || potValue==4095){
        digitalWrite(5,HIGH);
        delay(1000);
        digitalWrite(5,LOW);
        Serial.println("mission failed");
      }
      break;
    case 3: // Step 4
      if (potValue >= 4090 && button1Pressed) {
        state = 4; // Success
        Serial.println("success");
      }
      if(isTurningLeft){
        digitalWrite(5,HIGH);
        delay(1000);
        digitalWrite(5,LOW);
        Serial.println("mission failed");
      }
      break;
    case 4: // Step 5
        digitalWrite(6,HIGH);
        delay(1000);
        digitalWrite(6,LOW);
        delay(1000);
        digitalWrite(6,HIGH);
        delay(1000);
        digitalWrite(6,LOW);
        delay(1000);
        digitalWrite(6,HIGH);
        delay(1000);
        digitalWrite(6,LOW);
    default:
      break;
  }
  delay(100);
}