#include <IRremote.h>

/*
int IRpin = 3;
IRrecv irReceiver(IRpin);
IRsend irSender(SEND_PIN);
*/

const uint16_t receivePin = 4;
const uint16_t sendPin = 3;

IRrecv irReceiver(receivePin);
IRsend irSender(sendPin);

IRData msg;

uint16_t receiveIdx = 0;
uint16_t sendIdx = 0;

void setup()
{
  Serial.begin(9600);
  irReceiver.enableIRIn();
  irSender.begin(sendPin);
}

void loop() 
{
  if (irReceiver.decode()) {

    msg = irReceiver.decodedIRData;

    Serial.print("Received Data [");
    Serial.print(receiveIdx++);
    Serial.print("] : ");
    Serial.print(msg.protocol, HEX);
    Serial.print(", ");
    Serial.print(msg.command, HEX);
    Serial.println("");

    irReceiver.resume();
  }

  if (Serial.available() > 0) {
    while (Serial.available() > 0) 
    {
      uint8_t serial_msg = Serial.read();
    }
  
    irReceiver.stop();

    msg.command = 0xE6;

    if (irSender.write(&msg, 0)) {
      Serial.print("Sent Data [");
      Serial.print(sendIdx++);
      Serial.print("] : ");
      Serial.print(msg.protocol, HEX);
      Serial.print(", ");
      Serial.print(msg.command, HEX);
      Serial.println("");
    }




    
    else {
      Serial.print("Send Data is corrupt"); 
    }

    irReceiver.start();
  }
  
  delay(100);

}