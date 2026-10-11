#include "SPI.h" 
#include "RF24.h" 
#include "nRF24L01.h" 

#define relayControlPin 7
#define relayActive LOW
#define relayInactive HIGH

#define CE_PIN 9
#define CSN_PIN 10
#define INTERVAL_MS_SIGNAL_LOST 3000
#define INTERVAL_MS_SIGNAL_RETRY 250
#define CHANNEL 108

unsigned long lastSignalMillis = 0;

struct Payload { 
   char data1[24];
  //String data2; 
}; 
Payload payload;
RF24 radio(CE_PIN, CSN_PIN);

void setupRadio() { 
  //RF24 _radio(CE_PIN, CSN_PIN); 
  const byte address[6] = "00001"; 
  //NRF24L01 buffer limit is 32 bytes (max struct size) 
  unsigned long lastSignalMillis = 0; 
  radio.begin(); 
	//Append ACK packet from the receiving radio back to the transmitting radio 
	radio.setAutoAck(false); //(true|false) 
  //Set the transmission datarate 
  radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS) 
  //Greater level = more consumption = longer distance 
  radio.setPALevel(RF24_PA_MIN); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX) 
  //Default value is the maximum 32 bytes1 
  radio.setPayloadSize(sizeof(payload)); 
  //Act as receiver 
  radio.openReadingPipe(0, address); 
  radio.startListening();
}

void lostConnection() 
{ 
	 //Serial.println("We have lost connection, preventing unwanted behavior"); 
	 delay(INTERVAL_MS_SIGNAL_RETRY); 
} 

void readData() {
  unsigned long currentMillis = millis(); 
  if (radio.available() > 0) { 
    radio.read(&payload, sizeof(payload)); 
    Serial.print(radio.getDynamicPayloadSize());
    Serial.println(radio.getPayloadSize());
    Serial.println("Received "); 
    Serial.print(sizeof(payload));
    Serial.println(" bytes of data");
    Serial.print("Data1:"); 
    Serial.println(payload.data1); 
    if (payload.data1[0]=='1') {
      Serial.print(toggleRelay("1",relayControlPin));
      delay(1000);
      Serial.print(toggleRelay("0",relayControlPin));
    }

    //Serial.print("Data2:"); 
    //Serial.println(payload.data2); 
    lastSignalMillis = currentMillis; 
  } 
  if (currentMillis - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST) { 
    lostConnection(); 
  }
}
void setupRelayController() {
  pinMode(relayControlPin, OUTPUT);
  digitalWrite(relayControlPin, relayInactive);
}

int toggleRelay(String command,int relayPin) {
    int pinState = digitalRead(relayPin);
    // Commands to control the pin
    if (command == "1") {
      digitalWrite(relayPin, relayActive);
      Serial.println("Activating relay");
      delay(500);
      pinState=digitalRead(relayPin);
    } 
    else if (command == "0") {
      digitalWrite(relayPin, relayInactive);
      Serial.println("Deactivating relay");
      delay(500);
      pinState=digitalRead(relayPin);
    }
    return pinState;
}

void setup() {
    Serial.begin(115200);
    setupRadio();
    setupRelayController();
}

void loop() {
  readData();
  //delay(INTERVAL_MS_SIGNAL_RETRY); 
  //delay(5000);
  //Serial.print(toggleRelay("1",relayControlPin));
  //delay(5000);
  //Serial.print(toggleRelay("0",relayControlPin));
}