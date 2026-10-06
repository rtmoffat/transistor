const int transistorPin = 12; // Pin connected to the transistor

void setup() {
  pinMode(transistorPin, OUTPUT);
  digitalWrite(transistorPin, HIGH); // Start with transistor OFF
  Serial.begin(9600);              // Open USB serial communication
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    // Commands to control the pin
    if (command == '1') {
      digitalWrite(transistorPin, LOW);
    } 
    else if (command == '0') {
      digitalWrite(transistorPin, HIGH);
    }
    // Command to READ the pin
    else if (command == 'R') {
      int pinState = digitalRead(transistorPin); // Read physical state
      Serial.println(pinState);                 // Send the state back to Python
    }
  }
}
