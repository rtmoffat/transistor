#include "SPI.h" 
#include "RF24.h" 
#include "nRF24L01.h" 
#include <Base64.h>
#define CE_PIN 9 
#define CSN_PIN 10 
#define INTERVAL_MS_TRANSMISSION 1000 
#define CHANNEL 108
RF24 radio(CE_PIN, CSN_PIN); 
const byte address[6] = "00001"; 
//NRF24L01 buffer limit is 32 bytes (max struct size) 
struct payload { 
	 String data1; 
	 String data2; 
}; 
payload payload; 

char key[]="My Secret Key";

//Decrypt
String decrypt(char message[],char key[]) {
  //BASE64
  int msgLen=strlen(message);
  int decodedLength = Base64.decodedLength(message,msgLen);
  char decodedString[decodedLength + 1];
  Base64.decode(decodedString, message, msgLen);
  msgLen=strlen(decodedString);
  //XOR
  int keyLen = strlen(key);
  for (int i = 0; i < msgLen; i++) {
    decodedString[i] = decodedString[i] ^ key[i % keyLen];
  }
  return decodedString;
}

//Encrypt
String encrypt(char message[],char key[]) {
  //XOR
  int msgLen = strlen(message);
  int keyLen = strlen(key);
  for (int i = 0; i < msgLen; i++) {
    message[i] = message[i] ^ key[i % keyLen];
  }
  //BASE64
  msgLen=strlen(message);
  int encodedLength=Base64.encodedLength(msgLen);
  //Create a place to store the encoded string
  char encodedString[encodedLength + 1];
  Base64.encode(encodedString, message, msgLen);
  Serial.println("String encrypted.");
	return encodedString;
}

void setup() 
{ 
	 payload.data1 = "1234"; 
	 payload.data2 = encrypt("My other secret e",key); 
	 Serial.begin(115200); 
	 radio.begin(); 
	 //Set channel if desired. Otherwise, channel 76 is used. 2476Mhz
	 //radio.setChannel(CHANNEL);
	 //Append ACK packet from the receiving radio back to the transmitting radio 
	 radio.setAutoAck(false); //(true|false) 
	 //Set the transmission datarate 
	 radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS) 
	 //Greater level = more consumption = longer distance 
	 radio.setPALevel(RF24_PA_MAX); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX) 
	 //Default value is the maximum 32 bytes 
	 radio.setPayloadSize(sizeof(payload)); 
	 //Act as transmitter 
	 radio.openWritingPipe(address); 
	 radio.stopListening(); 
} 
void loop() 
{ 
	 Serial.print("Data1:"); 
	 Serial.println(payload.data1); 
	 Serial.print("Data2:"); 
	 Serial.println(payload.data2); 
	 Serial.print("Size of payload:");
	 Serial.println(sizeof(payload));
	 Serial.println("Sending message");
	 radio.write(&payload, sizeof(payload)); 
	 Serial.println("Message sent");
	 Serial.print("Data1:"); 
	 Serial.println(payload.data1); 
	 Serial.print("Data2:"); 
	 Serial.println(payload.data2); 
	 Serial.println("Sent"); 
	 delay(INTERVAL_MS_TRANSMISSION); 
} 
