/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * This is very simple teaching code to show you how to use the library.
 * If you are trying to decode your Infra-Red remote(s) for later replay,
 * use the IRrecvDumpV2.ino example code instead of this.
 * An IR detector/demodulator must be connected to the input kRecvPin.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/markszabo/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>


const uint16_t RECEIVE_PIN = 5;
const uint16_t SEND_PIN = 4;

IRrecv ir_receiver(RECEIVE_PIN);
IRsend ir_sender(SEND_PIN);  // Set the GPIO to be used to sending the message.

decode_results msg_received;
uint16_t* msg_send;
uint16_t msg_send_length = 0;


void setup() 
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(50);
  }
  
  ir_receiver.enableIRIn();
  ir_sender.begin();
}

void loop() 
{
  if (msg_send_length==0 && ir_receiver.decode(&msg_received)) 
  {
    msg_send = resultToRawArray(&msg_received);
    msg_send_length = getCorrectedRawLength(&msg_received);

    Serial.print("Received: ");
    serialPrintUint64(msg_received.value, HEX);
    Serial.println("");
    
    ir_receiver.resume();
  }

  if (msg_send_length>0 && Serial.available() > 0)
  {
    while (Serial.available() > 0) 
    {
      uint8_t serial_msg = Serial.read();
    }
    
    ir_sender.sendRaw(msg_send, msg_send_length, 38);

    Serial.print("Send: ");
    serialPrintUint64(msg_received.value, HEX);
    Serial.println("");
  }
  
  delay(100);
  yield();
}
