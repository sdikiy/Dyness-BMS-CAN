#include "STM32_CAN.h"

STM32_CAN Can( CAN1, ALT ); //Use PB8/9 pins for CAN1.
static CAN_message_t CAN_RX_msg;

HardwareSerial Serial2( PA3, PA2 );
HardwareSerial Serial3( PB11, PB10 );

uint16_t counter = 0;

void setup() {
  pinMode( LED_BUILTIN, OUTPUT );
  Serial.begin( 921600 );
  Serial2.begin( 921600 );
  Serial3.begin( 921600 );
  Can.begin();
  Can.enableSilentMode();
  Can.setBaudRate( 500000 ); //500KBPS
}

void loop() {
  if ( Can.read( CAN_RX_msg ) ) {
    counter++;
    digitalWrite( LED_BUILTIN, counter % 2 );
    Serial2.println( counter );
    Serial3.println( counter );

    if ( !CAN_RX_msg.flags.extended ) {
      Serial.print( "sID: " );
    } else {
      Serial.print( "eID: " );
    }
    Serial.print( CAN_RX_msg.id, HEX );

    if ( !CAN_RX_msg.flags.remote ) {
        Serial.print( " D: " );
      for ( int i = 0; i < CAN_RX_msg.len; i++ ) {
        Serial.print( "0x" ); 
        Serial.print( CAN_RX_msg.buf[i], HEX ); 
        if ( i != ( CAN_RX_msg.len - 1 ) )
          Serial.print( " " );
      }
      Serial.println();
    } else {
        Serial.println( " D: REMOTE REQUEST FRAME" );
    }
  }
}