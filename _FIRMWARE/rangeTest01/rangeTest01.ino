#define trigPin 13
#define echoPin 12

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  
  Serial.println(distance);
  
//  if (distance < 4) {  // This is where the LED On/Off happens
//    digitalWrite(led, HIGH); // When the Red condition is met, the Green LED should turn off
//    digitalWrite(led2, LOW);
//  }
//  else {
//    digitalWrite(led, LOW);
//    digitalWrite(led2, HIGH);
//  }
//  
//  if (distance >= 200 || distance <= 0) {
//    Serial.println("Out of range");
//  }
//  
//  else {
//    Serial.print(distance);
//    Serial.println(" cm");
//  }
  
  delay(500);

}