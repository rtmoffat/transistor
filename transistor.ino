const int transistorPin = 2; // Pin connected to the transistor

void setup() {
  pinMode(transistorPin, OUTPUT);
  digitalWrite(transistorPin, LOW); // Start with transistor OFF
  Serial.begin(9600);              // Open USB serial communication
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    // Commands to control the pin
    if (command == '1') {
      digitalWrite(transistorPin, HIGH);
    } 
    else if (command == '0') {
      digitalWrite(transistorPin, LOW);
    }
    // Command to READ the pin
    else if (command == 'R') {
      int pinState = digitalRead(transistorPin); // Read physical state
      Serial.println(pinState);                 // Send the state back to Python
    }
  }
}
